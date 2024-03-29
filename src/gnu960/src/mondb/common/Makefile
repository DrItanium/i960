#*******************************************************************************
#*
#* Copyright (c) 1993, 1994 Intel Corporation
#*
#* Intel hereby grants you permission to copy, modify, and distribute this
#* software and its documentation.  Intel grants this permission provided
#* that the above copyright notice appears in all copies and that both the
#* copyright notice and this permission notice appear in supporting
#* documentation.  In addition, Intel grants this permission provided that
#* you prominently mark as "not part of the original" any modifications
#* made to this software or documentation, and that the name of Intel
#* Corporation not be used in advertising or publicity pertaining to
#* distribution of the software or the documentation without specific,
#* written prior permission.
#*
#* Intel Corporation provides this AS IS, WITHOUT ANY WARRANTY, EXPRESS OR
#* IMPLIED, INCLUDING, WITHOUT LIMITATION, ANY WARRANTY OF MERCHANTABILITY
#* OR FITNESS FOR A PARTICULAR PURPOSE.  Intel makes no guarantee or
#* representations regarding the use of, or the results of the use of,
#* the software and documentation in terms of correctness, accuracy,
#* reliability, currentness, or otherwise; and you rely on the software,
#* documentation and results solely at your own risk.
#*
#* IN NO EVENT SHALL INTEL BE LIABLE FOR ANY LOSS OF USE, LOSS OF BUSINESS,
#* LOSS OF PROFITS, INDIRECT, INCIDENTAL, SPECIAL OR CONSEQUENTIAL DAMAGES
#* OF ANY KIND.  IN NO EVENT SHALL INTEL'S TOTAL LIABILITY EXCEED THE SUM
#* PAID TO INTEL FOR THE PRODUCT LICENSED HEREUNDER.
#*
#******************************************************************************/
#*****************************************************************************
# Makefile for mondb -- 
#
# MONDB is used by a host to run programs on Mon960.
#
#*****************************************************************************

# Essential under System V, harmless elsewhere
SHELL = /bin/sh

#-----------  DEFAULTS  ---------------------------
TARG	= mondb
TARG_FULL_NAME ="MONDB"
IDIR	= .
CFLAGS  = $(OPT) $(HOSTDEFS) -I$(IDIR) $(DEBUG)
LDFLAGS = 
O 	= o
RM	= rm -f
CP	= cp
#DEBUG   = -DDEBUG -g -O0
#DEBUG_OBJ = debug.o
OPT	= -g
IOPT	= -O
NOLINK  = -c
AR	= ar cvr $(TARG) $(OBJS)
RANLIB	= ranlib
LIBSUFFIX = a
DEST	= $(G960BASE)/bin
LIB	= $(G960BASE)/lib

#-----------------------------------------------------------------------------
#		Uncomment for Sun4
#__sun4__#HOSTDEFS	= -DUSG -DHOST -DSUNOS_SUN4
#__sun4__#LDFLAGS	= -Bstatic
#-----------------------------------------------------------------------------
#		Uncomment for Solaris
#__sol-sun4__#HOSTDEFS	= -DUSG -DHOST -DSOLARIS_SUN4
#__sol-sun4__#RANLIB	= echo >/dev/null
#__sol-sun4__#LDFLAGS	= -Bstatic 
#-----------------------------------------------------------------------------
#		Uncomment for system V / i386  (release 4)
#__i386vr4__#HOSTDEFS	= -DUSG -DHOST
#__i386vr4__#RANLIB	= echo >/dev/null
#-----------------------------------------------------------------------------
#		Uncomment for RS/6000
#__rs6000__#HOSTDEFS	= -DUSG -DPOSIX -DRS6000 -DHOST
#-----------------------------------------------------------------------------
#		Uncomment for HP-UX (hp 9000/700)
#__hp700__#HOSTDEFS	= -DUSG  -DHOST -DHP700
#__hp700__#RANLIB	= echo >/dev/null
#__hp700__#SYSLIBS	= -ldvio
#-----------------------------------------------------------------------------
#		Uncomment for HP-UX (hp 9000/300)
#__hp9000__#HOSTDEFS	= -DUSG -DHOST
#__hp9000__#RANLIB	= echo >/dev/null
#-----------------------------------------------------------------------------
#		Uncomment for MS/DOS
# Customized for Metaware's DOS compiler and Phar Lap's DOS-Extender.
#
#__dos__#TARG       = mondb.exe
#__dos__#O          = obj
#__dos__#RM         = del
#__dos__#CP         = copy
#__dos__#HOSTDEFS   = -DMSDOS -DHOST -fsoft -Heol=10
#__dos__#HOSTLIBS   =
#__dos__#CC         = HC386
#__dos__#OPT        = -g
#__dos__#IOPT       = -O3
#__dos__#RANLIB     = @echo 
#__dos__#AR         = 386lib $(TARG) -create -twocase @makelib.mw
#__dos__#LIBSUFFIX  = lib
#__dos__#DEST       = $(G960BASE,/=\)\bin
#__dos__#LIB        = $(G960BASE,/=\)\lib
#__dos__#LDFLAGS    = -fsoft "-stack 10000" -twocase
#__dos__#DOSXP      = c:\tnt
#__dos__#DOSXB      = $(DOSXP)\bin
#__dos__#CFIG       = $(DOSXB)\cfig386
#__dos__#CFIGFLAGS  = -SAVEREGS -MAXVCPIMEM 0
#-----------------------------------------------------------------------------
#		Uncomment for Windows 95
# Customized for Microsoft's C 9.00 compiler (Visual C++ 2.0)
#
#__win95__#TARG       = mondb.exe
#__win95__#O          = obj
#__win95__#HOSTDEFS   = -W2 -DWIN95 -DMSDOS -DHOST
#__win95__#HOSTLIBS   =
#__win95__#CC         = CL
#__win95__#OPT        = -Zi
#__win95__#IOPT       = -O2
#__win95__#RANLIB     = @echo 
#__win95__#LIBSUFFIX  = lib
#__win95__#DEST       = $(G960BASE,/=\)\bin
#__win95__#LIB        = $(G960BASE,/=\)\lib
#-----------------------------------------------------------------------------

