/* ho-sysv.h  System V specific header file.
   Copyright (C) 1987, 1991 Free Software Foundation, Inc.

This file is part of GAS, the GNU Assembler.

GAS is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation; either version 1, or (at your option)
any later version.

GAS is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with GAS; see the file COPYING.  If not, write to
the Free Software Foundation, 675 Mass Ave, Cambridge, MA 02139, USA.  */

/* $Id: ho-sysv.h,v 1.1 1991/03/22 00:33:04 chrisb Exp $ */

#define HO_USG

#define bcopy(from,to,n) memcpy((to),(from),(n))
#define bzero(s,n) memset((s),0,(n))
#define index (char *)strchr
#define rindex (char *)strrchr
#define setbuffer(stream, buf, size) setvbuf((stream), (buf), _IOLBF, (size))

extern int free();
extern char *malloc();

/* end of ho-sysv.h */
