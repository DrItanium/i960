##############################################################################
# host/resource/tcl/Help.win32.tcl - Tornado (Windows) Support help
#
# Copyright (C) 1995-96 Wind River Systems, Inc.
#


##############################################################################

#
# Sub-classing the following functions:
#

rename controlCreate controlCreateOrig
rename dialogCreate dialogCreateOrig
rename windowCreate windowCreateOrig
rename toolbarCreate toolbarCreateOrig

#
# getFileContents - Return a list of symbolic and numeric IDs
#

proc getFileContents {} {
	set symbolicNumericHelpIdMap ""
	set dicFile "[wtxPath host resource help]tornadow.dic"

	if {[file exists $dicFile]} {
		set dicFileHandle [open $dicFile r]
		while {[gets $dicFileHandle line] >= 0} {
			if {$line != ""} {
				set symbolicNumericHelpIdMap "$symbolicNumericHelpIdMap $line"
			}
		}
		close $dicFileHandle
	} {
		puts "Can't find $dicFile"
	}

	return $symbolicNumericHelpIdMap
}

#
# Set symbolicNumericHelpIdMap global variable only once for later symbolic and numeric mapping
#

set symbolicNumericHelpIdMap [getFileContents]


#
# controlCreate - This function append the "-helpid <id>" string to the control, and
# 		  calling real controlCreate function to complete the job.
#

proc controlCreate {args} {
	global symbolicNumericHelpIdMap

	set controlIndex [expr [lsearch [lindex $args 1] "-name"] +1]	
	set controlName ""
	set controlType [lindex [lindex $args 1] 0]

	if {$controlIndex != 0 && $controlType != "label" && $controlType != "frame"} {
		set controlName [lindex [lindex $args 1] $controlIndex]
	} 

	set windowName [lindex $args 0]

	set cmd ""
	if {$controlName != ""} {
		set symbolicHelpId [format "HIDC_%s_%s" $windowName $controlName]
		set idIndex [expr [lsearch $symbolicNumericHelpIdMap $symbolicHelpId] +1]
		if {$idIndex != 0} {
			set cmd [linsert [lindex $args 1] 1 -helpid [format "%d" [lindex $symbolicNumericHelpIdMap $idIndex]]]
		}
	}

	if {$cmd == ""} {
		set cmd [lindex $args 1]
	}

	controlCreateOrig [lindex $args 0] $cmd
}


#
# toolbarCreate - This function append the "-helpid <id>" string to the control, and
# 		  calling real toolbarCreate function to complete the job.
#

proc toolbarCreate {args} {
	global nextNumericHelpId
	global symbolicNumericHelpIdMap

	set index [expr [lsearch $args "-name"] +1]
	set windowName ""
	set helpStr ""

	if {$index != 0 } {
		set windowName [lindex $args $index]
	}

	if {$windowName != ""} {
		set symbolicHelpId [format "HIDW_%s" $windowName]
		set idIndex [expr [lsearch $symbolicNumericHelpIdMap $symbolicHelpId] +1]

		if {$idIndex != 0} {
			set helpStr [format "-helpid %d" \
					[expr [lindex $symbolicNumericHelpIdMap $idIndex] - 0x50000]]
		}
	}

	set cmd [format "toolbarCreateOrig %s %s" $helpStr $args]

	eval "$cmd"
}


#
# dialogCreate - This function append the "-helpid <id>" string to the control, and
# 		 calling real dialogCreate function to complete the job.
#

proc dialogCreate {args} {
	global symbolicNumericHelpIdMap

	set cmd ""
	set helpStr ""
	set index [expr [lsearch $args "-name"] +1]
	set windowName ""

	if {$index != 0} {
		set windowName [lindex $args $index]
	}

	if {$windowName != ""} {
		set symbolicHelpId [format "HIDD_%s" $windowName]
		set index [expr [lsearch $symbolicNumericHelpIdMap $symbolicHelpId] +1]

		if {$index != 0} {
			set helpStr [format "-helpid %d" \
					[expr [lindex $symbolicNumericHelpIdMap $index] - 0x20000]]
		}
	}

	# If there is a -controls option, process all the controls as well
	set ctrlIndex [expr [lsearch $args "-controls"] +1]
	set newCtrlsData ""
	
	if {$ctrlIndex != 0} {
		set ctrlsData [lindex $args $ctrlIndex]
		set newCtrlsData [controlProcess $ctrlsData $windowName]
		set args [lreplace $args $ctrlIndex $ctrlIndex $newCtrlsData]
	}

	set cmd [format "dialogCreateOrig %s %s" $helpStr $args]

	eval "$cmd"
}



#
# windowCreate - This function append the "-helpid <id>" string to the control, and
# 		 calling real windowCreate function to complete the job.
#

proc windowCreate {args} {
	global symbolicNumericHelpIdMap

	set cmd ""
	set index [expr [lsearch $args "-name"] +1]
	set windowName ""
	set helpStr ""

	if {$index != 0} {
		set windowName [lindex $args $index]
	}

	if {$windowName != ""} {
		set symbolicHelpId [format "HIDW_%s" $windowName]
		set index [expr [lsearch $symbolicNumericHelpIdMap $symbolicHelpId] +1]

		if {$index != 0} {
			set helpStr [format "-helpid %d" [expr [lindex $symbolicNumericHelpIdMap $index] - 0x50000]]
		}
	}

	# If there is a -controls option, process all the controls as well
	set ctrlIndex [expr [lsearch $args "-controls"] +1]
	set newCtrlsData ""

	if {$ctrlIndex != 0} {
		set ctrlsData [lindex $args $ctrlIndex]
		set newCtrlsData [controlProcess $ctrlsData $windowName]
		set args [lreplace $args $ctrlIndex $ctrlIndex $newCtrlsData]
	}

	set cmd [format "windowCreateOrig %s %s" $helpStr $args]

	eval "$cmd"
}



#
# controlProcess - This function take a list of list, append the "-helpid <id>" string to 
# 		 each sublist, and return the modified list of list.
#

proc controlProcess {args} {
	global symbolicNumericHelpIdMap

	set windowName [lindex $args 1]

	foreach ctrlData [lindex $args 0] {
		set index [expr [lsearch $ctrlData "-name"] +1]
		set ctrlName ""
		
		if {$index != 0} {
			set ctrlName [lindex $ctrlData $index]
		}

		set newCtrlData ""
		if {$ctrlName != ""} {
			set symbolicHelpId [format "HIDC_%s_%s" $windowName $ctrlName]
			set idIndex [expr [lsearch $symbolicNumericHelpIdMap $symbolicHelpId] +1]

			if {$idIndex != 0} {
				set newCtrlData [linsert $ctrlData 1 "-helpid" \
							[format "%d" [lindex $symbolicNumericHelpIdMap $idIndex]]]
				lappend newCtrlsData $newCtrlData
			}
		}
		if {$newCtrlData == ""} {
			lappend newCtrlsData $ctrlData
		}
	}

	return $newCtrlsData
}
