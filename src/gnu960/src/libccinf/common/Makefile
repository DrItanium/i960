# Copyright (C) 1990, 1991 Free Software Foundation, Inc.
#
# This file is part of BFD, the Binary File Diddler.
#
# BFD is free software; you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation; either version 1, or (at your option)
# any later version.
#
# BFD is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with BFD; see the file COPYING.  If not, write to
# the Free Software Foundation, 675 Mass Ave, Cambridge, MA 02139, USA.  */
#-----------------------------------------------------------------------------

#*****************************************************************************
# Makefile for cc_info.a -- the Binary File Diddler library
#
#*****************************************************************************

# Essential under System V, harmless elsewhere
SHELL = /bin/sh

TARG	= cc_info.a
IDIR	= ../../include
LDIR	= $(G960BASE)/lib
CFLAGS	= -DPROTO2 $(OPT) -I$(IDIR) $(HOSTDEFS)

#-----------  DEFAULTS  ---------------------------
O	= o
RM	= rm -f
CP	= cp
OPT	= -g
IOPT	= -O
HOSTNM	= \"$(HOST)\"
AR	= ar cvr $(TARG) $(OBJS)
RANLIB	= ranlib
DEST	= $(G960BASE)/lib

#-----------------------------------------------------------------------------
#		Uncomment for GCC960 HOSTED toolib
#
# Please see instructions in the gld960 makefile for more information.
#
#__gcc960__#TARGET   = cycx
#__gcc960__#CC       = gcc960 -Fcoff -Tm$(TARGET) -fsigned-char -mic-compat -traditional -D__SELFHOST__
#__gcc960__#HOSTDEFS =
#__gcc960__#AR       = gar960 rvc $(TARG) $(OBJS)
#__gcc960__#RANLIB   = echo >/dev/null
#-----------------------------------------------------------------------------
# 		Uncomment for system V (i386v) release 4
#__i386vr4__#RANLIB	= echo >/dev/null
#__i386vr4__#HOSTDEFS	=
#-----------------------------------------------------------------------------
# 		Uncomment for system V (i386v) Release 3.2
#__i386v__#RANLIB	= echo >/dev/null
#__i386v__#HOSTDEFS	=
#-----------------------------------------------------------------------------
# 		Uncomment for NCR System V (68020v).
#__68020v__#RANLIB	= echo >/dev/null
#__68020v__#HOSTDEFS	=
#-----------------------------------------------------------------------------
# 		Uncomment for RS6000.
#__rs6000__#HOSTDEFS	= -DUSG -qchars=signed
#-----------------------------------------------------------------------------
# 		Uncomment for HP9000-300
#__hp9000__#RANLIB	= echo >/dev/null
#__hp9000__#HOSTDEFS	=
#__hp9000__#OPT		= 
#-----------------------------------------------------------------------------
# 		Uncomment for HP9000-700
#__hp700__#RANLIB	= echo >/dev/null
#__hp700__#HOSTDEFS	=
#__hp700__#OPT		= 
#-----------------------------------------------------------------------------
# 		Uncomment for Mac A/UX
#__mac-aux__#RANLIB	= echo >/dev/null
#__mac-aux__#HOSTDEFS	=
#-----------------------------------------------------------------------------
# 		Uncomment for Sun 3
#__sun3__#HOSTDEFS	=
#-----------------------------------------------------------------------------
# 		Uncomment for Sun 386i
#__sun386i__#HOSTDEFS	=
#-----------------------------------------------------------------------------
# 		Uncomment for Sun 4
#__sun4__#HOSTDEFS	=
#-----------------------------------------------------------------------------
#               Uncomment for Solaris on the sun4 (sol-sun4).
#__sol-sun4__#RANLIB	= echo >/dev/null
#__sol-sun4__#HOSTDEFS	=
#-----------------------------------------------------------------------------
# 		Uncomment for Dec 3100
#__dec3100__#HOSTDEFS	=
#-----------------------------------------------------------------------------
# 		Uncomment for Vax-Ultrix
#__vax-ultrix__#HOSTDEFS	=
#-----------------------------------------------------------------------------
# 		Uncomment for Apollo 400
#__ap400__#HOSTDEFS	= -A nansi
#-----------------------------------------------------------------------------
# 		Uncomment for DOS
#__dos__#TARG		= cc_info.lib   
#__dos__#O		= obj
#__dos__#RM		= del
#__dos__#CP		= copy
#__dos__#HOSTNM		= "dos"
#				 Metaware can't handle -DHOST=\"xxx\"
#__dos__#HOSTDEFS	= -DDOS
#__dos__#OPT		= -g
#__dos__#IOPT		= -O3
#__dos__#RANLIB		= @echo
#__dos__#AR		= 386lib $(TARG) -create -twocase @objlist
#__dos__#DEST		=$(G960BASE,/=\)\lib
#__dos__#LDIR		=$(G960BASE,/=\)\lib
#__dos__#I_DIR          =$(IDIR,/=\)
#__dos__#CC             = HC386
#__dos__#CFLAGS	= -w1 -fsoft -Heol=10 $(OPT) -I$(IDIR) $(HOSTDEFS)
#-----------------------------------------------------------------------------
# 		Uncomment for Windows 95
#__win95__#TARG		= cc_info.lib   
#__win95__#O		= obj
#__win95__#HOSTDEFS	= -W2 -DWIN95 -DDOS
#__win95__#OPT		= -Zi
#__win95__#IOPT		= -O2
#__win95__#RANLIB		= @echo
#__win95__#AR		= lib /OUT:$(TARG) @objlist.lbc
#__win95__#DEST		=$(G960BASE,/=\)\lib
#__win95__#LDIR		=$(G960BASE,/=\)\lib
#__win95__#I_DIR          =$(IDIR,/=\)
#__win95__#CC             = CL
#-----------------------------------------------------------------------------


