##############################################################################
# app-config/Tornado/01Launch.win32.tcl - Tornado (Windows) Tool Launching
#
# Copyright (C) 1995-96 Wind River Systems, Inc.
#

#puts "sourcing 01launch.win32.tcl"

################################################################################
# GLOBAL VARIABLES
#
set tgtsvr_selected	""
set selectedTool ""
set tsManage 0

################################################################################
# MISC. TOOL LAUNCHING CALLBACK ROUTINES
# 
# Callback routines (called from Tornado.Exe) to launch: tools, shells, and
# debuggers.
#

#
# toolLaunch - Given the six arguments, this command expands any Tcl variables
# and braced Tcl commands within 'menuText', 'toolCommandLine' and 'workingDir'.
# Once the expansion is complete and results in a non-zero length string, the
# built-in routine 'launchTool' is invoked with appropriate arguments.  Removes
# the last occurances of the '&' character in 'menuText'.  No return value.
#
proc toolLaunch {menuText toolCommandLine workingDir \
				 redirectStdInput redirectStdOutput redirectStdError \
				 promptForArgs closeWindowOnExit redirectToChildWindow} {
	# Remove the last occurance of '&' character.
	regsub {(.*)&(.*)} $menuText {\1\2} menuText

	set toolCommandLine [expandAndOrEvalutateList $toolCommandLine]
	set windowTitle [expandAndOrEvalutateList $menuText]
	set workingDir [expandAndOrEvalutateList $workingDir]

	if {[llength $toolCommandLine]} {
		# In 'toolCommandLine' and 'workingDir', if the last character is a
		# backslash, replace it with a double backslash to fix a bug with
		# 'RunCMD.Exe'.
		regsub {(.*)\\$} $toolCommandLine {\1\\\\} toolCommandLine
		regsub {(.*)\\$} $workingDir {\1\\\\} workingDir

		puts stdout "\nExecuting: toolCommandLine: \"$toolCommandLine\""
		puts stdout "           workingDir:: \"$workingDir\""

		if {[catch {launchTool $windowTitle $toolCommandLine $workingDir \
							   $redirectStdInput $redirectStdOutput $redirectStdError \
							   $promptForArgs $closeWindowOnExit \
							   $redirectToChildWindow} result]} {
			puts "\nError: Tool launch failed: '$result'\n"
		}
	}
}


#
# buildLaunch - Given the six arguments, this command expands any Tcl variables
# and braced Tcl commands within 'menuText', 'buildTarget' and 'workingDir'.
# Once the expansion is complete, the built-in routine 'launchBuild' is invoked
# with appropriate arguments.  Removes the last occurances of the '&' character
# in 'menuText'.  No return value.
#
proc buildLaunch {menuText buildTarget workingDir} {
	# Remove the last occurance of '&' character.
	regsub {(.*)&(.*)} $menuText {\1\2} menuText

	set makeCommandLine [format "make.exe %s" [expandAndOrEvalutateList $buildTarget]]
	set windowTitle [expandAndOrEvalutateList $menuText]
	set workingDir [expandAndOrEvalutateList $workingDir]

	# In 'makeCommandLine' and 'workingDir', if the last character is a
	# backslash, replace it with a double backslash to fix a bug with
	# 'RunCMD.Exe'.
	regsub {(.*)\\$} $makeCommandLine {\1\\\\} makeCommandLine
	regsub {(.*)\\$} $workingDir {\1\\\\} workingDir

	puts stdout "\nExecuting: makdeCommandLine:\"$makeCommandLine\""
	puts stdout "           workingDir:: \"$workingDir\""

	if {[catch {launchBuild $windowTitle $makeCommandLine $workingDir} result]} {
		puts "\nError: Build launch failed: '$result'\n"
		error $result
	}
}


#
# shellLaunch - Given the target name in 'targetName', this command forms the
# WindSh command line and passes it back to the Tornado dev. env. for actual
# execution.  Currently it adds no special command line options, but can be
# customized to add any user required options (e.g. "-c[plus]" for C++
# demangling, "-T[cl mode]" to start WindSh in Tcl mode, "-p[oll]" change the
# WindSh poll interval, etc.).
#
proc shellLaunch {targetName} {
	if {[catch {launchShell "Shell $targetName" "windsh $targetName"} result]} {
		puts "\nError: Shell launch failed: '$result'\n"
	}
}


