###############################################################################
# %WIND_BASE%/.wind/crosswind.tcl - Rdb Debugger setup routine.
#

rename architectureHandler architectureHandler.orig

proc architectureHandler {msg} {
    global gdbCommandTrace
    global architecture
    global targetArch
    global targetServer

    set rdbDebug [regexp {(.*)@rdb} $targetServer match ts]
    if {$rdbDebug == 1} {
	set architecture $msg
	if {$gdbCommandTrace} {
	    downtcl -noresult gdbCommandTrace ON
	} {
	    downtcl -noresult gdbCommandTrace OFF
	}

	# @@@ possibly check the consistency between $architecture and $targetArch!

	# Inform the debugger of any source search paths previously enterred
	# by the user in the "Source Search Path" dialog.
	setInitialSourceSearchPath
	#puts "Sending target command now."
	# Now attach to the target specified when the GUI debugger was instantiated.
	#downtcl -noresult gdb target vxw $ts

    } else {
	architectureHandler.orig $msg
    }
}

proc openBinaryDlgInit {pn} {
    global targetConnected

    controlValuesSet openBinaryDlg.pathName "$pn"
    controlValuesSet openBinaryDlg.loadSyms 1
    if {$targetConnected} {
	controlValuesSet openBinaryDlg.downLoad 1
    }
}

set targetConnected 0

proc addDir {path} {
    global readyCommandQueue

    set index [string last "/" $path]
    set dirname [string range $path 0 [expr $index - 1]]
    lappend readyCommandQueue "downtcl -noresult gdb dir $dirname"
}

proc openBinaryDlgOK {} {
    global targetConnected

    set pathName [controlValuesGet openBinaryDlg.pathName]
    set fc [controlValuesGet openBinaryDlg.loadSyms]
    if {$targetConnected} {
	set lc [controlValuesGet openBinaryDlg.downLoad]
    } else {
	set lc 0
    }
    #puts "pathName is: $pathName file cmd: $fc load cmd: $lc"
    windowClose openBinaryDlg
    beginWaitCursor
    if {$fc} {
	downtcl gdb file $pathName
	menuItemInsert -after {&View {&Status Bar}} { {&List Code} } FuncFileLister
    }
    if {$targetConnected && $lc} {
	downtcl gdb load $pathName
    }
    endWaitCursor
}

proc openBinaryLoadSymsCB {} {
    if {[controlValuesGet openBinaryDlg.loadSyms]} {
	controlValuesSet openBinaryDlg.loadSyms 0
    } else {
	controlValuesSet openBinaryDlg.loadSyms 1
    }
}

proc openBinaryDownLoadCB {} {
    if {[controlValuesGet openBinaryDlg.downLoad]} {
	controlValuesSet openBinaryDlg.downLoad 0
    } else {
	controlValuesSet openBinaryDlg.downLoad 1
    }
}

proc openBinary {} {
    #puts "Open Binary"
    #set pathName [join [fileDialogCreate -filefilters "Load modules *.out|*.o|*.*||"
    set pathName [join [fileDialogCreate -filemustexist -showreadonly -title "Open binary" ]]
    if { "$pathName" != "" } {
	global targetConnected

	set shortName [file tail $pathName]
	regsub -all {[\\]} "$pathName" "/" pathName
	if {$targetConnected} {
	    dialogCreate -name openBinaryDlg -title "Open: $shortName" \
		    -w 110 -h 45 -init "openBinaryDlgInit $pathName" -controls {
		{text -hidden -name pathName}
		{boolean -callback openBinaryLoadSymsCB -title {Load symbols}  -name loadSyms -x 5  -y 5  -w 70 -h 12 }
		{boolean -callback openBinaryDownLoadCB -title {Download code} -name downLoad -x 5  -y 25 -w 70 -h 12 }
		{button  -default -callback openBinaryDlgOK -title "OK"
		-name okButt                                   -x 80 -y 5  -w 25 -h 12}
		{button -callback {windowClose openBinaryDlg} -title "Cancel" -name
		cancelButt                                     -x 80 -y 25 -w 25 -h 12}
	    }
	} else {
	    dialogCreate -name openBinaryDlg -title "Open: $shortName" \
		    -w 110 -h 45 -init "openBinaryDlgInit $pathName" -controls {
		{text -hidden -name pathName}
		{boolean -callback openBinaryLoadSymsCB -title {Load symbols}  -name loadSyms -x 5  -y 5  -w 70 -h 12 }
		{button  -default -callback openBinaryDlgOK -title "OK"
		-name okButt                                   -x 80 -y 5  -w 25 -h 12}
		{button -callback {windowClose openBinaryDlg} -title "Cancel" -name
		cancelButt                                     -x 80 -y 25 -w 25 -h 12}
	    }
	}
    } else {
	#puts "No file selected."
    }
}

