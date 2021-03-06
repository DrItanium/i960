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
#include "pubnames.h"

/*
    LIBRARY FUNCTION

	dwarf_srcpubs -- Read a block of pubnames for an entire
	    compilation unit

    SYNOPSIS

	Dwarf_Signed dwarf_srcpubs(Dwarf_Debug dbg,
				   Dwarf_Die die,
				   Dwarf_Pn_Tuple *pubnames_buf);
    DESCRIPTION
    
	Allocate and assign contiguous storage to hold Dwarf_Pn_Tuple 
        descriptors for an entire compilation unit.  Set PUBNAMES_BUF to 
        point to the start of the array of descriptors.  The DIE parameter 
        must represent a compilation unit.  

    RETURNS

	The number of descriptors in the block.  DLV_NOCOUNT is returned
	on error, or if .debug_pubnames section DOES exist but there
        are no tuples in it.  This is the case for compilers (like
        WATCOM) that add a header in .debug_pubnames without any tuples.

    BUGS

	In the future we may find a way to read in only some of the pubnames
	info for a CU, not all of it.  We need a way to tell what has been
	done and what hasn't.
*/

Dwarf_Signed 
dwarf_srcpubs(dbg, die, pubnames_buf)
    Dwarf_Debug dbg;
    Dwarf_Die die; 
    Dwarf_Pn_Tuple *pubnames_buf;
{
    Dwarf_Pubnames_Table *pn_table;

    RESET_ERR();
    if ( die->tag != DW_TAG_compile_unit )
    {
    	if(LIBDWARF_ERR(DLE_NOT_CU_DIE) == DLS_ERROR)
    	    return(DLV_NOCOUNT);
    }

    /* If pubnames have not been read in for this C.U., they
       haven't been read in for any C.U.  Build tables for ALL C.U's,
       then continue with search as if nothing had happened. */
    if (! die->cu_node->pubnames_table)
    {
        _dw_build_pubnames(dbg);
    }

    pn_table = die->cu_node->pubnames_table;

    if (pn_table->pn_list)
    {
        *pubnames_buf = pn_table->pn_list;
        return pn_table->pn_count;
    }
    else
    {
        /* no tuple list for this CU; don't error out, in case there's
          a list in a later CU */
        *pubnames_buf = NULL;
        return(DLV_NOCOUNT);
    }
}

