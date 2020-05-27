# gdb.tcl - gdb specific Tcl commands loaded on gdb startup
#
# Copyright 1995 Wind River Systems, Inc.
#
# modification history
# --------------------
# 01k,21dec95,wmd added sourcing of gdb.tcl from WIND_BASE/.wind (SPR 5765)
# 01j,01dec95,s_w add gdbTaskAttachHook and gdbSystemAttachHook that get
#		  get called before an attach occurs. Allows checking of
#		  VX_UNBREAKABLE task option (SPR 5575)
# 01i,14nov95,s_w change error message in task kill hook
# 01h,09nov95,s_w add gdbTaskKillHook (see SPR 5090 and 5421) and 
#		  gdbSystemKillHook for completeness. Add gdbWtxErrorHook
#		  which gets called when a C API (only) call causes a 
#		  WTX error (part of fix for SPR 4591).
# 01g,07nov95,s_w make loading and unloading of modules on receipt of
#		  OBJ_[UN]LOADED events optional.
# 01f,07nov95,s_w add code to read users gdb.tcl file, add Tcl proc
#		  to get and dispatch events received by gdb. Add hook
#		  procs called on target, task and system attach/detach
#		  SPR 4860, 5047
# 01e,28sep95,s_w add gdb register set info (SPR 4916)
# 01d,13sep95,s_w add gdbIORedirect and gdbIOClose to support redirection
#		  of task and global IO by Gdb (SPR 4592)
# 01c,05sep95,s_w add gdbTaskNameToId so gdb can convert task names to
#		  task ids (for SPR 4587). Correct use of wtxPath to avoid
#		  trailing slash in file names.
# 01b,14jun95,s_w override the shell.tcl WTX error handler. Change
#		  gdbCpuTypeNameGet to take a type number arg.
# 01a,08jun95,s_w written

# Register set info used by Gdb, format is:
#
# $gdbRegs(<cpuFamily>,numRegs) == <number of gdb registers>
# $gdbRegs(<cpuFamily>,regSetSizes) == {<IU reg set size>, <FPU ...>, ...}
# $gdbRegs(<cpuFamily>,<gdbRegNum>) == {<offset> <regSize> <regSet> <regName>}
#
# where <offset> is the offset in the register set data as returned by the
# target agent, <regSize> is the size in bytes, <regSet> is the register set
# that the register is stored in and <regName> is the gdb register name.

global gdbRegs

# sparc family register info
set gdbRegs(sparc,numRegs)	72
set gdbRegs(sparc,regSetSizes)	{160 136 0 0 0 0}

# global registers g0-g7 
set gdbRegs(sparc,0)  { 0x20 4 IU g0 }
set gdbRegs(sparc,1)  { 0x24 4 IU g1 }
set gdbRegs(sparc,2)  { 0x28 4 IU g2 }

set gdbRegs(sparc,3)  { 0x2c 4 IU g3 }
set gdbRegs(sparc,4)  { 0x30 4 IU g4 }
set gdbRegs(sparc,5)  { 0x34 4 IU g5 }
set gdbRegs(sparc,6)  { 0x38 4 IU g6 }
set gdbRegs(sparc,7)  { 0x3c 4 IU g7 }

# out registers o0-o5 sp and o7
set gdbRegs(sparc,8)  { 0x40 4 IU o0 }
set gdbRegs(sparc,9)  { 0x44 4 IU o1 }
set gdbRegs(sparc,10) { 0x48 4 IU o2 }
set gdbRegs(sparc,11) { 0x4c 4 IU o3 }
set gdbRegs(sparc,12) { 0x50 4 IU o4 }
set gdbRegs(sparc,13) { 0x54 4 IU o5 }
set gdbRegs(sparc,14) { 0x58 4 IU sp }
set gdbRegs(sparc,15) { 0x5c 4 IU o7 } 

# local registers l0-l7 
set gdbRegs(sparc,16) { 0x60 4 IU l0 }
set gdbRegs(sparc,17) { 0x64 4 IU l1 }
set gdbRegs(sparc,18) { 0x68 4 IU l2 } 
set gdbRegs(sparc,19) { 0x6c 4 IU l3 }
set gdbRegs(sparc,20) { 0x70 4 IU l4 }
set gdbRegs(sparc,21) { 0x74 4 IU l5 }
set gdbRegs(sparc,22) { 0x78 4 IU l6 }
set gdbRegs(sparc,23) { 0x7c 4 IU l7 }

# in registers i0-i8 
set gdbRegs(sparc,24) { 0x80 4 IU i0 }
set gdbRegs(sparc,25) { 0x84 4 IU i1 }
set gdbRegs(sparc,26) { 0x88 4 IU i2 }
set gdbRegs(sparc,27) { 0x8c 4 IU i3 }
set gdbRegs(sparc,28) { 0x90 4 IU i4 }
set gdbRegs(sparc,29) { 0x94 4 IU i5 }
set gdbRegs(sparc,30) { 0x98 4 IU i6 }
set gdbRegs(sparc,31) { 0x9c 4 IU i7 }

