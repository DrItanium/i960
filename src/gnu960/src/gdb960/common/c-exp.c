
# line 39 "c-exp.y"

#include "defs.h"
#include <string.h>
#include "expression.h"
#include "value.h"
#include "parser-defs.h"
#include "language.h"
#include "c-lang.h"
#include "bfd.h" /* Required by objfiles.h.  */
#include "symfile.h" /* Required by objfiles.h.  */
#include "objfiles.h" /* For have_full_symbols and have_partial_symbols */

/* Remap normal yacc parser interface names (yyparse, yylex, yyerror, etc),
   as well as gratuitiously global symbol names, so we can have multiple
   yacc generated parsers in gdb.  Note that these are only the variables
   produced by yacc.  If other parser generators (bison, byacc, etc) produce
   additional global names that conflict at link time, then those parser
   generators need to be fixed instead of adding those names to this list. */

#define	yymaxdepth c_maxdepth
#define	yyparse	c_parse
#define	yylex	c_lex
#define	yyerror	c_error
#define	yylval	c_lval
#define	yychar	c_char
#define	yydebug	c_debug
#define	yypact	c_pact	
#define	yyr1	c_r1			
#define	yyr2	c_r2			
#define	yydef	c_def		
#define	yychk	c_chk		
#define	yypgo	c_pgo		
#define	yyact	c_act		
#define	yyexca	c_exca
#define yyerrflag c_errflag
#define yynerrs	c_nerrs
#define	yyps	c_ps
#define	yypv	c_pv
#define	yys	c_s
#define	yy_yys	c_yys
#define	yystate	c_state
#define	yytmp	c_tmp
#define	yyv	c_v
#define	yy_yyv	c_yyv
#define	yyval	c_val
#define	yylloc	c_lloc
#define yyreds	c_reds		/* With YYDEBUG defined */
#define yytoks	c_toks		/* With YYDEBUG defined */

#ifndef YYDEBUG
#define	YYDEBUG	0		/* Default to no yydebug support */
#endif

int
yyparse PARAMS ((void));

static int
yylex PARAMS ((void));

void
yyerror PARAMS ((char *));


# line 107 "c-exp.y"
typedef union 
  {
    LONGEST lval;
    struct {
      LONGEST val;
      struct type *type;
    } typed_val;
    double dval;
    struct symbol *sym;
    struct type *tval;
    struct stoken sval;
    struct ttype tsym;
    struct symtoken ssym;
    int voidval;
    struct block *bval;
    enum exp_opcode opcode;
    struct internalvar *ivar;

    struct type **tvec;
    int *ivec;
  } YYSTYPE;

# line 130 "c-exp.y"
/* YYSTYPE gets defined by %union */
static int
parse_number PARAMS ((char *, int, int, YYSTYPE *));
#ifdef __cplusplus
#  include <stdio.h>
   extern "C" {
     extern void yyerror(char *);
     extern int yylex();
   }
#endif	/* __cplusplus */ 
# define INT 257
# define FLOAT 258
# define STRING 259
# define NAME 260
# define TYPENAME 261
# define NAME_OR_INT 262
# define STRUCT 263
# define CLASS 264
# define UNION 265
# define ENUM 266
# define SIZEOF 267
# define UNSIGNED 268
# define COLONCOLON 269
# define TEMPLATE 270
# define ERROR 271
# define SIGNED_KEYWORD 272
# define LONG 273
# define SHORT 274
# define INT_KEYWORD 275
# define CONST_KEYWORD 276
# define VOLATILE_KEYWORD 277
# define LAST 278
# define REGNAME 279
# define VARIABLE 280
# define ASSIGN_MODIFY 281
# define THIS 282
# define ABOVE_COMMA 283
# define OROR 284
# define ANDAND 285
# define EQUAL 286
# define NOTEQUAL 287
# define LEQ 288
# define GEQ 289
# define LSH 290
# define RSH 291
# define UNARY 292
# define INCREMENT 293
# define DECREMENT 294
# define ARROW 295
# define BLOCKNAME 296
#define yyclearin yychar = -1
#define yyerrok yyerrflag = 0
extern int yychar;
#ifndef YYMAXDEPTH
#define YYMAXDEPTH 150
#endif

/* __YYSCLASS defines the scoping/storage class for global objects
 * that are NOT renamed by the -p option.  By default these names
 * are going to be 'static' so that multi-definition errors
 * will not occur with multiple parsers.
 * If you want (unsupported) access to internal names you need
 * to define this to be null so it implies 'extern' scope.
 * This should not be used in conjunction with -p.
 */
#ifndef __YYSCLASS
# define __YYSCLASS static
#endif
YYSTYPE yylval;
__YYSCLASS YYSTYPE yyval;
typedef int yytabelem;
# define YYERRCODE 256

# line 895 "c-exp.y"


/* Take care of parsing a number (anything that starts with a digit).
   Set yylval and return the token type; update lexptr.
   LEN is the number of characters in it.  */

/*** Needs some error checking for the float case ***/

static int
parse_number (p, len, parsed_float, putithere)
     register char *p;
     register int len;
     int parsed_float;
     YYSTYPE *putithere;
{
  /* FIXME: Shouldn't these be unsigned?  We don't deal with negative values
     here, and we do kind of silly things like cast to unsigned.  */
  register LONGEST n = 0;
  register LONGEST prevn = 0;
  unsigned LONGEST un;

  register int i = 0;
  register int c;
  register int base = input_radix;
  int unsigned_p = 0;

  /* Number of "L" suffixes encountered.  */
  int long_p = 0;

  /* We have found a "L" or "U" suffix.  */
  int found_suffix = 0;

  unsigned LONGEST high_bit;
  struct type *signed_type;
  struct type *unsigned_type;

  if (parsed_float)
    {
      /* It's a float since it contains a point or an exponent.  */
      putithere->dval = atof (p);
      return FLOAT;
    }

  /* Handle base-switching prefixes 0x, 0t, 0d, 0 */
  if (p[0] == '0')
    switch (p[1])
      {
      case 'x':
      case 'X':
	if (len >= 3)
	  {
	    p += 2;
	    base = 16;
	    len -= 2;
	  }
	break;

      case 't':
      case 'T':
      case 'd':
      case 'D':
	if (len >= 3)
	  {
	    p += 2;
	    base = 10;
	    len -= 2;
	  }
	break;

      default:
	base = 8;
	break;
      }

  while (len-- > 0)
    {
      c = *p++;
      if (c >= 'A' && c <= 'Z')
	c += 'a' - 'A';
      if (c != 'l' && c != 'u')
	n *= base;
      if (c >= '0' && c <= '9')
	{
	  if (found_suffix)
	    return ERROR;
	  n += i = c - '0';
	}
      else
	{
	  if (base > 10 && c >= 'a' && c <= 'f')
	    {
	      if (found_suffix)
		return ERROR;
	      n += i = c - 'a' + 10;
	    }
	  else if (c == 'l')
	    {
	      ++long_p;
	      found_suffix = 1;
	    }
	  else if (c == 'u')
	    {
	      unsigned_p = 1;
	      found_suffix = 1;
	    }
	  else
	    return ERROR;	/* Char not a digit */
	}
      if (i >= base)
	return ERROR;		/* Invalid digit in this base */

      /* Portably test for overflow (only works for nonzero values, so make
	 a second check for zero).  FIXME: Can't we just make n and prevn
	 unsigned and avoid this?  */
      if (c != 'l' && c != 'u' && (prevn >= n) && n != 0)
	unsigned_p = 1;		/* Try something unsigned */

      /* Portably test for unsigned overflow.
	 FIXME: This check is wrong; for example it doesn't find overflow
	 on 0x123456789 when LONGEST is 32 bits.  */
      if (c != 'l' && c != 'u' && n != 0)
	{	
	  if ((unsigned_p && (unsigned LONGEST) prevn >= (unsigned LONGEST) n))
	    error ("Numeric constant too large.");
	}
      prevn = n;
    }

  /* An integer constant is an int, a long, or a long long.  An L
     suffix forces it to be long; an LL suffix forces it to be long
     long.  If not forced to a larger size, it gets the first type of
     the above that it fits in.  To figure out whether it fits, we
     shift it right and see whether anything remains.  Note that we
     can't shift sizeof (LONGEST) * HOST_CHAR_BIT bits or more in one
     operation, because many compilers will warn about such a shift
     (which always produces a zero result).  Sometimes TARGET_INT_BIT
     or TARGET_LONG_BIT will be that big, sometimes not.  To deal with
     the case where it is we just always shift the value more than
     once, with fewer bits each time.  */

  un = (unsigned LONGEST)n >> 2;
  if (long_p == 0
      && (un >> (TARGET_INT_BIT - 2)) == 0)
    {
      high_bit = ((unsigned LONGEST)1) << (TARGET_INT_BIT-1);

      /* A large decimal (not hex or octal) constant (between INT_MAX
	 and UINT_MAX) is a long or unsigned long, according to ANSI,
	 never an unsigned int, but this code treats it as unsigned
	 int.  This probably should be fixed.  GCC gives a warning on
	 such constants.  */

      unsigned_type = builtin_type_unsigned_int;
      signed_type = builtin_type_int;
    }
  else if (long_p <= 1
	   && (un >> (TARGET_LONG_BIT - 2)) == 0)
    {
      high_bit = ((unsigned LONGEST)1) << (TARGET_LONG_BIT-1);
      unsigned_type = builtin_type_unsigned_long;
      signed_type = builtin_type_long;
    }
  else
    {
      high_bit = (((unsigned LONGEST)1)
		  << (TARGET_LONG_LONG_BIT - 32 - 1)
		  << 16
		  << 16);
      if (high_bit == 0)
	/* A long long does not fit in a LONGEST.  */
	high_bit =
	  (unsigned LONGEST)1 << (sizeof (LONGEST) * HOST_CHAR_BIT - 1);
      unsigned_type = builtin_type_unsigned_long_long;
      signed_type = builtin_type_long_long;
    }

   putithere->typed_val.val = n;

   /* If the high bit of the worked out type is set then this number
      has to be unsigned. */

   if (unsigned_p || (n & high_bit)) 
     {
       putithere->typed_val.type = unsigned_type;
     }
   else 
     {
       putithere->typed_val.type = signed_type;
     }

   return INT;
}

struct token
{
  char *operator;
  int token;
  enum exp_opcode opcode;
};

static const struct token tokentab3[] =
  {
    {">>=", ASSIGN_MODIFY, BINOP_RSH},
    {"<<=", ASSIGN_MODIFY, BINOP_LSH}
  };

