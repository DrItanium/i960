#-----------------------------------------------------------------------------
#               Makefile for  GNU binary-file utilities
#
# $Id: Makefile,v 1.71 1995/07/27 00:42:25 rdsmithx Exp $
#-----------------------------------------------------------------------------

# Essential under System V, harmless elsewhere
SHELL = /bin/sh

GAR	= gar960
GNM	= gnm960
# uncomment next line if gran960 desired
#GRAN	= gran960
GSIZE	= gsize960
GSTRIP	= gstrip960
OBJCOPY	= objcopy

IGAR	= arc960
IGNM	= nam960
IGSIZE	= siz960
IGSTRIP	= str960
IOBJCOPY	= cof960

GAR_HDR =    "Intel 80960 Archiver"
GNM_HDR =    "Intel 80960 Namer"
GSIZE_HDR =  "Intel 80960 Sizer"
# for stripper and obcopy, the name will be supplied at runtime, thus the
# %s 
GSTRIP_HDR = "Intel 80960 Object %s"
OBJCOPY_HDR ="Intel 80960 Object %s"

ITARG	= $(IGAR) $(IGRAN) $(IOBJCOPY) $(IGSTRIP) $(IGNM) $(IGSIZE)
TARG	= $(GAR) $(GRAN) $(OBJCOPY) $(GSTRIP) $(GNM) $(GSIZE)
ITARGEXE= $(IGAR)$(EXE) $(IGRAN)$(EXE) $(IOBJCOPY)$(EXE) $(IGSTRIP)$(EXE) $(IGNM)$(EXE) $(IGSIZE)$(EXE)
TARGEXE	= $(GAR)$(EXE) $(GRAN)$(EXE) $(OBJCOPY)$(EXE) $(GSTRIP)$(EXE) $(GNM)$(EXE) $(GSIZE)$(EXE)

IDIR	= ../../include
LDIR	= $(G960BASE)/lib
CFLAGS  = $(OPT) -DGNU960 $(HOSTDEFS) -I$(IDIR)

#-----------  DEFAULTS  ---------------------------
O	= o
RM	= rm -f
CP	= cp
MV	= mv
LN	= ln
STRIP	= strip
OPT	= -g
IOPT	= -O
OUT	= -o
DEST	= $(G960BASE)/bin

#-----------------------------------------------------------------------------
#		Uncomment for HP9000-300
#__hp9000__#HOSTDEFS	= -DHOST_SYS=HP9000_SYS
#-----------------------------------------------------------------------------
#		Uncomment for HP9000-700
#__hp700__#HOSTDEFS	= -DHOST_SYS=HP9000_SYS
#-----------------------------------------------------------------------------
#		Uncomment for system V release 4 (i386vr4).
#__i386vr4__#HOSTDEFS	= -DHOST_SYS=i386_SYSV_SYS
#-----------------------------------------------------------------------------
#		Uncomment for system V (i386v).
#__i386v__#HOSTDEFS	= -DHOST_SYS=i386_SYSV_SYS
#-----------------------------------------------------------------------------
#		Uncomment for NCR System V (68020v).
#__68020v__#HOSTDEFS	= -DHOST_SYS=NCR_SYSV_SYS -O0
#-----------------------------------------------------------------------------
#		Uncomment for Sun 3
#__sun3__#HOSTDEFS=-DHOST_SYS=SUN3_SYS
#__sun3__#LDFLAGS	= -Bstatic
#-----------------------------------------------------------------------------
#		Uncomment for Sun 386i
#__sun386i__#HOSTDEFS=-DHOST_SYS=SUN3_SYS
#__sun386i__#LDFLAGS	= -Bstatic
#-----------------------------------------------------------------------------
#		Uncomment for Sun 4
#__sun4__#HOSTDEFS=-DHOST_SYS=SUN4_SYS
#__sun4__#LDFLAGS	= -Bstatic
#-----------------------------------------------------------------------------
#		Uncomment for Solaris on the sun4 (sol-sun4).
#__sol-sun4__#HOSTDEFS	= -DHOST_SYS=SOL-SUN4_SYS -DSOLARIS -Xs
#__sol-sun4__#LDFLAGS	= -Bstatic
#-----------------------------------------------------------------------------
#		Uncomment for RS6000
#__rs6000__#HOSTDEFS	= -DHOST_SYS=AIX_SYS -qchars=signed
#-----------------------------------------------------------------------------
#		Uncomment for Decstations
#__dec3100__#HOSTDEFS	= -DHOST_SYS=DEC3100_SYS
#-----------------------------------------------------------------------------
#		Uncomment for VAX/Ultrix
#__vax-ultrix__#HOSTDEFS= -DHOST_SYS=VAX_ULTRIX_SYS
#-----------------------------------------------------------------------------
#		Uncomment for Apollo 400
#__ap400__#HOSTDEFS	= -DHOST_SYS=APOLLO400_SYS -A nansi
#-----------------------------------------------------------------------------
#		Uncomment for DOS
#__dos__#O		= obj
#__dos__#EXE		= .exe
#__dos__#EXP		= .exp
#__dos__#RM		= del
#__dos__#CP		= copy
#__dos__#MV		= mv
#__dos__#LN		= copy
#__dos__#STRIP		= echo
#__dos__#		# Strip is no-op;  might want dtrim instead someday
#__dos__#HOSTDEFS	= -DHOST_SYS=DOS_SYS -DDOS -DUSG -w1 -fsoft -Heol=10
#__dos__#I_DIR		= $(IDIR,/=\)
#__dos__#CC		= HC386
#__dos__#OPT		= -g
#__dos__#IOPT		= -O3
#__dos__#CFLAGS 	= $(OPT) -DGNU960 $(HOSTDEFS) -I$(IDIR)
#__dos__#LDFLAGS	= "-stack 10000h" "-maxreal 0ffffh" -fsoft -twocase -nomap -nostub
#__dos__#OUT		= -e
#__dos__#DEST		= $(G960BASE,/=\)\bin
#__dos__#CLIST		= hc.pro liblst.rf
#__dos__#DOSXP	    = C:\tnt
#__dos__#DOSXN	    = $(DOSXP)\bin\gotnt.exe
#__dos__#CFIG           = $(DOSXP)\bin\cfig386
#__dos__#CFIGFLAGS      = -saveregs -maxvcpimem 0 -vmfile vmmdrv.exp
#-----------------------------------------------------------------------------
#		Uncomment for Windows 95
#__win95__#O		= obj
#__win95__#EXE		= .exe
#__win95__#LN		= copy
#__win95__#STRIP		= echo
#__win95__#HOSTDEFS	= -W2 -DWIN95 -DHOST_SYS=DOS_SYS -DDOS -DUSG
#__win95__#I_DIR		= $(IDIR,/=\)
#__win95__#CC		= CL
#__win95__#OPT		= -Zi
#__win95__#IOPT		= -O2
#__win95__#OUT		= -o
#__win95__#DEST		= $(G960BASE,/=\)\bin
#__win95__#GSTRIP		= gstrip96
#-----------------------------------------------------------------------------


