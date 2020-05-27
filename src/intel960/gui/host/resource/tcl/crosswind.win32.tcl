##############################################################################
# CrossWind.win32.tcl - Debugger GUI Tcl implementation file
#
# Copyright 1996 Wind River Systems, Inc.
#

#puts "sourcing crosswind.win32.tcl"
#
# Add context sensitive help
#
if [catch {source [wtxPath host resource tcl]Help.win32.tcl} result] {
	puts "Missing \"[wtxPath host resource tcl]Help.win32.tcl\".  Help will be disabled."
}

# 'downtclCommandTrace' - This boolean flag controls whether requests from
# the # GUI and the associated replies from GDB should be displayed on
# Tornado Console.
set downtclCommandTrace 0

# 'debugEventTrace' - This boolean flag controls whether asynchronous events
# from GDB to the GUI should be displayed on Tornado Console.
set debugEventTrace 0

# 'gdbCommandTrace' - This boolean flag controls whether GDB should print any
# and all commands, results, errors, and warnings out to its standard out/err.
set gdbCommandTrace 0

#
# Set up globals.
#

# A flag that controls whether protocol messages are printed to stdout
# when received for debugging purposes.
set debugProtocolMessages 1

# 'targetServer' - The name of target server *as known to the GUI* and that
# which the GUI wishes to connect to upon startup of the debug process.
if {! [info exists targetServer]} {
    set targetServer ""
}

# 'architecture' - The target architecture *as known to the debug engine*
# received upon a "architecture" event from the debug engine.
if {! [info exists architecture]} {
    set architecture ""
}

# 'srcDisplayMode' - The current source display mode.
if {! [info exists srcDisplayMode]} {
    set srcDisplayMode "C"
}

# 'debugEngineBusy' - A flag indicating that the debug engine (gdb) is busy.
if {! [info exists debugEngineBusy]} {
    global debugEngineBusy
    set debugEngineBusy 0
}

# 'currentContext' - The currently known context.  If the debugger is running
# then this is blank ("").
if {! [info exists currentContext]} {
    set currentContext ""
}

# 'bpArray()' is a global array indexed by a breakpoint number.  Each element of
# the array is a list of the form:
#	<bpnumber> <enabled> <disposition> <filename> <lno> <addr>
if {! [info exists bpArray()]} {
    set bpArray() ""
}

# 'fileLineInfo' - A cached array indexed by "source file names" where each is a list.
# The list is of the form "lineNumber startAddress endAddress".  Only lines with actual
# machine code are kept.
if {! [info exists fileLineInfo()]} {
    set fileLineInfo() ""
}

# 'fileLineInfo' - A cached array indexed by "source file names" where each is a list.
# The list is of the form "firstLine lastLineNumber startAddress endAddress".  The first
# and last lines are lines containing real machine code.  The addresses represent the
# files range in memory.
if {! [info exists fileAddressRangeInfo()]} {
    set fileAddressRangeInfo() ""
}

# 'refreshMemoryOnStop' control if the memory window should be updated each
# and every time the debugger stops or refresh on demand
if {! [info exists refreshMemoryOnStop]} {
    set refreshMemoryOnStop "0"
}


# Should show this some where such that the user knows
# what state the target is in.  Maybe Frame title
# debugStatusSet "No Target"

# Global variables
set needStack 0
set currentTarget "???"
set pendingDisplays ""
set readyCommandQueue ""
set previousLocalsStruct ""
set pendingDisplayReqs ""


# 		      GDB/GUI Protocol Reference
#
#
# bp <number> <enabled> <disposition> <filename> <lno> <addr>
#
# 	Breakpoint <number> has either been set or has changed state.
# 	<Enabled> is nonzero if the breakpoint is "enabled" in the
# 	GDB sense.  <Disposition> is nonzero if the breakpoint is
# 	"temporary", that is, will be deleted or disabled the next
# 	time it is hit.  <Filename> is the name of the source file
# 	where the breakpoint lies, if known (it will not be known
# 	if the breakpoint lies in an address range of a file not
# 	compiled with -g:  In this case the filename is reported as
# 	`nil'); <lno> is the line number of the breakpoint, which
# 	is only significant if <filename> is not `nil'.  <Addr> is
# 	the machine address of the breakpoint on the target, and
# 	this is always valid, whether or not a source address is given.
#
# 	This message is sent when a breakpoint is placed, and also
# 	when it changes state (for example, the use of the
# 	enable or disable GDB commands would cause a new `bp'
# 	message with a changed <enabled> field.
#
# bpdel <number>
#
# 	Breakpoint <number> has been deleted.  The <number> will
# 	never be used again.
#
# forget <filename>
#
# 	An object module containing code from the named source file
# 	has been reloaded.  If the GUI has cached information
# 	about the source file, it may now be out of date.
#
# dispsetup <number> <structure>
#
# 	The user has requested a new auto display.  <Number> is the
# 	GDB number of the auto-display; <structure> is the structure
# 	of the data item being displayed in Tcl format.
#
# 	In CrossWind, this is used to create a Hierarchy window with
# 	the given structure string.  See the man page for the Tcl
# 	routine `hierarchyCreate' for more information.
#
# 	There is a secret GDB command, `pptype', that prints the type
# 	of an expression in Tcl format.
#
# dispreq <number>
#
# 	Execution has stopped at a place where auto-display <number>
# 	is in scope.  In a traditional, non-GUI GDB, GDB would now
# 	print the value of the auto-display expression.  When the
# 	GUI gets this message, it is expected to query GDB with
# 	downtcl to get the value of the expression and put it in
# 	the window that was prepared when `dispsetup' was received.
# 	There is a secret gdb command, `ppval', that prints the value
# 	of an expression in Tcl format.
#
# dispdel <number>
#
# 	The user has discarded the numbered auto-display.  The number
# 	will never be used again.
#
# newtarget
#
# 	A new target has been connected to; all cached information
# 	should now be considered invalid (because we might even have
# 	connected to a different machine!)
#
# 	This message is probably obsolete with the "status target"
# 	message below, but is still used by CrossWind.win32.tcl at the
#	moment.
#
# status [`run' | `stop' | `target' <t0> <t1>...]
#
# 	`Status run' means the inferior has been resumed.  GDB is probably
# 	busy until `status stop' is received.
#
# 	`Status stop' means the inferior has stopped, either by a breakpoint,
# 	user interruption, or a normal exit, or some unforeseen happening.
#
# 	`Status target' reports the current contents of GDB's target stack,
# 	starting with the top level.  Each level on the target stack
# 	contributes one word to the message.  For example, when attaching
# 	to a WTX task, the message would be
#
# 		status target wtxtask wtx None
#
# 	Evidently there's always a dummy target named "None" at the
# 	bottom of the stack.  `wtx' represents the connection to the
# 	target server, and `wtxtask' indicates that we are communicating
# 	with a particular task.  The target stack is reported from
# 	top to bottom.  This status message is generated every time
# 	anything is pushed or popped from the target stack.
#
# context <level> [<file> | `nil'] <lno> <addr>
#
# 	The debugger is "looking at" the given source or machine context.
# 	This could be because a breakpoint was hit, or the user typed
# 	any of several GDB commands that look at source (frame, up, down...).
# 	If the source context is known, <file> and <lno> are the file
# 	name and line number to display.  <addr> is always valid: the
# 	machine address.  If <addr> is not in a module compiled with -g,
# 	then the filename is `nil' and <lno> is indeterminate.  <Level>
# 	represents the level in the call stack.  `0' is the innermost
# 	level (current PC); `1' is the caller of that routine; etc.
#
# list <file> <lno>
# list `address' <addr>
#
# 	The user has explicitly asked to look at either the given
# 	file and line number, or the given machine address.  It's
# 	similar to context, but provides less information.  It is
# 	used when the user issues the "list" command, which bypasses
# 	GDB's symbol table smarts; this is because GDB allows the
# 	listing of a file even before any symbols have been read.


##############################################################################
#
# gdbMotionCommand - run GDB motion command with popup result
#
# Runs the command in args, and if the result is a string starting with the
# word "Program", as in "Program received signal..." or "Program completed..."
# this text is popped up in a notice box.
#
proc gdbMotionCommand {args} {
	if [catch {downtcl gdb $args} result] {
		# There was an error.  Pop it up.
		set result [string trim $result "\n"]
			if {$result != ""} {
				# noticePost error $result
				messageBox error $result
			}
	} {
		if [regexp "\n*(Program.*)" $result whole message] {
			set message [string trim $message "\n"]
			# noticePost info $message
			setStatusMessage $message
		}
	}
}

##############################################################################
#
# stepButton - callback attached to toolbar "step" button
#
# GDB is asked to "step" or "stepi", depending on the current value of
# srcDisplayMode.
#
# SYNOPSIS:
#   stepButton
#
# PARAMETERS:
#   all are ingored.
#
# RETURNS:
#   NONE
#
# ERRORS:
#   NONE
#
proc stepButton {args} {
    global srcDisplayMode
    if {$srcDisplayMode == "C"} {
		downtcl -noresult gdb step
    } {
		downtcl -noresult gdb stepi
    }
}