menuItemInsert -after {&File {&Open...	Ctrl+O}} { {Open &Binary} } openBinary

proc targetConnectSerialInit {} {
    controlValuesSet targetConnectSerialDlg.serialPort [list COM1 COM2 COM3 COM4]
    set sp [appRegistryEntryRead IntelGdb960 serialport]
    if {$sp == ""} {
	set sp "COM1"
    }
    controlSelectionSet targetConnectSerialDlg.serialPort -string $sp
    controlValuesSet targetConnectSerialDlg.serialBaud [list "115200" "57600" "38400" "19200" "9600" "4800" "2400" "1200"]
    set sb [appRegistryEntryRead IntelGdb960 serialbaud]
    if {$sb == ""} {
        set sb "115200"
    }
    controlSelectionSet targetConnectSerialDlg.serialBaud -string $sb
    controlValuesSet targetConnectSerialDlg.serialparallelport [list "<none>" "PRT1"]
    set spp [appRegistryEntryRead IntelGdb960 serialparallelport]
    if {$spp == ""} {
	set spp "<none>"
    }
    controlSelectionSet targetConnectSerialDlg.serialparallelport $spp
}

proc targetInfoInit {} {
    regsub -all {\	} [downtcl gdb info target] {   } rawti
    set tilist [split $rawti "\n"]
    controlValuesSet targetInfoDlg.targetinfo $tilist
}

proc targetInfo {} {
    dialogCreate -name targetInfoDlg -init targetInfoInit -title "Target Connected" \
	    -w 230 -h 220 -controls {
	{label -title "Target Information"                        -x  10 -y  10 -w 100 -h 15}
	{list -name targetinfo                                    -x  10 -y  25 -h 150 -w 210 }
	{button -callback {windowClose targetInfoDlg} -title "OK" -x 105 -y 180 }
    }
}

proc targetConnectSerialOK {} {
    set port [controlSelectionGet targetConnectSerialDlg.serialPort -string]
    set baud [controlSelectionGet targetConnectSerialDlg.serialBaud -string]
    set pport [controlSelectionGet targetConnectSerialDlg.serialparallelport -string]

    appRegistryEntryDelete IntelGdb960 serialport
    appRegistryEntryDelete IntelGdb960 serialbaud
    appRegistryEntryDelete IntelGdb960 serialparallelport

    appRegistryEntryWrite IntelGdb960 serialport         "$port"
    appRegistryEntryWrite IntelGdb960 serialbaud         "$baud"
    appRegistryEntryWrite IntelGdb960 serialparallelport "$pport"

    if {$pport == "<none>"} {
	set pport ""
    } else {
	set pport "-parallel $pport"
    }
    downtcl -noresult gdb target mon960 $port -baud $baud $pport
    #beginWaitCursor
    #set x [downtcl gdb target mon960 $port -baud $baud $pport]
    #endWaitCursor
    windowClose targetConnectSerialDlg
}

