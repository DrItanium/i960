
/*(c****************************************************************************** *
 * Copyright (c) 1990, 1991, 1992, 1993 Intel Corporation
 * 
 * Intel hereby grants you permission to copy, modify, and distribute this
 * software and its documentation.  Intel grants this permission provided
 * that the above copyright notice appears in all copies and that both the
 * copyright notice and this permission notice appear in supporting
 * documentation.  In addition, Intel grants this permission provided that
 * you prominently mark as "not part of the original" any modifications
 * made to this software or documentation, and that the name of Intel
 * Corporation not be used in advertising or publicity pertaining to
 * distribution of the software or the documentation without specific,
 * written prior permission.
 * 
 * Intel Corporation provides this AS IS, WITHOUT ANY WARRANTY, EXPRESS OR
 * IMPLIED, INCLUDING, WITHOUT LIMITATION, ANY WARRANTY OF MERCHANTABILITY
 * OR FITNESS FOR A PARTICULAR PURPOSE.  Intel makes no guarantee or
 * representations regarding the use of, or the results of the use of,
 * the software and documentation in terms of correctness, accuracy,
 * reliability, currentness, or otherwise; and you rely on the software,
 * documentation and results solely at your own risk.
 * 
 * IN NO EVENT SHALL INTEL BE LIABLE FOR ANY LOSS OF USE, LOSS OF BUSINESS,
 * LOSS OF PROFITS, INDIRECT, INCIDENTAL, SPECIAL OR CONSEQUENTIAL DAMAGES
 * OF ANY KIND.  IN NO EVENT SHALL INTEL'S TOTAL LIABILITY EXCEED THE SUM
 * PAID TO INTEL FOR THE PRODUCT LICENSED HEREUNDER.
 * 
 *****************************************************************************c)*/

/******************************************************************************
 *
 * This file attempts to hide host-dependent differences in tty control
 * from utilities that interact with NINDY over a tty.  As of this writing, it
 * is used by the gdb960 remote communications module and the comm960 utility.
 * 
 * It is assumed that either 'POSIX' or 'USG' is defined on the compiler
 * invocation line if the code should compile and run on a system using either
 * the POSIX or USG(SysV) line control discipline, respectively.  Otherwise,
 * BSD is assumed.
 *
 * Note that a system can be both system V *and* have a POSIX-compliant line
 * discipline; this file checks for POSIX first since it may be necessary to
 * build with other system-V-isms while still using the POSIX tty discipline.
 *
 * Also note that the DOS port is modeled on a Sys-V interface.
 *
 * The application code may access these macros:
 *
 *	TTY_STRUCT	Data type used by host to describe tty attributes.
 *
 *	TTY_GET(fd,tty)	Copies the attributes of the tty with file descriptor
 *			'fd' into the TTY_STRUCT 'tty'.
 *
 *	TTY_SET(fd,tty)	Sets the attributes of the tty with file descriptor
 *			'fd' to those in the TTY_STRUCT 'tty'.
 *
 *	TTY_NINDYTERM(tty)
 *			'tty' is assumed to be the TTY_STRUCT of a terminal.
 *			It is modified to permit it to act as a virtual
 *			terminal for NINDY: all user input passed through
 *			unmodified to NINDY as soon as it is typed, and all
 *			NINDY output passed through unmodified to the display
 *			as soon as it is received.  TTY_SET must be used to
 *			make the changes take effect.
 *
 *	TTY_REMOTE(tty,baud)
 *			Modifies the TTY_STRUCT 'tty' to permit it to be used
 *			as a connection between the host and NINDY at the
 *			baud rate 'baud' (which must be one of the "B..."
 *			defined constants).  TTY_SET must be used to
 *			make the changes take effect.
 *
 *	TTY_FLUSH(fd)	Flushes all pending input and output on the tty whose
 *			file descriptor is 'fd'.
 *
 *	TTY_NBREAD(fd,n,bufptr)
 *			Performs non-blocking read of 'n' characters on the
 *			file descriptor 'fd'.  Sets the integer 'n' to the
 *			number of characters actually read.  The characters
 *			are read into the buffer pointed at by bufptr.
 ******************************************************************************/

/*****************************************************************************
 * MACROS FOR BOTH INTERNAL AND APPLICATION USE
 *****************************************************************************/
#ifdef DOS
#       define  OPEN_TTY        open_port
#       define  READ_TTY        read_port
#       define  WRITE_TTY       write_port
#       define  DUP2_TTY        dup2_port
#if defined(__HIGHC__) || defined(WIN95)
#       define  O_NDELAY        0
#endif
#else  /* UNIX */
#       define  OPEN_TTY        open
#       define  READ_TTY        read
#       define  WRITE_TTY       write
#       define  DUP2_TTY        dup2
#endif /*DOS*/