#
# browserLaunch - Given the target name in 'targetName', this command forms the
# Browser command line and invokes the browser by passing it back to the Tornado 
# dev. env. for actual execution.
#
proc browserLaunch {targetName} {
	global env

	set browserFile [wtxPath host resource tcl]Browser.win32.tcl

	if {[catch {windowCreate -wi 470 -name Browser -s "$browserFile" \
				-icon [wtxPath host resource bitmaps browser controls]Browser.ico \
				-init "browserInit $targetName"} result]} {
		puts "\nError: Browser launch failed: '$result'\n"
	}
}


#
# debuggerLaunch - Given the target name in 'targetName', this command forms 
# the Debugger command line and invokes the 
# Debugger by passing it back to the Tornado dev. env. for actual execution.
#
proc debuggerLaunch {targetName} {
	if {[catch {launchDebugger $targetName} result]} {
		puts "\nError: Debugger launch failed: '$result'\n"
	}
}


#
# expandAndOrEvalutateList - This routine performs the variable substituion and
# Tcl command expansion for 'lst'.  Care has been taken to prevent backslash
# substitution by the Tcl interpretter during evaluation.
#
proc expandAndOrEvalutateList {lst} {
	global env
	if {[getCurrentFile] != ""} {
		# If there is no active document, leave the variables undefined causing
		# subsequent substitution errors that are handled below.
		set filepath [getCurrentFile]
		set filedir  [file dirname $filepath]
		set filename [file tail $filepath]
		set basename [file rootname $filename]
	}
	catch {set line [getCurrentFileLine]}
	catch {set column [getCurrentFileColumn]}
	catch {set textsel [getCurrentFileTextSelection]}
	catch {set targetName [getCurrentTarget]}
	catch {set workingDir [workingDirectoryGet]}
	if {![catch {getCurrentTarget}]} {
		catch {regexp {(.*)\@} [getCurrentTarget] dummy target}
	}
	if [info exists [wtxPath]] {set wind_base [wtxPath]} {set wind_base ""}
	set wind_host_type 	[getPlatfrom]
	if [info exists env(WIND_REGISTRY)] {set wind_registry $env(WIND_REGISTRY)} {set wind_registry ""}

	set newList ""

	# Does the list actually contain anything?
	if {[llength $lst]} {
		# Expand each list element.
		for {set i 0} {$i < [llength $lst]} {incr i} {
			set error ""

			# Extact the $i'th item back as a list.  'lrange' is used, because
			# 'lindex' would cause one layer of backslashes to be removed;
			# 'lrange' does not do this (possibly a Tcl v7.3 bug).
			set listElem [lrange $lst $i $i]

			# Protect single back-slashes ('\') from one level of 'eval'.
			regsub -all {\\} $listElem {\\\\} listElem

			# Does the element represent a "braced" Tcl script.
			if {([string range $listElem 0 0] == "\{") &&
				([string range $listElem [expr [string length $listElem] - 1] end] == "\}")} {
				# Evaluate the Tcl script, while catching any evaluation errors.
				if {[catch {eval [lindex $listElem 0] [lrange $listElem 1 end]} listElem]} {
					set error $listElem
				}
			} {
				# Expand the list element; causes substitute of all referenced
				# variables, while catching any substitution error.
				if {[catch {eval format "%s" $listElem} listElem]} {
					set error $listElem
				}
			}

			# Did an error occur during script or substitution evaluation?
			if {$error != ""} {
				if {[regexp {.*("filepath").*} $error dummy errVar] ||
					[regexp {.*("filedir").*} $error dummy errVar] ||
					[regexp {.*("filename").*} $error dummy errVar] ||
					[regexp {.*("basename").*} $error dummy errVar]} {
					set error "attempt to use '$errVar' with no active document"
				} elseif {[regexp {.*("line").*} $error dummy errVar]} {
					catch {getCurrentFileLine} errString
					set error "attempt to use '$errVar' caused the error: $errString"
				} elseif {[regexp {.*("column").*} $error dummy errVar]} {
					catch {getCurrentFileColumn} errString
					set error "attempt to use '$errVar' caused the error: $errString"
				} elseif {[regexp {.*(textsel).*} $error dummy errVar]} {
					catch {getCurrentFileTextSelectionz} errString
					set error "attempt to use '$errVar' caused the error: $errString"
				} elseif {[regexp {.*("target.*").*} $error dummy errVar]} {
					catch {getCurrentTarget} errString
					set error "attempt to use '$errVar' with no selected target"
				}
			}
			if {$error != ""} {
				messageBox -excl "Unable to evaluate '$lst'.  Cause: $error."
				error $error
			}

			# Append 'listElem' to '$newList'.
			if {$i == 0} {
				set newList $listElem
			} {
				set newList [format "%s %s" $newList $listElem]
			}
		}
	}

	# Return the expanded/evaluated list!
	return $newList
}