proc targetConnectSerial {} {
    #puts "Target Connect Serial"
    dialogCreate -name targetConnectSerialDlg -title "Target Connect - Serial" \
	    -w 170 -h 105 -init targetConnectSerialInit -controls {
	{label -title "Serial Port" -name lbl1              -x 10 -y 5  -w 40 -h 12 }
	{list  -name serialPort                             -x 10 -y 17 -w 50 -h 42 }
	{label -title "Baud Rate" -name lbl2                -x 10 -y 55 -w 40 -h 12 }
	{list  -name serialBaud                             -x 10 -y 67 -w 50 -h 42 }
	{label -title "Parallel (Download) Port" -name lbl3 -x 85 -y 55 -w 70 -h 24 }
	{list  -name serialparallelport                     -x 85 -y 75 -w 50 -h 30 }
	{button  -default -callback targetConnectSerialOK -title "OK"
	-name okButt -x 114 -y 7 -w 50 -h 14}
	{button -callback {windowClose targetConnectSerialDlg} -title "Cancel" -name
	cancelButt -x 114 -y 25 -w 50 -h 14}
    }
}

proc targetConnectPCIOK {} {
    set pcidev [controlSelectionGet targetConnectPCIDlg.pcidevices -string]

    appRegistryEntryDelete IntelGdb960 pcidevice

    appRegistryEntryWrite  IntelGdb960 pcidevice  "$pcidev"

    set bus       [lindex $pcidev 0]
    set devicen   [lindex $pcidev 1]
    set function  [lindex $pcidev 2]
    downtcl -noresult gdb target mon960 -pcib $bus $devicen $function
    windowClose targetConnectPCIDlg
}

proc targetConnectPCIInit {} {
    set pcidevices [list]
    set good_dev ""
    set r [downtcl gdbPciDevices]
    if {$r == "no pci devices\n"} {
	windowClose targetConnectPCIDlg
	messageBox -ok -exclamation "No PCI devices found."
    } else {
	foreach d $r {
	    set x [split $d " "]
	    set bus       [lindex $x 0]
	    set devicen   [lindex $x 1]
	    set function  [lindex $x 2]
	    set vendor_id [lindex $x 3]
	    set device_id [lindex $x 4]
	    if {$vendor_id == "113c"} {
		set type "CYCLONE"
	    } else {
		set type "unknown"
	    }
	    set str [format "%-6s %-6s %-8s %-6s %-6s %-6s" $bus $devicen $function $vendor_id $device_id $type]
	    lappend pcidevices "$str"
	    if {$type == "CYCLONE"} {
		set good_dev "$str"
	    }
	}
	controlValuesSet targetConnectPCIDlg.pcidevices $pcidevices
	if {"$good_dev" != ""} {
	    controlSelectionSet targetConnectPCIDlg.pcidevices -string "$good_dev"
	}
    }
}

proc targetConnectPCI {} {
    dialogCreate -name targetConnectPCIDlg -title "Target Connect - PCI" \
            -w 275 -h 100 -init targetConnectPCIInit -controls {
	{label -fixed -title "Bus    Device Function Vendor Device Device" -name lbl1 -x 12  -y  8 -w 205 -h 12}
	{label -fixed -title "Number Number Number   Ident. Ident. Type"   -name lbl2 -x 12  -y 16 -w 205 -h 12}
        {list  -fixed -name pcidevices                                                -x 10  -y 26 -w 255 -h 52}
	{button -default -callback targetConnectPCIOK -title "OK"                    -x 10  -y 79 -w 30  -h 14}
	{button -callback {windowClose targetConnectPCIDlg} -title "Cancel"           -x 185 -y 79 -w 30  -h 14}
    }
}