##############################################################################
#
# nextButton - callback attached to toolbar "next" button
#
# GDB is asked to "step" or "stepi", depending on the current value of
# srcDisplayMode.
#
# SYNOPSIS:
#   nextButton
#
# PARAMETERS:
#   all are ingored.
#
# RETURNS:
#   NONE
#
# ERRORS:
#   NONE
#
proc nextButton {args} {
    global srcDisplayMode
    if {$srcDisplayMode == "C"} {
		downtcl -noresult gdb next
    } {
		downtcl -noresult gdb nexti
    }
}


proc continueButton {args} {
	downtcl -noresult gdb continue
}


proc finishButton {args} {
	downtcl -noresult gdb finish
}



proc selectInspectWordOk {} {
	global g_inspectSelection
	set g_inspectSelection [ lindex [controlValuesGet \
		inspectWordSelectDlg.inspectWord]	 0]
	windowClose inspectWordSelectDlg
}

proc selectInspectWordCancel {} {
	global g_inspectSelection
	set g_inspectSelection ""
	windowClose inspectWordSelectDlg
}

proc inspectDialogShow {} {
	dialogCreate -name inspectWordSelectDlg -title "Inspect" \
	  -w 169 -h 59 -controls {
		{text -name inspectWord -x 7 -y 16 -w 100 -h 14}
		{button  -default -callback selectInspectWordOk -title "OK"
			-name okButt -x 114 -y 7 -w 50 -h 14}
		{button -callback selectInspectWordCancel -title "Cancel" -name
			cancelButt -x 114 -y 25 -w 50 -h 14}
		{button  -title "&Help" -helpbutton
			-x 114 -y 43 -w 50 -h 14}
		{label -title "Symbol:" -name lbl1 -x 7 -y 6 -w 77 -h 10}
	}
}

proc inspectSelection {sel} {
	global g_inspectSelection

	if {$sel == ""} {
		inspectDialogShow
		if {"" != $g_inspectSelection} {
			downtcl -noresult gdb disp/W $g_inspectSelection
		}
	} else {
		downtcl -noresult gdb disp/W $sel
	}
}

# inspectStarSelection proc is not being used in Win32
proc inspectStarSelection {args} {
	set sel [lindex $args 0]
	if {$sel != ""} {
		downtcl -noresult gdb disp/W * $sel
    }
}


#||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||
#
# DEBUG ENGINE PROTOCOL HANDLING
#

#
# protocolMessageHandler - main dispatch center for debug-engine protocol
# messages.
#

proc protocolMessageHandler {args} {
	if {[llength [lindex $args 0]] > 1} {
		set args [lindex $args 0]
	}
    set messageHandler [lindex $args 0]Handler

    if {[info procs $messageHandler] == ""} {
		puts stdout [format "unhandled protocol message: %s" $args]
    } {
		$messageHandler [lrange $args 1 end]
    }
}


proc errorHandler {msg} {
	messageBox -ok -stop [lindex $msg 0]
}


proc warningHandler {msg} {
	messageBox -ok -excl [lindex $msg 0]
}


proc queryHandler {msg} {
	writeDebugInput [string range [messageBox -yesno -quest [lindex $msg 0]] 0 0]
}


set oldBtFrameNum 0
set newBtFrameNum 0

#
# contextHandler - Respond to the "context" protocol message, a request
# by the debug engine to display a source context and annotate it.  The
# first argument is the frame number (where zero represents the innermost
# frame, and the numbers ascend from there).  The following three arguments
# are the source file, line number, and machine address.  The source file
# may be the string "nil" if the source file is not known to the debug
# engine (and in this case the line number is ignored).
#
proc contextHandler {msg} {
	# [lindex args 0] is frame
	# [lindex args 1] is file
	# [lindex args 2] is lno
	# [lindex args 3] is addr

	global currentContext
	global oldBtFrameNum
	global newBtFrameNum
    global targetServer
    global architecture
	global readyCommandQueue

    #puts "in context handler $msg."
	set currentContext $msg
	set currentFileName \{[lrange $msg 1 1]\}
	regsub -all {[\\]} "$currentFileName" "/" currentFileName
	#puts "currentFileName: $currentFileName"
	if {[hierarchyExists "Back Trace"] == 1} {
		hierarchyHighlightUnset "Back Trace" "{Call Stack} $oldBtFrameNum"
	}

	lappend readyCommandQueue "updateContext \
                                   [lindex $msg 0] [lindex $msg 2] \
                                   $currentFileName [lindex $msg 3]"
	#
	#updateContext [lindex $msg 0] [lindex $msg 2] \
	#	  	  [lrange $msg 1 1] [lindex $msg 3]

	if {[lindex $msg 0] > 0} {
		# If the frame number is >0, we mark it as an ancestor.
		# Sometimes GDB simply writes -1 for the frame number--
		# we treat that as the "here" frame.
		set markType "ancestor"
		set frame [lindex $msg 0]
	} {
		set frame 0
	}
    set oldBtFrameNum $newBtFrameNum
    set newBtFrameNum $frame
	if {[hierarchyExists "Back Trace"] == 1} {
		hierarchyHighlightUnset "Back Trace" "{Call Stack} $oldBtFrameNum"
                hierarchyHighlightSet   "Back Trace" "{Call Stack} $newBtFrameNum"
                #lappend readyCommandQueue "hierarchyHighlightSet \"Back Trace\" \"{Call Stack} $newBtFrameNum\""
	}
}


proc getCurrentContext {} {
	global currentContext

	return $currentContext
}


proc isCurrentSourceLineContext {filename lineNumber} {
	global currentContext

	set currentContextFilePath [lindex currentContext 1]
	set currentContextLineNumber [lindex currentContext 2]

	if {$currentContextFilePath == "nul"} {
		return "0"
	} elseif {$currentContextLineNumber == $lineNumber &&
			  [string last [file tail $currentContextFilePath] $filename] != -1} {
		return "1"
	} {
		return "0"
	}
}


#
# listHandler - repsond to the "list" protocol message, a request from the
# debug engine to display a certan source or machine context.  If the
# first argument is "address", the second argument will contain the address
# to display in machine format.  Otherwise the two arguments represent the
# source file and line number.
#
proc listHandler {msg} {
	global currentContext

	# Since we don't handle addresses
	if {[lindex $msg 0] != "address"} {
		set currentContextFilePath "[lrange $currentContext 1 1]"
		set fileName "[lrange $msg 0 0]"

		set newLine 1
		if {$currentContextFilePath == $fileName} {
			set listSize [downtcl gdb show listsize]
			set listSize [lindex $listSize [expr [llength $listSize] - 1]]
			set currentContextLineNumber [lindex currentContext 2]
			set gdbLine [lindex $msg 1]
			if {$gdbLine != "1"} {
				set newLine [expr $gdbLine + [expr $listSize / 2] - 1]
			}
		} {
			set listSize [downtcl gdb show listsize]
			set listSize [lindex $listSize [expr [llength $listSize] - 1]]
			set gdbLine [lindex $msg 1]
			if {$gdbLine != "1"} {
				set newLine [expr $gdbLine + [expr $listSize / 2] - 1]
			}
		}
		openFile $fileName $newLine
	}

	#if {[lindex $msg 0] == "address"} {
	#	sourceContextDisplay nil 0 [lindex $msg 1]
	#} {
	#	sourceContextDisplay [lindex $msg 0] [expr [lindex $msg 1] - 1]
	#}
}


#
# architectureHandler - respond to the "architecture" protocol message,
# which announces the architecture of the debugged target.  We expect this
# message early, and record the result.  It is used to dispatch architecture-
# specific procedures in this file.
#
proc architectureHandler {msg} {
	global gdbCommandTrace
    global architecture
    global targetServer

    set architecture $msg
    #puts stdout "msg is $msg"

	if {$gdbCommandTrace} {
		downtcl -noresult gdbCommandTrace ON
	} {
		downtcl -noresult gdbCommandTrace OFF
	}

	# Inform the debugger of any source search paths previously enterred
	# by the user in the "Source Search Path" dialog.
	setInitialSourceSearchPath

	# Now attach to the target specified when the GUI debugger was instantiated.
	downtcl -noresult gdb target wtx $targetServer
}


proc isDebuggerBusy {} {
    global debugEngineBusy
	return $debugEngineBusy
}


proc stopDebugging {} {
	downtcl gdb quit
}