# floating point registers f0-f31 
set gdbRegs(sparc,32) { 0x00 4 FPU f0 } 
set gdbRegs(sparc,33) { 0x04 4 FPU f1 } 
set gdbRegs(sparc,34) { 0x08 4 FPU f2 }
set gdbRegs(sparc,35) { 0x0c 4 FPU f3 }
set gdbRegs(sparc,36) { 0x10 4 FPU f4 }
set gdbRegs(sparc,37) { 0x14 4 FPU f5 }
set gdbRegs(sparc,38) { 0x18 4 FPU f6 }
set gdbRegs(sparc,39) { 0x1c 4 FPU f7 }
set gdbRegs(sparc,40) { 0x20 4 FPU f8 }
set gdbRegs(sparc,41) { 0x24 4 FPU f9 }
set gdbRegs(sparc,42) { 0x28 4 FPU f10 }
set gdbRegs(sparc,43) { 0x2c 4 FPU f11 }
set gdbRegs(sparc,44) { 0x30 4 FPU f12 }
set gdbRegs(sparc,45) { 0x34 4 FPU f13 }
set gdbRegs(sparc,46) { 0x38 4 FPU f14 }
set gdbRegs(sparc,47) { 0x3c 4 FPU f15 }
set gdbRegs(sparc,48) { 0x40 4 FPU f16 }
set gdbRegs(sparc,49) { 0x44 4 FPU f17 }
set gdbRegs(sparc,50) { 0x48 4 FPU f18 }
set gdbRegs(sparc,51) { 0x4c 4 FPU f19 }
set gdbRegs(sparc,52) { 0x50 4 FPU f20 }
set gdbRegs(sparc,53) { 0x54 4 FPU f21 }
set gdbRegs(sparc,54) { 0x58 4 FPU f22 }
set gdbRegs(sparc,55) { 0x5c 4 FPU f23 }
set gdbRegs(sparc,56) { 0x60 4 FPU f24 }
set gdbRegs(sparc,57) { 0x64 4 FPU f25 }
set gdbRegs(sparc,58) { 0x68 4 FPU f26 }
set gdbRegs(sparc,59) { 0x6c 4 FPU f27 }
set gdbRegs(sparc,60) { 0x70 4 FPU f28 }
set gdbRegs(sparc,61) { 0x74 4 FPU f29 }
set gdbRegs(sparc,62) { 0x78 4 FPU f30 }
set gdbRegs(sparc,63) { 0x7c 4 FPU f31 }

set gdbRegs(sparc,64) { 0x14 4 IU y    }
set gdbRegs(sparc,65) { 0x08 4 IU psr  } 
set gdbRegs(sparc,66) { 0x0c 4 IU wim  }
set gdbRegs(sparc,67) { 0x10 4 IU tbr  }
set gdbRegs(sparc,68) { 0x00 4 IU pc   }
set gdbRegs(sparc,69) { 0x04 4 IU npc  }
set gdbRegs(sparc,70) { 0x84 4 FPU fpsr }
# "cpsr" register not available from VxWorks 
set gdbRegs(sparc,71) { -1  -1 FPU cpsr  }

# m68k family register info

set gdbRegs(m68k,numRegs)	29
set gdbRegs(m68k,regSetSizes)	{72 108 0 0 0 0}

# data registers
set gdbRegs(m68k,0)  {0x00 4 IU d0}
set gdbRegs(m68k,1)  {0x04 4 IU d1}
set gdbRegs(m68k,2)  {0x08 4 IU d2}
set gdbRegs(m68k,3)  {0x0c 4 IU d3}
set gdbRegs(m68k,4)  {0x10 4 IU d4}
set gdbRegs(m68k,5)  {0x14 4 IU d5}
set gdbRegs(m68k,6)  {0x18 4 IU d6}
set gdbRegs(m68k,7)  {0x1c 4 IU d7}

# address registers
set gdbRegs(m68k,8)  {0x20 4 IU a0}
set gdbRegs(m68k,9)  {0x24 4 IU a1}
set gdbRegs(m68k,10) {0x28 4 IU a2}
set gdbRegs(m68k,11) {0x2c 4 IU a3}
set gdbRegs(m68k,12) {0x30 4 IU a4}
set gdbRegs(m68k,13) {0x34 4 IU a5}

set gdbRegs(m68k,14) {0x38 4 IU fp}
set gdbRegs(m68k,15) {0x3c 4 IU sp}

# Between sp and ps there are 2 bytes of padding which are included
# here as part of ps. This because gdb treats the ps as 4 bytes
	
# "ps" also known as "sr" status register to VxWorks
set gdbRegs(m68k,16) {0x40 4 IU ps}    
set gdbRegs(m68k,17) {0x44 4 IU pc}