proc targetConnectTCPOK {} {
    set tcphn [controlValuesGet targetConnectTCPDlg.tcp_host_name   ]
    set tcppt [controlValuesGet targetConnectTCPDlg.tcp_port_number -string]

    appRegistryEntryDelete IntelGdb960 tcp_host_name
    appRegistryEntryDelete IntelGdb960 tcp_port_number


    appRegistryEntryWrite  IntelGdb960 tcp_host_name   "$tcphn"
    appRegistryEntryWrite  IntelGdb960 tcp_port_number "$tcppt"

    if {$tcphn == ""} {
        windowClose targetConnectTCPDlg
        messageBox -ok -exclamation "TCP Full HOST Name must be defined"
    }

    if {$tcppt == ""} {
        windowClose targetConnectTCPDlg
        messageBox -ok -exclamation "TCP PORT number must be defined"
    }

    downtcl -noresult gdb target mon960 -tcp $tcphn $tcppt

    windowClose targetConnectTCPDlg
}



proc targetConnectTCPInit {} {
    set tcphn [appRegistryEntryRead IntelGdb960 tcp_host_name]
    controlValuesSet targetConnectTCPDlg.tcp_host_name -string $tcphn

    set tcppt [appRegistryEntryRead IntelGdb960 tcp_port_number]
    controlValuesSet targetConnectTCPDlg.tcp_port_number -string $tcppt
}

proc targetConnectTCP_IP {} {
    dialogCreate -name targetConnectTCPDlg -title "Target Connect - TCP/IP" \
            -w 225 -h 80 -init targetConnectTCPInit -controls {
        {label -fixed -title "Full Host Name"  -name lbl1                    -x 12  -y  8 -w 190 -h 12}
        {text  -fixed -name tcp_host_name                                    -x 10  -y 16 -w 200 -h 12}
        {label -fixed -title "Port Number"     -name lbl2                    -x 12  -y 28 -w 190 -h 12}
        {text  -fixed -name tcp_port_number                                  -x 10  -y 36 -w 100 -h 12}
        {button -default -callback targetConnectTCPOK -title "OK"            -x 10  -y 56 -w 30  -h 14}
        {button -callback {windowClose targetConnectTCPDlg} -title "Cancel"  -x 185 -y 56 -w 30  -h 14}
    }
}

menuItemInsert -after {&File {Open   &Binary}  }   { {Target &Connect} &Serial } targetConnectSerial
menuItemInsert -after {&File {Target &Connect} }   { &PCI }     targetConnectPCI
menuItemInsert -after {&File {Target &Connect} }   { &TCP/IP }  targetConnectTCP_IP

########################################################################

proc FuncFileListerListCB {} {
    set funclist [controlSelectionGet fileFunctionListerDlg.functionName -string]
    set filelist [controlSelectionGet fileFunctionListerDlg.fileName -string]
    set tlist ""
    if {$funclist == {}} {
	if {$filelist == {}} {
	    messageBox -ok -exclamationicon "INTERNAL ERROR.  Please report this bug. (neither function nor file selected"
	    return
	} else {
	    set tlist "$filelist:1"
	}
    } else {
	set tlist $funclist
    }

    downtcl -noresult gdb list $tlist
}

proc llast {listvar} {
    set lastidx [expr [llength $listvar] - 1]
    if {$lastidx == -1} {
	return ""
    } else {
	return "[lindex $listvar $lastidx]"
    }
}

proc FunctionName {string} {
    set spaces [split $string " "]
    set fname [llast $spaces]
    regsub {\(\)$}    $fname    "" funcname
    regsub {[\*]*}    $funcname "" funcname1
    return $funcname1
}

proc FuncFileListerDblClk {} {
    set le [lindex [eventInfoGet fileFunctionListerDlg] 0]
    if {$le == "dblclk"} {
	FuncFileListerListCB
    }
}

proc FuncFileListerFileListCB {} {
    controlSelectionSet fileFunctionListerDlg.functionName -noevent -1
    FuncFileListerDblClk
}

proc FuncFileListerFuncListCB {} {
    controlSelectionSet fileFunctionListerDlg.fileName -noevent -1
    FuncFileListerDblClk
}

