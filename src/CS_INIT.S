/* initial PRCB  */


	.globl  _rom_prcb
	.align 4

_rom_prcb:
	.word   boot_flt_table          #  0 - Fault Table
	.word   _boot_control_table     #  4 - Control Table
	.word   0x00001000              #  8 - AC reg mask overflow fault
	.word   0x40000001              # 12 - Flt - Allow Unaligned fault
	.word   boot_intr_table         # 16 - Interrupt Table
	.word   rom_sys_proc_table      # 20 - System Procedure Table
	.word   0                       # 24 - Reserved
	.word   _intr_stack             # 28 - Interrupt Stack Pointer
	.word   0x00000000              # 32 - Inst. Cache - enable cache
	.word   5                       # 36 - Reg. Cache - 5 sets cached

/* Rom system procedure table */

	.equ    supervisor_proc, 2
	.text
	.align 6
rom_sys_proc_table:
	.space  12                                # Reserved
	.word   _supervisor_stack                 # Supervisor stack pointer
	.space  32                                # Preserved
	.word   _default_sysproc                  # sysproc 0
	.word   _default_sysproc                  # sysproc 1
	.word   _default_sysproc                  # sysproc 2
	.word   _default_sysproc                  # sysproc 3
	.word   _default_sysproc                  # sysproc 4
	.word   _default_sysproc                  # sysproc 5
	.word   _default_sysproc                  # sysproc 6
	.word   _fault_handler + supervisor_proc  # sysproc 7
	.word   _default_sysproc                  # sysproc 8
	.space  251*4                             # sysproc 9-259

/* Fault Table */

	.equ    syscall, 2
	.equ    fault_proc, 7
	.text
	.align  4
boot_flt_table:
	.word   (fault_proc<<2) + syscall    # 0-Parallel Fault
	.word   0x27f
	.word   (fault_proc<<2) + syscall    # 1-Trace Fault
	.word   0x27f
	.word   (fault_proc<<2) + syscall    # 2-Operation Fault
	.word   0x27f
	.word   (fault_proc<<2) + syscall    # 3-Arithmetic Fault
	.word   0x27f
	.word   (fault_proc<<2) + syscall    # 4-Reserved
	.word   0x27f
	.word   (fault_proc<<2) + syscall    # 5-Constraint Fault
	.word   0x27f
	.word   (fault_proc<<2) + syscall    # 6-Reserved
	.word   0x27f
	.word   (fault_proc<<2) + syscall    # 7-Protection Fault
	.word   0x27f
	.word   (fault_proc<<2) + syscall    # 8-Reserved
	.word   0x27f
	.word   (fault_proc<<2) + syscall    # 9-Reserved
	.word   0x27f
	.word   (fault_proc<<2) + syscall    # 0xa-Type Fault
	.word   0x27f
	.space  21*8                         # reserved



/* Boot Interrupt Table */

	.text
boot_intr_table:
	.word   0
	.word   0, 0, 0, 0, 0, 0, 0, 0
	.word   _intx, _intx, _intx, _intx, _intx, _intx, _intx, _intx
	.word   _intx, _intx, _intx, _intx, _intx, _intx, _intx, _intx
	.word   _intx, _intx, _intx, _intx, _intx, _intx, _intx, _intx
	.word   _intx, _intx, _intx, _intx, _intx, _intx, _intx, _intx
	.word   _intx, _intx, _intx, _intx, _intx, _intx, _intx, _intx
	.word   _intx, _intx, _intx, _intx, _intx, _intx, _intx, _intx
	.word   _intx, _intx, _intx, _intx, _intx, _intx, _intx, _intx
	.word   _intx, _intx, _intx, _intx, _intx, _intx, _intx, _intx
	.word   _intx, _intx, _intx, _intx, _intx, _intx, _intx, _intx
	.word   _intx, _intx, _intx, _intx, _intx, _intx, _intx, _intx
	.word   _intx, _intx, _intx, _intx, _intx, _intx, _intx, _intx
	.word   _intx, _intx, _intx, _intx, _intx, _intx, _intx, _intx
	.word   _intx, _intx, _intx, _intx, _intx, _intx, _intx, _intx
	.word   _intx, _intx, _intx, _intx, _intx, _intx, _intx, _intx
	.word   _intx, _intx, _intx, _intx, _intx, _intx, _intx, _intx
	.word   _intx, _intx, _intx, _intx, _intx, _intx, _intx, _intx
	.word   _intx, _intx, _intx, _intx, _intx, _intx, _intx, _intx
	.word   _intx, _intx, _intx, _intx, _intx, _intx, _intx, _intx
	.word   _intx, _intx, _intx, _intx, _intx, _intx, _intx, _intx
	.word   _intx, _intx, _intx, _intx, _intx, _intx, _intx, _intx
	.word   _intx, _intx, _intx, _intx, _intx, _intx, _intx, _intx
	.word   _intx, _intx, _intx, _intx, _intx, _intx, _intx, _intx
	.word   _intx, _intx, _intx, _intx, _intx, _intx, _intx, _intx
	.word   _intx, _intx, _intx, _intx, _intx, _intx, _intx, _intx
	.word   _intx, _intx, _intx, _intx, _intx, _intx, _intx, _intx
	.word   _intx, _intx, _intx, _intx, _intx, _intx, _intx, _intx
	.word   _intx, _intx, _intx, _intx, _intx, _intx, _intx, _intx
	.word   _intx, _intx, _intx, _intx, _intx, _intx, _intx, _intx
	.word   _intx, _intx, _intx, _intx, _intx, _intx, _intx, _intx
	.word   _intx, _intx, _intx, _intx, _intx, _intx, _intx, _intx
	.word   _intx, _intx, _intx, _intx, _intx, _intx, _intx, _intx