#
# statusHanlder - respond to the status message.  This message reports
# changes in the execution status of the target program.  The argument
# describes what the new status is (ex. "stop" for when the program has
# stopped at a breakpoint).  We signal text windows so that they may
# refresh their context and refresh the backtrace window also after
# fetching an up-to-date list of stack frame tags from the debug engine.
#
proc statusHandler {msg} {
# proc statusHandler {status args}
	set status [lindex $msg 0]
	set args [lrange $msg 1 end]
   	global debugEngineBusy
    global architecture
    global currentTarget
    global needStack
    global displayExpression
    global pendingDisplays
    global readyCommandQueue
	global refreshMemoryOnStop

	global oldBtFrameNum
	global newBtFrameNum

	if {$status == "stop"} {
		set needStack 1
	    set debugEngineBusy 0
	} {
		if {$status == "ready"} {

			# If there are any commands to execute in the readyCommandQueue,
			# execute them now.

			set rcq $readyCommandQueue
			set readyCommandQueue ""
			foreach command $rcq {
				eval $command
			}

			# time to update the backtrace window.
			set needBt 0

			if {$needStack} {
				set needStack 0
				set needBt 1
				# reconfigure backtrace window
				if {$needBt} {
					set needBt 0
					if {[hierarchyExists "Back Trace"] == 1} {
						fetchBacktrace
					}
				}

				if {[hierarchyExists "Registers: i960"] == 1} {
                                        setStatusMessage "Refreshing Registers window..."
					hierarchySetValues "Registers: i960" \
									[RegisterVector]
					setStatusMessage "Registers window refreshed"
				}
				if {$refreshMemoryOnStop == 1} {
					if {[hierarchyExists Memory] == 1} {
						setStatusMessage "Refreshing Memory window..."
						windowRefresh Memory
						setStatusMessage "Memory window refreshed"
					}
				}
                                if {[hierarchyExists "Locals"] == 1} {
					setStatusMessage "Refreshing Locals window..."
					fetchCurrentFrameLocals
					setStatusMessage "Locals window refreshed"
				}
			}

			# If there are any pending auto-display updates, process them
			# now.
			refreshPendingDispReqs
			refreshPendingDisplay

			return

		} elseif {$status == "run"} {
			global currentContext
			if {$currentContext != ""} {
				updateContext -delete \
							  [lindex $currentContext 0] [lindex $currentContext 2] \
							  [lrange $currentContext 1 1] [lindex $currentContext 3]
				set currentContext ""
			}

		    set debugEngineBusy 1
		}
	}

    # case $status {
	# stop		{setStatusMessage "$currentTarget/Stopped"}
	# run		{setStatusMessage "$currentTarget/Running"}
	# target		{setTargetStatus $args}
	# *			{setStatusMessage "$msg"}
    # }

	if {$status == "target"} {
		setTargetStatus $args
	}
}


#
# setTargetStatus - decide what to put in the debug status window based
# on a new configuration of the target stack.
#
proc setTargetStatus {stack} {
	global currentTarget
	global currentContext
    global readyCommandQueue

	case [lindex $stack 0] {
	    mon960 {
		global targetConnected
		global readyCommandQueue

		set currentTarget "mon960"
		setStatusMessage -normal "Connected to MON960 debug monitor."
		#puts "received a mon960 target message."
		if {!$targetConnected} {
		    set targetConnected 1
		    controlDestroy Gdb960Buttons.targetConnect
		    lappend readyCommandQueue targetInfo
		}
	    }
		exec		{set currentTarget "executable"}
		child		{set currentTarget "process"}
		wtx			{set currentTarget "WTX server"}
		wtxsystem	{set currentTarget "WTX System"}
		wtxtask		{set currentTarget "WTX Task"}
		*			{set currentTarget "[lindex $stack 0]"}
	}

	# generally a change in the target stack means the program isn't
	# "stopped" anywhere anymore--it has gone away.  Remove the marks
	# that suggest the program is executing.
	if {[lindex $stack 1] == "None" && $currentContext != "" } {
		lappend readyCommandQueue "updateContext -delete \
					  		[lindex $currentContext 0] [lindex $currentContext 2] \
					  		[lrange $currentContext 1 1] [lindex $currentContext 3]"
		set currentContext ""
	}
}


#
# bpHandler - respond to the "bp" protocol message, which indicates that
# a breakpoint has been set.  The arguments of the message are the bp number,
# the enable status (integer boolean), the temporary status (integer boolean),
# the source file (or "nil" if it is not known), the line of the source file
# (ignored if the file is "nil"), and the machine address where the breakpoint
# is set.
#
# if "lindex          lindex
#     args 1" is   &  args 2" is  then it is a
#    ----------       ---------   ------------
#    0                1           disabled temporary breakpoint
#    0                0           disabled regular breakpoint
#    1                1           enabled temporary breakpoint
#    1                0           enabled regular breakpoint
#
proc bpHandler {args} {
    global readyCommandQueue
	set args [lindex $args 0]
	# [lindex $args 0] is $number
	# [lindex $args 1] is $enable
	# [lindex $args 2] is $disposition
	# [lindex $args 3] is $filename
	# [lindex $args 4] is $lno
	# [lindex $args 5] is $addr

	global bpArray
	set operation "-set"
	foreach bpNumber [array names bpArray] {
		if {$bpNumber == [lindex $args 0]} {
			set operation "-update"
			break
		}
	}

	if [string match {*-set*} $operation] {
		set bpArray([lindex $args 0]) $args
		# if {[lindex $args 2] == "1" && [lindex $args 1] == "1"} {
		#	lappend readyCommandQueue "downtcl gdb enable delete [lindex $args 0]"
		# }
	}

	updateBreakpointList $operation \
						 [lindex $args 0] [lindex $args 1] [lindex $args 2] \
						 [lrange $args 3 3] [lindex $args 4] [lindex $args 5]
}


#
# 'toggleBreakpoint'
#
proc toggleBreakpoint {sourceFile lineNumber address type} {
	global bpArray
	# Search for a breakpoint that may exist at this location.
	foreach bpNumber [array names bpArray] {
		set filename [lindex $bpArray($bpNumber) 3]
		set line [lindex $bpArray($bpNumber) 4]
		set addr [lindex $bpArray($bpNumber) 5]
		if {(($addr == $address) ||
			 (($filename != "nil") &&
			  ($filename == $sourceFile) &&
			  ($line == $lineNumber)))} {
			# Found one; delete it and return.
			downtcl -noresult gdb del $bpNumber
			return
		}
	}

	# A breakpoint does not exist at this location, create one.
	if {$sourceFile == "nil"} {
		downtcl -noresult gdb $type * $address
	} {
		downtcl -noresult gdb $type $sourceFile:$lineNumber
	}
}


#
# bpdelHandler - respond to the "bpdel" protocol message, which indicates
# that a breakpoint has been deleted.  We delete the breakpoint mark from
# the mark list and redisplay all marks (an inefficient means of erasing
# the mark corresponding to the deleted breakpoint).  The argument of the
# message is the number of the breakpoint that has been deleted.
#
proc bpdelHandler {msg} {
	global bpArray

	set bpInfo $bpArray([lindex $msg 0])
	updateBreakpointList -delete \
						 [lindex $bpInfo 0] [lindex $bpInfo 1] [lindex $bpInfo 2] \
						 [lrange $bpInfo 3 3] [lindex $bpInfo 4] [lindex $bpInfo 5]
	unset bpArray([lindex $msg 0])
}


#
# uptclHandler - respond to the "uptcl" protocol message.  This message
# is a catch-all means for the debug engine to request Tcl evaluations
# to take place in the context of the GUI's Tcl interpreter.  We simply
# evaluate the rest of the message (perhaps removing the outermost layer
# of quoting).
#
proc uptclHandler {args} {
	if {[llength $args] == 1} {
		# the string has been "braced up" to prevent evaluations
		# in the lower Tcl interpreter.  We must remove these braces
		# to allow evaluation here.  Or maybe it's just a one-word
		# command.  Either way, this will work to evaluate it.
		eval [lindex $args 0]
	} {
		eval $args
	}
}


#
# forgetHandler - respond to the "forget" protocol message.  This message
# means that a source module should be considered out of date with respect
# to the machine code loaded on the target and so the source code should
# be flushed from the display cache.
#
proc forgetHandler {file} {
    global readyCommandQueue
	global fileAddressRangeInfo
	global fileLineInfo

	if {[info exists fileAddressRangeInfo($file)]} {
		set fileAddressRangeInfo($file) ""
	}
	if {[info exists fileLineInfo($file)]} {
		set fileLineInfo($file) ""
	}

	forgetFileInfo "[lrange $file 0 0]"
}

#
# newtargetHandler - respond to the "newtarget" protocol message.  This
# message means that a new target has been attached to (or perhaps an
# old target has been reattached to.)  In any case we must assume that
# all cached information is invalid.
#
# the argument "msg" is just a place holder so that errors do not get
# generated when this proc is called from protocolMessageHandler
proc newtargetHandler {msg} {
	global fileAddressRangeInfo
	global fileLineInfo

	set fileAddressRangeInfo() ""
	set fileLineInfo() ""

	# Attempt to restore any persistent breakpoints.
	# ...
}


#
# 'displayExpression'
#
global displayExpression ()