################################################################################
# PROCEDURES RELATED TO LAUNCH TOOLBAR AND TORNADO TOOL LAUNCHING DIALOGS
#

#
# getTargetServerArch -
#
proc getTargetServerArch {ts} {
    global cpuFamily

	catch {wtxToolAttach $ts -launcher} errString
	
    if {! [string match {*Couldn't obtain client handle for target server*} $errString]} {
	    set info		[wtxTsInfoGet]
	    set typeId		[lindex [lindex $info 2] 0]

		wtxToolDetach
	    return "$cpuFamily($typeId)"
	} {
		return ""
	}
}


#
# retrieveTargetInfo -
#
proc retrieveTargetInfo {ts} {
	global cpuType

	catch {wtxToolAttach $ts -launcher} errString
	
    if {! [string match {*Couldn't obtain client handle for target server*} $errString]} {
 	    set info		[wtxTsInfoGet]
	    set link		[lindex $info 0]
	    set typeId		[lindex [lindex $info 2] 0]
	    set tgtsvr_cpu	$cpuType($typeId)
	    set tgtsvr_rtvers	"VxWorks [lindex [lindex $info 3] 1]"
	    set tgtsvr_bsp	[lindex $info 4]
	    set tgtsvr_core	[lindex $info 5]
	    set tgtsvr_host	"Unknown"
		# set tgtsvr_host	[exec uname -a]

	    set memSize		[lindex [lindex $info 6] 1]
	    set tgtsvr_user	[lindex $info 8]
	    set start		[lindex $info 9]
		set access		[lindex $info 10]
	    set tgtsvr_lock	[lindex $info 11]
	    set agentVersion	"WDB 1.0"

	    set infoString              "Name   : $ts"
	    set infoString "$infoString\nStatus : $tgtsvr_lock"
	    set infoString "$infoString\nRuntime: $tgtsvr_rtvers"
	    set infoString "$infoString\nAgent  : $agentVersion"
	    set infoString "$infoString\nCPU    : $tgtsvr_cpu"
	    set infoString "$infoString\nBSP    : $tgtsvr_bsp"
	    set infoString "$infoString\nMemory : [format "%#x" $memSize]"
	    set infoString "$infoString\nLink   : $link"
	    set infoString "$infoString\nUser   : $tgtsvr_user"
	    set infoString "$infoString\nStart  : $start"
	    set infoString "$infoString\nLast   : $access"


		set toolListString "Attached Tools :\n"
	    set toolList [lrange $info 14 end]

	    foreach tool $toolList {
			if {[lindex $tool 4] != ""} {
			    set toolUser "([lindex $tool 4])"
			} {
			    set toolUser ""
			}

			set toolListString "$toolListString [lindex $tool 1] $toolUser\n"
	    }

		set infoString "$infoString\n$toolListString"

		regsub -all "\n" $infoString "\r\n" infoString1
	    set returnVal $infoString1
		
		wtxToolDetach
	} {
	    set returnVal $errString
	}

	return $returnVal
}


#
# selectNewTarget -
#
proc selectNewTarget {} {

	set eventInfo [eventInfoGet launch]

	if [string match {*dropdown*} [lindex $eventInfo 0]] {
		targetServerListUpdate
	} {
		set tgt_sel [getSelectedTarget]
		if {$tgt_sel == ""} {
		    #controlEnable launch.browser 0
			#controlEnable launch.shell 0
		    #controlEnable launch.debugger 0
		} {
			#controlEnable launch.browser 1
			#controlEnable launch.shell 1
			#controlEnable launch.debugger 1
			checkTarget $tgt_sel
		}
	}

}


#
# getSelectedTarget -
#
proc getSelectedTarget {} {
    #set tgt_sel [controlSelectionGet launch.tgtCombo -string]
    return $tgt_sel
}


#
# selectATarget -
#
proc selectATarget {tgtName} {
    #controlSelectionSet launch.tgtCombo -string $tgtName
}


#
# browserSelect -
#
proc browserSelect {} {
	global tgtsvr_selected

    #set tgtsvr_selected [controlSelectionGet launch.tgtCombo -string]

	if {$tgtsvr_selected != "" &&
		[checkTarget $tgtsvr_selected] == "0"} {
		browserLaunch $tgtsvr_selected
	}
}


#
# shellSelect -
#
proc shellSelect {} {
	global tgtsvr_selected

    #set tgtsvr_selected [controlSelectionGet launch.tgtCombo -string]

	if {$tgtsvr_selected != "" &&
		[checkTarget $tgtsvr_selected] == "0"} {
		shellLaunch $tgtsvr_selected
	}
}


#
# debuggerSelect -
#
proc debuggerSelect {} {
	global tgtsvr_selected

    #set tgtsvr_selected [controlSelectionGet launch.tgtCombo -string]

	if {$tgtsvr_selected != "" &&
		[checkTarget $tgtsvr_selected] == "0"} {
		debuggerLaunch $tgtsvr_selected
	}
}


#
# targetServerListUpdate -
#
proc targetServerListUpdate {} {
    global tgtsvr_selected

    set servList [getTgtServerList]
    #controlValuesSet launch.tgtCombo $servList

    #set tgtsvr_selected [controlSelectionGet launch.tgtCombo -string]
	if {$tgtsvr_selected == ""} {
	    #controlEnable launch.browser 0
	    #controlEnable launch.shell 0
	    #controlEnable launch.debugger 0
	} {
	    #controlEnable launch.browser 1
	    #controlEnable launch.shell 1
		#controlEnable launch.debugger 1
		checkTarget $tgtsvr_selected
	}
}


#
# getTgtServerList -
#
proc getTgtServerList {} {
    set servList ""
    foreach server [wtxInfoQ .*] { 
		if {[lindex $server 1] == "tgtsvr"} {
		    set servList "$servList [lindex $server 0]"
		}
    }
	return $servList
}


#
# checkTarget -
#
proc checkTarget {ts} {
	if {[catch {wtxToolAttach $ts -launcher} attachRetVal]} {
		# Target Server not responding !!!
		set ans ""
		set ans [messageBox -yesno "Target Server $ts is not responding.  \
									It may simply be too busy to \
									answer.  If the Target Server is known to \
									have exited we suggest you remove \
									it from the Launcher.  Would you like to remove it now?"]

		if {$ans == "yes"} {
			if {[catch {wtxUnregister $ts} result]} {
				messageBox "Can't communicate with the WTX registry."
			}
			targetServerListUpdate
		}
		return 1
	}
	return 0
}


#
# targetServerSelect -
#
proc targetServerSelect {args} {
	global selectedTool ""
	global tsManage

	# Creation of the Configuration panel
	set selectedTool [lindex $args 0]
	set tsManage 0

	if {$selectedTool == "targetServerManage"} {
		set tsManage 1
	}

	if {$tsManage} {
	 	dialogCreate -name targetServerManage -title "Manage Target Servers" \
		  -w 218 -height 235 -init targetServerManageInit -controls {	\
			{label -title "&Targets:" -name lbl1 -x 9 -y 9 -w 32 -h 9}  \
			{combo -name targetManage_targets -callback on_tm_targets \
			  -x 40 -y 7 -w 148 -h 67}  \
			{label -name lbl2 -title "Target &Information" -x 9 -y 25 -w 72 -h 9}  \
			{text -fixed -readonly -vscroll -multiline \
			  -name targetManage_tsInfo -x 9 -y 38 -w 200 -h 150}  \
			{label -title "Select &Action:" -name lbl1 -x 30 -y 197 -w 49 -h 9}  \
			{combo -name targetManage_combo_action -callback on_targetManage_combo_action \
			  -x 80 -y 195 -w 100 -h 67} \
			{button -default -callback on_targetManage_apply -title "Apply" -name applyButton \
			  -x 7 -y 215 -w 50 -h 14} \
			{button -callback on_targetManage_close -title "Close" -name tm_closeButton	\
			  -x 64 -y 215 -w 50 -h 14} \
			{button -callback on_targetManage_help -title "&Help" -helpbutton -name tm_helpButtom \
			  -x 160 -y 215 -w 50 -h 14}  \
			{button -callback on_targetManage_refresh -title "!" -name tm_bangRefresh -x 193 -y 6 -w 16 -h 14} \
		  }
	} else {
		dialogCreate -name targetServerSelect -title "Launch $selectedTool" \
			-init targetServerSelectDlgInit -w 218 -height 207 \
		    -controls { \
		    {label -title "&Targets:" -name lbl1 -x 7 -y 7 -w 32 -h 9} \
			{combo -name targets -callback dlgNewTarget -x 46 -y 7 -w 145 -h 67} \
			{button -callback tgtSvrListRefresh -title "!" -name targetServerSelectDlgInit -x 195 -y 6 -w 16 -h 14} \
			{label -name lbl2 -title "Target &Information" -x 7 -y 25 -w 72 -h 9} \
			{text -fixed -readonly -vscroll -multiline -name tsInfo -x 7 -y 38 -w 204 -h 142} \
			{button  -default -callback selectTarget -title "OK" -name okButt -x 7 -y 186 -w 50 -h 14} \
			{button -callback tgtSvrDlgClose -title "Cancel" -name cancelButt -x 64 -y 186 -w 50 -h 14} \
			{button  -title "&Help" -helpbutton -name helpButt \
									-x 161 -y 186 -w 50 -h 14} \
		    }
	}
}


#
# targetServerSelectDlgInit -
#
proc targetServerSelectDlgInit {} {
	controlValuesSet targetServerSelect.targets [getTgtServerList]
	controlSelectionSet targetServerSelect.targets 0
	controlFocusSet targetServerSelect.okButt
}


proc tgtSvrListRefresh {} {
	controlValuesSet targetServerSelect.targets [getTgtServerList]
	controlSelectionSet targetServerSelect.targets 0
}


#
# dlgNewTarget -
#
proc dlgNewTarget {} {
	set eventInfo [eventInfoGet launch]
	if ![string match {*dropdown*} [lindex $eventInfo 0]] {
		set tgt_sel [controlSelectionGet targetServerSelect.targets -string]
		if {$tgt_sel != ""} {
			controlValuesSet targetServerSelect.tsInfo [retrieveTargetInfo $tgt_sel]
		} {
			controlEnable targetServerSelect.okButt 0
		}
	}
}


#
# selectTarget -
#
proc selectTarget {} {
	global selectedTool
	
	set savedTool $selectedTool
	set tgt_sel [controlSelectionGet targetServerSelect.targets -string]
	tgtSvrDlgClose

	# Depending on the tool launched, launch it
	if [string match {*Debugger*} $savedTool] {
		debuggerLaunch $tgt_sel
	} elseif [string match {*Browser*} $savedTool] {
		browserLaunch $tgt_sel
	} elseif [string match {*Shell*} $savedTool] {
		shellLaunch $tgt_sel
	}
}


#
# tgtSvrDlgClose -
#
proc tgtSvrDlgClose {} {
	windowClose targetServerSelect
	set selectedTool ""
}


#
# writeTargetName -
#
proc writeTargetName {} {
	writeRegistryEntry [getSelectedTarget Target]
}


################################################################################
# SOURCE LIBRARIES
#
source [wtxPath host resource tcl]wtxcore.tcl


################################################################################
# UI CREATION
#
#controlCreate launch \
#	"combo -callback selectNewTarget -name tgtCombo \
#			-tooltip \"Target Server List\" \
#			-x 5 -y 100 -w 72 -h 50"
#
#controlCreate launch "separator -w 5"
#
#controlCreate launch "button -name browser -callback browserSelect \
#			-tooltip \"Launch Browser\" \
#			-bitmap \"[wtxPath host resource bitmaps launch controls]Browser.bmp\""

#controlCreate launch "button -name shell \
#			-callback shellSelect \
#			-tooltip \"Launch Shell\" \
#			-bitmap \"[wtxPath host resource bitmaps launch controls]Shell.bmp\""

#controlCreate launch "button -name debugger -callback debuggerSelect \
#			-tooltip \"Launch Debugger\" \
#			-bitmap \"[wtxPath host resource bitmaps launch controls]Debugger.bmp\""


# Finally, update the target server list in the Tornado Launch toolbar.
targetServerListUpdate