# floating point registers
set gdbRegs(m68k,18) {0x0c 12 FPU fp0}
set gdbRegs(m68k,19) {0x18 12 FPU fp1}
set gdbRegs(m68k,20) {0x24 12 FPU fp2}
set gdbRegs(m68k,21) {0x30 12 FPU fp3}
set gdbRegs(m68k,22) {0x3c 12 FPU fp4}
set gdbRegs(m68k,23) {0x48 12 FPU fp5}
set gdbRegs(m68k,24) {0x54 12 FPU fp6}
set gdbRegs(m68k,25) {0x60 12 FPU fp7}
set gdbRegs(m68k,26) {0x00 4 FPU fpcontrol}
set gdbRegs(m68k,27) {0x04 4 FPU fpstatus }
set gdbRegs(m68k,28) {0x08 4 FPU fpiaddr  }


# i86 family register info

set gdbRegs(i86,numRegs)	21
set gdbRegs(i86,regSetSizes)	{40 92 0 0 0 0}

# data registers 
set gdbRegs(i86,0)  {0x1c 4 IU eax}
set gdbRegs(i86,1)  {0x18 4 IU ecx}
set gdbRegs(i86,2)  {0x14 4 IU edx}
set gdbRegs(i86,3)  {0x10 4 IU ebx}
set gdbRegs(i86,4)  {0x0c 4 IU esp}
set gdbRegs(i86,5)  {0x08 4 IU ebp}
set gdbRegs(i86,6)  {0x04 4 IU esi}
set gdbRegs(i86,7)  {0x00 4 IU edi}
set gdbRegs(i86,8)  {0x20 4 IU eflags}
set gdbRegs(i86,9)  {0x24 4 IU pc }

# floating point registers
set gdbRegs(i86,10) {0x0c 10 FPU st0}
set gdbRegs(i86,11) {0x16 10 FPU st1}
set gdbRegs(i86,12) {0x20 10 FPU st2}
set gdbRegs(i86,13) {0x2a 10 FPU st3}
set gdbRegs(i86,14) {0x34 10 FPU st4}
set gdbRegs(i86,15) {0x3e 10 FPU st5}
set gdbRegs(i86,16) {0x48 10 FPU st6}
set gdbRegs(i86,17) {0x52 10 FPU st7}
set gdbRegs(i86,18) {0x00 4  FPU fpc}
set gdbRegs(i86,19) {0x04 4  FPU fps}
set gdbRegs(i86,20) {0x08 4  FPU fpt}

set gdbRegs(i960,numRegs)	39
set gdbRegs(i960,regSetSizes)	{140 64 0 0 0 0}

# data registers
set gdbRegs(i960,0)  {0x00 4 IU pfp}
set gdbRegs(i960,1)  {0x04 4 IU sp }
set gdbRegs(i960,2)  {0x08 4 IU rip}
set gdbRegs(i960,3)  {0x0c 4 IU r3 }
set gdbRegs(i960,4)  {0x10 4 IU r4 }
set gdbRegs(i960,5)  {0x14 4 IU r5 }
set gdbRegs(i960,6)  {0x18 4 IU r6 }
set gdbRegs(i960,7)  {0x1c 4 IU r7 }
set gdbRegs(i960,8)  {0x20 4 IU r8 }
set gdbRegs(i960,9)  {0x24 4 IU r9 }
set gdbRegs(i960,10) {0x28 4 IU r10}
set gdbRegs(i960,11) {0x2c 4 IU r11}
set gdbRegs(i960,12) {0x30 4 IU r12}
set gdbRegs(i960,13) {0x34 4 IU r13}
set gdbRegs(i960,14) {0x38 4 IU r14}
set gdbRegs(i960,15) {0x3c 4 IU r15}
set gdbRegs(i960,16) {0x40 4 IU g0 }
set gdbRegs(i960,17) {0x44 4 IU g1 }
set gdbRegs(i960,18) {0x48 4 IU g2 }
set gdbRegs(i960,19) {0x4c 4 IU g3 }
set gdbRegs(i960,20) {0x50 4 IU g4 }
set gdbRegs(i960,21) {0x54 4 IU g5 }
set gdbRegs(i960,22) {0x58 4 IU g6 }
set gdbRegs(i960,23) {0x5c 4 IU g7 }
set gdbRegs(i960,24) {0x60 4 IU g8 }
set gdbRegs(i960,25) {0x64 4 IU g9 }
set gdbRegs(i960,26) {0x68 4 IU g10}
set gdbRegs(i960,27) {0x6c 4 IU g11}
set gdbRegs(i960,28) {0x70 4 IU g12}
set gdbRegs(i960,29) {0x74 4 IU g13}
set gdbRegs(i960,30) {0x78 4 IU g14}
set gdbRegs(i960,31) {0x7c 4 IU fp }
set gdbRegs(i960,32) {0x80 4 IU pcw}
set gdbRegs(i960,33) {0x84 4 IU ac }
set gdbRegs(i960,34) {0x88 4 IU tc }