#
# dispsetupHandler - set up for a newly displayed expression.  We get
# a number and an expression to display.  Record this in a global array,
# so we can look it up later.  Then fetch the type in protocol format,
# and create a dialog with that type's structure in it.  We'll display
# it when the debug engine tells us its value might have changed.
#
proc dispsetupHandler {msg} {
	set dispNum [lindex $msg 0]
	set args [lrange $msg 1 end]
	global displayExpression

	beginWaitCursor

	# crack open any extraneous braces Tcl might have wrapped around the
	# argument to protect them, as might happen if the object being displayed
	# contained an array reference (e.g., foo[6].bar would come down to
	# us as {foo[6].bar}).  By referencing the 0'th element of every
	# 'list', we will rip off any of this extraneous quoting.

	set dispExpr ""
	foreach a $args {
		append dispExpr [lindex $a 0] " "
	}

	# set displayExpression($dispNum) $dispExpr
	addToDispSetupQ $msg
	# eval {}

	endWaitCursor
}

proc dispsetupHandler_Old {msg} {
	set dispNum [lindex $msg 0]
	set args [lrange $msg 1 end]
	global displayExpression
	global pendingDisplays

	beginWaitCursor

	# crack open any extraneous braces Tcl might have wrapped around the
	# argument to protect them, as might happen if the object being displayed
	# contained an array reference (e.g., foo[6].bar would come down to
	# us as {foo[6].bar}).  By referencing the 0'th element of every
	# 'list', we will rip off any of this extraneous quoting.

	set dispExpr ""
	foreach a $args {
		append dispExpr [lindex $a 0] " "
	}

	set displayExpression($dispNum) $dispExpr

	if [catch {downtcl gdb pptype $dispExpr} exType] {
		# error -- something is faulty about the expression,
		# or GDB's protocol extension can't understand its type
		unset displayExpression($dispNum)
	} {
		hierarchyCreate -destroy displayDestroyed -change -special \
						"Inspect: [displayName $dispNum]" \
						"displayTraverse $dispNum"

		hierarchySetColor "Inspect: [displayName $dispNum]" \
						[getColorItemIdFor Inspect] \
						[getChangeColorItemIdFor Inspect]
		hierarchySetIcon "Inspect: [displayName $dispNum]" \
						[getIconIdFor Inspect]
		hierarchySetStructure "Inspect: [displayName $dispNum]" \
			[collectDataMembers $exType $dispExpr "" 0]

		# now would be a good time to display it!  Make GDB give us
		# the ready signal by evaluating a null expression.

		addToPendingDisplays $dispNum
		# eval {}
		# refreshPendingDisplay
	}
	endWaitCursor
}

#
# 'addToDispSetupQ'
#
proc addToDispSetupQ {msg} {
	global pendingDisplayReqs

	if {[lsearch $pendingDisplayReqs $msg] == -1} {
		lappend pendingDisplayReqs $msg
	}
}

proc refreshPendingDispReqs {} {
	global displayExpression
	global pendingDisplays
	global pendingDisplayReqs

	beginWaitCursor
	set pr $pendingDisplayReqs
	set pendingDisplayReqs ""
	foreach msg $pr {

		set dispNum [lindex $msg 0]
		set args [lrange $msg 1 end]

		set dispExpr ""
		foreach a $args {
			append dispExpr [lindex $a 0] " "
		}

		set displayExpression($dispNum) $dispExpr

		if [catch {downtcl gdb pptype $dispExpr} exType] {
			# error -- something is faulty about the expression,
			# or GDB's protocol extension can't understand its type
			unset displayExpression($dispNum)
		} {
			hierarchyCreate -destroy displayDestroyed -change -special \
							"Inspect: [displayName $dispNum]" \
							"displayTraverse $dispNum"

			hierarchySetColor "Inspect: [displayName $dispNum]" \
							[getColorItemIdFor Inspect] \
							[getChangeColorItemIdFor Inspect]
			hierarchySetIcon "Inspect: [displayName $dispNum]" \
							[getIconIdFor Inspect]
			hierarchySetStructure "Inspect: [displayName $dispNum]" \
				[collectDataMembers $exType $dispExpr "" 0]

			# now would be a good time to display it!  Make GDB give us
			# the ready signal by evaluating a null expression.

			addToPendingDisplays $dispNum
		}
	}
	endWaitCursor
}


#
# 'refreshPendingDisplay'
#
proc refreshPendingDisplay {} {
	global displayExpression
	global pendingDisplays

	beginWaitCursor
	set pd $pendingDisplays
	set pendingDisplays ""
	foreach dispNum $pd {
		if [info exists displayExpression($dispNum)] {
		    if [catch \
			    {downtcl gdb ppval $displayExpression($dispNum)} exVal] {
				# error--maybe the expression can't be displayed or is
				# defective somehow.
		    } {
				set dname [displayName $dispNum]
				if [hierarchyExists "Inspect: $dname"] {
				    hierarchySetValues "Inspect: $dname" $exVal
				    # if {![hierarchyIsDisplayed $dname]} {
					#	hierarchyPost $dname
					# }
				}
			}
	    }
	}
	endWaitCursor
}


proc addToPendingDisplays {dispNum} {
	dispreqHandler $dispNum
}


proc dispreqHandler {dispNum} {
	global pendingDisplays

	if {[lsearch $pendingDisplays $dispNum] == -1} {
		lappend pendingDisplays $dispNum
	}
}


proc dispdelHandler {dispNum} {
	set name [displayName $dispNum]
	if {$name != ""} {
		hierarchyDestroy "Inspect: $name"
	}
}


proc displayDestroyed {args} {
	set dispName [lindex $args 1]
	scan $dispName "(%d)" dispNum
	downtcl -noresult gdb undisplay $dispNum
}


proc displayName {dispNum} {
	global displayExpression
	if {[info exists displayExpression($dispNum)]} {
		return "($dispNum) $displayExpression($dispNum)"
	}
	return ""
}


proc displayTraverse {args} {
	global displayExpression
	set dNum [lindex $args 0]
	set dStack [lrange $args 1 end]
	set baseExp "$displayExpression($dNum)"

	# throw away the last element of dStack and reverse the order of the rest
	# and separate them with "."s.  That's the path to the traversed element.

    set dStack [lrange $dStack 0 [expr [llength $dStack]-2]]
	set path ""

	foreach d $dStack {
		if {[lindex $d 0] == "struct" || [lindex $d 0] == "union"} {
			set element [lindex $d 1]
		} {
			set element [lindex $d 0]
		}
		set element [lindex $element [expr [llength $element] - 1]]

		if [string match {*\[\]*} $element] {
			set element [string trim $element \[\]]
		}
		if {[regexp {^[0-9]+$} $element]} {
			# it's an array index.
			set path "\[$element\]$path"
		} {
			set path ".$element$path"
		}
	}
	if {$path != ""} {
		downtcl -noresult gdb disp/W *($baseExp)$path
	}
}


proc collectDataMembers {item name prefix level} {
	if {[llength $item] == 1} {
		set item [lindex $item 0]
	}

	if {[lindex $item 0] == "struct" || [lindex $item 0] == "union"} {
		set fieldlist [lindex $item 2]
		set retstring ""
		if {$name != ""} {
			set retstring "\{\{[lindex $item 0] $name\} "
		}
		set retstring "$retstring \{"

		while {[llength $fieldlist] >= 2} {
			set substring [collectDataMembers [lindex $fieldlist 0]  \
			[lindex $fieldlist 1] \
			  "    $prefix" [expr $level+1]]
			set retstring "$retstring$substring"
			set fieldlist [lrange $fieldlist 2 end]
		}
		if {$name != ""} {
			set retstring "$retstring \} "
		}

		return "$retstring \} "
	}

	if {[lindex $item 0] == "\[\]"} {
		# It's an array.
		set basetype [lindex [lindex $item 2] 0]

		if [isAggregate $basetype] {
			# It's a pointer to a compound object.
			set substring [collectDataMembers [lindex $item 2] \
			"" "    $prefix" [expr $level+1]]

			if {$name == ""} {
				return "{+ $substring}"
			} {
				set nextItem [lindex $item 2]
				while {1} {
					if {[string index $nextItem 0] == "\["} {
						set nextItem [lindex $nextItem 2]
					} {
						break
					}
				}
				# return "{\"\{$nextItem $name\[\]\}\" {+ $substring}} "
				return "{\"\{[lindex $nextItem 0] [lindex $nextItem 1] $name\[\]\}\" {+ $substring}} "
			}
		} {
			if {$name == ""} {
				return +
			} {
				set nextItem [lindex $item 2]
				while {1} {
					if {[string index $nextItem 0] == "\["} {
						set nextItem [lindex $nextItem 2]
					} {
						break
					}
				}
				return "{\"\{$nextItem $name\[\]\}\" +} "

			}
		}
	}

	if {[lindex $item 0] == "*" \
		&& ([lindex [lindex $item 1] 0] == "struct" \
		|| [lindex [lindex $item 1] 0] == "union" \
		|| [lindex [lindex $item 1] 0] == "*")} {
		# Pointer to a struct or union. Prefix the name with
		# '*:' so it will be treated as a traversible element.
		return "\"*:$name\" "
	} {
		return "\"\{[lindex $item [expr [llength $item] - 1]] $name\}\" "
	}
}


