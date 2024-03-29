#-----------------------------------------------------------------------------
# Makefile for readline.a -- support for gdb960 2.4+
#
# $Id: Makefile,v 1.19 1995/12/24 10:15:00 cmorgan Exp $
#-----------------------------------------------------------------------------

# Essential under System V, harmless elsewhere
SHELL = /bin/sh

TARG	= readline.a
GDB_SRC	= ../../gdb960/common
GNU960_INC = ../../include

ALL_CFLAGS  = $(HOSTCFLAGS) $(OPT) $(STDDEFS) $(HOSTDEFS) -I. -I$(GDB_SRC)

# For Unix: CFLAGS is the same as ALL_CFLAGS
# For DOS: CFLAGS is too long for the command line; make a response file.
CFLAGS = $(ALL_CFLAGS)

#-----------  DEFAULTS  ---------------------------
O 	= o
RM	= rm -f
CP	= cp
LN	= ln -s 
OPT	= -g
IOPT	= -O
NOLINK	= -c
HOSTNM	= \"$(HOST)\"
AR	= ar cvr $(TARG) $(OBJS)
RANLIB	= ranlib
DEST	= $(G960BASE)/lib
STDDEFS = -DIMSTG -DVOID_SIGHANDLER

#-----------------------------------------------------------------------------
#  Host-specific defines.
#  HOSTDEFS parameters are coming from the gdb readline/config/mh-* files,
#  where * determines the host.  SYSDEP_H files are found by finding your
#  host in readline/configure.in
#-----------------------------------------------------------------------------
#		Uncomment for system V / i386 (release 4)
#__i386vr4__#HOSTDEFS	= -DVI_MODE -DUSG -DUSGr4
#__i386vr4__#SYSDEP_H	= sysdep-sysv4.h
#__i386vr4__#RANLIB	= true
#-----------------------------------------------------------------------------
#               Uncomment for sun4
#__sun4__#HOSTDEFS	= -DVI_MODE
#__sun4__#SYSDEP_H	= sysdep-norm.h
#-----------------------------------------------------------------------------
#               Uncomment for Solaris
#__sol-sun4__#HOSTDEFS	= -DVI_MODE -DUSG -DUSGr4
#__sol-sun4__#SYSDEP_H	= sysdep-norm.h
#__sol-sun4__#RANLIB	= true
#-----------------------------------------------------------------------------
#               Uncomment for hp9000/700
#__hp700__#HOSTDEFS	= -DVI_MODE
#__hp700__#SYSDEP_H	= sysdep-norm.h
#__hp700__#RANLIB	= true
#-----------------------------------------------------------------------------
#               Uncomment for hp9000/300
#__hp9000__#HOSTDEFS	= -DVI_MODE
#__hp9000__#SYSDEP_H	= sysdep-norm.h
#__hp9000__#RANLIB	= true
#-----------------------------------------------------------------------------
#		Uncomment for IBM RS/6000
#__rs6000__#HOSTDEFS	= -DVI_MODE -DUSG -DTIOCGWINSZ_BROKEN
#__rs6000__#SYSDEP_H	= sysdep-aix.h
#-----------------------------------------------------------------------------
#		Uncomment for MS/DOS
#  Set up for 386|DOS-Extender (Phar Lap) with Metaware 3.x compiler
#__dos__#TARG           = readline.lib
#__dos__#O 		= obj
#__dos__#RM 		= del
#__dos__#CP 		= copy
#__dos__#LN 		= copy
#__dos__#HOSTDEFS 	= -D__GO32__ -DVI_MODE -DDOS -DNO_SYS_FILE
#__dos__#SYSDEP_H	= sysdep-dos.h
#__dos__#OPT 		= -g 
#__dos__#IOPT 		= -O3 
#__dos__#RANLIB 	= @echo 
#__dos__#AR		= 386lib $(TARG) -create -twocase @objlist.rf
#__dos__#DEST		=$(G960BASE,/=\)\lib
#__dos__#GNU960INC	=$(GNU960_INC,/=\)
#__dos__#AS		= 386ASM
#__dos__#ASFLAGS	= -twocase
#__dos__#CC             = HC386
#__dos__#HOSTCFLAGS  	= -fsoft -Heol=10 -w1 -I$(GNU960INC)
#__dos__#CFLAGS		= @defines.rf
#__dos__#PHARLAP	= c:\phar386
#__dos__#PHARBIN        = $(PHARLAP)\bin
#__dos__#GORUN		= $(PHARBIN)\gorun386.exe
#-----------------------------------------------------------------------------
#		Uncomment for Windows 95
#  Set up for Microsoft C 9.00 (Visual C++ 2.0)
#__win95__#TARG		= readline.lib
#__win95__#O		= obj
#__win95__#LN		= copy
#__win95__#HOSTDEFS	= -DWIN95 -DDOS -DNO_SYS_FILE -DVI_MODE -D__GO32__
#__win95__#SYSDEP_H	= sysdep-dos.h
#__win95__#OPT		= -Zi
#__win95__#IOPT		= -Ot
#__win95__#RANLIB		= @echo 
#__win95__#AR		= lib /OUT:$(TARG) @objlist.rf
#__win95__#DEST		= $(G960BASE,/=\)\lib
#__win95__#GNU960INC	= $(GNU960_INC,/=\)
#__win95__#CC		= CL
#__win95__#HOSTCFLAGS	= -W2 -I$(GNU960INC)
#__win95__#CFLAGS		= @defines.rf
#-----------------------------------------------------------------------------

.c.$O:
	$(CC) $(NOLINK) $(CFLAGS) $<

OBJS = bind.$O complete.$O display.$O funmap.$O history.$O isearch.$O \
	keymaps.$O parens.$O readline.$O rltty.$O search.$O signals.$O \
	tilde.$O vi_mode.$O

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
#__win95__#	echo $(ALL_CFLAGS) > defines.rf

hc.pro:
#__dos__#	$(CP) $(GNU960INC)\sys\hc.pro hc.pro
#__win95__#	@echo

sysdep.h: $(SYSDEP_H)
	$(RM) sysdep.h
	$(LN) $(SYSDEP_H) sysdep.h

#-----------------------------------------------------------------------------
#   Source file dependencies
#-----------------------------------------------------------------------------
$(OBJS):	sysdep.h readline.h keymaps.h chardefs.h
bind.$O: 	rldefs.h  history.h 
complete.$O: 	rldefs.h  
display.$O: 	rldefs.h  history.h 
history.$O:	history.h
isearch.$O: 	history.h 
keymaps.$O:	emacs_keymap.c vi_keymap.c
readline.$O: 	rldefs.h  history.h highc.c win95.c
rltty.$O: 	rldefs.h  
search.$O: 	history.h 
signals.$O: 	rldefs.h  history.h 
tilde.$O: 	tilde.h 
vi_mode.$O:	history.h

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
	$(RM) sysdep.h
	$(RM) core
	$(RM) *.rf		# DOS -- doesn't hurt anybody else
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
