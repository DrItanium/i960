/*****************************************************************************
 * 
 * Copyright (c) 1995 Intel Corporation
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
 ****************************************************************************/

#include "libdwarf.h"
#include "internal.h"

/* 
 * dwarf_bytesize -- Finds the number of bytes needed to 
 * contain an instance of the DIE. 
 *
 * RETURNS: The number of bytes needed to contain an instance
 * of the DIE, otherwise, -1.
 *
 * ERROR: Returns -1.
 */     
Dwarf_Signed
dwarf_bytesize(die)
    Dwarf_Die die; 
{
    Dwarf_Attribute fp;
 
    RESET_ERR();
    if(die) {
        fp = die->attr_list;
        while(fp) {
            if(fp->at_name == DW_AT_byte_size)
                return(fp->at_value.val_signed);
            else
                fp = fp->next;
        }
        return(-1);      /* couldn't find the attribute */
    }
    else {
        LIBDWARF_ERR(DLE_IA);
        return(-1);
    }
}

