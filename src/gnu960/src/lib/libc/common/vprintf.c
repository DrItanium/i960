/*******************************************************************************
 * 
 * Copyright (c) 1993 Intel Corporation
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
 ******************************************************************************/

/* vprintf - Formatted output from a va_list to a file.
 * Copyright (c) 1986 Computer Innovations Inc, ALL RIGHTS RESERVED.
 */

#include <stdio.h>
#include <stdarg.h>

int vprintf(const char *format, va_list args)
{
    int i;
    int size, rw_flag;
    int buf_flag = 0;
    char buf[130];
    unsigned char *save_base;


    if (stdout->_size <= 1) {
        buf_flag = 1;
        _semaphore_wait(&stdout->_sem);
        save_base = stdout->_base;
        stdout->_base = (unsigned char *)buf;
        stdout->_cnt = 128;
        stdout->_ptr = (unsigned char *)buf;
        size = stdout->_size;
        stdout->_size = 128;
        rw_flag = stdout->_flag;
        stdout->_flag |= _IODIRTY;      /* update the buffer */
        _semaphore_signal(&stdout->_sem);
    }

    i = _Ldoprnt(format, &args, stdout, putc);

    if (buf_flag) {
        fflush(stdout);
        _semaphore_wait(&stdout->_sem);
        stdout->_base = save_base;
        stdout->_size = size;
        stdout->_flag = rw_flag;
        _semaphore_signal(&stdout->_sem);
    }

    return i;
}