proc isAggregate {basetype} {
    return [expr \
	{$basetype == "struct" || $basetype == "union" || $basetype == {[]} }]
}


proc debugHandler {args} {
	set ix 0
	foreach arg $args {
		puts stdout [format "%2d: %s" $ix $arg]
		incr ix
	}
}


#||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||
#
# REGISTER WINDOW PROGRAMMING
#

proc setUpRegisterWindow {} {
	global architecture

	# Here I cannot use architecture because cpuFamily name is different
	# For example architecture says it is 68k but in fact it is m68k
	# which is stored in targetArch that is why it is being used instead
	# of architecture like in the unix product

	if {$architecture == "" || $architecture == "unknown"} {
		messageBox -ok -stop "The debugger has not indicated\nits architecture!"
	} {
		hierarchyCreate -change "Registers: i960"
		hierarchySetColor "Registers: i960" \
						[getColorItemIdFor Registers]  [getChangeColorItemIdFor Registers]
		hierarchySetIcon "Registers: i960" \
						[getIconIdFor Registers]
		hierarchySetStructure "Registers: i960" [RegisterStructure]
		hierarchySetValues "Registers: i960" [RegisterVector]
		# hierarchyPost Registers
	}
}


proc RegisterStructure {} {
	return "{\"i960 Registers\" {
	{Local {pfp sp rip r3 r4 r5 r6 r7 r8 r9 r10 r11 r12 r13 r14 r15}}
	{Global {g0 g1 g2 g3 g4 g5 g6 g7 g8 g9 g10 g11 g12 g13 g14 fp}}
	{Control {pc ac tc}}}}"
}

proc RegisterVector {} {

	beginWaitCursor
    set rvec [downtcl gdbEvalScalar \
	    \$pfp \$sp \$rip \$r3 \$r4 \$r5 \$r6 \$r7 \
	    \$r8 \$r9 \$r10 \$r11 \$r12 \$r13 \$r14 \$r15 \
	    \$g0 \$g1 \$g2 \$g3 \$g4 \$g5 \$g6 \$g7 \
	    \$g8 \$g9 \$g10 \$g11 \$g12 \$g13 \$g14 \$fp \
	    \$pc \$ac \$tc]

	endWaitCursor

    return "{{[lrange $rvec 0 15]} {[lrange $rvec 16 31]}
            {[lrange $rvec 32 34]}}"
}


#||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||
#
# Procedures for retrieving address information for source files and
# source lines.
#

#
# fileLineAddress - Returns the address of line 'lineNumber' in the file
# 'sourceFile'.  If an error occurs in the process, an error string is
# returned.
#
proc fileLineAddress {sourceFile lineNumber} {
	# First see if the file 'sourceFile' is known to the debugger by using
	# the builtin command 'gdbFileAddressRange'.  If this call succeeds in
	# returning something (we don't care what), then the 'sourceFile' is
	# known to GDB.
	#
	# Then we do a 'info line $sourceFile:$lineNumber' to retieve the line
	# information for the line.  We trust that this will succeed becuase
	# the call above succeeded.
	#
	# We then parse the returned line information to retrieve the starting
	# address of the line.
	#
	# If any of the above fails, we return an error to the calling routine.
	#
	if {[llength [downtcl gdbFileAddrInfo $sourceFile]] &&
		![catch {downtcl gdb info line $sourceFile:$lineNumber} lineInfo] &&
		[regexp {[^0-9]*([0-9]+)[^0]*(0x[0-9a-fA-F]+).*} \
			 $lineInfo dummy line startAddr]} {
		# The regular expression extracts the starting address from lines
		# of the form:
		#	"Line $line starts at $a0 and ends at $a1",
		# 	"Line $line starts at $a0 and contains no code", and
		# 	"Line $line is out of range".
		return $startAddr
	} {
		error "Source file $sourceFile contains no code at line $lineNumber."
	}
}


#
# 'fetchFileAddressRangeInfo'
#
proc fetchFileAddressRangeInfo {sourceFile} {
	global fileAddressRangeInfo

	# Has file address info ever been retrieved for $sourceFile?
	if {![info exists fileAddressRangeInfo($sourceFile)]} {
		if {![catch {downtcl gdbFileAddrInfo $sourceFile} fileAddrInfo] &&
			[llength fileInfo]} {
			set fileAddressRangeInfo($sourceFile) $fileInfo
		} {
			# Return the error!
			error "Could not obtain file address information for source file '$sourceFile'."
		}
	}

	return $fileAddressRangeInfo($sourceFile)
}


#
# 'fetchFileLineInfo'
#
proc fetchFileLineInfo {sourceFile} {
	global fileLineInfo

	# Has file line info ever been retrieved for $sourceFile?
	if {![info exists fileLineInfo($sourceFile)]} {
		# Retrieve the "file line info" from GDB.  The return from GDB is a
		# list of list, where each element is of the for {line no. startAddr
		# endAddr}.
		if {![catch {downtcl gdbFileLineInfo $sourceFile} linesInfo] && [llength $linesInfo]} {
			proc sortFunc {el1 el2} {
				set el1_line [lindex $el1 0]
				set el2_line [lindex $el2 0]
				if {[expr $el1_line < $el2_line]} {
					return -1
				} elseif {[expr $el1_line > $el2_line]} {
					return 1
				} else {
					return 0
				}
			}

			# GDB sometimes returns line number information that is out of order.
			# This particularly happens for "for" loops.
			set linesInfo [lsort -command sortFunc $linesInfo]

			# Save the new file line info in the globabl cache array.
			set fileLineInfo($sourceFile) $linesInfo
		} {
			# Return the error!
			error "Could not obtain line information for source file '$sourceFile'."
		}
	}

	return $fileLineInfo($sourceFile)
}


#||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||
#
# LOCALS WINDOW PROGRAMMING
#

#||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||
#
# Procedure for setting up the locals window
#

#
# 'setUpLocalsWindow' - This procecure sets up the necessary windows and
# structures for the locals window
#
proc setUpLocalsWindow {} {
	hierarchyCreate -destroy destroyLocals -change Locals
	hierarchySetColor Locals \
			[getColorItemIdFor Locals] [getChangeColorItemIdFor Locals]
	hierarchySetIcon Locals [getIconIdFor Locals]
	fetchCurrentFrameLocals
}

#
# 'fetchCurrentFrameLocals'
#
proc fetchCurrentFrameLocals {} {
	global previousLocalsStruct

	beginWaitCursor

	set frameLocalsTags [downtcl gdbLocalsTags]
	regsub -all "\n" $frameLocalsTags "" frameLocalsTags

    if [string match {*No Locals*} $frameLocalsTags] {
		set frameLocalsTags ""
    }

	set frameLocalsStruct ""
    if {$frameLocalsTags != ""} {
		for {set i 0} {$i < [expr [llength $frameLocalsTags]]} {incr i +1} {
			# if [catch {downtcl gdb -noecho pptype [lindex $frameLocalsTags $i]} exType]
			if [catch {downtcl gdb pptype [lindex $frameLocalsTags $i]} exType] {
				# error -- something is faulty about the expression,
				# or GDB's protocol extension can't understand its type
			} {
		  		set frameLocalsStruct [concat $frameLocalsStruct \
		  				[collectDataMembers $exType [lindex $frameLocalsTags $i] "" 0]]
			}
		}

		set frameLocalsValues ""
		for {set j 0} {$j < [expr [llength $frameLocalsTags]]} {incr j +1} {
			# if [catch {downtcl gdb -noecho ppval [lindex $frameLocalsTags $j]} exVal]
			if [catch {downtcl gdb ppval [lindex $frameLocalsTags $j]} exVal] {
				# error -- something is faulty about the expression,
				# or GDB's protocol extension can't understand its type
			} {
		  		set frameLocalsValues [concat $frameLocalsValues $exVal]
			}
		}
		if {$previousLocalsStruct != $frameLocalsStruct} {
			hierarchySetStructure Locals $frameLocalsStruct
			set previousLocalsStruct $frameLocalsStruct
		}
		hierarchySetValues Locals $frameLocalsValues
	} {
		hierarchySetStructure Locals "{\"No Locals\"} "
		set previousLocalsStruct $frameLocalsStruct
		hierarchySetValues Locals ""
	}

	endWaitCursor
}


#
# 'destroyLocals'
#
proc destroyLocals {args} {
	global previousLocalsStruct

	# Reset the previous locals structure
	set previousLocalsStruct ""
}

#||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||
#
# BACKTRACE WINDOW PROGRAMMING
#

