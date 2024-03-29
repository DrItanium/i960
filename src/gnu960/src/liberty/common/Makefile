#-----------------------------------------------------------------------------
# Makefile for liberty.a -- support for gdb960 2.4+
#
# $Id: Makefile,v 1.27 1995/07/27 00:40:48 rdsmithx Exp $
#-----------------------------------------------------------------------------

# Essential under System V, harmless elsewhere
SHELL = /bin/sh

TARG	= liberty.a
GDB_SRC	= ../../gdb960/common
GNU960_INC	= ../../include
ALL_CFLAGS  = -DIMSTG $(HOSTCFLAGS) $(OPT) $(HOSTDEFS) -I. -I$(GDB_SRC)

# For Unix: CFLAGS is the same as ALL_CFLAGS
# For DOS: CFLAGS is too long for the command line; make a response file.
CFLAGS = $(ALL_CFLAGS)

#-----------  DEFAULTS  ---------------------------
O 	= o
RM	= rm -f
CP	= cp
LN	= ln
OPT	= -g
IOPT	= -O
NOLINK	= -c
HOSTNM	= \"$(HOST)\"
AR	= ar cvr $(TARG) $(OBJS)
RANLIB	= ranlib
DEST	= $(G960BASE)/lib

#-----------------------------------------------------------------------------
#  Host-specific defines.
#  HOSTDEFS is coming from, in the gdb distribution, libiberty/config.h
#  and, for some hosts, libiberty/config/mh-$HOST.  The libiberty makefile
#  creates config.h on the fly with a fairly complex system that has been
#  simplified here by just copying the config.h #defines into our makefile.
#  Similarly, HOSTOBJS is found from the sum of the file "needed-list" and,
#  for some hosts, the EXTRA_OFILES macro in config/mh-$HOST.
#-----------------------------------------------------------------------------
#		Uncomment for system V / i386 (release 4)
#__i386vr4__#HOSTDEFS	= -DHAVE_SYSCONF -DNEED_sys_siglist -DNEED_psignal
#__i386vr4__#HOSTOBJS	= index.o sigsetmask.o getpagesize.o bcmp.o \
#__i386vr4__#			bzero.o bcopy.o random.o rindex.o 
#__i386vr4__#RANLIB	= true
#-----------------------------------------------------------------------------
#               Uncomment for sun4
#__sun4__#HOSTDEFS	= -DNEED_strerror
#__sun4__#HOSTOBJS	= memmove.o strtoul.o
#-----------------------------------------------------------------------------
#               Uncomment for Solaris
#__sol-sun4__#HOSTDEFS	= -DHAVE_SYSCONF -DNEED_sys_siglist
#__sol-sun4__#HOSTOBJS	= index.o bzero.o bcopy.o rindex.o
#__sol-sun4__#RANLIB	= true
#-----------------------------------------------------------------------------
#               Uncomment for hp9000/700
#__hp700__#HOSTDEFS	= -DNEED_sys_siglist -DNEED_psignal
#__hp700__#HOSTOBJS	= insque.o random.o getpagesize.o
#__hp700__#RANLIB	= true
#-----------------------------------------------------------------------------
#               Uncomment for hp9000/300
#__hp9000__#HOSTDEFS	= -DNEED_sys_siglist -DNEED_psignal
#__hp9000__#HOSTOBJS	= insque.o random.o getpagesize.o
#__hp9000__#RANLIB	= true
#-----------------------------------------------------------------------------
#               Uncomment for rs6000
#__rs6000__#HOSTDEFS	= -DNEED_sys_siglist
#__rs6000__#HOSTOBJS	= index.o sigsetmask.o getpagesize.o bcmp.o \
#__rs6000__#			bzero.o bcopy.o random.o rindex.o
#-----------------------------------------------------------------------------
#		Uncomment for MS/DOS
#  Set up for 386|DOS-Extender (Phar Lap) with Metaware 3.x compiler
#__dos__#TARG           = liberty.lib
#__dos__#O 		= obj
#__dos__#RM 		= del
#__dos__#CP 		= copy
#__dos__#LN 		= copy
#__dos__#HOSTDEFS	= -DDOS -DNEED_sys_siglist -DNEED_psignal
#__dos__#HOSTOBJS	= index.obj sigsetmask.obj getpagesize.obj \
#__dos__#	bcmp.obj bzero.obj bcopy.obj random.obj rindex.obj insque.obj
#__dos__#OPT 		= -g 
#__dos__#IOPT 		= -O3 
#__dos__#RANLIB 	= @echo 
#__dos__#AR		= 386lib $(TARG) -create -twocase @objlist.rf
#__dos__#DEST		=$(G960BASE,/=\)\lib
#__dos__#GNU960INC	=$(GNU960_INC,/=\)
#__dos__#GDB_SRC	= ..\..\gdb960\dos
#__dos__#AS		= 386ASM
#__dos__#ASFLAGS	= -twocase
#__dos__#CC             = HC386
#__dos__#CFLAGS		= @defines.rf
#__dos__#HOSTCFLAGS  	= -fsoft -Heol=10 -w1
#__dos__#PHARLAP	= c:\phar386
#__dos__#PHARBIN        = $(PHARLAP)\bin
#__dos__#GORUN		= $(PHARBIN)\gorun386.exe
#-----------------------------------------------------------------------------
#		Uncomment for Windows 95
#  Set up for Microsoft C 9.0 (Visual C++ 2.0)
#__win95__#TARG           = liberty.lib
#__win95__#O 		= obj
#__win95__#LN 		= copy
#__win95__#HOSTDEFS	= -W2 -DWIN95 -DDOS -DNEED_sys_siglist -DNEED_psignal
#__win95__#HOSTOBJS	= index.obj sigsetmask.obj getpagesize.obj \
#__win95__#	bcmp.obj bzero.obj bcopy.obj random.obj rindex.obj insque.obj
#__win95__#OPT 		= -Zi
#__win95__#IOPT 		= -Ot
#__win95__#RANLIB 	= @echo 
#__win95__#AR		= lib /OUT:$(TARG) @objlist.rf
#__win95__#DEST		=$(G960BASE,/=\)\lib
#__win95__#GNU960INC	=$(GNU960_INC,/=\)
#__win95__#GDB_SRC	= ..\..\gdb960\win95
#__win95__#CC             = CL
#-----------------------------------------------------------------------------