/* START */
/* Processor starts execution here after reset. */
	.text
	.globl  _start_ip
	.globl  _reinit
_start_ip:
	mov     0, g14              /* g14 must be 0 for ic960 C compiler */

/* Copy the .data area into RAM.  It has been packed in the ROM
 * after the code area.  If the copy is not needed (RAM-based monitor),
 * the symbol rom_data can be defined as 0 in the linker directives file.
 */

	lda     rom_data, g1            # load source of copy
	cmpobe  0, g1, 1f
	lda     __Bdata, g2             # load destination
	lda     __Edata, g3
 init_data:
	ldq     (g1), r4
	addo    16, g1, g1
	stq     r4, (g2)
	addo    16, g2, g2
	cmpobl  g2, g3, init_data
1:

/* Initialize the BSS area of RAM. */
	lda     __Bbss, g2              # start of bss
	lda     __Ebss, g3              # end of bss
	movq    0,r4
bss_fill:
	stq     r4, (g2)
	addo    16, g2, g2
	cmpobl  g2, g3, bss_fill

/* Save initial value of g0; for the CA/CF it contains the stepping number. */
	st      g0, _componentid

_reinit:
	ldconst 0x300, r4       # reinitialize sys control
	lda     1f, r5
	lda     _rom_prcb, r6
	sysctl  r4, r5, r6
1:

	mov     0, g14

	lda     _user_stack, g0         /* new fp */
	lda     _user_stack, g1         /* new pfp */
	call    move_frame

	ldconst 0x001f2403, r3          /* PC mask */
	ldconst 0x000f0003, r4          /* PC value */
	modpc   r3, r3, r4              /* out of interrupted state */

	call    _main                   # to main routine

terminated:
	fmark                           # cause breakpoint trace fault
	b       terminated

/* move_frame -
		g0 - new frame pointer (FP)
		g1 - new previous frame pointer (PFP)

	This routine switches stacks.  It should be called using a "local"
	call.  The new stack pointer (SP) is calculated by finding the
	relative offset between the old FP and old SP, then adding this
	offset to the new FP.
*/

move_frame:
	andnot  0xf, pfp, r3    /* old FP */
	mov     g0, r6          /* new FP */
	flushreg
	ld      4(r3), r4       /* old SP */
	subo    r3, r4, r5      /* old SP offset from FP */
1:
	ldq     (r3), r8        /* from old frame */
	addo    16, r3, r3
	stq     r8, (r6)        /* to new frame */
	addo    16, r6, r6
	cmpobl  r3, r4, 1b

	addo    g0, r5, r4      /* new SP */
	st      g1, (g0)        /* store new PFP in new frame */
	st      r4, 4(g0)       /* store new SP in new frame */
	mov     g0, pfp         /* new FP */
	mov     g0, g0          /* Allow pfp time to update */
	mov     g0, g0
	mov     g0, g0
	ret


	.globl  _intr_stack
	.globl  _user_stack
	.globl  _supervisor_stack
	.bss    _user_stack, 0x0200, 6          # default application stack
	.bss    _intr_stack, 0x0200, 6          # interrupt stack
	.bss    _supervisor_stack, 0x0600, 6    # fault (supervisor) stack

	.text
_fault_handler:
	ldconst 'F', g0
	call    _co
	ret

_default_sysproc:
	ret

_intx:
	ldconst 'I', g0
	call    _co
	ret



                                                           