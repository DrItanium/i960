#include <errno.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <ctype.h>
#include <string.h>
#include <sys/file.h>
/* need to include <fcntl.h> after <file.h> in A/UX 2.0 */
#include <fcntl.h>
#ifndef O_ACCMODE
#define O_ACCMODE (O_RDONLY | O_WRONLY | O_RDWR)
#endif
#define SEEK_SET 0
#define SEEK_CUR 1

/*
 * Some things that need to be defined in order to make code written for
 * BSD Unix compile under System V ("USG") Unix.  This file should be
 * conditionally included in the source.
 */

#include <memory.h>
#define bcmp(b1,b2,len)		memcmp(b1,b2,len)
#define bcopy(src,dst,len)	memcpy(dst,src,len)
#define bzero(s,n)		memset(s,0,n)


#define rename(from,to) \
	( unlink(to), ((link(from,to)==0) && (unlink(from)==0) ? 0 : -1))

/*
 * Might not need these. Leave them out for now.
 *
#include <string.h>
#define index(s,c)		strchr(s,c)
#define rindex(s,c)		strrchr(s,c)

#ifdef SEEK_SET
#	ifndef L_SET
#		define L_SET SEEK_SET
#	endif
# endif

#ifdef SEEK_CUR
#	ifndef L_INCR
#		define L_INCR SEEK_CUR
#	endif
# endif
 */

/* EXACT TYPES */
typedef char int8e_type;
typedef unsigned char uint8e_type;
typedef short int16e_type;
typedef unsigned short uint16e_type;
typedef int int32e_type;
typedef unsigned int uint32e_type;

/* CORRECT SIZE OR GREATER */
typedef char int8_type;
typedef unsigned char uint8_type;
typedef short int16_type;
typedef unsigned short uint16_type;
typedef int int32_type;
typedef unsigned int uint32_type;