LIBLST	= bucomm.$O $(LDIR)/libbfd.a $(LDIR)/toolib.a
LIBS	= $(LIBLST)
#__dos__#LIBLST	= bucomm.$O $(LDIR)/libbfd.lib $(LDIR)/toolib.lib
#__dos__#LIBS	= @liblst.rf
#__win95__#LIBLST	= bucomm.$O $(LDIR)/libbfd.lib $(LDIR)/toolib.lib

all: $(TARG)

$(IGSIZE): $(GSIZE)
	$(MV) $(GSIZE) $(IGSIZE)

$(GSIZE): $(CLIST) $(LIBLST) size.$O _version
	$(RM) ver960.$O
	make ver960.$O TOOL_HDR=$(GSIZE_HDR)
	$(CC) $(OPT) $(LDFLAGS) $(OUT) $(GSIZE) size.$O $(LIBS) ver960.$O
#__dos__#	copy /b $(DOSXN)+$(GSIZE)$(EXP) $(GSIZE)$(EXE)
#__dos__#       $(CFIG) $(GSIZE) $(CFIGFLAGS)

$(IGNM): $(GNM)
	$(MV) $(GNM) $(IGNM)

$(GNM): $(CLIST) $(LIBLST) nm.$O _version
	$(RM) ver960.$O
	make ver960.$O TOOL_HDR=$(GNM_HDR)
	$(CC) $(OPT) $(LDFLAGS) $(OUT) $(GNM) nm.$O $(LIBS) ver960.$O
#__dos__#	copy /b $(DOSXN)+$(GNM)$(EXP) $(GNM)$(EXE)
#__dos__#       $(CFIG) $(GNM) $(CFIGFLAGS)

#-----------------------------------------------------------------------------
# objcopy and gstrip960 are the same binary 
#-----------------------------------------------------------------------------

$(IOBJCOPY): $(OBJCOPY)
	$(MV) $(OBJCOPY) $(IOBJCOPY)

$(OBJCOPY): $(CLIST) $(LIBLST) copy.$O _version
	$(RM) ver960.$O
	make ver960.$O TOOL_HDR=$(OBJCOPY_HDR)
	$(CC) $(OPT) $(LDFLAGS) $(OUT) $(OBJCOPY) copy.$O $(LIBS) ver960.$O
#__dos__#	copy /b $(DOSXN)+$(OBJCOPY)$(EXP) $(OBJCOPY)$(EXE)
#__dos__#       $(CFIG) $(OBJCOPY) $(CFIGFLAGS)

$(IGSTRIP): $(IOBJCOPY)
	$(RM) $(IGSTRIP)$(EXE)
	$(LN) $(IOBJCOPY)$(EXE) $(IGSTRIP)$(EXE)
#__dos__#	$(RM) $(IGSTRIP)$(EXP)
#__dos__#	$(LN) $(IOBJCOPY)$(EXP) $(IGSTRIP)$(EXP)

