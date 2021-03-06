#The next line was generated by 'mkmake'
HOST=i386_nbsd1

#-----------------------------------------------------------------------------
#			Makefile for gver960
#
# $Id: Makefile,v 1.21 1995/07/27 00:49:43 rdsmithx Exp $
#-----------------------------------------------------------------------------

# Essential under System V, harmless elsewhere
SHELL	= /bin/sh

TARG	= gver960
IDIR	= ../../include
LDIR	= $(G960BASE)/lib
CFLAGS	= $(OPT) -I$(IDIR) $(HOSTDEFS)


#-----------  DEFAULTS  ---------------------------
O	= o
RM	= rm -f
CP	= cp
LB	= a
STRIP	= strip
OPT	= -g
IOPT	= -O
OUT	= -o
DEST	= $(G960BASE)/bin

#-----------------------------------------------------------------------------
#		Uncomment for i386 System V
#__i386v__#HOSTDEFS	= -DUSG
#-----------------------------------------------------------------------------
#		Uncomment for i386 System V release 4
#__i386vr4__#HOSTDEFS	= -DUSG
#-----------------------------------------------------------------------------
#		Uncomment for NCR System V
#__68020v__#HOSTDEFS	= -DUSG
#-----------------------------------------------------------------------------
#		Uncomment for HP-UX on HP9000-300
#__hp9000__#HOSTDEFS	= -DUSG
#-----------------------------------------------------------------------------
#		Uncomment for HP-UX on HP9000-700
#__hp700__#HOSTDEFS	= -DUSG
#-----------------------------------------------------------------------------
#		Uncomment for Apollo 400
#__ap400__#HOSTDEFS	= -A nansi
#-----------------------------------------------------------------------------
#		Uncomment for IBM RS/6000
#__rs6000__#HOSTDEFS	= -DUSG -DPOSIX -qchars=signed
#__rs6000__#LDFLAGS	= -b nodelcsect
#-----------------------------------------------------------------------------
#		Uncomment for Sun 3
#__sun3__#LDFLAGS	= -Bstatic
#-----------------------------------------------------------------------------
#		Uncomment for Sun 386i
#__sun386i__#LDFLAGS	= -Bstatic
#-----------------------------------------------------------------------------
#		Uncomment for Sun 4
#__sun4__#LDFLAGS	= -Bstatic
#-----------------------------------------------------------------------------
#               Uncomment for Solaris on the sun4 (sol-sun4).
#__sol-sun4__#HOSTDEFS	= -DUSG
#__sol-sun4__#LDFLAGS	= -Bstatic
#-----------------------------------------------------------------------------
#		Uncomment for DOS
#__dos__#O		= obj
#__dos__#EXE		= .exe
#__dos__#EXP		= .exp
#__dos__#RM		= del
#__dos__#CP		= copy
#__dos__#CC		= hc386
#__dos__#LB		= lib
#__dos__#LN		= copy
#__dos__#STRIP		= echo	# no-op
#__dos__#HOSTDEFS	= -DHOST_SYS=DOS_SYS -DDOS -DUSG -w1 -fsoft 
#__dos__#OPT		= -g -O0
#__dos__#IOPT		= -O3
#__dos__#LDFLAGS	= $(OPT) -fsoft -stack 80000H -nomap -twocase -maxreal 0ffffh -nostub
#__dos__#OUT		= -e
#__dos__#DEST		= $(G960BASE,/=\)\bin
#__dos__#DOSXP          = c:\tnt
#__dos__#DOSXN          = $(DOSXP)\bin\gotnt.exe
#__dos__#CFIG           = $(DOSXP)\bin\cfig386
#__dos__#CFIGFLAGS      = -saveregs -maxvcpimem 0 -vmfile vmmdrv.exp


#-----------------------------------------------------------------------------
#		Uncomment for Win95
#__win95__#O		= obj
#__win95__#EXE		= .exe
#__win95__#CC		= CL
#__win95__#LB		= lib
#__win95__#LN		= copy
#__win95__#STRIP		= echo	# no-op
#__win95__#HOSTDEFS	= -W2 -DWIN95 -DWIN32 -D_CONSOLE -DHOST_SYS=DOS_SYS -DDOS -DUSG
#__win95__#OPT		= -Zi
#__win95__#IOPT		= -O2
#__win95__#DEST		= $(G960BASE,/=\)\bin
#-----------------------------------------------------------------------------

OBJS	= gver960.$O ver960.$O
LIBS	= $(LDIR)/toolib.$(LB)
LLIBS	= $(LIBS)

# Had to to this for DOS because of 128 char command line limit
#__dos__#LLIBS	= @libs.rf

$(TARG): hc.pro libs.rf $(OBJS) $(LIBS)
	$(CC) $(OPT) $(OUT) $(TARG) $(LDFLAGS) $(OBJS) $(LLIBS)
#__dos__#       copy /b $(DOSXN)+$(TARG)$(EXP) $(TARG)$(EXE)
#__dos__#       $(CFIG) $(TARG) $(CFIGFLAGS)

hc.pro:
#__dos__#	$(CP) $(IDIR,/=\)\sys\hc.pro hc.pro
#__win95__#	@echo

libs.rf:
#__dos__#	$(RM) libs.rf
#__dos__#	!foreach i $(LIBS)
#__dos__#		echo -l$(i) >> libs.rf
#__dos__#	!end
#__win95__#	@echo

gver960.c: $(IDIR)/getopt.h

#-----------------------------------------------------------------------------
#		'STANDARD' GNU/960 TARGETS BELOW THIS POINT
#-----------------------------------------------------------------------------
ver960.o: FORCE
	$(RM) ver960.c
	echo static char gnu960_ver[]=	 		 > ver960.c
	echo "	\"$(TARG) `cat _version`, `date`\";"	>> ver960.c
	echo \#include \"ver960.h\"			>> ver960.c
	$(CC) $(CFLAGS) -c ver960.c

# DOS version
ver960.obj: FORCE
	$(RM) ver960.c
	echo static char gnu960_ver[]=	 	 > ver960.c
	=$(LDIR)/verstr $(TARG)$(EXE) _version	>> ver960.c
	echo \#include "ver960.h"		>> ver960.c
	$(CC) $(CFLAGS) -c ver960.c

# Dummy target to force execution of dependent targets.
#
FORCE:

# 'G960BASE' will be defined at invocation
install:
	make $(TARG) LDIR="$(LDIR)" OPT="$(IOPT)"
	-$(STRIP) $(TARG)$(EXE)
	(cd $(DEST); $(RM) $(TARG)$(EXE))
	$(CP) $(TARG)$(EXE) $(DEST)
#__dos__#	(cd $(DEST); $(RM) $(TARG)$(EXP))
#__dos__#	$(CP) $(TARG)$(EXP) $(DEST)

# Separate lines required in order for DOS version to work
#
clean:
	$(RM) $(TARG)$(EXE)
	$(RM) *.$O
	$(RM) core
	$(RM) *.rex		# for DOS, doesn't hurt anybody else
	$(RM) *.exp		# for DOS, doesn't hurt anybody else
	$(RM) hc.pro		# for DOS, doesn't hurt anybody else
	$(RM) libs.rf		# for DOS, doesn't hurt anybody else

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
