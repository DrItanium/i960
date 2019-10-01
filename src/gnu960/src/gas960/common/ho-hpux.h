/* m-hpux.h -- Header to compile the 68020 assembler under HP-UX
   Copyright (C) 1988, 1991 Free Software Foundation, Inc.

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

/* $Id: ho-hpux.h,v 1.3 1991/05/30 14:17:03 chrisb Exp $ */

#include "ho-sysv.h"

/* This header file contains the #defines specific
   to HPUX changes sent me by cph@zurich.ai.mit.edu */
#ifndef hpux
#define hpux
#endif

#ifdef setbuffer
#undef setbuffer
#endif /* setbuffer */

#define setbuffer(stream, buf, size)

#define NO_STDARG

#ifdef GNU960
#define M_HP9000
#endif