#
# 'setUpBacktraceWindow'
#
proc setUpBacktraceWindow {} {
	global oldBtFrameNum
	global newBtFrameNum

	hierarchyCreate -change -special "Back Trace" "backtraceCallback"
	hierarchySetColor "Back Trace" \
			[getColorItemIdFor "Backtrace"] [getChangeColorItemIdFor "Backtrace"]
	hierarchySetIcon "Back Trace" \
					[getIconIdFor "Backtrace"]
	# hierarchySetStructure "Back Trace" {{"Call Stack" +}}
	fetchBacktrace

	set result [lindex [split [downtcl gdb info frame] \n] 0]

	if {$result != ""} {
		regexp {(Stack level)*[ ]([0-9]+)*} $result a b frame
		if {$frame != ""} {
		    set oldBtFrameNum $newBtFrameNum
		    set newBtFrameNum $frame
			hierarchyHighlightSet "Back Trace" "{Call Stack} $newBtFrameNum"
		}
	}
}


#
# 'fetchBacktrace'
#
proc fetchBacktrace {} {
	global oldBtFrameNum
	global newBtFrameNum

	beginWaitCursor
	hierarchySetStructure "Back Trace" {{"Call Stack" +}}
	if [catch {downtcl gdbStackFrameTags} frameTags] {
		# GDB got sick trying to figure out the call stack.
		# Just put "unknown" in level 1 of the stack.

		hierarchySetValues "Back Trace" "\{unknown\}"
	} {
		hierarchySetValues "Back Trace" "\{$frameTags\}"
		hierarchyHighlightSet "Back Trace" "{Call Stack} $newBtFrameNum"
	}


	endWaitCursor
}


#||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||
#
# Procedures for processing user input in the back trace tree view window.
# and responding to the "Up" and "Down" stack frame menu/toolbar options.
#

#
# setFrame - called when buttons are clicked in the Stack Inspector.
# Generate a command that will set the debug engine to the indicated
# frame (represented by a number, where zero represents the innermost
# frame).
#
proc setFrame {frameId} {
    downtcl -noresult gdb frame $frameId
    updateRegisters
}


#
# 'backtraceCallback'
#
proc backtraceCallback {args} {
    set frameNum [lindex [lindex $args 0] 0]
    downtcl -noresult gdb frame $frameNum
    updateRegisters
}


#
# 'moveFrame'
#
proc moveFrame {args} {
	if {[string match {*up*} [lindex $args 0]]} {
		downtcl -noresult gdb up
	} elseif {[string match {*down*} [lindex $args 0]]} {
		downtcl -noresult gdb down
	}
        updateRegisters
}

proc updateRegisters {} {
        if {[hierarchyExists "Registers: i960"] == 1} {
                setStatusMessage "Refreshing Registers window..."
                hierarchySetValues "Registers: i960" \
                                                [RegisterVector]
                setStatusMessage "Registers window refreshed"
        }
        if {[hierarchyExists "Locals"] == 1} {
                setStatusMessage "Refreshing Locals window..."
                fetchCurrentFrameLocals
                setStatusMessage "Locals window refreshed"
        }

}


#||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||
#
# Procedures for processing the Memory Window.
#

#
# fetchMemoryRange - Query the debug engine for the given target
# memory range.  The format expected by the caller of
# this routine is
#
# count       --should be the number of units of memory to display
# displayFormat unitSize startAddress} {
# lowAddress  --should be equal to the loAddr supplied to this routine
# highAddress --the first byte not contained in the disassembly
# numeric-address <symbolic-address>: instruction [commentary]
# ...
#
# This routine is designed to massage the output of GDB's disass
# command into this format.  Trailing newlines and commentary are
# removed from the GDB output and the relevant addresses are prepended.
#
# GDB won't tell us what the address of the "next" byte is after the
# disassembly, which is very useful information to have!  To synthesize
# it, we don't consume the last disassembled instruction, and use its
# address as the endpoint of the dump.  Since this function is used
# to disassemble blocks of target memory for the disassembly cache,
# the wasted effort is negligible.
#
proc fetchMemoryRange {count displayFormat unitSize startAddress} {
	beginWaitCursor

	set returnVal ""

    if {![string match {*Hex/ASCII*} $displayFormat]} {
		set formatLetter x
		set sizeTypeLetter b
		case $displayFormat {
			{Octal}            { set formatLetter o }
			{Hex}              { set formatLetter x }
			{Decimal}          { set formatLetter c }
			{Unsigned decimal} { set formatLetter u }
			{Binary}           { set formatLetter t }
			{Float}            { set formatLetter f }
			{Address}          { set formatLetter a }
			{Instruction}      { set formatLetter i }
			{Char}             { set formatLetter c }
			{String}           { set formatLetter s }
			default            { set formatLetter x }
		}

		case $unitSize {
			{Byte}             { set sizeTypeLetter b }
			{Halfword}         { set sizeTypeLetter h }
			{Word}             { set sizeTypeLetter w }
			{Giant (8 bytes)}  { set sizeTypeLetter g }
			default            { set sizeTypeLetter b }
		}
		set memry [downtcl gdb x /[format "%s%s%s %s" $count \
								   $formatLetter $sizeTypeLetter \
								   $startAddress]]
	    if {![string match {*Cannot access memory at address*} $memry] ||
			![string match {*No symbol table is loaded*} $memry]} {
			set returnVal [split $memry \n]
		}
	} {
		set memry ""
		set mem [downtcl gdb x /[format "%sxb %s" $count $startAddress]]
		set tmpHexMem [split $mem "\n"]

	    if {![string match {*Cannot access memory at address*} $tmpHexMem] ||
			![string match {*No symbol table is loaded*} $tmpHexMem]} {

			set nElems [llength $tmpHexMem]
			for {set i 0} {$i < $nElems} {incr i 2} {
				if {[lindex $tmpHexMem $i] != ""} {
					set hexline ""
					set charline ""
					set addr ""

					for {set k $i} {$k < [expr $i + 2]} {incr k 1} {
						set firstLine [lindex $tmpHexMem $k]
						if {$k == $i} {
							set hexline \
								[format "%08x: " \
								[expr [string trim [lindex $firstLine 0] :]]]
						}
						if {[string first : [lindex $firstLine 1]] == -1} {
							set hexVals [lrange $firstLine 1 end]
						} {
							set hexVals [lrange $firstLine 2 end]
						}

						set nHexChars [llength $hexVals]
						for {set j 0} {$j < $nHexChars} {incr j 1} {
							set nextElem [lindex $hexVals $j]
							if {$nextElem < 0x20 || $nextElem > 0x7e} {
							    # change it to a period
							    set nextElem 0x2e
							}
							set charline [format "%s%c" $charline $nextElem]
							set hexline [format "%s %02x" $hexline [lindex $hexVals $j]]
						}
					}
					set fullhexline [getPaddedString $hexline 58]
					set fullcharline [getPaddedString $charline 16]
					set fullline [format "%58s %16s" $fullhexline $fullcharline]
					set memry [format "%s\n%s" $memry $fullline]
				}
			}
			set returnVal [split $memry \n]
		}
	}

	endWaitCursor

	if {$returnVal == ""} {
		messageBox -ok -stop "$memry"
	}

   	return $returnVal
}


#
# 'getPaddedString'
#
proc getPaddedString {args} {
	set fullLine [lindex $args 0]
	set spacesToPad [expr [lindex $args 1] - [string length $fullLine]]
	for {set i 0} {$i <= $spacesToPad} {incr i 1} {set fullLine "$fullLine "}
	return $fullLine
}


#||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||
#
# Procedures for acquiring disassembly from the debug engine
#

#
# 'fileAddressRange'
#
proc fileAddressRange {sourceFile} {
	set sourceFileTail [file tail $sourceFile]

	if {![catch {fetchFileAddressRangeInfo $sourceFile} fileAddrRangeInfo]} {
		return [concat [lindex $fileAddrRangeInfo 2] [lindex $fileAddrRangeInfo 3]]
	} {
		return ""
	}
}


#
# 'reformatDisassemblyLine' - Takes the given line of dissasembly in GDB
# format and converts it to a Tornado specific format.  Specifically GDB
# format looks something like:
#
#		| 0x3ba0f4 <main>:	linkw fp,#-516
# or
#		| 0x3ba0f8 <main+4>:	bsr 0x3ba408 <__main>
#
# Tornado specific format is:
#
#		|     3ba0f4       main                linkw fp,#-516
# and
#		|     3ba0f8                           bsr 0x3ba408 <__main>
#
#       ^Left Margin
#        -----5 spaces
#             --------8 char width hex address w/o leading "0x"
#                     -----5 spaces
#                          ------------------18 spaces for symbol
#							                 --2 spaces
#                                              ------ assembly instruction >>>>
#
proc reformatDisassemblyLine {line} {
	# CAUTION: when editing this routine: the following regexp contains embedded
	# tab characters between the brackets (e.g. one space followed by one tab).
	if {[regexp {0x([0-9a-fA-F]*)[ 	]*[<]*([^>]*)[>]*\:[ 	]*(.*)} \
			    $line dummy address symbol instruction]} {
		# The regular expression succeeded and found all elements of the line.

		# Blank away any symbols that have a "+" sign in them; keep only the
		# first occurance of the symbol.
		if {[string first "+" $symbol] != -1} {
			set symbol ""
		}

		# Put the line back together according to the above spec'd format.
		set line [format "%5s%+8s%5s%-18s%2s%s" \
				  "" $address "" $symbol "" $instruction]
	}
	return $line
}