.c.$O:
	$(CC) $(NOLINK) $(CFLAGS) $<

#
# The following are always part of liberty.  Some hosts may define HOSTOBJS.
# If you want to add modules to the library, use EXTRAOBJS.
#
BASEOBJS = alloca.$(O) argv.$(O) basename.$(O) concat.$(O) cplus-dem.$(O) \
	fdmatch.$(O) obstack.$(O) spaces.$(O) strerror.$(O) strsignal.$(O) \
	vasprintf.$(O)

OBJS = $(BASEOBJS) $(HOSTOBJS) $(EXTRAOBJS)

$(TARG): objlist.rf defines.rf hc.pro $(OBJS)
	$(RM) $(TARG)
	$(AR)
	$(RANLIB) $(TARG)

objlist.rf:
#__dos__#	!foreach i $(OBJS)
#__dos__#		echo -add $i >> objlist.rf
#__dos__#	!end
#__win95__#	!foreach i $(OBJS)
#__win95__#		echo $i >> objlist.rf
#__win95__#	!end

defines.rf:
#__dos__#	echo $(ALL_CFLAGS) > defines.rf
#__win95__#	@echo

hc.pro:
#__dos__#	$(CP) $(GNU960INC)\sys\hc.pro hc.pro
#__win95__#	@echo

#-----------------------------------------------------------------------------
#   Source file dependencies
#-----------------------------------------------------------------------------
$(OBJS): $(GDB_SRC)/ansidecl.h
cplus-dem.$O: $(GDB_SRC)/demangle.h
getopt.$O: $(GDB_SRC)/getopt.h
getopt1.$O: $(GDB_SRC)/getopt.h

#-----------------------------------------------------------------------------
#		'STANDARD' GNU/960 TARGETS BELOW THIS POINT
#-----------------------------------------------------------------------------

FORCE:

# 'G960BASE' should be defined at invocation
# Avoid explicit use of "/" in pathnames, since DOS version needs "\"
install:
	make $(TARG) OPT="$(IOPT)"
	(cd $(DEST) ; $(RM) $(TARG))
	$(CP) $(TARG) $(DEST)

install_ctools:	install

# Separate lines required in order for DOS version to work
#
clean:
	$(RM) $(TARG)
	$(RM) *.$O
	$(RM) core
	$(RM) objlist.rf	# DOS -- doesn't hurt anybody else
	$(RM) defines.rf	# DOS -- doesn't hurt anybody else
	$(RM) *.rex		# DOS -- doesn't hurt anybody else
	$(RM) hc.pro		# DOS -- doesn't hurt anybody else

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