OBJS	= cc_info.$O \
	cc_dinfo.$O \
	cc_finfo.$O \
	cc_linfo.$O \
	cc_pinfo.$O \
	cc_vinfo.$O \
	cc_strpf.$O \
	cc_stran.$O \
	cc_rdwrt.$O \
	cc_x960.$O

HDR	= $(IDIR)/cc_info.h

$(TARG): objlist.lbc hc.pro $(OBJS) 
	$(RM) $(TARG)
	$(AR)
	$(RANLIB) $(TARG)

objlist.lbc:
#__dos__#	!foreach i $(OBJS)
#__dos__#		echo -add $i >>objlist.lbc
#__dos__#	!end
#__win95__#	!foreach i $(OBJS)
#__win95__#		echo $i >>objlist.lbc
#__win95__#	!end

hc.pro:
#__dos__#       $(CP) $(I_DIR)\sys\hc.pro hc.pro
#__win95__#	@echo

cc_info.$O    : cc_info.c    $(HDR)
cc_dinfo.$O   : cc_dinfo.c   $(HDR)
cc_finfo.$O   : cc_finfo.c   $(HDR)
cc_linfo.$O   : cc_linfo.c   $(HDR)
cc_pinfo.$O   : cc_pinfo.c   $(HDR) cc_pinfo.h
cc_strpf.$O   : cc_strpf.c  $(HDR) cc_pinfo.h
cc_rdwrt.$O   : cc_rdwrt.c   $(HDR)
cc_stran.$O   : cc_stran.c

#-----------------------------------------------------------------------------
#		'STANDARD' GNU/960 TARGETS BELOW THIS POINT
#-----------------------------------------------------------------------------

VERSTR = echo "	\"$(TARG) `cat _version`, `date`\";"
#__dos__#VERSTR = =$(LDIR)/verstr $(TARG) _version
#__win95__#VERSTR = =$(LDIR)/verstr $(TARG) _version

cc_vinfo.$O: FORCE
	$(RM) cc_vinfo.c
	echo static char cc_info_ver[]=	 > cc_vinfo.c
	$(VERSTR) 			>> cc_vinfo.c
	$(CC) $(CFLAGS) -c cc_vinfo.c

FORCE:


# 'G960BASE' should be defined at invocation
# Avoid explicit use of "/" in pathnames, since DOS version needs "\"
install:
	make $(TARG) OPT="$(IOPT)"
	(cd $(DEST); $(RM) $(TARG))
	$(CP) $(TARG) $(DEST)
	(cd $(DEST); $(RANLIB) $(TARG))

install_ctools: install

# Separate lines required in order for DOS version to work
#
clean:
	$(RM) $(TARG)
	$(RM) *.$O
	$(RM) core
	$(RM) objlist.lbc	# for DOS, doesn't hurt anybody else
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
