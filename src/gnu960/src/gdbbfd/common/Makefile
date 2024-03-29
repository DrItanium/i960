#-----------------------------------------------------------------------------
# Makefile for gdbbfd.a -- support for gdb960 2.4+
#
# This is a newer version of bfd than ../../libbfd.  This version is here
# to support gdb960 only.  Do not link this library in when building any
# of the other GNU/960 tools.
#
#	$Id: Makefile,v 1.32 1995/12/15 22:33:34 paulr Exp $
#-----------------------------------------------------------------------------

# Essential under System V, harmless elsewhere
SHELL = /bin/sh

TARG	= gdbbfd.a
GDB_SRC	= ../../gdb960/common
GNU960_INC = ../../include
LDIR	= $(G960BASE)/lib

ALL_CFLAGS  = $(OPT) -DIMSTG -I. -I$(GDB_SRC) $(HOSTCFLAGS) $(HOSTDEFS)

# For Unix: CFLAGS is the same as ALL_CFLAGS
# For DOS: CFLAGS is too long for the command line; make a response file.
CFLAGS = $(ALL_CFLAGS)

#-----------  DEFAULTS  ---------------------------
O	= o
RM	= rm -f
CP	= cp
LN	= ln -s
OPT	= -g
IOPT	= -O
NOLINK	= -c
AR	= ar cvr $(TARG) $(OBJS)
RANLIB	= ranlib
DEST	= $(G960BASE)/lib

#-----------------------------------------------------------------------------
#  Host-specific defines.
#  HOSTDEFS is coming from, in the gdb distribution, bfd/config/$HOST.mh.
#  SYSDEP_H comes from bfd/hosts/$HOST.h  SYSDEP_DEP are Makefile dependencies
#  for SYSDEP_H not already covered in "Header file dependencies".
#-----------------------------------------------------------------------------
# 		Uncomment for system V (i386v) release 4
#__i386vr4__#HOSTDEFS	= 
#__i386vr4__#SYSDEP_H	= i386v4.h
#__i386vr4__#SYSDEP_DEP	= 
#__i386vr4__#RANLIB	= true
#-----------------------------------------------------------------------------
# 		Uncomment for Sun 4
#__sun4__#HOSTDEFS	= 
#__sun4__#SYSDEP_H	= sparc.h
#__sun4__#SYSDEP_DEP	= std-host.h
#-----------------------------------------------------------------------------
# 		Uncomment for Solaris
#__sol-sun4__#HOSTDEFS	= -Dconst=
#__sol-sun4__#SYSDEP_H	= solaris2.h
#__sol-sun4__#SYSDEP_DEP	= sysv4.h
#__sol-sun4__#RANLIB	= true
#-----------------------------------------------------------------------------
# 		Uncomment for HP9000/700
#__hp700__#HOSTDEFS	=
#__hp700__#SYSDEP_H	= hppahpux.h
#__hp700__#SYSDEP_DEP	= 
#__hp700__#RANLIB	= true
#-----------------------------------------------------------------------------
# 		Uncomment for HP9000/300
#__hp9000__#HOSTDEFS	= 
#__hp9000__#SYSDEP_H	= hp300.h
#__hp9000__#SYSDEP_DEP	= 
#__hp9000__#RANLIB	= true
#-----------------------------------------------------------------------------
# 		Uncomment for IBM RS/6000
#__rs6000__#HOSTDEFS	= -Dconst=
#__rs6000__#SYSDEP_H	= rs6000.h
#__rs6000__#SYSDEP_DEP	= 
#-----------------------------------------------------------------------------
# 		Uncomment for DOS
#  Set up for 386|DOS-Extender (Phar Lap) with Metaware 3.x compiler
#__dos__#TARG		= gdbbfd.lib   
#__dos__#O		= obj
#__dos__#RM		= del
#__dos__#CP		= copy
#__dos__#LN		= copy
#__dos__#HOSTDEFS	= -DDOS
#__dos__#SYSDEP_H	= msdos.h
#__dos__#SYSDEP_DEP	= 
#__dos__#OPT		= -g
#__dos__#IOPT		= -O3
#__dos__#RANLIB		= @echo
#__dos__#AR		= 386lib $(TARG) -create -twocase @objlist.rf
#__dos__#DEST		=$(G960BASE,/=\)\lib
#__dos__#GNU960INC	=$(GNU960_INC,/=\)
#__dos__#GDB_SRC	= ..\..\gdb960\dos
#__dos__#CC             = HC386
#__dos__#CFLAGS		= @defines.rf
#__dos__#HOSTCFLAGS	= -fsoft -Heol=10 -w1 -I$(GNU960INC)
#-----------------------------------------------------------------------------
# 		Uncomment for Windows 95
#  Set up for Microsoft C 9.00 (Visual C++ 2.0)
#__win95__#TARG		= gdbbfd.lib   
#__win95__#O		= obj
#__win95__#LN		= copy
#__win95__#HOSTDEFS	= -DWIN95 -DALMOST_STDC -DDOS
#__win95__#SYSDEP_H	= msdos.h
#__win95__#SYSDEP_DEP	=
#__win95__#OPT		= -Zi
#__win95__#IOPT		= -O2
#__win95__#RANLIB		= @echo
#__win95__#AR		= lib /OUT:$(TARG) @objlist.rf
#__win95__#DEST		= $(G960BASE,/=\)\lib
#__win95__#GNU960INC	= $(GNU960_INC,/=\)
#__win95__#GDB_SRC		= ..\..\gdb960\win95
#__win95__#CC		= CL
#__win95__#CFLAGS		= @defines.rf
#__win95__#HOSTCFLAGS	= -W2 -I$(GNU960INC)
#-----------------------------------------------------------------------------