#DEBUG = -g
INCLUDE = ../../hdil/common

OBJ   = mondb.$O version.$O
LIBS  = $(G960BASE)/lib/hdil.$(LIBSUFFIX) $(G960BASE)/lib/hdilcomm.$(LIBSUFFIX) $(HOSTLIBS)
OBJLIST = $(OBJ) $(LIBS)
#__dos__#OBJLIST = @objs.rf
#__win95__#OBJLIST = @objs.rf

.c.$O: 
	$(CC) $(NOLINK) $(CFLAGS) $<

$(TARG):  $(OBJ) makelib.mw objs.rf
	$(RM) $(TARG)
	$(CC) $(OPT) -o mondb $(LDFLAGS) $(OBJLIST) $(SYSLIBS)
#__dos__#	$(CFIG) $(TARG) $(CFIGFLAGS)
#__dos__#	$(RM) version.$O      # opus make won't rebuild empty targets

makelib.mw:
#__dos__#	!foreach i $(OBJ)
#__dos__#		echo -add $i >>makelib.mw
#__dos__#	!end
#__win95__#	@echo

mondb.$O: mondb.c $(INCLUDE)/common.h  \
		$(INCLUDE)/hdil.h  \
		$(INCLUDE)/hdi_com.h  \
		$(INCLUDE)/hdi_regs.h  \
		$(INCLUDE)/hdi_errs.h  \
		$(INCLUDE)/hdi_brk.h  \
		$(INCLUDE)/hdi_arch.h  \
		$(INCLUDE)/hdi_stop.h
	$(CC) $(OPT) -I$(INCLUDE) $(HOSTDEFS) $(DEBUG) $(NOLINK) mondb.c

verstr.exe: FORCE
	$(CC) verstr.c

version.o: FORCE
	echo  char gnu960_ver[]=	 		 > version.c
	echo "	\"$(TARG_FULL_NAME) `cat _version`, `date`\";"	>> version.c
	$(CC) $(NOLINK) $(CFLAGS) $(GNU960_CFLAGS) version.c 

# DOS version
version.obj: FORCE
	$(MAKE) verstr.exe
	echo char gnu960_ver[]=	 	 > version.c
	verstr $(TARG_FULL_NAME) _version	>> version.c
	$(CC) $(NOLINK) $(CFLAGS) $(GNU960_CFLAGS) version.c
	$(RM) verstr.exe
	$(RM) verstr.obj

# Dummy target to force execution of dependent targets.
#
FORCE:

objs.rf: FORCE
#__dos__#	$(RM) objs.rf
#__dos__#	!foreach i $(OBJ)
#__dos__#		echo $i >> objs.rf
#__dos__#	!end
#__dos__#	!foreach i $(LIBS)
#__dos__#		echo $i >> objs.rf
#__dos__#	!end
#__win95__#	$(RM) objs.rf
#__win95__#	!foreach i $(OBJ) $(LIBS)
#__win95__#		echo $i >> objs.rf
#__win95__#	!end

#-----------------------------------------------------------------------------
#		'STANDARD' GNU/960 TARGETS BELOW THIS POINT
#-----------------------------------------------------------------------------

# 'G960BASE' should be defined at invocation
# Avoid explicit use of "/" in pathnames, since DOS version needs "\"
install:
	make clean
	make $(TARG) OPT=$(INSTOPT)
	(cd $(DEST) ; $(RM) $(TARG))
	$(CP) $(TARG) $(DEST)


# Separate lines required in order for DOS version to work
#
clean:
	$(RM) $(TARG)
	$(RM) *.$O
	$(RM) version.c
#__dos__#	$(RM) maklib.mw
#__dos__#	$(RM) objs.rf
#__win95__#	$(RM) maklib.mw
#__win95__#	$(RM) objs.rf

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
		echo "#The next line was generated by 'make make'"> Makefile; \
		echo "HOST=$(HOST)"				 >> Makefile; \
		echo						 >> Makefile; \
		sed "s/^#__$(HOST)__#//" < Makefile.old		 >> Makefile; \
		if test $(HOST) = dos; then mv Makefile makefile; \
			mv Makefile.old makefile.unx; \
		fi \
	fi