$(GSTRIP): $(OBJCOPY)
	$(RM) $(GSTRIP)$(EXE)
	$(LN) $(OBJCOPY)$(EXE) $(GSTRIP)$(EXE)
#__dos__#	$(RM) $(GSTRIP)$(EXP)
#__dos__#	$(LN) $(OBJCOPY)$(EXP) $(GSTRIP)$(EXP)

#-----------------------------------------------------------------------------
# gar960 and gran960 are the same binary
#-----------------------------------------------------------------------------

$(IGAR): $(GAR)
	$(MV) $(GAR) $(IGAR)

$(GAR): $(CLIST) $(LIBLST) ar.$O _version
	$(RM) ver960.$O
	make ver960.$O TOOL_HDR=$(GAR_HDR)
	$(CC) $(OPT) $(LDFLAGS) $(OUT) $(GAR) ar.$O $(LIBS) ver960.$O
#__dos__#	copy /b $(DOSXN)+$(GAR)$(EXP) $(GAR)$(EXE)
#__dos__#       $(CFIG) $(GAR) $(CFIGFLAGS)

# uncomment here if gran960 desired
#$(GRAN): $(GAR)
#	$(RM) $(GRAN)$(EXE)
#	$(LN) $(GAR)$(EXE) $(GRAN)$(EXE)

hc.pro:
#__dos__#	$(CP) $(I_DIR)\sys\hc.pro hc.pro

liblst.rf:
#__dos__#	echo bucomm.$o >> liblst.rf
#__dos__#	echo -l$(LDIR)\libbfd.lib >> liblst.rf
#__dos__#	echo -l$(LDIR)\toolib.lib >> liblst.rf

ar.$O:		$(IDIR)/ar.h $(IDIR)/bfd.h $(IDIR)/sysdep.h
bucomm.$O:	$(IDIR)/bfd.h $(IDIR)/sysdep.h
copy.$O:	$(IDIR)/bfd.h $(IDIR)/sysdep.h
i960pins.$O:	$(IDIR)/bfd.h $(IDIR)/sysdep.h
nm.$O:		$(IDIR)/bfd.h $(IDIR)/getopt.h $(IDIR)/sysdep.h
size.$O:	$(IDIR)/bfd.h $(IDIR)/getopt.h $(IDIR)/sysdep.h

#-----------------------------------------------------------------------------
#		'STANDARD' GNU/960 TARGETS BELOW THIS POINT
#-----------------------------------------------------------------------------

ver960.o: FORCE
	$(RM) ver960.c
	echo static char gnu960_ver[]=	 		 > ver960.c
	echo "	\"$(TOOL_HDR) `cat _version`, `date`\";" >> ver960.c
	echo \#include \"ver960.h\"			>> ver960.c
	echo \#include \"ver960s.h\"			>> ver960.c
	$(CC) $(CFLAGS) -c ver960.c

# DOS version
ver960.obj: FORCE
	$(RM) ver960.c
	echo static char gnu960_ver[]=	 	 > ver960.c
	=$(LDIR)/verstr "$(TOOL_HDR)" _version	>> ver960.c
	echo \#include "ver960.h"		>> ver960.c
	echo \#include "ver960s.h"		>> ver960.c
	$(CC) $(CFLAGS) -c ver960.c

#Dummy target to force execution of dependent targets
FORCE:

# 'G960BASE' will be defined at invocation
# NOTE: the makefile trickery here is fragile!  It has to work for
#	both DOS and Unix; i.e. don't mess with it flippantly.
install:
	make $(TARG) LDIR=$(LDIR) OPT="$(IOPT)" DOSXP=$(DOSXP) DOSXN=$(DOSXN)
	-$(STRIP) $(TARGEXE)
#__dos__#	!foreach TARG $(TARG)
#__win95__#	!foreach TARG $(TARG)
			(cd $(DEST); $(RM) $(TARG)$(EXE))
			$(CP) $(TARG)$(EXE) $(DEST)
#__dos__#		(cd $(DEST); $(RM) $(TARG)$(EXP))
#__dos__#		$(CP) $(TARG)$(EXP) $(DEST)
#__dos__#	!end
#__win95__#	!end

install_ctools:
	make install TARG="$(ITARG)" TARGEXE="$(ITARGEXE)" LDIR="$(LDIR)" OPT="$(IOPT)" G960BASE="$(G960BASE)"

# Separate lines required in order for DOS version to work
#
clean:
	$(RM) *.$O
#__dos__#	echo "Ignore the following warning from delete."
	$(RM) $(TARGEXE)
#__dos__#
#__dos__#	!foreach i $(TARGEXE)
#__dos__#		$(RM) $(i)
#__dos__#	!end
	$(RM) core
	$(RM) *.rex		# for DOS, doesn't hurt anybody else
	$(RM) *.exp		# for DOS, doesn't hurt anybody else
	$(RM) liblst.rf		# for DOS, doesn't hurt anybody else
	$(RM) hc.pro		# for DOS, doesn't hurt anybody else


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