/*****************************************************************************
 * MACROS FOR INTERNAL USE (within this file) ONLY
 *****************************************************************************/

#define __TTY_REMOTE(tty,baud)				\
	(tty).c_iflag	= 0;				\
	(tty).c_oflag	= 0;				\
	(tty).c_cflag	= (CBAUD & baud) | CS8 | CREAD  \
	                    | CLOCAL | HUPCL;		\
	(tty).c_lflag	= 0;				\
	(tty).c_cc[VEOF]= 1;				\
	(tty).c_cc[VEOL]= 0;

#define __TTY_NINDYTERM(tty)				\
	(tty).c_iflag	= 0;				\
	(tty).c_oflag	= 0;				\
	(tty).c_lflag	= ISIG;				\
	(tty).c_cc[VEOF]= 1;				\
	(tty).c_cc[VEOL]= 0;

#define __TTY_NBREAD(fd,n,bufptr) {			\
	int _saveflags_;				\
	_saveflags_ = fcntl( fd, F_GETFL, 0 );		\
	_saveflags_ &= ~O_NDELAY;			\
	fcntl( fd, F_SETFL, _saveflags_ | O_NDELAY );	\
	n = READ_TTY( fd, bufptr, n );			\
	fcntl( fd, F_SETFL, _saveflags_ );		\
}


/*****************************************************************************
 * MACROS FOR USE BY APPLICATION
 *****************************************************************************/

#ifdef POSIX
#	include <termios.h>
#	define TTY_STRUCT		struct termios
#	define TTY_GET(fd,tty)		tcgetattr(fd,&tty)
#	define TTY_SET(fd,tty)		tcsetattr(fd,TCSANOW,&tty)
#	define TTY_REMOTE(tty,baud)	__TTY_REMOTE(tty,baud)
#	define TTY_NINDYTERM(tty)	__TTY_NINDYTERM(tty)
#	define TTY_NBREAD(fd,n,bufptr)	__TTY_NBREAD(fd,n,bufptr)
#	define TTY_FLUSH(fd)		tcflush(fd,TCIOFLUSH);

#else
#ifdef USG
#	ifdef DOS
#		include "gnudos.h"
#	else
#		include <termio.h>
#	endif
#	define TTY_STRUCT		struct termio
#	define TTY_GET(fd,tty)		ioctl(fd,TCGETA,&tty)
#	define TTY_SET(fd,tty)		ioctl(fd,TCSETAF,&tty)
#	define TTY_REMOTE(tty,baud)	__TTY_REMOTE(tty,baud)
#	define TTY_NINDYTERM(tty)	__TTY_NINDYTERM(tty)
#	define TTY_NBREAD(fd,n,bufptr)	__TTY_NBREAD(fd,n,bufptr)
#	define TTY_FLUSH(fd)		ioctl(fd,TCFLSH,2);

#else /* BSD */
#	include <sys/ioctl.h>
#	define TTY_STRUCT		struct sgttyb
#	define TTY_GET(fd,tty)		ioctl(fd,TIOCGETP,&tty)
#	define TTY_SET(fd,tty)		ioctl(fd,TIOCSETP,&tty)
#	ifdef apollo
#		define _REMOTE_FLAGS	RAW|TANDEM|EVENP|ODDP
#	else
#		define _REMOTE_FLAGS	RAW|TANDEM
#	endif

#	define TTY_REMOTE(tty,baud)			\
		tty.sg_flags = _REMOTE_FLAGS;		\
		tty.sg_ispeed = tty.sg_ospeed = baud;
#	define TTY_NINDYTERM(tty)			\
		tty.sg_flags |= CBREAK;			\
		tty.sg_flags &= ~(ECHO|CRMOD);
#	define TTY_NBREAD(fd,n,bufptr) {		\
		int _n_;				\
		ioctl(fd,FIONREAD,&_n_);		\
		n = (_n_>0) ? read(fd,bufptr,n) : 0;	\
	}
#	define TTY_FLUSH(fd)	{ int _i_ = 0; ioctl(fd,TIOCFLUSH,&_i_); }

#endif	/* USG	*/
#endif	/* POSIX*/

#define GBAUD_DFLT "9600"

#ifndef B19200
#define B19200 EXTA
#endif

#ifndef B38400
#define B38400 EXTB
#endif