set FuncFileListerFileList     [list]
set FuncFileListerFunctionList [list]
set FuncFileListerSawMain       0
set FuncFileListerSawMainString ""

proc FuncFileListerDismiss {} {
    global position2

    windowClose fileFunctionListerDlg
    controlCreate Gdb960Buttons "button -name funcFileLister \
	    -title {List source code} \
	    -callback FuncFileLister \
	    $position2"
    return 1
}

proc FuncFileListerInit {} {
    global FuncFileListerFileList
    global FuncFileListerFunctionList
    global FuncFileListerSawMain
    global FuncFileListerSawMainString ""

    if {$FuncFileListerFileList == {} && $FuncFileListerFunctionList == {}} {
	set raw [downtcl gdb info functions]
	regsub -all "\n" $raw "" raw1
	set raw2 [split $raw1 ":"]

	# lop off first and last elements of the list:
	set fflist [lrange $raw2 1 [expr [llength $raw2] - 2]]
	regsub "^File " "[lindex $fflist 0]" "" filename

	foreach funclist [lrange $fflist 1 end] {
	    lappend FuncFileListerFileList $filename
	    set funcs [split $funclist ";"]
	    foreach func [lrange $funcs 0 [expr [llength $funcs]-2]] {
		set f [FunctionName $func]
		if {!$FuncFileListerSawMain && $f == "main"} {
		    set FuncFileListerSawMain 1
		} else {
		    lappend FuncFileListerFunctionList $f
		}
	    }
	    regsub "^File " [llast $funcs] "" filename
	}
	set FuncFileListerFunctionList [lsort $FuncFileListerFunctionList]
	if {$FuncFileListerSawMain} {
	    set FuncFileListerFunctionList [linsert $FuncFileListerFunctionList 0 "main"]
	}
    }
    controlValuesSet fileFunctionListerDlg.fileName [lsort $FuncFileListerFileList]
    if {$FuncFileListerSawMain} {
	set sel "main"
    } else {
	set sel [lindex $FuncFileListerFunctionList 0]
    }
    controlValuesSet fileFunctionListerDlg.functionName $FuncFileListerFunctionList
    controlSelectionSet fileFunctionListerDlg.functionName $sel
    endWaitCursor
}

proc FuncFileLister {} {
    beginWaitCursor
    controlDestroy Gdb960Buttons.funcFileLister
    dialogCreate -name fileFunctionListerDlg -title "File / Function Lister" \
	    -w 220 -h 135 -modeless -init FuncFileListerInit -controls {
	{label -title {Select a filename to list:}     -x   5 -y   5 -w 110 -h  10 }
	{list -name fileName                           -x   5 -y  15 -w 105 -h 100 -callback FuncFileListerFileListCB }
	{label -title {Or, select a function to list:} -x 110 -y   5 -w 110 -h  10 }
	{list -name functionName                       -x 110 -y  15 -w 105 -h 100 -callback FuncFileListerFuncListCB }
	{button -title List                            -x   5 -y 120 -w  25 -h  12 -callback FuncFileListerListCB}
	{button -title Dismiss                         -x 110 -y 120 -w  25 -h  12 -callback FuncFileListerDismiss}
    }
    #windowQueryCloseCallbackSet fileFunctionListerDlg FuncFileListerDismiss
    windowExitCallbackSet fileFunctionListerDlg FuncFileListerDismiss
}

windowCreate -name Gdb960Buttons -noNewInterp -Title "Gdb960 Buttons" -width 220 -height 140

set position1 "-x 10 -y 10 -w 80 -h 15"
set position2 "-x 10 -y 30 -w 80 -h 15"

controlCreate Gdb960Buttons "button -name targetConnect \
	-title {Target Connect} \
	-callback targetConnect \
	-tooltip {Establish Communication with Target} $position1"

controlCreate Gdb960Buttons "button -name openBinary \
-title {Open Binary} \
-callback openBinary \
-tooltip {Load symbols / download code to Target}      $position2"