.c.$O:
	$(CC) $(NOLINK) $(CFLAGS) $<


BFD_LIBS = archive.$O archures.$O bfd.$O cache.$O coffgen.$O core.$O \
	ctor.$O format.$O hash.$O init.$O libbfd.$O linker.$O opncls.$O \
	reloc.$O section.$O srec.$O syms.$O targets.$O

ALL_MACHINES = cpu-i960.$O

BFD_BACKENDS = 	aout32.$O bout.$O elf.$O \
  elf32.$O elf32-gen.$O \
  coff-i960.$O stab-syms.$O

OBJS = $(BFD_LIBS) $(ALL_MACHINES) $(BFD_BACKENDS)

$(TARG): objlist.rf defines.rf hc.pro $(OBJS) 
	$(RM) $(TARG)
	$(AR)
	$(RANLIB) $(TARG)

#-----------------------------------------------------------------------------
#  Special Cases (i.e. these do not use the .c.o rule)
#-----------------------------------------------------------------------------

# 
# sysdep.h is a link to a host-specific header file
#
sysdep.h:
	$(RM) sysdep.h
	$(LN) $(SYSDEP_H) sysdep.h

#
# libbfd has a special switch which will turn off a chunk of code
#
libbfd.$O: libbfd.c
	$(CC) $(NOLINK) $(CFLAGS) -DNDEBUG $<

#
# DOS response file (for linking objects)
#
objlist.rf:
#__dos__#	!foreach i $(OBJS)
#__dos__#		echo -add $i >> $@
#__dos__#	!end
#__win95__#	!foreach i $(OBJS)
#__win95__#		echo $i >> $@
#__win95__#	!end

#
# DOS response file (for compiling a few objects)
#
defines.rf:
#__dos__#	echo $(ALL_CFLAGS) > defines.rf
#__win95__#	echo $(ALL_CFLAGS) > defines.rf

#
# Metaware profile file
#
hc.pro:
#__dos__#       $(CP) $(GNU960INC)\sys\hc.pro hc.pro
#__win95__#	@echo

#-----------------------------------------------------------------------------
#   Header file dependencies
#-----------------------------------------------------------------------------

$(OBJS): sysdep.h $(SYSDEP_H) $(SYSDEP_DEP) bfd.h libbfd.h bfdlink.h
$(OBJS): $(GDB_SRC)/ansidecl.h $(GDB_SRC)/obstack.h $(GDB_SRC)/fopen-same.h
#__dos__#$(OBJS): $(GDB_SRC)/fopen-bin.h

aout32.$O: aoutx.h libaout.h $(GDB_SRC)/aout-ar.h 
archive.$O: $(GDB_SRC)/aout-ar.h $(GDB_SRC)/aout-ranlib.h 
bfd.$O: libecoff.h 
bout.$O: $(GDB_SRC)/bout.h $(GDB_SRC)/aout-stab_gnu.h libaout.h genlink.h
coff-i960.$O: $(GDB_SRC)/coff-i960.h $(GDB_SRC)/coff-internal.h libcoff.h 
coff-i960.$O: coffcode.h coffswap.h
coffgen.$O: $(GDB_SRC)/coff-internal.h libcoff.h 
elf.$O: $(GDB_SRC)/elf-external.h 
elf.$O: libelf.h $(GDB_SRC)/elf-common.h $(GDB_SRC)/elf-internal.h 
elf32.$O: elfcode.h
elf32-gen.$O: libelf.h elf32-target.h
linker.$O: linker.c bfdlink.h genlink.h
srec.$O: srec.c
stab-syms.$O: libaout.h $(GDB_SRC)/aout-aout64.h $(GDB_SRC)/aout-stab.def 
syms.$O: syms.c

elf32-gen.$O: elf32-gen.c
	$(CC) $(NOLINK) $(CFLAGS) -DUSE_REL elf32-gen.c
	
#-----------------------------------------------------------------------------
#		'STANDARD' GNU/960 TARGETS BELOW THIS POINT
#-----------------------------------------------------------------------------

FORCE:

install:
	make $(TARG) OPT="$(IOPT)"
	(cd $(DEST); $(RM) $(TARG))
	$(CP) $(TARG) $(DEST)

install_ctools: install

# Separate lines required in order for DOS version to work
#
clean:
	$(RM) $(TARG)
	$(RM) *.$(O)
	$(RM) sysdep.h
	$(RM) core
	$(RM) objlist.rf	# for DOS, doesn't hurt anybody else
	$(RM) defines.rf	# for DOS, doesn't hurt anybody else
	$(RM) hc.pro            # for DOS, doesn't hurt anybody else

#-----------------------------------------------------------------------------
# Target to uncomment host-specific lines in this makefile, i.e. lines
# beginning in column 1 with the following string:  #__<hostname>__# .
# Original Makefile is backed up as 'Makefile.old'.
#
# Invoke with:  make make HOST=xxx
#-----------------------------------------------------------------------------
make:
	-@grep -s "^#The next line was generated by 'make make'" Makefile; \
	if test $$? = 0 ; then	\
		echo "Makefile has already been processed with 'make make'";\
		exit 1; \
	elif test $(HOST)x = x ; then \
		echo 'Specify "make make HOST=???"'; \
		exit 1; \
	else \
		mv -f Makefile Makefile.old; \
		echo "#The next line was generated by 'make make'"> Makefile;\
		echo "HOST=$(HOST)"				 >> Makefile; \
		echo						 >> Makefile; \
		sed "s/^#__$(HOST)__#//" < Makefile.old		 >> Makefile; \
		if test $(HOST) = dos; then mv Makefile makefile; fi \
	fi
