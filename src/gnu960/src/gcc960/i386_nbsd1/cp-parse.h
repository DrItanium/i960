typedef union {long itype; tree ttype; char *strtype; enum tree_code code; } YYSTYPE;

#ifndef YYLTYPE
typedef
  struct yyltype
    {
      int timestamp;
      int first_line;
      int first_column;
      int last_line;
      int last_column;
      char *text;
   }
  yyltype;

#define YYLTYPE yyltype
#endif

#define	IDENTIFIER	258
#define	TYPENAME	259
#define	SCSPEC	260
#define	TYPESPEC	261
#define	TYPE_QUAL	262
#define	CONSTANT	263
#define	STRING	264
#define	ELLIPSIS	265
#define	SIZEOF	266
#define	ENUM	267
#define	IF	268
#define	ELSE	269
#define	WHILE	270
#define	DO	271
#define	FOR	272
#define	SWITCH	273
#define	CASE	274
#define	DEFAULT	275
#define	BREAK	276
#define	CONTINUE	277
#define	RETURN	278
#define	GOTO	279
#define	ASM_KEYWORD	280
#define	GCC_ASM_KEYWORD	281
#define	TYPEOF	282
#define	ALIGNOF	283
#define	HEADOF	284
#define	CLASSOF	285
#define	SIGOF	286
#define	ATTRIBUTE	287
#define	EXTENSION	288
#define	LABEL	289
#define	AGGR	290
#define	VISSPEC	291
#define	DELETE	292
#define	NEW	293
#define	OVERLOAD	294
#define	THIS	295
#define	OPERATOR	296
#define	CXX_TRUE	297
#define	CXX_FALSE	298
#define	LEFT_RIGHT	299
#define	TEMPLATE	300
#define	TYPEID	301
#define	DYNAMIC_CAST	302
#define	STATIC_CAST	303
#define	REINTERPRET_CAST	304
#define	CONST_CAST	305
#define	SCOPE	306
#define	EMPTY	307
#define	PTYPENAME	308
#define	ASSIGN	309
#define	OROR	310
#define	ANDAND	311
#define	MIN_MAX	312
#define	EQCOMPARE	313
#define	ARITHCOMPARE	314
#define	LSHIFT	315
#define	RSHIFT	316
#define	POINTSAT_STAR	317
#define	DOT_STAR	318
#define	UNARY	319
#define	PLUSPLUS	320
#define	MINUSMINUS	321
#define	HYPERUNARY	322
#define	PAREN_STAR_PAREN	323
#define	POINTSAT	324
#define	TRY	325
#define	CATCH	326
#define	THROW	327
#define	TYPENAME_ELLIPSIS	328
#define	PRE_PARSED_FUNCTION_DECL	329
#define	EXTERN_LANG_STRING	330
#define	ALL	331
#define	PRE_PARSED_CLASS_DECL	332
#define	TYPENAME_DEFN	333
#define	IDENTIFIER_DEFN	334
#define	PTYPENAME_DEFN	335
#define	END_OF_SAVED_INPUT	336


extern YYSTYPE yylval;
#define YYEMPTY		-2