#
# 'disassembleFile'
#
proc disassembleFile {sourceFile} {
	# '$linesInfo' will contain line information, extracted from the
	# above '$linesInfo', for source lines which actually contain code.
	if {![catch {fetchFileLineInfo $sourceFile} linesInfo]} {
		beginWaitCursor
		setStatusMessage "Fetching disassembly for $sourceFile."

		# '$coalescedDisassembly' will be a list, each element consisting of a
		# line number and the associated disassembly lines for that line.
		set coalescedDisassembly ""

		set nElems [llength $linesInfo]
		for {set i 0} {$i < $nElems} {incr i} {
			setStatusMessage [format "Retrieving disassembly for $sourceFile %d%%..." \
							  [expr 100 * $i / $nElems]]

			set iElem [lindex $linesInfo $i]
			set line [lindex $iElem 0]
			set startAddr [lindex $iElem 1]
			set endAddr [lindex $iElem 2]

			# Fetch the disassembly from GDB for line '$line' from range
			# '$startAddr' to '$endAddr'
			set disessemblyLines [split [downtcl gdb disass $startAddr $endAddr] "\n"]

			# From each line of disassembly, extract the address of each assembly
			# line.  In the process leave out the 1st ("Dump...") and last lines
			# ("End of...\n\n").
			set disassemblyLinesWithAddress ""
			set nAssemblyLines [expr [llength $disessemblyLines] - 3]
			for {set j 1} {$j < $nAssemblyLines} {incr j} {
				set disessemblyLine [reformatDisassemblyLine [lindex $disessemblyLines $j]]
				if {[scan $disessemblyLine "%lx" lineAddr] == 1} {
					lappend disassemblyLinesWithAddress [list $lineAddr $disessemblyLine]
				}
			}

			lappend coalescedDisassembly [list $line $disassemblyLinesWithAddress]
		}

		setStatusMessage "Finished disassembly for $sourceFile."
		endWaitCursor

		return $coalescedDisassembly
	} {
		# Return the error!
		error $linesInfo
	}
}


#
# 'disassembleRange' - Disassembles a range of memory between the 'startAddr'
#
#
proc disassembleRange {startAddr endAddr} {
	beginWaitCursor
	setStatusMessage "Fetching disassembly at address $startAddr"

	set disessemblyLines [split [downtcl gdb disass $startAddr $endAddr] "\n"]

	# From each line of disassembly, extract the address of each assembly
	# line.  In the process leave out the 1st ("Dump...") and last lines
	# ("End of...\n\n").
	set disassemblyLinesWithAddress ""
	set nAssemblyLines [expr [llength $disessemblyLines] - 3]
	for {set j 1} {$j < $nAssemblyLines} {incr j} {
		set disessemblyLine [reformatDisassemblyLine [lindex $disessemblyLines $j]]
		if {[scan $disessemblyLine "%lx" lineAddr] == 1} {
			lappend disassemblyLinesWithAddress [list $lineAddr $disessemblyLine]
		}
	}

	setStatusMessage "Finished disassembing at address $startAddr"
	endWaitCursor

	return $disassemblyLinesWithAddress
}


#||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||
#
# Procedures to process data from miscelaneous utility dialogs.
#

#
# 'onDebugDownload'
#
proc onDebugDownload {} {
	set files [fileDialogCreate -filemustexist \
								-multiselect \
								-pathmustexist \
								-noreadonlyreturn \
								-filefilters \
								"Object Files (*.o;*.out)|*.o;*.out|All Files (*.*)|*.*||" \
					 			-title "Download objects" \
					 			-okbuttontitle "&Download"]
	if {$files != ""} {
		downloadFiles $files
	}
}


#
# 'downloadFiles'
#
proc downloadFiles {fileNames} {
	set nElems [llength $fileNames]

	for {set i 0} {$i < $nElems} {incr i} {
		set currentPath [lindex $fileNames $i]
		if {[catch {downtcl gdb load $currentPath} errString]} {
			# An error occurred downloading the file
			messageBox -stop "Error downloading file: [lindex $fileNames $i]"
		} {
			set index [string last "\\" $currentPath]
			if {$index == 0} {
				set index [string last "/" $currentPath] - 1]
			}
			set newString [string range $currentPath 0 [expr $index - 1]]
			downtcl -noresult gdb path $newString
			downtcl -noresult gdb dir $newString

			set errString [lindex [split $errString "\n"] 0]
			setStatusMessage $errString
		}
	}
}


#
# 'setSearchPaths'
#
proc setSearchPaths {args} {
	set nElems [llength $args]

	# Each path element is sent separately because the list in 'args'
	# is designed to prevent Tcl backslash ('\') translation.  Thus the
	# list is a Tcl list--not a list as understood by either the 'dir'
	# command or 'path' command.

	# For each element in 'args' do a 'dir' command in GDBs context.
	# This allows for the assumption that the source file and object
	# files are in the same location.

	# 'args' is formed in search order.  In order to preserve the search
	# order in .GDB, we send from the last path to the first path.

	for {set i [expr $nElems - 1]} {$i >= 0} {incr i -1} {
		downtcl -noresult gdb dir [lindex $args $i]
	}
}


set prevTasks {}
set prevArgs {}
set prevBrkAtEntrys {}
set selTaskData {}

proc showRunDialog {} {
    global prevTasks
    global prevArgs
    global prevBrkAtEntrys
	global selTaskData

    set prevTasks {}
    set prevArgs {}
    set prevBrkAtEntrys {}
	set selTaskData {}

#	set prevTasks [appRegistryEntryRead "DebugTasks" "PrevTasks"]
	set prevArgs [appRegistryEntryRead "DebugTasks" "PrevRunArgs"]
	set prevBrkAtEntrys [appRegistryEntryRead "DebugTasks" "BreakAtEntries"]

#	dialogCreate -name runDialog \
#		-title "Run Task" \
#		-init runDlgInitProc \
#		-w 187 -h 75 -controls {
#	    { label -title "&Task name:" -x 7 -y 7 -w 39 -h 9 }
#		{ combo -editable -name taskSelCmb -callback taskSelCB -x 47 -y 7 -w 135 -h 49 }
#	    { label -title "&Arguments:" -x 7 -y 26 -w 38 -h 9 }
#		{ text -name taskArgs -x 47 -y 24 -w 135 -h 12 }
#	    { boolean -auto -title "&Break at main()" -name brkAtEntry -x 7 -y 42 -w 78 -h 10 }
#	    { button -default -title "OK" -name runDlgokbutt -callback runTaskCB -x 7 -y 57 -w 50 -h 14 }
#	    { button -title "Cancel" -callback {windowClose runDialog} -x 62 -y 57 -w 50 -h 14 }
#	    { button -title "&Help" -helpbutton -x 131 -y 57 -w 50 -h 14 }
#	}

	dialogCreate -name runDialog \
		-title "Run" \
		-init runDlgInitProc \
		-w 187 -h 75 -controls {
	    { label -title "&Arguments:" -x 7 -y 7 -w 38 -h 9 }
		{ text -name taskArgs -x 47 -y 7 -w 135 -h 12 }
	    { boolean -auto -title "&Break at main()" -name brkAtEntry -x 7 -y 42 -w 78 -h 10 }
	    { button -default -title "OK" -callback runAppCB -x 7 -y 57 -w 50 -h 14 }
	    { button -title "Cancel" -callback {windowClose runDialog} -x 62 -y 57 -w 50 -h 14 }
	    { button -title "&Help" -helpbutton -x 131 -y 57 -w 50 -h 14 }
	}

	return $selTaskData
}


proc runDlgInitProc {} {
    set initArgs     [appRegistryEntryRead "Intelgdb960" "runargs"]
    set breakAtInit  [appRegistryEntryRead "Intelgdb960" "breakatinit"]
    controlValuesSet runDialog.taskArgs $initArgs
    controlValuesSet runDialog.brkAtEntry $breakAtInit
}


proc taskSelCB {} {
    global prevTasks
    global prevArgs
    global prevBrkAtEntrys

	set eventInfo [eventInfoGet runDialog]
	if [string match {*selchange*} [lindex $eventInfo 0]] {
		set sel [controlSelectionGet runDialog.taskSelCmb]
		controlValuesSet runDialog.taskArgs [lindex $prevArgs $sel]
		controlValuesSet runDialog.brkAtEntry [lindex $prevBrkAtEntrys $sel]
	}
}

