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

/******************************************************************************/
/*									      */
/*      ceill.as - Extended Precision Ceil Function (80960 with FP)	      */
/*									      */
/******************************************************************************/

	.file "ceill.as"
	.align	4
	.globl	_ceill,__ceill
	.leafproc _ceill,__ceill
	.link_pix

	.text
_ceill:
	lda	Ll0-(.+8)(ip),g14
__ceill:
	shlo	30,3,g4			/* Round mode mask */
	shlo	30,2,g5			/* "Round up" mode bits */
	modac	g4,g5,g6		/* Set to round up, save current */
	movre	g0,fp0			/* Extended precision must be */
	roundr	fp0,fp0			/* ... performed in a floating */
	movre	fp0,g0			/* ... point register */
	modac	g4,g6,g5		/* Restore previous rounding mode */
	mov	g14,g7			/* Copy return address */
	mov	0,g14			/* 0 -> g14 (? C convention ?) */
	bx	(g7)			/* return */

Ll0:
	ret