static const struct token tokentab2[] =
  {
    {"+=", ASSIGN_MODIFY, BINOP_ADD},
    {"-=", ASSIGN_MODIFY, BINOP_SUB},
    {"*=", ASSIGN_MODIFY, BINOP_MUL},
    {"/=", ASSIGN_MODIFY, BINOP_DIV},
    {"%=", ASSIGN_MODIFY, BINOP_REM},
    {"|=", ASSIGN_MODIFY, BINOP_BITWISE_IOR},
    {"&=", ASSIGN_MODIFY, BINOP_BITWISE_AND},
    {"^=", ASSIGN_MODIFY, BINOP_BITWISE_XOR},
    {"++", INCREMENT, BINOP_END},
    {"--", DECREMENT, BINOP_END},
    {"->", ARROW, BINOP_END},
    {"&&", ANDAND, BINOP_END},
    {"||", OROR, BINOP_END},
    {"::", COLONCOLON, BINOP_END},
    {"<<", LSH, BINOP_END},
    {">>", RSH, BINOP_END},
    {"==", EQUAL, BINOP_END},
    {"!=", NOTEQUAL, BINOP_END},
    {"<=", LEQ, BINOP_END},
    {">=", GEQ, BINOP_END}
  };

/* Read one token, getting characters through lexptr.  */