# floating point registers
set gdbRegs(i960,35) {0x00 10 FPU fp0}
set gdbRegs(i960,36) {0x10 10 FPU fp1}
set gdbRegs(i960,37) {0x20 10 FPU fp2}
set gdbRegs(i960,38) {0x30 10 FPU fp3}


##############################################################################
#
# gdbNumRegsGet - get number of gdb registers for current target
#
# RETURNS
#   register count or raises error

proc gdbNumRegsGet {family} {
    global gdbRegs

    if {! [info exists gdbRegs($family,numRegs)]} {
	error "No register info availble for CPU family $family"
    } else {
	return $gdbRegs($family,numRegs)
    }    
}

##############################################################################
#
# gdbRegSetSizesGet - get register set sizes for current target
#
# RETURNS
#   register set sizes string

proc gdbRegSetSizesGet {family} {
    global gdbRegs

    if {! [info exists gdbRegs($family,regSetSizes)]} {
	error "No register set size info availble for CPU family $cpuFamily"
    } else {
	return $gdbRegs($family,regSetSizes)
    }    
}

##############################################################################
#
# gdbRegInfoGet - get gdb register info for given CPU family
#
# RETURNS
#   register info string

proc gdbRegsInfoGet { family regNum } {
    global gdbRegs

    if {! [info exists gdbRegs($family,$regNum)]} {
	error "No register info for CPU family $family register $regNum"
    } else {
	set regInfo $gdbRegs($family,$regNum)
	return [format "%d %d %d" [lindex $regInfo 0] [lindex $regInfo 1] [wtxEnumFromString REG_SET_TYPE [lindex $regInfo 2]]]
    }    
}

##############################################################################
#
# gdbCheckConfiguration - check compatibility of gdb with current target
#
# RETURNS
#   raises error on incompatibility

proc gdbCheckConfiguration { gdbArch } {
    global cpuFamily
    global asmClass
    global __wtxCpuType

    set family $asmClass($cpuFamily($__wtxCpuType))
    if {$gdbArch != $family} {
	error "Gdb configuration ($gdbArch) incomaptible with target ($family)"
    }

}

##############################################################################
#
# gdbMakeCommandName - builds a command name from a Tcl proc name
#
# This routines takes a Tcl proc name and converts it to a form that gdb
# can use for a command name. Leading underscores are trimmed and upper
# case letters are lowercased with a preceeding `-' ie. "wtxTargetName"
# becomes "wtx-target-name" and "__foo" becomes "foo"
#
# SYNOPSIS:
#   gdbMakeCommandName name
#
# RETURNS
#   name string

proc gdbMakeCommandName { name } {
    regsub -all {([A-Z])} $name {-\1} newName
    return [string tolower [string trimleft $newName _]]
}

##############################################################################
#
# demangle - no-op name demangler
#
# This routine does a no-op demangle and is a temporary stand-in for a real
# demangle command in gdb.  It overcomes the problem of using shell Tcl procs
# when the windsh demangle command isn't present.
#
# SYNOPSIS:
#   demangle style name
#
# RETURNS:
#   name

proc demangle { style name } {
    return $name
}


##############################################################################
#
# gdbWtxCommandsInit - add wtx Tcl commands to gdb command list
#
# This routine adds all the Tcl commands starting with "wtx" to the 
# gdb command list using gdbMakeCommandName to form gdb friendly names
#
# RETURNS: 
#   N/A

proc gdbWtxCommandsInit {} {
    foreach cmd [info command wtx*] {
	gdb tclproc [gdbMakeCommandName $cmd] $cmd
    }
}


##############################################################################
#
# gdbWtxErrorHandler - WTX error handler for gdb tcl calls.
#
# This routine is used to override the error handler set by the shell
#
# RETURNS: N/A

proc gdbWtxErrorHandler {hwtx cmd err tag} {

    # Just resubmit the error so it is reported by gdb.
    error $err

}


##############################################################################
#
# gdbWindShellInit - initialize windShell Tcl module
#
# This routine sets up the windShell module and makes the windShell commands
# available from gdb
#
# RETURNS:
#   N/A

proc gdbWindShellInit {} {
    global shellProcList

    # Load shell if needed
    if { [info command shellInit] == "" } {
	uplevel #0 source [wtxPath host resource tcl]shell.tcl
    }
    
    # Initialize shell
    shellInit

    # Override its setting of the error handler that is for windsh use only.
    wtxErrorHandler [wtxHandle] gdbWtxErrorHandler

    foreach cmd $shellProcList {
	set shellCmd [shellName $cmd]
	gdb tclproc wind-[gdbMakeCommandName $shellCmd] $shellCmd
    }
}


##############################################################################
#
# gdbWindTaskListGet - get a list of tasks from Wind kernel
#
# This routine fetches a list of tasks running from the Wind kernel for
# use by Gdb.  gdbWindTaskListGet is called indirectly by gdb using
# gdbTaskListGet.
#
# RETURNS:
#    List of tasks and task info

