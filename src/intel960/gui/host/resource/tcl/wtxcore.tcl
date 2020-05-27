# wtxcore.tcl - convenience arrays and functions for wtxtcl
#
# modification history
# --------------------
# 01j,20dec95,p_m  added vxCpuFamily(24) for I960JX (SPR# 5711).
#		   added comment telling to update all necessary variables
#		   when a new CPU is supported.
# 01i,31oct95,pad  added PPC processors and family.
# 01h,30oct95,s_w  added I960JX *family*
# 01g,28sep95,p_m  added I960JX.
# 01f,12jun95,c_s  added wtxErrorName.
# 01e,15may95,c_s  added wtxObjModuleLoad timeout.
# 01d,03may95,f_v  added vxCpuFamily array
# 01c,24apr95,c_s  took care of 5.2 CPU numbering.
# 01b,06mar95,p_m  took care of 5.2 CPU naming.
# 01a,22jan95,c_s  extracted from shelcore.tcl.
#*/

# WARNING: The three tables: cpuType, cpuFamily, and vxCpuFamily must 
# be updated when a new CPU is supported.

# cpuType(): a mapping of the numeric CPU type to the VxWorks architecture
# name.

set cpuType(1)		MC68000
set cpuType(2)		MC68010
set cpuType(3)		MC68020
set cpuType(4)		MC68030
set cpuType(5)		MC68040
set cpuType(6)		MC68LC040
set cpuType(7)		MC68060
set cpuType(8)          CPU32

set cpuType(10)		SPARC
set cpuType(11)		SPARClite

set cpuType(21)		I960CA
set cpuType(22)		I960KA
set cpuType(23)		I960KB
set cpuType(24)		I960JX

set cpuType(41)		R3000
set cpuType(42)		R33000
set cpuType(43)		R33020
set cpuType(44)		R4000

set cpuType(51)		AM29030
set cpuType(52)		AM29200
set cpuType(53)		AM29035

set cpuType(60)		SIMSPARCSUNOS
set cpuType(70)		SIMHPPA

set cpuType(81)		I80386
set cpuType(82)		I80486

set cpuType(91)		PPC601
set cpuType(92)		PPC602
set cpuType(93)		PPC603
set cpuType(94)		PPC604
set cpuType(95)		PPC403

# cpuFamily(): a mapping of the numeric CPU family to the VxWorks architecture
# family name.  This name is used as a key to load the proper architecture-
# specific module for shell support (sh-<family>.tcl). 

set cpuFamily(1)	m68k
set cpuFamily(2)	m68k
set cpuFamily(3)	m68k
set cpuFamily(4)	m68k
set cpuFamily(5)	m68k
set cpuFamily(6)	m68k
set cpuFamily(7)	m68k
set cpuFamily(8)	m68k

set cpuFamily(10)	sparc
set cpuFamily(11)	sparc

set cpuFamily(21)	i960
set cpuFamily(22)	i960
set cpuFamily(23)	i960
set cpuFamily(24)	i960

set cpuFamily(41)	mips
set cpuFamily(42)	mips
set cpuFamily(43)	mips
set cpuFamily(44)	mips

set cpuFamily(51)	am29k
set cpuFamily(52)	am29k
set cpuFamily(53)	am29k

set cpuFamily(60)	simsp
set cpuFamily(70)	simhp

set cpuFamily(81)	i86
set cpuFamily(82)	i86

set cpuFamily(91)	ppc
set cpuFamily(92)	ppc
set cpuFamily(93)	ppc
set cpuFamily(94)	ppc
set cpuFamily(95)	ppc

# this array is used by to get loader object format - see wtxCommonProc.tcl

set vxCpuFamily(1)	MC680X0
set vxCpuFamily(2)	MC680X0
set vxCpuFamily(3)	MC680X0
set vxCpuFamily(4)	MC680X0
set vxCpuFamily(5)	MC680X0
set vxCpuFamily(6)	MC680X0
set vxCpuFamily(7)	MC680X0
set vxCpuFamily(8)	MC680X0

set vxCpuFamily(10)	SPARC
set vxCpuFamily(11)	SPARC

set vxCpuFamily(21)	I960
set vxCpuFamily(22)	I960
set vxCpuFamily(23)	I960
set vxCpuFamily(24)	I960

set vxCpuFamily(51)	AM29XXX
set vxCpuFamily(52)	AM29XXX
set vxCpuFamily(53)	AM29XXX

set vxCpuFamily(60)	SIMSPARCSUNOS

set vxCpuFamily(81)	I80X86
set vxCpuFamily(82)	I80X86

set vxCpuFamily(91)	PPC
set vxCpuFamily(92)	PPC
set vxCpuFamily(93)	PPC
set vxCpuFamily(94)	PPC
set vxCpuFamily(95)	PPC

# asmClass(): a mapping of CPU family name to the disassembler format name
# used by memBlockDis.

set asmClass(m68k)      m68k
set asmClass(sparc)     sparc
set asmClass(simsp)     sparc
set asmClass(i960)      i960
set asmClass(mips)      mips
set asmClass(simhp)     hppa
set asmClass(i86)       i86
set asmClass(am29k)     am29k
set asmClass(ppc)	ppc

#
# A procedure that returns the error name given a WTX Tcl error result string.
#

proc wtxErrorName {wtxErr} {
    if [regexp {WTX Error [0-9A-Fa-fx]+ \((.*)\).*} $wtxErr all errName] {
	return $errName
    } 
    return ""
}

#
# Give a 2-minute timeout for object module loads.
#

if { ![info exists wtxCmdTimeout(wtxObjModuleLoad)]} {
    set wtxCmdTimeout(wtxObjModuleLoad) 120
}

