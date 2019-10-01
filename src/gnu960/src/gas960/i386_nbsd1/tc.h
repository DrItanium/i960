/* tc.h -target cpu dependent- */

/* Copyright (C) 1987, 1990, 1991 Free Software Foundation, Inc.

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

/* static const char rcsid[] = "$Id: tc.h,v 1.13 1995/01/13 17:26:18 paulr Exp $"; */

/* In theory (mine, at least!) the machine dependent part of the assembler
   should only have to include one file.  This one.  -- JF */

extern const pseudo_typeS md_pseudo_table[];

/* JF moved this here from as.h under the theory that nobody except MACHINE.c
   and write.c care about it anyway. */

typedef struct
{
	long	rlx_forward;	/* Forward  reach. Signed number. > 0. */
	long	rlx_backward;	/* Backward reach. Signed number. < 0. */
	unsigned char rlx_length;	/* Bytes length of this address. */
	relax_substateT rlx_more;	/* Next longer relax-state. */
				/* 0 means there is no 'next' relax-state. */
}
relax_typeS;

extern const relax_typeS md_relax_table[]; /* Define it in MACHINE.c */

extern int md_reloc_size; /* Size of a relocation record */

extern void (*md_emit_relocations)();

#ifdef __STDC__

char *md_atof(int what_statement_type, char *literalP, int *sizeP);
int md_estimate_size_before_relax(fragS *fragP, int segment);
long md_pcrel_from(fixS *fixP);
long md_segment_align(relax_addressT slide, int seg);
symbolS *md_undefined_symbol(char *name);
void md_apply_fix(fixS *fixP, long val, char *labelname);
void md_assemble(char *str);
void md_begin(void);
void md_convert_frag(fragS *fragP);
void md_create_long_jump(char *ptr, long from_addr, long to_addr, fragS *frag, symbolS *to_symbol);
void md_create_short_jump(char *ptr, long from_addr, long to_addr, fragS *frag, symbolS *to_symbol);
void md_end(void);
void md_number_to_chars(char *buf, long val, int n, int big_endian_flag);
void md_operand(expressionS *expressionP);
void md_parse_arch(char *arg);
void md_parse_pix(char *arg);
void md_ri_to_chars(struct reloc_info_generic *ri);

#ifndef tc_crawl_symbol_chain
void tc_crawl_symbol_chain(object_headers *headers);
#endif /* tc_crawl_symbol_chain */

#ifndef tc_headers_hook
void tc_headers_hook(object_headers *headers);
#endif /* tc_headers_hook */

#else

char *md_atof();
int md_estimate_size_before_relax();
long md_pcrel_from();
long md_segment_align();
symbolS *md_undefined_symbol();
void md_apply_fix();
void md_assemble();
void md_begin();
void md_convert_frag();
void md_create_long_jump();
void md_create_short_jump();
void md_end();
void md_number_to_chars();
void md_operand();
void md_parse_arch();
void md_parse_pix();
void md_ri_to_chars();

#ifndef tc_headers_hook
void tc_headers_hook();
#endif /* tc_headers_hook */

#ifndef tc_crawl_symbol_chain
void tc_crawl_symbol_chain();
#endif /* tc_crawl_symbol_chain */

#endif /* __STDC_ */

/*
 * Local Variables:
 * comment-column: 0
 * fill-column: 131
 * End:
 */

/* end of tp.h */