static int
yylex ()
{
  int c;
  int namelen;
  unsigned int i;
  char *tokstart;
  char *tokptr;
  int tempbufindex;
  static char *tempbuf;
  static int tempbufsize;
  
 retry:

  tokstart = lexptr;
  /* See if it is a special token of length 3.  */
  for (i = 0; i < sizeof tokentab3 / sizeof tokentab3[0]; i++)
    if (STREQN (tokstart, tokentab3[i].operator, 3))
      {
	lexptr += 3;
	yylval.opcode = tokentab3[i].opcode;
	return tokentab3[i].token;
      }

  /* See if it is a special token of length 2.  */
  for (i = 0; i < sizeof tokentab2 / sizeof tokentab2[0]; i++)
    if (STREQN (tokstart, tokentab2[i].operator, 2))
      {
	lexptr += 2;
	yylval.opcode = tokentab2[i].opcode;
	return tokentab2[i].token;
      }

  switch (c = *tokstart)
    {
    case 0:
      return 0;

    case ' ':
    case '\t':
    case '\n':
      lexptr++;
      goto retry;

    case '\'':
      /* We either have a character constant ('0' or '\177' for example)
	 or we have a quoted symbol reference ('foo(int,int)' in C++
	 for example). */
      lexptr++;
      c = *lexptr++;
      if (c == '\\')
	c = parse_escape (&lexptr);

      yylval.typed_val.val = c;
      yylval.typed_val.type = builtin_type_char;

      c = *lexptr++;
      if (c != '\'')
	{
	  namelen = skip_quoted (tokstart) - tokstart;
	  if (namelen > 2)
	    {
	      lexptr = tokstart + namelen;
	      if (lexptr[-1] != '\'')
		error ("Unmatched single quote.");
	      namelen -= 2;
	      tokstart++;
	      goto tryname;
	    }
	  error ("Invalid character constant.");
	}
      return INT;

    case '(':
      paren_depth++;
      lexptr++;
      return c;

    case ')':
      if (paren_depth == 0)
	return 0;
      paren_depth--;
      lexptr++;
      return c;

    case ',':
      if (comma_terminates && paren_depth == 0)
	return 0;
      lexptr++;
      return c;

    case '.':
      /* Might be a floating point number.  */
      if (lexptr[1] < '0' || lexptr[1] > '9')
	goto symbol;		/* Nope, must be a symbol. */
      /* FALL THRU into number case.  */

    case '0':
    case '1':
    case '2':
    case '3':
    case '4':
    case '5':
    case '6':
    case '7':
    case '8':
    case '9':
      {
	/* It's a number.  */
	int got_dot = 0, got_e = 0, toktype;
	register char *p = tokstart;
	int hex = input_radix > 10;

	if (c == '0' && (p[1] == 'x' || p[1] == 'X'))
	  {
	    p += 2;
	    hex = 1;
	  }
	else if (c == '0' && (p[1]=='t' || p[1]=='T' || p[1]=='d' || p[1]=='D'))
	  {
	    p += 2;
	    hex = 0;
	  }

	for (;; ++p)
	  {
	    /* This test includes !hex because 'e' is a valid hex digit
	       and thus does not indicate a floating point number when
	       the radix is hex.  */
	    if (!hex && !got_e && (*p == 'e' || *p == 'E'))
	      got_dot = got_e = 1;
	    /* This test does not include !hex, because a '.' always indicates
	       a decimal floating point number regardless of the radix.  */
	    else if (!got_dot && *p == '.')
	      got_dot = 1;
	    else if (got_e && (p[-1] == 'e' || p[-1] == 'E')
		     && (*p == '-' || *p == '+'))
	      /* This is the sign of the exponent, not the end of the
		 number.  */
	      continue;
	    /* We will take any letters or digits.  parse_number will
	       complain if past the radix, or if L or U are not final.  */
	    else if ((*p < '0' || *p > '9')
		     && ((*p < 'a' || *p > 'z')
				  && (*p < 'A' || *p > 'Z')))
	      break;
	  }
	toktype = parse_number (tokstart, p - tokstart, got_dot|got_e, &yylval);
        if (toktype == ERROR)
	  {
	    char *err_copy = (char *) alloca (p - tokstart + 1);

	    memcpy (err_copy, tokstart, p - tokstart);
	    err_copy[p - tokstart] = 0;
	    error ("Invalid number \"%s\".", err_copy);
	  }
	lexptr = p;
	return toktype;
      }

    case '+':
    case '-':
    case '*':
    case '/':
    case '%':
    case '|':
    case '&':
    case '^':
    case '~':
    case '!':
    case '@':
    case '<':
    case '>':
    case '[':
    case ']':
    case '?':
    case ':':
    case '=':
    case '{':
    case '}':
    symbol:
      lexptr++;
      return c;

    case '"':

      /* Build the gdb internal form of the input string in tempbuf,
	 translating any standard C escape forms seen.  Note that the
	 buffer is null byte terminated *only* for the convenience of
	 debugging gdb itself and printing the buffer contents when
	 the buffer contains no embedded nulls.  Gdb does not depend
	 upon the buffer being null byte terminated, it uses the length
	 string instead.  This allows gdb to handle C strings (as well
	 as strings in other languages) with embedded null bytes */

      tokptr = ++tokstart;
      tempbufindex = 0;

      do {
	/* Grow the static temp buffer if necessary, including allocating
	   the first one on demand. */
	if (tempbufindex + 1 >= tempbufsize)
	  {
	    tempbuf = (char *) xrealloc (tempbuf, tempbufsize += 64);
	  }
	switch (*tokptr)
	  {
	  case '\0':
	  case '"':
	    /* Do nothing, loop will terminate. */
	    break;
	  case '\\':
	    tokptr++;
	    c = parse_escape (&tokptr);
	    if (c == -1)
	      {
		continue;
	      }
	    tempbuf[tempbufindex++] = c;
	    break;
	  default:
	    tempbuf[tempbufindex++] = *tokptr++;
	    break;
	  }
      } while ((*tokptr != '"') && (*tokptr != '\0'));
      if (*tokptr++ != '"')
	{
	  error ("Unterminated string in expression.");
	}
      tempbuf[tempbufindex] = '\0';	/* See note above */
      yylval.sval.ptr = tempbuf;
      yylval.sval.length = tempbufindex;
      lexptr = tokptr;
      return (STRING);
    }

  if (!(c == '_' || c == '$'
	|| (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z')))
    /* We must have come across a bad character (e.g. ';').  */
    error ("Invalid character '%c' in expression.", c);

  /* It's a name.  See how long it is.  */
  namelen = 0;
  for (c = tokstart[namelen];
       (c == '_' || c == '$' || (c >= '0' && c <= '9')
	|| (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z'));
       c = tokstart[++namelen])
    ;

  /* The token "if" terminates the expression and is NOT 
     removed from the input stream.  */
  if (namelen == 2 && tokstart[0] == 'i' && tokstart[1] == 'f')
    {
      return 0;
    }

  lexptr += namelen;

  /* Handle the tokens $digits; also $ (short for $0) and $$ (short for $$1)
     and $$digits (equivalent to $<-digits> if you could type that).
     Make token type LAST, and put the number (the digits) in yylval.  */

  tryname:
  if (*tokstart == '$')
    {
      register int negate = 0;
      c = 1;
      /* Double dollar means negate the number and add -1 as well.
	 Thus $$ alone means -1.  */
      if (namelen >= 2 && tokstart[1] == '$')
	{
	  negate = 1;
	  c = 2;
	}
      if (c == namelen)
	{
	  /* Just dollars (one or two) */
	  yylval.lval = - negate;
	  return LAST;
	}
      /* Is the rest of the token digits?  */
      for (; c < namelen; c++)
	if (!(tokstart[c] >= '0' && tokstart[c] <= '9'))
	  break;
      if (c == namelen)
	{
	  yylval.lval = atoi (tokstart + 1 + negate);
	  if (negate)
	    yylval.lval = - yylval.lval;
	  return LAST;
	}
    }

  /* Handle tokens that refer to machine registers:
     $ followed by a register name.  */

  if (*tokstart == '$') {
    for (c = 0; c < NUM_REGS; c++)
      if (namelen - 1 == strlen (reg_names[c])
	  && STREQN (tokstart + 1, reg_names[c], namelen - 1))
	{
	  yylval.lval = c;
	  return REGNAME;
	}
    for (c = 0; c < num_std_regs; c++)
     if (namelen - 1 == strlen (std_regs[c].name)
	 && STREQN (tokstart + 1, std_regs[c].name, namelen - 1))
       {
	 yylval.lval = std_regs[c].regnum;
	 return REGNAME;
       }
  }
  /* Catch specific keywords.  Should be done with a data structure.  */
  switch (namelen)
    {
    case 8:
      if (STREQN (tokstart, "unsigned", 8))
	return UNSIGNED;
      if (current_language->la_language == language_cplus
	  && STREQN (tokstart, "template", 8))
	return TEMPLATE;
      if (STREQN (tokstart, "volatile", 8))
	return VOLATILE_KEYWORD;
      break;
    case 6:
      if (STREQN (tokstart, "struct", 6))
	return STRUCT;
      if (STREQN (tokstart, "signed", 6))
	return SIGNED_KEYWORD;
      if (STREQN (tokstart, "sizeof", 6))      
	return SIZEOF;
      break;
    case 5:
      if (current_language->la_language == language_cplus
	  && STREQN (tokstart, "class", 5))
	return CLASS;
      if (STREQN (tokstart, "union", 5))
	return UNION;
      if (STREQN (tokstart, "short", 5))
	return SHORT;
      if (STREQN (tokstart, "const", 5))
	return CONST_KEYWORD;
      break;
    case 4:
      if (STREQN (tokstart, "enum", 4))
	return ENUM;
      if (STREQN (tokstart, "long", 4))
	return LONG;
      if (current_language->la_language == language_cplus
	  && STREQN (tokstart, "this", 4))
	{
	  static const char this_name[] =
				 { CPLUS_MARKER, 't', 'h', 'i', 's', '\0' };

	  if (lookup_symbol (this_name, expression_context_block,
			     VAR_NAMESPACE, (int *) NULL,
			     (struct symtab **) NULL))
	    return THIS;
	}
      break;
    case 3:
      if (STREQN (tokstart, "int", 3))
	return INT_KEYWORD;
      break;
    default:
      break;
    }

  yylval.sval.ptr = tokstart;
  yylval.sval.length = namelen;

  /* Any other names starting in $ are debugger internal variables.  */

  if (*tokstart == '$')
    {
      yylval.ivar =  lookup_internalvar (copy_name (yylval.sval) + 1);
      return VARIABLE;
    }

  /* Use token-type BLOCKNAME for symbols that happen to be defined as
     functions or symtabs.  If this is not so, then ...
     Use token-type TYPENAME for symbols that happen to be defined
     currently as names of types; NAME for other symbols.
     The caller is not constrained to care about the distinction.  */
  {
    char *tmp = copy_name (yylval.sval);
    struct symbol *sym;
    int is_a_field_of_this = 0;
    int hextype;

    sym = lookup_symbol (tmp, expression_context_block,
			 VAR_NAMESPACE,
			 current_language->la_language == language_cplus
			 ? &is_a_field_of_this : (int *) NULL,
			 (struct symtab **) NULL);
    /* Call lookup_symtab, not lookup_partial_symtab, in case there are
       no psymtabs (coff, xcoff, or some future change to blow away the
       psymtabs once once symbols are read).  */
    if ((sym && SYMBOL_CLASS (sym) == LOC_BLOCK) ||
        lookup_symtab (tmp))
      {
	yylval.ssym.sym = sym;
	yylval.ssym.is_a_field_of_this = is_a_field_of_this;
	return BLOCKNAME;
      }
    if (sym && SYMBOL_CLASS (sym) == LOC_TYPEDEF)
        {
#if 1
	  /* Despite the following flaw, we need to keep this code enabled.
	     Because we can get called from check_stub_method, if we don't
	     handle nested types then it screws many operations in any
	     program which uses nested types.  */
	  /* In "A::x", if x is a member function of A and there happens
	     to be a type (nested or not, since the stabs don't make that
	     distinction) named x, then this code incorrectly thinks we
	     are dealing with nested types rather than a member function.  */

	  char *p;
	  char *namestart;
	  struct symbol *best_sym;

	  /* Look ahead to detect nested types.  This probably should be
	     done in the grammar, but trying seemed to introduce a lot
	     of shift/reduce and reduce/reduce conflicts.  It's possible
	     that it could be done, though.  Or perhaps a non-grammar, but
	     less ad hoc, approach would work well.  */

	  /* Since we do not currently have any way of distinguishing
	     a nested type from a non-nested one (the stabs don't tell
	     us whether a type is nested), we just ignore the
	     containing type.  */

	  p = lexptr;
	  best_sym = sym;
	  while (1)
	    {
	      /* Skip whitespace.  */
	      while (*p == ' ' || *p == '\t' || *p == '\n')
		++p;
	      if (*p == ':' && p[1] == ':')
		{
		  /* Skip the `::'.  */
		  p += 2;
		  /* Skip whitespace.  */
		  while (*p == ' ' || *p == '\t' || *p == '\n')
		    ++p;
		  namestart = p;
		  while (*p == '_' || *p == '$' || (*p >= '0' && *p <= '9')
			 || (*p >= 'a' && *p <= 'z')
			 || (*p >= 'A' && *p <= 'Z'))
		    ++p;
		  if (p != namestart)
		    {
		      struct symbol *cur_sym;
		      /* As big as the whole rest of the expression, which is
			 at least big enough.  */
		      char *tmp = (char *) alloca (strlen (namestart)+1);

		      memcpy (tmp, namestart, p - namestart);
		      tmp[p - namestart] = '\0';
		      cur_sym = lookup_symbol (tmp, expression_context_block,
					       VAR_NAMESPACE, (int *) NULL,
					       (struct symtab **) NULL);
		      if (cur_sym)
			{
			  if (SYMBOL_CLASS (cur_sym) == LOC_TYPEDEF)
			    {
			      best_sym = cur_sym;
			      lexptr = p;
			    }
			  else
			    break;
			}
		      else
			break;
		    }
		  else
		    break;
		}
	      else
		break;
	    }

	  yylval.tsym.type = SYMBOL_TYPE (best_sym);
#else /* not 0 */
	  yylval.tsym.type = SYMBOL_TYPE (sym);
#endif /* not 0 */
	  return TYPENAME;
        }
    if ((yylval.tsym.type = lookup_primitive_typename (tmp)) != 0)
	return TYPENAME;

    /* Input names that aren't symbols but ARE valid hex numbers,
       when the input radix permits them, can be names or numbers
       depending on the parse.  Note we support radixes > 16 here.  */
    if (!sym && 
        ((tokstart[0] >= 'a' && tokstart[0] < 'a' + input_radix - 10) ||
         (tokstart[0] >= 'A' && tokstart[0] < 'A' + input_radix - 10)))
      {
 	YYSTYPE newlval;	/* Its value is ignored.  */
	hextype = parse_number (tokstart, namelen, 0, &newlval);
	if (hextype == INT)
	  {
	    yylval.ssym.sym = sym;
	    yylval.ssym.is_a_field_of_this = is_a_field_of_this;
	    return NAME_OR_INT;
	  }
      }

    /* Any other kind of symbol */
    yylval.ssym.sym = sym;
    yylval.ssym.is_a_field_of_this = is_a_field_of_this;
    return NAME;
  }
}

void
yyerror (msg)
     char *msg;
{
  error ("A %s in expression, near `%s'.", (msg ? msg : "error"), lexptr);
}
__YYSCLASS yytabelem yyexca[] ={
-1, 1,
	0, -1,
	-2, 0,
-1, 45,
	269, 66,
	-2, 131,
-1, 131,
	269, 96,
	-2, 128,
-1, 184,
	269, 67,
	-2, 68,
	};
# define YYNPROD 132
# define YYLAST 1009
__YYSCLASS yytabelem yyact[]={

     9,    57,   205,   188,    53,     7,    55,    15,   185,     6,
    51,    56,     8,   168,   104,   106,   107,    32,   111,    37,
    38,    39,    40,   187,    36,   186,    42,   108,    41,    33,
    35,    34,    43,    44,   169,   182,   159,    53,   102,   110,
     9,   109,    86,    51,   115,     7,   161,    85,   115,     6,
   105,   163,     8,    99,   204,    52,   122,   123,   116,   189,
   112,   113,   116,    47,   100,   176,   162,    96,   210,    98,
    47,    95,   207,    90,     2,   161,   201,    29,   166,   202,
     9,    47,   169,   200,   195,   165,   136,    15,    52,   114,
    27,    28,    31,    10,    25,    97,   179,    14,    57,    68,
    19,    53,     1,    55,    58,     3,    59,    51,    56,    94,
     0,     0,   192,     0,     0,     0,   101,     0,   168,   194,
   101,    66,    74,    67,    73,    54,   135,   163,   129,   133,
    27,   121,     0,    10,    57,    68,     0,    53,     0,    55,
    58,   134,    59,    51,    56,     0,     0,   104,   106,   107,
     0,   174,    52,     0,   164,    69,     0,    66,    74,    67,
    73,    54,   175,     0,     0,     0,    57,     0,     0,    53,
    27,    55,    58,    10,    59,    51,    56,     0,     0,   193,
     0,     0,     0,   105,     0,    70,     0,     0,    52,     0,
     0,    69,     0,    54,     0,     0,     0,     0,     0,   183,
     0,     0,   170,   171,     0,   172,   173,     0,   177,     0,
     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
    52,    70,     0,     0,    16,    18,    23,    46,    32,    17,
    37,    38,    39,    40,    13,    36,    30,    42,     0,    41,
    33,    35,    34,    43,    44,    20,    21,    22,     0,    24,
     0,     0,   104,   106,   107,     0,     0,    48,    49,    50,
    11,    12,     0,    45,    16,    18,    23,    46,    32,    17,
    37,    38,    39,    40,    13,    36,    30,    42,     0,    41,
    33,    35,    34,    43,    44,    20,    21,    22,   105,    24,
    48,    49,    50,     0,     0,     0,     0,     0,     0,     0,
    11,    12,     0,    45,    16,    18,    23,    46,    32,    17,
    37,    38,    39,    40,    13,    36,    30,    42,     0,    41,
    33,    35,    34,    43,    44,    20,    21,    22,     0,    24,
     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
    11,    12,    75,    45,   130,    72,    71,    62,    63,    64,
    65,    60,    61,     0,    48,    49,    50,     0,     0,   104,
   131,   107,    37,    38,    39,    40,     0,    36,     0,    42,
     0,    41,    33,    35,    34,    43,    44,   178,    75,     0,
     0,    72,    71,    62,    63,    64,    65,    60,    61,     0,
    48,    49,    50,    57,    68,   105,    53,     0,    55,    58,
     0,    59,    51,    56,    57,    68,     0,    53,     0,    55,
    58,     0,    59,    51,    56,     0,    66,     0,    67,    73,
    54,     0,    48,    49,    50,     0,     0,    66,     0,    67,
     0,    54,    57,    68,     0,    53,     0,    55,    58,     0,
    59,    51,    56,    57,    68,     0,    53,    52,    55,    58,
    69,    59,    51,    56,     0,    66,     0,    67,    52,    54,
     0,    69,     0,     0,     0,     0,    66,     0,    67,    57,
    54,     0,    53,     0,    55,    58,     0,    59,    51,    56,
    70,    96,     0,    98,   178,    95,    52,     0,     0,    69,
     0,    70,     0,    57,    68,     0,    53,    52,    55,    58,
    69,    59,    51,    56,    57,     0,     0,    53,     0,    55,
    58,     0,    59,    51,    56,     0,    66,     0,    67,    70,
    54,     0,     0,    52,     0,     0,     0,    66,     0,    67,
    57,    54,     0,    53,   101,    55,    58,     0,    59,    51,
    56,    96,     0,    98,     0,    95,     0,    52,     0,     0,
     0,     0,     0,    66,     0,    67,     0,    54,    52,     0,
     0,     0,   104,   131,   107,    37,    38,    39,    40,    96,
    36,    98,    42,    95,    41,    33,    35,    34,    43,    44,
    57,     0,     0,    53,    52,    55,    58,     0,    59,    51,
    56,     0,     0,     0,   101,     0,     0,    32,   105,    37,
    38,    39,    40,     0,    36,     0,    42,    54,    41,    33,
    35,    34,    43,    44,     0,     0,     0,     0,     0,     0,
     0,     0,   101,     0,     0,     0,     0,     0,   167,     0,
     0,     0,     0,     0,    52,     0,     0,     0,     0,     0,
    72,    71,    62,    63,    64,    65,    60,    61,     0,    48,
    49,    50,    71,    62,    63,    64,    65,    60,    61,   103,
    48,    49,    50,     0,     0,     0,   117,   118,   119,   120,
     0,   124,     0,     0,     0,     0,     0,     0,     0,   128,
   132,    62,    63,    64,    65,    60,    61,     0,    48,    49,
    50,     0,    62,    63,    64,    65,    60,    61,     0,    48,
    49,    50,     0,     0,    32,     0,    37,    38,    39,    40,
     0,    36,     0,    42,     0,    41,    33,    35,    34,    43,
    44,     0,     0,     0,     0,    48,    49,    50,     0,     0,
     0,   184,     0,     0,     0,     0,     0,     0,     0,     0,
     0,     0,    62,    63,    64,    65,    60,    61,     0,    48,
    49,    50,     0,    62,    63,    64,    65,    60,    61,     0,
    48,    49,    50,     0,     0,     0,     0,     0,     0,     0,
     0,     0,   203,     0,     0,     0,     0,     0,     0,    92,
    93,    64,    65,    60,    61,     0,    48,    49,    50,     0,
     0,     0,     0,     0,     0,     0,     0,   199,     0,     0,
    91,     0,     4,    77,    26,   180,     5,    92,    93,    76,
    78,    79,    80,    81,    82,    83,    84,    88,    26,    26,
    87,    89,     0,     0,     0,     0,     0,     0,     0,     0,
     0,     0,     0,    60,    61,     0,    48,    49,    50,     0,
     0,     0,     0,     0,     0,     0,     0,   125,   126,     0,
   127,     0,     0,     0,     0,     0,     0,   137,   138,   139,
   140,   141,   142,   143,   144,   145,   146,   147,   148,   149,
   150,   151,   152,   153,   154,   155,   156,   157,   158,     0,
     0,     0,     0,     0,     0,     0,     0,     0,     0,    26,
     0,   160,     0,     0,     0,     0,     0,     0,     0,     0,
     0,     0,   181,     0,     0,     0,     0,     0,     0,     0,
     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     0,     0,     0,   190,     0,     0,     0,   191,     0,    88,
     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     0,     0,     0,     0,   196,     0,     0,   197,   198,     0,
     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
   181,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     0,     0,     0,   181,     0,   206,     0,   208,   198,     0,
     0,     0,     0,     0,     0,     0,   181,     0,   209 };
__YYSCLASS yytabelem yypact[]={

   -33, -3000,    26, -3000,    97, -3000,   -33,   -33,   -33,   -33,
   -33,   -33,   -33,     7,   -33,   -33, -3000, -3000, -3000, -3000,
 -3000, -3000, -3000, -3000, -3000, -3000,   531, -3000,  -231, -3000,
  -246, -3000, -3000,  -234, -3000,  -257,  -213,  -246,  -246,  -246,
  -246,  -217,  -246,  -244,  -244, -3000, -3000,   -33, -3000, -3000,
   302,    99,   -33, -3000,   -33,   -33,   -33,   -33,   -33,   -33,
   -33,   -33,   -33,   -33,   -33,   -33,   -33,   -33,   -33,   -33,
   -33,   -33,   -33,   -33,   -33,   -33,    -3,  -233,    -3,    -3,
    -3,    -3,    -3,    -3,    -3,   -33,     2,   -74,    97,    44,
    37,    -8,    29,    29, -3000,    29,    29,    25,   443, -3000,
 -3000,   -58,  -246, -3000, -3000, -3000, -3000, -3000, -3000, -3000,
  -267, -3000,  -250,  -272, -3000, -3000, -3000, -3000, -3000, -3000,
 -3000, -3000, -3000, -3000,    -1, -3000, -3000,    97, -3000, -3000,
   -33, -3000, -3000, -3000,   -33,    19,   -33,   432,    -3,    -3,
    -3,   -36,   -36,   129,   129,   493,   493,   543,   543,   543,
   543,   467,   456,   406,   395,   367,    61,    97,    97,  -113,
    43,   -33, -3000, -3000,   -33,   -33, -3000, -3000,  -246, -3000,
 -3000, -3000, -3000, -3000, -3000, -3000,   336,    42, -3000,    35,
 -3000,   503, -3000,   -39, -3000, -3000, -3000,  -273, -3000,  -244,
    -3,    -3, -3000,    31,   -33,    47,    97,    -3,    -3, -3000,
 -3000, -3000,  -244,    40, -3000, -3000,     6, -3000,   356, -3000,
 -3000 };
__YYSCLASS yytabelem yypgo[]={

     0,   802,    73,   105,   102,   100,    77,    97,    66,   805,
   803,    96,    64,    95,   109,    94,    53,   628,    92,    89,
    91,    86,    42 };
__YYSCLASS yytabelem yyr1[]={

     0,     4,     4,     3,     2,     2,     1,     1,     1,     1,
     1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
     1,     1,     1,    21,     1,     7,    22,    22,    22,     8,
     1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
     1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
     1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
     1,     1,     1,     1,     1,     1,    20,    20,     5,     6,
     6,     5,     5,     5,    15,    15,    15,    15,    15,    15,
    14,    14,    14,    14,    14,    13,    13,    13,    13,    13,
    16,    16,    12,    12,     9,     9,    10,    10,    10,    10,
    10,    10,    10,    10,    10,    10,    10,    10,    10,    10,
    10,    10,    10,    10,    10,    10,    10,    10,    10,    10,
    19,    19,    19,    19,    11,    11,    17,    17,    17,    17,
    18,    18 };
__YYSCLASS yytabelem yyr2[]={

     0,     2,     2,     3,     2,     7,     5,     5,     5,     5,
     5,     5,     5,     5,     5,     5,     7,     7,     9,     7,
     7,     9,     9,     1,    11,     3,     0,     3,     7,     3,
     7,     9,     9,     7,     7,     7,     7,     7,     7,     7,
     7,     7,     7,     7,     7,     7,     7,     7,     7,     7,
     7,     7,     7,    11,     7,     7,     3,     3,     3,     2,
     3,     3,     3,     9,     3,     3,     3,     7,     7,     7,
     9,     2,     5,     3,     2,     4,     4,     5,     7,     7,
     3,     5,     3,     5,     2,     7,     5,     3,     5,     3,
     5,     7,     5,     7,     2,     7,     3,     5,     3,     3,
     3,     5,     7,     5,     7,     7,     9,     5,     7,     5,
     5,     5,     5,     5,     3,     5,     3,    11,     5,     5,
     2,     3,     3,     3,     3,     7,     3,     3,     3,     3,
     2,     2 };
__YYSCLASS yytabelem yychk[]={

 -3000,    -4,    -2,    -3,    -1,    -9,    42,    38,    45,    33,
   126,   293,   294,   267,    -7,    40,   257,   262,   258,    -5,
   278,   279,   280,   259,   282,   -15,   -10,   123,   -20,    -6,
   269,   -18,   261,   273,   275,   274,   268,   263,   264,   265,
   266,   272,   270,   276,   277,   296,   260,    44,   293,   294,
   295,    46,    91,    40,    64,    42,    47,    37,    43,    45,
   290,   291,   286,   287,   288,   289,    60,    62,    38,    94,
   124,   285,   284,    63,    61,   281,    -1,   -10,    -1,    -1,
    -1,    -1,    -1,    -1,    -1,    40,   -22,    -9,    -1,    -9,
    -2,   269,   276,   277,   -14,    42,    38,   -13,    40,   -16,
   -12,    91,   269,   -17,   260,   296,   261,   262,   261,   275,
   273,   275,   273,   274,   -19,   261,   275,   -17,   -17,   -17,
   -17,   -19,   273,   274,   -17,   -10,   -10,    -1,   -17,    -6,
    42,   261,   -17,    -6,    42,    -2,   -21,    -1,    -1,    -1,
    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   269,
    -9,    44,    -8,   125,    -8,    41,    41,   -17,   126,    42,
   -14,   -14,   -14,   -14,   -16,   -12,    40,   -14,    41,   -11,
    -9,   -10,    93,   257,   -17,   275,   275,   273,   275,    60,
    -1,    -1,    93,   -22,    58,    41,    -1,    -1,    -1,   -17,
    41,    41,    44,   269,    93,   275,    -9,    41,    -1,    -9,
    62 };
__YYSCLASS yytabelem yydef[]={

     0,    -2,     1,     2,     4,     3,     0,     0,     0,     0,
     0,     0,     0,     0,    26,     0,    56,    57,    58,    59,
    60,    61,    62,    64,    65,    94,    74,    25,     0,    71,
     0,    73,    96,    99,    98,   100,   114,     0,     0,     0,
     0,   116,     0,     0,     0,    -2,   130,     0,    13,    14,
     0,     0,     0,    23,     0,     0,     0,     0,     0,     0,
     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     0,     0,     0,     0,     0,     0,     6,     0,     7,     8,
     9,    10,    11,    12,    15,     0,     0,     0,    27,     0,
     0,     0,    75,    76,    77,    80,    82,    84,     0,    87,
    89,     0,     0,    72,   126,   127,   128,   129,    97,   101,
   103,   107,   122,   123,   113,   120,   121,   109,   110,   111,
   112,   115,   122,   123,     0,   118,   119,     5,    16,    17,
     0,    -2,    19,    20,     0,     0,    26,    34,    35,    36,
    37,    38,    39,    40,    41,    42,    43,    44,    45,    46,
    47,    48,    49,    50,    51,    52,     0,    54,    55,     0,
     0,     0,    30,    29,     0,     0,    33,    69,     0,    95,
    78,    79,    81,    83,    86,    88,     0,     0,    92,     0,
   124,    74,    90,     0,    -2,   104,   102,   105,   108,     0,
    18,    21,    22,     0,     0,    63,    28,    31,    32,    70,
    85,    93,     0,     0,    91,   106,     0,    24,    53,   125,
   117 };
typedef struct { char *t_name; int t_val; } yytoktype;
#ifndef YYDEBUG
#	define YYDEBUG	0	/* don't allow debugging */
#endif

#if YYDEBUG

__YYSCLASS yytoktype yytoks[] =
{
	"INT",	257,
	"FLOAT",	258,
	"STRING",	259,
	"NAME",	260,
	"TYPENAME",	261,
	"NAME_OR_INT",	262,
	"STRUCT",	263,
	"CLASS",	264,
	"UNION",	265,
	"ENUM",	266,
	"SIZEOF",	267,
	"UNSIGNED",	268,
	"COLONCOLON",	269,
	"TEMPLATE",	270,
	"ERROR",	271,
	"SIGNED_KEYWORD",	272,
	"LONG",	273,
	"SHORT",	274,
	"INT_KEYWORD",	275,
	"CONST_KEYWORD",	276,
	"VOLATILE_KEYWORD",	277,
	"LAST",	278,
	"REGNAME",	279,
	"VARIABLE",	280,
	"ASSIGN_MODIFY",	281,
	"THIS",	282,
	",",	44,
	"ABOVE_COMMA",	283,
	"=",	61,
	"?",	63,
	"OROR",	284,
	"ANDAND",	285,
	"|",	124,
	"^",	94,
	"&",	38,
	"EQUAL",	286,
	"NOTEQUAL",	287,
	"<",	60,
	">",	62,
	"LEQ",	288,
	"GEQ",	289,
	"LSH",	290,
	"RSH",	291,
	"@",	64,
	"+",	43,
	"-",	45,
	"*",	42,
	"/",	47,
	"%",	37,
	"UNARY",	292,
	"INCREMENT",	293,
	"DECREMENT",	294,
	"ARROW",	295,
	".",	46,
	"[",	91,
	"(",	40,
	"BLOCKNAME",	296,
	"-unknown-",	-1	/* ends search */
};

__YYSCLASS char * yyreds[] =
{
	"-no such reduction-",
	"start : exp1",
	"start : type_exp",
	"type_exp : type",
	"exp1 : exp",
	"exp1 : exp1 ',' exp",
	"exp : '*' exp",
	"exp : '&' exp",
	"exp : '-' exp",
	"exp : '!' exp",
	"exp : '~' exp",
	"exp : INCREMENT exp",
	"exp : DECREMENT exp",
	"exp : exp INCREMENT",
	"exp : exp DECREMENT",
	"exp : SIZEOF exp",
	"exp : exp ARROW name",
	"exp : exp ARROW qualified_name",
	"exp : exp ARROW '*' exp",
	"exp : exp '.' name",
	"exp : exp '.' qualified_name",
	"exp : exp '.' '*' exp",
	"exp : exp '[' exp1 ']'",
	"exp : exp '('",
	"exp : exp '(' arglist ')'",
	"lcurly : '{'",
	"arglist : /* empty */",
	"arglist : exp",
	"arglist : arglist ',' exp",
	"rcurly : '}'",
	"exp : lcurly arglist rcurly",
	"exp : lcurly type rcurly exp",
	"exp : '(' type ')' exp",
	"exp : '(' exp1 ')'",
	"exp : exp '@' exp",
	"exp : exp '*' exp",
	"exp : exp '/' exp",
	"exp : exp '%' exp",
	"exp : exp '+' exp",
	"exp : exp '-' exp",
	"exp : exp LSH exp",
	"exp : exp RSH exp",
	"exp : exp EQUAL exp",
	"exp : exp NOTEQUAL exp",
	"exp : exp LEQ exp",
	"exp : exp GEQ exp",
	"exp : exp '<' exp",
	"exp : exp '>' exp",
	"exp : exp '&' exp",
	"exp : exp '^' exp",
	"exp : exp '|' exp",
	"exp : exp ANDAND exp",
	"exp : exp OROR exp",
	"exp : exp '?' exp ':' exp",
	"exp : exp '=' exp",
	"exp : exp ASSIGN_MODIFY exp",
	"exp : INT",
	"exp : NAME_OR_INT",
	"exp : FLOAT",
	"exp : variable",
	"exp : LAST",
	"exp : REGNAME",
	"exp : VARIABLE",
	"exp : SIZEOF '(' type ')'",
	"exp : STRING",
	"exp : THIS",
	"block : BLOCKNAME",
	"block : block COLONCOLON name",
	"variable : block COLONCOLON name",
	"qualified_name : typebase COLONCOLON name",
	"qualified_name : typebase COLONCOLON '~' name",
	"variable : qualified_name",
	"variable : COLONCOLON name",
	"variable : name_not_typename",
	"ptype : typebase",
	"ptype : typebase CONST_KEYWORD",
	"ptype : typebase VOLATILE_KEYWORD",
	"ptype : typebase abs_decl",
	"ptype : typebase CONST_KEYWORD abs_decl",
	"ptype : typebase VOLATILE_KEYWORD abs_decl",
	"abs_decl : '*'",
	"abs_decl : '*' abs_decl",
	"abs_decl : '&'",
	"abs_decl : '&' abs_decl",
	"abs_decl : direct_abs_decl",
	"direct_abs_decl : '(' abs_decl ')'",
	"direct_abs_decl : direct_abs_decl array_mod",
	"direct_abs_decl : array_mod",
	"direct_abs_decl : direct_abs_decl func_mod",
	"direct_abs_decl : func_mod",
	"array_mod : '[' ']'",
	"array_mod : '[' INT ']'",
	"func_mod : '(' ')'",
	"func_mod : '(' nonempty_typelist ')'",
	"type : ptype",
	"type : typebase COLONCOLON '*'",
	"typebase : TYPENAME",
	"typebase : LONG TYPENAME",
	"typebase : INT_KEYWORD",
	"typebase : LONG",
	"typebase : SHORT",
	"typebase : LONG INT_KEYWORD",
	"typebase : UNSIGNED LONG INT_KEYWORD",
	"typebase : LONG LONG",
	"typebase : LONG LONG INT_KEYWORD",
	"typebase : UNSIGNED LONG LONG",
	"typebase : UNSIGNED LONG LONG INT_KEYWORD",
	"typebase : SHORT INT_KEYWORD",
	"typebase : UNSIGNED SHORT INT_KEYWORD",
	"typebase : STRUCT name",
	"typebase : CLASS name",
	"typebase : UNION name",
	"typebase : ENUM name",
	"typebase : UNSIGNED typename",
	"typebase : UNSIGNED",
	"typebase : SIGNED_KEYWORD typename",
	"typebase : SIGNED_KEYWORD",
	"typebase : TEMPLATE name '<' type '>'",
	"typebase : CONST_KEYWORD typebase",
	"typebase : VOLATILE_KEYWORD typebase",
	"typename : TYPENAME",
	"typename : INT_KEYWORD",
	"typename : LONG",
	"typename : SHORT",
	"nonempty_typelist : type",
	"nonempty_typelist : nonempty_typelist ',' type",
	"name : NAME",
	"name : BLOCKNAME",
	"name : TYPENAME",
	"name : NAME_OR_INT",
	"name_not_typename : NAME",
	"name_not_typename : BLOCKNAME",
};
#endif /* YYDEBUG */
#define YYFLAG  (-3000)
/* @(#) $Revision: 1.5 $ */    

/*
** Skeleton parser driver for yacc output
*/

#if defined(NLS) && !defined(NL_SETN)
#include <msgbuf.h>
#endif

#ifndef nl_msg
#define nl_msg(i,s) (s)
#endif

/*
** yacc user known macros and defines
*/
#define YYERROR		goto yyerrlab

#ifndef __RUNTIME_YYMAXDEPTH
#define YYACCEPT	return(0)
#define YYABORT		return(1)
#else
#define YYACCEPT	{free_stacks(); return(0);}
#define YYABORT		{free_stacks(); return(1);}
#endif

#define YYBACKUP( newtoken, newvalue )\
{\
	if ( yychar >= 0 || ( yyr2[ yytmp ] >> 1 ) != 1 )\
	{\
		yyerror( (nl_msg(30001,"syntax error - cannot backup")) );\
		goto yyerrlab;\
	}\
	yychar = newtoken;\
	yystate = *yyps;\
	yylval = newvalue;\
	goto yynewstate;\
}
#define YYRECOVERING()	(!!yyerrflag)
#ifndef YYDEBUG
#	define YYDEBUG	1	/* make debugging available */
#endif

/*
** user known globals
*/
int yydebug;			/* set to 1 to get debugging */

/*
** driver internal defines
*/
/* define for YYFLAG now generated by yacc program. */
/*#define YYFLAG		(FLAGVAL)*/

/*
** global variables used by the parser
*/
# ifndef __RUNTIME_YYMAXDEPTH
__YYSCLASS YYSTYPE yyv[ YYMAXDEPTH ];	/* value stack */
__YYSCLASS int yys[ YYMAXDEPTH ];		/* state stack */
# else
__YYSCLASS YYSTYPE *yyv;			/* pointer to xmalloc'ed value stack */
__YYSCLASS int *yys;			/* pointer to xmalloc'ed stack stack */

#if defined(__STDC__) || defined (__cplusplus)
#include <stdlib.h>
#else
#endif /* __STDC__ or __cplusplus */


static int allocate_stacks(); 
static void free_stacks();
# ifndef YYINCREMENT
# define YYINCREMENT (YYMAXDEPTH/2) + 10
# endif
# endif	/* __RUNTIME_YYMAXDEPTH */
long  yymaxdepth = YYMAXDEPTH;

__YYSCLASS YYSTYPE *yypv;			/* top of value stack */
__YYSCLASS int *yyps;			/* top of state stack */

__YYSCLASS int yystate;			/* current state */
__YYSCLASS int yytmp;			/* extra var (lasts between blocks) */

int yynerrs;			/* number of errors */
__YYSCLASS int yyerrflag;			/* error recovery flag */
int yychar;			/* current input token number */



/*
** yyparse - return 0 if worked, 1 if syntax error not recovered from
*/
int
yyparse()
{
	register YYSTYPE *yypvt;	/* top of value stack for $vars */

	/*
	** Initialize externals - yyparse may be called more than once
	*/
# ifdef __RUNTIME_YYMAXDEPTH
	if (allocate_stacks()) YYABORT;
# endif
	yypv = &yyv[-1];
	yyps = &yys[-1];
	yystate = 0;
	yytmp = 0;
	yynerrs = 0;
	yyerrflag = 0;
	yychar = -1;

	goto yystack;
	{
		register YYSTYPE *yy_pv;	/* top of value stack */
		register int *yy_ps;		/* top of state stack */
		register int yy_state;		/* current state */
		register int  yy_n;		/* internal state number info */

		/*
		** get globals into registers.
		** branch to here only if YYBACKUP was called.
		*/
	yynewstate:
		yy_pv = yypv;
		yy_ps = yyps;
		yy_state = yystate;
		goto yy_newstate;

		/*
		** get globals into registers.
		** either we just started, or we just finished a reduction
		*/
	yystack:
		yy_pv = yypv;
		yy_ps = yyps;
		yy_state = yystate;

		/*
		** top of for (;;) loop while no reductions done
		*/
	yy_stack:
		/*
		** put a state and value onto the stacks
		*/
#if YYDEBUG
		/*
		** if debugging, look up token value in list of value vs.
		** name pairs.  0 and negative (-1) are special values.
		** Note: linear search is used since time is not a real
		** consideration while debugging.
		*/
		if ( yydebug )
		{
			register int yy_i;

			printf( "State %d, token ", yy_state );
			if ( yychar == 0 )
				printf( "end-of-file\n" );
			else if ( yychar < 0 )
				printf( "-none-\n" );
			else
			{
				for ( yy_i = 0; yytoks[yy_i].t_val >= 0;
					yy_i++ )
				{
					if ( yytoks[yy_i].t_val == yychar )
						break;
				}
				printf( "%s\n", yytoks[yy_i].t_name );
			}
		}
#endif /* YYDEBUG */
		if ( ++yy_ps >= &yys[ yymaxdepth ] )	/* room on stack? */
		{
# ifndef __RUNTIME_YYMAXDEPTH
			yyerror( (nl_msg(30002,"yacc stack overflow")) );
			YYABORT;
# else
			/* save old stack bases to recalculate pointers */
			YYSTYPE * yyv_old = yyv;
			int * yys_old = yys;
			yymaxdepth += YYINCREMENT;
			yys = (int *) xrealloc(yys, yymaxdepth * sizeof(int));
			yyv = (YYSTYPE *) xrealloc(yyv, yymaxdepth * sizeof(YYSTYPE));
			if (yys==0 || yyv==0) {
			    yyerror( (nl_msg(30002,"yacc stack overflow")) );
			    YYABORT;
			    }
			/* Reset pointers into stack */
			yy_ps = (yy_ps - yys_old) + yys;
			yyps = (yyps - yys_old) + yys;
			yy_pv = (yy_pv - yyv_old) + yyv;
			yypv = (yypv - yyv_old) + yyv;
# endif

		}
		*yy_ps = yy_state;
		*++yy_pv = yyval;

		/*
		** we have a new state - find out what to do
		*/
	yy_newstate:
		if ( ( yy_n = yypact[ yy_state ] ) <= YYFLAG )
			goto yydefault;		/* simple state */
#if YYDEBUG
		/*
		** if debugging, need to mark whether new token grabbed
		*/
		yytmp = yychar < 0;
#endif
		if ( ( yychar < 0 ) && ( ( yychar = yylex() ) < 0 ) )
			yychar = 0;		/* reached EOF */
#if YYDEBUG
		if ( yydebug && yytmp )
		{
			register int yy_i;

			printf( "Received token " );
			if ( yychar == 0 )
				printf( "end-of-file\n" );
			else if ( yychar < 0 )
				printf( "-none-\n" );
			else
			{
				for ( yy_i = 0; yytoks[yy_i].t_val >= 0;
					yy_i++ )
				{
					if ( yytoks[yy_i].t_val == yychar )
						break;
				}
				printf( "%s\n", yytoks[yy_i].t_name );
			}
		}
#endif /* YYDEBUG */
		if ( ( ( yy_n += yychar ) < 0 ) || ( yy_n >= YYLAST ) )
			goto yydefault;
		if ( yychk[ yy_n = yyact[ yy_n ] ] == yychar )	/*valid shift*/
		{
			yychar = -1;
			yyval = yylval;
			yy_state = yy_n;
			if ( yyerrflag > 0 )
				yyerrflag--;
			goto yy_stack;
		}

	yydefault:
		if ( ( yy_n = yydef[ yy_state ] ) == -2 )
		{
#if YYDEBUG
			yytmp = yychar < 0;
#endif
			if ( ( yychar < 0 ) && ( ( yychar = yylex() ) < 0 ) )
				yychar = 0;		/* reached EOF */
#if YYDEBUG
			if ( yydebug && yytmp )
			{
				register int yy_i;

				printf( "Received token " );
				if ( yychar == 0 )
					printf( "end-of-file\n" );
				else if ( yychar < 0 )
					printf( "-none-\n" );
				else
				{
					for ( yy_i = 0;
						yytoks[yy_i].t_val >= 0;
						yy_i++ )
					{
						if ( yytoks[yy_i].t_val
							== yychar )
						{
							break;
						}
					}
					printf( "%s\n", yytoks[yy_i].t_name );
				}
			}
#endif /* YYDEBUG */
			/*
			** look through exception table
			*/
			{
				register int *yyxi = yyexca;

				while ( ( *yyxi != -1 ) ||
					( yyxi[1] != yy_state ) )
				{
					yyxi += 2;
				}
				while ( ( *(yyxi += 2) >= 0 ) &&
					( *yyxi != yychar ) )
					;
				if ( ( yy_n = yyxi[1] ) < 0 )
					YYACCEPT;
			}
		}

		/*
		** check for syntax error
		*/
		if ( yy_n == 0 )	/* have an error */
		{
			/* no worry about speed here! */
			switch ( yyerrflag )
			{
			case 0:		/* new error */
				yyerror( (nl_msg(30003,"syntax error")) );
				yynerrs++;
				goto skip_init;
			yyerrlab:
				/*
				** get globals into registers.
				** we have a user generated syntax type error
				*/
				yy_pv = yypv;
				yy_ps = yyps;
				yy_state = yystate;
				yynerrs++;
			skip_init:
			case 1:
			case 2:		/* incompletely recovered error */
					/* try again... */
				yyerrflag = 3;
				/*
				** find state where "error" is a legal
				** shift action
				*/
				while ( yy_ps >= yys )
				{
					yy_n = yypact[ *yy_ps ] + YYERRCODE;
					if ( yy_n >= 0 && yy_n < YYLAST &&
						yychk[yyact[yy_n]] == YYERRCODE)					{
						/*
						** simulate shift of "error"
						*/
						yy_state = yyact[ yy_n ];
						goto yy_stack;
					}
					/*
					** current state has no shift on
					** "error", pop stack
					*/
#if YYDEBUG
#	define _POP_ "Error recovery pops state %d, uncovers state %d\n"
					if ( yydebug )
						printf( _POP_, *yy_ps,
							yy_ps[-1] );
#	undef _POP_
#endif
					yy_ps--;
					yy_pv--;
				}
				/*
				** there is no state on stack with "error" as
				** a valid shift.  give up.
				*/
				YYABORT;
			case 3:		/* no shift yet; eat a token */
#if YYDEBUG
				/*
				** if debugging, look up token in list of
				** pairs.  0 and negative shouldn't occur,
				** but since timing doesn't matter when
				** debugging, it doesn't hurt to leave the
				** tests here.
				*/
				if ( yydebug )
				{
					register int yy_i;

					printf( "Error recovery discards " );
					if ( yychar == 0 )
						printf( "token end-of-file\n" );
					else if ( yychar < 0 )
						printf( "token -none-\n" );
					else
					{
						for ( yy_i = 0;
							yytoks[yy_i].t_val >= 0;
							yy_i++ )
						{
							if ( yytoks[yy_i].t_val
								== yychar )
							{
								break;
							}
						}
						printf( "token %s\n",
							yytoks[yy_i].t_name );
					}
				}
#endif /* YYDEBUG */
				if ( yychar == 0 )	/* reached EOF. quit */
					YYABORT;
				yychar = -1;
				goto yy_newstate;
			}
		}/* end if ( yy_n == 0 ) */
		/*
		** reduction by production yy_n
		** put stack tops, etc. so things right after switch
		*/
#if YYDEBUG
		/*
		** if debugging, print the string that is the user's
		** specification of the reduction which is just about
		** to be done.
		*/
		if ( yydebug )
			printf( "Reduce by (%d) \"%s\"\n",
				yy_n, yyreds[ yy_n ] );
#endif
		yytmp = yy_n;			/* value to switch over */
		yypvt = yy_pv;			/* $vars top of value stack */
		/*
		** Look in goto table for next state
		** Sorry about using yy_state here as temporary
		** register variable, but why not, if it works...
		** If yyr2[ yy_n ] doesn't have the low order bit
		** set, then there is no action to be done for
		** this reduction.  So, no saving & unsaving of
		** registers done.  The only difference between the
		** code just after the if and the body of the if is
		** the goto yy_stack in the body.  This way the test
		** can be made before the choice of what to do is needed.
		*/
		{
			/* length of production doubled with extra bit */
			register int yy_len = yyr2[ yy_n ];

			if ( !( yy_len & 01 ) )
			{
				yy_len >>= 1;
				yyval = ( yy_pv -= yy_len )[1];	/* $$ = $1 */
				yy_state = yypgo[ yy_n = yyr1[ yy_n ] ] +
					*( yy_ps -= yy_len ) + 1;
				if ( yy_state >= YYLAST ||
					yychk[ yy_state =
					yyact[ yy_state ] ] != -yy_n )
				{
					yy_state = yyact[ yypgo[ yy_n ] ];
				}
				goto yy_stack;
			}
			yy_len >>= 1;
			yyval = ( yy_pv -= yy_len )[1];	/* $$ = $1 */
			yy_state = yypgo[ yy_n = yyr1[ yy_n ] ] +
				*( yy_ps -= yy_len ) + 1;
			if ( yy_state >= YYLAST ||
				yychk[ yy_state = yyact[ yy_state ] ] != -yy_n )
			{
				yy_state = yyact[ yypgo[ yy_n ] ];
			}
		}
					/* save until reenter driver code */
		yystate = yy_state;
		yyps = yy_ps;
		yypv = yy_pv;
	}
	/*
	** code supplied by user is placed in this switch
	*/
	switch( yytmp )
	{
		
case 3:
# line 216 "c-exp.y"
{ write_exp_elt_opcode(OP_TYPE);
			  write_exp_elt_type(yypvt[-0].tval);
			  write_exp_elt_opcode(OP_TYPE);} break;
case 5:
# line 224 "c-exp.y"
{ write_exp_elt_opcode (BINOP_COMMA); } break;
case 6:
# line 229 "c-exp.y"
{ write_exp_elt_opcode (UNOP_IND); } break;
case 7:
# line 232 "c-exp.y"
{ write_exp_elt_opcode (UNOP_ADDR); } break;
case 8:
# line 235 "c-exp.y"
{ write_exp_elt_opcode (UNOP_NEG); } break;
case 9:
# line 239 "c-exp.y"
{ write_exp_elt_opcode (UNOP_LOGICAL_NOT); } break;
case 10:
# line 243 "c-exp.y"
{ write_exp_elt_opcode (UNOP_COMPLEMENT); } break;
case 11:
# line 247 "c-exp.y"
{ write_exp_elt_opcode (UNOP_PREINCREMENT); } break;
case 12:
# line 251 "c-exp.y"
{ write_exp_elt_opcode (UNOP_PREDECREMENT); } break;
case 13:
# line 255 "c-exp.y"
{ write_exp_elt_opcode (UNOP_POSTINCREMENT); } break;
case 14:
# line 259 "c-exp.y"
{ write_exp_elt_opcode (UNOP_POSTDECREMENT); } break;
case 15:
# line 263 "c-exp.y"
{ write_exp_elt_opcode (UNOP_SIZEOF); } break;
case 16:
# line 267 "c-exp.y"
{ write_exp_elt_opcode (STRUCTOP_PTR);
			  write_exp_string (yypvt[-0].sval);
			  write_exp_elt_opcode (STRUCTOP_PTR); } break;
case 17:
# line 273 "c-exp.y"
{ /* exp->type::name becomes exp->*(&type::name) */
			  /* Note: this doesn't work if name is a
			     static member!  FIXME */
			  write_exp_elt_opcode (UNOP_ADDR);
			  write_exp_elt_opcode (STRUCTOP_MPTR); } break;
case 18:
# line 280 "c-exp.y"
{ write_exp_elt_opcode (STRUCTOP_MPTR); } break;
case 19:
# line 284 "c-exp.y"
{ write_exp_elt_opcode (STRUCTOP_STRUCT);
			  write_exp_string (yypvt[-0].sval);
			  write_exp_elt_opcode (STRUCTOP_STRUCT); } break;
case 20:
# line 290 "c-exp.y"
{ /* exp.type::name becomes exp.*(&type::name) */
			  /* Note: this doesn't work if name is a
			     static member!  FIXME */
			  write_exp_elt_opcode (UNOP_ADDR);
			  write_exp_elt_opcode (STRUCTOP_MEMBER); } break;
case 21:
# line 298 "c-exp.y"
{ write_exp_elt_opcode (STRUCTOP_MEMBER); } break;
case 22:
# line 302 "c-exp.y"
{ write_exp_elt_opcode (BINOP_SUBSCRIPT); } break;
case 23:
# line 308 "c-exp.y"
{ start_arglist (); } break;
case 24:
# line 310 "c-exp.y"
{ write_exp_elt_opcode (OP_FUNCALL);
			  write_exp_elt_longcst ((LONGEST) end_arglist ());
			  write_exp_elt_opcode (OP_FUNCALL); } break;
case 25:
# line 316 "c-exp.y"
{ start_arglist (); } break;
case 27:
# line 323 "c-exp.y"
{ arglist_len = 1; } break;
case 28:
# line 327 "c-exp.y"
{ arglist_len++; } break;
case 29:
# line 331 "c-exp.y"
{ yyval.lval = end_arglist () - 1; } break;
case 30:
# line 334 "c-exp.y"
{ write_exp_elt_opcode (OP_ARRAY);
			  write_exp_elt_longcst ((LONGEST) 0);
			  write_exp_elt_longcst ((LONGEST) yypvt[-0].lval);
			  write_exp_elt_opcode (OP_ARRAY); } break;
case 31:
# line 341 "c-exp.y"
{ write_exp_elt_opcode (UNOP_MEMVAL);
			  write_exp_elt_type (yypvt[-2].tval);
			  write_exp_elt_opcode (UNOP_MEMVAL); } break;
case 32:
# line 347 "c-exp.y"
{ write_exp_elt_opcode (UNOP_CAST);
			  write_exp_elt_type (yypvt[-2].tval);
			  write_exp_elt_opcode (UNOP_CAST); } break;
case 33:
# line 353 "c-exp.y"
{ } break;
case 34:
# line 359 "c-exp.y"
{ write_exp_elt_opcode (BINOP_REPEAT); } break;
case 35:
# line 363 "c-exp.y"
{ write_exp_elt_opcode (BINOP_MUL); } break;
case 36:
# line 367 "c-exp.y"
{ write_exp_elt_opcode (BINOP_DIV); } break;
case 37:
# line 371 "c-exp.y"
{ write_exp_elt_opcode (BINOP_REM); } break;
case 38:
# line 375 "c-exp.y"
{ write_exp_elt_opcode (BINOP_ADD); } break;
case 39:
# line 379 "c-exp.y"
{ write_exp_elt_opcode (BINOP_SUB); } break;
case 40:
# line 383 "c-exp.y"
{ write_exp_elt_opcode (BINOP_LSH); } break;
case 41:
# line 387 "c-exp.y"
{ write_exp_elt_opcode (BINOP_RSH); } break;
case 42:
# line 391 "c-exp.y"
{ write_exp_elt_opcode (BINOP_EQUAL); } break;
case 43:
# line 395 "c-exp.y"
{ write_exp_elt_opcode (BINOP_NOTEQUAL); } break;
case 44:
# line 399 "c-exp.y"
{ write_exp_elt_opcode (BINOP_LEQ); } break;
case 45:
# line 403 "c-exp.y"
{ write_exp_elt_opcode (BINOP_GEQ); } break;
case 46:
# line 407 "c-exp.y"
{ write_exp_elt_opcode (BINOP_LESS); } break;
case 47:
# line 411 "c-exp.y"
{ write_exp_elt_opcode (BINOP_GTR); } break;
case 48:
# line 415 "c-exp.y"
{ write_exp_elt_opcode (BINOP_BITWISE_AND); } break;
case 49:
# line 419 "c-exp.y"
{ write_exp_elt_opcode (BINOP_BITWISE_XOR); } break;
case 50:
# line 423 "c-exp.y"
{ write_exp_elt_opcode (BINOP_BITWISE_IOR); } break;
case 51:
# line 427 "c-exp.y"
{ write_exp_elt_opcode (BINOP_LOGICAL_AND); } break;
case 52:
# line 431 "c-exp.y"
{ write_exp_elt_opcode (BINOP_LOGICAL_OR); } break;
case 53:
# line 435 "c-exp.y"
{ write_exp_elt_opcode (TERNOP_COND); } break;
case 54:
# line 439 "c-exp.y"
{ write_exp_elt_opcode (BINOP_ASSIGN); } break;
case 55:
# line 443 "c-exp.y"
{ write_exp_elt_opcode (BINOP_ASSIGN_MODIFY);
			  write_exp_elt_opcode (yypvt[-1].opcode);
			  write_exp_elt_opcode (BINOP_ASSIGN_MODIFY); } break;
case 56:
# line 449 "c-exp.y"
{ write_exp_elt_opcode (OP_LONG);
			  write_exp_elt_type (yypvt[-0].typed_val.type);
			  write_exp_elt_longcst ((LONGEST)(yypvt[-0].typed_val.val));
			  write_exp_elt_opcode (OP_LONG); } break;
case 57:
# line 456 "c-exp.y"
{ YYSTYPE val;
			  parse_number (yypvt[-0].ssym.stoken.ptr, yypvt[-0].ssym.stoken.length, 0, &val);
			  write_exp_elt_opcode (OP_LONG);
			  write_exp_elt_type (val.typed_val.type);
			  write_exp_elt_longcst ((LONGEST)val.typed_val.val);
			  write_exp_elt_opcode (OP_LONG);
			} break;
case 58:
# line 467 "c-exp.y"
{ write_exp_elt_opcode (OP_DOUBLE);
			  write_exp_elt_type (builtin_type_double);
			  write_exp_elt_dblcst (yypvt[-0].dval);
			  write_exp_elt_opcode (OP_DOUBLE); } break;
case 60:
# line 477 "c-exp.y"
{ write_exp_elt_opcode (OP_LAST);
			  write_exp_elt_longcst ((LONGEST) yypvt[-0].lval);
			  write_exp_elt_opcode (OP_LAST); } break;
case 61:
# line 483 "c-exp.y"
{ write_exp_elt_opcode (OP_REGISTER);
			  write_exp_elt_longcst ((LONGEST) yypvt[-0].lval);
			  write_exp_elt_opcode (OP_REGISTER); } break;
case 62:
# line 489 "c-exp.y"
{ write_exp_elt_opcode (OP_INTERNALVAR);
			  write_exp_elt_intern (yypvt[-0].ivar);
			  write_exp_elt_opcode (OP_INTERNALVAR); } break;
case 63:
# line 495 "c-exp.y"
{ write_exp_elt_opcode (OP_LONG);
			  write_exp_elt_type (builtin_type_int);
			  write_exp_elt_longcst ((LONGEST) TYPE_LENGTH (yypvt[-1].tval));
			  write_exp_elt_opcode (OP_LONG); } break;
case 64:
# line 502 "c-exp.y"
{ /* C strings are converted into array constants with
			     an explicit null byte added at the end.  Thus
			     the array upper bound is the string length.
			     There is no such thing in C as a completely empty
			     string. */
			  char *sp = yypvt[-0].sval.ptr; int count = yypvt[-0].sval.length;
			  while (count-- > 0)
			    {
			      write_exp_elt_opcode (OP_LONG);
			      write_exp_elt_type (builtin_type_char);
			      write_exp_elt_longcst ((LONGEST)(*sp++));
			      write_exp_elt_opcode (OP_LONG);
			    }
			  write_exp_elt_opcode (OP_LONG);
			  write_exp_elt_type (builtin_type_char);
			  write_exp_elt_longcst ((LONGEST)'\0');
			  write_exp_elt_opcode (OP_LONG);
			  write_exp_elt_opcode (OP_ARRAY);
			  write_exp_elt_longcst ((LONGEST) 0);
			  write_exp_elt_longcst ((LONGEST) (yypvt[-0].sval.length));
			  write_exp_elt_opcode (OP_ARRAY); } break;
case 65:
# line 527 "c-exp.y"
{ write_exp_elt_opcode (OP_THIS);
			  write_exp_elt_opcode (OP_THIS); } break;
case 66:
# line 534 "c-exp.y"
{
			  if (yypvt[-0].ssym.sym != 0)
			      yyval.bval = SYMBOL_BLOCK_VALUE (yypvt[-0].ssym.sym);
			  else
			    {
			      struct symtab *tem =
				  lookup_symtab (copy_name (yypvt[-0].ssym.stoken));
			      if (tem)
				yyval.bval = BLOCKVECTOR_BLOCK
					 (BLOCKVECTOR (tem), STATIC_BLOCK);
			      else
				error ("No file or function \"%s\".",
				       copy_name (yypvt[-0].ssym.stoken));
			    }
			} break;
case 67:
# line 552 "c-exp.y"
{ struct symbol *tem
			    = lookup_symbol (copy_name (yypvt[-0].sval), yypvt[-2].bval,
					     VAR_NAMESPACE, (int *) NULL,
					     (struct symtab **) NULL);
			  if (!tem || SYMBOL_CLASS (tem) != LOC_BLOCK)
			    error ("No function \"%s\" in specified context.",
				   copy_name (yypvt[-0].sval));
			  yyval.bval = SYMBOL_BLOCK_VALUE (tem); } break;
case 68:
# line 563 "c-exp.y"
{ struct symbol *sym;
			  sym = lookup_symbol (copy_name (yypvt[-0].sval), yypvt[-2].bval,
					       VAR_NAMESPACE, (int *) NULL,
					       (struct symtab **) NULL);
			  if (sym == 0)
			    error ("No symbol \"%s\" in specified context.",
				   copy_name (yypvt[-0].sval));

			  write_exp_elt_opcode (OP_VAR_VALUE);
			  /* block_found is set by lookup_symbol.  */
			  write_exp_elt_block (block_found);
			  write_exp_elt_sym (sym);
			  write_exp_elt_opcode (OP_VAR_VALUE); } break;
case 69:
# line 579 "c-exp.y"
{
			  struct type *type = yypvt[-2].tval;
			  if (TYPE_CODE (type) != TYPE_CODE_STRUCT
			      && TYPE_CODE (type) != TYPE_CODE_UNION)
			    error ("`%s' is not defined as an aggregate type.",
				   TYPE_NAME (type));

			  write_exp_elt_opcode (OP_SCOPE);
			  write_exp_elt_type (type);
			  write_exp_string (yypvt[-0].sval);
			  write_exp_elt_opcode (OP_SCOPE);
			} break;
case 70:
# line 592 "c-exp.y"
{
			  struct type *type = yypvt[-3].tval;
			  struct stoken tmp_token;
			  if (TYPE_CODE (type) != TYPE_CODE_STRUCT
			      && TYPE_CODE (type) != TYPE_CODE_UNION)
			    error ("`%s' is not defined as an aggregate type.",
				   TYPE_NAME (type));

			  if (!STREQ (type_name_no_tag (type), yypvt[-0].sval.ptr))
			    error ("invalid destructor `%s::~%s'",
				   type_name_no_tag (type), yypvt[-0].sval.ptr);

			  tmp_token.ptr = (char*) alloca (yypvt[-0].sval.length + 2);
			  tmp_token.length = yypvt[-0].sval.length + 1;
			  tmp_token.ptr[0] = '~';
			  memcpy (tmp_token.ptr+1, yypvt[-0].sval.ptr, yypvt[-0].sval.length);
			  tmp_token.ptr[tmp_token.length] = 0;
			  write_exp_elt_opcode (OP_SCOPE);
			  write_exp_elt_type (type);
			  write_exp_string (tmp_token);
			  write_exp_elt_opcode (OP_SCOPE);
			} break;
case 72:
# line 618 "c-exp.y"
{
			  char *name = copy_name (yypvt[-0].sval);
			  struct symbol *sym;
			  struct minimal_symbol *msymbol;

			  sym =
			    lookup_symbol (name, (const struct block *) NULL,
					   VAR_NAMESPACE, (int *) NULL,
					   (struct symtab **) NULL);
			  if (sym)
			    {
			      write_exp_elt_opcode (OP_VAR_VALUE);
			      write_exp_elt_block (NULL);
			      write_exp_elt_sym (sym);
			      write_exp_elt_opcode (OP_VAR_VALUE);
			      break;
			    }

			  msymbol = lookup_minimal_symbol (name,
				      (struct objfile *) NULL);
			  if (msymbol != NULL)
			    {
			      write_exp_msymbol (msymbol,
						 lookup_function_type (builtin_type_int),
						 builtin_type_int);
			    }
			  else
			    if (!have_full_symbols () && !have_partial_symbols ())
			      error ("No symbol table is loaded.  Use the \"file\" command.");
			    else
			      error ("No symbol \"%s\" in current context.", name);
			} break;
case 73:
# line 653 "c-exp.y"
{ struct symbol *sym = yypvt[-0].ssym.sym;

			  if (sym)
			    {
			      if (symbol_read_needs_frame (sym))
				{
				  if (innermost_block == 0 ||
				      contained_in (block_found, 
						    innermost_block))
				    innermost_block = block_found;
				}

			      write_exp_elt_opcode (OP_VAR_VALUE);
			      /* We want to use the selected frame, not
				 another more inner frame which happens to
				 be in the same block.  */
			      write_exp_elt_block (NULL);
			      write_exp_elt_sym (sym);
			      write_exp_elt_opcode (OP_VAR_VALUE);
			    }
			  else if (yypvt[-0].ssym.is_a_field_of_this)
			    {
			      /* C++: it hangs off of `this'.  Must
			         not inadvertently convert from a method call
				 to data ref.  */
			      if (innermost_block == 0 || 
				  contained_in (block_found, innermost_block))
				innermost_block = block_found;
			      write_exp_elt_opcode (OP_THIS);
			      write_exp_elt_opcode (OP_THIS);
			      write_exp_elt_opcode (STRUCTOP_PTR);
			      write_exp_string (yypvt[-0].ssym.stoken);
			      write_exp_elt_opcode (STRUCTOP_PTR);
			    }
			  else
			    {
			      struct minimal_symbol *msymbol;
			      register char *arg = copy_name (yypvt[-0].ssym.stoken);

			      msymbol = lookup_minimal_symbol (arg,
					  (struct objfile *) NULL);
			      if (msymbol != NULL)
				{
				  write_exp_msymbol (msymbol,
						     lookup_function_type (builtin_type_int),
						     builtin_type_int);
				}
			      else if (!have_full_symbols () && !have_partial_symbols ())
				error ("No symbol table is loaded.  Use the \"file\" command.");
			      else
				error ("No symbol \"%s\" in current context.",
				       copy_name (yypvt[-0].ssym.stoken));
			    }
			} break;
case 77:
# line 719 "c-exp.y"
{ yyval.tval = follow_types (yypvt[-1].tval); } break;
case 78:
# line 721 "c-exp.y"
{ yyval.tval = follow_types (yypvt[-2].tval); } break;
case 79:
# line 723 "c-exp.y"
{ yyval.tval = follow_types (yypvt[-2].tval); } break;
case 80:
# line 727 "c-exp.y"
{ push_type (tp_pointer); yyval.voidval = 0; } break;
case 81:
# line 729 "c-exp.y"
{ push_type (tp_pointer); yyval.voidval = yypvt[-0].voidval; } break;
case 82:
# line 731 "c-exp.y"
{ push_type (tp_reference); yyval.voidval = 0; } break;
case 83:
# line 733 "c-exp.y"
{ push_type (tp_reference); yyval.voidval = yypvt[-0].voidval; } break;
case 85:
# line 738 "c-exp.y"
{ yyval.voidval = yypvt[-1].voidval; } break;
case 86:
# line 740 "c-exp.y"
{
			  push_type_int (yypvt[-0].lval);
			  push_type (tp_array);
			} break;
case 87:
# line 745 "c-exp.y"
{
			  push_type_int (yypvt[-0].lval);
			  push_type (tp_array);
			  yyval.voidval = 0;
			} break;
case 88:
# line 752 "c-exp.y"
{ push_type (tp_function); } break;
case 89:
# line 754 "c-exp.y"
{ push_type (tp_function); } break;
case 90:
# line 758 "c-exp.y"
{ yyval.lval = -1; } break;
case 91:
# line 760 "c-exp.y"
{ yyval.lval = yypvt[-1].typed_val.val; } break;
case 92:
# line 764 "c-exp.y"
{ yyval.voidval = 0; } break;
case 93:
# line 766 "c-exp.y"
{ free ((PTR)yypvt[-1].tvec); yyval.voidval = 0; } break;
case 95:
# line 779 "c-exp.y"
{ yyval.tval = lookup_member_type (builtin_type_int, yypvt[-2].tval); } break;
case 96:
# line 784 "c-exp.y"
{ yyval.tval = yypvt[-0].tsym.type; } break;
case 97:
# line 786 "c-exp.y"
{ if (yypvt[-0].tsym.stoken.length == 6 &&
			      !strncmp("double",yypvt[-0].tsym.stoken.ptr,6))
				  yyval.tval = builtin_type_long_double;
			  else /* emit syntax error here. */
				  yyerror(NULL); } break;
case 98:
# line 792 "c-exp.y"
{ yyval.tval = builtin_type_int; } break;
case 99:
# line 794 "c-exp.y"
{ yyval.tval = builtin_type_long; } break;
case 100:
# line 796 "c-exp.y"
{ yyval.tval = builtin_type_short; } break;
case 101:
# line 798 "c-exp.y"
{ yyval.tval = builtin_type_long; } break;
case 102:
# line 800 "c-exp.y"
{ yyval.tval = builtin_type_unsigned_long; } break;
case 103:
# line 802 "c-exp.y"
{ yyval.tval = builtin_type_long_long; } break;
case 104:
# line 804 "c-exp.y"
{ yyval.tval = builtin_type_long_long; } break;
case 105:
# line 806 "c-exp.y"
{ yyval.tval = builtin_type_unsigned_long_long; } break;
case 106:
# line 808 "c-exp.y"
{ yyval.tval = builtin_type_unsigned_long_long; } break;
case 107:
# line 810 "c-exp.y"
{ yyval.tval = builtin_type_short; } break;
case 108:
# line 812 "c-exp.y"
{ yyval.tval = builtin_type_unsigned_short; } break;
case 109:
# line 814 "c-exp.y"
{ yyval.tval = lookup_struct (copy_name (yypvt[-0].sval),
					      expression_context_block); } break;
case 110:
# line 817 "c-exp.y"
{ yyval.tval = lookup_struct (copy_name (yypvt[-0].sval),
					      expression_context_block); } break;
case 111:
# line 820 "c-exp.y"
{ yyval.tval = lookup_union (copy_name (yypvt[-0].sval),
					     expression_context_block); } break;
case 112:
# line 823 "c-exp.y"
{ yyval.tval = lookup_enum (copy_name (yypvt[-0].sval),
					    expression_context_block); } break;
case 113:
# line 826 "c-exp.y"
{ yyval.tval = lookup_unsigned_typename (TYPE_NAME(yypvt[-0].tsym.type)); } break;
case 114:
# line 828 "c-exp.y"
{ yyval.tval = builtin_type_unsigned_int; } break;
case 115:
# line 830 "c-exp.y"
{ yyval.tval = lookup_signed_typename (TYPE_NAME(yypvt[-0].tsym.type)); } break;
case 116:
# line 832 "c-exp.y"
{ yyval.tval = builtin_type_int; } break;
case 117:
# line 834 "c-exp.y"
{ yyval.tval = lookup_template_type(copy_name(yypvt[-3].sval), yypvt[-1].tval,
						    expression_context_block);
			} break;
case 118:
# line 840 "c-exp.y"
{ yyval.tval = yypvt[-0].tval; } break;
case 119:
# line 841 "c-exp.y"
{ yyval.tval = yypvt[-0].tval; } break;
case 121:
# line 846 "c-exp.y"
{
		  yyval.tsym.stoken.ptr = "int";
		  yyval.tsym.stoken.length = 3;
		  yyval.tsym.type = builtin_type_int;
		} break;
case 122:
# line 852 "c-exp.y"
{
		  yyval.tsym.stoken.ptr = "long";
		  yyval.tsym.stoken.length = 4;
		  yyval.tsym.type = builtin_type_long;
		} break;
case 123:
# line 858 "c-exp.y"
{
		  yyval.tsym.stoken.ptr = "short";
		  yyval.tsym.stoken.length = 5;
		  yyval.tsym.type = builtin_type_short;
		} break;
case 124:
# line 867 "c-exp.y"
{ yyval.tvec = (struct type **) xmalloc (sizeof (struct type *) * 2);
		  yyval.ivec[0] = 1;	/* Number of types in vector */
		  yyval.tvec[1] = yypvt[-0].tval;
		} break;
case 125:
# line 872 "c-exp.y"
{ int len = sizeof (struct type *) * (++(yypvt[-2].ivec[0]) + 1);
		  yyval.tvec = (struct type **) xrealloc ((char *) yypvt[-2].tvec, len);
		  yyval.tvec[yyval.ivec[0]] = yypvt[-0].tval;
		} break;
case 126:
# line 878 "c-exp.y"
{ yyval.sval = yypvt[-0].ssym.stoken; } break;
case 127:
# line 879 "c-exp.y"
{ yyval.sval = yypvt[-0].ssym.stoken; } break;
case 128:
# line 880 "c-exp.y"
{ yyval.sval = yypvt[-0].tsym.stoken; } break;
case 129:
# line 881 "c-exp.y"
{ yyval.sval = yypvt[-0].ssym.stoken; } break;
	}
	goto yystack;		/* reset registers in driver code */
}

# ifdef __RUNTIME_YYMAXDEPTH

static int allocate_stacks() {
	/* allocate the yys and yyv stacks */
	yys = (int *) xmalloc(yymaxdepth * sizeof(int));
	yyv = (YYSTYPE *) xmalloc(yymaxdepth * sizeof(YYSTYPE));

	if (yys==0 || yyv==0) {
	   yyerror( (nl_msg(30004,"unable to allocate space for yacc stacks")) );
	   return(1);
	   }
	else return(0);

}


static void free_stacks() {
	if (yys!=0) free((char *) yys);
	if (yyv!=0) free((char *) yyv);
}

# endif  /* defined(__RUNTIME_YYMAXDEPTH) */

