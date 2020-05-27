
#puts "sourcing 01rdbdebugger.win32.tcl"

rename getTargetServerArch getTargetServerArch.orig

########################################################################
#
# getTargetServerArch -
#
proc getTargetServerArch {ts} {
    global cpuFamily
    global rdbArch

    set rdbDebug [regexp "(.*)@rdb" $ts match target]

    if {$rdbDebug == 1} {
	return "960w"
    } else {
	return [getTargetServerArch.orig $ts]
    }

 }

set rdbArch "960w"
set rdbTarget "gdb960w"
set rdbDebug 1
set targetServer $rdbTarget@rdb
# save the rdbTarget in the registry
appRegistryEntryDelete Target rdbTarget
appRegistryEntryWrite Target rdbTarget $rdbTarget

#puts "launching debugger"

debuggerLaunch $targetServer