proc targetConnectInit {} {
    set tm [appRegistryEntryRead IntelGdb960 targetmedia]

    if {$tm == "pci"} {
	controlValuesSet targetConnectDlg.pci 1
    } elseif {$tm == "tcp"} {
	controlValuesSet targetConnectDlg.tcpip 1
    } else {
	controlValuesSet targetConnectDlg.serial 1
    }
}

proc targetConnectOK {} {
    set s [controlValuesGet targetConnectDlg.serial]
    set p [controlValuesGet targetConnectDlg.pci]
    set t [controlValuesGet targetConnectDlg.tcpip]

    windowClose targetConnectDlg
    if {$s} {
	appRegistryEntryDelete IntelGdb960 targetmedia
	appRegistryEntryWrite IntelGdb960 targetmedia serial
	targetConnectSerial
    } elseif {$p} {
	appRegistryEntryDelete IntelGdb960 targetmedia
	appRegistryEntryWrite IntelGdb960 targetmedia pci
	targetConnectPCI
    } elseif {$t} {
	appRegistryEntryDelete IntelGdb960 targetmedia
	appRegistryEntryWrite IntelGdb960 targetmedia tcp
	targetConnectTCP_IP
    }
}

proc targetConnect {} {
    dialogCreate -name targetConnectDlg -title "Target Connect" \
	    -w 110 -h 60 -init targetConnectInit -controls {
	{label -title {Media type:}      -x 5 -y  5 -w 50 -h 10 }
	{choice -name serial -title {Serial} -auto -x 5 -y 17 -w 50 -h 10 }
	{choice -name pci    -title {PCI}    -auto -x 5 -y 29 -w 50 -h 10 }
	{choice -name tcpip  -title {TCP/ip} -auto -x 5 -y 41 -w 50 -h 10 }
	{button -default -callback targetConnectOK -title "OK"
	-name okButt                                                     -x 80 -y 5  -w 25 -h 12}
	{button -callback {windowClose targetConnectDlg} -title "Cancel" -x 80 -y 25 -w 25 -h 12}
    }
}

proc ProgramFileCommand {name} {
    global position2

    controlDestroy Gdb960Buttons.openBinary
    controlCreate Gdb960Buttons "button -name funcFileLister \
	    -title {List source code} \
	    -callback FuncFileLister \
	    $position2"
    set shortName [llast [split $name "/"]]
    setStatusMessage -normal "Loaded symbols for: $shortName"
    addDir $name
}

proc ProgramRunning {} {
}

proc ProgramExited {} {
    messageBox -ok -informationico "Program exited."
}

proc ProgramStartLoad {} {
    messageBox -ok -informationico "Starting download of code. Please stand by."
    beginWaitCursor
}

proc ProgramLoadFailed {} {
    endWaitCursor
    messageBox -ok -exclamation "Program download failed."
}

proc ProgramEndLoad {} {
    endWaitCursor
}


menuItemInsert {&Help {&About Tornado...}} { {&About gdb960v...} } aboutGdb960v
proc aboutGdb960v {} {
    messageBeep -exclamation
    messageBox -ok -informationico "               About gdb960v(visual)
                      Version 6.0\n
Copyright(C) by Wind River Systems, Inc 1996
Portions Copyright(C) by Intel(R) 1997\n\n
Warning: This computer program is protected by
copyright law and international treaties.
Unauthorized reproduction or distribution of
this program, or any portion of it, may
result in severe civil and criminal penalties,
and will be prosecuted to the maximum extent
possible under the law."
}
menuItemDelete {&Help {&About Tornado...}}


#toolbarCreate Gdb960 -name toolBarGdb960

#controlCreate toolBarGdb960 "button -name openBinary \
#-title {Open Binary} \
#-callback openBinary \
#-tooltip {Load symbols / download code to Target}      $position2"