proc runAppCB {} {
    set runargs [controlValuesGet runDialog.taskArgs]
    set breakAtInit [controlValuesGet runDialog.brkAtEntry]

    #puts "runargs are: $runargs"
    appRegistryEntryWrite "Intelgdb960" "runargs"     $runargs
    appRegistryEntryWrite "Intelgdb960" "breakatinit" $breakAtInit
    windowClose runDialog
    if {$breakAtInit} {
	downtcl gdb tbreak main
    }
    downtcl -noresult gdb run $runargs
}

proc runTaskCB {} {
    global prevTasks
    global prevArgs
    global prevBrkAtEntrys
	global selTaskData

    #	set cmbVal [controlValuesGet runDialog.taskSelCmb -edit]
    set textVal [controlValuesGet runDialog.taskArgs]
    set boolVal [controlValuesGet runDialog.brkAtEntry]

    #	set selItem [controlSelectionGet runDialog.taskSelCmb]
    set selItem

    if {$selItem == ""} {
	lappend prevTasks "$cmbVal"
	lappend prevArgs "$textVal"
	lappend prevBrkAtEntrys "$boolVal"
    } {
	set prevArgs [lreplace $prevArgs $selItem $selItem $textVal]
	set prevBrkAtEntrys [lreplace $prevBrkAtEntrys $selItem $selItem $boolVal]
    }

    set strtIndex 0
    if {[llength $prevTasks] > 10} {
	set strtIndex [expr [llength $prevTasks] - 10]
    }

    if {$strtIndex != -1} {
	set prevTasks [lrange $prevTasks $strtIndex [expr $strtIndex + 9]]
	set prevArgs [lrange $prevArgs $strtIndex [expr $strtIndex + 9]]
	set prevBrkAtEntrys [lrange $prevBrkAtEntrys $strtIndex [expr $strtIndex + 9]]
    }

    if {$selItem == ""} {
	set selItem [expr [llength $prevTasks] - 1]
    }
    appRegistryEntryWrite -int "DebugTasks" "PreviousSelection" $selItem
    appRegistryEntryWrite "DebugTasks" "PrevTasks" $prevTasks
    appRegistryEntryWrite "DebugTasks" "PrevRunArgs" $prevArgs
    appRegistryEntryWrite "DebugTasks" "BreakAtEntries" $prevBrkAtEntrys

    lappend selTaskData [lindex $prevTasks $selItem] \
	    [lindex $prevArgs $selItem] \
	    [lindex $prevBrkAtEntrys $selItem]

    windowClose runDialog
}


#
# 'onDebugRun'
#
proc onDebugRun {} {
	global currentTarget

	if {![string match {*WTX System*} $currentTarget]} {
		while {1} {
			set runArgs [showRunDialog]
			if {[llength $runArgs] > 0} {
				if [sendRun $runArgs] {
					break
				}
			} {
				break
			}
		}

	} {
		messageBox "The target is in System mode.\n\
							'Run' cannot be used in this mode"
	}
}


#
# 'sendRun'
#
proc sendRun {runArgs} {
	set errString ""
	set res 0

	set res [downtcl gdbSymbolExists [lindex $runArgs 0]]

	if [string match {*1*} $res] {
		set funcInfo ""
		catch {downtcl gdb print [lindex $runArgs 0]} funcInfo
		# $1 = {int ()} 0x3ba146 <mungeSimple>

		# {(\$[0-9]+[ ]\=[ ]\{[0-9a-zA-Z]+[ ]\(\)\}[ ])(0x[0-9a-fA-F]+)([ ]\<.+\>)}
		set address ""
		if {[regexp \
		{(\$[0-9]+[ ]\=[ ]\{.*\}[ ])(0x[0-9a-fA-F]+)([ ]\<.+\>)}\
		$funcInfo a b address d] && \
		[regexp {(0x[0-9a-fA-F]+)} $address]} {

			if {[lindex $runArgs 2] == "1"} {
				downtcl -noresult gdb tbreak * $address
			}
			downtcl -noresult gdb run [lindex $runArgs 0] [lindex $runArgs 1]
		}
		return 1
	} {
		messageBox -ok -stop "Symbol not found."
		return 0
	}
	return 0
}


#
# 'refreshTaskList'
#
proc refreshTaskList {} {
	set __pat0 "tSl\[0-9\]+Wrt|tFtpdServ\[0-9\]+|tFtpdTask|tNetTask|tShell|"
	set __pat1 "tRlogind|tRlogOutTask|tRlogInTask|tRlogChildTask|"
	set __pat2 "tPortmapd|tTelnetd|tTelnetOutTask|tTelnetInTask|"
	set __pat3 "tTftpTask|tTftpdTask|tTftpRRQ|tTftpWRQ|tExcTask|"
	set __pat4 "tLogTask|tSpyTask|tRootTask|tRestart|tRdbTask|tWdbTask"
	set wrsTaskPattern "$__pat0$__pat1$__pat2$__pat3$__pat4"

	set tasksList ""
	lappend tasksList [format "%s      %-15s" "system" System]

	if {![catch {downtcl gdbTaskListGet} tasks]} {
		while {[llength $tasks] > 1} {
			bindNamesToList {taskId taskName taskStatus pri priNormal} \
					[lrange $tasks 0 4]
			set string [format "%#7x    %-15s" $taskId $taskName]

			if [regexp $wrsTaskPattern $taskName] {
				lappend tasksList $string
			} {
				lappend tasksList $string
			}
			set tasks [lrange $tasks 5 end]
		}
		return $tasksList
	} {
		messageBox "Unable to retrieve task list.\n$tasks"
	}
}


##############################################################################
#
# bindNamesToList - Create variables in the caller bound to list values
#
# This function can be used to assign names to list elements.  Here's an
# example of its use:
#
#    bindNamesToList {a b c} {10 11 12}
#
# After this call, there will be variables a, b, and c in the procedure scope
# with the values 10, 11, and 12 respectively.  If there are more names than
# values, excess names will be given the value 0.  If there are more values
# than names, excess values are ignored.
#
# SYNOPSIS:
#   bindNamesToList nameList valList
#
# PARAMETERS:
#   nameList: a list of names of variables to create in caller's scope
#   valList: a list of values to bind the created variables to
#
proc bindNamesToList {nameList valList} {
    foreach name $nameList {
	upvar $name n
	if {[llength $valList] > 0} {
	    set val [lindex $valList 0]
	    set valList [lrange $valList 1 end]
	} {
	    set val 0
	}
		set n $val
    }
}


#
# 'onDebugDetach'
#
proc onDebugDetach {} {
	# This has to be with -noresult, otherwise the context does not get
	# update because GDB doe snot send a "status ready ph" message.
	downtcl -noresult gdb detach
}


#
# 'attachToNewTask'
#
proc attachToNewTask {args} {
	downtcl -noresult gdb detach
	downtcl -noresult gdb attach [lindex $args 0]
}


#
# 'onDebugAttach'
#
proc onDebugAttach {} {
	dialogCreate -name attachDialog \
		-title "Attach" \
		-init attachDlgInit \
		-w 185 -h 137 -controls {
	    { label -fixed -title "Task ID     Task Name" -x 7 -y 11 -w 121 -h 7 }
	    { button -title "&!" -name refreshTaskList -callback attachDlgInit -x 162 -y 4 -w 15 -h 14 }
	    { list -fixed -callback taskSelChange -name tasksList -x 7 -y 22 -w 171 -h 65 }
	    { label -title "&Attach to:" -x 7 -y 94 -w 34 -h 7 }
	    { text -name taskToAttach -x 41 -y 93 -w 135 -h 12 }
	    { button -callback attachToTask -name okbutt -default -title "Attach" -x 7 -y 118 -w 50 -h 14 }
	    { button -title "Cancel" -callback closeAttachDlg -x 67 -y 118 -w 50 -h 14 }
	    { button -title "&Help" -helpbutton -x 129 -y 118 -w 50 -h 14 }
	}
}


proc closeAttachDlg {args} {
	windowClose attachDialog
}


proc taskSelChange {} {
	set selItem [controlSelectionGet attachDialog.tasksList -string]
	set taskId ""

	set event [eventInfoGet attachDialog]
	if {$selItem != ""} {
		regexp {(.*)([ ]+[0-9a-zA-Z]+[ ]*)} $selItem dummy taskId
	}
	controlValuesSet attachDialog.taskToAttach [string trim $taskId]

	if [string match {*dblclk*} [lindex $event 0]] {
		attachToTask
	}
}


proc attachToTask {} {
	set attachTask [controlValuesGet attachDialog.taskToAttach]

	if {$attachTask != ""} {
		attachToNewTask $attachTask
	}
	closeAttachDlg
}


proc attachDlgInit {args} {
	set newTaskList [refreshTaskList]

	controlValuesSet attachDialog.tasksList $newTaskList
}

# Read in the user's home-directory CrossWind Tcl initialization file.
if [catch {source [wtxPath host resource tcl]crosswind.tcl} result] {
    puts "could not source crosswind.tcl"
}
if {![catch {file exists $env(HOME)/.wind/crosswind.tcl} result] && $result} {
    source $env(HOME)/.wind/crosswind.tcl
}