proc gdbWindTaskListGet {} {
    global offset

    return [wtxGopherEval "
    [shSymAddr activeQHead] * 
    {
	< 
	-$offset(WIND_TCB,activeNode) !
	<+$offset(WIND_TCB,name) *$>
	<+$offset(WIND_TCB,pStackBase) @>
	<+$offset(WIND_TCB,pStackEnd) @>
	<+$offset(WIND_TCB,regs) !>
	> 
	*
    }"]
}


##############################################################################
#
# gdbTaskListGet - get a list of tasks form the target kernel
#
# This routine fetches a list of tasks running on the target for use
# by gdb.  It does so by calling a kernel specific Tcl routine - currently
# only the Wind kernel is supported so the call is hard coded.
#
# RETURNS:
#    List of tasks and task info.

proc gdbTaskListGet {} {
    # FIXME: Should vector through a pointer based on kernel type
    return [gdbWindTaskListGet]
}

##############################################################################
#
# gdbTaskNameToId - convert a task name to a task ID
#
# This routine takes a task name and converts it to a task ID
#
# RETURNS:
#    The task ID or 0 on error

proc gdbTaskNameToId { name } {
    # FIXME: Should vector through a pointer based on kernel type

    # Only try to convert names starting with "t"
    if { [string range $name 0 0] != "t" } {
	return 0
    } else {
	return [taskNameToId $name]
    }
}


##############################################################################
#
# gdbCpuTypeNameGet - converts a CPU type number to a name
#
# RETURNS:
#   A string representation of the CPU type eg "MC68020"

proc gdbCpuTypeNameGet { cpuTypeNum } {
    global cpuType

    # FIXME: Should vector through a pointer based on kernel type

    return $cpuType($cpuTypeNum)
}


##############################################################################
#
# gdbSystemRun - do a run command in system mode
#
# This routine spawns of a new task for gdb given the entry point and the
# arguments. It relies on Wind kernel facilites to work in both system and
# task mode.
#
# RETURNS:
#   N/A

proc gdbSystemRun { entryName {arg0 0} {arg1 0} {arg2 0} {arg3 0} {arg4 0} } {
    # FIXME: Should vector through a pointer based on kernel type
    
    # 
    # Note: there is currently no way to tell what mode the agent is in
    # to we first try to create a new task using task mode facilites and
    # if that fails try system mode.
    #

    gdb call excJobAdd (taskSpawn,"tDbgSys",100,0,20000,$entryName,$arg0,$arg1,$arg2,$arg3,$arg4)
}


##############################################################################
#
# gdbTargetFileOpen - open a file on the target
#
# This routine uses shFuncCall to open a file on the target system
#
# RETURNS:
#   The fd number of the new file

proc gdbTargetFileOpen { filename mode } {
    
    set openFunc [shSymAddr "open"]
    set block [memBlockCreate -string $filename]
    set pFilename [wtxMemAlloc [lindex [memBlockInfo $block] 0]]

    wtxMemWrite $block $pFilename
    set fileFd [shFuncCall -int $openFunc $pFilename $mode 0 0 0 0 0 0 0 0]
    wtxMemFree $pFilename

    memBlockDelete $block

    if {$fileFd == -1} {
	error "Could not open target file $filename"
    }
    
    return $fileFd
}

##############################################################################
#
# gdbIORedirect - performs I/O redirections on a task or globally
#
# RETURNS:
#   N/A

proc gdbIORedirect { inFile outFile {taskId -1} } {

    global vioOut
    global vioIn
    global fdIn
    global fdOut
    global hostFdIn
    global hostFdOut

    # Figure out if out and in are the same file
    if {[string compare $inFile $outFile] == 0} {
	set shared 1
    } else {
	set shared 0
    }

    # If both input and output are "-" then nothing to do
    if { $shared && ($inFile == "-") } {
	return
    }

    # Ignore any previous redirections
    set vioIn  0
    set vioOut 0
    set fdIn   -1
    set fdOut  -1
    set hostFdOut -1
    set hostFdIn  -1

    # Open host based files for the input and output, or a single r/w file,
    # with a VIO channel redirected to/from the host file. Only do this if
    # the file is host and not target based.  Target names are prefixed with
    # '@' as in windShell.

    if { $shared && ([string index $inFile 0] != "@") } {

	# Use O_RDWR | O_CREAT, mode is rwrwrw
	set vioIn [wtxVioChanGet]
	set hostFdIn [wtxOpen -channel $vioIn $inFile 0x0202 0666]

    } else {
	
	if { $inFile != "-" && ([string index $inFile 0] != "@") } {
	    # Use O_RDONLY, mode is ignored
	    set vioIn [wtxVioChanGet]
	    set hostFdIn [wtxOpen -channel $vioIn $inFile 0x0 0]
	}
	if { $outFile != "-" && ([string index $outFile 0] != "@") } {
	    # Use O_WRONLY | O_CREAT, mode is rwrwrw
	    set vioOut [wtxVioChanGet]
	    set hostFdOut [wtxOpen -channel $vioOut $outFile 0x0201 0666]
	}
	
    }
    
    # Open files on the target now to redirect the input and output to.
    # If the input/output name starts with a '@' then use that, otherwise
    # use the appropriate VIO channel opened above.

    if { $shared } {

	if { [string index $inFile 0] != "@" } {
	    set file [format "/vio/%d" $vioIn]
	} else {
	    # Trim '@' prefix
	    set file [string range $inFile 1 end]
	}

	if {[regexp {^[0-9]+$} $file]} {
	    set fdIn $file
	} else {
	    set fdIn [gdbTargetFileOpen $file 0x02]
	}
	set fdOut $fdIn

    } else {

	if { $inFile != "-" } {

	    if { [string index $inFile 0] != "@" } {
		set file [format "/vio/%d" $vioIn]
	    } else {
		# Trim '@' prefix
		set file [string range $inFile 1 end]	
	    }
	    
	    if {[regexp {^[0-9]+$} $file]} {
		set fdIn $file
	    } else {
		set fdIn [gdbTargetFileOpen $file 0x0]
	    }
		
	}
	if { $outFile != "-" } {

	    if { [string index $outFile 0] != "@" } {
		set file [format "/vio/%d" $vioOut]
	    } else {
		# Trim '@' prefix
		set file [string range $outFile 1 end]	
	    }
	    
	    if {[regexp {^[0-9]+$} $file]} {
		set fdOut $file
	    } else {
		set fdOut [gdbTargetFileOpen $file 0x1]
	    }

	}

    }

    # Now call a function to redirect the output 

    if { $taskId == -1 } {

	set redirFunc [shSymAddr "ioGlobalStdSet"]

	if {$fdIn != -1} {
	    shFuncCall -int $redirFunc 0 $fdIn  0 0 0 0 0 0 0 0
	}
	if {$fdOut != -1} {
	    shFuncCall -int $redirFunc 1 $fdOut 0 0 0 0 0 0 0 0
	    shFuncCall -int $redirFunc 2 $fdOut 0 0 0 0 0 0 0 0
	}

    } else {

	set redirFunc [shSymAddr "ioTaskStdSet"]
	
	if {$fdIn != -1} {
	    shFuncCall -int $redirFunc $taskId 0 $fdIn  0 0 0 0 0 0 0
	}
	if {$fdOut != -1} {
	    shFuncCall -int $redirFunc $taskId 1 $fdOut 0 0 0 0 0 0 0
	    shFuncCall -int $redirFunc $taskId 2 $fdOut 0 0 0 0 0 0 0
	}
    }	

    # If there wasn't a file opened on the target side then set fds
    # back to -1 so that gdbIOClose wont try to close them

    if {[regexp {^@[0-9]+$} $inFile]} {
	set fdIn -1
    }
    if {[regexp {^@[0-9]+$} $outFile]} {
	set fdOut -1
    }
    
    return
}

##############################################################################
#
# gdbIOClose - closes file redirections by previous gdbIORedirect
#
# RETURNS:
#   0 on success, -1 on failure

proc gdbIOClose {} {

    global vioOut
    global vioIn
    global hostFdIn
    global hostFdOut
    global fdIn
    global fdOut

    set closeFunc [shSymAddr close]

    # Close up open target file descriptors (ignoring errors). However,
    # don't close fd's 0-2 because these are the global fd's.

    if {$fdIn != -1} {
	catch { shFuncCall -int $closeFunc $fdIn  0 0 0 0 0 0 0 0 0 }
	set fdIn  -1
    } 
	
    if { ($fdOut != -1) && ($fdOut != $fdIn) } {
	catch { shFuncCall -int $closeFunc $fdOut 0 0 0 0 0 0 0 0 0 }
	set fdOut -1
    }


    # Close any host based files and release reserved VIO channels
    if {$hostFdIn != -1} {
	catch { wtxClose $hostFdIn }
	set hostFdIn -1
	catch { wtxVioChanRelease $vioIn }
	set vioIn 0
    }
    
    if {$hostFdOut != -1} {
	catch { wtxClose $hostFdOut }
	set hostFdOut -1
	catch { wtxVioChanRelease $vioOut }
	set vioOut 0
    }

    return
}


##############################################################################
#
# gdbTargetTclInit - initialize target specific Tcl routines
#
# This routine is called by gdb everytime a new target is connected to
# It queries the target type and does appropriate initializations for it.
# Currently on the Wind kernel is supported.
#
# RETURNS:
#    N/A

proc gdbTargetTclInit {} {

    set tsInfo [wtxTsInfoGet]
    set rtType [lindex [lindex $tsInfo 3] 0]

    # First check that the runtime type is supported
    if {$rtType == 1} {

	# Got a VxWorks wind kernel so suck in the shell stuff
	gdbWindShellInit

    } else {

	error "gdbTargetTclInit: unsupported runtime type $rtType"

    }

    return
}


###############################################################################
#
# gdbWtxErrorHook - a hook called when a WTX error occurs
#
# This proc is called when an error is caused by a WTX request called by
# gdb itself (rather than in Tcl code). If this proc return a non-error,
# non-empty result then it is displayed as a warning string and the user
# is given the opportunity to detach from the current target. If the yes
# answers in the affirmative then all targets are popped and the current
# command is aborted with error. If no then the WTX error is handled as it
# would normal be by gdb.
#
# RETURNS: See description

proc gdbWtxErrorHook { errMsg } {
    return
}

###############################################################################
#
# gdbTaskAttachHook - a hook called just before attaching to a task
#
# This routine is called just before gdb attaches to a task
# If it raises an error the attach is aborted by gdb with the error raised. 
# It it returns the value "0" gdb doesn't take any further action. 
# It if returns any value that is not "0" then gdb will continue with
# the attach itself.
#
# RETURNS: See description

proc gdbTaskAttachHook { taskId } {
    global tcbOptionBit
    global gdbAttachAny

    if { ! $gdbAttachAny } {
	set options [lindex [taskInfoGetVerbose $taskId] 11]
    
	if {($options & $tcbOptionBit(VX_UNBREAKABLE))} {
	    error "cannot attach to task with VX_UNBREAKABLE option set"
	}
    }

    # Hunky dory
    return
}


###############################################################################
#
# gdbSystemAttachHook - a hook called just before an attach command is executed
#
# This routine is called just before gdb attaches to the system.
# If it raises an error the attach is aborted by gdb with the error raised. 
# It it returns the value "0" gdb doesn't take any further action. 
# It if returns any value that is not "0" then gdb will continue with
# the attach itself.
#
# RETURNS: See description

proc gdbSystemAttachHook {} {
    return
}

###############################################################################
#
# gdbTargetOpenHook - a hook called when target is attached to
#
# This proc is called just after the wtxToolAttach call in gdb, at a point
# where any failure will result in gdbTargetCloseHook being called. Any
# error in this routine wall cause the target open to fail (and call the
# close hook).
#
# RETURNS:
#   N/A

proc gdbTargetOpenHook {} {
    return
}

###############################################################################
#
# gdbTargetCloseHook - a hook called just before the target is detached from
#
# This proc is called after gdb has cleaned up all its events for a target
# but before the actual wtxToolDetach call so the current WTX handle is
# still valid and connected.  Any errors in this routine are printed as
# warnings but otherwise ignored.
#
# RETURNS:
#   N/A

proc gdbTargetCloseHook {} {
    return
}	

###############################################################################
#
# gdbTaskOpenHook - a hook called when a task is attached too
#
# This routine is called after gdb has successfully attach to a task
# It is passed the task ID as an argument (in hex format)
#
# RETURNS:
#   N/A

proc gdbTaskOpenHook {taskId} {
    return
}


###############################################################################
#
# gdbTaskCloseHook - a hook called when a task is detached from
#
# This routine is called when gdb detaches from a task, after all eventpoints
# are removed but while the task is still suspended. It is passed the task 
# ID as an argument (in hex format)
#
# RETURNS:
#   N/A

proc gdbTaskCloseHook {taskId} {
    return
}


###############################################################################
#
# gdbSystemOpenHook - a hook called when the system is attached too
#
# This routine is called after gdb has successfully attach to the target
# agent in system mode (when the system should be suspended in external
# mode).
#
# RETURNS:
#   N/A

proc gdbSystemOpenHook {} {
    return
}

###############################################################################
#
# gdbSystemCloseHook - a hook called when the system is dettached from
#
# This routine is called after gdb has successfully detached from the
# agent in system mode (when it should be back in task mode but with the
# system still suspended).
#
# RETURNS:
#   N/A

proc gdbSystemCloseHook {} {
    return
}


###############################################################################
#
# gdbTaskKillHook - a hook called just before a task is killed
#
# This routine is called just before gdb does a task kill.  If it raises
# an error the kill is aborted by gdb with the error raised. It it returns
# the value "0" gdb doesn't take any further action. It if returns any value
# that is not "0" then gdb will continue with the kill itself.
#
# RETURNS:
#   N/A

proc gdbTaskKillHook { taskId } {

    global offset

    # Look at the safeCnt value in the TCB
    set result [wtxGopherEval "$taskId <+$offset(WIND_TCB,safeCnt) @>"]

    if {$result > 0} {
	error "cannot kill a task that is safe from deletion"
    } else {
	# Let it rip!
	return
    }
}


###############################################################################
#
# gdbSystemKillHook - a hook called just before the system is killed
#
# This routine is called just before gdb does a kill in system mode.
# If it raises an error the kill is aborted by gdb with the error raised. 
# It it returns the value "0" gdb doesn't take any further action. 
# It if returns any value that is not "0" then gdb will continue with
# the kill itself.
#
# RETURNS: See description

proc gdbSystemKillHook {} {
    return
}


###############################################################################
#
# gdbEventGetAndDispatch - get an event from the target server and dispatch
#
# This routine gets one event from the target server and then dispatches
# it to any registered event handler procs.  The event string is then
# passed back to the caller for any further processing.
#
# RETURNS: event string or empty string
#

proc gdbEventGetAndDispatch {} {

    global gdbEventPollTime
    global gdbEventPollRetry

    # Get the event (a simple poll)
    set event [wtxEventPoll $gdbEventPollTime $gdbEventPollRetry]

    if {$event == ""} {

	# No event, just return nothing
	return

    } else {

	# An event was received - now try to process it
	if [catch {gdbEventDispatch $event} result] {
	    #puts stdout "Event dispatch error: $result"
	    return $event
	} else {
	    return $result
	}

    }

}

###############################################################################
#
# gdbEventDispatch - dispatch events received by gdb
#
# This routine dispatches all events received by gdb to an event specific
# Tcl event proc.  If no event proc has been defined then a default event
# proc is called which in most cases should just return the event string
# unchanged. The result returned by the event proc called is returned to
# gdb and if not further action by gdb is required the result should be
# an empty string, otherwise the event proc may rewrite or synthesis a 
# new event string as it desires.  Gdb will then carry out its default
# handling of events using the returned event string.
#
# The global array gdbEventProcs() defines the name of the event proc
# to call on each event type, if none is found gdbEventProcDefault is
# called.
#
# RETURNS:
#    Event string for gdb to process

proc gdbEventDispatch {event} {
    global gdbEventProcs
    global gdbEventShowAll

    set result $event

    if {$gdbEventShowAll} { 
	puts stdout "$event" 
    }
    
    set eventType [lindex $event 0]

    if [info exists gdbEventProcs($eventType)] {
	return [$gdbEventProcs($eventType) $event]
    } else {
	# The default handler
	return [$gdbEventProcs(DEFAULT) $event]
    }

}

##############################################################################
#
# gdbEventProcDefault - default handler for events
#
# RETURNS: event string unchanged

proc gdbEventProcDefault { event } {
    return $event
}


##############################################################################
#
# gdbEventProcVioWrite - handle VIO write events
#
# RETURNS: nothing

proc gdbEventProcVioWrite { event } {

    set vioChan [lindex $event 1]
    set mblk [lindex $event 2]
    if {$mblk != ""} {
	# The data of the VIO is in the memory block returned with the
	# event.  Dump that block to stdout and free it.
	memBlockWriteFile $mblk -
	memBlockDelete $mblk
    }

    # Gobble the event
    return
}


##############################################################################
#
# gdbEventProcLoad - handle OBJ_LOADED events
#
# RETURNS: nothing

proc gdbEventProcLoad { event } {
    global gdbAutoLoad

    set moduleName [lindex $event 2]

    if {$gdbAutoLoad} {
	puts stdout "Auto-loading object module: $moduleName"
	if [catch {gdb add-symbol-file $moduleName} result] {
	    puts "Failed."
	}
    } else {
	puts stdout "Another tool has loaded object module: $moduleName (ignored)"
    }

    return
}

##############################################################################
#
# gdbEventProcUnload - handle OBJ_UNLOADED events
#
# RETURNS: nothing

proc gdbEventProcUnload { event } {
    global gdbAutoUnload

    set moduleName [lindex $event 2]

    if {$gdbAutoUnload} {
	puts stdout "Auto-unloading object module: $moduleName"
	if [catch {gdb unload $moduleName} result] {
	    puts "Failed."
	}
    } else {
	puts stdout "Another tool has unloaded object module: $moduleName (ignored)"
    }

    return
}



##############################################################################
#
# Initialization starts here. Gdb loads this module once the first
# time a target is connected to.
#
##############################################################################

# Set up global variables for I/O redirection
set vioIn  0
set vioOut 0
set fdIn   -1
set fdOut  -1
set hostFdOut -1
set hostFdIn  -1

set gdbEventProcs(DEFAULT)	gdbEventProcDefault
set gdbEventProcs(VIO_WRITE)	gdbEventProcVioWrite
set gdbEventProcs(OBJ_LOADED)	gdbEventProcLoad
set gdbEventProcs(OBJ_UNLOADED) gdbEventProcUnload

set gdbEventPollTime		100
set gdbEventPollRetry		2
set gdbEventShowAll		0

set gdbAttachAny	0

set gdbAutoLoad		1
set gdbAutoUnload	1

# Add in WTX command bindings, shell bindings are done on first target
# connect (when the shell Tcl is first initialized)

gdbWtxCommandsInit

# Source the Tcl initialization file in the %WIND_BASE% directory if any

if {![catch {file exists $env(WIND_BASE)/.wind/gdb.tcl} result] && $result} {
    uplevel #0 source $env(WIND_BASE)/.wind/gdb.tcl
}

# Read in users gdb.tcl file if available

if [info exists env(HOME)] {
    if [file exists $env(HOME)/.wind/gdb.tcl] {
	uplevel #0 source $env(HOME)/.wind/gdb.tcl
    }
}

