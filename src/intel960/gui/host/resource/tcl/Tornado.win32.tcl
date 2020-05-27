##############################################################################
# Tornado.win32.tcl - Tornado Windows GUI Tcl implementation file
#
# Copyright 1996 Wind River Systems, Inc.
#


#puts "sourcing tornado.win32.tcl"

#
# Read in the app customizations.
#

if [catch {source [wtxPath host resource tcl]Help.win32.tcl} result] {
	puts "Missing \"[wtxPath host resource tcl]Help.win32.tcl\".  Help will be disabled."
}

set cfgdir [wtxPath host resource tcl app-config Tornado]\*win32.tcl
foreach file [lsort [glob -nocomplain $cfgdir]] {source $file}

#
# Read in the user's home-directory Tornado Tcl initialization file.
#
if [catch {source [wtxPath .wind]tornado.tcl} result] {
    puts "did not source [wtxPath .wind]tornado.tcl"
}
if {![catch {file exists $env(HOME)/.wind/tornado.tcl} result] && $result} {
    source $env(HOME)/.wind/tornado.tcl
}
