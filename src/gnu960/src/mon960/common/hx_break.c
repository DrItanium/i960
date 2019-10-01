/*(cb*/
/*******************************************************************************
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
 ******************************************************************************/
/*)ce*/
/* This file contains CA-specific code to set and clear hardware
 * breakpoints, and the breakpoint table, which contains entries for
 * the two hardware and two data breakpoitns defined by the CA. */

#include "common.h"
#include "i960.h"
#include "hdi_brk.h"
#include "hdi_stop.h"

extern void set_ipb(), set_dab(), set_bpcon(), set_xbpcon();
extern int get_dab();
extern send_sysctl();
extern PRCB *prcb_ptr;

/* 960 HX architecture has 6 instruction breakpoints and 6 data breakpoints */
struct bpt bptable[] = {
	{ BRK_HW, FALSE, 0, 0L },
	{ BRK_HW, FALSE, 0, 0L },
	{ BRK_HW, FALSE, 0, 0L },
	{ BRK_HW, FALSE, 0, 0L },
	{ BRK_HW, FALSE, 0, 0L },
	{ BRK_HW, FALSE, 0, 0L },
	{ BRK_DATA, FALSE, 0, 0L },
	{ BRK_DATA, FALSE, 0, 0L },
	{ BRK_DATA, FALSE, 0, 0L },
	{ BRK_DATA, FALSE, 0, 0L },
	{ BRK_DATA, FALSE, 0, 0L },
	{ BRK_DATA, FALSE, 0, 0L },
	{ BRK_NONE }
};

const int have_data_bpts = TRUE;

/****************************************
 * Program Breakpoint Hardware 		
 * This version, for the 960HX, uses the sysctl instruction to reload the	
 * appropriate registers in the control table.  This routine reprograms all
 * breakpoints to the state currently found in 'bptable'.	
 ****************************************/
void
pgm_bp_hw()
{
	int i;
	unsigned long bpcon = 0L;
	unsigned long xbpcon = 0L;

	for (i = 0; i < 12; i++)
	{
		register struct bpt *bp = &bptable[i];

		if (bp->active)
		{
		    switch (bp->type)
		    {
			case BRK_HW:
			    set_ipb(i, bp->addr | IBP_ENABLE(TRUE));
			    break;
			case BRK_DATA:
			    set_dab(i-6, bp->addr);
                if (i < 8)
			        bpcon |= BPCON_MODE(i-6, bp->mode) | BPCON_ENABLE(i-6, TRUE);
                else
			        xbpcon |= XBPCON_MODE(i-6, bp->mode) | XBPCON_ENABLE(i-6, TRUE);
			    break;
		    }
		}
		else
			{
			if (i < 6)
				set_ipb(i,0);
			else	
				set_dab(i-6,0);
			}
	}

	set_bpcon(bpcon);
	set_xbpcon(xbpcon);
}

/* This routine is called when execution stops for a trace event, to
 * determine whether execution stopped because of a hardware breakpoint.
 * It checks the trace flags to make this determination.  */
void
check_brk_pt(ADDR ip, STOP_RECORD *stop_reason)
{
    int i=0;

	if ((register_set[REG_TC] &
	    (TC_INSTRUCTION_EVENT_0 | TC_INSTRUCTION_EVENT_1 |
	     TC_INSTRUCTION_EVENT_2 | TC_INSTRUCTION_EVENT_3 |
	     TC_INSTRUCTION_EVENT_4 | TC_INSTRUCTION_EVENT_5 )) != 0)
	    {
	    stop_reason->reason |= STOP_BP_HW;
	    stop_reason->info.hw_bp_addr = ip;
	    }
        
    if ((register_set[REG_TC] & TC_DATA_EVENT_0) != 0)
	    {
	    stop_reason->reason |= STOP_BP_DATA0;
	    stop_reason->info.da0_bp_addr = get_dab(0);
        i=1;
        }

    if ((register_set[REG_TC] & TC_DATA_EVENT_1) != 0)
	    {
        if (i==0)
            {
	        stop_reason->info.da0_bp_addr = get_dab(1);
	        stop_reason->reason |= STOP_BP_DATA0;
            i=1;
            }
        else
            {
	        stop_reason->info.da1_bp_addr = get_dab(1);
	        stop_reason->reason |= STOP_BP_DATA1;
            return;
            }
	    }
        
    if ((register_set[REG_TC] & TC_DATA_EVENT_2) != 0)
	    {
        if (i==0)
            {
	        stop_reason->info.da0_bp_addr = get_dab(2);
	        stop_reason->reason |= STOP_BP_DATA0;
            i=1;
            }
        else
            {
	        stop_reason->info.da1_bp_addr = get_dab(2);
	        stop_reason->reason |= STOP_BP_DATA1;
            return;
            }
	    }
        
    if ((register_set[REG_TC] & TC_DATA_EVENT_3) != 0)
	    {
        if (i==0)
            {
	        stop_reason->info.da0_bp_addr = get_dab(3);
	        stop_reason->reason |= STOP_BP_DATA0;
            i=1;
            }
        else
            {
	        stop_reason->info.da1_bp_addr = get_dab(3);
	        stop_reason->reason |= STOP_BP_DATA1;
            return;
            }
	    }
        
    if ((register_set[REG_TC] & TC_DATA_EVENT_4) != 0)
	    {
        if (i==0)
            {
	        stop_reason->info.da0_bp_addr = get_dab(4);
	        stop_reason->reason |= STOP_BP_DATA0;
            i=1;
            }
        else
            {
	        stop_reason->info.da1_bp_addr = get_dab(4);
	        stop_reason->reason |= STOP_BP_DATA1;
            return;
            }
	    }
        
    if ((register_set[REG_TC] & TC_DATA_EVENT_5) != 0)
	    {
        if (i==0)
            {
	        stop_reason->info.da0_bp_addr = get_dab(5);
	        stop_reason->reason |= STOP_BP_DATA0;
            i=1;
            }
        else
            {
	        stop_reason->info.da1_bp_addr = get_dab(5);
	        stop_reason->reason |= STOP_BP_DATA1;
            return;
            }
	    }
}
