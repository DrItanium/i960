/*(cb*/
/*******************************************************************************
 *
 * Copyright (c) 1993, 1994 Intel Corporation
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

/* Control/status register offsets from base address */

#define RBR 0x00
#define THR 0x00
#define DLL 0x00
#define IER 0x01
#define DLM 0x01
#define IIR 0x02
#define FCR 0x02
#define LCR 0x03
#define MCR 0x04
#define LSR 0x05
#define MSR 0x06
#define SCR 0x07

/* 16550A Line Control Register */

#define LCR_5BITS 0x00
#define LCR_6BITS 0x01
#define LCR_7BITS 0x02
#define LCR_8BITS 0x03
#define LCR_NSB 0x04
#define LCR_PEN 0x08
#define LCR_EPS 0x10
#define LCR_SP 0x20
#define LCR_SB 0x40
#define LCR_DLAB 0x80

/* 16550A Line Status Register */

#define LSR_DR 0x01
#define LSR_OE 0x02
#define LSR_PE 0x04
#define LSR_FE 0x08
#define LSR_BI 0x10
#define LSR_THRE 0x20
#define LSR_TSRE 0x40
#define LSR_FERR 0x80

/* 16550A Interrupt Identification Register */

#define IIR_IP 0x01
#define IIR_ID 0x0e
#define IIR_RLS 0x06
#define IIR_RDA 0x04
#define IIR_THRE 0x02
#define IIR_MSTAT 0x00
#define IIR_TIMEOUT 0x0c

/* 16550A interrupt enable register bits */

#define IER_DAV 0x01
#define IER_TXE 0x02
#define IER_RLS 0x04
#define IER_MS 0x08

/* 16550A Modem control register */

#define MCR_DTR 0x01
#define MCR_RTS 0x02
#define MCR_OUT1 0x04
#define MCR_OUT2 0x08
#define MCR_LOOP 0x10

/* 16550A Modem Status Register */

#define MSR_DCTS 0x01
#define MSR_DDSR 0x02
#define MSR_TERI 0x04
#define MSR_DRLSD 0x08
#define MSR_CTS 0x10
#define MSR_DSR 0x20
#define MSR_RI 0x40
#define MSR_RLSD 0x80

/* (*) 16550A FIFO Control Register */

#define FCR_EN 0x01
#define FCR_RXCLR 0x02
#define FCR_TXCLR 0x04
#define FCR_DMA 0x08
#define FCR_RES1 0x10
#define FCR_RES2 0x20
#define FCR_RXTRIG_L 0x40
#define FCR_RXTRIG_H 0x80


#define CHAN1 0x8
#define CHAN2 0x0
