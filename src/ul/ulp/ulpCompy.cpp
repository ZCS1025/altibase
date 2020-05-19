
/* A Bison parser, made by GNU Bison 2.4.1.  */

/* Skeleton implementation for Bison's Yacc-like parsers in C
   
      Copyright (C) 1984, 1989, 1990, 2000, 2001, 2002, 2003, 2004, 2005, 2006
   Free Software Foundation, Inc.
   
   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.
   
   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.
   
   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.  */

/* As a special exception, you may create a larger work that contains
   part or all of the Bison parser skeleton and distribute that work
   under terms of your choice, so long as that work isn't itself a
   parser generator using the skeleton or a modified version thereof
   as a parser skeleton.  Alternatively, if you modify or redistribute
   the parser skeleton itself, you may (at your option) remove this
   special exception, which will cause the skeleton and the resulting
   Bison output files to be licensed under the GNU General Public
   License without this special exception.
   
   This special exception was added by the Free Software Foundation in
   version 2.2 of Bison.  */

/* C LALR(1) parser skeleton written by Richard Stallman, by
   simplifying the original so-called "semantic" parser.  */

/* All symbols defined below should begin with yy or YY, to avoid
   infringing on user name space.  This should be done even for local
   variables, as they might otherwise be expanded by user macros.
   There are some unavoidable exceptions within include files to
   define necessary library symbols; they are noted "INFRINGES ON
   USER NAME SPACE" below.  */

/* Identify Bison output.  */
#define YYBISON 1

/* Bison version.  */
#define YYBISON_VERSION "2.4.1"

/* Skeleton name.  */
#define YYSKELETON_NAME "yacc.c"

/* Pure parsers.  */
#define YYPURE 1

/* Push parsers.  */
#define YYPUSH 0

/* Pull parsers.  */
#define YYPULL 1

/* Using locations.  */
#define YYLSP_NEEDED 0

/* Substitute the variable and function names.  */
#define yyparse         COMPparse
#define yylex           COMPlex
#define yyerror         COMPerror
#define yylval          COMPlval
#define yychar          COMPchar
#define yydebug         COMPdebug
#define yynerrs         COMPnerrs


/* Copy the first part of user declarations.  */

/* Line 189 of yacc.c  */
#line 19 "ulpCompy.y"

#include <ulpComp.h>
#include <sqlcli.h>


/* Line 189 of yacc.c  */
#line 87 "ulpCompy.cpp"

/* Enabling traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
#endif

/* Enabling verbose error messages.  */
#ifdef YYERROR_VERBOSE
# undef YYERROR_VERBOSE
# define YYERROR_VERBOSE 1
#else
# define YYERROR_VERBOSE 0
#endif

/* Enabling the token table.  */
#ifndef YYTOKEN_TABLE
# define YYTOKEN_TABLE 0
#endif


/* Tokens.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
   /* Put the tokens into the symbol table, so that GDB and other debuggers
      know about them.  */
   enum yytokentype {
     END_OF_FILE = 258,
     C_AUTO = 259,
     C_BREAK = 260,
     C_CASE = 261,
     C_CHAR = 262,
     C_VARCHAR = 263,
     C_CONST = 264,
     C_CONTINUE = 265,
     C_DEFAULT = 266,
     C_DO = 267,
     C_DOUBLE = 268,
     C_ENUM = 269,
     C_EXTERN = 270,
     C_FLOAT = 271,
     C_FOR = 272,
     C_GOTO = 273,
     C_INT = 274,
     C_LONG = 275,
     C_REGISTER = 276,
     C_RETURN = 277,
     C_SHORT = 278,
     C_SIGNED = 279,
     C_SIZEOF = 280,
     C_STATIC = 281,
     C_STRUCT = 282,
     C_SWITCH = 283,
     C_TYPEDEF = 284,
     C_UNION = 285,
     C_UNSIGNED = 286,
     C_VOID = 287,
     C_VOLATILE = 288,
     C_WHILE = 289,
     C_ELIPSIS = 290,
     C_ELSE = 291,
     C_IF = 292,
     C_CONSTANT = 293,
     C_IDENTIFIER = 294,
     C_TYPE_NAME = 295,
     C_STRING_LITERAL = 296,
     C_RIGHT_ASSIGN = 297,
     C_LEFT_ASSIGN = 298,
     C_ADD_ASSIGN = 299,
     C_SUB_ASSIGN = 300,
     C_MUL_ASSIGN = 301,
     C_DIV_ASSIGN = 302,
     C_MOD_ASSIGN = 303,
     C_AND_ASSIGN = 304,
     C_XOR_ASSIGN = 305,
     C_OR_ASSIGN = 306,
     C_INC_OP = 307,
     C_DEC_OP = 308,
     C_PTR_OP = 309,
     C_AND_OP = 310,
     C_EQ_OP = 311,
     C_NE_OP = 312,
     C_RIGHT_OP = 313,
     C_LEFT_OP = 314,
     C_OR_OP = 315,
     C_LE_OP = 316,
     C_GE_OP = 317,
     C_APRE_BINARY = 318,
     C_APRE_BINARY2 = 319,
     C_APRE_BIT = 320,
     C_APRE_BYTES = 321,
     C_APRE_VARBYTES = 322,
     C_APRE_NIBBLE = 323,
     C_APRE_INTEGER = 324,
     C_APRE_NUMERIC = 325,
     C_APRE_BLOB_LOCATOR = 326,
     C_APRE_CLOB_LOCATOR = 327,
     C_APRE_BLOB = 328,
     C_APRE_CLOB = 329,
     C_SQLLEN = 330,
     C_SQL_TIMESTAMP_STRUCT = 331,
     C_SQL_TIME_STRUCT = 332,
     C_SQL_DATE_STRUCT = 333,
     C_SQL_DA_STRUCT = 334,
     C_FAILOVERCB = 335,
     C_NCHAR_CS = 336,
     C_ATTRIBUTE = 337,
     M_INCLUDE = 338,
     M_DEFINE = 339,
     M_UNDEF = 340,
     M_FUNCTION = 341,
     M_LBRAC = 342,
     M_RBRAC = 343,
     M_DQUOTE = 344,
     M_FILENAME = 345,
     M_IF = 346,
     M_ELIF = 347,
     M_ELSE = 348,
     M_ENDIF = 349,
     M_IFDEF = 350,
     M_IFNDEF = 351,
     M_CONSTANT = 352,
     M_IDENTIFIER = 353,
     EM_SQLSTART = 354,
     EM_ERROR = 355,
     TR_ADD = 356,
     TR_AFTER = 357,
     TR_AGER = 358,
     TR_ALL = 359,
     TR_ALTER = 360,
     TR_AND = 361,
     TR_ANY = 362,
     TR_ARCHIVE = 363,
     TR_ARCHIVELOG = 364,
     TR_AS = 365,
     TR_ASC = 366,
     TR_AT = 367,
     TR_BACKUP = 368,
     TR_BEFORE = 369,
     TR_BEGIN = 370,
     TR_BY = 371,
     TR_BIND = 372,
     TR_CASCADE = 373,
     TR_CASE = 374,
     TR_CAST = 375,
     TR_CHECK_OPENING_PARENTHESIS = 376,
     TR_CLOSE = 377,
     TR_COALESCE = 378,
     TR_COLUMN = 379,
     TR_COMMENT = 380,
     TR_COMMIT = 381,
     TR_COMPILE = 382,
     TR_CONNECT = 383,
     TR_CONSTRAINT = 384,
     TR_CONSTRAINTS = 385,
     TR_CONTINUE = 386,
     TR_CREATE = 387,
     TR_VOLATILE = 388,
     TR_CURSOR = 389,
     TR_CYCLE = 390,
     TR_DATABASE = 391,
     TR_DECLARE = 392,
     TR_DEFAULT = 393,
     TR_DELETE = 394,
     TR_DEQUEUE = 395,
     TR_DESC = 396,
     TR_DIRECTORY = 397,
     TR_DISABLE = 398,
     TR_DISCONNECT = 399,
     TR_DISTINCT = 400,
     TR_DROP = 401,
     TR_DESCRIBE = 402,
     TR_DESCRIPTOR = 403,
     TR_EACH = 404,
     TR_ELSE = 405,
     TR_ELSEIF = 406,
     TR_ENABLE = 407,
     TR_END = 408,
     TR_ENQUEUE = 409,
     TR_ESCAPE = 410,
     TR_EXCEPTION = 411,
     TR_EXEC = 412,
     TR_EXECUTE = 413,
     TR_EXIT = 414,
     TR_FAILOVERCB = 415,
     TR_FALSE = 416,
     TR_FETCH = 417,
     TR_FIFO = 418,
     TR_FLUSH = 419,
     TR_FOR = 420,
     TR_FOREIGN = 421,
     TR_FROM = 422,
     TR_FULL = 423,
     TR_FUNCTION = 424,
     TR_GOTO = 425,
     TR_GRANT = 426,
     TR_GROUP = 427,
     TR_HAVING = 428,
     TR_IF = 429,
     TR_IN = 430,
     TR_IN_BF_LPAREN = 431,
     TR_INNER = 432,
     TR_INSERT = 433,
     TR_INTERSECT = 434,
     TR_INTO = 435,
     TR_IS = 436,
     TR_ISOLATION = 437,
     TR_JOIN = 438,
     TR_KEY = 439,
     TR_LEFT = 440,
     TR_LESS = 441,
     TR_LEVEL = 442,
     TR_LIFO = 443,
     TR_LIKE = 444,
     TR_LIMIT = 445,
     TR_LOCAL = 446,
     TR_LOGANCHOR = 447,
     TR_LOOP = 448,
     TR_MERGE = 449,
     TR_MOVE = 450,
     TR_MOVEMENT = 451,
     TR_NEW = 452,
     TR_NOARCHIVELOG = 453,
     TR_NOCYCLE = 454,
     TR_NOT = 455,
     TR_NULL = 456,
     TR_OF = 457,
     TR_OFF = 458,
     TR_OLD = 459,
     TR_ON = 460,
     TR_OPEN = 461,
     TR_OR = 462,
     TR_ORDER = 463,
     TR_OUT = 464,
     TR_OUTER = 465,
     TR_OVER = 466,
     TR_PARTITION = 467,
     TR_PARTITIONS = 468,
     TR_POINTER = 469,
     TR_PRIMARY = 470,
     TR_PRIOR = 471,
     TR_PRIVILEGES = 472,
     TR_PROCEDURE = 473,
     TR_PUBLIC = 474,
     TR_QUEUE = 475,
     TR_READ = 476,
     TR_REBUILD = 477,
     TR_RECOVER = 478,
     TR_REFERENCES = 479,
     TR_REFERENCING = 480,
     TR_REGISTER = 481,
     TR_RESTRICT = 482,
     TR_RETURN = 483,
     TR_REVOKE = 484,
     TR_RIGHT = 485,
     TR_ROLLBACK = 486,
     TR_ROW = 487,
     TR_SAVEPOINT = 488,
     TR_SELECT = 489,
     TR_SEQUENCE = 490,
     TR_SESSION = 491,
     TR_SET = 492,
     TR_SOME = 493,
     TR_SPLIT = 494,
     TR_START = 495,
     TR_STATEMENT = 496,
     TR_SYNONYM = 497,
     TR_TABLE = 498,
     TR_TEMPORARY = 499,
     TR_THAN = 500,
     TR_THEN = 501,
     TR_TO = 502,
     TR_TRIGGER = 503,
     TR_TRUE = 504,
     TR_TYPE = 505,
     TR_TYPESET = 506,
     TR_UNION = 507,
     TR_UNIQUE = 508,
     TR_UNREGISTER = 509,
     TR_UNTIL = 510,
     TR_UPDATE = 511,
     TR_USER = 512,
     TR_USING = 513,
     TR_VALUES = 514,
     TR_VARIABLE = 515,
     TR_VARIABLES = 516,
     TR_VIEW = 517,
     TR_WHEN = 518,
     TR_WHERE = 519,
     TR_WHILE = 520,
     TR_WITH = 521,
     TR_WORK = 522,
     TR_WRITE = 523,
     TR_PARALLEL = 524,
     TR_NOPARALLEL = 525,
     TR_LOB = 526,
     TR_STORE = 527,
     TR_ENDEXEC = 528,
     TR_PRECEDING = 529,
     TR_FOLLOWING = 530,
     TR_CURRENT_ROW = 531,
     TR_LINK = 532,
     TR_ROLE = 533,
     TR_WITHIN = 534,
     TR_LOGGING = 535,
     TK_BETWEEN = 536,
     TK_EXISTS = 537,
     TO_ACCESS = 538,
     TO_CONSTANT = 539,
     TO_IDENTIFIED = 540,
     TO_INDEX = 541,
     TO_MINUS = 542,
     TO_MODE = 543,
     TO_OTHERS = 544,
     TO_RAISE = 545,
     TO_RENAME = 546,
     TO_REPLACE = 547,
     TO_ROWTYPE = 548,
     TO_SEGMENT = 549,
     TO_WAIT = 550,
     TO_PIVOT = 551,
     TO_UNPIVOT = 552,
     TO_MATERIALIZED = 553,
     TO_CONNECT_BY_NOCYCLE = 554,
     TO_CONNECT_BY_ROOT = 555,
     TO_NULLS = 556,
     TO_PURGE = 557,
     TO_FLASHBACK = 558,
     TO_VC2COLL = 559,
     TO_KEEP = 560,
     TA_ELSIF = 561,
     TA_EXTENTSIZE = 562,
     TA_FIXED = 563,
     TA_LOCK = 564,
     TA_MAXROWS = 565,
     TA_ONLINE = 566,
     TA_OFFLINE = 567,
     TA_REPLICATION = 568,
     TA_REVERSE = 569,
     TA_ROWCOUNT = 570,
     TA_STEP = 571,
     TA_TABLESPACE = 572,
     TA_TRUNCATE = 573,
     TA_SQLCODE = 574,
     TA_SQLERRM = 575,
     TA_LINKER = 576,
     TA_REMOTE_TABLE = 577,
     TA_SHARD = 578,
     TA_DISJOIN = 579,
     TA_CONJOIN = 580,
     TA_SEC = 581,
     TA_MSEC = 582,
     TA_USEC = 583,
     TA_SECOND = 584,
     TA_MILLISECOND = 585,
     TA_MICROSECOND = 586,
     TA_ANALYSIS_PROPAGATION = 587,
     TI_NONQUOTED_IDENTIFIER = 588,
     TI_QUOTED_IDENTIFIER = 589,
     TI_HOSTVARIABLE = 590,
     TL_TYPED_LITERAL = 591,
     TL_LITERAL = 592,
     TL_NCHAR_LITERAL = 593,
     TL_UNICODE_LITERAL = 594,
     TL_INTEGER = 595,
     TL_NUMERIC = 596,
     TS_AT_SIGN = 597,
     TS_CONCATENATION_SIGN = 598,
     TS_DOUBLE_PERIOD = 599,
     TS_EXCLAMATION_POINT = 600,
     TS_PERCENT_SIGN = 601,
     TS_OPENING_PARENTHESIS = 602,
     TS_CLOSING_PARENTHESIS = 603,
     TS_OPENING_BRACKET = 604,
     TS_CLOSING_BRACKET = 605,
     TS_ASTERISK = 606,
     TS_PLUS_SIGN = 607,
     TS_COMMA = 608,
     TS_MINUS_SIGN = 609,
     TS_PERIOD = 610,
     TS_SLASH = 611,
     TS_COLON = 612,
     TS_SEMICOLON = 613,
     TS_LESS_THAN_SIGN = 614,
     TS_EQUAL_SIGN = 615,
     TS_GREATER_THAN_SIGN = 616,
     TS_QUESTION_MARK = 617,
     TS_OUTER_JOIN_OPERATOR = 618,
     TX_HINTS = 619,
     SES_V_NUMERIC = 620,
     SES_V_INTEGER = 621,
     SES_V_HOSTVARIABLE = 622,
     SES_V_LITERAL = 623,
     SES_V_TYPED_LITERAL = 624,
     SES_V_DQUOTE_LITERAL = 625,
     SES_V_IDENTIFIER = 626,
     SES_V_ABSOLUTE = 627,
     SES_V_ALLOCATE = 628,
     SES_V_ASENSITIVE = 629,
     SES_V_AUTOCOMMIT = 630,
     SES_V_BATCH = 631,
     SES_V_BLOB_FILE = 632,
     SES_V_BREAK = 633,
     SES_V_CLOB_FILE = 634,
     SES_V_CUBE = 635,
     SES_V_DEALLOCATE = 636,
     SES_V_DESCRIPTOR = 637,
     SES_V_DO = 638,
     SES_V_FIRST = 639,
     SES_V_FOUND = 640,
     SES_V_FREE = 641,
     SES_V_HOLD = 642,
     SES_V_IMMEDIATE = 643,
     SES_V_INDICATOR = 644,
     SES_V_INSENSITIVE = 645,
     SES_V_LAST = 646,
     SES_V_NEXT = 647,
     SES_V_ONERR = 648,
     SES_V_ONLY = 649,
     APRE_V_OPTION = 650,
     SES_V_PREPARE = 651,
     SES_V_RELATIVE = 652,
     SES_V_RELEASE = 653,
     SES_V_ROLLUP = 654,
     SES_V_SCROLL = 655,
     SES_V_SENSITIVE = 656,
     SES_V_SQLERROR = 657,
     SES_V_THREADS = 658,
     SES_V_WHENEVER = 659,
     SES_V_CURRENT = 660,
     SES_V_GROUPING_SETS = 661
   };
#endif



#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
typedef union YYSTYPE
{

/* Line 214 of yacc.c  */
#line 24 "ulpCompy.y"

    char *strval;



/* Line 214 of yacc.c  */
#line 535 "ulpCompy.cpp"
} YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
#endif


/* Copy the second part of user declarations.  */

/* Line 264 of yacc.c  */
#line 53 "ulpCompy.y"


#undef YY_READ_BUF_SIZE
#undef YY_BUF_SIZE
#define YY_READ_BUF_SIZE (16384)
#define YY_BUF_SIZE (YY_READ_BUF_SIZE * 2) /* size of default input buffer */

//============== global variables for COMPparse ================//

/* externs of ulpMain.h */
extern ulpProgOption gUlpProgOption;
extern ulpCodeGen    gUlpCodeGen;
extern iduMemory    *gUlpMem;

// Macro table
extern ulpMacroTable  gUlpMacroT;
// Scope table
extern ulpScopeTable  gUlpScopeT;
// Struct tabletable
extern ulpStructTable gUlpStructT;

/* externs of COMPLexer */
extern idBool         gDontPrint2file;
extern SInt           gUlpCOMPMacroExpIndex;
/* BUG-31831 : An additional error message is needed to notify 
the unacceptability of using varchar type in #include file.
include file 파싱중인지를 알려줌 */
extern SInt           gUlpCOMPIncludeIndex;

/* extern of PPIF parser */
extern SChar         *gUlpPPIFbufptr;
extern SChar         *gUlpPPIFbuflim;

// lexer의 시작상태를 지정함.
SInt                 gUlpCOMPStartCond = CP_ST_NONE;
/* 이전 상태로 복귀하기 위한 변수 */
SInt                 gUlpCOMPPrevCond  = CP_ST_NONE;

/* BUG-35518 Shared pointer should be supported in APRE */
SInt                 gUlpSharedPtrPrevCond  = CP_ST_NONE;

// parsing중에 상태 정보 & C 변수에 대한 정보 저장.
ulpParseInfo         gUlpParseInfo;

// 현제 scope depth
SInt                 gUlpCurDepth = 0;

// 현재 처리중인 stmt type
ulpStmtType          gUlpStmttype    = S_UNKNOWN;
// sql query string 을 저장해야하는지 여부. 
idBool               gUlpIsPrintStmt = ID_TRUE;

// 현재 처리중인 host변수의 indicator 정보
ulpSymTElement      *gUlpIndNode = NULL;
SChar                gUlpIndName[MAX_HOSTVAR_NAME_SIZE * 2];
// 현재 처리중인 host변수의 file option 변수 정보
SChar                gUlpFileOptName[MAX_HOSTVAR_NAME_SIZE * 2];

/* macro if 조건문처리를 위한 변수들. */
ulpPPifstackMgr     *gUlpCOMPifstackMgr[MAX_HEADER_FILE_NUM];
SInt                 gUlpCOMPifstackInd = -1;

extern SChar        *gUlpCOMPErrCode;

//============================================================//


//=========== Function declarations for COMPparse ============//

// Macro if 구문 처리를 위한 parse 함수
extern SInt PPIFparse ( void *aBuf, SInt *aRes );
extern int  COMPlex   ( YYSTYPE *lvalp );
extern void COMPerror ( const SChar* aMsg );

extern void ulpFinalizeError(void);

//============================================================//

/* BUG-42357 */
extern int COMPlineno;



/* Line 264 of yacc.c  */
#line 631 "ulpCompy.cpp"

#ifdef short
# undef short
#endif

#ifdef YYTYPE_UINT8
typedef YYTYPE_UINT8 yytype_uint8;
#else
typedef unsigned char yytype_uint8;
#endif

#ifdef YYTYPE_INT8
typedef YYTYPE_INT8 yytype_int8;
#elif (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
typedef signed char yytype_int8;
#else
typedef short int yytype_int8;
#endif

#ifdef YYTYPE_UINT16
typedef YYTYPE_UINT16 yytype_uint16;
#else
typedef unsigned short int yytype_uint16;
#endif

#ifdef YYTYPE_INT16
typedef YYTYPE_INT16 yytype_int16;
#else
typedef short int yytype_int16;
#endif

#ifndef YYSIZE_T
# ifdef __SIZE_TYPE__
#  define YYSIZE_T __SIZE_TYPE__
# elif defined size_t
#  define YYSIZE_T size_t
# elif ! defined YYSIZE_T && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
#  include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  define YYSIZE_T size_t
# else
#  define YYSIZE_T unsigned int
# endif
#endif

#define YYSIZE_MAXIMUM ((YYSIZE_T) -1)

#ifndef YY_
# if YYENABLE_NLS
#  if ENABLE_NLS
#   include <libintl.h> /* INFRINGES ON USER NAME SPACE */
#   define YY_(msgid) dgettext ("bison-runtime", msgid)
#  endif
# endif
# ifndef YY_
#  define YY_(msgid) msgid
# endif
#endif

/* Suppress unused-variable warnings by "using" E.  */
#if ! defined lint || defined __GNUC__
# define YYUSE(e) ((void) (e))
#else
# define YYUSE(e) /* empty */
#endif

/* Identity function, used to suppress warnings about constant conditions.  */
#ifndef lint
# define YYID(n) (n)
#else
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static int
YYID (int yyi)
#else
static int
YYID (yyi)
    int yyi;
#endif
{
  return yyi;
}
#endif

#if ! defined yyoverflow || YYERROR_VERBOSE

/* The parser invokes alloca or malloc; define the necessary symbols.  */

# ifdef YYSTACK_USE_ALLOCA
#  if YYSTACK_USE_ALLOCA
#   ifdef __GNUC__
#    define YYSTACK_ALLOC __builtin_alloca
#   elif defined __BUILTIN_VA_ARG_INCR
#    include <alloca.h> /* INFRINGES ON USER NAME SPACE */
#   elif defined _AIX
#    define YYSTACK_ALLOC __alloca
#   elif defined _MSC_VER
#    include <malloc.h> /* INFRINGES ON USER NAME SPACE */
#    define alloca _alloca
#   else
#    define YYSTACK_ALLOC alloca
#    if ! defined _ALLOCA_H && ! defined _STDLIB_H && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
#     include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#     ifndef _STDLIB_H
#      define _STDLIB_H 1
#     endif
#    endif
#   endif
#  endif
# endif

# ifdef YYSTACK_ALLOC
   /* Pacify GCC's `empty if-body' warning.  */
#  define YYSTACK_FREE(Ptr) do { /* empty */; } while (YYID (0))
#  ifndef YYSTACK_ALLOC_MAXIMUM
    /* The OS might guarantee only one guard page at the bottom of the stack,
       and a page size can be as small as 4096 bytes.  So we cannot safely
       invoke alloca (N) if N exceeds 4096.  Use a slightly smaller number
       to allow for a few compiler-allocated temporary stack slots.  */
#   define YYSTACK_ALLOC_MAXIMUM 4032 /* reasonable circa 2006 */
#  endif
# else
#  define YYSTACK_ALLOC YYMALLOC
#  define YYSTACK_FREE YYFREE
#  ifndef YYSTACK_ALLOC_MAXIMUM
#   define YYSTACK_ALLOC_MAXIMUM YYSIZE_MAXIMUM
#  endif
#  if (defined __cplusplus && ! defined _STDLIB_H \
       && ! ((defined YYMALLOC || defined malloc) \
	     && (defined YYFREE || defined free)))
#   include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#   ifndef _STDLIB_H
#    define _STDLIB_H 1
#   endif
#  endif
#  ifndef YYMALLOC
#   define YYMALLOC malloc
#   if ! defined malloc && ! defined _STDLIB_H && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
void *malloc (YYSIZE_T); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
#  ifndef YYFREE
#   define YYFREE free
#   if ! defined free && ! defined _STDLIB_H && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
void free (void *); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
# endif
#endif /* ! defined yyoverflow || YYERROR_VERBOSE */


#if (! defined yyoverflow \
     && (! defined __cplusplus \
	 || (defined YYSTYPE_IS_TRIVIAL && YYSTYPE_IS_TRIVIAL)))

/* A type that is properly aligned for any stack member.  */
union yyalloc
{
  yytype_int16 yyss_alloc;
  YYSTYPE yyvs_alloc;
};

/* The size of the maximum gap between one aligned stack and the next.  */
# define YYSTACK_GAP_MAXIMUM (sizeof (union yyalloc) - 1)

/* The size of an array large to enough to hold all stacks, each with
   N elements.  */
# define YYSTACK_BYTES(N) \
     ((N) * (sizeof (yytype_int16) + sizeof (YYSTYPE)) \
      + YYSTACK_GAP_MAXIMUM)

/* Copy COUNT objects from FROM to TO.  The source and destination do
   not overlap.  */
# ifndef YYCOPY
#  if defined __GNUC__ && 1 < __GNUC__
#   define YYCOPY(To, From, Count) \
      __builtin_memcpy (To, From, (Count) * sizeof (*(From)))
#  else
#   define YYCOPY(To, From, Count)		\
      do					\
	{					\
	  YYSIZE_T yyi;				\
	  for (yyi = 0; yyi < (Count); yyi++)	\
	    (To)[yyi] = (From)[yyi];		\
	}					\
      while (YYID (0))
#  endif
# endif

/* Relocate STACK from its old location to the new one.  The
   local variables YYSIZE and YYSTACKSIZE give the old and new number of
   elements in the stack, and YYPTR gives the new location of the
   stack.  Advance YYPTR to a properly aligned location for the next
   stack.  */
# define YYSTACK_RELOCATE(Stack_alloc, Stack)				\
    do									\
      {									\
	YYSIZE_T yynewbytes;						\
	YYCOPY (&yyptr->Stack_alloc, Stack, yysize);			\
	Stack = &yyptr->Stack_alloc;					\
	yynewbytes = yystacksize * sizeof (*Stack) + YYSTACK_GAP_MAXIMUM; \
	yyptr += yynewbytes / sizeof (*yyptr);				\
      }									\
    while (YYID (0))

#endif

/* YYFINAL -- State number of the termination state.  */
#define YYFINAL  106
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   15169

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  431
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  715
/* YYNRULES -- Number of rules.  */
#define YYNRULES  1992
/* YYNRULES -- Number of states.  */
#define YYNSTATES  3815

/* YYTRANSLATE(YYLEX) -- Bison symbol number corresponding to YYLEX.  */
#define YYUNDEFTOK  2
#define YYMAXUTOK   661

#define YYTRANSLATE(YYX)						\
  ((unsigned int) (YYX) <= YYMAXUTOK ? yytranslate[YYX] : YYUNDEFTOK)

/* YYTRANSLATE[YYLEX] -- Bison symbol number corresponding to YYLEX.  */
static const yytype_uint16 yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,   418,     2,     2,     2,   420,   413,     2,
     407,   408,   414,   415,   412,   416,   411,   419,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,   426,   428,
     421,   427,   422,   425,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,   409,     2,   410,   423,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,   430,   424,   429,   417,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     1,     2,     3,     4,
       5,     6,     7,     8,     9,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    21,    22,    23,    24,
      25,    26,    27,    28,    29,    30,    31,    32,    33,    34,
      35,    36,    37,    38,    39,    40,    41,    42,    43,    44,
      45,    46,    47,    48,    49,    50,    51,    52,    53,    54,
      55,    56,    57,    58,    59,    60,    61,    62,    63,    64,
      65,    66,    67,    68,    69,    70,    71,    72,    73,    74,
      75,    76,    77,    78,    79,    80,    81,    82,    83,    84,
      85,    86,    87,    88,    89,    90,    91,    92,    93,    94,
      95,    96,    97,    98,    99,   100,   101,   102,   103,   104,
     105,   106,   107,   108,   109,   110,   111,   112,   113,   114,
     115,   116,   117,   118,   119,   120,   121,   122,   123,   124,
     125,   126,   127,   128,   129,   130,   131,   132,   133,   134,
     135,   136,   137,   138,   139,   140,   141,   142,   143,   144,
     145,   146,   147,   148,   149,   150,   151,   152,   153,   154,
     155,   156,   157,   158,   159,   160,   161,   162,   163,   164,
     165,   166,   167,   168,   169,   170,   171,   172,   173,   174,
     175,   176,   177,   178,   179,   180,   181,   182,   183,   184,
     185,   186,   187,   188,   189,   190,   191,   192,   193,   194,
     195,   196,   197,   198,   199,   200,   201,   202,   203,   204,
     205,   206,   207,   208,   209,   210,   211,   212,   213,   214,
     215,   216,   217,   218,   219,   220,   221,   222,   223,   224,
     225,   226,   227,   228,   229,   230,   231,   232,   233,   234,
     235,   236,   237,   238,   239,   240,   241,   242,   243,   244,
     245,   246,   247,   248,   249,   250,   251,   252,   253,   254,
     255,   256,   257,   258,   259,   260,   261,   262,   263,   264,
     265,   266,   267,   268,   269,   270,   271,   272,   273,   274,
     275,   276,   277,   278,   279,   280,   281,   282,   283,   284,
     285,   286,   287,   288,   289,   290,   291,   292,   293,   294,
     295,   296,   297,   298,   299,   300,   301,   302,   303,   304,
     305,   306,   307,   308,   309,   310,   311,   312,   313,   314,
     315,   316,   317,   318,   319,   320,   321,   322,   323,   324,
     325,   326,   327,   328,   329,   330,   331,   332,   333,   334,
     335,   336,   337,   338,   339,   340,   341,   342,   343,   344,
     345,   346,   347,   348,   349,   350,   351,   352,   353,   354,
     355,   356,   357,   358,   359,   360,   361,   362,   363,   364,
     365,   366,   367,   368,   369,   370,   371,   372,   373,   374,
     375,   376,   377,   378,   379,   380,   381,   382,   383,   384,
     385,   386,   387,   388,   389,   390,   391,   392,   393,   394,
     395,   396,   397,   398,   399,   400,   401,   402,   403,   404,
     405,   406
};

#if YYDEBUG
/* YYPRHS[YYN] -- Index of the first RHS symbol of rule number YYN in
   YYRHS.  */
static const yytype_uint16 yyprhs[] =
{
       0,     0,     3,     5,     8,    10,    12,    14,    16,    18,
      20,    22,    24,    26,    30,    32,    37,    41,    46,    50,
      54,    57,    60,    62,    66,    68,    71,    74,    77,    80,
      85,    87,    89,    91,    93,    95,    97,    99,   104,   106,
     110,   114,   118,   120,   124,   128,   130,   134,   138,   140,
     144,   148,   152,   156,   158,   162,   166,   168,   172,   174,
     178,   180,   184,   186,   190,   192,   196,   198,   204,   206,
     210,   212,   214,   216,   218,   220,   222,   224,   226,   228,
     230,   232,   234,   238,   240,   243,   247,   249,   252,   254,
     257,   259,   263,   265,   267,   271,   273,   275,   277,   279,
     281,   283,   285,   288,   291,   293,   295,   297,   299,   301,
     303,   305,   307,   309,   311,   313,   315,   317,   319,   321,
     323,   325,   327,   329,   331,   333,   335,   337,   339,   341,
     343,   345,   347,   349,   351,   353,   354,   356,   361,   367,
     372,   378,   381,   384,   386,   388,   390,   392,   395,   397,
     399,   403,   404,   406,   410,   412,   414,   417,   421,   426,
     432,   435,   437,   441,   443,   447,   449,   452,   454,   458,
     462,   467,   471,   476,   481,   483,   485,   487,   490,   493,
     497,   499,   502,   504,   508,   510,   514,   516,   520,   522,
     526,   529,   531,   533,   536,   538,   540,   543,   547,   550,
     554,   558,   563,   566,   570,   574,   579,   581,   585,   590,
     592,   596,   598,   600,   602,   604,   606,   608,   612,   616,
     621,   625,   630,   634,   637,   641,   643,   645,   647,   649,
     652,   654,   656,   659,   661,   664,   670,   678,   684,   690,
     698,   705,   713,   721,   730,   738,   747,   756,   766,   770,
     773,   776,   779,   783,   786,   790,   792,   795,   797,   799,
     802,   804,   806,   808,   810,   812,   814,   816,   818,   820,
     825,   830,   833,   836,   839,   841,   843,   846,   849,   851,
     853,   858,   863,   868,   874,   879,   883,   887,   891,   894,
     895,   898,   901,   903,   905,   907,   909,   911,   913,   915,
     917,   919,   923,   927,   935,   936,   938,   940,   942,   943,
     945,   946,   949,   952,   957,   958,   962,   966,   970,   975,
     976,   979,   981,   985,   989,   992,   997,  1002,  1008,  1015,
    1022,  1023,  1025,  1027,  1030,  1032,  1034,  1036,  1038,  1040,
    1043,  1046,  1048,  1051,  1054,  1058,  1061,  1064,  1067,  1073,
    1081,  1089,  1099,  1111,  1121,  1123,  1128,  1132,  1134,  1136,
    1138,  1140,  1142,  1144,  1146,  1148,  1150,  1155,  1160,  1161,
    1165,  1169,  1173,  1176,  1179,  1182,  1186,  1190,  1193,  1196,
    1199,  1202,  1205,  1208,  1214,  1219,  1224,  1232,  1236,  1244,
    1247,  1250,  1253,  1260,  1267,  1269,  1271,  1274,  1277,  1284,
    1291,  1298,  1302,  1305,  1312,  1319,  1323,  1328,  1333,  1338,
    1344,  1348,  1353,  1359,  1365,  1371,  1378,  1383,  1384,  1386,
    1393,  1398,  1400,  1402,  1404,  1406,  1408,  1410,  1412,  1414,
    1416,  1418,  1420,  1422,  1424,  1426,  1428,  1430,  1432,  1434,
    1436,  1438,  1440,  1442,  1444,  1446,  1448,  1450,  1452,  1454,
    1456,  1458,  1460,  1462,  1464,  1466,  1468,  1470,  1472,  1474,
    1476,  1478,  1480,  1482,  1484,  1486,  1488,  1490,  1492,  1494,
    1496,  1498,  1500,  1502,  1504,  1506,  1508,  1510,  1512,  1514,
    1516,  1518,  1520,  1522,  1524,  1526,  1528,  1530,  1532,  1534,
    1536,  1538,  1540,  1542,  1544,  1546,  1548,  1550,  1553,  1555,
    1558,  1560,  1563,  1565,  1567,  1569,  1573,  1577,  1579,  1581,
    1583,  1586,  1591,  1594,  1597,  1601,  1605,  1607,  1610,  1612,
    1615,  1617,  1619,  1621,  1623,  1625,  1627,  1629,  1631,  1633,
    1635,  1637,  1639,  1641,  1643,  1645,  1647,  1649,  1651,  1653,
    1655,  1657,  1659,  1661,  1663,  1665,  1667,  1669,  1671,  1673,
    1675,  1677,  1679,  1681,  1683,  1685,  1687,  1689,  1691,  1693,
    1695,  1697,  1699,  1701,  1703,  1705,  1707,  1709,  1711,  1713,
    1715,  1717,  1719,  1721,  1723,  1725,  1727,  1729,  1731,  1733,
    1735,  1737,  1739,  1741,  1748,  1755,  1762,  1769,  1777,  1785,
    1793,  1800,  1808,  1815,  1819,  1824,  1830,  1837,  1845,  1852,
    1859,  1865,  1873,  1874,  1876,  1878,  1880,  1883,  1886,  1889,
    1895,  1896,  1898,  1902,  1908,  1914,  1920,  1923,  1926,  1931,
    1936,  1940,  1942,  1946,  1950,  1956,  1963,  1971,  1976,  1979,
    1981,  1983,  1985,  1987,  1989,  1991,  1993,  1995,  1997,  1999,
    2002,  2005,  2007,  2010,  2014,  2018,  2022,  2026,  2028,  2030,
    2035,  2039,  2043,  2045,  2047,  2052,  2060,  2069,  2073,  2075,
    2078,  2082,  2086,  2090,  2093,  2097,  2101,  2105,  2109,  2112,
    2116,  2120,  2124,  2127,  2131,  2135,  2139,  2142,  2146,  2150,
    2154,  2158,  2161,  2164,  2167,  2171,  2175,  2179,  2183,  2187,
    2191,  2195,  2199,  2202,  2205,  2208,  2211,  2215,  2219,  2223,
    2226,  2229,  2232,  2236,  2240,  2244,  2249,  2254,  2259,  2262,
    2266,  2270,  2274,  2278,  2282,  2284,  2286,  2288,  2290,  2292,
    2294,  2296,  2298,  2300,  2303,  2305,  2307,  2309,  2313,  2315,
    2317,  2319,  2320,  2324,  2326,  2328,  2333,  2342,  2352,  2353,
    2356,  2357,  2359,  2365,  2372,  2376,  2381,  2392,  2399,  2406,
    2413,  2420,  2427,  2434,  2441,  2448,  2457,  2464,  2468,  2474,
    2481,  2487,  2494,  2500,  2501,  2504,  2507,  2509,  2511,  2514,
    2517,  2520,  2524,  2526,  2528,  2529,  2531,  2533,  2535,  2538,
    2540,  2545,  2546,  2549,  2553,  2554,  2557,  2560,  2564,  2565,
    2568,  2572,  2574,  2583,  2596,  2598,  2599,  2602,  2604,  2608,
    2612,  2614,  2616,  2619,  2620,  2623,  2627,  2629,  2633,  2639,
    2640,  2642,  2648,  2652,  2657,  2665,  2666,  2670,  2674,  2678,
    2683,  2687,  2694,  2702,  2708,  2716,  2726,  2730,  2732,  2744,
    2754,  2762,  2763,  2765,  2766,  2768,  2771,  2776,  2781,  2784,
    2786,  2790,  2795,  2799,  2804,  2807,  2811,  2813,  2815,  2817,
    2828,  2840,  2851,  2862,  2872,  2888,  2901,  2908,  2915,  2921,
    2927,  2934,  2943,  2949,  2954,  2963,  2973,  2984,  2993,  2994,
    2996,  3000,  3001,  3004,  3005,  3010,  3011,  3014,  3018,  3021,
    3025,  3034,  3040,  3045,  3061,  3077,  3081,  3085,  3089,  3097,
    3098,  3103,  3107,  3109,  3113,  3114,  3119,  3123,  3125,  3129,
    3130,  3135,  3139,  3141,  3145,  3151,  3155,  3157,  3159,  3165,
    3172,  3180,  3190,  3191,  3193,  3194,  3196,  3201,  3207,  3209,
    3211,  3215,  3219,  3221,  3224,  3226,  3228,  3231,  3234,  3240,
    3241,  3243,  3244,  3246,  3250,  3262,  3265,  3269,  3274,  3275,
    3279,  3280,  3285,  3290,  3291,  3293,  3295,  3300,  3304,  3306,
    3312,  3313,  3316,  3317,  3321,  3322,  3324,  3327,  3329,  3332,
    3335,  3337,  3346,  3357,  3365,  3374,  3375,  3377,  3379,  3382,
    3385,  3389,  3391,  3392,  3396,  3400,  3410,  3414,  3416,  3426,
    3430,  3438,  3444,  3448,  3450,  3452,  3455,  3457,  3460,  3465,
    3466,  3468,  3471,  3473,  3483,  3490,  3493,  3495,  3498,  3500,
    3504,  3506,  3508,  3510,  3516,  3523,  3531,  3538,  3545,  3550,
    3551,  3554,  3556,  3558,  3562,  3564,  3571,  3572,  3574,  3576,
    3577,  3581,  3582,  3584,  3587,  3589,  3592,  3596,  3601,  3607,
    3614,  3617,  3624,  3625,  3628,  3629,  3632,  3634,  3639,  3646,
    3654,  3662,  3663,  3665,  3669,  3670,  3674,  3678,  3683,  3686,
    3691,  3694,  3698,  3700,  3705,  3706,  3710,  3714,  3718,  3722,
    3725,  3727,  3730,  3731,  3733,  3744,  3753,  3762,  3773,  3774,
    3777,  3779,  3780,  3784,  3788,  3790,  3792,  3793,  3797,  3802,
    3806,  3813,  3821,  3829,  3838,  3843,  3849,  3851,  3853,  3860,
    3865,  3870,  3876,  3883,  3890,  3893,  3896,  3901,  3907,  3911,
    3918,  3925,  3928,  3932,  3940,  3948,  3957,  3964,  3974,  3980,
    3983,  3985,  3993,  3997,  3999,  4001,  4003,  4007,  4009,  4013,
    4022,  4032,  4034,  4036,  4043,  4046,  4048,  4052,  4054,  4055,
    4057,  4059,  4063,  4065,  4069,  4073,  4079,  4081,  4085,  4089,
    4091,  4093,  4097,  4109,  4112,  4114,  4119,  4121,  4124,  4127,
    4134,  4142,  4143,  4147,  4151,  4153,  4154,  4158,  4170,  4171,
    4175,  4179,  4181,  4183,  4185,  4187,  4189,  4193,  4198,  4200,
    4203,  4205,  4207,  4211,  4213,  4214,  4217,  4220,  4224,  4226,
    4233,  4237,  4239,  4250,  4261,  4263,  4265,  4269,  4274,  4278,
    4280,  4283,  4287,  4289,  4296,  4300,  4302,  4313,  4314,  4316,
    4317,  4319,  4320,  4324,  4325,  4327,  4329,  4331,  4333,  4334,
    4337,  4341,  4343,  4347,  4353,  4356,  4357,  4359,  4362,  4364,
    4367,  4370,  4374,  4376,  4383,  4388,  4394,  4402,  4408,  4413,
    4415,  4421,  4423,  4425,  4430,  4432,  4438,  4442,  4443,  4445,
    4447,  4454,  4458,  4460,  4466,  4472,  4477,  4480,  4485,  4489,
    4491,  4494,  4503,  4504,  4507,  4511,  4513,  4517,  4519,  4521,
    4526,  4530,  4532,  4536,  4538,  4542,  4544,  4548,  4550,  4552,
    4556,  4558,  4562,  4564,  4566,  4568,  4572,  4574,  4578,  4582,
    4584,  4588,  4592,  4594,  4597,  4600,  4602,  4604,  4606,  4608,
    4610,  4612,  4614,  4616,  4622,  4626,  4628,  4631,  4636,  4643,
    4644,  4646,  4649,  4652,  4655,  4656,  4658,  4663,  4668,  4672,
    4674,  4676,  4681,  4685,  4687,  4691,  4693,  4696,  4698,  4700,
    4704,  4706,  4708,  4710,  4713,  4714,  4717,  4718,  4721,  4722,
    4726,  4730,  4734,  4735,  4737,  4741,  4744,  4745,  4748,  4749,
    4753,  4758,  4759,  4761,  4764,  4768,  4772,  4774,  4775,  4777,
    4779,  4782,  4783,  4787,  4788,  4790,  4794,  4795,  4797,  4799,
    4801,  4803,  4805,  4807,  4811,  4813,  4817,  4818,  4820,  4822,
    4823,  4826,  4829,  4838,  4849,  4859,  4871,  4874,  4877,  4881,
    4883,  4884,  4888,  4889,  4892,  4894,  4898,  4900,  4904,  4906,
    4909,  4911,  4917,  4924,  4928,  4933,  4939,  4946,  4950,  4954,
    4958,  4962,  4966,  4970,  4974,  4978,  4982,  4986,  4990,  4994,
    4998,  5002,  5006,  5010,  5014,  5018,  5022,  5027,  5030,  5033,
    5037,  5041,  5043,  5045,  5048,  5051,  5053,  5056,  5058,  5061,
    5064,  5068,  5072,  5075,  5078,  5082,  5085,  5089,  5092,  5095,
    5097,  5101,  5105,  5109,  5113,  5116,  5119,  5123,  5127,  5130,
    5133,  5137,  5141,  5143,  5147,  5151,  5153,  5159,  5163,  5165,
    5167,  5169,  5171,  5175,  5177,  5181,  5185,  5187,  5191,  5195,
    5197,  5200,  5203,  5206,  5208,  5210,  5212,  5214,  5216,  5218,
    5222,  5224,  5226,  5228,  5230,  5232,  5234,  5243,  5250,  5255,
    5258,  5262,  5264,  5266,  5270,  5272,  5274,  5276,  5282,  5286,
    5288,  5289,  5291,  5297,  5302,  5305,  5307,  5311,  5314,  5315,
    5318,  5321,  5323,  5327,  5335,  5342,  5347,  5353,  5360,  5368,
    5376,  5383,  5392,  5400,  5405,  5409,  5416,  5419,  5420,  5428,
    5432,  5434,  5436,  5439,  5442,  5443,  5450,  5451,  5455,  5459,
    5461,  5463,  5464,  5468,  5469,  5475,  5478,  5481,  5483,  5486,
    5489,  5492,  5494,  5497,  5500,  5502,  5506,  5510,  5512,  5518,
    5522,  5524,  5526,  5527,  5536,  5538,  5540,  5549,  5556,  5562,
    5565,  5570,  5573,  5578,  5581,  5586,  5588,  5590,  5591,  5594,
    5598,  5602,  5604,  5609,  5610,  5612,  5614,  5617,  5618,  5620,
    5621,  5625,  5628,  5630,  5632,  5634,  5636,  5640,  5642,  5644,
    5648,  5653,  5657,  5659,  5664,  5671,  5678,  5687,  5689,  5693,
    5699,  5701,  5703,  5707,  5713,  5717,  5723,  5731,  5733,  5735,
    5739,  5745,  5750,  5757,  5765,  5773,  5782,  5789,  5795,  5798,
    5799,  5801,  5804,  5806,  5808,  5810,  5812,  5814,  5817,  5819,
    5827,  5831,  5837,  5838,  5840,  5849,  5858,  5860,  5862,  5866,
    5872,  5876,  5880,  5882,  5885,  5886,  5888,  5891,  5892,  5894,
    5897,  5899,  5903,  5907,  5911,  5913,  5915,  5919,  5922,  5924,
    5926,  5928,  5930,  5932,  5934,  5936,  5938,  5940,  5942,  5944,
    5946,  5948,  5950,  5952,  5954,  5956,  5962,  5966,  5969,  5972,
    5975,  5978,  5981,  5984,  5987,  5990,  5993,  5996,  5999,  6001,
    6005,  6011,  6017,  6023,  6031,  6039,  6040,  6043,  6045,  6050,
    6055,  6058,  6065,  6073,  6076,  6078,  6082,  6085,  6087,  6091,
    6093,  6094,  6097,  6099,  6101,  6103,  6105,  6110,  6116,  6120,
    6131,  6143,  6144,  6147,  6155,  6159,  6165,  6170,  6171,  6174,
    6178,  6181,  6185,  6191,  6195,  6198,  6201,  6205,  6208,  6213,
    6218,  6222,  6226,  6230,  6232,  6235,  6237,  6239,  6241,  6245,
    6251,  6255,  6260,  6265,  6270,  6274,  6281,  6283,  6285,  6288,
    6291,  6295,  6300,  6305,  6310,  6314,  6321,  6329,  6336,  6343,
    6351,  6358,  6363,  6368,  6369,  6372,  6376,  6377,  6381,  6382,
    6385,  6387,  6390,  6393,  6396,  6400,  6404,  6408,  6412,  6419,
    6427,  6436,  6444,  6449,  6458,  6466,  6471,  6474,  6477,  6484,
    6491,  6499,  6508,  6513,  6517,  6519,  6522,  6527,  6533,  6537,
    6544,  6545,  6547,  6550,  6553,  6558,  6563,  6569,  6576,  6581,
    6584,  6587,  6588,  6590,  6593,  6595,  6597,  6598,  6600,  6603,
    6607,  6609,  6612,  6614,  6617,  6619,  6621,  6623,  6625,  6629,
    6631,  6633,  6634,  6638,  6644,  6653,  6656,  6661,  6666,  6670,
    6673,  6677,  6681,  6686,  6688,  6690,  6693,  6695,  6697,  6698,
    6701,  6702,  6705,  6709,  6711,  6716,  6718,  6720,  6721,  6723,
    6725,  6726,  6728,  6730,  6731,  6736,  6740,  6741,  6746,  6749,
    6751,  6753,  6755,  6760,  6763,  6768,  6772,  6783,  6784,  6787,
    6790,  6794,  6799,  6804,  6810,  6813,  6816,  6822,  6825,  6828,
    6832,  6837,  6849,  6850,  6853,  6854,  6856,  6860,  6861,  6863,
    6865,  6867,  6868,  6870,  6873,  6880,  6887,  6894,  6900,  6906,
    6912,  6916,  6918,  6920,  6923,  6928,  6933,  6938,  6945,  6952,
    6954,  6958,  6962,  6966,  6971,  6974,  6979,  6984,  6988,  6990,
    6991,  6994,  6997,  7002,  7007,  7012,  7019,  7026,  7027,  7030,
    7032,  7034,  7037,  7042,  7047,  7052,  7059,  7066,  7069,  7072,
    7073,  7075,  7076,  7078,  7081,  7082,  7084,  7085,  7088,  7092,
    7093,  7095,  7098,  7101,  7104,  7105,  7109,  7113,  7115,  7119,
    7120,  7125,  7127
};

/* YYRHS -- A `-1'-separated list of the rules' RHS.  */
static const yytype_int16 yyrhs[] =
{
     432,     0,    -1,   433,    -1,   432,   433,    -1,   434,    -1,
     518,    -1,   508,    -1,     3,    -1,   504,    -1,   456,    -1,
     506,    -1,    38,    -1,   507,    -1,   407,   454,   408,    -1,
     435,    -1,   436,   409,   454,   410,    -1,   436,   407,   408,
      -1,   436,   407,   437,   408,    -1,   436,   411,   506,    -1,
     436,    54,   506,    -1,   436,    52,    -1,   436,    53,    -1,
     452,    -1,   437,   412,   452,    -1,   436,    -1,    52,   438,
      -1,    53,   438,    -1,   439,   440,    -1,    25,   438,    -1,
      25,   407,   488,   408,    -1,   413,    -1,   414,    -1,   415,
      -1,   416,    -1,   417,    -1,   418,    -1,   438,    -1,   407,
     488,   408,   440,    -1,   440,    -1,   441,   414,   440,    -1,
     441,   419,   440,    -1,   441,   420,   440,    -1,   441,    -1,
     442,   415,   441,    -1,   442,   416,   441,    -1,   442,    -1,
     443,    59,   442,    -1,   443,    58,   442,    -1,   443,    -1,
     444,   421,   443,    -1,   444,   422,   443,    -1,   444,    61,
     443,    -1,   444,    62,   443,    -1,   444,    -1,   445,    56,
     444,    -1,   445,    57,   444,    -1,   445,    -1,   446,   413,
     445,    -1,   446,    -1,   447,   423,   446,    -1,   447,    -1,
     448,   424,   447,    -1,   448,    -1,   449,    55,   448,    -1,
     449,    -1,   450,    60,   449,    -1,   450,    -1,   450,   425,
     450,   426,   451,    -1,   451,    -1,   438,   453,   452,    -1,
     427,    -1,    46,    -1,    47,    -1,    48,    -1,    44,    -1,
      45,    -1,    43,    -1,    42,    -1,    49,    -1,    50,    -1,
      51,    -1,   452,    -1,   454,   412,   452,    -1,   451,    -1,
     457,   428,    -1,   457,   458,   428,    -1,   461,    -1,   461,
     457,    -1,   462,    -1,   462,   457,    -1,   460,    -1,   458,
     459,   460,    -1,   412,    -1,   477,    -1,   477,   427,   491,
      -1,    29,    -1,    15,    -1,    26,    -1,     4,    -1,    21,
      -1,     7,    -1,     8,    -1,     7,    81,    -1,     8,    81,
      -1,    23,    -1,    19,    -1,    20,    -1,    24,    -1,    31,
      -1,    16,    -1,    13,    -1,     9,    -1,    33,    -1,    32,
      -1,   464,    -1,   474,    -1,    40,    -1,    63,    -1,    64,
      -1,    65,    -1,    66,    -1,    67,    -1,    68,    -1,    69,
      -1,    70,    -1,    71,    -1,    72,    -1,    73,    -1,    74,
      -1,    75,    -1,    76,    -1,    77,    -1,    78,    -1,    79,
      -1,    80,    -1,    -1,    82,    -1,   467,   465,   429,   463,
      -1,   467,   465,   468,   429,   463,    -1,   467,   466,   429,
     463,    -1,   467,   466,   468,   429,   463,    -1,   467,   506,
      -1,   506,   430,    -1,   430,    -1,    27,    -1,    30,    -1,
     469,    -1,   468,   469,    -1,   470,    -1,   508,    -1,   482,
     471,   428,    -1,    -1,   473,    -1,   471,   472,   473,    -1,
     412,    -1,   477,    -1,   426,   455,    -1,   477,   426,   455,
      -1,    14,   430,   475,   429,    -1,    14,   506,   430,   475,
     429,    -1,    14,   506,    -1,   476,    -1,   475,   412,   476,
      -1,   506,    -1,   506,   427,   455,    -1,   478,    -1,   481,
     478,    -1,   506,    -1,   407,   477,   408,    -1,   478,   409,
     410,    -1,   478,   479,   455,   410,    -1,   478,   407,   408,
      -1,   478,   480,   485,   408,    -1,   478,   407,   483,   408,
      -1,   409,    -1,   407,    -1,   414,    -1,   414,   482,    -1,
     414,   481,    -1,   414,   482,   481,    -1,   462,    -1,   482,
     462,    -1,   484,    -1,   484,   412,    35,    -1,   506,    -1,
     484,   412,   506,    -1,   486,    -1,   486,   412,    35,    -1,
     487,    -1,   486,   412,   487,    -1,   482,   477,    -1,   488,
      -1,   482,    -1,   482,   489,    -1,   481,    -1,   490,    -1,
     481,   490,    -1,   407,   489,   408,    -1,   409,   410,    -1,
     409,   455,   410,    -1,   490,   409,   410,    -1,   490,   409,
     455,   410,    -1,   407,   408,    -1,   407,   485,   408,    -1,
     490,   407,   408,    -1,   490,   407,   485,   408,    -1,   452,
      -1,   430,   492,   429,    -1,   430,   492,   412,   429,    -1,
     491,    -1,   492,   412,   491,    -1,   494,    -1,   495,    -1,
     500,    -1,   501,    -1,   502,    -1,   503,    -1,   506,   426,
     493,    -1,   506,   426,   518,    -1,     6,   455,   426,   493,
      -1,    11,   426,   493,    -1,     6,   455,   426,   518,    -1,
      11,   426,   518,    -1,   498,   429,    -1,   498,   497,   429,
      -1,   456,    -1,   493,    -1,   518,    -1,   496,    -1,   497,
     496,    -1,   430,    -1,   456,    -1,   499,   456,    -1,   428,
      -1,   454,   428,    -1,    37,   407,   454,   408,   493,    -1,
      37,   407,   454,   408,   493,    36,   493,    -1,    28,   407,
     454,   408,   493,    -1,    34,   407,   454,   408,   493,    -1,
      12,   493,    34,   407,   454,   408,   428,    -1,    17,   407,
     428,   428,   408,   493,    -1,    17,   407,   428,   428,   454,
     408,   493,    -1,    17,   407,   428,   454,   428,   408,   493,
      -1,    17,   407,   428,   454,   428,   454,   408,   493,    -1,
      17,   407,   454,   428,   428,   408,   493,    -1,    17,   407,
     454,   428,   428,   454,   408,   493,    -1,    17,   407,   454,
     428,   454,   428,   408,   493,    -1,    17,   407,   454,   428,
     454,   428,   454,   408,   493,    -1,    18,   506,   428,    -1,
      10,   428,    -1,     5,   428,    -1,    22,   428,    -1,    22,
     454,   428,    -1,   477,   505,    -1,   457,   477,   505,    -1,
     495,    -1,   499,   495,    -1,    39,    -1,    41,    -1,    41,
     507,    -1,   509,    -1,   510,    -1,   511,    -1,   514,    -1,
     515,    -1,   512,    -1,   513,    -1,   516,    -1,   517,    -1,
      83,    87,    90,    88,    -1,    83,    89,    90,    89,    -1,
      84,    98,    -1,    84,    86,    -1,    85,    98,    -1,    91,
      -1,    92,    -1,    95,    98,    -1,    96,    98,    -1,    93,
      -1,    94,    -1,    99,   519,   561,   358,    -1,    99,   519,
     520,   358,    -1,    99,   519,   540,   358,    -1,    99,   519,
     554,   273,   358,    -1,    99,   519,   556,   358,    -1,    99,
     557,   358,    -1,    99,   558,   358,    -1,    99,   559,   358,
      -1,    99,   560,    -1,    -1,   112,   371,    -1,   112,   367,
      -1,   521,    -1,   529,    -1,   530,    -1,   531,    -1,   535,
      -1,   536,    -1,   537,    -1,   538,    -1,   539,    -1,   522,
     807,   526,    -1,   522,   371,   526,    -1,   137,   371,   523,
     524,   134,   525,   165,    -1,    -1,   401,    -1,   390,    -1,
     374,    -1,    -1,   400,    -1,    -1,   266,   387,    -1,   266,
     228,    -1,   266,   387,   266,   228,    -1,    -1,   165,   221,
     394,    -1,   165,   256,   527,    -1,   165,   256,   371,    -1,
     165,   256,   295,   366,    -1,    -1,   202,   528,    -1,  1121,
      -1,   528,   353,  1121,    -1,   137,   371,   241,    -1,   206,
     371,    -1,   206,   371,   258,  1123,    -1,   206,   371,   258,
     547,    -1,   162,   532,   371,   180,  1127,    -1,   566,   162,
     532,   371,   180,  1127,    -1,   162,   532,   371,   258,   148,
    1124,    -1,    -1,   167,    -1,   533,    -1,   533,   167,    -1,
     384,    -1,   216,    -1,   392,    -1,   391,    -1,   405,    -1,
     397,   534,    -1,   372,   534,    -1,   366,    -1,   352,   366,
      -1,   354,   366,    -1,   122,   398,   371,    -1,   122,   371,
      -1,   375,   205,    -1,   375,   203,    -1,   128,   367,   285,
     116,   367,    -1,   128,   367,   285,   116,   367,   206,   367,
      -1,   128,   367,   285,   116,   367,   258,   367,    -1,   128,
     367,   285,   116,   367,   258,   367,   353,   367,    -1,   128,
     367,   285,   116,   367,   258,   367,   353,   367,   206,   367,
      -1,   128,   367,   285,   116,   367,   258,   367,   206,   367,
      -1,   144,    -1,   566,   386,   271,  1125,    -1,   386,   271,
    1125,    -1,   541,    -1,   543,    -1,   544,    -1,   546,    -1,
     548,    -1,   550,    -1,   551,    -1,   552,    -1,   553,    -1,
     373,   382,   367,   542,    -1,   373,   382,   371,   542,    -1,
      -1,   266,   371,   366,    -1,   381,   382,   367,    -1,   381,
     382,   371,    -1,   545,   367,    -1,   545,   562,    -1,   545,
     563,    -1,   396,   371,   167,    -1,   381,   396,   371,    -1,
     148,  1124,    -1,   549,   367,    -1,   549,   562,    -1,   549,
     563,    -1,   158,   388,    -1,   158,   371,    -1,   566,   158,
     371,   258,  1122,    -1,   158,   371,   258,  1122,    -1,   158,
     371,   258,   547,    -1,   147,   117,   261,   165,   371,   180,
    1124,    -1,   165,   371,  1124,    -1,   147,   234,   371,   165,
     371,   180,  1124,    -1,   956,   358,    -1,   955,   358,    -1,
     957,   358,    -1,   158,   555,  1041,   358,   153,   358,    -1,
     158,   555,  1042,   358,   153,   358,    -1,   115,    -1,   386,
      -1,   376,   205,    -1,   376,   203,    -1,   105,   236,   237,
     375,   360,   249,    -1,   105,   236,   237,   375,   360,   161,
      -1,   105,   236,   237,   371,   360,   368,    -1,   226,   160,
     367,    -1,   254,   160,    -1,   395,   347,   403,   360,   249,
     348,    -1,   395,   347,   403,   360,   161,   348,    -1,   404,
     402,   131,    -1,   404,   402,   170,   371,    -1,   404,   402,
     383,   378,    -1,   404,   402,   383,   131,    -1,   404,   402,
     383,   371,   347,    -1,   404,   402,   371,    -1,   404,   200,
     385,   131,    -1,   404,   200,   385,   170,   371,    -1,   404,
     200,   385,   383,   378,    -1,   404,   200,   385,   383,   131,
      -1,   404,   200,   385,   383,   371,   347,    -1,   404,   200,
     385,   371,    -1,    -1,   403,    -1,   115,   137,   214,   112,
     371,   358,    -1,   153,   137,   214,   358,    -1,   562,    -1,
     563,    -1,   567,    -1,   568,    -1,   572,    -1,   573,    -1,
    1046,    -1,   577,    -1,   580,    -1,   581,    -1,   582,    -1,
     586,    -1,   587,    -1,   601,    -1,   609,    -1,   616,    -1,
     698,    -1,   756,    -1,   757,    -1,   656,    -1,   674,    -1,
     638,    -1,   639,    -1,   645,    -1,   647,    -1,   637,    -1,
     685,    -1,   681,    -1,   741,    -1,   742,    -1,   653,    -1,
     677,    -1,   748,    -1,   643,    -1,   644,    -1,   642,    -1,
     641,    -1,   598,    -1,   749,    -1,   750,    -1,   751,    -1,
     754,    -1,   755,    -1,  1102,    -1,  1104,    -1,   892,    -1,
    1036,    -1,  1037,    -1,  1038,    -1,  1039,    -1,  1040,    -1,
    1048,    -1,  1053,    -1,  1059,    -1,  1060,    -1,  1061,    -1,
    1064,    -1,  1062,    -1,  1065,    -1,  1083,    -1,  1085,    -1,
    1086,    -1,   614,    -1,   615,    -1,   740,    -1,   758,    -1,
     759,    -1,  1105,    -1,  1108,    -1,  1110,    -1,   760,    -1,
    1111,    -1,  1119,    -1,  1120,    -1,   600,    -1,   599,    -1,
     564,   762,    -1,   762,    -1,   564,   763,    -1,   763,    -1,
     564,   770,    -1,   770,    -1,   784,    -1,   792,    -1,   807,
     885,   747,    -1,   772,   778,   886,    -1,   771,    -1,   566,
      -1,   565,    -1,   565,   566,    -1,   393,   367,   353,   367,
      -1,   165,   366,    -1,   165,   367,    -1,   371,   165,   366,
      -1,   371,   165,   367,    -1,   576,    -1,   576,   398,    -1,
     578,    -1,   578,   398,    -1,  1121,    -1,   103,    -1,   108,
      -1,   109,    -1,   113,    -1,   120,    -1,   127,    -1,   142,
      -1,   143,    -1,   152,    -1,   164,    -1,   387,    -1,   391,
      -1,   185,    -1,   190,    -1,   192,    -1,   195,    -1,   198,
      -1,   221,    -1,   223,    -1,   230,    -1,   315,    -1,   242,
      -1,   255,    -1,   258,    -1,   269,    -1,   154,    -1,   220,
      -1,   140,    -1,   163,    -1,   188,    -1,   270,    -1,   316,
      -1,   235,    -1,   250,    -1,   251,    -1,   271,    -1,   272,
      -1,   112,    -1,   186,    -1,   245,    -1,   196,    -1,   123,
      -1,   194,    -1,   213,    -1,   239,    -1,   222,    -1,   133,
      -1,   206,    -1,   122,    -1,   125,    -1,   294,    -1,   283,
      -1,   301,    -1,  1121,    -1,   139,    -1,   282,    -1,   216,
      -1,   384,    -1,   391,    -1,   392,    -1,   252,    -1,   292,
      -1,   105,   236,   237,   313,   360,   249,    -1,   105,   236,
     237,   313,   360,   161,    -1,   105,   236,   237,   313,   360,
     138,    -1,   105,   236,   237,   313,   360,   371,    -1,   105,
     236,   237,   371,   371,   360,   394,    -1,   105,   236,   237,
     371,   371,   360,   205,    -1,   105,   236,   237,   371,   371,
     360,   203,    -1,   105,   236,   237,   371,   360,   366,    -1,
     105,   236,   237,   371,   371,   360,   366,    -1,   105,   236,
     237,   371,   360,   371,    -1,   105,   371,   371,    -1,   105,
     371,   371,   371,    -1,   105,   371,   108,   371,   575,    -1,
     105,   371,   237,   371,   360,   366,    -1,   105,   371,   237,
     371,   360,   354,   366,    -1,   105,   371,   237,   371,   360,
     371,    -1,   105,   371,   237,   371,   360,   368,    -1,   105,
     371,   371,   283,   371,    -1,   105,   371,   371,   323,   371,
     371,   574,    -1,    -1,   191,    -1,   240,    -1,   371,    -1,
     126,   579,    -1,   233,  1121,    -1,   231,   579,    -1,   231,
     579,   247,   233,  1121,    -1,    -1,   267,    -1,   237,   371,
     583,    -1,   105,   236,   237,   371,   583,    -1,   126,   579,
     371,   136,   277,    -1,   231,   579,   371,   136,   277,    -1,
     221,   394,    -1,   221,   268,    -1,   182,   187,   221,   371,
      -1,   182,   187,   371,   221,    -1,   182,   187,   371,    -1,
    1121,    -1,  1121,   355,  1121,    -1,  1121,   355,  1121,    -1,
    1121,   355,  1121,   355,  1121,    -1,   132,   257,  1121,   285,
     116,  1121,    -1,   132,   257,  1121,   285,   116,  1121,   588,
      -1,   105,   257,  1121,   590,    -1,   588,   589,    -1,   589,
      -1,   594,    -1,   595,    -1,   596,    -1,   591,    -1,   593,
      -1,   594,    -1,   595,    -1,   592,    -1,   591,    -1,   152,
     371,    -1,   143,   371,    -1,   596,    -1,   592,   596,    -1,
     285,   116,  1121,    -1,   244,   317,  1121,    -1,   138,   317,
    1121,    -1,   283,  1121,   597,    -1,   205,    -1,   203,    -1,
     146,   257,  1121,   676,    -1,   146,   278,  1121,    -1,   132,
     278,  1121,    -1,   602,    -1,   603,    -1,   171,   604,   247,
     606,    -1,   171,   604,   205,   584,   247,   606,   608,    -1,
     171,   604,   205,   142,  1121,   247,   606,   608,    -1,   604,
     353,   605,    -1,   605,    -1,   105,   371,    -1,   132,   107,
     286,    -1,   105,   107,   286,    -1,   146,   107,   286,    -1,
     132,   218,    -1,   132,   107,   218,    -1,   105,   107,   218,
      -1,   146,   107,   218,    -1,   158,   107,   218,    -1,   132,
     248,    -1,   132,   107,   248,    -1,   105,   107,   248,    -1,
     146,   107,   248,    -1,   132,   242,    -1,   132,   219,   242,
      -1,   146,   107,   242,    -1,   146,   219,   242,    -1,   132,
     235,    -1,   132,   107,   235,    -1,   105,   107,   235,    -1,
     146,   107,   235,    -1,   234,   107,   235,    -1,   132,   236,
      -1,   105,   236,    -1,   132,   243,    -1,   132,   107,   243,
      -1,   105,   107,   243,    -1,   139,   107,   243,    -1,   146,
     107,   243,    -1,   178,   107,   243,    -1,   309,   107,   243,
      -1,   234,   107,   243,    -1,   256,   107,   243,    -1,   132,
     257,    -1,   105,   257,    -1,   146,   257,    -1,   132,   262,
      -1,   132,   107,   262,    -1,   146,   107,   262,    -1,   171,
     107,   217,    -1,   132,   317,    -1,   105,   317,    -1,   146,
     317,    -1,   132,   107,   142,    -1,   146,   107,   142,    -1,
     132,   298,   262,    -1,   132,   107,   298,   262,    -1,   105,
     107,   298,   262,    -1,   146,   107,   298,   262,    -1,   132,
     278,    -1,   146,   107,   278,    -1,   171,   107,   278,    -1,
     132,   107,   371,    -1,   105,   107,   371,    -1,   146,   107,
     371,    -1,   105,    -1,   139,    -1,   158,    -1,   286,    -1,
     178,    -1,   224,    -1,   234,    -1,   256,    -1,   104,    -1,
     104,   217,    -1,   221,    -1,   268,    -1,  1121,    -1,   606,
     353,   607,    -1,   607,    -1,  1121,    -1,   219,    -1,    -1,
     266,   171,   395,    -1,   610,    -1,   611,    -1,   229,   604,
     167,   606,    -1,   229,   604,   205,   584,   167,   606,   612,
     613,    -1,   229,   604,   205,   142,  1121,   167,   606,   612,
     613,    -1,    -1,   118,   130,    -1,    -1,   371,    -1,   132,
     242,   584,   165,   584,    -1,   132,   219,   242,   584,   165,
     584,    -1,   146,   242,   584,    -1,   146,   219,   242,   584,
      -1,   132,   622,   313,  1121,   627,   628,   617,   266,   623,
     629,    -1,   105,   313,  1121,   101,   243,   630,    -1,   105,
     313,  1121,   146,   243,   630,    -1,   105,   313,  1121,   101,
     371,   624,    -1,   105,   313,  1121,   146,   371,   624,    -1,
     105,   313,  1121,   237,   371,   624,    -1,   105,   313,  1121,
     237,   288,   621,    -1,   105,   313,  1121,   237,   371,   673,
      -1,   105,   313,  1121,   237,   269,   366,    -1,   105,   313,
    1121,   237,   312,   152,   266,   620,    -1,   105,   313,  1121,
     237,   312,   143,    -1,   146,   313,  1121,    -1,   105,   313,
    1121,   240,   636,    -1,   105,   313,  1121,   240,   266,   312,
      -1,   105,   313,  1121,   371,   633,    -1,   105,   313,  1121,
     371,   632,   633,    -1,   105,   313,  1121,   164,   631,    -1,
      -1,   371,   618,    -1,   618,   619,    -1,   619,    -1,   371,
      -1,   269,   366,    -1,   312,   620,    -1,   191,  1121,    -1,
     620,   353,   368,    -1,   368,    -1,   371,    -1,    -1,   621,
      -1,   624,    -1,   371,    -1,   624,   625,    -1,   625,    -1,
     368,   353,   366,   626,    -1,    -1,   258,   371,    -1,   258,
     371,   366,    -1,    -1,   165,   332,    -1,   165,   371,    -1,
     165,   371,   280,    -1,    -1,   110,   371,    -1,   629,   353,
     630,    -1,   630,    -1,   167,  1121,   355,  1121,   247,  1121,
     355,  1121,    -1,   167,  1121,   355,  1121,   212,  1121,   247,
    1121,   355,  1121,   212,  1121,    -1,   355,    -1,    -1,   295,
     366,    -1,   104,    -1,   104,   295,   366,    -1,   394,   269,
     366,    -1,   394,    -1,   371,    -1,   269,   366,    -1,    -1,
     243,   634,    -1,   634,   353,   635,    -1,   635,    -1,  1121,
     355,  1121,    -1,  1121,   355,  1121,   212,  1121,    -1,    -1,
     371,    -1,   112,   371,   347,   366,   348,    -1,   318,   243,
     584,    -1,   291,   584,   247,  1121,    -1,   303,   243,   584,
     247,   114,   146,   640,    -1,    -1,   291,   247,  1121,    -1,
     146,   235,   584,    -1,   146,   286,   584,    -1,   146,   243,
     584,   652,    -1,   302,   243,   584,    -1,   324,   243,  1121,
     347,   646,   348,    -1,   212,  1121,   247,   243,  1121,   353,
     646,    -1,   212,  1121,   247,   243,  1121,    -1,   325,   243,
    1121,   648,   700,   651,   708,    -1,   212,   116,  1121,   347,
     734,   348,   347,   649,   348,    -1,   649,   353,   650,    -1,
     650,    -1,   243,  1121,   247,   212,  1121,   259,   186,   245,
     347,   704,   348,    -1,   243,  1121,   247,   212,  1121,   259,
     347,   704,   348,    -1,   243,  1121,   247,   212,  1121,   259,
     138,    -1,    -1,  1140,    -1,    -1,   118,    -1,   118,   130,
      -1,   105,   235,   584,   654,    -1,   105,   235,   584,   684,
      -1,   654,   655,    -1,   655,    -1,   240,   266,   366,    -1,
     240,   266,   354,   366,    -1,   371,   116,   366,    -1,   371,
     116,   354,   366,    -1,   371,   366,    -1,   371,   354,   366,
      -1,   371,    -1,   135,    -1,   199,    -1,   105,   243,   584,
     101,   675,   347,   717,   348,   708,  1141,    -1,   105,   243,
     584,   105,   675,   347,   569,   237,   138,   921,   348,    -1,
     105,   243,   584,   105,   675,   347,   569,   146,   138,   348,
      -1,   105,   243,   584,   105,   675,   347,   569,   200,   201,
     348,    -1,   105,   243,   584,   105,   675,   347,   569,   201,
     348,    -1,   105,   243,   584,   105,   675,   271,   347,   734,
     348,   272,   110,   347,   711,   348,  1141,    -1,   105,   243,
     584,   105,   675,   271,   272,   110,   347,   711,   348,  1141,
      -1,   105,   243,   584,   146,   675,   569,    -1,   105,   243,
     584,   291,   247,  1121,    -1,   105,   243,   584,   310,   366,
      -1,   105,   243,   584,   371,   661,    -1,   105,   243,   584,
     104,   286,   673,    -1,   105,   243,   584,   291,   124,   569,
     247,   569,    -1,   105,   243,   584,   283,  1140,    -1,   105,
     243,   584,   662,    -1,   105,   243,   584,   105,   317,  1121,
     666,   663,    -1,   105,   243,   584,   292,   584,   661,   657,
     659,   660,    -1,   105,   243,   584,   146,   191,   253,   347,
     733,   348,   676,    -1,   105,   243,   584,   291,   129,  1121,
     247,  1121,    -1,    -1,   658,    -1,   258,   371,  1121,    -1,
      -1,   291,   371,    -1,    -1,   371,   166,   184,   371,    -1,
      -1,   212,  1121,    -1,   101,   672,   669,    -1,   123,   212,
      -1,   146,   212,  1121,    -1,   194,   213,  1121,   353,  1121,
     180,   672,   669,    -1,   291,   212,  1121,   247,  1121,    -1,
     283,   212,  1121,  1140,    -1,   239,   212,  1121,   112,   347,
     704,   348,   180,   347,   672,   669,   353,   672,   669,   348,
      -1,   239,   212,  1121,   259,   347,   704,   348,   180,   347,
     672,   669,   353,   672,   669,   348,    -1,   318,   212,  1121,
      -1,   152,   232,   196,    -1,   143,   232,   196,    -1,   105,
     212,  1121,   317,  1121,   666,   663,    -1,    -1,   271,   347,
     664,   348,    -1,   664,   353,   665,    -1,   665,    -1,   569,
     317,  1121,    -1,    -1,   286,   347,   667,   348,    -1,   667,
     353,   668,    -1,   668,    -1,  1121,   317,  1121,    -1,    -1,
     286,   347,   670,   348,    -1,   670,   353,   671,    -1,   671,
      -1,  1121,   212,  1121,    -1,  1121,   212,  1121,   317,  1121,
      -1,   212,  1121,  1138,    -1,   152,    -1,   143,    -1,   105,
     243,   584,   101,   715,    -1,   105,   243,   584,   146,   129,
    1121,    -1,   105,   243,   584,   146,   215,   184,   676,    -1,
     105,   243,   584,   146,   253,   347,   733,   348,   676,    -1,
      -1,   124,    -1,    -1,   118,    -1,   105,   286,   584,   678,
      -1,   105,   286,   584,   237,   679,    -1,   371,    -1,   222,
      -1,   222,   212,  1121,    -1,   291,   247,   584,    -1,   679,
      -1,   371,   680,    -1,   205,    -1,   203,    -1,   360,   205,
      -1,   360,   203,    -1,   132,   235,   584,   682,   683,    -1,
      -1,   654,    -1,    -1,   684,    -1,   673,   371,   243,    -1,
     686,   584,   205,   584,   347,   732,   348,   689,   687,   688,
     695,    -1,   132,   286,    -1,   132,   253,   286,    -1,   132,
     191,   253,   286,    -1,    -1,   371,   181,   371,    -1,    -1,
     237,   371,   360,   205,    -1,   237,   371,   360,   203,    -1,
      -1,   690,    -1,   191,    -1,   191,   347,   691,   348,    -1,
     691,   353,   692,    -1,   692,    -1,   212,  1121,   205,  1121,
     693,    -1,    -1,   317,  1121,    -1,    -1,   258,   286,   695,
      -1,    -1,   696,    -1,   696,   697,    -1,   697,    -1,   317,
    1121,    -1,   269,   366,    -1,   270,    -1,   132,   243,   584,
     347,   713,   348,   699,   708,    -1,   132,   243,   584,   347,
     713,   348,   699,   708,   110,   796,    -1,   132,   243,   584,
     699,   708,   110,   796,    -1,   132,   243,   584,   167,   243,
     371,   584,   658,    -1,    -1,   738,    -1,   706,    -1,   738,
     706,    -1,   701,   700,    -1,   701,   700,   706,    -1,  1140,
      -1,    -1,   152,   232,   196,    -1,   143,   232,   196,    -1,
     212,   116,  1121,   347,   734,   348,   347,   702,   348,    -1,
     702,   353,   703,    -1,   703,    -1,   212,  1121,   259,   186,
     245,   347,   704,   348,  1138,    -1,   212,  1121,  1138,    -1,
     212,  1121,   259,   347,   704,   348,  1138,    -1,   212,  1121,
     259,   138,  1138,    -1,   704,   353,   705,    -1,   705,    -1,
     921,    -1,   706,   707,    -1,   707,    -1,   317,  1121,    -1,
     178,   371,   190,   366,    -1,    -1,   709,    -1,   709,   710,
      -1,   710,    -1,   271,   347,   734,   348,   272,   110,   347,
     711,   348,    -1,   271,   272,   110,   347,   711,   348,    -1,
     711,   712,    -1,   712,    -1,   317,  1121,    -1,   371,    -1,
     713,   353,   714,    -1,   714,    -1,   715,    -1,   719,    -1,
     716,   253,   731,   688,   694,    -1,   716,   215,   184,   731,
     688,   694,    -1,   716,   166,   184,   347,   734,   348,   735,
      -1,   716,   191,   253,   731,   688,   694,    -1,   716,   191,
     253,   890,   688,   694,    -1,   716,   121,   896,   348,    -1,
      -1,   129,  1121,    -1,   718,    -1,   715,    -1,   718,   353,
     719,    -1,   719,    -1,   569,   726,   720,   721,   725,   722,
      -1,    -1,   308,    -1,   260,    -1,    -1,   175,   232,   366,
      -1,    -1,   723,    -1,   723,   724,    -1,   724,    -1,   716,
     201,    -1,   716,   200,   201,    -1,   716,   121,   896,   348,
      -1,   716,   253,   890,   688,   694,    -1,   716,   215,   184,
     890,   688,   694,    -1,   716,   735,    -1,   716,   191,   253,
     890,   688,   694,    -1,    -1,   138,   921,    -1,    -1,   727,
     728,    -1,  1121,    -1,  1121,   347,   366,   348,    -1,  1121,
     347,   366,   353,   366,   348,    -1,  1121,   347,   366,   353,
     352,   366,   348,    -1,  1121,   347,   366,   353,   354,   366,
     348,    -1,    -1,   729,    -1,   371,   258,   368,    -1,    -1,
     347,   734,   348,    -1,   347,   733,   348,    -1,   732,   353,
     896,   890,    -1,   896,   890,    -1,   733,   353,   569,   890,
      -1,   569,   890,    -1,   734,   353,   569,    -1,   569,    -1,
     224,   584,   730,   736,    -1,    -1,   205,   178,   737,    -1,
     205,   256,   737,    -1,   205,   139,   737,    -1,   205,   139,
     118,    -1,   371,   371,    -1,   227,    -1,   310,   366,    -1,
      -1,   738,    -1,   132,   220,   584,   347,   366,   720,   721,
     348,   739,  1137,    -1,   132,   220,   584,   347,   718,   348,
     739,  1137,    -1,   132,   743,   262,   584,   744,   110,   796,
     747,    -1,   132,   207,   292,   743,   262,   584,   744,   110,
     796,   747,    -1,    -1,   371,   371,    -1,   371,    -1,    -1,
     347,   745,   348,    -1,   745,   353,   746,    -1,   746,    -1,
     569,    -1,    -1,   266,   221,   394,    -1,   105,   262,   584,
     127,    -1,   146,   262,   584,    -1,   132,   136,   277,  1121,
     258,  1121,    -1,   132,   752,   136,   277,  1121,   258,  1121,
      -1,   132,   136,   277,  1121,   753,   258,  1121,    -1,   132,
     752,   136,   277,  1121,   753,   258,  1121,    -1,   146,   136,
     277,  1121,    -1,   146,   752,   136,   277,  1121,    -1,   219,
      -1,   371,    -1,   128,   247,  1121,   285,   116,  1121,    -1,
     105,   136,   321,   240,    -1,   105,   136,   321,   371,    -1,
     105,   136,   321,   371,   371,    -1,   105,   236,   122,   136,
     277,   104,    -1,   105,   236,   122,   136,   277,  1121,    -1,
     138,   921,    -1,   228,   896,    -1,   105,   220,   584,   371,
      -1,   105,   220,   584,   371,   371,    -1,   146,   220,   584,
      -1,   125,   205,   243,   584,   181,   368,    -1,   125,   205,
     124,   585,   181,   368,    -1,   584,  1144,    -1,   347,   796,
     348,    -1,   139,   816,   817,   761,   824,   895,   880,    -1,
     178,   816,   180,   761,   824,   138,   259,    -1,   178,   816,
     180,   761,   824,   791,   259,   768,    -1,   178,   816,   180,
     761,   824,   796,    -1,   178,   816,   180,   761,   824,   347,
     790,   348,   796,    -1,   178,   816,   104,   764,   796,    -1,
     764,   765,    -1,   765,    -1,   180,   761,   791,   259,   347,
     766,   348,    -1,   766,   353,   767,    -1,   767,    -1,   921,
      -1,   138,    -1,   768,   353,   769,    -1,   769,    -1,   347,
     766,   348,    -1,   256,   816,   761,   824,   237,   779,   895,
     880,    -1,   154,   816,   180,   584,   730,   259,   347,   766,
     348,    -1,   773,    -1,   774,    -1,   140,   816,   820,  1126,
     775,   895,    -1,   167,   776,    -1,   777,    -1,  1121,   355,
    1121,    -1,  1121,    -1,    -1,   163,    -1,   188,    -1,   779,
     353,   780,    -1,   780,    -1,   781,   360,   921,    -1,   781,
     360,   138,    -1,   347,   782,   348,   360,   921,    -1,   569,
      -1,  1121,   355,   569,    -1,   782,   353,   783,    -1,   783,
      -1,   569,    -1,  1121,   355,  1121,    -1,   194,   816,   180,
     584,  1144,   824,   258,   827,   205,   896,   785,    -1,   785,
     786,    -1,   786,    -1,   263,   787,   246,   788,    -1,   371,
      -1,   200,   371,    -1,   371,   371,    -1,   256,   237,   779,
     895,   880,   789,    -1,   178,   791,   259,   347,   766,   348,
     895,    -1,    -1,   139,   895,   880,    -1,   790,   353,   781,
      -1,   781,    -1,    -1,   347,   790,   348,    -1,   195,   816,
     180,   584,  1144,   730,   167,   584,   793,   895,   880,    -1,
      -1,   347,   794,   348,    -1,   794,   353,   795,    -1,   795,
      -1,   921,    -1,   138,    -1,   797,    -1,   798,    -1,   800,
     879,   883,    -1,   802,   800,   879,   883,    -1,   252,    -1,
     252,   104,    -1,   179,    -1,   287,    -1,   800,   799,   805,
      -1,   805,    -1,    -1,   266,   803,    -1,   266,   803,    -1,
     803,   353,   804,    -1,   804,    -1,  1121,   744,   110,   347,
     797,   348,    -1,   347,   800,   348,    -1,   806,    -1,   234,
     816,   819,   820,   821,   825,   895,   874,   818,   873,    -1,
     234,   816,   819,   820,  1126,   825,   895,   874,   818,   873,
      -1,   808,    -1,   809,    -1,   810,   879,   883,    -1,   811,
     810,   879,   883,    -1,   810,   799,   814,    -1,   814,    -1,
     266,   812,    -1,   812,   353,   813,    -1,   813,    -1,  1121,
     744,   110,   347,   808,   348,    -1,   347,   810,   348,    -1,
     815,    -1,   234,   816,   819,   820,  1128,   825,   895,   874,
     818,   873,    -1,    -1,   364,    -1,    -1,   167,    -1,    -1,
     172,   116,   870,    -1,    -1,   104,    -1,   145,    -1,   351,
      -1,   822,    -1,    -1,   180,   973,    -1,   822,   353,   823,
      -1,   823,    -1,  1121,   355,   351,    -1,  1121,   355,  1121,
     355,   351,    -1,   921,   824,    -1,    -1,  1121,    -1,   110,
     569,    -1,   368,    -1,   110,   368,    -1,   167,   826,    -1,
     826,   353,   827,    -1,   827,    -1,  1121,   355,  1121,  1144,
     829,   824,    -1,  1121,  1144,   829,   824,    -1,   347,   796,
     348,   829,   824,    -1,   322,   347,  1121,   353,   368,   348,
     824,    -1,   323,   347,   796,   348,   824,    -1,  1121,   342,
    1121,   824,    -1,   860,    -1,   243,   347,   828,   348,   824,
      -1,   857,    -1,   936,    -1,   304,   347,   949,   348,    -1,
     371,    -1,  1121,   355,  1121,   355,   569,    -1,  1121,   355,
     569,    -1,    -1,   830,    -1,   837,    -1,   296,   347,   831,
     833,   834,   348,    -1,   831,   353,   832,    -1,   832,    -1,
     371,   347,   921,   348,   824,    -1,   371,   347,   351,   348,
     824,    -1,   165,   347,   569,   348,    -1,   165,   569,    -1,
     176,   347,   835,   348,    -1,   835,   353,   836,    -1,   836,
      -1,   855,   824,    -1,   297,   838,   347,   839,   165,   839,
     842,   348,    -1,    -1,   371,   301,    -1,   347,   840,   348,
      -1,   841,    -1,   840,   353,   841,    -1,   841,    -1,   569,
      -1,   176,   347,   843,   348,    -1,   843,   353,   844,    -1,
     844,    -1,   845,   110,   848,    -1,   845,    -1,   347,   846,
     348,    -1,   847,    -1,   846,   353,   847,    -1,   847,    -1,
     569,    -1,   347,   849,   348,    -1,   850,    -1,   849,   353,
     850,    -1,   850,    -1,   851,    -1,   852,    -1,   852,   343,
     853,    -1,   853,    -1,   853,   352,   854,    -1,   853,   354,
     854,    -1,   854,    -1,   854,   351,   855,    -1,   854,   356,
     855,    -1,   855,    -1,   352,   856,    -1,   354,   856,    -1,
     856,    -1,   201,    -1,   366,    -1,   365,    -1,   368,    -1,
     369,    -1,   338,    -1,   339,    -1,  1121,   347,   858,   348,
     824,    -1,   858,   353,   859,    -1,   859,    -1,  1121,  1144,
      -1,  1121,   355,  1121,  1144,    -1,   827,   861,   183,   827,
     205,   896,    -1,    -1,   177,    -1,   185,   862,    -1,   230,
     862,    -1,   168,   862,    -1,    -1,   210,    -1,   399,   347,
     864,   348,    -1,   380,   347,   864,   348,    -1,   864,   353,
     865,    -1,   865,    -1,   921,    -1,   406,   347,   867,   348,
      -1,   867,   353,   869,    -1,   869,    -1,   867,   353,   868,
      -1,   868,    -1,   347,   348,    -1,   863,    -1,   921,    -1,
     870,   353,   871,    -1,   871,    -1,   863,    -1,   866,    -1,
     921,   872,    -1,    -1,   266,   399,    -1,    -1,   173,   896,
      -1,    -1,   875,   877,   878,    -1,   877,   878,   876,    -1,
     240,   266,   896,    -1,    -1,   875,    -1,   128,   116,   896,
      -1,   299,   896,    -1,    -1,   371,   193,    -1,    -1,   208,
     116,   888,    -1,   208,   371,   116,   888,    -1,    -1,   881,
      -1,   190,   882,    -1,   882,   353,   896,    -1,   882,   371,
     896,    -1,   896,    -1,    -1,   881,    -1,   884,    -1,   193,
     896,    -1,    -1,   165,   256,   886,    -1,    -1,   371,    -1,
     295,   366,   887,    -1,    -1,   326,    -1,   327,    -1,   328,
      -1,   329,    -1,   330,    -1,   331,    -1,   888,   353,   889,
      -1,   889,    -1,   921,   890,   891,    -1,    -1,   111,    -1,
     141,    -1,    -1,   301,   384,    -1,   301,   391,    -1,   309,
     243,  1121,   175,   893,   288,   886,   894,    -1,   309,   243,
    1121,   355,  1121,   175,   893,   288,   886,   894,    -1,   309,
     243,  1121,  1144,   175,   893,   288,   886,   894,    -1,   309,
     243,  1121,   355,  1121,  1144,   175,   893,   288,   886,   894,
      -1,   232,   371,    -1,   371,   256,    -1,   371,   232,   371,
      -1,   371,    -1,    -1,   255,   392,   371,    -1,    -1,   264,
     896,    -1,   897,    -1,   897,   207,   898,    -1,   898,    -1,
     898,   106,   899,    -1,   899,    -1,   200,   900,    -1,   900,
      -1,   921,   281,   921,   106,   921,    -1,   921,   200,   281,
     921,   106,   921,    -1,   921,   189,   921,    -1,   921,   200,
     189,   921,    -1,   921,   189,   921,   155,   921,    -1,   921,
     200,   189,   921,   155,   921,    -1,   921,   901,   921,    -1,
     921,   902,   921,    -1,   921,   903,   921,    -1,   921,   904,
     921,    -1,   921,   905,   921,    -1,   921,   906,   921,    -1,
     921,   907,   920,    -1,   921,   908,   920,    -1,   921,   909,
     920,    -1,   921,   910,   920,    -1,   921,   911,   920,    -1,
     921,   912,   920,    -1,   921,   913,   920,    -1,   921,   914,
     920,    -1,   921,   915,   920,    -1,   921,   916,   920,    -1,
     921,   917,   920,    -1,   921,   918,   920,    -1,   921,   181,
     201,    -1,   921,   181,   200,   201,    -1,   282,   950,    -1,
     253,   950,    -1,   919,   346,   371,    -1,   919,   346,   385,
      -1,   921,    -1,   360,    -1,   359,   361,    -1,   345,   360,
      -1,   359,    -1,   359,   360,    -1,   361,    -1,   361,   360,
      -1,   360,   104,    -1,   359,   361,   104,    -1,   345,   360,
     104,    -1,   200,   176,    -1,   359,   104,    -1,   359,   360,
     104,    -1,   361,   104,    -1,   361,   360,   104,    -1,   360,
     107,    -1,   360,   238,    -1,   176,    -1,   359,   361,   107,
      -1,   359,   361,   238,    -1,   345,   360,   107,    -1,   345,
     360,   238,    -1,   359,   107,    -1,   359,   238,    -1,   359,
     360,   107,    -1,   359,   360,   238,    -1,   361,   107,    -1,
     361,   238,    -1,   361,   360,   107,    -1,   361,   360,   238,
      -1,  1121,    -1,  1121,   355,  1121,    -1,   347,   949,   348,
      -1,   950,    -1,  1121,   355,  1121,   355,   569,    -1,  1121,
     355,   569,    -1,   569,    -1,  1124,    -1,   856,    -1,   922,
      -1,   922,   343,   923,    -1,   923,    -1,   923,   352,   924,
      -1,   923,   354,   924,    -1,   924,    -1,   924,   351,   925,
      -1,   924,   356,   925,    -1,   925,    -1,   352,   926,    -1,
     354,   926,    -1,   216,   926,    -1,   926,    -1,   201,    -1,
     249,    -1,   161,    -1,   319,    -1,   320,    -1,   919,   346,
     315,    -1,   366,    -1,   365,    -1,   368,    -1,   369,    -1,
     362,    -1,  1124,    -1,  1121,   355,  1121,   355,  1121,   355,
     569,   928,    -1,  1121,   355,  1121,   355,   569,   928,    -1,
    1121,   355,   569,   928,    -1,   569,   928,    -1,   335,   355,
     569,    -1,   187,    -1,   936,    -1,   347,   949,   348,    -1,
     950,    -1,   974,    -1,   929,    -1,  1121,   355,  1121,   355,
     569,    -1,  1121,   355,   569,    -1,   569,    -1,    -1,   363,
      -1,   119,   921,   930,   933,   153,    -1,   119,   934,   933,
     153,    -1,   930,   931,    -1,   931,    -1,   263,   921,   932,
      -1,   246,   921,    -1,    -1,   150,   921,    -1,   934,   935,
      -1,   935,    -1,   263,   896,   932,    -1,  1121,   347,   949,
     348,   937,   953,   940,    -1,  1121,   355,   570,   347,   949,
     348,    -1,  1121,   347,   348,   940,    -1,  1121,   355,   570,
     347,   348,    -1,  1121,   347,   351,   348,   953,   940,    -1,
    1121,   347,   104,   351,   348,   953,   940,    -1,  1121,   347,
     104,   949,   348,   953,   940,    -1,  1121,   347,   145,   949,
     348,   940,    -1,  1121,   355,  1121,   355,   570,   347,   949,
     348,    -1,  1121,   355,  1121,   355,   570,   347,   348,    -1,
     571,   347,   949,   348,    -1,   571,   347,   348,    -1,   120,
     347,   896,   110,   727,   348,    -1,   300,   927,    -1,    -1,
     279,   172,   347,   208,   116,   938,   348,    -1,   938,   353,
     939,    -1,   939,    -1,   921,    -1,   921,   111,    -1,   921,
     141,    -1,    -1,   211,   347,   941,   944,   945,   348,    -1,
      -1,   212,   116,   942,    -1,   942,   353,   943,    -1,   943,
      -1,   921,    -1,    -1,   208,   116,   888,    -1,    -1,   371,
     281,   946,   106,   947,    -1,   371,   946,    -1,   371,   274,
      -1,   276,    -1,   948,   274,    -1,   948,   275,    -1,   371,
     275,    -1,   276,    -1,   948,   274,    -1,   948,   275,    -1,
     366,    -1,   371,   366,   371,    -1,   949,   353,   896,    -1,
     896,    -1,   347,   801,   951,   883,   348,    -1,   951,   799,
     952,    -1,   952,    -1,   806,    -1,    -1,   305,   347,   371,
     954,   208,   116,   888,   348,    -1,   384,    -1,   391,    -1,
     958,   584,   962,   228,   977,   961,   980,   966,    -1,   959,
     584,   962,   961,   980,   966,    -1,   960,   584,   961,   981,
     966,    -1,   132,   169,    -1,   132,   207,   292,   169,    -1,
     132,   218,    -1,   132,   207,   292,   218,    -1,   132,   251,
      -1,   132,   207,   292,   251,    -1,   110,    -1,   181,    -1,
      -1,   347,   348,    -1,   347,   963,   348,    -1,   963,   353,
     964,    -1,   964,    -1,  1121,   965,   977,   967,    -1,    -1,
     175,    -1,   209,    -1,   175,   209,    -1,    -1,  1121,    -1,
      -1,   357,   360,   970,    -1,   138,   970,    -1,   921,    -1,
     896,    -1,   896,    -1,  1121,    -1,  1121,   355,  1121,    -1,
     936,    -1,  1121,    -1,  1121,   347,   348,    -1,  1121,   347,
     949,   348,    -1,   973,   353,   975,    -1,   975,    -1,  1121,
     349,   970,   350,    -1,  1121,   355,  1121,   349,   970,   350,
      -1,  1121,   349,   970,   350,   355,   569,    -1,  1121,   355,
    1121,   349,   970,   350,   355,   569,    -1,  1121,    -1,  1121,
     355,   569,    -1,  1121,   355,  1121,   355,   569,    -1,   974,
      -1,  1121,    -1,   569,   346,   293,    -1,  1121,   355,   569,
     346,   293,    -1,   569,   346,   250,    -1,  1121,   355,   569,
     346,   250,    -1,  1121,   355,  1121,   355,   569,   346,   250,
      -1,   978,    -1,  1121,    -1,  1121,   355,  1121,    -1,  1121,
     355,  1121,   355,  1121,    -1,  1121,   347,   366,   348,    -1,
    1121,   347,   366,   353,   366,   348,    -1,  1121,   347,   366,
     353,   352,   366,   348,    -1,  1121,   347,   366,   353,   354,
     366,   348,    -1,   137,   982,   115,  1002,   995,   153,   966,
     358,    -1,   115,  1002,   995,   153,   966,   358,    -1,   982,
     115,  1002,   995,   153,    -1,   985,   153,    -1,    -1,   983,
      -1,   983,   984,    -1,   984,    -1,   986,    -1,   987,    -1,
     988,    -1,   990,    -1,   985,   990,    -1,   990,    -1,   134,
    1121,   962,   181,   796,   885,   358,    -1,  1121,   156,   358,
      -1,  1121,   989,   977,   967,   358,    -1,    -1,   284,    -1,
     250,  1121,   181,   371,   347,   993,   348,   358,    -1,   250,
    1121,   181,   243,   202,   991,   992,   358,    -1,   978,    -1,
    1121,    -1,  1121,   355,  1121,    -1,  1121,   355,  1121,   355,
    1121,    -1,   286,   116,   727,    -1,   993,   353,   994,    -1,
     994,    -1,   569,   727,    -1,    -1,   996,    -1,   156,   997,
      -1,    -1,   998,    -1,   998,   999,    -1,   999,    -1,   263,
    1000,  1012,    -1,   263,   289,  1012,    -1,  1001,   207,  1001,
      -1,  1001,    -1,  1121,    -1,  1121,   355,  1121,    -1,  1002,
    1003,    -1,  1003,    -1,  1004,    -1,  1007,    -1,  1027,    -1,
    1028,    -1,  1008,    -1,  1030,    -1,  1009,    -1,  1013,    -1,
    1020,    -1,  1031,    -1,  1032,    -1,   979,    -1,  1033,    -1,
    1034,    -1,  1035,    -1,  1005,    -1,   359,   359,  1121,   361,
     361,    -1,   796,   885,   358,    -1,   763,   358,    -1,   770,
     358,    -1,   762,   358,    -1,   784,   358,    -1,   792,   358,
      -1,   771,   358,    -1,   580,   358,    -1,   577,   358,    -1,
     576,   358,    -1,   578,   358,    -1,  1006,   358,    -1,   972,
      -1,  1121,   355,   972,    -1,   975,   357,   360,   970,   358,
      -1,   237,   975,   360,   970,   358,    -1,   162,  1121,   180,
     973,   358,    -1,   162,  1121,   355,  1121,   180,   973,   358,
      -1,   174,   969,  1012,  1010,   153,   174,   358,    -1,    -1,
     150,  1002,    -1,  1011,    -1,   306,   969,  1012,  1010,    -1,
     151,   969,  1012,  1010,    -1,   246,  1002,    -1,   119,  1014,
    1019,   153,   119,   358,    -1,   119,   968,  1016,  1019,   153,
     119,   358,    -1,  1015,  1014,    -1,  1015,    -1,   263,   969,
    1012,    -1,  1017,  1016,    -1,  1017,    -1,   263,  1018,  1012,
      -1,   968,    -1,    -1,   150,  1002,    -1,  1023,    -1,  1022,
      -1,  1024,    -1,  1026,    -1,   193,  1002,   153,   193,    -1,
     265,   969,  1021,   966,   358,    -1,  1021,   966,   358,    -1,
     165,   976,  1145,   968,   344,   968,  1025,  1021,   966,   358,
      -1,   165,   976,   175,   314,   968,   344,   968,  1025,  1021,
     966,   358,    -1,    -1,   316,   968,    -1,   165,   976,  1145,
     972,  1021,   966,   358,    -1,   122,  1121,   358,    -1,   122,
    1121,   355,  1121,   358,    -1,   159,   966,  1029,   358,    -1,
      -1,   263,   969,    -1,   170,  1121,   358,    -1,   201,   358,
      -1,   206,   972,   358,    -1,   206,  1121,   355,   972,   358,
      -1,   290,  1001,   358,    -1,   290,   358,    -1,   228,   358,
      -1,   228,   970,   358,    -1,   131,   358,    -1,   105,   218,
     584,   127,    -1,   105,   169,   584,   127,    -1,   146,   218,
     584,    -1,   146,   169,   584,    -1,   146,   251,   584,    -1,
    1044,    -1,  1045,   971,    -1,   157,    -1,   158,    -1,   972,
      -1,  1121,   355,   972,    -1,  1121,   355,  1121,   355,   972,
      -1,  1131,   357,   360,    -1,   237,  1121,   360,  1121,    -1,
     237,   103,   360,   152,    -1,   237,   103,   360,   143,    -1,
     371,   360,  1076,    -1,   132,   136,  1121,  1047,  1049,  1050,
      -1,   109,    -1,   198,    -1,  1051,  1052,    -1,  1052,  1051,
      -1,   371,   237,  1121,    -1,   371,   371,   237,  1121,    -1,
     105,   136,  1121,  1057,    -1,   105,   136,  1121,  1058,    -1,
     105,   136,  1049,    -1,   105,   136,   113,   192,   247,   368,
      -1,   105,   136,   113,   317,  1121,   247,   368,    -1,   105,
     136,   113,   136,   247,   368,    -1,   105,   136,   132,   371,
     368,  1056,    -1,   105,   136,   291,   371,   368,   247,   368,
      -1,   105,   136,   223,   136,  1054,  1055,    -1,   105,   136,
     115,   371,    -1,   105,   136,   153,   371,    -1,    -1,   255,
     371,    -1,   255,   371,   368,    -1,    -1,   258,   113,   192,
      -1,    -1,   110,   368,    -1,   371,    -1,   371,   371,    -1,
     371,   388,    -1,   371,   159,    -1,   236,   122,   366,    -1,
     371,   126,   366,    -1,   371,   231,   366,    -1,   146,   136,
    1121,    -1,   132,   317,  1121,   371,  1066,  1071,    -1,   132,
     133,   317,  1121,   371,   366,  1068,    -1,   132,   133,   317,
    1121,   371,   366,   371,  1068,    -1,   132,   244,   317,  1121,
     371,  1066,  1073,    -1,   105,   317,  1121,  1078,    -1,   105,
     317,  1121,   291,   371,  1080,   247,  1080,    -1,   105,   317,
    1121,   105,   371,   368,  1079,    -1,   105,   317,  1121,  1063,
      -1,   115,   113,    -1,   153,   113,    -1,   105,   317,  1121,
     106,   371,  1066,    -1,   105,   317,  1121,   146,   371,  1080,
      -1,   105,   317,  1121,   105,   371,   368,  1069,    -1,   105,
     317,  1121,   105,   371,   368,   371,  1077,    -1,   146,   317,
    1121,  1082,    -1,  1066,   353,  1067,    -1,  1067,    -1,   368,
    1068,    -1,   368,   371,   366,  1068,    -1,   368,   371,   366,
     371,  1068,    -1,   368,   371,  1068,    -1,   368,   371,   366,
     371,   371,  1068,    -1,    -1,  1069,    -1,   371,   203,    -1,
     371,   205,    -1,   371,   205,   392,  1077,    -1,   371,   205,
     371,  1077,    -1,   371,   205,   392,   366,  1070,    -1,   371,
     205,   392,   366,   371,  1070,    -1,   371,   205,   371,   371,
      -1,   371,   371,    -1,   371,  1077,    -1,    -1,  1072,    -1,
    1072,  1072,    -1,  1074,    -1,  1075,    -1,    -1,  1074,    -1,
     307,  1077,    -1,   294,   371,   371,    -1,   366,    -1,   366,
     371,    -1,   366,    -1,   366,   371,    -1,   311,    -1,   312,
      -1,   311,    -1,   312,    -1,  1080,   353,  1081,    -1,  1081,
      -1,   368,    -1,    -1,   371,   371,   612,    -1,   371,   371,
     106,   371,   612,    -1,  1084,   584,  1087,   205,   584,  1091,
    1098,  1100,    -1,   132,   248,    -1,   132,   207,   292,   248,
      -1,   105,   248,   584,  1101,    -1,   146,   248,   584,    -1,
    1089,  1088,    -1,  1088,   207,   139,    -1,  1088,   207,   178,
      -1,  1088,   207,   256,  1090,    -1,   178,    -1,   139,    -1,
     256,  1090,    -1,   114,    -1,   102,    -1,    -1,   202,   734,
      -1,    -1,   225,  1092,    -1,  1092,   353,  1093,    -1,  1093,
      -1,  1094,  1095,  1096,  1097,    -1,   204,    -1,   197,    -1,
      -1,   232,    -1,   243,    -1,    -1,   110,    -1,  1121,    -1,
      -1,   165,   149,   232,  1099,    -1,   165,   149,   241,    -1,
      -1,   263,   347,   896,   348,    -1,   961,   980,    -1,   152,
      -1,   143,    -1,   127,    -1,  1103,  1121,   110,   368,    -1,
     132,   142,    -1,   132,   207,   292,   142,    -1,   146,   142,
    1121,    -1,   132,   298,   262,   584,   744,   699,   708,  1106,
     110,   797,    -1,    -1,   371,   371,    -1,   371,  1107,    -1,
     371,   371,  1107,    -1,   371,   371,   371,   371,    -1,   371,
     371,   371,  1107,    -1,   371,   371,   371,   371,  1107,    -1,
     205,   371,    -1,   205,   126,    -1,   105,   298,   262,   584,
    1109,    -1,   371,   371,    -1,   371,  1107,    -1,   371,   371,
    1107,    -1,   146,   298,   262,   584,    -1,   132,   371,   371,
    1043,  1044,   240,   921,  1112,  1113,  1115,  1117,    -1,    -1,
     153,   921,    -1,    -1,  1114,    -1,   371,   366,   371,    -1,
      -1,  1116,    -1,   152,    -1,   143,    -1,    -1,  1118,    -1,
     125,   368,    -1,   105,   371,   371,   237,  1043,  1044,    -1,
     105,   371,   371,   237,   240,   921,    -1,   105,   371,   371,
     237,   153,   921,    -1,   105,   371,   371,   237,  1114,    -1,
     105,   371,   371,   237,  1116,    -1,   105,   371,   371,   237,
    1118,    -1,   146,   371,   371,    -1,   371,    -1,   370,    -1,
     367,  1134,    -1,   377,   367,  1132,  1135,    -1,   379,   367,
    1132,  1135,    -1,  1122,   353,   367,  1134,    -1,  1122,   353,
     377,   367,  1132,  1135,    -1,  1122,   353,   379,   367,  1132,
    1135,    -1,  1124,    -1,  1123,   353,  1124,    -1,   367,   175,
    1134,    -1,   367,   209,  1134,    -1,   367,   175,   209,  1134,
      -1,   367,  1134,    -1,   377,   367,  1132,  1135,    -1,   379,
     367,  1132,  1135,    -1,  1125,   353,   367,    -1,   367,    -1,
      -1,   180,  1127,    -1,   367,  1134,    -1,  1127,   353,   367,
    1134,    -1,   377,   367,  1132,  1135,    -1,   379,   367,  1132,
    1135,    -1,  1127,   353,   377,   367,  1132,  1135,    -1,  1127,
     353,   379,   367,  1132,  1135,    -1,    -1,   180,  1130,    -1,
     367,    -1,   371,    -1,  1129,  1134,    -1,  1130,   353,  1129,
    1134,    -1,   377,  1129,  1132,  1135,    -1,   379,  1129,  1132,
    1135,    -1,  1130,   353,   377,  1129,  1132,  1135,    -1,  1130,
     353,   379,  1129,  1132,  1135,    -1,   367,  1134,    -1,  1133,
     367,    -1,    -1,   395,    -1,    -1,  1135,    -1,  1136,   367,
      -1,    -1,   389,    -1,    -1,   317,  1121,    -1,  1137,   708,
    1139,    -1,    -1,  1140,    -1,   221,   394,    -1,   221,   268,
      -1,   221,   371,    -1,    -1,   347,  1142,   348,    -1,  1142,
     353,  1143,    -1,  1143,    -1,   212,  1121,   709,    -1,    -1,
     212,   347,  1121,   348,    -1,   176,    -1,   175,    -1
};

/* YYRLINE[YYN] -- source line where rule number YYN was defined.  */
static const yytype_uint16 yyrline[] =
{
       0,   562,   562,   563,   567,   568,   569,   570,   584,   585,
     589,   590,   591,   592,   596,   597,   598,   599,   600,   601,
     602,   603,   607,   608,   612,   613,   614,   615,   616,   617,
     621,   622,   623,   624,   625,   626,   630,   631,   635,   636,
     637,   638,   642,   643,   644,   648,   649,   650,   654,   655,
     656,   657,   658,   662,   663,   664,   668,   669,   673,   674,
     678,   679,   683,   684,   688,   689,   693,   694,   698,   699,
     703,   704,   705,   706,   707,   708,   709,   710,   711,   712,
     713,   717,   718,   722,   726,   733,   799,   800,   801,   802,
     806,   951,  1073,  1118,  1119,  1123,  1127,  1135,  1136,  1137,
    1141,  1146,  1161,  1166,  1181,  1186,  1200,  1213,  1217,  1221,
    1227,  1233,  1234,  1235,  1241,  1248,  1254,  1262,  1268,  1274,
    1280,  1286,  1292,  1298,  1304,  1310,  1316,  1322,  1328,  1334,
    1349,  1355,  1361,  1368,  1373,  1381,  1383,  1390,  1404,  1427,
    1441,  1457,  1482,  1523,  1538,  1580,  1619,  1620,  1624,  1625,
    1629,  1665,  1669,  1780,  1894,  1938,  1939,  1940,  1944,  1945,
    1946,  1950,  1951,  1955,  1956,  1960,  1965,  1973,  1974,  1975,
    2005,  2048,  2049,  2053,  2057,  2067,  2075,  2080,  2085,  2090,
    2098,  2099,  2103,  2104,  2108,  2109,  2113,  2114,  2118,  2119,
    2124,  2193,  2197,  2198,  2202,  2203,  2204,  2208,  2209,  2210,
    2211,  2212,  2213,  2214,  2215,  2216,  2220,  2221,  2222,  2226,
    2227,  2231,  2232,  2233,  2234,  2235,  2236,  2240,  2242,  2243,
    2244,  2245,  2246,  2250,  2251,  2256,  2257,  2264,  2268,  2269,
    2273,  2283,  2284,  2288,  2289,  2293,  2294,  2295,  2299,  2300,
    2301,  2302,  2303,  2304,  2305,  2306,  2307,  2308,  2312,  2313,
    2314,  2315,  2316,  2320,  2321,  2325,  2326,  2330,  2355,  2356,
    2366,  2372,  2376,  2380,  2385,  2390,  2395,  2400,  2405,  2409,
    2437,  2469,  2502,  2540,  2548,  2614,  2690,  2731,  2772,  2808,
    2846,  2865,  2880,  2895,  2910,  2924,  2930,  2939,  2953,  2965,
    2968,  2988,  3006,  3010,  3015,  3020,  3025,  3029,  3034,  3039,
    3044,  3062,  3075,  3083,  3103,  3107,  3112,  3117,  3127,  3131,
    3142,  3146,  3151,  3158,  3167,  3169,  3177,  3178,  3179,  3182,
    3184,  3188,  3189,  3197,  3208,  3212,  3217,  3228,  3233,  3238,
    3244,  3246,  3247,  3248,  3252,  3256,  3260,  3264,  3268,  3272,
    3276,  3283,  3287,  3291,  3305,  3310,  3322,  3326,  3337,  3361,
    3395,  3430,  3472,  3522,  3571,  3579,  3583,  3597,  3602,  3607,
    3612,  3617,  3622,  3628,  3633,  3638,  3646,  3647,  3650,  3652,
    3667,  3668,  3672,  3702,  3714,  3728,  3745,  3763,  3767,  3797,
    3804,  3814,  3818,  3822,  3826,  3830,  3838,  3845,  3853,  3881,
    3888,  3894,  3900,  3914,  3931,  3942,  3947,  3952,  3957,  3964,
    3971,  3989,  4010,  4026,  4032,  4048,  4055,  4062,  4069,  4076,
    4087,  4094,  4101,  4108,  4115,  4122,  4133,  4143,  4145,  4152,
    4160,  4177,  4183,  4187,  4192,  4199,  4201,  4203,  4205,  4206,
    4207,  4208,  4210,  4211,  4213,  4214,  4216,  4218,  4219,  4220,
    4221,  4222,  4223,  4224,  4225,  4226,  4227,  4228,  4229,  4230,
    4231,  4232,  4233,  4234,  4235,  4236,  4237,  4238,  4239,  4240,
    4241,  4242,  4243,  4244,  4246,  4247,  4248,  4249,  4250,  4251,
    4252,  4254,  4256,  4257,  4258,  4260,  4261,  4262,  4263,  4264,
    4266,  4267,  4268,  4270,  4271,  4273,  4274,  4275,  4277,  4278,
    4279,  4281,  4283,  4284,  4285,  4287,  4288,  4294,  4305,  4315,
    4326,  4337,  4348,  4359,  4370,  4381,  4392,  4403,  4415,  4416,
    4417,  4421,  4429,  4450,  4461,  4492,  4520,  4524,  4532,  4536,
    4550,  4551,  4552,  4553,  4554,  4555,  4556,  4557,  4558,  4559,
    4560,  4561,  4562,  4563,  4564,  4565,  4566,  4567,  4568,  4569,
    4570,  4571,  4572,  4573,  4574,  4577,  4578,  4579,  4580,  4581,
    4582,  4583,  4584,  4585,  4586,  4587,  4588,  4589,  4590,  4591,
    4592,  4593,  4594,  4595,  4596,  4597,  4598,  4599,  4600,  4601,
    4602,  4603,  4604,  4605,  4612,  4613,  4614,  4615,  4616,  4617,
    4618,  4623,  4624,  4628,  4629,  4630,  4631,  4646,  4660,  4674,
    4688,  4690,  4704,  4709,  4736,  4771,  4784,  4797,  4810,  4823,
    4836,  4850,  4867,  4869,  4873,  4874,  4889,  4893,  4897,  4902,
    4913,  4915,  4919,  4931,  4947,  4963,  4978,  4980,  4981,  4996,
    5009,  5025,  5026,  5030,  5031,  5035,  5036,  5040,  5044,  5045,
    5049,  5050,  5051,  5052,  5056,  5057,  5058,  5059,  5060,  5064,
    5065,  5069,  5070,  5074,  5078,  5082,  5086,  5090,  5091,  5095,
    5100,  5104,  5108,  5109,  5113,  5119,  5124,  5132,  5133,  5138,
    5150,  5151,  5152,  5153,  5154,  5155,  5156,  5157,  5159,  5160,
    5161,  5162,  5164,  5165,  5166,  5167,  5168,  5169,  5170,  5171,
    5172,  5173,  5174,  5175,  5176,  5177,  5178,  5179,  5180,  5181,
    5182,  5183,  5184,  5185,  5186,  5187,  5188,  5189,  5190,  5191,
    5192,  5193,  5195,  5196,  5198,  5199,  5200,  5201,  5203,  5204,
    5205,  5206,  5218,  5230,  5243,  5244,  5245,  5246,  5247,  5248,
    5249,  5250,  5251,  5252,  5254,  5255,  5257,  5261,  5262,  5266,
    5267,  5270,  5272,  5276,  5277,  5281,  5286,  5291,  5298,  5300,
    5303,  5305,  5321,  5326,  5335,  5338,  5348,  5351,  5353,  5355,
    5368,  5381,  5394,  5396,  5416,  5419,  5422,  5425,  5426,  5427,
    5429,  5448,  5463,  5467,  5469,  5484,  5485,  5489,  5507,  5508,
    5509,  5513,  5514,  5518,  5533,  5535,  5539,  5540,  5555,  5556,
    5560,  5564,  5566,  5579,  5594,  5596,  5597,  5610,  5624,  5626,
    5642,  5643,  5647,  5649,  5651,  5654,  5656,  5657,  5658,  5662,
    5663,  5664,  5676,  5679,  5681,  5685,  5686,  5690,  5691,  5694,
    5696,  5708,  5727,  5731,  5735,  5738,  5740,  5744,  5748,  5752,
    5756,  5760,  5764,  5765,  5769,  5773,  5777,  5778,  5782,  5783,
    5784,  5787,  5789,  5792,  5794,  5795,  5799,  5800,  5804,  5805,
    5809,  5810,  5811,  5824,  5837,  5854,  5867,  5884,  5885,  5889,
    5895,  5898,  5901,  5904,  5907,  5911,  5915,  5916,  5917,  5918,
    5933,  5935,  5938,  5939,  5941,  5944,  5949,  5953,  5958,  5960,
    5965,  5969,  5971,  5975,  5977,  5980,  5982,  5986,  5987,  5988,
    5989,  5992,  5994,  5995,  6001,  6007,  6008,  6009,  6010,  6014,
    6016,  6021,  6022,  6026,  6029,  6031,  6036,  6037,  6041,  6044,
    6046,  6051,  6052,  6056,  6057,  6062,  6066,  6067,  6071,  6072,
    6073,  6075,  6081,  6083,  6086,  6088,  6092,  6093,  6097,  6098,
    6099,  6100,  6101,  6105,  6109,  6110,  6111,  6112,  6116,  6119,
    6121,  6124,  6126,  6130,  6134,  6149,  6150,  6151,  6154,  6156,
    6175,  6177,  6189,  6203,  6205,  6209,  6210,  6214,  6215,  6219,
    6223,  6225,  6228,  6230,  6233,  6235,  6239,  6240,  6244,  6247,
    6248,  6252,  6255,  6259,  6262,  6267,  6269,  6270,  6271,  6272,
    6273,  6275,  6279,  6281,  6282,  6287,  6293,  6294,  6299,  6302,
    6303,  6306,  6311,  6312,  6316,  6321,  6322,  6326,  6327,  6344,
    6346,  6350,  6351,  6355,  6358,  6363,  6364,  6368,  6369,  6387,
    6388,  6392,  6393,  6398,  6402,  6406,  6409,  6413,  6417,  6420,
    6422,  6426,  6427,  6431,  6432,  6436,  6440,  6442,  6443,  6446,
    6448,  6451,  6453,  6457,  6458,  6462,  6463,  6464,  6465,  6469,
    6473,  6474,  6478,  6480,  6483,  6485,  6489,  6490,  6491,  6493,
    6495,  6499,  6501,  6505,  6508,  6510,  6514,  6520,  6521,  6525,
    6526,  6530,  6531,  6535,  6539,  6541,  6542,  6543,  6544,  6549,
    6562,  6566,  6569,  6571,  6576,  6586,  6597,  6604,  6610,  6612,
    6625,  6639,  6641,  6645,  6646,  6650,  6653,  6655,  6660,  6665,
    6670,  6671,  6672,  6673,  6678,  6679,  6684,  6685,  6701,  6706,
    6707,  6719,  6744,  6745,  6750,  6754,  6759,  6770,  6786,  6790,
    6791,  6799,  6800,  6804,  6811,  6812,  6814,  6816,  6819,  6824,
    6825,  6829,  6835,  6836,  6840,  6841,  6845,  6846,  6849,  6853,
    6861,  6873,  6877,  6881,  6888,  6892,  6896,  6897,  6900,  6902,
    6903,  6907,  6908,  6912,  6913,  6914,  6919,  6920,  6924,  6925,
    6929,  6930,  6937,  6945,  6946,  6950,  6954,  6955,  6956,  6960,
    6962,  6967,  6969,  6973,  6974,  6977,  6979,  6986,  6992,  6994,
    6998,  6999,  7003,  7004,  7011,  7012,  7016,  7020,  7024,  7025,
    7026,  7027,  7031,  7032,  7035,  7037,  7041,  7045,  7046,  7050,
    7054,  7055,  7059,  7066,  7076,  7077,  7081,  7085,  7090,  7091,
    7100,  7104,  7105,  7109,  7113,  7114,  7118,  7127,  7129,  7132,
    7134,  7137,  7139,  7142,  7144,  7145,  7149,  7150,  7153,  7155,
    7159,  7160,  7164,  7165,  7166,  7169,  7171,  7172,  7173,  7174,
    7178,  7182,  7183,  7187,  7188,  7189,  7190,  7191,  7192,  7193,
    7194,  7195,  7199,  7200,  7201,  7202,  7203,  7206,  7208,  7209,
    7213,  7217,  7218,  7222,  7223,  7227,  7228,  7232,  7236,  7237,
    7241,  7245,  7248,  7250,  7254,  7255,  7259,  7260,  7264,  7268,
    7272,  7273,  7277,  7278,  7282,  7283,  7287,  7288,  7292,  7296,
    7297,  7301,  7302,  7306,  7310,  7314,  7315,  7319,  7320,  7321,
    7325,  7326,  7327,  7331,  7332,  7333,  7337,  7338,  7339,  7340,
    7341,  7342,  7343,  7347,  7352,  7353,  7357,  7358,  7363,  7367,
    7369,  7370,  7371,  7372,  7375,  7377,  7381,  7383,  7388,  7389,
    7393,  7397,  7402,  7403,  7404,  7405,  7409,  7413,  7414,  7418,
    7419,  7423,  7424,  7425,  7428,  7430,  7433,  7435,  7438,  7440,
    7443,  7449,  7452,  7454,  7458,  7459,  7462,  7464,  7478,  7480,
    7481,  7497,  7499,  7503,  7507,  7508,  7523,  7554,  7556,  7557,
    7561,  7564,  7566,  7569,  7571,  7585,  7589,  7591,  7592,  7593,
    7594,  7595,  7596,  7600,  7601,  7605,  7608,  7610,  7611,  7614,
    7616,  7617,  7624,  7626,  7628,  7630,  7635,  7650,  7663,  7677,
    7695,  7697,  7717,  7719,  7723,  7727,  7728,  7732,  7733,  7737,
    7738,  7742,  7744,  7746,  7748,  7750,  7752,  7754,  7756,  7758,
    7760,  7762,  7764,  7766,  7767,  7768,  7769,  7770,  7771,  7772,
    7773,  7774,  7775,  7776,  7777,  7778,  7779,  7780,  7781,  7782,
    7795,  7796,  7800,  7804,  7805,  7809,  7813,  7817,  7821,  7825,
    7829,  7830,  7831,  7835,  7839,  7843,  7847,  7851,  7852,  7853,
    7857,  7858,  7859,  7860,  7864,  7865,  7869,  7870,  7874,  7875,
    7879,  7880,  7884,  7885,  7889,  7890,  7891,  7892,  7893,  7894,
    7895,  7899,  7903,  7904,  7908,  7909,  7910,  7914,  7915,  7916,
    7920,  7921,  7922,  7923,  7927,  7928,  7929,  7930,  7931,  7932,
    7933,  7934,  7935,  7936,  7937,  7938,  7940,  7941,  7942,  7943,
    7944,  7945,  7946,  7947,  7948,  7949,  7950,  7955,  7956,  7957,
    7960,  7962,  7973,  7985,  7992,  7993,  7997,  8003,  8007,  8009,
    8014,  8015,  8019,  8027,  8032,  8034,  8037,  8039,  8043,  8047,
    8051,  8054,  8056,  8058,  8060,  8062,  8064,  8068,  8070,  8078,
    8079,  8083,  8084,  8085,  8089,  8091,  8099,  8101,  8106,  8107,
    8112,  8115,  8117,  8120,  8122,  8123,  8127,  8128,  8129,  8130,
    8134,  8135,  8136,  8137,  8141,  8142,  8146,  8147,  8151,  8155,
    8156,  8160,  8163,  8165,  8175,  8176,  8183,  8194,  8204,  8215,
    8216,  8220,  8221,  8225,  8226,  8230,  8231,  8237,  8239,  8240,
    8246,  8247,  8251,  8257,  8259,  8260,  8261,  8267,  8269,  8272,
    8274,  8275,  8282,  8286,  8291,  8298,  8299,  8300,  8307,  8308,
    8309,  8316,  8317,  8321,  8325,  8331,  8337,  8348,  8349,  8350,
    8355,  8359,  8366,  8369,  8374,  8377,  8382,  8389,  8390,  8391,
    8394,  8405,  8406,  8408,  8410,  8418,  8426,  8435,  8445,  8452,
    8454,  8458,  8459,  8463,  8464,  8465,  8466,  8473,  8474,  8481,
    8494,  8501,  8508,  8510,  8517,  8535,  8544,  8545,  8546,  8547,
    8551,  8555,  8556,  8560,  8566,  8568,  8572,  8575,  8577,  8581,
    8582,  8586,  8589,  8595,  8596,  8600,  8601,  8608,  8609,  8613,
    8614,  8615,  8616,  8617,  8618,  8619,  8620,  8621,  8622,  8623,
    8624,  8625,  8626,  8627,  8628,  8632,  8640,  8641,  8642,  8643,
    8644,  8645,  8646,  8647,  8648,  8649,  8650,  8651,  8655,  8656,
    8663,  8668,  8679,  8684,  8697,  8704,  8706,  8707,  8711,  8715,
    8722,  8730,  8734,  8742,  8743,  8747,  8753,  8754,  8758,  8764,
    8767,  8769,  8776,  8777,  8778,  8779,  8783,  8793,  8804,  8811,
    8821,  8834,  8836,  8843,  8856,  8857,  8865,  8871,  8873,  8877,
    8881,  8885,  8886,  8894,  8895,  8900,  8901,  8905,  8912,  8917,
    8922,  8926,  8931,  8938,  8942,  8946,  8947,  8951,  8952,  8954,
    8958,  8962,  8963,  8964,  8968,  8985,  8994,  8995,  8999,  9000,
    9004,  9021,  9040,  9041,  9042,  9043,  9044,  9045,  9046,  9059,
    9072,  9074,  9087,  9100,  9102,  9115,  9130,  9132,  9135,  9137,
    9141,  9160,  9192,  9205,  9221,  9222,  9235,  9251,  9255,  9270,
    9289,  9323,  9341,  9344,  9360,  9376,  9380,  9381,  9385,  9401,
    9417,  9433,  9460,  9465,  9466,  9470,  9472,  9486,  9511,  9525,
    9550,  9552,  9556,  9570,  9584,  9599,  9616,  9630,  9648,  9669,
    9684,  9701,  9703,  9704,  9708,  9709,  9712,  9714,  9718,  9722,
    9726,  9727,  9747,  9748,  9768,  9769,  9773,  9774,  9778,  9779,
    9783,  9786,  9788,  9803,  9825,  9835,  9836,  9840,  9844,  9848,
    9852,  9853,  9854,  9855,  9856,  9857,  9861,  9862,  9865,  9867,
    9871,  9873,  9878,  9879,  9883,  9890,  9891,  9894,  9896,  9897,
    9900,  9902,  9906,  9909,  9911,  9912,  9915,  9917,  9921,  9926,
    9927,  9928,  9932,  9938,  9939,  9943,  9947,  9955,  9957, 10017,
   10032, 10067, 10129, 10170, 10223, 10237, 10241, 10246, 10281, 10296,
   10334, 10339, 10361, 10363, 10366, 10368, 10372, 10376, 10378, 10383,
   10384, 10388, 10390, 10395, 10399, 10412, 10425, 10438, 10450, 10463,
   10479, 10494, 10495, 10503, 10521, 10532, 10543, 10554, 10565, 10579,
   10580, 10584, 10595, 10606, 10617, 10628, 10639, 10653, 10691, 10718,
   10720, 10727, 10738, 10749, 10760, 10771, 10782, 10795, 10797, 10804,
   10805, 10809, 10820, 10831, 10842, 10853, 10864, 10878, 10893, 10919,
   10921, 10927, 10929, 10933, 11031, 11033, 11039, 11041, 11044, 11047,
   11049, 11053, 11054, 11055, 11071, 11073, 11079, 11080, 11084, 11087,
   11089, 11094, 11095
};
#endif

#if YYDEBUG || YYERROR_VERBOSE || YYTOKEN_TABLE
/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "$end", "error", "$undefined", "END_OF_FILE", "C_AUTO", "C_BREAK",
  "C_CASE", "C_CHAR", "C_VARCHAR", "C_CONST", "C_CONTINUE", "C_DEFAULT",
  "C_DO", "C_DOUBLE", "C_ENUM", "C_EXTERN", "C_FLOAT", "C_FOR", "C_GOTO",
  "C_INT", "C_LONG", "C_REGISTER", "C_RETURN", "C_SHORT", "C_SIGNED",
  "C_SIZEOF", "C_STATIC", "C_STRUCT", "C_SWITCH", "C_TYPEDEF", "C_UNION",
  "C_UNSIGNED", "C_VOID", "C_VOLATILE", "C_WHILE", "C_ELIPSIS", "C_ELSE",
  "C_IF", "C_CONSTANT", "C_IDENTIFIER", "C_TYPE_NAME", "C_STRING_LITERAL",
  "C_RIGHT_ASSIGN", "C_LEFT_ASSIGN", "C_ADD_ASSIGN", "C_SUB_ASSIGN",
  "C_MUL_ASSIGN", "C_DIV_ASSIGN", "C_MOD_ASSIGN", "C_AND_ASSIGN",
  "C_XOR_ASSIGN", "C_OR_ASSIGN", "C_INC_OP", "C_DEC_OP", "C_PTR_OP",
  "C_AND_OP", "C_EQ_OP", "C_NE_OP", "C_RIGHT_OP", "C_LEFT_OP", "C_OR_OP",
  "C_LE_OP", "C_GE_OP", "C_APRE_BINARY", "C_APRE_BINARY2", "C_APRE_BIT",
  "C_APRE_BYTES", "C_APRE_VARBYTES", "C_APRE_NIBBLE", "C_APRE_INTEGER",
  "C_APRE_NUMERIC", "C_APRE_BLOB_LOCATOR", "C_APRE_CLOB_LOCATOR",
  "C_APRE_BLOB", "C_APRE_CLOB", "C_SQLLEN", "C_SQL_TIMESTAMP_STRUCT",
  "C_SQL_TIME_STRUCT", "C_SQL_DATE_STRUCT", "C_SQL_DA_STRUCT",
  "C_FAILOVERCB", "C_NCHAR_CS", "C_ATTRIBUTE", "M_INCLUDE", "M_DEFINE",
  "M_UNDEF", "M_FUNCTION", "M_LBRAC", "M_RBRAC", "M_DQUOTE", "M_FILENAME",
  "M_IF", "M_ELIF", "M_ELSE", "M_ENDIF", "M_IFDEF", "M_IFNDEF",
  "M_CONSTANT", "M_IDENTIFIER", "EM_SQLSTART", "EM_ERROR", "TR_ADD",
  "TR_AFTER", "TR_AGER", "TR_ALL", "TR_ALTER", "TR_AND", "TR_ANY",
  "TR_ARCHIVE", "TR_ARCHIVELOG", "TR_AS", "TR_ASC", "TR_AT", "TR_BACKUP",
  "TR_BEFORE", "TR_BEGIN", "TR_BY", "TR_BIND", "TR_CASCADE", "TR_CASE",
  "TR_CAST", "TR_CHECK_OPENING_PARENTHESIS", "TR_CLOSE", "TR_COALESCE",
  "TR_COLUMN", "TR_COMMENT", "TR_COMMIT", "TR_COMPILE", "TR_CONNECT",
  "TR_CONSTRAINT", "TR_CONSTRAINTS", "TR_CONTINUE", "TR_CREATE",
  "TR_VOLATILE", "TR_CURSOR", "TR_CYCLE", "TR_DATABASE", "TR_DECLARE",
  "TR_DEFAULT", "TR_DELETE", "TR_DEQUEUE", "TR_DESC", "TR_DIRECTORY",
  "TR_DISABLE", "TR_DISCONNECT", "TR_DISTINCT", "TR_DROP", "TR_DESCRIBE",
  "TR_DESCRIPTOR", "TR_EACH", "TR_ELSE", "TR_ELSEIF", "TR_ENABLE",
  "TR_END", "TR_ENQUEUE", "TR_ESCAPE", "TR_EXCEPTION", "TR_EXEC",
  "TR_EXECUTE", "TR_EXIT", "TR_FAILOVERCB", "TR_FALSE", "TR_FETCH",
  "TR_FIFO", "TR_FLUSH", "TR_FOR", "TR_FOREIGN", "TR_FROM", "TR_FULL",
  "TR_FUNCTION", "TR_GOTO", "TR_GRANT", "TR_GROUP", "TR_HAVING", "TR_IF",
  "TR_IN", "TR_IN_BF_LPAREN", "TR_INNER", "TR_INSERT", "TR_INTERSECT",
  "TR_INTO", "TR_IS", "TR_ISOLATION", "TR_JOIN", "TR_KEY", "TR_LEFT",
  "TR_LESS", "TR_LEVEL", "TR_LIFO", "TR_LIKE", "TR_LIMIT", "TR_LOCAL",
  "TR_LOGANCHOR", "TR_LOOP", "TR_MERGE", "TR_MOVE", "TR_MOVEMENT",
  "TR_NEW", "TR_NOARCHIVELOG", "TR_NOCYCLE", "TR_NOT", "TR_NULL", "TR_OF",
  "TR_OFF", "TR_OLD", "TR_ON", "TR_OPEN", "TR_OR", "TR_ORDER", "TR_OUT",
  "TR_OUTER", "TR_OVER", "TR_PARTITION", "TR_PARTITIONS", "TR_POINTER",
  "TR_PRIMARY", "TR_PRIOR", "TR_PRIVILEGES", "TR_PROCEDURE", "TR_PUBLIC",
  "TR_QUEUE", "TR_READ", "TR_REBUILD", "TR_RECOVER", "TR_REFERENCES",
  "TR_REFERENCING", "TR_REGISTER", "TR_RESTRICT", "TR_RETURN", "TR_REVOKE",
  "TR_RIGHT", "TR_ROLLBACK", "TR_ROW", "TR_SAVEPOINT", "TR_SELECT",
  "TR_SEQUENCE", "TR_SESSION", "TR_SET", "TR_SOME", "TR_SPLIT", "TR_START",
  "TR_STATEMENT", "TR_SYNONYM", "TR_TABLE", "TR_TEMPORARY", "TR_THAN",
  "TR_THEN", "TR_TO", "TR_TRIGGER", "TR_TRUE", "TR_TYPE", "TR_TYPESET",
  "TR_UNION", "TR_UNIQUE", "TR_UNREGISTER", "TR_UNTIL", "TR_UPDATE",
  "TR_USER", "TR_USING", "TR_VALUES", "TR_VARIABLE", "TR_VARIABLES",
  "TR_VIEW", "TR_WHEN", "TR_WHERE", "TR_WHILE", "TR_WITH", "TR_WORK",
  "TR_WRITE", "TR_PARALLEL", "TR_NOPARALLEL", "TR_LOB", "TR_STORE",
  "TR_ENDEXEC", "TR_PRECEDING", "TR_FOLLOWING", "TR_CURRENT_ROW",
  "TR_LINK", "TR_ROLE", "TR_WITHIN", "TR_LOGGING", "TK_BETWEEN",
  "TK_EXISTS", "TO_ACCESS", "TO_CONSTANT", "TO_IDENTIFIED", "TO_INDEX",
  "TO_MINUS", "TO_MODE", "TO_OTHERS", "TO_RAISE", "TO_RENAME",
  "TO_REPLACE", "TO_ROWTYPE", "TO_SEGMENT", "TO_WAIT", "TO_PIVOT",
  "TO_UNPIVOT", "TO_MATERIALIZED", "TO_CONNECT_BY_NOCYCLE",
  "TO_CONNECT_BY_ROOT", "TO_NULLS", "TO_PURGE", "TO_FLASHBACK",
  "TO_VC2COLL", "TO_KEEP", "TA_ELSIF", "TA_EXTENTSIZE", "TA_FIXED",
  "TA_LOCK", "TA_MAXROWS", "TA_ONLINE", "TA_OFFLINE", "TA_REPLICATION",
  "TA_REVERSE", "TA_ROWCOUNT", "TA_STEP", "TA_TABLESPACE", "TA_TRUNCATE",
  "TA_SQLCODE", "TA_SQLERRM", "TA_LINKER", "TA_REMOTE_TABLE", "TA_SHARD",
  "TA_DISJOIN", "TA_CONJOIN", "TA_SEC", "TA_MSEC", "TA_USEC", "TA_SECOND",
  "TA_MILLISECOND", "TA_MICROSECOND", "TA_ANALYSIS_PROPAGATION",
  "TI_NONQUOTED_IDENTIFIER", "TI_QUOTED_IDENTIFIER", "TI_HOSTVARIABLE",
  "TL_TYPED_LITERAL", "TL_LITERAL", "TL_NCHAR_LITERAL",
  "TL_UNICODE_LITERAL", "TL_INTEGER", "TL_NUMERIC", "TS_AT_SIGN",
  "TS_CONCATENATION_SIGN", "TS_DOUBLE_PERIOD", "TS_EXCLAMATION_POINT",
  "TS_PERCENT_SIGN", "TS_OPENING_PARENTHESIS", "TS_CLOSING_PARENTHESIS",
  "TS_OPENING_BRACKET", "TS_CLOSING_BRACKET", "TS_ASTERISK",
  "TS_PLUS_SIGN", "TS_COMMA", "TS_MINUS_SIGN", "TS_PERIOD", "TS_SLASH",
  "TS_COLON", "TS_SEMICOLON", "TS_LESS_THAN_SIGN", "TS_EQUAL_SIGN",
  "TS_GREATER_THAN_SIGN", "TS_QUESTION_MARK", "TS_OUTER_JOIN_OPERATOR",
  "TX_HINTS", "SES_V_NUMERIC", "SES_V_INTEGER", "SES_V_HOSTVARIABLE",
  "SES_V_LITERAL", "SES_V_TYPED_LITERAL", "SES_V_DQUOTE_LITERAL",
  "SES_V_IDENTIFIER", "SES_V_ABSOLUTE", "SES_V_ALLOCATE",
  "SES_V_ASENSITIVE", "SES_V_AUTOCOMMIT", "SES_V_BATCH", "SES_V_BLOB_FILE",
  "SES_V_BREAK", "SES_V_CLOB_FILE", "SES_V_CUBE", "SES_V_DEALLOCATE",
  "SES_V_DESCRIPTOR", "SES_V_DO", "SES_V_FIRST", "SES_V_FOUND",
  "SES_V_FREE", "SES_V_HOLD", "SES_V_IMMEDIATE", "SES_V_INDICATOR",
  "SES_V_INSENSITIVE", "SES_V_LAST", "SES_V_NEXT", "SES_V_ONERR",
  "SES_V_ONLY", "APRE_V_OPTION", "SES_V_PREPARE", "SES_V_RELATIVE",
  "SES_V_RELEASE", "SES_V_ROLLUP", "SES_V_SCROLL", "SES_V_SENSITIVE",
  "SES_V_SQLERROR", "SES_V_THREADS", "SES_V_WHENEVER", "SES_V_CURRENT",
  "SES_V_GROUPING_SETS", "'('", "')'", "'['", "']'", "'.'", "','", "'&'",
  "'*'", "'+'", "'-'", "'~'", "'!'", "'/'", "'%'", "'<'", "'>'", "'^'",
  "'|'", "'?'", "':'", "'='", "';'", "'}'", "'{'", "$accept", "program",
  "combined_grammar", "C_grammar", "primary_expr", "postfix_expr",
  "argument_expr_list", "unary_expr", "unary_operator", "cast_expr",
  "multiplicative_expr", "additive_expr", "shift_expr", "relational_expr",
  "equality_expr", "and_expr", "exclusive_or_expr", "inclusive_or_expr",
  "logical_and_expr", "logical_or_expr", "conditional_expr",
  "assignment_expr", "assignment_operator", "expr", "constant_expr",
  "declaration", "declaration_specifiers", "init_declarator_list",
  "var_decl_list_begin", "init_declarator", "storage_class_specifier",
  "type_specifier", "attribute_specifier", "struct_or_union_specifier",
  "struct_decl_begin", "no_tag_struct_decl_begin", "struct_or_union",
  "struct_declaration_or_macro_list", "struct_declaration_or_macro",
  "struct_declaration", "struct_declarator_list", "struct_decl_list_begin",
  "struct_declarator", "enum_specifier", "enumerator_list", "enumerator",
  "declarator", "declarator2", "arr_decl_begin", "func_decl_begin",
  "pointer", "type_specifier_list", "parameter_identifier_list",
  "identifier_list", "parameter_type_list", "parameter_list",
  "parameter_declaration", "type_name", "abstract_declarator",
  "abstract_declarator2", "initializer", "initializer_list", "statement",
  "labeled_statement", "compound_statement", "super_compound_stmt",
  "super_compound_stmt_list", "compound_statement_begin",
  "declaration_list", "expression_statement", "selection_statement",
  "iteration_statement", "jump_statement", "function_definition",
  "function_body", "identifier", "string_literal", "Macro_grammar",
  "Macro_include", "Macro_define", "Macro_undef", "Macro_if", "Macro_elif",
  "Macro_ifdef", "Macro_ifndef", "Macro_else", "Macro_endif",
  "Emsql_grammar", "at_clause", "esql_stmt", "declare_cursor_stmt",
  "begin_declare", "cursor_sensitivity_opt", "cursor_scroll_opt",
  "cursor_hold_opt", "cursor_method_opt", "cursor_update_list",
  "cursor_update_column_list", "declare_statement_stmt",
  "open_cursor_stmt", "fetch_cursor_stmt", "fetch_orientation_from",
  "fetch_orientation", "fetch_integer", "close_cursor_stmt",
  "autocommit_stmt", "conn_stmt", "disconn_stmt", "free_lob_loc_stmt",
  "dsql_stmt", "alloc_descriptor_stmt", "with_max_option",
  "dealloc_descriptor_stmt", "prepare_stmt", "begin_prepare",
  "dealloc_prepared_stmt", "using_descriptor", "execute_immediate_stmt",
  "begin_immediate", "execute_stmt", "bind_stmt", "set_array_stmt",
  "select_list_stmt", "proc_stmt", "proc_func_begin", "etc_stmt",
  "option_stmt", "exception_stmt", "threads_stmt", "sharedptr_stmt",
  "sql_stmt", "direct_sql_stmt", "indirect_sql_stmt", "pre_clause",
  "onerr_clause", "for_clause", "commit_sql_stmt", "rollback_sql_stmt",
  "column_name", "memberfunc_name", "keyword_function_name",
  "alter_session_set_statement", "alter_system_statement", "opt_local",
  "archivelog_start_option", "commit_statement", "savepoint_statement",
  "rollback_statement", "opt_work_clause", "set_transaction_statement",
  "commit_force_database_link_statement",
  "rollback_force_database_link_statement", "transaction_mode",
  "user_object_name", "user_object_column_name", "create_user_statement",
  "alter_user_statement", "user_options", "create_user_option",
  "user_option", "disable_tcp_option", "access_options", "password_def",
  "temporary_tablespace", "default_tablespace", "access", "access_option",
  "drop_user_statement", "drop_role_statement", "create_role_statement",
  "grant_statement", "grant_system_privileges_statement",
  "grant_object_privileges_statement", "privilege_list", "privilege",
  "grantees_clause", "grantee", "opt_with_grant_option",
  "revoke_statement", "revoke_system_privileges_statement",
  "revoke_object_privileges_statement", "opt_cascade_constraints",
  "opt_force", "create_synonym_statement", "drop_synonym_statement",
  "replication_statement", "opt_repl_options", "repl_options",
  "repl_option", "offline_dirs", "repl_mode", "opt_repl_mode",
  "replication_with_hosts", "replication_hosts", "repl_host",
  "opt_using_conntype", "opt_role", "opt_conflict_resolution",
  "repl_tbl_commalist", "repl_tbl", "repl_flush_option", "repl_sync_retry",
  "opt_repl_sync_table", "repl_sync_table_commalist", "repl_sync_table",
  "repl_start_option", "truncate_table_statement",
  "rename_table_statement", "flashback_table_statement",
  "opt_flashback_rename", "drop_sequence_statement",
  "drop_index_statement", "drop_table_statement", "purge_table_statement",
  "disjoin_table_statement", "disjoin_partitioning_clause",
  "join_table_statement", "join_partitioning_clause",
  "join_table_attr_list", "join_table_attr", "join_table_options",
  "opt_drop_behavior", "alter_sequence_statement", "sequence_options",
  "sequence_option", "alter_table_statement", "opt_using_prefix",
  "using_prefix", "opt_rename_force", "opt_ignore_foreign_key_child",
  "opt_partition", "alter_table_partitioning", "opt_lob_storage",
  "lob_storage_list", "lob_storage_element", "opt_index_storage",
  "index_storage_list", "index_storage_element",
  "opt_index_part_attr_list", "index_part_attr_list", "index_part_attr",
  "partition_spec", "enable_or_disable",
  "alter_table_constraint_statement", "opt_column_tok", "opt_cascade_tok",
  "alter_index_statement", "alter_index_clause", "alter_index_set_clause",
  "on_off_clause", "create_sequence_statement", "opt_sequence_options",
  "opt_sequence_sync_table", "sequence_sync_table",
  "create_index_statement", "opt_index_uniqueness", "opt_index_type",
  "opt_index_pers", "opt_index_partitioning_clause",
  "local_partitioned_index", "on_partitioned_table_list",
  "on_partitioned_table", "opt_index_part_desc", "constr_using_option",
  "opt_index_attributes", "opt_index_attribute_list",
  "opt_index_attribute_element", "create_table_statement", "table_options",
  "opt_row_movement", "table_partitioning_clause", "part_attr_list",
  "part_attr", "part_key_cond_list", "part_key_cond",
  "table_TBS_limit_options", "table_TBS_limit_option",
  "opt_lob_attribute_list", "lob_attribute_list", "lob_attribute_element",
  "lob_storage_attribute_list", "lob_storage_attribute_element",
  "table_element_commalist", "table_element", "table_constraint_def",
  "opt_constraint_name", "column_def_commalist_or_table_constraint_def",
  "column_def_commalist", "column_def", "opt_variable_flag", "opt_in_row",
  "opt_column_constraint_def_list", "column_constraint_def_list",
  "column_constraint", "opt_default_clause", "opt_rule_data_type",
  "rule_data_type", "opt_encryption_attribute", "encryption_attribute",
  "opt_column_commalist", "key_column_with_opt_sort_mode_commalist",
  "expression_with_opt_sort_mode_commalist",
  "column_with_opt_sort_mode_commalist", "column_commalist",
  "references_specification", "opt_reference_spec", "referential_action",
  "table_maxrows", "opt_table_maxrows", "create_queue_statement",
  "create_view_statement", "create_or_replace_view_statement",
  "opt_no_force", "opt_view_column_def", "view_element_commalist",
  "view_element", "opt_with_read_only", "alter_view_statement",
  "drop_view_statement", "create_database_link_statement",
  "drop_database_link_statement", "link_type_clause", "user_clause",
  "alter_database_link_statement", "close_database_link_statement",
  "get_default_statement", "get_condition_statement",
  "alter_queue_statement", "drop_queue_statement", "comment_statement",
  "dml_table_reference", "delete_statement", "insert_statement",
  "multi_insert_value_list", "multi_insert_value", "insert_atom_commalist",
  "insert_atom", "multi_rows_list", "one_row", "update_statement",
  "enqueue_statement", "dequeue_statement", "dequeue_query_term",
  "dequeue_query_spec", "dequeue_from_clause",
  "dequeue_from_table_reference_commalist", "dequeue_from_table_reference",
  "opt_fifo", "assignment_commalist", "assignment", "set_column_def",
  "assignment_column_comma_list", "assignment_column", "merge_statement",
  "merge_actions_list", "merge_actions", "when_condition", "then_action",
  "opt_delete_where_clause", "table_column_commalist",
  "opt_table_column_commalist", "move_statement",
  "opt_move_expression_commalist", "move_expression_commalist",
  "move_expression", "select_or_with_select_statement", "select_statement",
  "with_select_statement", "set_op", "query_exp",
  "opt_subquery_factoring_clause", "subquery_factoring_clause",
  "subquery_factoring_clause_list", "subquery_factoring_element",
  "query_term", "query_spec", "select_or_with_select_statement_4emsql",
  "select_statement_4emsql", "with_select_statement_4emsql",
  "query_exp_4emsql", "subquery_factoring_clause_4emsql",
  "subquery_factoring_clause_list_4emsql",
  "subquery_factoring_element_4emsql", "query_term_4emsql",
  "query_spec_4emsql", "opt_hints", "opt_from", "opt_groupby_clause",
  "opt_quantifier", "target_list", "opt_into_list",
  "select_sublist_commalist", "select_sublist", "opt_as_name",
  "from_clause", "sel_from_table_reference_commalist",
  "sel_from_table_reference", "table_func_argument",
  "opt_pivot_or_unpivot_clause", "pivot_clause", "pivot_aggregation_list",
  "pivot_aggregation", "pivot_for", "pivot_in", "pivot_in_item_list",
  "pivot_in_item", "unpivot_clause", "opt_include_nulls", "unpivot_column",
  "unpivot_colname_list", "unpivot_colname", "unpivot_in",
  "unpivot_in_list", "unpivot_in_info", "unpivot_in_col_info",
  "unpivot_in_col_list", "unpivot_in_column", "unpivot_in_alias_info",
  "unpivot_in_alias_list", "unpivot_in_alias",
  "constant_arithmetic_expression", "constant_concatenation",
  "constant_addition_subtraction", "constant_multiplication_division",
  "constant_plus_minus_prior", "constant_terminal_expression",
  "dump_object_table", "dump_object_list", "dump_object", "joined_table",
  "opt_join_type", "opt_outer", "rollup_cube_clause",
  "rollup_cube_elements", "rollup_cube_element", "grouping_sets_clause",
  "grouping_sets_elements", "empty_group_operator",
  "grouping_sets_element", "group_concatenation",
  "group_concatenation_element", "opt_with_rollup", "opt_having_clause",
  "opt_hierarchical_query_clause", "start_with_clause",
  "opt_start_with_clause", "connect_by_clause", "opt_ignore_loop_clause",
  "opt_order_by_clause", "opt_limit_clause", "limit_clause",
  "limit_values", "opt_limit_or_loop_clause", "loop_clause",
  "opt_for_update_clause", "opt_wait_clause", "opt_time_unit_expression",
  "sort_specification_commalist", "sort_specification", "opt_sort_mode",
  "opt_nulls_mode", "lock_table_statement", "table_lock_mode",
  "opt_until_next_ddl_clause", "opt_where_clause", "expression",
  "logical_or", "logical_and", "logical_not", "condition",
  "equal_operator", "not_equal_operator", "less_than_operator",
  "less_equal_operator", "greater_than_operator", "greater_equal_operator",
  "equal_all_operator", "not_equal_all_operator", "less_than_all_operator",
  "less_equal_all_operator", "greater_than_all_operator",
  "greater_equal_all_operator", "equal_any_operator",
  "not_equal_any_operator", "less_than_any_operator",
  "less_equal_any_operator", "greater_than_any_operator",
  "greater_equal_any_operator", "cursor_identifier",
  "quantified_expression", "arithmetic_expression", "concatenation",
  "addition_subtraction", "multiplication_division", "plus_minus_prior",
  "terminal_expression", "terminal_column", "opt_outer_join_operator",
  "case_expression", "case_when_value_list", "case_when_value",
  "case_then_value", "opt_case_else_clause", "case_when_condition_list",
  "case_when_condition", "unified_invocation", "opt_within_group_clause",
  "within_group_order_by_column_list", "within_group_order_by_column",
  "over_clause", "opt_over_partition_by_clause",
  "partition_by_column_list", "partition_by_column",
  "opt_over_order_by_clause", "opt_window_clause",
  "windowing_start_clause", "windowing_end_clause", "window_value",
  "list_expression", "subquery", "subquery_exp", "subquery_term",
  "opt_keep_clause", "keep_option",
  "SP_create_or_replace_function_statement",
  "SP_create_or_replace_procedure_statement",
  "SP_create_or_replace_typeset_statement",
  "create_or_replace_function_clause",
  "create_or_replace_procedure_clause", "create_or_replace_typeset_clause",
  "SP_as_o_is", "SP_parameter_declaration_commalist_option",
  "SP_parameter_declaration_commalist", "SP_parameter_declaration",
  "SP_parameter_access_mode_option", "SP_name_option",
  "SP_assign_default_value_option", "SP_arithmetic_expression",
  "SP_boolean_expression", "SP_unified_expression",
  "SP_function_opt_arglist", "SP_ident_opt_arglist",
  "SP_variable_name_commalist", "SP_arrayIndex_variable_name",
  "SP_variable_name", "SP_counter_name", "SP_data_type",
  "SP_rule_data_type", "SP_block", "SP_first_block", "SP_typeset_block",
  "SP_item_declaration_list_option", "SP_item_declaration_list",
  "SP_item_declaration", "SP_type_declaration_list",
  "SP_cursor_declaration", "SP_exception_declaration",
  "SP_variable_declaration", "SP_constant_option", "SP_type_declaration",
  "SP_array_element", "SP_opt_index_by_clause", "record_elem_commalist",
  "record_elem", "SP_exception_block_option", "SP_exception_block",
  "SP_exception_handler_list_option", "SP_exception_handler_list",
  "SP_exception_handler", "SP_exception_name_or_list", "SP_exception_name",
  "SP_statement_list", "SP_statement", "SP_label_statement",
  "SP_sql_statement", "SP_invocation_statement", "SP_assignment_statement",
  "SP_fetch_statement", "SP_if_statement", "SP_else_option", "SP_else_if",
  "SP_then_statement", "SP_case_statement", "SP_case_when_condition_list",
  "SP_case_when_condition", "SP_case_when_value_list",
  "SP_case_when_value", "SP_case_right_operand", "SP_case_else_option",
  "SP_loop_statement", "SP_common_loop", "SP_while_loop_statement",
  "SP_basic_loop_statement", "SP_for_loop_statement", "SP_step_option",
  "SP_cursor_for_loop_statement", "SP_close_statement",
  "SP_exit_statement", "SP_exit_when_option", "SP_goto_statement",
  "SP_null_statement", "SP_open_statement", "SP_raise_statement",
  "SP_return_statement", "SP_continue_statement",
  "SP_alter_procedure_statement", "SP_alter_function_statement",
  "SP_drop_procedure_statement", "SP_drop_function_statement",
  "SP_drop_typeset_statement", "exec_proc_stmt", "exec_func_stmt",
  "SP_exec_or_execute", "SP_ident_opt_simple_arglist",
  "assign_return_value", "set_statement", "initsize_spec",
  "create_database_statement", "archivelog_option", "character_set_option",
  "db_character_set", "national_character_set", "alter_database_statement",
  "until_option", "usinganchor_option", "filespec_option",
  "alter_database_options", "alter_database_options2",
  "drop_database_statement", "create_tablespace_statement",
  "create_temp_tablespace_statement", "alter_tablespace_dcl_statement",
  "backupTBS_option", "alter_tablespace_ddl_statement",
  "drop_tablespace_statement", "datafile_spec", "filespec",
  "autoextend_clause", "autoextend_statement", "maxsize_clause",
  "opt_createTBS_options", "tablespace_option", "opt_extentsize_option",
  "extentsize_clause", "segment_management_clause", "database_size_option",
  "size_option", "opt_alterTBS_onoff_options", "online_offline_option",
  "filename_list", "filename", "opt_droptablespace_options",
  "create_trigger_statement", "create_or_replace_trigger_clause",
  "alter_trigger_statement", "drop_trigger_statement",
  "trigger_event_clause", "trigger_event_type_list",
  "trigger_event_time_clause", "trigger_event_columns",
  "trigger_referencing_clause", "trigger_referencing_list",
  "trigger_referencing_item", "trigger_old_or_new", "trigger_row_or_table",
  "trigger_as_or_none", "trigger_referencing_name",
  "trigger_action_information", "trigger_when_condition",
  "trigger_action_clause", "alter_trigger_option",
  "create_or_replace_directory_statement",
  "create_or_replace_directory_clause", "drop_directory_statement",
  "create_materialized_view_statement", "opt_mview_build_refresh",
  "mview_refresh_time", "alter_materialized_view_statement",
  "mview_refresh_alter", "drop_materialized_view_statement",
  "create_job_statement", "opt_end_statement", "opt_interval_statement",
  "interval_statement", "opt_enable_statement", "enable_statement",
  "opt_job_comment_statement", "job_comment_statement",
  "alter_job_statement", "drop_job_statement", "object_name",
  "in_host_var_list", "host_var_list", "host_variable",
  "free_lob_loc_list", "opt_into_host_var", "out_host_var_list",
  "opt_into_ses_host_var_4emsql", "out_1hostvariable_4emsql",
  "out_host_var_list_4emsql", "out_psm_host_var", "file_option",
  "option_keyword_opt", "indicator_opt", "indicator",
  "indicator_keyword_opt", "tablespace_name_option", "opt_table_part_desc",
  "opt_record_access", "record_access", "opt_partition_lob_attr_list",
  "partition_lob_attr_list", "partition_lob_attr", "opt_partition_name",
  "SES_V_IN", 0
};
#endif

# ifdef YYPRINT
/* YYTOKNUM[YYLEX-NUM] -- Internal token number corresponding to
   token YYLEX-NUM.  */
static const yytype_uint16 yytoknum[] =
{
       0,   256,   257,   258,   259,   260,   261,   262,   263,   264,
     265,   266,   267,   268,   269,   270,   271,   272,   273,   274,
     275,   276,   277,   278,   279,   280,   281,   282,   283,   284,
     285,   286,   287,   288,   289,   290,   291,   292,   293,   294,
     295,   296,   297,   298,   299,   300,   301,   302,   303,   304,
     305,   306,   307,   308,   309,   310,   311,   312,   313,   314,
     315,   316,   317,   318,   319,   320,   321,   322,   323,   324,
     325,   326,   327,   328,   329,   330,   331,   332,   333,   334,
     335,   336,   337,   338,   339,   340,   341,   342,   343,   344,
     345,   346,   347,   348,   349,   350,   351,   352,   353,   354,
     355,   356,   357,   358,   359,   360,   361,   362,   363,   364,
     365,   366,   367,   368,   369,   370,   371,   372,   373,   374,
     375,   376,   377,   378,   379,   380,   381,   382,   383,   384,
     385,   386,   387,   388,   389,   390,   391,   392,   393,   394,
     395,   396,   397,   398,   399,   400,   401,   402,   403,   404,
     405,   406,   407,   408,   409,   410,   411,   412,   413,   414,
     415,   416,   417,   418,   419,   420,   421,   422,   423,   424,
     425,   426,   427,   428,   429,   430,   431,   432,   433,   434,
     435,   436,   437,   438,   439,   440,   441,   442,   443,   444,
     445,   446,   447,   448,   449,   450,   451,   452,   453,   454,
     455,   456,   457,   458,   459,   460,   461,   462,   463,   464,
     465,   466,   467,   468,   469,   470,   471,   472,   473,   474,
     475,   476,   477,   478,   479,   480,   481,   482,   483,   484,
     485,   486,   487,   488,   489,   490,   491,   492,   493,   494,
     495,   496,   497,   498,   499,   500,   501,   502,   503,   504,
     505,   506,   507,   508,   509,   510,   511,   512,   513,   514,
     515,   516,   517,   518,   519,   520,   521,   522,   523,   524,
     525,   526,   527,   528,   529,   530,   531,   532,   533,   534,
     535,   536,   537,   538,   539,   540,   541,   542,   543,   544,
     545,   546,   547,   548,   549,   550,   551,   552,   553,   554,
     555,   556,   557,   558,   559,   560,   561,   562,   563,   564,
     565,   566,   567,   568,   569,   570,   571,   572,   573,   574,
     575,   576,   577,   578,   579,   580,   581,   582,   583,   584,
     585,   586,   587,   588,   589,   590,   591,   592,   593,   594,
     595,   596,   597,   598,   599,   600,   601,   602,   603,   604,
     605,   606,   607,   608,   609,   610,   611,   612,   613,   614,
     615,   616,   617,   618,   619,   620,   621,   622,   623,   624,
     625,   626,   627,   628,   629,   630,   631,   632,   633,   634,
     635,   636,   637,   638,   639,   640,   641,   642,   643,   644,
     645,   646,   647,   648,   649,   650,   651,   652,   653,   654,
     655,   656,   657,   658,   659,   660,   661,    40,    41,    91,
      93,    46,    44,    38,    42,    43,    45,   126,    33,    47,
      37,    60,    62,    94,   124,    63,    58,    61,    59,   125,
     123
};
# endif

/* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_uint16 yyr1[] =
{
       0,   431,   432,   432,   433,   433,   433,   433,   434,   434,
     435,   435,   435,   435,   436,   436,   436,   436,   436,   436,
     436,   436,   437,   437,   438,   438,   438,   438,   438,   438,
     439,   439,   439,   439,   439,   439,   440,   440,   441,   441,
     441,   441,   442,   442,   442,   443,   443,   443,   444,   444,
     444,   444,   444,   445,   445,   445,   446,   446,   447,   447,
     448,   448,   449,   449,   450,   450,   451,   451,   452,   452,
     453,   453,   453,   453,   453,   453,   453,   453,   453,   453,
     453,   454,   454,   455,   456,   456,   457,   457,   457,   457,
     458,   458,   459,   460,   460,   461,   461,   461,   461,   461,
     462,   462,   462,   462,   462,   462,   462,   462,   462,   462,
     462,   462,   462,   462,   462,   462,   462,   462,   462,   462,
     462,   462,   462,   462,   462,   462,   462,   462,   462,   462,
     462,   462,   462,   462,   462,   463,   463,   464,   464,   464,
     464,   464,   465,   466,   467,   467,   468,   468,   469,   469,
     470,   471,   471,   471,   472,   473,   473,   473,   474,   474,
     474,   475,   475,   476,   476,   477,   477,   478,   478,   478,
     478,   478,   478,   478,   479,   480,   481,   481,   481,   481,
     482,   482,   483,   483,   484,   484,   485,   485,   486,   486,
     487,   487,   488,   488,   489,   489,   489,   490,   490,   490,
     490,   490,   490,   490,   490,   490,   491,   491,   491,   492,
     492,   493,   493,   493,   493,   493,   493,   494,   494,   494,
     494,   494,   494,   495,   495,   496,   496,   496,   497,   497,
     498,   499,   499,   500,   500,   501,   501,   501,   502,   502,
     502,   502,   502,   502,   502,   502,   502,   502,   503,   503,
     503,   503,   503,   504,   504,   505,   505,   506,   507,   507,
     508,   508,   508,   508,   508,   508,   508,   508,   508,   509,
     509,   510,   510,   511,   512,   513,   514,   515,   516,   517,
     518,   518,   518,   518,   518,   518,   518,   518,   518,   519,
     519,   519,   520,   520,   520,   520,   520,   520,   520,   520,
     520,   521,   521,   522,   523,   523,   523,   523,   524,   524,
     525,   525,   525,   525,   526,   526,   526,   526,   526,   527,
     527,   528,   528,   529,   530,   530,   530,   531,   531,   531,
     532,   532,   532,   532,   533,   533,   533,   533,   533,   533,
     533,   534,   534,   534,   535,   535,   536,   536,   537,   537,
     537,   537,   537,   537,   538,   539,   539,   540,   540,   540,
     540,   540,   540,   540,   540,   540,   541,   541,   542,   542,
     543,   543,   544,   544,   544,   545,   546,   547,   548,   548,
     548,   549,   550,   550,   550,   550,   551,   552,   553,   554,
     554,   554,   554,   554,   555,   556,   556,   556,   556,   556,
     556,   556,   556,   557,   557,   558,   558,   558,   558,   558,
     558,   558,   558,   558,   558,   558,   558,   559,   559,   560,
     560,   561,   561,   561,   561,   562,   562,   562,   562,   562,
     562,   562,   562,   562,   562,   562,   562,   562,   562,   562,
     562,   562,   562,   562,   562,   562,   562,   562,   562,   562,
     562,   562,   562,   562,   562,   562,   562,   562,   562,   562,
     562,   562,   562,   562,   562,   562,   562,   562,   562,   562,
     562,   562,   562,   562,   562,   562,   562,   562,   562,   562,
     562,   562,   562,   562,   562,   562,   562,   562,   562,   562,
     562,   562,   562,   562,   562,   562,   562,   563,   563,   563,
     563,   563,   563,   563,   563,   563,   563,   563,   564,   564,
     564,   565,   566,   566,   566,   566,   567,   567,   568,   568,
     569,   569,   569,   569,   569,   569,   569,   569,   569,   569,
     569,   569,   569,   569,   569,   569,   569,   569,   569,   569,
     569,   569,   569,   569,   569,   569,   569,   569,   569,   569,
     569,   569,   569,   569,   569,   569,   569,   569,   569,   569,
     569,   569,   569,   569,   569,   569,   569,   569,   569,   569,
     569,   569,   569,   569,   570,   570,   570,   570,   570,   570,
     570,   571,   571,   572,   572,   572,   572,   572,   572,   572,
     572,   572,   572,   573,   573,   573,   573,   573,   573,   573,
     573,   573,   574,   574,   575,   575,   576,   577,   578,   578,
     579,   579,   580,   580,   581,   582,   583,   583,   583,   583,
     583,   584,   584,   585,   585,   586,   586,   587,   588,   588,
     589,   589,   589,   589,   590,   590,   590,   590,   590,   591,
     591,   592,   592,   593,   594,   595,   596,   597,   597,   598,
     599,   600,   601,   601,   602,   603,   603,   604,   604,   605,
     605,   605,   605,   605,   605,   605,   605,   605,   605,   605,
     605,   605,   605,   605,   605,   605,   605,   605,   605,   605,
     605,   605,   605,   605,   605,   605,   605,   605,   605,   605,
     605,   605,   605,   605,   605,   605,   605,   605,   605,   605,
     605,   605,   605,   605,   605,   605,   605,   605,   605,   605,
     605,   605,   605,   605,   605,   605,   605,   605,   605,   605,
     605,   605,   605,   605,   605,   605,   605,   606,   606,   607,
     607,   608,   608,   609,   609,   610,   611,   611,   612,   612,
     613,   613,   614,   614,   615,   615,   616,   616,   616,   616,
     616,   616,   616,   616,   616,   616,   616,   616,   616,   616,
     616,   616,   616,   617,   617,   618,   618,   619,   619,   619,
     619,   620,   620,   621,   622,   622,   623,   623,   624,   624,
     625,   626,   626,   626,   627,   627,   627,   627,   628,   628,
     629,   629,   630,   630,   630,   631,   631,   631,   631,   632,
     632,   632,   632,   633,   633,   634,   634,   635,   635,   636,
     636,   636,   637,   638,   639,   640,   640,   641,   642,   643,
     644,   645,   646,   646,   647,   648,   649,   649,   650,   650,
     650,   651,   651,   652,   652,   652,   653,   653,   654,   654,
     655,   655,   655,   655,   655,   655,   655,   655,   655,   656,
     656,   656,   656,   656,   656,   656,   656,   656,   656,   656,
     656,   656,   656,   656,   656,   656,   656,   656,   657,   657,
     658,   659,   659,   660,   660,   661,   661,   662,   662,   662,
     662,   662,   662,   662,   662,   662,   662,   662,   662,   663,
     663,   664,   664,   665,   666,   666,   667,   667,   668,   669,
     669,   670,   670,   671,   671,   672,   673,   673,   674,   674,
     674,   674,   675,   675,   676,   676,   677,   677,   678,   678,
     678,   678,   678,   679,   680,   680,   680,   680,   681,   682,
     682,   683,   683,   684,   685,   686,   686,   686,   687,   687,
     688,   688,   688,   689,   689,   690,   690,   691,   691,   692,
     693,   693,   694,   694,   695,   695,   696,   696,   697,   697,
     697,   698,   698,   698,   698,   699,   699,   699,   699,   699,
     699,   699,   700,   700,   700,   701,   702,   702,   703,   703,
     703,   703,   704,   704,   705,   706,   706,   707,   707,   708,
     708,   709,   709,   710,   710,   711,   711,   712,   712,   713,
     713,   714,   714,   715,   715,   715,   715,   715,   715,   716,
     716,   717,   717,   718,   718,   719,   720,   720,   720,   721,
     721,   722,   722,   723,   723,   724,   724,   724,   724,   724,
     724,   724,   725,   725,   726,   726,   727,   727,   727,   727,
     727,   728,   728,   729,   730,   730,   731,   732,   732,   733,
     733,   734,   734,   735,   736,   736,   736,   736,   736,   737,
     737,   738,   739,   739,   740,   740,   741,   742,   743,   743,
     743,   744,   744,   745,   745,   746,   747,   747,   748,   749,
     750,   750,   750,   750,   751,   751,   752,   752,   753,   754,
     754,   754,   755,   755,   756,   757,   758,   758,   759,   760,
     760,   761,   761,   762,   763,   763,   763,   763,   763,   764,
     764,   765,   766,   766,   767,   767,   768,   768,   769,   770,
     771,   772,   773,   774,   775,   776,   777,   777,   778,   778,
     778,   779,   779,   780,   780,   780,   781,   781,   782,   782,
     783,   783,   784,   785,   785,   786,   787,   787,   787,   788,
     788,   789,   789,   790,   790,   791,   791,   792,   793,   793,
     794,   794,   795,   795,   796,   796,   797,   798,   799,   799,
     799,   799,   800,   800,   801,   801,   802,   803,   803,   804,
     805,   805,   806,   806,   807,   807,   808,   809,   810,   810,
     811,   812,   812,   813,   814,   814,   815,   816,   816,   817,
     817,   818,   818,   819,   819,   819,   820,   820,   821,   821,
     822,   822,   823,   823,   823,   824,   824,   824,   824,   824,
     825,   826,   826,   827,   827,   827,   827,   827,   827,   827,
     827,   827,   828,   828,   828,   828,   828,   829,   829,   829,
     830,   831,   831,   832,   832,   833,   833,   834,   835,   835,
     836,   837,   838,   838,   839,   839,   840,   840,   841,   842,
     843,   843,   844,   844,   845,   845,   846,   846,   847,   848,
     848,   849,   849,   850,   851,   852,   852,   853,   853,   853,
     854,   854,   854,   855,   855,   855,   856,   856,   856,   856,
     856,   856,   856,   857,   858,   858,   859,   859,   860,   861,
     861,   861,   861,   861,   862,   862,   863,   863,   864,   864,
     865,   866,   867,   867,   867,   867,   868,   869,   869,   870,
     870,   871,   871,   871,   872,   872,   873,   873,   874,   874,
     874,   875,   876,   876,   877,   877,   878,   878,   879,   879,
     879,   880,   880,   881,   882,   882,   882,   883,   883,   883,
     884,   885,   885,   886,   886,   886,   887,   887,   887,   887,
     887,   887,   887,   888,   888,   889,   890,   890,   890,   891,
     891,   891,   892,   892,   892,   892,   893,   893,   893,   893,
     894,   894,   895,   895,   896,   897,   897,   898,   898,   899,
     899,   900,   900,   900,   900,   900,   900,   900,   900,   900,
     900,   900,   900,   900,   900,   900,   900,   900,   900,   900,
     900,   900,   900,   900,   900,   900,   900,   900,   900,   900,
     900,   900,   901,   902,   902,   903,   904,   905,   906,   907,
     908,   908,   908,   909,   910,   911,   912,   913,   913,   913,
     914,   914,   914,   914,   915,   915,   916,   916,   917,   917,
     918,   918,   919,   919,   920,   920,   920,   920,   920,   920,
     920,   921,   922,   922,   923,   923,   923,   924,   924,   924,
     925,   925,   925,   925,   926,   926,   926,   926,   926,   926,
     926,   926,   926,   926,   926,   926,   926,   926,   926,   926,
     926,   926,   926,   926,   926,   926,   926,   927,   927,   927,
     928,   928,   929,   929,   930,   930,   931,   932,   933,   933,
     934,   934,   935,   936,   936,   936,   936,   936,   936,   936,
     936,   936,   936,   936,   936,   936,   936,   937,   937,   938,
     938,   939,   939,   939,   940,   940,   941,   941,   942,   942,
     943,   944,   944,   945,   945,   945,   946,   946,   946,   946,
     947,   947,   947,   947,   948,   948,   949,   949,   950,   951,
     951,   952,   953,   953,   954,   954,   955,   956,   957,   958,
     958,   959,   959,   960,   960,   961,   961,   962,   962,   962,
     963,   963,   964,   965,   965,   965,   965,   966,   966,   967,
     967,   967,   968,   969,   970,   971,   971,   971,   972,   972,
     972,   973,   973,   974,   974,   974,   974,   975,   975,   975,
     975,   976,   977,   977,   977,   977,   977,   977,   977,   977,
     977,   978,   978,   978,   978,   979,   979,   980,   981,   982,
     982,   983,   983,   984,   984,   984,   984,   985,   985,   986,
     987,   988,   989,   989,   990,   990,   991,   991,   991,   991,
     992,   993,   993,   994,   995,   995,   996,   997,   997,   998,
     998,   999,   999,  1000,  1000,  1001,  1001,  1002,  1002,  1003,
    1003,  1003,  1003,  1003,  1003,  1003,  1003,  1003,  1003,  1003,
    1003,  1003,  1003,  1003,  1003,  1004,  1005,  1005,  1005,  1005,
    1005,  1005,  1005,  1005,  1005,  1005,  1005,  1005,  1006,  1006,
    1007,  1007,  1008,  1008,  1009,  1010,  1010,  1010,  1011,  1011,
    1012,  1013,  1013,  1014,  1014,  1015,  1016,  1016,  1017,  1018,
    1019,  1019,  1020,  1020,  1020,  1020,  1021,  1022,  1023,  1024,
    1024,  1025,  1025,  1026,  1027,  1027,  1028,  1029,  1029,  1030,
    1031,  1032,  1032,  1033,  1033,  1034,  1034,  1035,  1036,  1037,
    1038,  1039,  1040,  1041,  1042,  1043,  1043,  1044,  1044,  1044,
    1045,  1046,  1046,  1046,  1047,  1048,  1049,  1049,  1050,  1050,
    1051,  1052,  1053,  1053,  1053,  1053,  1053,  1053,  1053,  1053,
    1053,  1053,  1053,  1054,  1054,  1054,  1055,  1055,  1056,  1056,
    1057,  1057,  1057,  1057,  1058,  1058,  1058,  1059,  1060,  1060,
    1060,  1061,  1062,  1062,  1062,  1062,  1063,  1063,  1064,  1064,
    1064,  1064,  1065,  1066,  1066,  1067,  1067,  1067,  1067,  1067,
    1068,  1068,  1069,  1069,  1069,  1069,  1069,  1069,  1069,  1070,
    1070,  1071,  1071,  1071,  1072,  1072,  1073,  1073,  1074,  1075,
    1076,  1076,  1077,  1077,  1078,  1078,  1079,  1079,  1080,  1080,
    1081,  1082,  1082,  1082,  1083,  1084,  1084,  1085,  1086,  1087,
    1088,  1088,  1088,  1088,  1088,  1088,  1089,  1089,  1090,  1090,
    1091,  1091,  1092,  1092,  1093,  1094,  1094,  1095,  1095,  1095,
    1096,  1096,  1097,  1098,  1098,  1098,  1099,  1099,  1100,  1101,
    1101,  1101,  1102,  1103,  1103,  1104,  1105,  1106,  1106,  1106,
    1106,  1106,  1106,  1106,  1107,  1107,  1108,  1109,  1109,  1109,
    1110,  1111,  1112,  1112,  1113,  1113,  1114,  1115,  1115,  1116,
    1116,  1117,  1117,  1118,  1119,  1119,  1119,  1119,  1119,  1119,
    1120,  1121,  1121,  1122,  1122,  1122,  1122,  1122,  1122,  1123,
    1123,  1124,  1124,  1124,  1124,  1124,  1124,  1125,  1125,  1126,
    1126,  1127,  1127,  1127,  1127,  1127,  1127,  1128,  1128,  1129,
    1129,  1130,  1130,  1130,  1130,  1130,  1130,  1131,  1132,  1133,
    1133,  1134,  1134,  1135,  1136,  1136,  1137,  1137,  1138,  1139,
    1139,  1140,  1140,  1140,  1141,  1141,  1142,  1142,  1143,  1144,
    1144,  1145,  1145
};

/* YYR2[YYN] -- Number of symbols composing right hand side of rule YYN.  */
static const yytype_uint8 yyr2[] =
{
       0,     2,     1,     2,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     3,     1,     4,     3,     4,     3,     3,
       2,     2,     1,     3,     1,     2,     2,     2,     2,     4,
       1,     1,     1,     1,     1,     1,     1,     4,     1,     3,
       3,     3,     1,     3,     3,     1,     3,     3,     1,     3,
       3,     3,     3,     1,     3,     3,     1,     3,     1,     3,
       1,     3,     1,     3,     1,     3,     1,     5,     1,     3,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     3,     1,     2,     3,     1,     2,     1,     2,
       1,     3,     1,     1,     3,     1,     1,     1,     1,     1,
       1,     1,     2,     2,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     0,     1,     4,     5,     4,
       5,     2,     2,     1,     1,     1,     1,     2,     1,     1,
       3,     0,     1,     3,     1,     1,     2,     3,     4,     5,
       2,     1,     3,     1,     3,     1,     2,     1,     3,     3,
       4,     3,     4,     4,     1,     1,     1,     2,     2,     3,
       1,     2,     1,     3,     1,     3,     1,     3,     1,     3,
       2,     1,     1,     2,     1,     1,     2,     3,     2,     3,
       3,     4,     2,     3,     3,     4,     1,     3,     4,     1,
       3,     1,     1,     1,     1,     1,     1,     3,     3,     4,
       3,     4,     3,     2,     3,     1,     1,     1,     1,     2,
       1,     1,     2,     1,     2,     5,     7,     5,     5,     7,
       6,     7,     7,     8,     7,     8,     8,     9,     3,     2,
       2,     2,     3,     2,     3,     1,     2,     1,     1,     2,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     4,
       4,     2,     2,     2,     1,     1,     2,     2,     1,     1,
       4,     4,     4,     5,     4,     3,     3,     3,     2,     0,
       2,     2,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     3,     3,     7,     0,     1,     1,     1,     0,     1,
       0,     2,     2,     4,     0,     3,     3,     3,     4,     0,
       2,     1,     3,     3,     2,     4,     4,     5,     6,     6,
       0,     1,     1,     2,     1,     1,     1,     1,     1,     2,
       2,     1,     2,     2,     3,     2,     2,     2,     5,     7,
       7,     9,    11,     9,     1,     4,     3,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     4,     4,     0,     3,
       3,     3,     2,     2,     2,     3,     3,     2,     2,     2,
       2,     2,     2,     5,     4,     4,     7,     3,     7,     2,
       2,     2,     6,     6,     1,     1,     2,     2,     6,     6,
       6,     3,     2,     6,     6,     3,     4,     4,     4,     5,
       3,     4,     5,     5,     5,     6,     4,     0,     1,     6,
       4,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     2,     1,     2,
       1,     2,     1,     1,     1,     3,     3,     1,     1,     1,
       2,     4,     2,     2,     3,     3,     1,     2,     1,     2,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     6,     6,     6,     6,     7,     7,     7,
       6,     7,     6,     3,     4,     5,     6,     7,     6,     6,
       5,     7,     0,     1,     1,     1,     2,     2,     2,     5,
       0,     1,     3,     5,     5,     5,     2,     2,     4,     4,
       3,     1,     3,     3,     5,     6,     7,     4,     2,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     2,
       2,     1,     2,     3,     3,     3,     3,     1,     1,     4,
       3,     3,     1,     1,     4,     7,     8,     3,     1,     2,
       3,     3,     3,     2,     3,     3,     3,     3,     2,     3,
       3,     3,     2,     3,     3,     3,     2,     3,     3,     3,
       3,     2,     2,     2,     3,     3,     3,     3,     3,     3,
       3,     3,     2,     2,     2,     2,     3,     3,     3,     2,
       2,     2,     3,     3,     3,     4,     4,     4,     2,     3,
       3,     3,     3,     3,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     2,     1,     1,     1,     3,     1,     1,
       1,     0,     3,     1,     1,     4,     8,     9,     0,     2,
       0,     1,     5,     6,     3,     4,    10,     6,     6,     6,
       6,     6,     6,     6,     6,     8,     6,     3,     5,     6,
       5,     6,     5,     0,     2,     2,     1,     1,     2,     2,
       2,     3,     1,     1,     0,     1,     1,     1,     2,     1,
       4,     0,     2,     3,     0,     2,     2,     3,     0,     2,
       3,     1,     8,    12,     1,     0,     2,     1,     3,     3,
       1,     1,     2,     0,     2,     3,     1,     3,     5,     0,
       1,     5,     3,     4,     7,     0,     3,     3,     3,     4,
       3,     6,     7,     5,     7,     9,     3,     1,    11,     9,
       7,     0,     1,     0,     1,     2,     4,     4,     2,     1,
       3,     4,     3,     4,     2,     3,     1,     1,     1,    10,
      11,    10,    10,     9,    15,    12,     6,     6,     5,     5,
       6,     8,     5,     4,     8,     9,    10,     8,     0,     1,
       3,     0,     2,     0,     4,     0,     2,     3,     2,     3,
       8,     5,     4,    15,    15,     3,     3,     3,     7,     0,
       4,     3,     1,     3,     0,     4,     3,     1,     3,     0,
       4,     3,     1,     3,     5,     3,     1,     1,     5,     6,
       7,     9,     0,     1,     0,     1,     4,     5,     1,     1,
       3,     3,     1,     2,     1,     1,     2,     2,     5,     0,
       1,     0,     1,     3,    11,     2,     3,     4,     0,     3,
       0,     4,     4,     0,     1,     1,     4,     3,     1,     5,
       0,     2,     0,     3,     0,     1,     2,     1,     2,     2,
       1,     8,    10,     7,     8,     0,     1,     1,     2,     2,
       3,     1,     0,     3,     3,     9,     3,     1,     9,     3,
       7,     5,     3,     1,     1,     2,     1,     2,     4,     0,
       1,     2,     1,     9,     6,     2,     1,     2,     1,     3,
       1,     1,     1,     5,     6,     7,     6,     6,     4,     0,
       2,     1,     1,     3,     1,     6,     0,     1,     1,     0,
       3,     0,     1,     2,     1,     2,     3,     4,     5,     6,
       2,     6,     0,     2,     0,     2,     1,     4,     6,     7,
       7,     0,     1,     3,     0,     3,     3,     4,     2,     4,
       2,     3,     1,     4,     0,     3,     3,     3,     3,     2,
       1,     2,     0,     1,    10,     8,     8,    10,     0,     2,
       1,     0,     3,     3,     1,     1,     0,     3,     4,     3,
       6,     7,     7,     8,     4,     5,     1,     1,     6,     4,
       4,     5,     6,     6,     2,     2,     4,     5,     3,     6,
       6,     2,     3,     7,     7,     8,     6,     9,     5,     2,
       1,     7,     3,     1,     1,     1,     3,     1,     3,     8,
       9,     1,     1,     6,     2,     1,     3,     1,     0,     1,
       1,     3,     1,     3,     3,     5,     1,     3,     3,     1,
       1,     3,    11,     2,     1,     4,     1,     2,     2,     6,
       7,     0,     3,     3,     1,     0,     3,    11,     0,     3,
       3,     1,     1,     1,     1,     1,     3,     4,     1,     2,
       1,     1,     3,     1,     0,     2,     2,     3,     1,     6,
       3,     1,    10,    10,     1,     1,     3,     4,     3,     1,
       2,     3,     1,     6,     3,     1,    10,     0,     1,     0,
       1,     0,     3,     0,     1,     1,     1,     1,     0,     2,
       3,     1,     3,     5,     2,     0,     1,     2,     1,     2,
       2,     3,     1,     6,     4,     5,     7,     5,     4,     1,
       5,     1,     1,     4,     1,     5,     3,     0,     1,     1,
       6,     3,     1,     5,     5,     4,     2,     4,     3,     1,
       2,     8,     0,     2,     3,     1,     3,     1,     1,     4,
       3,     1,     3,     1,     3,     1,     3,     1,     1,     3,
       1,     3,     1,     1,     1,     3,     1,     3,     3,     1,
       3,     3,     1,     2,     2,     1,     1,     1,     1,     1,
       1,     1,     1,     5,     3,     1,     2,     4,     6,     0,
       1,     2,     2,     2,     0,     1,     4,     4,     3,     1,
       1,     4,     3,     1,     3,     1,     2,     1,     1,     3,
       1,     1,     1,     2,     0,     2,     0,     2,     0,     3,
       3,     3,     0,     1,     3,     2,     0,     2,     0,     3,
       4,     0,     1,     2,     3,     3,     1,     0,     1,     1,
       2,     0,     3,     0,     1,     3,     0,     1,     1,     1,
       1,     1,     1,     3,     1,     3,     0,     1,     1,     0,
       2,     2,     8,    10,     9,    11,     2,     2,     3,     1,
       0,     3,     0,     2,     1,     3,     1,     3,     1,     2,
       1,     5,     6,     3,     4,     5,     6,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     4,     2,     2,     3,
       3,     1,     1,     2,     2,     1,     2,     1,     2,     2,
       3,     3,     2,     2,     3,     2,     3,     2,     2,     1,
       3,     3,     3,     3,     2,     2,     3,     3,     2,     2,
       3,     3,     1,     3,     3,     1,     5,     3,     1,     1,
       1,     1,     3,     1,     3,     3,     1,     3,     3,     1,
       2,     2,     2,     1,     1,     1,     1,     1,     1,     3,
       1,     1,     1,     1,     1,     1,     8,     6,     4,     2,
       3,     1,     1,     3,     1,     1,     1,     5,     3,     1,
       0,     1,     5,     4,     2,     1,     3,     2,     0,     2,
       2,     1,     3,     7,     6,     4,     5,     6,     7,     7,
       6,     8,     7,     4,     3,     6,     2,     0,     7,     3,
       1,     1,     2,     2,     0,     6,     0,     3,     3,     1,
       1,     0,     3,     0,     5,     2,     2,     1,     2,     2,
       2,     1,     2,     2,     1,     3,     3,     1,     5,     3,
       1,     1,     0,     8,     1,     1,     8,     6,     5,     2,
       4,     2,     4,     2,     4,     1,     1,     0,     2,     3,
       3,     1,     4,     0,     1,     1,     2,     0,     1,     0,
       3,     2,     1,     1,     1,     1,     3,     1,     1,     3,
       4,     3,     1,     4,     6,     6,     8,     1,     3,     5,
       1,     1,     3,     5,     3,     5,     7,     1,     1,     3,
       5,     4,     6,     7,     7,     8,     6,     5,     2,     0,
       1,     2,     1,     1,     1,     1,     1,     2,     1,     7,
       3,     5,     0,     1,     8,     8,     1,     1,     3,     5,
       3,     3,     1,     2,     0,     1,     2,     0,     1,     2,
       1,     3,     3,     3,     1,     1,     3,     2,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     5,     3,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     1,     3,
       5,     5,     5,     7,     7,     0,     2,     1,     4,     4,
       2,     6,     7,     2,     1,     3,     2,     1,     3,     1,
       0,     2,     1,     1,     1,     1,     4,     5,     3,    10,
      11,     0,     2,     7,     3,     5,     4,     0,     2,     3,
       2,     3,     5,     3,     2,     2,     3,     2,     4,     4,
       3,     3,     3,     1,     2,     1,     1,     1,     3,     5,
       3,     4,     4,     4,     3,     6,     1,     1,     2,     2,
       3,     4,     4,     4,     3,     6,     7,     6,     6,     7,
       6,     4,     4,     0,     2,     3,     0,     3,     0,     2,
       1,     2,     2,     2,     3,     3,     3,     3,     6,     7,
       8,     7,     4,     8,     7,     4,     2,     2,     6,     6,
       7,     8,     4,     3,     1,     2,     4,     5,     3,     6,
       0,     1,     2,     2,     4,     4,     5,     6,     4,     2,
       2,     0,     1,     2,     1,     1,     0,     1,     2,     3,
       1,     2,     1,     2,     1,     1,     1,     1,     3,     1,
       1,     0,     3,     5,     8,     2,     4,     4,     3,     2,
       3,     3,     4,     1,     1,     2,     1,     1,     0,     2,
       0,     2,     3,     1,     4,     1,     1,     0,     1,     1,
       0,     1,     1,     0,     4,     3,     0,     4,     2,     1,
       1,     1,     4,     2,     4,     3,    10,     0,     2,     2,
       3,     4,     4,     5,     2,     2,     5,     2,     2,     3,
       4,    11,     0,     2,     0,     1,     3,     0,     1,     1,
       1,     0,     1,     2,     6,     6,     6,     5,     5,     5,
       3,     1,     1,     2,     4,     4,     4,     6,     6,     1,
       3,     3,     3,     4,     2,     4,     4,     3,     1,     0,
       2,     2,     4,     4,     4,     6,     6,     0,     2,     1,
       1,     2,     4,     4,     4,     6,     6,     2,     2,     0,
       1,     0,     1,     2,     0,     1,     0,     2,     3,     0,
       1,     2,     2,     2,     0,     3,     3,     1,     3,     0,
       4,     1,     1
};

/* YYDEFACT[STATE-NAME] -- Default rule to reduce with in state
   STATE-NUM when YYTABLE doesn't specify something else to do.  Zero
   means the default is an error.  */
static const yytype_uint16 yydefact[] =
{
       0,     7,    98,   100,   101,   111,   110,     0,    96,   109,
     105,   106,    99,   104,   107,    97,   144,    95,   145,   108,
     113,   112,   257,   116,   117,   118,   119,   120,   121,   122,
     123,   124,   125,   126,   127,   128,   129,   130,   131,   132,
     133,   134,     0,     0,     0,   274,   275,   278,   279,     0,
       0,   289,     0,   176,     0,     2,     4,     9,     0,    86,
      88,   114,     0,   115,     0,   165,     0,     8,   167,     6,
     260,   261,   262,   265,   266,   263,   264,   267,   268,     5,
     102,   103,     0,   160,     0,     0,   272,   271,   273,   276,
     277,     0,     0,     0,     0,   418,     0,     0,     0,     0,
       0,   288,     0,   180,   178,   177,     1,     3,    84,     0,
      90,    93,    87,    89,   143,     0,     0,   141,   230,   231,
       0,   255,     0,     0,   253,   175,   174,     0,     0,   166,
       0,   161,   163,     0,     0,     0,   291,   290,     0,     0,
       0,     0,     0,     0,     0,     0,   610,     0,   774,     0,
       0,  1197,  1197,   354,     0,     0,  1197,     0,   330,     0,
       0,  1197,  1197,  1197,     0,     0,     0,     0,   610,     0,
    1197,     0,     0,  1197,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   395,     0,
       0,     0,   292,     0,   293,   294,   295,   296,   297,   298,
     299,   300,     0,   357,   358,   359,     0,   360,   361,     0,
     362,   363,   364,   365,     0,     0,     0,   421,   422,     0,
     509,   508,   423,   424,   425,   426,   516,   428,   518,   429,
     430,   431,   432,   433,   458,   496,   495,   434,   652,   653,
     435,   733,   734,   483,   484,   436,   446,   442,   443,   457,
     456,   454,   455,   444,   445,   451,   440,   441,   452,   448,
     447,     0,   437,   485,   449,   450,   453,   459,   460,   461,
     462,   463,   438,   439,   486,   487,   491,   498,   500,   502,
     507,  1128,  1121,  1122,   503,   504,  1351,  1184,  1185,  1338,
       0,  1189,  1195,   466,     0,     0,     0,     0,     0,     0,
     467,   468,   469,   470,   471,   427,   472,   473,   474,   475,
     476,   478,   477,   479,   480,     0,   481,   482,   464,     0,
     465,   488,   489,   490,   492,   493,   494,   285,   286,   287,
     168,   181,   179,    92,    85,     0,     0,   254,   135,     0,
     146,   148,   151,   149,   135,     0,   142,    93,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      11,   258,     0,     0,     0,    30,    31,    32,    33,    34,
      35,   233,   223,    14,    24,    36,     0,    38,    42,    45,
      48,    53,    56,    58,    60,    62,    64,    66,    68,    81,
       0,   225,   226,   211,   212,   228,     0,   213,   214,   215,
     216,    10,    12,   227,   232,   256,   171,     0,   182,   184,
     169,    36,    83,     0,    10,   192,     0,   186,   188,   191,
       0,   158,     0,     0,   269,   270,     0,     0,     0,     0,
     405,     0,   410,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   345,
       0,     0,   611,   606,     0,     0,     0,  1893,  1569,     0,
       0,  1571,  1086,     0,     0,     0,     0,     0,  1855,  1573,
       0,     0,     0,   935,     0,     0,   773,   775,     0,     0,
       0,   304,   521,   522,   523,   558,   524,     0,   525,   569,
     562,   570,   526,   567,   548,   527,   528,   529,   546,  1476,
     549,   530,   533,   559,  1491,   550,   534,   535,   563,   536,
     561,   537,  1474,   568,   564,     0,   547,   538,   566,   539,
     540,   553,   565,   542,   560,  1475,   554,   555,   581,   543,
     544,   545,   551,   556,   557,   572,   582,   571,     0,   573,
     541,   552,  1477,  1478,     0,  1174,     0,     0,  1484,  1481,
    1480,  1971,  1482,  1483,  1932,  1931,     0,     0,   531,   532,
    1500,     0,     0,  1094,  1461,  1463,  1466,  1469,  1473,  1496,
    1492,  1494,  1495,   520,  1485,  1198,  1199,     0,     0,     0,
       0,     0,  1086,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,  1087,     0,     0,     0,
       0,   394,   382,   381,     0,   331,   335,     0,   334,   337,
     336,     0,   338,     0,   332,   512,   513,     0,   722,   714,
       0,   715,     0,   716,     0,   718,   724,   719,   720,   721,
     725,   717,     0,     0,   658,   726,     0,     0,     0,   324,
       0,     0,     0,     0,  1095,  1384,  1386,  1388,  1390,     0,
    1421,     0,   608,   607,  1203,     0,  1931,     0,   402,     0,
    1190,  1192,  1071,     0,   621,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   347,   346,   397,   396,     0,     0,
       0,     0,     0,   281,   314,   314,   282,     0,   610,   774,
       0,   610,   372,   373,   374,   508,   378,   379,   380,     0,
     284,   280,   497,   499,   501,   510,     0,   330,     0,   517,
     519,     0,  1129,  1130,  1353,     0,  1076,  1170,     0,  1168,
    1171,     0,  1347,  1338,   390,   389,   391,  1577,  1577,     0,
       0,     0,    91,     0,   206,    94,   136,   137,   135,   147,
       0,     0,   152,   155,   139,   135,   250,     0,   249,     0,
       0,     0,     0,   251,     0,     0,    28,     0,     0,     0,
     259,     0,    25,    26,     0,   192,     0,    20,    21,     0,
       0,     0,     0,    77,    76,    74,    75,    71,    72,    73,
      78,    79,    80,    70,     0,    27,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   234,   224,   229,     0,
     173,     0,   170,     0,     0,   190,   194,   193,   195,   172,
       0,   162,   164,   159,     0,   420,     0,   411,     0,   416,
       0,   406,   408,     0,   407,  1766,     0,     0,     0,     0,
    1767,     0,     0,     0,  1774,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   593,   344,     0,     0,     0,     0,     0,     0,
       0,     0,  1068,     0,     0,   929,     0,   965,     0,   936,
       0,   651,     0,     0,  1069,     0,     0,     0,   323,   307,
     306,   305,   308,     0,     0,  1508,  1511,     0,  1472,   525,
    1499,  1526,   520,     0,     0,     0,  1557,     0,  1470,  1471,
    1971,  1971,  1975,  1944,  1972,     0,  1969,  1969,  1501,  1489,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
    1200,     0,  1206,  1949,  1207,  1211,  1215,   520,     0,  1797,
    1895,  1751,  1750,     0,  1098,   817,   744,   833,  1858,  1752,
     914,  1079,   650,   818,     0,   757,  1851,  1930,     0,     0,
       0,     0,     0,  1971,  1757,     0,     0,  1753,     0,  1598,
       0,     0,     0,   341,   340,   339,     0,   333,   387,   723,
       0,   682,   693,   700,   659,     0,   663,     0,   676,   681,
     672,   683,   668,   692,   695,   708,     0,   699,     0,     0,
       0,   694,   701,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   401,  1389,  1174,
    1418,  1417,     0,     0,     0,  1439,     0,     0,     0,     0,
       0,  1425,  1422,  1427,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,  1204,  1205,     0,     0,
       0,     0,   612,     0,     0,  1989,  1215,     0,     0,     0,
       0,     0,   820,     0,     0,   812,     0,     0,  1194,   514,
     515,   368,   368,   370,   371,   376,  1948,   356,     0,   375,
       0,   302,   301,     0,     0,     0,     0,   283,     0,     0,
       0,     0,     0,  1354,   506,  1353,     0,   505,     0,     0,
    1169,  1188,     0,     0,  1348,  1186,  1349,  1347,     0,     0,
       0,  1575,  1576,     0,  1867,  1866,     0,     0,     0,   209,
       0,   138,   156,   154,   150,     0,     0,   140,     0,   220,
     222,     0,     0,     0,   248,   252,     0,     0,     0,     0,
      13,     0,   194,     0,    19,    16,     0,    22,     0,    18,
      69,    39,    40,    41,    43,    44,    47,    46,    51,    52,
      49,    50,    54,    55,    57,    59,    61,    63,    65,     0,
      82,   217,   218,   183,   185,   202,     0,     0,   198,     0,
     196,     0,     0,   187,   189,     0,     0,     0,   412,   414,
       0,   413,   409,     0,     0,     0,  1781,     0,  1782,  1783,
       0,  1089,  1090,     0,  1790,  1772,  1773,  1749,  1748,  1096,
     847,   907,   906,   848,     0,   846,   836,   839,     0,   837,
       0,     0,     0,     0,  1009,     0,   912,     0,     0,   912,
       0,     0,     0,     0,     0,     0,     0,     0,   875,   863,
    1891,  1890,  1889,  1857,     0,     0,     0,     0,     0,     0,
     627,   638,   637,   634,   635,   636,   641,  1078,   919,     0,
       0,   918,   916,   922,     0,     0,     0,   795,     0,   809,
     803,     0,     0,     0,     0,     0,     0,  1844,  1845,  1805,
    1802,     0,     0,     0,     0,     0,   594,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   937,  1894,  1570,  1572,
    1856,  1574,  1070,     0,     0,     0,   930,   931,     0,     0,
       0,     0,     0,     0,     0,  1009,   989,   972,   967,   986,
     966,   971,     0,     0,  1071,     0,  1755,  1756,     0,   784,
    1071,     0,   309,     0,     0,     0,  1508,  1505,     0,     0,
    1510,     0,     0,  1490,   520,  1175,  1178,  1071,  1197,  1561,
    1347,  1560,  1493,     0,  1971,  1941,  1942,  1973,  1970,  1974,
       0,  1974,  1524,     0,  1479,  1462,  1464,  1465,  1467,  1468,
       0,     0,  1534,     0,     0,  1594,     0,   575,   577,   576,
     578,   532,   580,  1500,     0,   520,  1215,     0,     0,     0,
       0,  1218,  1214,  1216,     0,  1084,   745,   834,   819,   915,
     649,  1910,     0,  1812,     0,     0,     0,  1044,     0,  1971,
       0,     0,   385,   384,  1967,     0,     0,     0,  1597,  1754,
    1595,     0,     0,     0,   342,   343,     0,     0,   665,   678,
     685,   670,   661,     0,   712,   702,   664,   677,   684,   669,
     696,   660,     0,   711,   673,   704,   686,   703,   666,   679,
     674,   687,   671,   697,   709,   662,     0,   713,   675,   667,
     698,   710,   688,   680,   690,   691,   689,     0,     0,   730,
     654,   728,   729,   657,     0,     0,  1110,  1215,  1989,  1989,
     326,   325,  1939,  1385,  1387,  1419,  1420,     0,  1415,  1393,
    1432,     0,     0,     0,  1424,  1433,  1444,  1445,  1426,  1423,
    1429,  1437,  1438,  1435,  1448,  1449,  1428,  1397,  1398,  1399,
    1400,  1401,  1402,  1286,  1291,  1292,  1174,  1288,  1287,  1289,
    1290,  1458,  1460,  1403,  1455,   520,  1459,  1404,  1405,  1406,
    1407,  1408,  1409,  1410,  1411,  1412,  1413,  1414,   735,     0,
       0,     0,     0,  1957,  1763,  1762,     0,   617,   616,  1761,
       0,     0,     0,  1164,  1165,  1338,     0,  1173,  1181,     0,
    1101,     0,  1191,  1075,     0,  1074,     0,   813,   622,     0,
       0,     0,     0,     0,     0,   972,     0,   366,   367,     0,
     511,     0,   319,     0,  1068,     0,     0,   355,     0,  1356,
    1352,     0,  1339,  1364,  1366,     0,  1343,  1346,  1350,  1187,
    1578,     0,  1581,  1583,     0,  1629,     0,  1587,     0,  1638,
       0,  1864,  1863,  1868,  1859,  1892,     0,   207,   153,   157,
     219,   221,     0,     0,     0,     0,    29,     0,     0,     0,
      37,    17,     0,    15,     0,   203,   197,   199,   204,     0,
     200,     0,   419,   404,   403,   415,     0,     0,     0,  1788,
       0,  1786,     0,  1091,     0,     0,  1793,     0,  1791,  1792,
    1097,     0,     0,     0,   844,   838,     0,     0,     0,     0,
       0,   613,     0,   913,     0,     0,   899,     0,   908,     0,
       0,     0,     0,     0,   878,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   862,     0,     0,     0,
       0,   875,   858,     0,     0,   859,     0,   640,   639,     0,
       0,     0,   642,     0,     0,   917,     0,   925,   924,     0,
     923,     0,  1906,     0,     0,     0,     0,   797,     0,   762,
       0,     0,     0,     0,     0,     0,   810,   758,     0,     0,
     801,   800,   803,   760,     0,     0,  1806,     0,  1807,     0,
     604,   605,   595,     0,     0,  1920,  1919,     0,     0,     0,
       0,  1927,  1928,  1929,   600,     0,     0,     0,     0,   614,
     348,     0,     0,     0,     0,     0,     0,  1069,     0,     0,
    1016,  1034,     0,  1014,   928,   932,   742,     0,     0,     0,
    1982,  1983,  1981,  1061,   987,     0,  1000,  1001,  1002,     0,
       0,   990,   992,     0,     0,   969,   985,   968,     0,     0,
     965,  1820,  1831,  1814,     0,     0,   788,     0,     0,   310,
       0,  1512,     0,  1504,     0,  1509,  1503,     0,  1498,   520,
       0,     0,  1203,     0,     0,  1556,  1943,  1945,  1968,  1946,
    1523,     0,     0,     0,     0,  1515,  1562,  1527,  1603,  1488,
       0,     0,     0,  1382,  1971,     0,     0,  1950,     0,  1382,
    1210,  1219,  1217,  1212,   520,   835,   738,  1085,     0,     0,
       0,     0,   377,  1933,  1969,  1969,     0,     0,     0,     0,
    1599,     0,  1758,  1598,  1760,   327,     0,   706,   705,   707,
       0,     0,     0,  1155,  1109,  1108,  1155,  1215,  1044,     0,
    1416,     0,  1394,     0,     0,  1431,  1442,  1443,  1434,  1446,
    1447,  1430,  1440,  1441,  1436,  1450,  1451,     0,     0,     0,
       0,   609,   615,     0,     0,     0,   620,  1176,     0,  1102,
       0,  1347,  1338,     0,     0,  1072,     0,     0,     0,     0,
    1379,     0,     0,     0,     0,     0,     0,   831,     0,  1947,
     315,     0,     0,   317,   316,     0,   383,     0,     0,  1357,
    1358,  1359,  1360,  1361,  1362,  1355,  1077,     0,  1367,  1368,
    1369,  1340,     0,     0,  1579,     0,  1584,  1585,     0,     0,
       0,  1617,  1618,     0,  1587,     0,  1630,  1632,  1633,  1634,
    1635,  1636,  1642,     0,  1568,  1588,  1628,  1637,  1870,     0,
    1865,     0,   208,   210,     0,     0,     0,     0,     0,     0,
     237,   238,   235,    23,    67,   205,   201,  1777,  1775,     0,
       0,  1778,  1784,     0,  1780,     0,  1794,  1795,  1796,     0,
     840,     0,   842,   845,   933,  1092,  1093,   585,   584,   583,
     586,   590,   400,   592,     0,   399,   398,  1010,  1976,     0,
     877,  1009,     0,     0,     0,     0,     0,   860,     0,   894,
       0,     0,   887,   909,     0,   879,   914,     0,   856,   886,
       0,     0,     0,     0,     0,     0,   857,   868,   885,   876,
     645,   644,   648,   647,   646,   643,   920,   921,   927,   926,
       0,  1907,  1908,     0,   794,   747,     0,   749,   779,   748,
     750,     0,   796,   754,   773,   752,   756,     0,   751,   753,
       0,   759,   804,   806,     0,   802,     0,   761,     0,  1808,
    1850,  1809,  1849,     0,     0,   596,   599,   598,  1923,  1926,
    1925,     0,  1924,   602,  1100,   623,  1099,     0,     0,  1820,
       0,  1080,     0,  1840,  1764,     0,  1765,     0,     0,  1071,
     743,  1018,  1017,  1019,  1016,  1041,  1036,  1062,     0,     0,
       0,     0,   965,  1009,     0,     0,     0,   991,     0,     0,
     970,  1836,   625,   989,  1820,  1815,  1821,     0,     0,     0,
    1798,  1832,  1834,  1835,     0,   785,   786,     0,   763,     0,
       0,     0,     0,     0,  1507,  1506,  1502,     0,     0,  1177,
       0,     0,  1559,  1558,  1562,  1562,  1534,  1536,     0,  1534,
       0,  1562,     0,  1516,     0,     0,  1500,     0,   520,     0,
    1341,  1951,  1969,  1969,     0,  1124,  1125,  1127,  1123,     0,
       0,     0,  1852,     0,     0,  1052,     0,     0,  1974,  1974,
    1971,     0,     0,   392,   393,   579,   574,  1600,     0,   329,
       0,   731,   727,     0,     0,     0,     0,     0,  1106,     0,
       0,  1940,  1395,     0,     0,  1391,  1454,  1457,   520,     0,
     738,  1959,  1960,     0,     0,  1971,  1958,     0,  1382,   618,
     619,  1180,  1172,  1166,  1347,     0,     0,  1136,  1382,  1132,
       0,   520,  1073,     0,   815,  1376,     0,  1377,  1353,     0,
       0,     0,     0,   821,     0,   989,   832,   369,   320,   321,
     318,     0,   328,     0,  1366,  1363,     0,  1365,  1344,  1345,
    1580,  1586,  1589,     0,  1629,     0,     0,  1577,  1567,     0,
    1631,     0,  1643,     0,     0,     0,  1883,  1869,  1860,  1861,
    1868,     0,   240,     0,     0,     0,     0,     0,     0,     0,
    1776,  1789,  1785,     0,  1779,   841,   843,   589,   588,   591,
     587,     0,   989,   905,     0,  1012,     0,  1011,     0,     0,
    1366,     0,     0,   940,     0,     0,   889,     0,     0,     0,
       0,   910,  1366,     0,     0,     0,     0,   882,     0,     0,
       0,     0,   871,   869,  1905,  1904,  1909,     0,     0,   778,
     798,     0,     0,     0,     0,   799,  1846,  1847,     0,  1810,
    1804,     0,     0,   597,  1916,   603,   601,     0,   349,   350,
    1820,  1799,     0,  1082,  1841,     0,     0,     0,  1768,     0,
    1769,     0,     0,     0,  1019,     0,  1035,  1042,     0,  1063,
    1976,  1013,     0,   988,     0,   989,   999,     0,     0,   963,
     974,   973,  1801,  1837,   626,   629,   633,   630,   631,   632,
    1897,  1822,  1823,  1820,     0,  1818,     0,  1842,  1838,  1813,
    1833,  1912,   787,   789,     0,     0,  1076,  1081,     0,   312,
     311,   303,  1525,  1497,     0,  1208,  1534,  1534,  1520,     0,
    1541,     0,  1517,     0,  1534,  1605,  1514,  1604,  1487,     0,
       0,  1383,  1103,  1342,  1974,  1974,  1971,     0,     0,     0,
    1213,   738,   739,   386,   388,  1045,     0,     0,  1934,  1935,
    1936,  1969,  1969,     0,  1759,  1598,   731,     0,   655,  1154,
       0,     0,  1104,     0,     0,     0,     0,  1396,  1392,     0,
     738,   740,  1969,  1969,  1961,     0,     0,     0,     0,     0,
    1220,  1222,  1231,  1229,  1989,  1328,  1167,  1990,  1140,     0,
    1139,   520,     0,  1341,     0,     0,  1193,     0,   814,  1378,
    1380,     0,     0,  1353,     0,     0,   824,     0,   943,     0,
    1048,  1370,  1371,     0,     0,  1582,  1614,  1612,  1587,     0,
       0,  1619,     0,     0,     0,     0,     0,   610,     0,  1629,
    1587,     0,     0,     0,     0,     0,     0,     0,     0,   610,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,  1351,  1698,  1610,     0,  1680,  1654,
    1668,  1669,  1684,     0,  1670,  1673,  1675,  1676,  1677,  1587,
    1723,  1722,  1724,  1725,  1671,  1672,  1674,  1678,  1679,  1681,
    1682,  1683,  1598,  1640,  1589,     0,     0,  1876,  1875,  1871,
    1873,  1877,     0,     0,  1862,   239,   241,   242,     0,   244,
       0,     0,     0,   236,  1787,  1977,  1979,     0,   902,     0,
     989,  1008,     0,   940,   940,   940,     0,     0,   952,   894,
       0,     0,   864,     0,     0,     0,     0,     0,     0,     0,
    1050,   914,     0,     0,     0,     0,   861,   867,   881,     0,
       0,   873,     0,   781,   772,   755,     0,   805,   807,  1811,
    1848,  1803,   624,     0,     0,  1800,     0,  1770,     0,     0,
       0,  1062,  1032,     0,     0,  1065,   964,     0,   961,     0,
       0,   628,     0,     0,     0,     0,  1820,  1816,  1839,  1843,
       0,  1914,     0,     0,     0,   767,   764,   766,     0,  1066,
    1083,     0,     0,     0,     0,     0,  1518,  1519,     0,     0,
    1543,     0,     0,  1513,     0,  1522,     0,  1500,  1953,  1954,
    1952,  1969,  1969,  1126,  1853,  1051,  1115,     0,  1113,  1114,
    1974,  1974,   574,   656,     0,  1156,     0,     0,  1156,     0,
    1105,  1117,  1299,  1158,  1456,   740,   741,   736,  1974,  1974,
       0,     0,  1971,     0,     0,     0,     0,     0,  1304,  1300,
    1304,  1304,     0,     0,     0,     0,  1237,     0,     0,     0,
    1201,     0,  1336,     0,     0,     0,  1131,  1119,  1134,  1133,
    1137,     0,     0,  1372,  1353,     0,  1380,     0,     0,   322,
     945,   938,   944,  1366,  1591,     0,  1566,  1621,     0,     0,
       0,     0,     0,  1654,     0,  1592,     0,  1720,  1714,     0,
     606,  1747,     0,  1737,     0,     0,  1611,     0,  1593,     0,
       0,  1740,     0,  1598,  1745,     0,   608,     0,  1607,     0,
    1744,     0,  1665,     0,  1695,  1694,  1696,  1693,  1689,  1687,
    1688,  1692,  1690,  1691,     0,     0,  1657,     0,  1655,  1667,
    1697,     0,     0,     0,     0,     0,     0,  1878,  1879,  1880,
       0,  1629,  1854,   243,   245,   246,     0,  1978,  1980,   900,
       0,     0,  1984,     0,   952,   952,   952,  1046,     0,     0,
    1003,   889,     0,   897,     0,     0,     0,     0,     0,     0,
     853,     0,   914,   911,  1366,     0,     0,   983,   984,     0,
     870,   872,     0,   865,     0,     0,   780,     0,   811,     0,
     353,   351,     0,  1771,  1076,  1020,  1976,     0,  1009,  1043,
    1037,     0,     0,     0,     0,   998,     0,   996,     0,  1898,
    1899,     0,  1828,  1825,  1842,  1824,  1820,  1817,  1913,  1917,
    1915,   770,   768,   769,   765,   777,     0,   776,   313,  1179,
    1209,  1602,  1382,  1382,  1540,  1537,  1539,     0,     0,     0,
    1564,  1565,     0,     0,  1606,  1521,  1486,  1974,  1974,  1120,
       0,  1937,  1938,   732,  1153,     0,  1107,     0,     0,     0,
       0,  1382,   737,  1963,  1964,  1969,  1969,  1962,     0,  1931,
       0,  1232,     0,     0,     0,  1237,  1221,  1305,  1303,  1301,
    1302,     0,  1215,     0,  1295,  1989,  1989,     0,  1252,  1215,
    1238,  1239,     0,     0,  1335,     0,  1326,  1336,     0,  1332,
       0,  1138,  1141,   816,     0,  1380,  1353,  1374,   823,     0,
       0,     0,   940,  1047,  1590,     0,     0,     0,  1615,  1613,
       0,  1620,  1351,     0,     0,     0,     0,  1720,  1717,     0,
       0,  1713,     0,  1734,     0,     0,     0,     0,     0,  1992,
    1991,     0,  1739,     0,  1705,     0,  1741,     0,  1746,     0,
       0,  1587,  1743,     0,     0,  1686,     0,     0,  1656,  1658,
    1660,  1627,  1728,  1608,  1699,   520,  1641,  1646,     0,  1647,
       0,     0,  1652,  1872,  1881,     0,  1886,  1885,  1888,   247,
     901,   903,     0,   849,     0,  1006,  1007,  1004,     0,   954,
     888,   895,     0,     0,     0,     0,   892,     0,     0,   851,
     852,     0,   866,  1049,   899,     0,     0,     0,     0,     0,
       0,   782,   771,   808,     0,  1088,  1067,  1064,  1033,     0,
    1015,  1009,  1024,     0,     0,     0,     0,   962,   997,   994,
     995,     0,     0,  1900,  1896,  1843,  1826,  1819,  1921,  1918,
     746,   791,     0,  1328,  1328,     0,  1542,  1547,     0,  1554,
       0,  1545,     0,  1535,     0,     0,  1955,  1956,  1112,  1111,
    1118,  1116,     0,  1163,     0,  1161,  1162,  1341,  1974,  1974,
       0,  1215,     0,     0,  1215,  1215,  1299,  1228,  1215,     0,
       0,  1296,  1237,     0,     0,     0,  1224,  1334,  1331,     0,
       0,  1196,  1329,  1337,  1333,  1330,  1135,  1381,  1373,  1380,
       0,     0,     0,     0,   948,     0,   954,     0,     0,  1622,
       0,     0,     0,  1587,  1715,  1719,     0,     0,  1716,  1721,
       0,     0,  1654,  1738,  1736,     0,     0,     0,     0,     0,
     520,  1710,     0,     0,     0,     0,  1707,  1726,     0,     0,
     520,     0,  1666,     0,     0,     0,     0,  1664,  1659,     0,
       0,     0,     0,  1653,     0,     0,  1874,  1882,     0,  1884,
       0,     0,     0,  1987,     0,  1005,   942,   941,     0,   960,
       0,   953,   955,   957,   896,   898,     0,   890,     0,  1984,
       0,   850,   880,     0,   982,     0,     0,     0,     0,   783,
     352,     0,     0,     0,  1025,     0,  1366,  1030,  1023,     0,
       0,  1038,     0,     0,   977,     0,  1901,  1902,  1829,  1827,
    1830,  1911,  1922,     0,  1601,  1201,  1201,  1538,     0,  1546,
       0,  1548,  1549,     0,  1531,     0,  1530,     0,  1142,  1144,
    1159,     0,  1157,  1965,  1966,     0,  1230,  1236,   520,     0,
    1227,  1225,     0,  1293,  1294,  1989,  1215,     0,     0,  1242,
    1253,     0,     0,     0,     0,  1321,  1322,  1202,  1320,  1324,
    1327,  1375,   822,     0,     0,   827,     0,   946,     0,   939,
     934,  1623,  1624,  1616,  1639,     0,  1718,     0,     0,  1735,
       0,  1702,     0,     0,     0,  1587,     0,  1706,     0,     0,
       0,  1742,  1701,  1727,  1685,  1700,  1662,  1661,     0,  1609,
       0,  1645,  1648,  1644,  1651,     0,   904,     0,  1985,     0,
    1044,   959,   958,   956,   893,   891,   855,     0,     0,     0,
     874,     0,     0,     0,  1366,  1026,  1366,   940,  1039,  1040,
    1976,   975,     0,     0,  1903,  1829,   790,  1326,  1326,     0,
    1555,     0,  1532,  1533,  1528,     0,     0,  1146,     0,  1143,
    1160,  1233,     0,  1215,  1298,  1297,  1223,     0,     0,     0,
       0,     0,  1258,     0,  1255,     0,     0,     0,     0,     0,
    1323,     0,   825,     0,     0,   947,  1626,     0,  1711,  1587,
       0,     0,  1731,     0,  1534,     0,  1705,  1705,     0,  1663,
    1650,     0,     0,  1988,  1986,  1054,     0,   899,   899,     0,
     792,  1027,   940,   940,   952,     0,   979,   976,   993,  1182,
    1183,  1551,     0,  1544,     0,  1563,  1529,  1147,  1148,     0,
    1235,   520,  1226,     0,     0,     0,  1246,  1241,     0,     0,
       0,  1257,     0,     0,  1309,  1310,     0,  1174,  1317,     0,
    1315,  1313,  1318,  1319,  1325,     0,   826,   950,  1712,     0,
    1703,  1731,     0,     0,  1733,  1527,  1709,  1708,  1704,  1649,
    1887,     0,  1053,  1984,     0,     0,     0,   952,   952,  1028,
    1976,     0,     0,  1550,  1552,  1553,  1155,     0,  1145,  1215,
    1215,     0,     0,  1240,  1254,     0,     0,  1307,     0,  1306,
    1316,  1311,     0,     0,     0,   949,  1625,     0,  1732,  1587,
       0,     0,     0,   854,     0,     0,     0,  1031,  1029,   981,
       0,     0,     0,     0,  1244,  1243,  1245,     0,     0,     0,
    1249,  1215,  1285,  1256,     0,     0,  1308,  1314,  1312,     0,
     951,  1587,     0,  1058,  1060,     0,  1057,  1055,  1056,   899,
     899,     0,     0,  1976,     0,  1382,  1283,  1284,  1247,     0,
    1250,     0,  1251,     0,     0,  1729,  1059,     0,     0,   793,
       0,   980,     0,  1341,  1248,     0,  1268,     0,  1261,  1263,
    1265,   830,     0,     0,  1730,   883,   884,  1976,     0,  1151,
       0,  1267,  1259,     0,     0,     0,     0,   978,  1382,  1382,
    1149,  1264,     0,  1260,     0,  1262,  1270,  1273,  1274,  1276,
    1279,  1282,     0,   829,  1150,  1341,  1266,     0,  1272,     0,
       0,     0,     0,     0,     0,  1152,  1269,     0,  1275,  1277,
    1278,  1280,  1281,   828,  1271
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
      -1,    54,    55,    56,   373,   374,  1156,   375,   376,   377,
     378,   379,   380,   381,   382,   383,   384,   385,   386,   387,
     388,   389,   784,   390,   413,    57,   120,   109,   335,   110,
      59,   103,   737,    61,   115,   116,    62,   339,   340,   341,
     741,  1135,   742,    63,   130,   131,    64,    65,   127,   128,
      66,   415,   407,   408,  1186,   417,   418,   419,   817,   818,
     735,  1130,   392,   393,   394,   395,   396,   122,   123,   397,
     398,   399,   400,    67,   124,   414,   402,   343,    70,    71,
      72,    73,    74,    75,    76,    77,    78,    79,    97,   191,
     192,   193,   892,  1343,  2203,  1091,  1964,  2318,   194,   195,
     196,   613,   614,   974,   197,   198,   199,   200,   201,   202,
     203,  1587,   204,   205,   206,   207,  1422,   208,   209,   210,
     211,   212,   213,   214,   604,   215,    98,    99,   100,   101,
     216,   217,   218,   219,   220,   695,   222,   223,   560,  1394,
     561,   224,   225,  2426,  1762,  2624,  2625,  2626,   453,  2627,
     230,   231,  1681,  1065,  1297,   232,   233,  2464,  2465,  1260,
    2466,  1262,  1263,  2467,  2468,  2469,  2094,   234,   235,   236,
     237,   238,   239,   633,   634,  1480,  1481,  2538,   240,   241,
     242,  2242,  2817,   243,   244,   245,  2485,  2766,  2767,  2725,
     477,   478,  3016,  2107,  2108,  2976,  1826,  2198,  3230,  2105,
    1739,  1752,  1753,  2122,  2123,  1747,   246,   247,   248,  2578,
     249,   250,   251,   252,   253,  1955,   254,  1585,  3454,  3455,
    2315,  1408,   255,  1226,  1227,   256,  2402,  2403,  2721,  2973,
    1715,  1249,  2702,  3185,  3186,  2386,  2952,  2953,  2060,  2687,
    2688,  1686,  1228,   257,  1687,  1410,   258,  1272,  1273,  1730,
     259,  1317,  1794,  1229,   260,   261,  3102,  2698,  2861,  2862,
    3293,  3294,  3685,  2950,  3361,  3362,  3363,   262,  1326,  1815,
    1327,  3393,  3394,  2966,  2967,  1328,  1329,  1810,  1811,  1812,
    2996,  2997,  1805,  1806,  1807,  1689,  2376,  1792,  1793,  2163,
    2443,  3210,  3211,  3212,  2988,  2164,  2165,  2446,  2447,  1881,
    2383,  2323,  2393,  2246,  3355,  3652,  3726,  1330,  2450,   263,
     264,   265,   479,  1069,  1574,  1575,  1107,   266,   267,   268,
     269,   480,  1784,   270,   271,   272,   273,   274,   275,   276,
    1066,  2628,  2629,  1485,  1486,  2797,  2798,  2810,  2811,  2630,
    2631,   281,   282,   283,  1869,  2235,  2236,   714,  2298,  2299,
    2300,  2569,  2570,  2632,  3418,  3419,  3538,  3668,  3780,  2540,
    2264,  2633,  3051,  3254,  3255,  2634,  1563,  1564,   721,  1565,
     905,  1566,  1355,  1356,  1567,  1568,   286,   287,   288,   289,
     290,   660,   661,   291,   292,   576,   931,  3086,  1058,   933,
    2774,   934,   935,  1402,  2288,  2560,  2561,  3060,  3079,  3080,
    3438,  3439,  3550,  3619,  3709,  3710,  3081,  3275,  3553,  3620,
    3554,  3715,  3757,  3758,  3759,  3770,  3760,  3785,  3797,  3786,
    3787,  3788,  3789,  3790,  3791,  1532,  2562,  3073,  3074,  2563,
    2832,  3068,  3445,  3623,  3624,  3446,  3629,  3630,  3631,  3447,
    3448,  3560,  3281,  2840,  2841,  3285,  2842,  3089,   722,  2512,
    1114,  1606,  1115,  1116,   716,  1104,  1975,  1602,  1603,  1980,
    2327,   293,  1951,  2853,  2230,   906,   645,   646,   647,   648,
    1034,  1035,  1036,  1037,  1038,  1039,  1040,  1041,  1042,  1043,
    1044,  1045,  1046,  1047,  1048,  1049,  1050,  1051,   562,  1533,
     650,   564,   565,   566,   567,   568,   901,   919,   569,  1346,
    1347,  1831,  1349,   895,   896,   570,  2221,  3415,  3416,  1855,
    2500,  3025,  3026,  2780,  3029,  3241,  3603,  3242,   907,   571,
    1360,  1361,  2219,  3032,   294,   295,   296,   297,   298,   299,
    1123,  1119,  1611,  1612,  1988,  2004,  2595,  2876,  2889,  1386,
    1429,  2635,  3020,   572,  2637,  2885,  1990,  1991,  2638,  1994,
    1617,  1995,  1996,  1997,  1618,  1998,  1999,  2000,  2343,  2001,
    3158,  3341,  3161,  3162,  2917,  2918,  3148,  3149,  3150,  3336,
    2901,  2639,  2640,  2641,  2642,  2643,  2644,  2645,  2646,  3325,
    3326,  3134,  2647,  2877,  2878,  3117,  3118,  3306,  3120,  2648,
    2649,  2650,  2651,  2652,  3643,  2653,  2654,  2655,  3126,  2656,
    2657,  2658,  2659,  2660,  2661,   300,   301,   302,   303,   304,
     965,   966,  1338,   967,   968,   305,  1305,   306,   844,  2156,
    2157,  2158,   307,  1661,  2034,  2031,  1215,  1216,   308,   309,
     310,   311,  1289,   312,   313,  1822,  1823,  2185,  2186,  3226,
    2190,  2191,  2462,  2192,  2193,  2154,  3400,  1290,  2420,  2131,
    2132,  1413,   314,   315,   316,   317,  1126,  1624,  1127,  2010,
    2346,  2669,  2670,  2671,  2929,  3165,  3346,  2673,  3349,  2932,
    1253,   318,   319,   320,   321,  2753,  2102,   322,  1732,   323,
     324,  2761,  3009,  1771,  3228,  1772,  3401,  1773,   325,   326,
     573,  1423,  1491,   574,  1087,  1398,  1867,  1934,  2285,  2286,
     970,  1369,  1370,   913,   914,   915,  2372,  2373,  2937,  1331,
    3173,  3352,  3353,  1570,  3131
};

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
#define YYPACT_NINF -3311
static const yytype_int16 yypact[] =
{
    4855, -3311, -3311,   253,  1049, -3311, -3311,   104, -3311, -3311,
   -3311, -3311, -3311, -3311, -3311, -3311, -3311, -3311, -3311, -3311,
   -3311, -3311, -3311, -3311, -3311, -3311, -3311, -3311, -3311, -3311,
   -3311, -3311, -3311, -3311, -3311, -3311, -3311, -3311, -3311, -3311,
   -3311, -3311,   639,  1646,  1155, -3311, -3311, -3311, -3311,  1327,
    1366,   230,   124,  2288,  4758, -3311, -3311, -3311,   161, 14927,
   14927, -3311,   114, -3311,  3363,  1763,   116, -3311, -3311, -3311,
   -3311, -3311, -3311, -3311, -3311, -3311, -3311, -3311, -3311, -3311,
   -3311, -3311,  1613,  1269,  1659,  1669, -3311, -3311, -3311, -3311,
   -3311,   853,  1652,  1680,  1481, -3311,    -1,  5857,  1485,  1516,
    1527, -3311,  1554, -3311, -3311,  2288, -3311, -3311, -3311,   302,
   -3311,  2402, -3311, -3311, -3311,   165,  3956,  1500, -3311, -3311,
     161, -3311,  4178,  3363, -3311,   106,  1541,  2510, 15089,  1763,
     313, -3311,  1559,  1613,  1921,  1904, -3311, -3311,  1862,  1869,
    1632,  1712,   348,  5096,   -46,  1860,  1843,  1755,  2702,  1761,
   11908,  1791,  1791, -3311,  5086,   612,  1791,    75,   548,  1794,
    2916,  1791,  1791,  1791,  1771,  2004,  9451,  2916,  1843,  1542,
    1791,    83,  2035,  1791,  1542,  1542,  1996,  2016,  2056,  2082,
    2093,  2147,   875,  2064,  1886,  2087,  2178,  1420,  2042,  2045,
    2049,  2104, -3311,   909, -3311, -3311, -3311, -3311, -3311, -3311,
   -3311, -3311,  2126, -3311, -3311, -3311, 14226, -3311, -3311, 14293,
   -3311, -3311, -3311, -3311,  2177,  2140,  2176, -3311, -3311,   817,
     101,   227, -3311, -3311, -3311, -3311,  2148, -3311,  2154, -3311,
   -3311, -3311, -3311, -3311, -3311, -3311, -3311, -3311, -3311, -3311,
   -3311, -3311, -3311, -3311, -3311, -3311, -3311, -3311, -3311, -3311,
   -3311, -3311, -3311, -3311, -3311, -3311, -3311, -3311, -3311, -3311,
   -3311,  1542, -3311, -3311, -3311, -3311, -3311, -3311, -3311, -3311,
   -3311, -3311, -3311, -3311, -3311, -3311, -3311, -3311, -3311, -3311,
   -3311,  1540, -3311, -3311, -3311, -3311,  2352, -3311, -3311,  1199,
     875, -3311, -3311, -3311,  2214,  2226,  2242,  1542,  1542,  1542,
   -3311, -3311, -3311, -3311, -3311, -3311, -3311, -3311, -3311, -3311,
   -3311, -3311, -3311, -3311, -3311,  1542, -3311, -3311, -3311,  1542,
   -3311, -3311, -3311, -3311, -3311, -3311, -3311, -3311, -3311, -3311,
   -3311, -3311, -3311, -3311, -3311,   124,  1062, -3311,  2527,  4384,
   -3311, -3311,  4596, -3311,  2527,  4488, -3311,  2197,  2201,  2510,
    2210,  2219,  1827,  2246,  1613,  1486,  2598,  2249,  2253,  2257,
   -3311,  2617,  2636,  2636,  4672, -3311, -3311, -3311, -3311, -3311,
   -3311, -3311, -3311, -3311,   201,   176,  2510, -3311,  1684,  1306,
    2191,   145,  1916,  2263,  2269,  2270,  2646,    99, -3311, -3311,
    1129, -3311, -3311, -3311, -3311, -3311,  4302, -3311, -3311, -3311,
   -3311,  2277, -3311, -3311, -3311, -3311, -3311,  2300,  2297, -3311,
   -3311, -3311, -3311,  2301, -3311,  4960,  2307,  2304, -3311, -3311,
    1613, -3311,  2510,   650, -3311, -3311,  2607,  2363,  2364,   536,
   -3311,  2356, -3311,    77,   927,  1542,  1542,  1542,  1542,   835,
    1542,  1542,  1542,  1542,  1542,  2461,  1542,  1542,    53, -3311,
    2357,   550, -3311,  2359,  2447,  2418,  1125, -3311, -3311,  2483,
    2446, -3311,  2497,  1542,  1542,  1542,  1542,  2423, -3311, -3311,
    2456,  1542,  1542, -3311,  2481,  1542,    51, -3311,  2433,  2487,
    2614,   747, -3311, -3311, -3311, -3311, -3311,  9997,  2404, -3311,
   -3311, -3311, -3311, -3311, -3311, -3311, -3311, -3311, -3311, -3311,
   -3311, -3311, -3311, -3311, -3311, -3311, -3311, -3311, -3311, -3311,
   -3311, -3311, -3311, -3311, -3311, 12181, -3311, -3311, -3311, -3311,
   -3311, -3311, -3311, -3311, -3311, -3311, -3311, -3311, -3311, -3311,
   -3311, -3311, -3311, -3311, -3311, -3311, -3311, -3311, 14099, -3311,
   -3311, -3311, -3311, -3311,  2397,  7540, 12181, 12181, -3311, -3311,
   -3311,   408, -3311, -3311, -3311, -3311,  2387,  2388, -3311, -3311,
    2395,  2409,  2413, -3311,  2419,  1983,  1596, -3311, -3311, -3311,
   -3311, -3311, -3311,  1643, -3311, -3311,  2596, 10270,  1135,  1542,
    1542,  1542,  2524,  1542,  1542,  1542,  1542,  1542,  1542,  1542,
    1542,  1542,  1542,  2507,  1542,  1542,  2400,  2639,  2516,  2407,
    2599, -3311,  2523, -3311,  1939, -3311, -3311,  1334, -3311, -3311,
   -3311,  1334, -3311,  2411,  2620, -3311, -3311,  1474,  2571,   102,
    2436,  2682,   508,  2683,  2684,  2685, -3311, -3311,  2686,  2687,
   -3311, -3311,  2688,    62, -3311, -3311,   180,  2616,  2618,  2539,
    2434,  9724,  2455,  2455, -3311,  2600,  2699, -3311, -3311,  2462,
    1594,   529,    24, -3311,   929,  2449,   137,  2450, -3311,  1365,
    2463, -3311,  2467,  2568,  2464,  1542,  1542,  1542,  1542,  1542,
    1542,   812,  1977,  1222, -3311, -3311, -3311, -3311,  1544,  2451,
    2457,  2465,  2650, -3311,  2658,  2658, -3311,  5454,  1843,  3554,
    2070,  1843, -3311, -3311, -3311, -3311, -3311, -3311, -3311,  2468,
   -3311, -3311, -3311, -3311, -3311, -3311,  2454,   548,  2556, -3311,
   -3311,  2623, -3311, -3311,     6,  2574,  2565, -3311,    95,  2729,
   -3311,   875,  1666,  1199, -3311, -3311, -3311,  2489,  2489,   567,
    1748,  2724, -3311,  1062, -3311, -3311, -3311, -3311,  2527, -3311,
    2510,  1343, -3311,  2415, -3311,  2527, -3311,  2416, -3311,  1101,
    2809,  1510,  2417, -3311,  1356,  4672, -3311,  2510,  2510,  2510,
   -3311,  2510, -3311, -3311,  1723,  5121,  2439, -3311, -3311,  1613,
    1624,  2510,  1613, -3311, -3311, -3311, -3311, -3311, -3311, -3311,
   -3311, -3311, -3311, -3311,  2510, -3311,  2510,  2510,  2510,  2510,
    2510,  2510,  2510,  2510,  2510,  2510,  2510,  2510,  2510,  2510,
    2510,  2510,  2510,  2510,  2510,  2510, -3311, -3311, -3311,  1101,
   -3311,  2086, -3311,  3596,  1701, -3311,   136, -3311,  2051, -3311,
   15015, -3311, -3311, -3311,  2477, -3311,   586, -3311,  2479, -3311,
     295, -3311, -3311,  2504, -3311, -3311,   457,  2482,  2485,  2486,
   -3311,  2716,  2488,   499, -3311,   700,  2731,  2733,  2490,   616,
    2727,  1157,  2660,  1582,  1217,  2742,   705,  1542,   540,  1172,
    2499,  2502,  1003, -3311,  1542,  1542,  2739,  2760,  1542,  1542,
    2520,  2606,   775,  1542,  2547,   162,  2730,  1024,  1542, -3311,
    2612, -3311,  1542,  2530,  2350,  1542,  1542,  2621, -3311, -3311,
   -3311, -3311,  2503,  9451,  2641,   966, -3311,  9451, -3311, -3311,
   -3311, -3311,  2552, 14099,  1542,  2668, -3311,  1753, -3311, -3311,
     554,  1031, -3311, -3311, -3311,  2541,  2515,  2515, -3311, -3311,
    7813,  2601, 11908, 11908, 11908, 11908, 11908,  6717,  9451,  6503,
   -3311,  1365, -3311,  2734,  2558, -3311,   194,  1713,  1542, -3311,
   -3311, -3311, -3311,  1542, -3311, -3311, -3311,  2795, -3311, -3311,
    2797, -3311, -3311, -3311,  1542, -3311,  2559, -3311,  2652,  2753,
    2768,  1542,   330,  1031, -3311,  2576,  2578, -3311,   340,  1287,
    2581,  2573,  2575, -3311, -3311, -3311,  1182, -3311, -3311, -3311,
    1410, -3311, -3311, -3311, -3311,  1118, -3311,  2700, -3311, -3311,
   -3311, -3311, -3311, -3311, -3311, -3311,  2692, -3311,  2704,  1515,
    2706, -3311, -3311,  2738,  1010,  2714,  1438,  2717,  2719,   390,
     715,  2916,  2769,  1365,  1542,  1542,   411, -3311, -3311,  2697,
   -3311, -3311,  9451,  9451,   964, -3311,  2367, 11908,  1132, 11908,
    2605,   244,  1078,   283, 11908, 11908, 11908, 11908, 11908, 11908,
   12390, 12390, 12390, 12390, 12390, 12390, 12390, 12390, 12390, 12390,
   12390, 12390,   715,   416,  2736,  2831, -3311, -3311, 10270,   181,
    2785,   378, -3311,  1542,   955,  2762,   194,  1542, 14099,  2865,
    1542,  1542, -3311,  2732,    87, -3311,  2629,  2765, -3311, -3311,
   -3311,  2720,  2720, -3311, -3311, -3311, -3311,  2630,  2615, -3311,
    1147, -3311, -3311,   936,  2359,  2693,  2622, -3311,  2744,  2633,
    2457,  1542,  2625, -3311, -3311,     6,  2766, -3311, 11908,  2873,
   -3311, -3311,  9451,  9451, -3311, -3311, -3311,  1666,   922,  2779,
     567, -3311, -3311,  2759, -3311, -3311,  2789,  1027,  2642, -3311,
    1318, -3311, -3311, -3311, -3311,   173,  2510, -3311,  1101, -3311,
   -3311,  2610,  1543,  1360, -3311, -3311,  2619,  1770,  1915,  1992,
   -3311,  5038,  2151,  2510, -3311, -3311,  2040, -3311,  2161, -3311,
   -3311, -3311, -3311, -3311,  1684,  1684,  1306,  1306,  2191,  2191,
    2191,  2191,   145,   145,  1916,  2263,  2269,  2270,  2646,   100,
   -3311, -3311, -3311, -3311, -3311, -3311,  2624,  2626, -3311,  2608,
    2051,  5217,  1773, -3311, -3311,  2664,  2675,  2676, -3311, -3311,
    2679, -3311, -3311,  2781,  2782,  1542, -3311,  2665, -3311,  2783,
    2669, -3311,  2670,  2914,   139, -3311, -3311, -3311, -3311,  2671,
   -3311, -3311, -3311, -3311,  2773,   215,   162, -3311,  2673, -3311,
    2763,  2696,   531,  2698,   524,  2761,   154,  2833,  2825,  1619,
    2827,  2847,  2849,  1198,  1057,  1542,  2701,  2853,  2854, -3311,
   -3311, -3311, -3311, -3311,  2751,  2705,  2708,  2752,  1542,  2954,
   -3311, -3311,  2788, -3311, -3311, -3311, -3311, -3311,  2860,  2709,
    2828,   732, -3311, -3311,  2710,   402,   712,   175,  1335,   103,
     841,  2711,  2712,  2964,  2713,  2975,  2718, -3311, -3311, -3311,
   -3311,   711,  2737,  1051,  2721,  2722, -3311,  2909,  2741,  2910,
    2822,  2735,  2740,   869,  2743,   184, -3311, -3311, -3311, -3311,
   -3311, -3311,  2745,  2838,  2936, 12777,   162,  1654,  1542,  2861,
    2746,  2991,   852,  2747,  1542, 12977,  2839,  1735,   675, -3311,
     675, -3311,  2748,  2993,  2467,  2750, -3311, -3311,  1542,  2947,
    2467,  1542, -3311,  2980,  2869, 11908,   994, -3311, 11908,  2968,
   -3311,  3012, 14099, -3311, -3311,  2770, -3311,  2467,  1791, -3311,
    1442, -3311, -3311,  9451,  1031, -3311, -3311, -3311, -3311,  2749,
    2758,  2749, -3311,  1785, -3311,  1983,  1596,  1596, -3311, -3311,
    8086,  9451,  2915,  2780,  1814, -3311,  2784, -3311, -3311, -3311,
   -3311,  2786, -3311,  2395,  2792,  1720,   194,  1490,  2963, 11908,
   13164, -3311, -3311, -3311,  6083, -3311, -3311,  3001, -3311, -3311,
   -3311, -3311,  2764, -3311,  1542,  2772,  2774,  2794,  1474,  1031,
    2777,  2787, -3311,  2793, -3311,  2979,  2983,  2404, -3311, -3311,
    1419,  8359,  1542,  2791, -3311, -3311,  1490,  2999, -3311, -3311,
   -3311, -3311, -3311,  2887, -3311, -3311, -3311, -3311, -3311, -3311,
   -3311, -3311,  2890, -3311, -3311, -3311, -3311, -3311, -3311, -3311,
   -3311, -3311, -3311, -3311, -3311, -3311,  2893, -3311, -3311, -3311,
   -3311, -3311, -3311, -3311, -3311, -3311, -3311,  1542,  2911, -3311,
    2803, -3311, -3311, -3311,  1365,   809, -3311,   194,  2762,  2762,
   -3311,  2804, -3311,  2699, -3311, -3311, -3311,  2959, -3311,  3006,
   -3311, 11908, 11908,  3056,  1110, -3311, -3311, -3311,  1121,  1127,
   -3311, -3311, -3311, -3311, -3311, -3311,  1134, -3311, -3311, -3311,
   -3311, -3311, -3311, -3311, -3311, -3311,  7540, -3311, -3311, -3311,
   -3311, -3311, -3311, -3311, -3311,  2810, -3311, -3311, -3311, -3311,
   -3311, -3311, -3311, -3311, -3311, -3311, -3311, -3311,  2803,  1542,
    2997,  1542,  2889,  2988, -3311, -3311,    25, -3311, -3311, -3311,
    1542,   967,  2821, -3311, -3311,  1199,   967, -3311, -3311,  2824,
   -3311,  2937, -3311, -3311,  1818, -3311,  2826, -3311, -3311,  3061,
     530,  1542,  3002,  2966,  3060,  1735,  2808, -3311, -3311,  2813,
   -3311,  2799,   653,   724,   109,  1552,  3003,  2630,  2834,  1821,
   -3311,  2800,  2829, -3311,  1483, 11908,   -35, -3311, -3311, -3311,
   -3311,  1820, -3311,   131, 14099,   231,  1542,  1542,   546, -3311,
    1542, -3311, -3311,  2984,  2978, -3311,  1158, -3311, -3311, -3311,
   -3311, -3311,  2510,  1856,  1368,  1567, -3311,  1827,  1827,  1827,
   -3311, -3311,  2510, -3311,  2510, -3311, -3311, -3311, -3311,  2790,
   -3311,  2778, -3311, -3311, -3311, -3311,  2819,  2823,  2942,  3085,
    2830,  2938,  2950, -3311,  2837,  2840, -3311,  2841, -3311, -3311,
   -3311,  1517,  1522,  2842, -3311, -3311,  2956,    98,   562,  1662,
    2844, -3311,  1083, -3311,  1542,  1542,  2925,  2867, -3311,  1294,
    1654,  1542,  1542,   786, -3311,  3004,  1542,  2962,  1542,  3028,
    2870, 14099,  3020,  1542,  1542,  1542, -3311, 14099,  1542,  1542,
    1542,  2854, -3311,  1542,  1542, -3311,  1542, -3311, -3311,  1542,
    2294,  1542, -3311,  1542,   732, -3311,  1542, -3311, -3311,  2376,
   -3311,     9, -3311,    16,  2850,    16,  2850,  2928,  2858, -3311,
    2862,  2856,  1739,   204,  2863,  2917, -3311, -3311,  1542,  2866,
   -3311,  2967,  2990, -3311,  2871,  2750, -3311,  2872, -3311,  2872,
   -3311, -3311, -3311,  1437,  2876, -3311, -3311, 11908, 11908,  2875,
    1542, -3311, -3311, -3311, -3311,  2864,  2877,  1542,  2878, -3311,
     105,  2882,  3005,  1542,  2992,  2883,  2884, -3311,  1542,  1542,
      36,  1542,  1828, -3311, -3311, -3311, -3311,  2886,  3063,  1542,
   -3311, -3311, -3311, -3311, -3311,  1831, -3311, -3311, -3311,   916,
    3148,  2839, -3311,  3030,  3032,   675, -3311,   675,  2750,  1542,
     956,  2894,  1315, -3311,  3026,   346,  3150,  3157,   897,  3007,
   11908, -3311,  2869, -3311,  3115, -3311, -3311,  1542, -3311,  2919,
    1542,  3159,   929,  2668,  2922, -3311, -3311, -3311, -3311, -3311,
   -3311,  2924,  1844,  1845,  2930, -3311,  2970,  3000,  2923, -3311,
    8632,  9451,  6503,  3016,  1031,  2918,  2921,  2929,  1542,  3016,
   -3311, -3311, -3311, -3311,  1790, -3311,  1778, -3311,  3101,  3103,
   14099,  3025, -3311, -3311,  2515,  2515,  1555,  2932,  2933,   677,
   -3311,  1846, -3311,  1499, -3311,  2929,  1474, -3311, -3311, -3311,
    3045,   715,   715,  2946, -3311, -3311,   516,   194,  2794,  1474,
   -3311, 11908,  3139,  3189, 11908, -3311, -3311, -3311, -3311, -3311,
   -3311, -3311, -3311, -3311, -3311, -3311, -3311,  1848, 14099,  3129,
     715, -3311, -3311,  1599,  3130,  2927,  3078,  2770,   865, -3311,
     967,  1666,  1199,  1542, 13351, -3311, 14099,   875,  3154,  2931,
    1509,  3013,  1298,   530,  1542,  2955,  1542,  3086,  2940, -3311,
   -3311,  1542,  2943, -3311, -3311,   717,  2793,  1490,  9451, -3311,
   -3311, -3311, -3311, -3311, -3311, -3311, -3311, 11908, -3311, -3311,
    3011,  2829,  9451,  9451, -3311,  1542,  3104, -3311, 14099,  2969,
     567, -3311,  1726,  1542,  1542,  3199,   231, -3311, -3311, -3311,
   -3311, -3311,   805,  3135, -3311, -3311, -3311, -3311,  3093, 14099,
   -3311,  1087, -3311, -3311,  2116,  1827,  2117,  2088,  2105,  1392,
   -3311, -3311,  3283, -3311, -3311, -3311, -3311, -3311, -3311,  2952,
    2953, -3311,  2957,  3209, -3311,  2960, -3311, -3311, -3311,  2965,
   -3311,  2971, -3311, -3311, -3311, -3311, -3311, -3311, -3311, -3311,
   -3311, -3311, -3311, -3311,   465, -3311, -3311, -3311,  3015,  2976,
   -3311, 12977,  9451,  3145,  3077,  3149,  2987, -3311,  3018,  3050,
     980, 14099, -3311, -3311,  2995, -3311,  2797, 14099, -3311, -3311,
    2985,   486,  3086,  3092,  3096,  3097, -3311,  3082, -3311, -3311,
   -3311, -3311, -3311, -3311, -3311, -3311, -3311, -3311, -3311, -3311,
      78,  3140, -3311,  1542, -3311, -3311,  2994,  2850, -3311, -3311,
    2850,  2982, -3311, -3311, -3311, -3311, -3311,  3080,  2850, -3311,
    3008, -3311,  2996, -3311,  2998, -3311,  2986, -3311,    -9,  3010,
   -3311,  3021, -3311,   818,  3009, -3311, -3311, -3311, -3311, -3311,
   -3311,  2989, -3311,  3160, -3311,  3014, -3311,  3024,  3031,  3017,
    1542, -3311,  1542,  3029, -3311,   710, -3311,  3033,  3038,  2467,
   -3311, -3311, -3311,  3175,    36,  3039,  3019,  3044, 14099,  1542,
    3035,  3034,   956, 12977,  3246, 14099,   955, -3311,  3161,  3162,
     675,   385,   993,  2839,   447, -3311, -3311,  3040,  3036,  2750,
   -3311,  1384, -3311, -3311, 11908, -3311,  3079,  3041,  3043,   955,
    1542,  3106,   369,  3196, -3311, -3311, -3311,  3051, 14099, -3311,
    3068, 10270, -3311, -3311,  2970,  2970,  2915,  3204,  3070,  2915,
    3201,  2970, 14099, -3311,  1854,  3069,  2395,  3071,  1563,  9451,
    3230, -3311,  2515,  2515,  1566, -3311, -3311,  3067, -3311,  6293,
    3052,  3294, -3311,  1474,  1474, -3311,  1855,  3098,  2749,  2749,
    1031,  3058,  3081, -3311, -3311, -3311,  2014, -3311,  1542, -3311,
     715,    11, -3311, 14099,  3185,  3187, 12590,  3188, -3311,  3191,
    3284, -3311, -3311, 11908, 11908, -3311, -3311, -3311,  3095,   715,
     158, -3311, -3311,  2159,  2159,  1031,  3100,  1256,  3016, -3311,
   -3311, -3311, -3311, -3311,  1666,  3107, 14099, -3311,   975, -3311,
    3094,  3105, -3311,  3108,  3168, -3311,  3090, -3311,     6,   530,
    3287,  3176,  3216, -3311,  3119,  2839, -3311, -3311,  3114, -3311,
   -3311,  1708,  2929,  1858,  1483, -3311,  1153, -3311, -3311, -3311,
   -3311, -3311,   126,  1053,   231,  3102, 14099,  2489, -3311, 14673,
   -3311,  3111, -3311, 14099,   803,  1595,  3305,  3118, -3311, -3311,
    2984,  3046, -3311,  1827,  1827,  2119,  1827,  2129,  2175,  1827,
   -3311, -3311, -3311,  3281, -3311, -3311, -3311, -3311, -3311, -3311,
   -3311,  1542,  2839, -3311,  1542, -3311,  3127,  3124,  3131,  3133,
     141,  2987, 14099,  3241,  1542,  3134,  3211,  3373, 14099,  1400,
   14099, -3311,  1483,  1864,  1542,  3137,  3138, -3311, 14099,  1542,
    1542,  3116,  3195, -3311, -3311, -3311, -3311,  3136,  3122, -3311,
   -3311,  3121,  3128,  1542,  1542, -3311, -3311, -3311,   434, -3311,
   -3311,  2872,  2872, -3311, -3311, -3311, -3311,  1542, -3311,   413,
     695, -3311,  3205, -3311, -3311,  1542,  3256,  3125, -3311,  3258,
   -3311,  3387,  3267,  3152,  3175,  3243, -3311, -3311,  3143, -3311,
    3015, -3311,  3082, -3311, 14099,  2839, -3311,  3155,  1867, -3311,
   -3311, -3311, -3311, -3311,   993, -3311, -3311, -3311, -3311, -3311,
    3132, -3311,   985,  3144,  2382, -3311,  3147,  3151, -3311, -3311,
   -3311,  3351, -3311, -3311,   502,  3240,  2565, -3311,  1542, -3311,
    3245, -3311, -3311, -3311,   967,  3336,  2915,  2915, -3311,  3404,
    3313,  3153, -3311,  3178,  2915, -3311, -3311,  3179, -3311,  8905,
   14099, -3311, -3311, -3311,  2749,  2749,  1031,  3163,  3164,  1542,
   -3311,  3417, -3311, -3311, -3311, -3311, 14099, 10543, -3311, -3311,
   -3311,  2515,  2515,   677, -3311,  3190,    11,  3365, -3311, -3311,
    1871,  3193, -3311,  1883,  3194,  1256,  1542, -3311, -3311, 14099,
     158,  3167,  2515,  2515, -3311,  1620,  3197,  3200,  3202,   955,
    3203,  1530, -3311, -3311,   837,   604, -3311, -3311, -3311,  1884,
   -3311,  3198, 13351,  3230, 10816, 14099, -3311,  3295, -3311, -3311,
    3288,  3260,   530,     6,  3307, 14099, -3311,  1542,  3361,  9451,
   -3311, -3311, -3311,  9451,  3206, -3311, -3311, -3311,  1542,  1885,
    3208,  1561,  3374,  3321, 14673, 11089,  1542,  1843,  3207,   231,
    1542,  1542,  1542,  1542,  9451, 14673,  3212,  1542,  9178,  1843,
    2236,  9451,  1477,  3213,  3215,  3217,  3218,  3219,  3220,  3221,
    3222,  3223,  3225,  3226,  2352, -3311, -3311,  3210, -3311, 14450,
   -3311, -3311, -3311,  3229, -3311, -3311, -3311, -3311, -3311,  1542,
   -3311, -3311, -3311, -3311, -3311, -3311, -3311, -3311, -3311, -3311,
   -3311, -3311,  1661, -3311,   126,  3356,  3214, -3311, -3311,  3235,
   -3311,  1311,  3410,   567, -3311, -3311, -3311, -3311,  1827, -3311,
    1827,  1827,  2132, -3311, -3311, -3311,  3086,  1898, -3311,  3348,
    2839, -3311, 14099,  3241,  3241,  3241,  1899,  3192,  3304,  3050,
    1542,  3224, -3311,  3227,  1905,  3426,  3388,  3242,  3453,  1908,
   -3311,  2797, 14099,  3412, 11908, 11908, -3311, -3311, -3311,  1542,
    3228,  3236,  1542,  3335, -3311,  3244,  3247, -3311,  3382, -3311,
   -3311,  3021, -3311,  3231,  3233, -3311,  3480, -3311,  1542,   955,
    3248,  3044,  3463,  3238,  1912, -3311, -3311,  1914,  3498,   733,
    3339, -3311,   446,  3507,  1910,  3255,   757, -3311, -3311, -3311,
   11908,  3251,  1542,  3259,  3121, -3311,   502, -3311,  2059, -3311,
   -3311,  3396,  3282,  1598,  3130,  3130, -3311, -3311, 11908,  3515,
    3261,  1439,  3425, -3311, 14099, -3311,  1929,  2395, -3311, -3311,
   -3311,  2515,  2515, -3311, -3311, -3311, -3311,  1930, -3311, -3311,
    2749,  2749, -3311, -3311,  3239, -3311, 14099, 10543,   955, 10543,
    3285, -3311,  1546,  3290, -3311,  3167, -3311, -3311,  2749,  2749,
    2159,  2159,  1031,   727,  1542,   955,  3291,  1256,  3430, -3311,
    3430,  3430,  3458,  1542,  1542,  1542,  2315,  3527,  3378,  9451,
    3473,   286,  3275,  3289, 14099,  1542, -3311, -3311, -3311, -3311,
   -3311,  1542,  3265, -3311,     6,  3364,  3288,  1542,  1931, -3311,
    3306,  3308, -3311,  1483, -3311,  9451, -3311, -3311,  1455,  1089,
   14099,   955,  3421, 14450,  9451, -3311,  3414,  3530,  3418,  2187,
   -3311, -3311,  3568,  3422,    33,  2438, -3311,  3326, -3311,  3440,
   14574, -3311,  3330,  1589, -3311,  3333,  3445,  3334,   -26,  3500,
   -3311,  3341,  3342,  1542, -3311, -3311, -3311, -3311, -3311, -3311,
   -3311, -3311, -3311, -3311,  3343,  3344,  3439,  3550, -3311, -3311,
   -3311,  3347, 14099,  3352,  1542, 14099,  1595, -3311, -3311,  3599,
    1747,   231, -3311, -3311, -3311, -3311,  1827, -3311, -3311, -3311,
    1542,  1542,  3359,  1932,  3304,  3304,  3304, -3311,  3353,  3428,
   -3311,  3211,  1938, -3311,  3394, 14099,   733,  3447,  3369,  3372,
   -3311, 11908,  2797, -3311,  1483,  3509,  1941, -3311, -3311,  1950,
   -3311, -3311,  3546, -3311,  1247,  3355, -3311,  3354, -3311,  1542,
   -3311,  3517,  1542, -3311,  2565, -3311,  3015, 11908,   243, -3311,
   -3311,  1470,  3377,   955,  1542, -3311,  1165, -3311,  3617,   578,
   -3311,   967, -3311, -3311,  3358, -3311,   695, -3311, -3311,  1882,
   -3311, -3311, -3311,  3244, -3311, -3311,    16,  2850, -3311, -3311,
    3379, -3311,  3016,  3016, -3311,  3380, -3311, 11908,  1288,  3386,
   -3311, -3311,  3522,  3619, -3311, -3311, -3311,  2749,  2749, -3311,
   10543, -3311, -3311, -3311, -3311,  1952, -3311,  1969,  3194,  9451,
   11362,  3016, -3311, -3311, -3311,  2515,  2515, -3311,  3391,  3393,
    3395, -3311,  1593,  3389,  3399,  2315,  1530, -3311, -3311, -3311,
   -3311,  1256,   194,  1978, -3311,   399,  2762,  3392,  3384,   194,
   -3311, -3311,  9451,  9451, -3311,  3628,  3575,  3275,  3556,  3510,
   11908, -3311, -3311, -3311,  3385,  3288,     6, -3311,  3398,  3406,
    3545,  3578,  3241, -3311, -3311,  3397,  3401,  3416, -3311, -3311,
    3419,  3423,  2352,  3400,  3607,  3440, 11908,  3530,  3414, 14673,
    3609, -3311,  1542, -3311, 14673,  9451,  3427,  1542,  1542,  3454,
   -3311, 11908, -3311, 14673,   513,  3577, -3311,  1542, -3311,  9451,
   14099,  1542, -3311,  1542,  3415, -3311,  9451,  1005, -3311,  3439,
   -3311, -3311, -3311, -3311, -3311,  1668, -3311, -3311,  3489,  1657,
    1542,  1985, -3311, -3311, -3311,  1542,  3514, -3311, -3311, -3311,
   -3311,  3464,  3571, -3311,  3562, -3311, -3311, -3311,  2393,    38,
   -3311, -3311,  1542,  1542,  3470,  1986, -3311,  1290,  3670, -3311,
   -3311,  3442, -3311, -3311,  2925,  3608, 11908,  3611,  3610,  1542,
    1542,  3429, -3311, -3311,  3432, -3311, -3311, -3311, -3311,  1297,
   -3311,   282, -3311,  3434,  3435,  3444,  3591, -3311, -3311, -3311,
   -3311,  3457,   670, -3311, -3311,  1979, -3311, -3311,  3680, -3311,
    3455, -3311,  1542,   604,   604, 11908,  2829, -3311,     7, -3311,
     945, -3311,  2343, -3311,  3690, 11908, -3311, -3311, -3311, -3311,
   -3311, -3311,  3547, -3311,  2022, -3311, -3311,  3230,  2749,  2749,
    9451,   194,  6503,  3441,   194,   194,  1610, -3311,   194,  1542,
    1542, -3311,  2315,  3443,  3511,  3466, -3311, -3311, -3311,  5316,
    9451, -3311, -3311, -3311, -3311, -3311, -3311, -3311, -3311,  3288,
    2966,  3572,  1542,  2025, -3311,  3446,    38,  3471,  3472, -3311,
    3573,  3460,   137,  1542, -3311, -3311,  3440,  3668, -3311, 14673,
    3705,  3467, 14450, -3311, -3311,  1851,  3648, 11908,  3485,  3500,
     683, 14673, 14673,  9451,  9451,  3677, -3311, -3311,  3475,  3476,
    1656,  3477, -3311,  3481,  3478,  3440,  3440,  3624, -3311, 14099,
    3721,  3483,  1542, -3311,  3486, 14099, -3311, -3311,  3491, -3311,
    1542,  1542,  2036, -3311,  1542, -3311, -3311, -3311,  3479, -3311,
    1542, -3311,    38, -3311, -3311, -3311,  1542, -3311, 14099,  3359,
    3496, -3311, -3311,  3499, -3311,  3501,  3482,  3602,  3495, -3311,
   -3311,  9451,  3598,  3653, -3311,  3671,  1483, -3311, -3311,  3508,
    3512, -3311,  1542,  2039, -3311,   733,  3140, -3311,  2028, -3311,
   -3311, -3311, -3311,    16, -3311,  3473,  3473, -3311,  3751, -3311,
    3488, -3311, -3311, 11908,  1505,  2048, -3311,   382,  3547, -3311,
   -3311, 11362, -3311, -3311, -3311,  2050, -3311, -3311,  1733,  3516,
   -3311, -3311,  9451, -3311, -3311,  2762,   194,  3518,   391, -3311,
   -3311, 13538,  3519,  3520,  3521, -3311, -3311,  3524, -3311,  3603,
   -3311, -3311, -3311,  1542,  2054, -3311,  3657, -3311,  3545, -3311,
   -3311, -3311, -3311, -3311, -3311,  3523, -3311,  3753,  3525, -3311,
    3717, -3311,  1542,  3529, 11908,  1542,  6994, 14673,  3440,  3440,
    3701, -3311, -3311, -3311, -3311, -3311, -3311, -3311,  1542, -3311,
    1542, -3311,  3531, -3311, -3311,  9451, -3311,  2839, -3311,  3571,
    2794, -3311, -3311, -3311, -3311, -3311, -3311,   733,  3509,  3509,
   -3311,  1542,  1542,  3532,  1483, -3311,  1483,  3241, -3311, -3311,
    1242, -3311,  3591,  1293, -3311, -3311, -3311,  3575,  3575,  1174,
   -3311,  2060, -3311, -3311, -3311, 11908,  3513,  3533,  3632, -3311,
   -3311, -3311,  6503,   194, -3311, -3311, -3311, 11635, 13725,  3443,
    3703, 14099, -3311,  3720, -3311, 11908, 11908,  5601,  5316,  3490,
   -3311,  3635, -3311,  3572,  1542, -3311, -3311,  3534, -3311,  1542,
    1877, 11908,  3574,  3536,  1404,  2066,   513,   513,  3537, -3311,
   -3311,  1542,  3539,  2839, -3311,  3683,  1331,  2925,  2925,  3541,
   -3311, -3311,  3241,  3241,  3304,   583, -3311, -3311, -3311, -3311,
   -3311, -3311,   792, -3311,  2345, -3311, -3311, -3311, -3311,  1029,
   -3311,  3544, -3311,  3552,  3553, 14099, -3311, -3311,  3555,  3557,
    2090, -3311, 13538,  2091, -3311, -3311,  2092,  7267, -3311,  2098,
   -3311, -3311, -3311, -3311, -3311,  3691, -3311,  3589, -3311,  3549,
   -3311,  3574, 11908,  3500, -3311,   829, -3311, -3311, -3311, -3311,
   -3311,  1123, -3311,  3359,  3558,  3559,  1542,  3304,  3304, -3311,
    3015,  3663, 11908, -3311, -3311, -3311,  2946,  3673, -3311,   194,
     194,  3566,  2331, -3311, -3311, 14099,  3740, -3311, 11908, -3311,
   -3311, -3311,  5601,  1542,  1542, -3311, -3311,  3500, -3311,  1542,
      85,    58,    58, -3311,  3509,  3509,  3706, -3311, -3311, -3311,
    3570,  2101,  3660, 13351, -3311, -3311, -3311,  1351,  1351,  2108,
   -3311,   194, -3311, -3311,  3576,  3579, -3311, -3311, -3311,  3661,
   -3311,  1542,  3563, -3311, -3311,  3551, -3311, -3311, -3311,  2925,
    2925,  1542, 11908,  3015,  3583,   975, -3311, -3311, -3311,  2331,
   -3311, 13912, -3311,   584,  3580, -3311, -3311,  3584,  3585, -3311,
    2111, -3311, 10543,  3230, -3311, 14099, -3311,  2135, -3311,  3814,
   -3311, -3311,  3681, 11908, -3311, -3311, -3311,  3015,  2137,  3792,
    2138, -3311, -3311, 13912,  2379,  3588,  2139, -3311,  3016,  3016,
   -3311, -3311, 14099, -3311,  2331, -3311, -3311, -3311,  3593,  2251,
    2158, -3311, 11908, -3311, -3311,  3230, -3311,  2162, -3311,  2331,
    2331,  2331,  2331,  2331,  2163, -3311, -3311,  2331,  2251,  2158,
    2158, -3311, -3311, -3311, -3311
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
   -3311, -3311,  3885, -3311, -3311, -3311, -3311,   214, -3311,  -284,
    1832,  1839,  1782,  1835,  3141,  3142,  3146,  3156,  3158,  3169,
    -102,  -269, -3311,  -351,  -264,   152,  1408, -3311, -3311,  3606,
   -3311,    71,  -203, -3311, -3311, -3311, -3311,  3827,  1553, -3311,
   -3311, -3311,  2811, -3311,  3811,  3528,    -3,    22, -3311, -3311,
     -10,    54, -3311, -3311,   -86, -3311,  3165,  -208,  -642,  -615,
    -676, -3311,  -332, -3311,  1502,  3564, -3311, -3311, -3311, -3311,
   -3311, -3311, -3311, -3311,  3834,    32,  3590,   703, -3311, -3311,
   -3311, -3311, -3311, -3311, -3311, -3311, -3311,   -68, -3311, -3311,
   -3311, -3311, -3311, -3311, -3311,  3264, -3311, -3311, -3311, -3311,
   -3311,  3252, -3311,  3357, -3311, -3311, -3311, -3311, -3311, -3311,
   -3311,  2880, -3311, -3311, -3311, -3311,  2958, -3311, -3311, -3311,
   -3311, -3311, -3311, -3311, -3311, -3311, -3311, -3311, -3311, -3311,
   -3311,  2341,  2347, -3311, -3311,   183, -3311, -3311,  1838, -1845,
   -3311, -3311, -3311, -3311, -3311,  3855,  1215,  3856,  -166,  1236,
   -3311, -3311,  3298,  -149, -3311, -3311, -3311, -3311,  1507, -3311,
    3112, -3311, -3311,  3123,  3183,  -708, -3311, -3311, -3311, -3311,
   -3311, -3311, -3311,  3815,  2973, -1018,  2076,  1445, -3311, -3311,
   -3311, -2100,  1175, -3311, -3311, -3311, -3311, -3311,  1225,  1228,
    2252, -3311, -3311, -1690, -2049, -3311, -3311, -3311, -3311, -1721,
   -3311, -3311,  2243, -3311,  1581, -3311, -3311, -3311, -3311, -3311,
   -3311, -3311, -3311, -3311, -3311,   707, -3311, -3311, -3311,   435,
   -3311, -3311, -3311,  3170,  -537, -3311, -3311,  1547, -3311, -3311,
    2289, -3311,  1056, -3311,   634,  1310, -3311,   830, -3058, -3311,
    1075, -2887, -1368, -3311,  1330, -2016, -3311, -3311,  2775, -3311,
   -3311, -3311, -3311,  2726, -3311, -3311, -3311, -2594, -3311, -3311,
   -3311,   558, -3311, -2818,   721, -3311,   656, -3311, -1652,  2453,
   -3311, -3311,   532, -2641,   857, -1197, -1265, -2046,   560, -1804,
   -2846, -2917, -3311,  1889, -1169, -2683, -3311,  1997, -1255,  1895,
    1622, -3311, -3311,   856, -3311, -3311, -1818, -3311, -3311, -1886,
     254, -3311,  -262, -1903,   854, -3311, -1051, -2054,  1323, -3311,
   -3311, -3311,  -799, -1253, -3311,  2122, -2364, -3311, -3311, -3311,
   -3311,  3917,  2244, -3311, -3311, -3311, -3311, -3311, -3311, -3311,
    -822,  1090,  1284, -3311,  2588, -2732,  1034, -3311,  1028,  1314,
    1349, -3311, -3311, -3311, -3311, -3311, -3311, -3311,   375,  1508,
   -2148, -3311,  1235,  1361, -3311,   663, -3311, -3311, -3311,  1816,
   -1897,  1377, -3311, -3311,   662, -1052, -2373, -3311, -1244, -1464,
   -3311, -3311,  2525,  2247,  2144,  -849,  3893,  2141, -3311,    91,
   -3311, -3311,  3023,  3370, -3311,  -111, -3311,  -763,  2250, -1021,
   -3311, -3311,  2694, -1043,  -127, -3311, -2411, -3311, -2797, -3311,
   -3311,   545, -3311, -3311, -3311,   356, -3311, -3311,   474, -3311,
   -2516, -3311, -3311,   325, -3311, -3311, -2445, -3311, -3311, -2089,
   -3311, -3311,   300, -1138, -2439,  -935, -3311, -3311,   831, -3311,
   -3311,  -164, -3257,   547,   424, -3311, -3311,   423,   426, -3311,
     552, -3311,  -847,  -547,  1017, -3311,  1270,  1025,  -675, -2537,
   -2197, -3311, -1079, -3311, -2496, -1102, -3311, -1584,  2136, -2284,
   -3311, -3311, -1864, -2716, -1858,   -19, -3311,  3099,  3091,  3474,
   -3311, -3311, -3311, -3311, -3311, -3311, -3311, -3311, -3311, -3311,
   -3311, -3311, -3311, -3311, -3311, -3311, -3311, -3311,   633,  1007,
    -150, -3311,  3232,  1767,  1779,  1128, -3311, -1346, -3311, -3311,
    2776,  2284,  2796, -3311,  3234,  -963, -3311, -3311,   585, -1873,
   -3311, -3311,   883, -3311, -3311,   881, -3311,   594,  -919,   343,
   -3311,  2281,  -129, -3311, -3311, -3311, -3311, -3311, -3311, -3311,
   -1093,  -704, -3311,  2145, -3311, -1984,  1461, -2987, -2539, -1826,
   -3311,  -598, -2965, -1710, -2556, -3311, -1837,  1202, -3311, -2229,
   -3311,  1523, -3311,  2142, -3311, -3311, -3311, -3311, -3311,  -991,
   -3311, -3311, -3311,   788, -2734, -3311, -3311, -3311,   982, -3311,
   -2983, -2524, -2563, -3311, -3311, -3311, -3311, -3311, -3311,  -870,
   -3311, -2938, -3311,  1257, -3311,  1018, -3311, -3311,  1020, -3311,
   -2806, -3311, -3311, -3311,   498, -3311, -3311, -3311, -3311, -3311,
   -3311, -3311, -3311, -3311, -3311, -3311, -3311, -3311, -3311, -3311,
   -3311, -3311,  2851, -1196, -3311, -3311, -3311, -3311,  2835, -3311,
    1987,  1989, -3311, -3311, -3311, -3311, -3311, -3311, -3311, -3311,
   -3311, -3311, -3311, -3311, -3311,  -480,  1954, -2081,  2019,   923,
   -3311,  1958, -3311,  1970, -3311, -3311, -2064, -3311, -3311, -1687,
    1729, -3311, -3311, -3311, -3311, -3311, -3311, -3311, -3311,  1802,
   -3311, -3311,  1227, -3311, -3311, -3311, -3311, -3311, -3311, -3311,
   -3311, -3311, -3311, -3311, -3311, -3311, -2073, -3311, -3311, -3311,
   -3311, -3311, -3311,  1394, -3311,  1148, -3311,   928, -3311, -3311,
     386,  2563, -3311,  -551,  3062,  1665, -1325, -3311, -2164, -3311,
   -3311,  -902, -3311,  -880, -1353, -3311, -2327, -3310, -3311, -1214,
   -3202, -3311,   664, -1030, -3311
};

/* YYTABLE[YYPACT[STATE-NUM]].  What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule which
   number is the opposite.  If zero, do what YYDEFACT says.
   If YYTABLE_NINF, syntax error.  */
#define YYTABLE_NINF -1975
static const yytype_int16 yytable[] =
{
     563,  1373,   652,  1600,   754,  1428,   964,  2177,  1384,  2267,
    2338,  2238,  1562,   764,  2109,  1371,  1847,  2227,  1849,  2207,
     750,  1981,  2270,  1571,  1120,   412,   663,  1615,  2406,  1706,
    1365,  1366,    68,  2513,  1548,  2225,  2847,  1553,  1609,    83,
    2590,   577,   416,   104,  1582,   600,  2110,  1859,  1117,   102,
     636,   637,   638,  2118,   403,   111,  1359,  1129,  2409,   654,
    2391,  2409,   659,  1816,  2897,  1688,   978,   734,  2431,  2409,
    1808,    60,  2133,  1313,  2969,  3045,  2919,  3047,  3194,  3220,
    2873,  1820,  2899,  1424,    68,   747,    68,  1827,   129,  2311,
      68,  2890,   785,  3141,   117,   332,  2694,  1938,    68,  2944,
    2945,  2946,  1942,  2475,  1841,  2598,  2347,   105,  2710,  1396,
    3187,  1895,   711,  2449,   132,  2539,  1843,   347,  2539,  2552,
    2553,  2772,  2769,  2745,  2478,    60,  3175,  3176,  3177,  3305,
      60,    60,  1619,  1817,  2812,    60,  3372,  2470,  2914,  3114,
    3097,   744,  1824,    22,  3318,    22,  1266,   644,   727,   728,
     729,  2332,    68,    22,   401,    22,   766,   409,   822,   803,
     803,   860,  3315,    22,  3337,   132,   730,  3506,  2183,   342,
     342,  1187,     3,     4,     5,    22,   331,  3304,     6,     7,
    2551,     9,    60,  2103,    10,    11,   655, -1087,    13,    14,
     601,  1487,    16,    60,    60,    18,    19,    20,    21,   141,
      22,  1190,  2045,  3723,  2404,    23,   793,   794,   832,   980,
    3596,  1108,    22,  3127,  2100,  1744,   119,  3021,   773,   774,
     775,   776,   777,   778,   779,   780,   781,   782,    24,    25,
      26,    27,    28,    29,    30,    31,    32,    33,    34,    35,
      36,    37,    38,    39,    40,    41,  1935,   412,    42,    43,
      44,  1307,  1978,   767,   768,   769,    45,    46,    47,    48,
      49,    50,  1580,   119,  2593,  1665,   690,  1009,  3265,  2586,
    3220,  1054,  2458,   671,   391,   404,  2241,  2537,  1683,  1737,
     221,  1844,  1979,  3237,  1012,  3724,   846,   847,   848,   849,
     861,   852,   853,   835,   855,   856,  2161,  1220,  1666,  1569,
    3628,  1102,  2416,  2417,  1400,  3209,  1986,  3358,  3359,  1010,
    2919,  2147,  3724, -1070,   874,   875,   876,   877,  1982,  1060,
     412,  1940,  2067,   928,  1554,   449,  2686,  2919,   403,  3140,
    3473,  1672,   347,  1555,    80,  3115,  1983,   894,   981,   743,
    1987,   411,    91,  2498,  2162,    92,  2502,  1221,  1505,  2735,
    3699,  1506,   450,  1863,  2729,  3360,  1222,  1310,  1061,   982,
    1013,  1223,  2418,  2148,  1902,  1993,  1691,    68,  3466,  1745,
    1667,  2104,  1684,  3239,    68,  2119,  2513,  1103,  3240,  3288,
    2101,   723,   840,    93,   401,   706,   752,  1513,  3128,   707,
    1514,  2822,  2757,   342,  2227,  1055,  1936,  3486,  3487,   342,
    1143,   142,  1224,   705,   764,   816,  1147,  1148,  1149,  2748,
     764,  1684,   815,   331,  2837,  1011,  3066,  1139,   765,   983,
    1158,  2794,   884,  3751,   862,  3628,  1199,   936,   401,  3725,
    2565,   941,   942,  1905,   944,   945,   946,   947,   948,   949,
    2573,   951,  1581,   953,  1906,  2581,   602,    68,   833,  2405,
    2815,  3693,   132,   554,   656,   834,  3725,  3777,  1907,  1908,
    1427,  1852,  1853,   603,   734,  1492,  1109,    60,   554,   555,
    1738,  1692,   183,   984,  1746,  3436,  1132,  1181,  1418,   430,
    1312,  1616,  1507,  2594,  1846,  2704,   331,  3572,  2382,  1536,
    1536,  1536,  1536,  1536,  1536,  1536,  1536,  1536,  1536,  1536,
    1536,  1157,  1161,  1162,  1163,  3579,  2664,  3570,  3296,  1187,
    1668,  1902,  1891,  3475,   406,  1160,  1072,  1073,   431,  1075,
    2455,  1515,  1094,    52,   804,  1096,  1644,  1669,  3209,  3654,
    3655,    52,  1477,  1225,    82,  1131,  1180,  1190,    53,  1883,
    3576,  3577,  1137,   813,   114,   814,   635,  1146,   391,  3523,
    1189,  2747,  1816,   635,  1722,   653,  3548,   657,  1549,  1418,
     662,   664,  1401,   411,   554,   555,   795,   796,    52,  1673,
     756,  3021,  2106,  3451,  2142,    53,   762,   763,  3470,  3103,
      52,  1674,  3536,   910,  3641,  2839,  3313,    53,  -417,   108,
     411, -1021,   528,  1203,   338,  3309, -1021,  2489,  2395,   740,
    3312,   554,   555,   783,  1508,  1509,  3220,  1927,   770,  3321,
     771,  1569,   772,   708,  2866,   999,  3301,   911,  2180,  2733,
    3206,  3587,  3588,  2776,  2777,    94,  2883,  2007,  3224,  2636,
   -1022,  2783,   536,    95,    96, -1022,   411,  2471,   412,  2472,
     538,  1275,  2322,  1516,  2942,  1733,  1557,   664,  1683,  1204,
    2471,  2100,  2472,  1684,  2265,  3688,  3055,  3056,  3044,  3207,
    3266,  3586,  1903,  3322,  3323,  2921,  1200,   827,  2367,  3220,
    2368,  3747,  3748,  1201,   864,  3007,  3404,  1121,  2195,  3000,
    3193,  1140,  2858,   664,   664,   664,  1276,  2449,  2227,  1675,
    3003,  3005,  2188,  2762,  1940,  2963,  1052,  1419,  1940,  2006,
    2047,   664,  3168,    69,  1277,   731,   828,  1420,  1274,  1421,
     554,   555,   412,  1060,   333,   605,  1299,  2196,  2855,   432,
    3422,  3660,  3761,  2048,  1314,   420,    84,  1000,    85,   598,
     334,   433,  2837,  1334,  1053,  2731,  1685,  1340,  2189,  1211,
     964,  1182,   421,  2316,  3549,  2396,  2919,  1196,  1122,  2919,
    1358,  1220,  1061,  3537,  3270,  1152,  2490,    69,  2919,  1221,
     554,   555,  1949,  1364,   606,  1001,  2734,  2864,  1222,  3661,
    3762,  2763,  1558,  1734,  1205, -1974,  3659,  1278,   551,  1675,
    1279,   401,  1560,  2100,  3478,  3479,   554,   555,   556,  2943,
     557,  1634,  2895,   865,  1406,  1313,  1616,   912,  3477,   649,
    2477,  1154,  1938,   816,  1159,  1411,  1630,  3729,  3730,   765,
     102,  2049,  1417,  2473,  2764,  1223,  1387,  2999,  2474,  3324,
     845,   664,   664,   664,   664,  1002,   664,   664,   854,   664,
     664,  2369,   858,   859,  1892,  1197,   331,  3689,   129,  3697,
    3698,   401,   870,  1184,  2838,    68,   599,  1427,    68,   664,
     664,   664,   664,  1320,  2268,  1961,  1224,   880,   881,  2370,
    1478,   883,  2293,  2266,  2269,  1488,  1489,  1882,  2397,  1640,
    1212,  -912,  1629,  2765,  1344,  2100, -1598,  1499,  1351,  1503,
    2508,  3721,  1011,  2261,  1517,  1518,  1519,  1520,  1521,  1522,
    1941,  1679,  2375,  1388,  2636,  2528,  2529,  2334,  2471,  1060,
    2472,  1950,  1680,  2839,  1550,  2636,  2441,   829,   936,  1385,
    2636,  1280,  2280,  2451,  2919,  1816,  3021,  1307,  1808,   830,
     607, -1974,  2310,  3594,   902,  3227,  3223,  1268,  1651,  2636,
    3662,  3763,   608,  2050,  1479,  1727,  1213,  1728,  1061,   609,
     610,  2224,  1269,   912,  1308,   611,  3192,  2435,  1962,  3222,
    2013,  1760,  1598,   612,   411,  1735,   151,   850,  1604,  1389,
    2471,  2341,  2472,   937,   939,   940,   664,   664,  2409,   664,
     664,   664,   664,   664,   664,   950,   664,   952,   664,   528,
     955,   956,  2248,  2249,  2231,  1020,  1021,  1225,   888,  1484,
     969,   717,  1324,  1309,  1359,   161,  1270,  1782,  3657,  3658,
     411,   411,   411,   411,   411,   411,   411,   411,   411,   411,
     411,   411,   411,   411,   411,   411,   411,   411,   411,   536,
    3768,  3701, -1600,  1310,  1963,  1782,  1311,   538,   411, -1452,
    3476,  3058,   928,  1056,   412,  3621,   835,  1231,   929,  3104,
     836,  3396,   837,  1358,   717,   664,  2665,   554,   555,  1569,
    2994,   664,   664,  1074,   664,  1076,  1077,  2070,   850,   838,
    2513,  1390,   420,  2636,   719,  2422,  2474,  3663,  2255,  1392,
    1631,  1214,   851,   173,  1057,  1560,  1271,  2321,  3017,   823,
     839,  2436,  1761,  1736,  1748,   554,   555,   356,  1680,  2342,
     412,  3750,  1729,  1607,  1608,  1965,  1711,   554,  3059,   720,
     360,    22,  3517,   361,  2995,  1649,   348,   349,  2220,   170,
    1749,   350,   351,   352,   362,   363,  1348,   719,   353,   354,
    1800,   889,  3776,   355,  2459,   840,   356,  1783,  3006,   357,
      81,  1254,   743,  2071,  1320,   358,  1255,   890,   359,   360,
      22,  1152,   361,   170,  1348,  1256,  1312,  2486,   891,  3397,
     841,  3804,   720,   362,   363,  2200,  1561,  3331,  3410,  3713,
    1078,  2788,  2789,  2636,  3233,  3234,  1621,    68,  1321,  1796,
     401,  2421,   964,  1593,  2666,   174,  1764,  1322,   649,  2833,
    2636,  1707,  1510,   356,  2834,  1511,  1708,   277,  2174,  1358,
    2495,  1319,  2835,  3257,  1765,  1832,   360,    22,  1835,   361,
      51,  1358,  1320,  1766,  1767,  1622,  2580,  3666,  1336,  1337,
     362,   363,  1750,  2291,  1915,  2566,  3769,  1916,   842,  3409,
     136,  1560,   182,  1801,   137,  1918,  2348,  1470,  1919,   893,
    3592,  1921,  3593,  3711,  1922,  1751,  1321,  1257,  1924,  2229,
    1293,  1925,  2536,   664,  2055,  1322,  1802,  1842,   843,   936,
    1298,   664,  2387,    88,  1302,  1303,   182,  1345,  3805,   664,
    1445,  2550,  3690,  2175,  1332,  2349,  1323,  2294,   664,  1709,
    1610,  1339,   664,  1324,   649,  2129,  1258,  1281,  1282,  1374,
     684,  2014,  2016,  1623,  2019,  3667,  1294,  1283,  1471,  1354,
    1357,  1768,   554,   555,  3335,  3231,   277,   554,   555,   277,
    3711,  3691,  1561,  2596,  1710,  2020,  2021,  2022,  1500,   702,
    3771,  3410,   227,  3329,  1561,  1395,  1512,   664,  1284,  3465,
    3334,  1501,  1403,  3524,  1405,  1285,  1295,  2388,  2572,   664,
    2514,  2515,  2056,   229,  1323,  1495,  1446,  3796,  2181,  3108,
     664,  1324,  3343,  2350,  1845,  2259,  2597,   664,  1917,  1496,
     411,  1912,  1913,  1447,  1430,  1254,  2754,   734,  2271,  1920,
    1255,  1448,  1436,  3811,  3812,  1923,  1449,   411,  1591,  1256,
    2530,  1325,  1926,  2023,  1296,   554,   555,  2755,   717,  3692,
    1450,   278,  3109,  1534,  1534,  1534,  1534,  1534,  1534,  1534,
    1534,  1534,  1534,  1534,  1534,   664,  1482,   635, -1974,   664,
     664,   664,   869,  1592,  1451,  2554,   411,   718,    58,  2636,
    1705,   279,   938,  1502,  2636,  2062,  1452,  2636,  3381,  1322,
     912,   227,  1769,  2636,   227,    89,  1535,  1535,  1535,  1535,
    1535,  1535,  1535,  1535,  1535,  1535,  1535,  1535,  1482,   664,
    1437,  3036,   229,  3236,   937,   229,   280,  3041,  3042,  1559,
    3601,   719,  1403,   662,  1354,  1604,  1577,  1578,   284,  3199,
    2063,  1257,    58,  1286,    90,  3053,  3054,   112,   113,   364,
    1231,  2008,  2938,  2309,   285,   365,   366,   367,   368,   369,
     370,  2856,  2994,  1287,  1288,  2064,   720,   664,  3382,  1453,
     278,  3573,   733,   278,  3200,   554,   555,  3383,  3384,  2556,
    1258,  3595,  1259,   703,  1613,   554,   555,  2826,   364,  2065,
    1569,   356,  3385,  3219,   365,   366,   367,   368,   369,   370,
     279,  3354,  2636,   279,   360,    22,   649,   361,  1232,   371,
     649,   118,  1233,   704,  2836,   356,  2995,  2591,   362,   363,
    3239,   805,  2024,  2927,  2592,  3602,  2705,  2066,   360,    22,
    3386,   361,  1523,   649,  2928,   280,  2513,   806,   280,  2371,
     649,   649,   362,   363,  3237,   364,   121,   284,   356,  3238,
     284,   365,   366,   367,   368,   369,   370,  2097,  2557,  2558,
    2931,   360,    22,   285,   361,  3639,   285,  2012,   733,  1081,
    2786,  1658,   356,  1082,  1978,   362,   363, -1599,  2513,  2636,
    2706,  2707,  2636,  2559,  1740,   360,    22,  2994,   361,  2187,
    2994,  2636,  2636,   121,  3585,  1854,  3532,  2139,  2140,   362,
     363,   717,  2188,  1741,  1979,   405,   554,   555,  1438,  2800,
    2801,   664,  1112,  2602,  1431,  1113,  2790,  2708,  3369,  2159,
    2160,  3598,  1432,   898,  1720,  1439,  3533,  1742,  2994,   356,
    2818,  2819,    22,  1440,  3239,   649,   649,  1457,  1441,  3240,
    2534,  2995,   360,    22,  2995,   361,  2355,  2357,  2189,   401,
     401,   401,  3580,  1473,   908,   909,   362,   363,  2187,  3653,
    2204,  1474,  3526,  2352,  3246,  3247,   971,  2984,   972,  1524,
    1525,  2188,  2523,  2524,   719,  3798,  1442,  2227,  2828,   133,
     973,  1354,  2995,   712,   664,  3722,  1743,  2829,  1443,  1250,
    1804,  1354,  1064, -1299,  2828,  2830,  1527,  1528,  3814,  1529,
    1530,   789,   790,  2829,   969,  1251,   356,  1828,   713,   720,
    1626,  2830,    86,  1458,  1252,   554,   555,  3744,  1839,   360,
      22,  2306,   361,  1683,    87,   649,   649,  1627,  1696,   134,
    1459,  3049,  3095,   362,   363,  1133,  3046,  1460,  1461,   135,
    2831,  2272,  2636,  1462,  2275,  2307,   927,  2636,   805,  3702,
    1025,  1134,   805,  3064,  1889,  1026,  2831,  1463,  2828,  2177,
     805,  1444,  1403,  1027,  1145,   937,  1354,  2829,  1635,   138,
    1874,  2134,  2667,  1464,  1028,  2830,  2017,  1221,   356,  2668,
    1877,  1465,   678,  2135,   805,  2136,  1222,  3105,  2137,  3106,
    1697,   360,    22,  1466,   361,  3432,   679,   139,  1893,  3112,
    2358,  3107,  3213,  3030,  3214,   362,   363,  1604,   140,  3531,
    3031,  1698,   348,   349,  1699,  2900,  3215,   350,   351,   352,
    2831,   551,  1385,   327,   353,   354,  1431,   554,   555,   355,
    1124,   556,   356,   557,  2258,   357,  1112,  1864,   411,  1113,
    3061,   358,  1125,  1900,   359,   360,    22,  1865,   361,  1866,
     664,  2039,  1700,  1403,   328,  1029,  2041,  3753,  1813,   362,
     363,   356,  2116,  2040,  2240,   329,  1467,  1814,  2042,  3037,
    3038,  2117,   739,   364,   360,    22,  2241,   361,   739,   365,
     366,   367,   368,   369,   370,  3423,  3424,  -520,   362,   363,
    -574,  1083,   554,   555,   753,  1084,  2870,   364,  2510,  1419,
    3794,  3795,  2250,   365,   366,   367,   368,   369,   370,  1420,
     346,  1421,  2251,  2516,  2252,  1929,  1431,  1931,  1142,  1030,
     927,  3217,  3057,  2517,  3137,  2518,  1357,   925,  3262,  2324,
     364,   410,   926,  1031,  1032,  1033,   365,   366,   367,   368,
     369,   370,   330,  2328,  2329,  1864,  2281,  1952,   554,   555,
    2282,  1633,   797,   798,   364,  1865,  2283,  1866,  2284,  3166,
     365,   366,   367,   368,   369,   370,   422,  2281,  3167, -1452,
     927,  2282,   928,   425,  3289,  2018,   649,  2820,   929,  2821,
    1992,  2002,  2003,  2005,  2335,  1861,   664,  2682,  1431,   424,
     928,  3339,  3342,   649,   649,  1431,  2922,  1861, -1607,  2892,
    2452,  2676,  2677,  3339,  2679,  1765, -1598,  2683,  2051,  3267,
    2052,   364,  1155,  2053,  1766,   428,  3276,   365,   366,   367,
     368,   369,   370,  2378,  2481,  3271,  3272,   401,  1537,  1538,
    1539,  1540,  1541,  1542,  1543,  1544,  1545,  1546,  1547, -1452,
     927,   936,   928,  2046,   649,   451, -1453,  -574,  1404,  1861,
    2057,  2058,  -520,  2335,  2051,  1862,   426,  2068,  2069,  2053,
    -574,  2336,  2073,   427,  2075,  2496,  2497,  1354,  3542,  2080,
    2081,  2082,  2504,  1354,  2084,  2085,  2086,   429,   786,  2088,
    2089,  1362,  2090,   787,   788,  2091,  1363,  2095,   364,  2096,
     452,  1188,   664,   356,   365,   366,   367,   368,   369,   370,
    2696,  1183,   454,  2547,  2548,    22,   360,    22,  2709,   361,
     356,  1150,   481,  1850,  2124,   805, -1453,  -574,  1363,  1861,
     362,   363,   639,   360,    22,  2239,   361,  1969,  1970,  1971,
    1972,  1973,  1974,  3258,  3259,   575,   969,   362,   363,   649,
     615,   616,  1857,  2145,   640,   617,  1945,  1363,  1984,  2151,
     125,  1946,   126,  1985,   664,   664,  2167,  2166,  1637,  2172,
     364,  2168,   805,  1650,  2173,  2171,   365,   366,   367,   368,
     369,   370,  2215,  2216,  2257,   658,  2276,  1363,  1363,  1363,
     356,  1363,  2506,  2525,  3232,  2182,  2588,  1363,  2526,  3471,
    2511,  2589,  2711,   360,    22,  2750,   361,  2712,  3426,  2805,
    2526,  3430,  3431,  2166,  2806,  3433,  1357,   362,   363,   672,
    3232,  2808,  2843,  2867,   364,  3640,  2806,  2844,  2868,   665,
     365,   366,   367,   368,   369,   370,  2939,  2947,  2228,   791,
     792,  2940,  2712,  2957,  2237,   371,  2962,   118,  2526,   666,
    2990,  2712,  2992,   364,  2015,  2991,  1354,  2526,   673,   365,
     366,   367,   368,   369,   370,  2256,  2477,  3035,  3039,  3099,
    3174,  3002,  1363,  3040,  2526,  2526,  3181,  1482,  1482,  3195,
     674,  3182,   675,  1403,  3196,     3,     4,     5,  3197,   667,
    3249,     6,     7,  3196,     9,  3040,   963,    10,    11,   554,
     555,    13,    14,   680,  2278,    16,  1482,  3250,    18,    19,
      20,    21,  3040,  1638,  3154,   668,  3268,   805,    23,  2295,
    2301,  3269,  1354,  3344,  3367,   923,   669,   924,  3345,  3368,
    2312,  3425,  2314,  1079,  1080,  2477,  2933,  2319,  2934,  2935,
    3398,    24,    25,    26,    27,    28,    29,    30,    31,    32,
      33,    34,    35,    36,    37,    38,    39,    40,    41,  2533,
    3420,  1613, -1596,  3457,  1992,  3421,   900,  2799,  3458,  2337,
    2005,   676,  2002,   677,  3498,   401,   401,  3521,   401,  3499,
     670,   401,  3522,  3546,  2477,  1354,  3534,  2813,  3541,  3525,
    1639,  3535,  3562,  1363,   805,  3545,     2,  3563,  3605,     3,
       4,     5,   681,  1977,  3645,     6,     7,     8,     9,  1363,
     682,    10,    11,    12,  2849,    13,    14,  2106,    15,    16,
    3015,    17,    18,    19,    20,    21,   615,   616,  3674,  3677,
    3679,  2880,    23,  3675,  3678,  3678,  3681,  1354,  1641,  3733,
     699,  3682,  1642,  2896,  3196,  2875,  3738,  1354,  1191,  3767,
    1192,  3739,   683,  1354,  3196,    24,    25,    26,    27,    28,
      29,    30,    31,    32,    33,    34,    35,    36,    37,    38,
      39,    40,    41,  3772,   686,  3778,  3781,  3793,  3773,  2407,
    3040,  3782,  3196,   649,   649,   364,  2354,  2092,   700,  2093,
    3612,   365,   366,   367,   368,   369,   370,  1336,  1337,  3802,
    3806,  3813,   364,  2356,  3803,  3807,  3196,   715,   365,   366,
     367,   368,   369,   370,  2351,  2353,  2281,  2678,   805,   805,
    2282,   805,  1523,  3319,   701,   356,  2432,  2680,  2433,  3328,
    2936,   805,  3122,   985,   805,  3123,   709,   693,   360,    22,
     697,   361,   710,   694,  1354,   664,   698,  3575,  1151,  1354,
     814,  1354,   362,   363,  2968,  2968,  1693,  1497,  1498,  1701,
    2863,  1643,   724,   805,  1385,  1168,  1169,  1170,  1171,  2098,
    1523,  2099,   364,  2681,   725,  2471,  2487,  2472,   365,   366,
     367,   368,   369,   370,  1354,  2888,  3356,   937,  3357,  1385,
     726,   649,  2888,  3800,  3169,  3801,   554,   656,  1354,   736,
    3008,  3077,  3078,  3129,  3130,   649,   649,  3411,  3412,  3664,
    3665,  1164,  1165,   356,   336,  2228,  3704,  3705,  3024,   746,
    1166,  1167,  1172,  1173,  2693,  2695,   360,    22,   748,   361,
    3727,  3728,  3527,  3528,  2535,   749,  1482,  3022,  3023,  2301,
     362,   363,  2301,   751,   986,   987,   757,  2799,   361,  2799,
     758,   356,  3809,  3810,   759,  1482,  3069,  3070,  3740,  1524,
    1525,   988,   989,  2564,   360,    22,   799,   361,   990,   991,
    3599,  3600,  2571,  3707,   992,  3708,  3405,  3406,   362,   363,
    1376,  1377,   800,   993,   801,   649,  1527,  1528,   994,  1529,
    1530,   802,    53,   809,  1378,  1379,  3646,  3647,   810,   811,
     401,   812,   401,   401,   995,   819,   820,  1524,  1525,   824,
    2002,   825,  2601,   857,   826,  2662,  3784,   831,   863,  1992,
     866,  3707,   867,  3708,   996,   868,   871,  3712,   872,   873,
     878,  1353,   879,   882,  1527,  1528,   885,  1529,  1530,   886,
     887,   897,   903,   997,   916,   917,   920,  2685,   918,   921,
    2689,  1234,   922,   930,  1235,  1236,   943,  1393,  1354,   954,
    2699,   957,  3736,  3737,  1354,   958,  1354,   959,   960,   961,
    2713,   962,   976,  1237,  1354,  2717,  2718,   977,   979,   998,
    1003,  1004,  1005,  1006,  1007,  1008,  1014,  1016,  1015,  2124,
    2728,  1017,  1019,  1238,  3712,  1023,  1239,  1022,  1024,  1059,
    1063,  3191,  1240,  2732,  1068,  1070,  1067,  1089,  1088,  1071,
    3084,  2737,  1085,  1090,  1086,  1098,  1097,  1100,  1101,   336,
    1105,  1106,   118,  1110,  1128,   455,  1118,  3208,   456,  3712,
    1354,  1136,  1138,  1141,   457,  1144,  1385,  1153,  1195,  3712,
    1198,  1202,  1209,  1206,  1241,  2888,  1207,  1208,  1217,  1210,
    1218,  1219,   649,  1230,  3712,  3712,  3712,  3712,  3712,  1267,
    1291,   458,  3712,  1292,  2770,  1300,  1301,  1604,  1531,  1531,
    1531,  1531,  1531,  1531,  1531,  1531,  1531,  1531,  1531,  1531,
    2799,  1304,  1306,   459,  1315,  1318,  1354,  1333,  1341,  1242,
    3256,  1335,  1358,  1342,  1345,  2793,  1573,  1352,  1367,   460,
    1368,  1399,  1354,  1407,  1397,  1409,  1374,   364,  1415,  2802,
     461,   462,   463,   365,   366,   367,   368,   369,   370,  1414,
    1412,  2564,   664,  1416,  1425,  1354,  1426,   464,  1433,  1434,
    3286,  1435,  1454,  1243,   465,   466,   467,  1456,  1468,  1484,
     468,  1244,  1245,   469,  1455,   470,  1469,  1472,  2301,   471,
    1475,  1354,  1476,   904, -1068,  1504,  2875,  1552,   401,  1551,
    1246,  1354,  1556,  2859,  1569,  1576,  1583,  1584,  1247,  1579,
     472,  2875,  1590,  1589,  2005,  1594,  1586,  1601,   473,  1605,
    2662,  1599,  2879,  1055,  1620,  2002,  2005,  2884,  2886,  2887,
     474,  2662,  1595,  2893,  1596,   755,  2898,  1614,  2902,  1616,
    1625,   365,   366,   367,   368,   369,   370,  1632,  1647,   475,
     618,   619,  1652,  1653,  1654,  2662,  1655,  1636,  1656,  1657,
    3252,  1248,  1645,  1659,  1646,  2005,  1664,  1662,  1660,  1671,
    1677,  1663,  1670,   761,  1676,  1694,  2968,  1690,   620,   365,
     366,   367,   368,   369,   370,   621,  1678,  1695,  1682,  1702,
    1703,  1704,   622,  3277,  3278,  1713,  1714,  1712,  1716,  1719,
    1721,  1258,  1723,   476,   623,  1726,  1717,  1756,  1354,  1718,
    1724,  1731,  1754,  1755,  1757,  3024,  2954,   624,  1758,  1759,
    1776,  1778,  1774,  1775,   625,  3414,  1777,  1763,  1354,  1779,
    1788,  1789,  1780,  1785,  1797,  2970,  2888,  1799,  2974,  1819,
    1809,  1781,  1825,  1803,  1829,  1830,  1787,  1798,  1821,  1818,
    1385,  1836,  1837,  1840,  2983,  1848,  1854,  1385,  1856,  3449,
    1868,  1875,  1887,  -579,  1858,  1876,  1888,   626,   912,  1860,
     627,  1880,   649,  1878,  1884,  1879,  1886,  1896,  3011,  1897,
     628,  1894,  1898,  1791,  1885,  1899,  1902,  1909,  1901,  2898,
    1910,  1911,  1914,  1791,  1930,  1928,  1932,  2875,  1933,  1939,
    1354,  1943,   629,  1947,  1944,  1948,  1956,  1953,  1954,  1958,
    1959,  1968,  1977,  1967,   630,  2011,  2009,  2027,  2026,  2029,
    1838,  2028,  2301,  1960,  1976,  2030,  2033,  2035,  2025,  2044,
    2072,  2032,   631,  2036,  2054,  3500,  2037,  2038,  2043,  3062,
    3063,  2059,  2076,  2564,  2061,  2074,  2079,  2077,  2106,  3072,
    3075,  3076,   649,  2111,  2112,   632,   649,  2114,  2113,  2121,
    2571,  3092,  2125,  1748,  2120,  2143,  2126,  3093,  1872,  2128,
    2130,  2141,  1393,  3098,  2138,  2144,  2146,   649,  2149,  2153,
    2152,   649,  2150,  2170,   649,  2155,  3111,  2169,  2176,  2662,
    2197,  3450,  2178,  1604,  2179,  2184,  2194,  2199,  2206,  2210,
    2213,  3256,  2214,  2202,  2208,  2218,  2662,  2217,  2222,  2220,
    2229,  2243,  2234,  2244,  2247,  2232,   554,   555,  2233,  3144,
    2253,  2254,  2260,  2263,  2273,  2274,  2279,  2287,  2289,  2290,
    2304,  2308,  2305,  2313,  2888,  2888,  2317,  1322,  3155,  2320,
    3159,  1354,  2326,  2331,  2339,  2333,  2344,  2002,  2345,  2359,
    2360,  2361,  2363,  2374,  2875,  2362,  2689,  3171,  2364,  2379,
    2380,  2365,  2371,  2381,  2382,  2384,  2385,  2366,  2394,  2398,
    2401,  1354,  2390,  2399,  2400,  2100,  2411,  2408,  2410,  2413,
    2442,  2425,  2415,  2414,  1323,  2412,  2457,  2460,  2461,  2482,
    2424,  2491,  3513,  2189,  2488,  3203,  2448,     2,  3205,  2427,
       3,     4,     5,  2503,  2421,  2423,     6,     7,     8,     9,
    3218,  2454,    10,    11,    12,  3414,    13,    14,  2430,    15,
      16,  2428,    17,    18,    19,    20,    21,  3614,  2429,  2492,
    2434,  2453,  2477,    23,  2437,  3625,  3625,  3632,  3449,  2439,
    2445,  2476,  2483,  3544,  2484,  2494,  2499,  2501,  2509,  2507,
    1112,  2875,  2519,  2521,  2522,  2531,    24,    25,    26,    27,
      28,    29,    30,    31,    32,    33,    34,    35,    36,    37,
      38,    39,    40,    41,  2541,  2527,  2542,  2544,  2532,  2545,
    2549,  2546,  1989,  2555,  2574,  2567,  2576,  2564,  1403,  2577,
    2575,  2579,  2582,  2584,  2583,  1403,  2585,  2587,  2599,  2663,
    2672,  2526,   649,  2684,  2675,  2690,  3582,  2168,  2697,  2691,
    2692,  2700,  2701,  2703,  2714,  2715,  2720,  2719,  2723,  2724,
    2736,  2722,  2875,  2738,  2726,  2435,  2436,  2739,   649,  2740,
    2741,  2743,  2749,  2752,  2760,  2662,  2768,   649,  3311,  2744,
    2662,  2771,  2968,  2898,  3316,  2756,  2773,  3320,  2758,  2662,
    2778,  2779,  2759,  2535,  2781,  2782,  3330,  2005,  3625,  3332,
    2791,  2792,  3632,  2902,  2784,  2241,  2804,  1431,  2816,  2078,
    2807,  2809,  2851,  2852,  2823,  2083,  2166,  2824,  2854,  2825,
    2857,  3347,  2860,  2845,  2869,  2871,  2827,  2872,  2924,  2930,
    2941,  2925,  2949,  2948,  2958,  2881,  2865,  2915,  2954,  3365,
    2891,  2955,  2903,  2904,  2956,  2905,  2906,  2907,  2908,  2909,
    2910,  2911,  2968,  2912,  2913,  3377,  3378,  2920,  2926,  2959,
    2960,  2961,  2965,  2975,  2979,  2978,  2982,  2977,  2980,  2971,
    2981,  2987,  2799,     3,     4,     5,  2989,  2972,  2993,     6,
       7,  2998,     9,  2968,  2985,    10,    11,  3001,  2898,    13,
      14,  3004,  1769,    16,  3018,  3012,    18,    19,    20,    21,
    3019,  3027,  3028,  3033,  3043,    22,    23,  3050,  3048,  3065,
    3067,  3071,  2968,  3082,  3083,  3085,  3088,  1403,  3428,  3090,
    1403,  1403,  3096,  3100,  1403,  3075,  3435,  3094,  3113,    24,
      25,    26,    27,    28,    29,    30,    31,    32,    33,    34,
      35,    36,    37,    38,    39,    40,    41,  3116,  3456,  3101,
    3119,  2874,   649,  3124,  3132,  3125,  3133,   455,  3136,  2005,
     456,  3138,  1054,  2615,  3139,  2662,   457,  3143,  2662,  3142,
    2226,  3145,  3147,  3151,  3146,  3152,  3172,  2662,  2662,  3164,
    3156,  3183,  3198,  3178,  3179,   649,   649,  3189,  2245,  3188,
    3190,  1685,  3202,  3204,  3216,  1354,  3201,  3221,  3492,  3225,
    3244,  1354,  3232,  3235,  3243,  3245,  3496,  3497,  3260,  3273,
     664, -1234,  3263,  3261,  3279,   459,  3502,  3264,  3280,  3283,
    2838,  3290,  3504,  3291,  1354,  3274,  3287,  3292,   649,  3295,
    3303,  1095,  3310,  3297,  3299,  3300,  2277,  3298,  3317,  -520,
    3327,  3302,   649,   462,   463,  3340,  3333,  3348,  3520,   649,
    3370,  3350,  2297,  3351,  1573,  3314,  3354,  3366,  3373,   464,
    3371,  3375,  3391,   118,  3376,  3379,   465,   466,   467,  3380,
    3389,  3390,   468,  3392,  3395,  1764,  3413,   470,  3403,  3429,
    3417,   471,  3440,  3441,  3437,  3453, -1068,  3459,  3464,  3461,
    3462,  3467,  1403,  3463,  3468,  3469,  1989,  1354,  3472,  3474,
    3480,  3488,   472,  3481,  3482,  3483,  3485,  3490,  3495,  3561,
     473,  3491,  3484,  3507,  3493,  3501,  3508,  2245,  3509,  3511,
    3512,  3514,   474,  3510,  3515,  3516,  3518,  3529,  2898,  3530,
    3519,  2005,  3564,  2662,  3543,  3547,  3555,  3556,  3557,  3559,
    3569,   475,  3567,  3571,  2902,  3578,  2166,  3558,  3609,  3618,
    3591,  3566,  3635,  3568,  3607,  3622,  3581,  3650,  3651,  3634,
    3642,  -574,  3638,   649,  3644,  3648,  3656,  3589,  3590,  1791,
    3669,  3670,  3672,  3683,  3608,  3673,  3684,  3686,  3700,  2389,
    3703,  3694,  3695,   649,  3706,  2392,  3714,  3732,  3731,  3734,
    3743,  3745,  3746,  3741,  3774,   476,  3775,  3742,  3611,  1403,
    3752,  3779,  3765,  3766,  1354,  3792,  3799,  1354,  3764,   107,
    1174,   732,  1175,   345,   423,   337,  1628,  1176,   821,  1092,
    3637,   760,   226,   228,  1062,  2005,   649,   649,  1177,  1099,
     808,  1178,  1588,     3,     4,     5,  1261,  3649,   975,     6,
       7,  2751,     9,  1179,  1490,    10,    11,  1264,  2262,    13,
      14,  2803,   651,    16,  1483,  1194,    18,    19,    20,    21,
    3052,  3014,  3013,  2115,  2727,  2127,    23,  3452,  3636,  2746,
    2087,  1354,  3505,   813,  1185,   814,  1791,  3180,  1354,  2951,
      53,  1791,  3364,  2245,   649,  3170,  3565,  3460,  3503,    24,
      25,    26,    27,    28,    29,    30,    31,    32,    33,    34,
      35,    36,    37,    38,    39,    40,    41,  1265,  1957,    42,
      43,    44,  3696,  1795,  1725,  1316,  2493,    45,    46,    47,
      48,    49,    50,  3374,  3597,  1403,  1403,  3583,  2377,  2444,
    2505,  1354,  2456,  3387,  2986,   649,  2742,  3388,  2302,  3719,
    3720,   597,  2201,  1904,  3248,  2005,  3251,  2226,  3735,  3091,
    2846,  3539,  2543,  3540,  2292,  1937,   685,  2209,  2303,  2301,
    1572,  1111,  2211,  1870,  3617,  3754,  3676,  1403,  3783,  3808,
    3434,  2297,  3716,  3626,  2297,  3717,  3284,  2005,  3718,   649,
    3633,  3087,  3282,  2325,  1494,  1018,  2205,  3749,  3407,  3408,
    3606,  1493,  1833,  3604,  2212,  2923,  3157,  1354,   649,  1350,
    2330,  3338,  2882,  3494,  2568,  3121,  3308,  3307,  2340,  3687,
    1786,  1354,  1834,  2479,  1770,  2440,  2438,  2419,  3399,  2480,
    2730,  2463,  2674,  3163,  1375,  3010,  3402,  3229,  1966,  1354,
    2775,     0,  1597,  3584,     0,     0,     0,     0,  1354,     0,
       0,     0,     0,     0,  2600,     0,     0,     0,     0,     0,
       0,  1989,     2,   348,   349,     3,     4,     5,   350,   351,
     352,     6,     7,     8,     9,   353,   354,    10,    11,    12,
     355,    13,    14,   356,    15,    16,   357,    17,    18,    19,
      20,    21,   358,     0,     0,   359,   360,    22,    23,   361,
    2392,     0,     0,     0,     0,     0,  2245,     0,  2392,     0,
     362,   363,     0,     0,     0,     0,  2716,     0,     0,     0,
       0,    24,    25,    26,    27,    28,    29,    30,    31,    32,
      33,    34,    35,    36,    37,    38,    39,    40,    41,     0,
     649,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    51,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,  2245,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     2,   348,   349,     3,
       4,     5,   350,   351,   352,     6,     7,     8,     9,   353,
     354,    10,    11,    12,   355,    13,    14,   356,    15,    16,
     357,    17,    18,    19,    20,    21,   358,     0,     0,   359,
     360,    22,    23,   361,     0,     0,     0,     0,  2787,     0,
       0,     0,     0,     0,   362,   363,     0,     0,     0,     0,
       0,     0,     0,     0,  2795,    24,    25,    26,    27,    28,
      29,    30,    31,    32,    33,    34,    35,    36,    37,    38,
      39,    40,    41,     0,     0,   344,     0,  2814,     0,     0,
       0,     3,     4,     5,     0,     0,     0,     6,     7,     0,
       9,    51,     0,    10,    11,     0,     0,    13,    14,     0,
    2297,    16,     0,  2850,    18,    19,    20,    21,     0,     0,
       0,     0,     0,  2245,    23,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    24,    25,    26,
      27,    28,    29,    30,    31,    32,    33,    34,    35,    36,
      37,    38,    39,    40,    41,     0,     0,    42,    43,    44,
       0,     0,     0,     0,     0,    45,    46,    47,    48,    49,
      50,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     3,     4,     5,     0,     0,
       0,     6,     7,     0,     9,     0,     0,    10,    11,     0,
       0,    13,    14,     0,     0,    16,     0,     0,    18,    19,
      20,    21,     0,     0,     0,     0,     0,     0,    23,     0,
    2245,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
    2964,    24,    25,    26,    27,    28,    29,    30,    31,    32,
      33,    34,    35,    36,    37,    38,    39,    40,    41,     0,
       0,    42,    43,    44,     0,     0,     0,     0,     0,    45,
      46,    47,    48,    49,    50,   364,     0,     0,     0,     0,
       0,   365,   366,   367,   368,   369,   370,     0,     0,     0,
       0,     0,     0,     3,     4,     5,   371,   372,   118,     6,
       7,     0,     9,     0,     0,    10,    11,     0,     0,    13,
      14,     0,  3034,    16,     0,     0,    18,    19,    20,    21,
       0,     0,     0,     0,     0,    22,    23,     0,     0,     0,
       0,     0,     0,     0,  2297,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    24,
      25,    26,    27,    28,    29,    30,    31,    32,    33,    34,
      35,    36,    37,    38,    39,    40,    41,     0,     0,     3,
       4,     5,  2568,     0,     0,     6,     7,     0,     9,     0,
       0,    10,    11,     0,     0,    13,    14,   356,     0,    16,
       0,     0,    18,    19,    20,    21,     0,     0,  3110,   364,
     360,    22,    23,   361,     0,   365,   366,   367,   368,   369,
     370,     0,     0,     0,   362,   363,     0,     0,     0,     0,
     371,   807,   118,     0,     0,    24,    25,    26,    27,    28,
      29,    30,    31,    32,    33,    34,    35,    36,    37,    38,
      39,    40,    41,     0,     0,     0,     0,     0,   106,     0,
    3153,     1,     2,  3160,     0,     3,     4,     5,     0,     0,
       0,     6,     7,     8,     9,     0,     0,    10,    11,    12,
       0,    13,    14,     0,    15,    16,     0,    17,    18,    19,
      20,    21,     0,  3184,     0,     0,     0,    22,    23,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   738,     0,     0,     0,     0,     0,     0,
       0,    24,    25,    26,    27,    28,    29,    30,    31,    32,
      33,    34,    35,    36,    37,    38,    39,    40,    41,     0,
       0,    42,    43,    44,     0,     0,     0,     0,     0,    45,
      46,    47,    48,    49,    50,     0,     0,    51,     1,     2,
       0,     0,     3,     4,     5,     0,     0,     0,     6,     7,
       8,     9,     0,     0,    10,    11,    12,     0,    13,    14,
       0,    15,    16,     0,    17,    18,    19,    20,    21,     0,
       0,     0,     0,     0,    22,    23,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   745,    24,    25,
      26,    27,    28,    29,    30,    31,    32,    33,    34,    35,
      36,    37,    38,    39,    40,    41,     0,     0,    42,    43,
      44,     0,     0,     0,     0,     0,    45,    46,    47,    48,
      49,    50,     0,     0,    51,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     3,     4,     5,
       0,     0,     0,     6,     7,     0,     9,     0,  3153,    10,
      11,     0,     0,    13,    14,     0,     0,    16,     0,     0,
      18,    19,    20,    21,     0,     0,     0,     0,     0,    22,
      23,     0,     0,    52,     0,     0,     0,     0,     0,     0,
      53,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   740,    24,    25,    26,    27,    28,    29,    30,
      31,    32,    33,    34,    35,    36,    37,    38,    39,    40,
      41,     0,     0,     0,     0,     3,     4,     5,     0,     0,
       0,     6,     7,     0,     9,     0,     0,    10,    11,     0,
       0,    13,    14,     0,     0,    16,     0,     0,    18,    19,
      20,    21,     0,     0,     0,     0,     0,     0,    23,   364,
       0,     0,     0,     0,     0,   365,   366,   367,   368,   369,
     370,     0,     0,     0,     0,     0,     0,     0,     0,     0,
    3427,    24,    25,    26,    27,    28,    29,    30,    31,    32,
      33,    34,    35,    36,    37,    38,    39,    40,    41,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     3,     4,
       5,     0,     0,     0,     6,     7,     0,     9,     0,     0,
      10,    11,     0,     0,    13,    14,     0,     0,    16,     0,
       0,    18,    19,    20,    21,     0,     0,     0,     0,     0,
       0,    23,     0,     0,     0,    52,     0,     0,     0,     0,
       0,     0,    53,     0,     0,     0,     0,  3489,     0,     0,
       0,     0,     0,  3160,    24,    25,    26,    27,    28,    29,
      30,    31,    32,    33,    34,    35,    36,    37,    38,    39,
      40,    41,     0,     0,     0,     0,  3184,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   578,     0,     3,     4,     5,     0,   579,     0,
       6,     7,   434,     9,     0,     0,    10,    11,     0,     0,
      13,    14,     0,     0,    16,     0,     0,    18,    19,    20,
      21,     0,     0,     0,     0,   580,     0,    23,     0,     0,
       0,     0,    52,     0,     0,   435,     0,     0,     0,    53,
       0,     0,     0,     0,     0,     0,     0,     0,     0,  3552,
      24,    25,    26,    27,    28,    29,    30,    31,    32,    33,
      34,    35,    36,    37,    38,    39,    40,    41,     0,     0,
       0,     0,     0,     0,   581,   582,   583,     0,     0,     0,
       0,     0,     0,     0,   436,     0,   437,     0,     0,     0,
       0,   584,     0,     0,     0,     0,     0,     0,   585,   586,
       0,   438,   439,     0,   587,     0,     0,   588,     0,   440,
       0,     0,     0,   589,   441,     0,     0,     0,   590,     0,
       0,     0,     0,   442,     0,     0,     0,     0,   443,     0,
       0,     0,     0,     0,   591,     0,     0,   813,     0,   814,
       0,     0,   592,     0,    53,     0,     0,     0,     0,     0,
    3610,     0,   444,     0,   593,     0,  3616,     0,     0,  3552,
       0,     0,     0,     0,   445,     0,     0,     0,     0,   594,
       0,     0,     0,   595,     0,     0,     0,     0,     0,   446,
       0,     0,     0,   447,     0,     0,     0,     0,     0,   482,
       0,     0,     0,     0,   483,   484,     0,     0,   485,   486,
       0,     0,     0,     0,     0,   487,   488,     0,   489,   490,
       0,   491,     0,   492,     0,  1151,  1185,   814,     0,   493,
       0,     0,    53,  3671,     0,     0,   494,   596,   495,   496,
    3552,     0,     0,     0,     0,     0,     0,   448,   497,     0,
     498,     0,     0,     0,     0,     0,     0,   499,     0,   500,
     501,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   502,   503,   504,   505,     0,   506,     0,   507,     0,
     508,   509,   510,  3552,   511,     0,     0,   512,     0,     0,
       0,     0,   513,     0,     0,     0,     0,     0,  1151,   514,
     814,     0,   515,     0,     0,    53,   516,   517,   518,   519,
       0,  2297,     0,     0,     0,     0,   520,     0,     0,     0,
       0,   521,     0,     0,     0,   522,     0,     0,   523,     0,
       0,   524,     0,     0,     0,   525,   526,   527,   528,     0,
       0,   529,     0,     0,   530,     0,     0,     0,     0,  3756,
       0,     0,     0,     0,     0,   531,   532,   533,   534,     0,
     434,     0,     0,  3756,     0,     0,     0,     0,     0,   535,
       0,     0,     0,     0,     0,     0,     0,     0,   536,     0,
     537,  3756,     0,     0,     0,     0,   538,   539,     0,     0,
    3756,     0,     0,   435,     0,  1648,     0,     0,     0,     0,
       0,   540,   541,     0,     0,   542,   543,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   544,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   545,     0,     0,     0,     0,   546,     0,
     547,     0,   436,     0,   437,     0,     0,     0,   548,     0,
       0,   549,   550,   551,   552,   553,   554,   555,     0,   438,
    1093,     0,     0,   556,     0,   557,  3442,   440,     0,     0,
       0,     0,   441,   558,   482,     0,     0,   559,     0,   483,
     484,   442,     0,   485,   486,  3443,   443,     0,     0,     0,
     487,   488,  3444,   489,   490,     0,   491,     0,   492,     0,
       0,     0,     0,     0,   493,     0,     0,     0,     0,     0,
     444,   494,     0,   495,   496,     0,     0,     0,     0,     0,
       0,     0,   445,   497,     0,   498,     0,     0,     0,     0,
       0,     0,   499,     0,   500,   501,     0,   446,     0,     0,
       0,   447,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   502,   503,   504,   505,
       0,   506,     0,   507,     0,   508,   509,   510,     0,   511,
       0,     0,   512,     0,     0,     0,     0,   513,     0,     0,
       0,     0,     0,     0,   514,     0,     0,   515,     0,     0,
       0,   516,   517,   518,   519,   448,     0,     0,     0,     0,
       0,   520,     0,     0,     0,     0,   521,     0,     0,     0,
     522,     0,     0,   523,     0,     0,   524,     0,     0,     0,
     525,   526,   527,   528,     0,     0,   529,     0,     0,   530,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     531,   532,   533,   534,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   535,     0,     0,     0,     0,     0,
       0,     0,     0,   536,     0,   537,     0,     0,     0,     0,
       0,   538,   539,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   540,   541,     0,     0,
     542,   543,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   544,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,  3627,     0,
       0,     0,     0,   546,     0,   547,     0,     0,     0,     0,
       0,     0,   143,   548,     0,     0,   549,   550,   551,   552,
     553,   554,   555,     0,     0,     0,     0,     0,   556,   144,
     557,  3442,   145,   146,     0,   147,     0,     0,   558,   148,
       0,     0,   559,     0,   149,   150,   151,   152,     0,     0,
    3443,   153,     0,   154,   155,     0,     0,     0,     0,     0,
       0,   156,     0,     0,     0,   157,     0,     0,     0,   158,
       0,     0,   159,     0,     0,     0,     0,     0,   160,     0,
       0,     0,     0,     0,     0,   161,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   162,   163,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   164,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   165,     0,   166,   167,     0,   168,     0,
     169,   170,     0,     0,   171,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   172,     0,   173,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   174,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   175,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   176,
     177,     0,     0,     0,     0,     0,   178,     0,     0,     0,
       0,     0,     0,     0,     0,   179,     0,     0,     0,     0,
       0,   180,   181,     0,     0,     0,   482,     0,     0,     0,
       0,   483,   484,     0,     0,   485,   486,     0,     0,     0,
       0,     0,     0,   899,   182,   489,   490,     0,   491,     0,
     492,     0,     0,     0,     0,     0,   493,     0,     0,     0,
       0,     0,  1387,   494,     0,   495,   496,     0,   183,     0,
     184,     0,   185,   186,     0,   497,     0,   498,   187,     0,
       0,     0,     0,   188,     0,     0,   500,   501,     0,     0,
     189,     0,     0,   190,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   502,   503,
       0,   505,     0,   506,     0,   507,     0,   508,   509,   510,
       0,   511,     0,     0,     0,     0,     0,     0,     0,   513,
       0,     0,     0,     0,     0,     0,   514,     0,     0,  1388,
       0,     0,     0,   516,   517,   518,   519,     0,     0,     0,
       0,     0,     0,   520,     0,     0,     0,     0,   521,     0,
       0,     0,   522,     0,     0,   523,     0,     0,   524,     0,
       0,     0,     0,   526,   527,     0,     0,     0,   529,     0,
       0,   530,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   531,   532,   533,   534,     0,     0,     0,     0,
       0,     0,     0,     0,     0,  1389,   535,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   537,     0,     0,
       0,     0,     0,     0,   539,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   482,     0,   540,   541,
       0,   483,   484,     0,     0,   485,   486,     0,     0,     0,
       0,     0,     0,   899,     0,   489,   490,     0,   491,     0,
     492,     0,     0,     0,     0,     0,   493,     0,     0,     0,
       0,     0,  1387,   494,  1873,   495,   496,     0,     0,     0,
       0,     0,     0,     0,     0,   497,     0,   498,     0,     0,
       0,     0,     0,   554,   555,     0,   500,   501,     0,     0,
       0,     0,     0,     0,     0,     0,     0,  1390,     0,     0,
     558,     0,     0,     0,  1391,  1392,     0,     0,   502,   503,
       0,   505,     0,   506,     0,   507,     0,   508,   509,   510,
       0,   511,     0,     0,     0,     0,     0,     0,     0,   513,
       0,     0,     0,     0,     0,     0,   514,     0,     0,  1388,
       0,     0,     0,   516,   517,   518,   519,     0,     0,     0,
       0,     0,     0,   520,     0,     0,     0,     0,   521,     0,
       0,     0,   522,     0,     0,   523,     0,     0,   524,     0,
       0,     0,     0,   526,   527,     0,     0,     0,   529,     0,
       0,   530,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   531,   532,   533,   534,     0,     0,     0,     0,
       0,     0,     0,     0,     0,  1389,   535,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   537,     0,     0,
       0,     0,     0,     0,   539,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   482,     0,   540,   541,
       0,   483,   484,     0,     0,   485,   486,     0,     0,     0,
       0,     0,     0,   899,     0,   489,   490,     0,   491,     0,
     492,     0,     0,     0,     0,     0,   493,     0,     0,     0,
       0,     0,  1387,   494,  2520,   495,   496,     0,     0,     0,
       0,     0,     0,     0,     0,   497,     0,   498,     0,     0,
       0,     0,     0,   554,   555,     0,   500,   501,     0,     0,
       0,     0,     0,     0,     0,     0,     0,  1390,     0,     0,
     558,     0,     0,     0,  1391,  1392,     0,     0,   502,   503,
       0,   505,     0,   506,     0,   507,     0,   508,   509,   510,
       0,   511,     0,     0,     0,     0,     0,     0,     0,   513,
       0,     0,     0,     0,     0,     0,   514,     0,     0,  1388,
       0,     0,     0,   516,   517,   518,   519,     0,     0,     0,
       0,     0,     0,   520,     0,     0,     0,     0,   521,     0,
       0,     0,   522,     0,     0,   523,     0,     0,   524,     0,
       0,     0,     0,   526,   527,     0,     0,     0,   529,     0,
       0,   530,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   531,   532,   533,   534,     0,     0,     0,     0,
       0,     0,     0,     0,     0,  1389,   535,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   537,     0,     0,
       0,     0,     0,     0,   539,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   540,   541,
     482,  1380,     0,     0,     0,   483,   484,     0,     0,   485,
     486,     0,     0,     0,     0,     0,   487,   488,     0,   489,
     490,     0,   491,     0,   492,     0,     0,     0,     0,     0,
     493,     0,     0,     0,     0,     0,     0,   494,     0,   495,
     496,     0,  1381,     0,     0,     0,     0,     0,     0,   497,
       0,   498,     0,   554,   555,     0,     0,     0,   499,     0,
     500,   501,     0,     0,     0,     0,     0,  1390,     0,     0,
     558,     0,     0,     0,  1391,  1392,     0,     0,     0,     0,
       0,     0,   502,   503,   504,   505,     0,   506,     0,   507,
       0,   508,   509,   510,     0,   511,     0,   641,   512,     0,
       0,     0,     0,   513,     0,     0,     0,     0,     0,     0,
     514,     0,     0,   515,     0,     0,     0,   516,   517,   518,
     519,     0,     0,     0,     0,     0,     0,   520,     0,     0,
       0,     0,   521,     0,     0,     0,   522,     0,     0,   523,
       0,     0,   524,     0,     0,     0,   525,   526,   527,   528,
     642,     0,   529,     0,     0,   530,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   531,   532,   533,   534,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   643,
     535,     0,     0,     0,     0,     0,     0,     0,     0,   536,
       0,   537,     0,     0,     0,     0,     0,   538,   539,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   540,   541,     0,     0,   542,   543,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   544,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   545,  1382,     0,     0,  1383,   546,
       0,   547,     0,     0,     0,     0,     0,     0,     0,   548,
       0,     0,   549,   550,   551,   552,   553,   554,   555,     0,
       0,     0,     0,     0,   556,     0,   557,   482,  1380,     0,
       0,     0,   483,   484,   558,     0,   485,   486,   559,     0,
       0,     0,     0,   487,   488,     0,   489,   490,     0,   491,
       0,   492,     0,     0,     0,     0,     0,   493,     0,     0,
       0,     0,     0,     0,   494,     0,   495,   496,     0,  1381,
       0,     0,     0,     0,     0,     0,   497,     0,   498,     0,
       0,     0,     0,     0,     0,   499,     0,   500,   501,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   502,
     503,   504,   505,     0,   506,     0,   507,     0,   508,   509,
     510,     0,   511,     0,   641,   512,     0,     0,     0,     0,
     513,     0,     0,     0,     0,     0,     0,   514,     0,     0,
     515,     0,     0,     0,   516,   517,   518,   519,     0,     0,
       0,     0,     0,     0,   520,     0,     0,     0,     0,   521,
       0,     0,     0,   522,     0,     0,   523,     0,     0,   524,
       0,     0,     0,   525,   526,   527,   528,   642,     0,   529,
       0,     0,   530,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   531,   532,   533,   534,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   643,   535,     0,     0,
       0,     0,     0,     0,     0,     0,   536,     0,   537,     0,
       0,     0,     0,     0,   538,   539,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   540,
     541,     0,     0,   542,   543,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   544,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   545,  3574,     0,     0,  1383,   546,     0,   547,     0,
       0,     0,     0,     0,     0,     0,   548,     0,     0,   549,
     550,   551,   552,   553,   554,   555,     0,     0,     0,     0,
     482,   556,     0,   557,     0,   483,   484,     0,     0,   485,
     486,   558,     0,     0,     0,   559,   487,   488,     0,   489,
     490,     0,   491,     0,   492,     0,     0,     0,     0,     0,
     493,     0,     0,     0,     0,     0,     0,   494,     0,   495,
     496,     0,     0,     0,     0,     0,     0,     0,     0,   497,
       0,   498,     0,     0,     0,     0,     0,     0,   499,     0,
     500,   501,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   502,   503,   504,   505,     0,   506,     0,   507,
       0,   508,   509,   510,     0,   511,     0,   641,   512,     0,
       0,     0,     0,   513,     0,     0,     0,     0,     0,     0,
     514,     0,     0,   515,     0,     0,     0,   516,   517,   518,
     519,     0,     0,     0,     0,     0,     0,   520,     0,     0,
       0,     0,   521,     0,     0,     0,   522,     0,     0,   523,
       0,     0,   524,     0,     0,     0,   525,   526,   527,   528,
     642,     0,   529,     0,     0,   530,     0,     0,     0,     0,
       0,     0,     0,   904,     0,     0,   531,   532,   533,   534,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   643,
     535,     0,     0,     0,     0,     0,     0,     0,     0,   536,
       0,   537,     0,     0,     0,     0,     0,   538,   539,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   540,   541,     0,     0,   542,   543,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   544,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   545,  3680,     0,     0,     0,   546,
       0,   547,     0,     0,     0,     0,     0,     0,     0,   548,
       0,     0,   549,   550,   551,   552,   553,   554,   555,     0,
       0,     0,     0,   482,   556,     0,   557,     0,   483,   484,
       0,     0,   485,   486,   558,     0,     0,     0,   559,   487,
     488,     0,   489,   490,     0,   491,     0,   492,     0,     0,
       0,     0,     0,   493,     0,     0,     0,     0,     0,     0,
     494,     0,   495,   496,     0,     0,     0,     0,     0,     0,
       0,     0,   497,     0,   498,     0,     0,     0,     0,     0,
       0,   499,     0,   500,   501,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   502,   503,   504,   505,     0,
     506,     0,   507,     0,   508,   509,   510,     0,   511,     0,
     641,   512,     0,     0,     0,     0,   513,     0,     0,     0,
       0,     0,     0,   514,     0,     0,   515,     0,     0,     0,
     516,   517,   518,   519,     0,     0,     0,     0,     0,     0,
     520,     0,     0,     0,     0,   521,     0,     0,     0,   522,
       0,     0,   523,     0,     0,   524,     0,     0,     0,   525,
     526,   527,   528,   642,     0,   529,     0,     0,   530,     0,
       0,     0,     0,     0,     0,     0,   904,     0,     0,   531,
     532,   533,   534,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   643,   535,     0,     0,     0,     0,     0,     0,
       0,     0,   536,     0,   537,     0,     0,     0,     0,     0,
     538,   539,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   540,   541,     0,     0,   542,
     543,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   544,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   545,     0,     0,
       0,     0,   546,     0,   547,     0,     0,     0,     0,     0,
       0,     0,   548,     0,     0,   549,   550,   551,   552,   553,
     554,   555,     0,     0,     0,     0,   482,   556,     0,   557,
       0,   483,   484,     0,     0,   485,   486,   558,     0,     0,
       0,   559,   487,   488,     0,   489,   490,     0,   491,     0,
     492,     0,     0,     0,     0,     0,   493,     0,     0,     0,
       0,     0,     0,   494,     0,   495,   496,     0,     0,     0,
       0,     0,     0,     0,     0,   497,     0,   498,     0,     0,
       0,     0,     0,     0,   499,     0,   500,   501,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   502,   503,
     504,   505,     0,   506,     0,   507,     0,   508,   509,   510,
       0,   511,     0,   641,   512,     0,     0,     0,     0,   513,
       0,     0,     0,     0,     0,     0,   514,     0,     0,   515,
       0,     0,     0,   516,   517,   518,   519,     0,     0,     0,
       0,     0,     0,   520,     0,     0,     0,     0,   521,     0,
       0,     0,   522,     0,     0,   523,     0,     0,   524,     0,
       0,     0,   525,   526,   527,   528,   642,     0,   529,     0,
       0,   530,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   531,   532,   533,   534,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   643,   535,     0,     0,     0,
       0,     0,     0,     0,     0,   536,     0,   537,     0,     0,
       0,     0,     0,   538,   539,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   540,   541,
       0,     0,   542,   543,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   544,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     545,  1372,     0,     0,     0,   546,     0,   547,     0,     0,
       0,     0,     0,     0,     0,   548,     0,     0,   549,   550,
     551,   552,   553,   554,   555,     0,     0,     0,     0,   482,
     556,     0,   557,     0,   483,   484,     0,     0,   485,   486,
     558,     0,     0,     0,   559,   487,   488,     0,   489,   490,
       0,   491,     0,   492,     0,     0,     0,     0,     0,   493,
       0,     0,     0,     0,     0,     0,   494,     0,   495,   496,
       0,     0,     0,     0,     0,     0,     0,     0,   497,     0,
     498,     0,     0,     0,     0,     0,     0,   499,     0,   500,
     501,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   502,   503,   504,   505,     0,   506,     0,   507,     0,
     508,   509,   510,     0,   511,     0,   641,   512,     0,     0,
       0,     0,   513,     0,     0,     0,     0,     0,     0,   514,
       0,     0,   515,     0,     0,     0,   516,   517,   518,   519,
       0,     0,     0,     0,     0,     0,   520,     0,     0,     0,
       0,   521,     0,     0,     0,   522,     0,     0,   523,     0,
       0,   524,     0,     0,     0,   525,   526,   527,   528,   642,
       0,   529,     0,     0,   530,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   531,   532,   533,   534,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   643,   535,
       0,     0,     0,     0,     0,     0,     0,     0,   536,     0,
     537,     0,     0,     0,     0,     0,   538,   539,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   540,   541,     0,     0,   542,   543,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   544,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   545,     0,     0,     0,  1851,   546,     0,
     547,     0,     0,     0,     0,     0,     0,     0,   548,     0,
       0,   549,   550,   551,   552,   553,   554,   555,     0,     0,
       0,     0,   482,   556,     0,   557,     0,   483,   484,     0,
       0,   485,   486,   558,     0,     0,     0,   559,   487,   488,
       0,   489,   490,     0,   491,     0,   492,     0,     0,     0,
       0,     0,   493,     0,     0,     0,     0,     0,     0,   494,
       0,   495,   496,     0,     0,     0,     0,     0,     0,     0,
       0,   497,     0,   498,     0,     0,     0,     0,     0,     0,
     499,     0,   500,   501,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   502,   503,   504,   505,     0,   506,
       0,   507,     0,   508,   509,   510,     0,   511,     0,   641,
     512,     0,     0,     0,     0,   513,     0,     0,     0,     0,
       0,     0,   514,     0,     0,   515,     0,     0,     0,   516,
     517,   518,   519,     0,     0,     0,     0,     0,     0,   520,
       0,     0,     0,     0,   521,     0,     0,     0,   522,     0,
       0,   523,     0,     0,   524,     0,     0,     0,   525,   526,
     527,   528,   642,     0,   529,     0,     0,   530,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   531,   532,
     533,   534,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   643,   535,     0,     0,     0,     0,     0,     0,     0,
       0,   536,     0,   537,     0,     0,     0,     0,     0,   538,
     539,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   540,   541,     0,     0,   542,   543,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   544,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   545,  1890,     0,     0,
       0,   546,     0,   547,     0,     0,     0,     0,     0,     0,
       0,   548,     0,     0,   549,   550,   551,   552,   553,   554,
     555,     0,     0,     0,     0,   482,   556,     0,   557,     0,
     483,   484,     0,     0,   485,   486,   558,     0,     0,     0,
     559,   487,   488,     0,   489,   490,     0,   491,     0,   492,
       0,     0,     0,     0,     0,   493,     0,     0,     0,     0,
       0,     0,   494,     0,   495,   496,     0,     0,     0,     0,
       0,     0,     0,     0,   497,     0,   498,     0,     0,     0,
       0,     0,     0,   499,     0,   500,   501,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   502,   503,   504,
     505,     0,   506,     0,   507,     0,   508,   509,   510,     0,
     511,     0,   641,   512,     0,     0,     0,     0,   513,     0,
       0,     0,     0,     0,     0,   514,     0,     0,   515,     0,
       0,     0,   516,   517,   518,   519,     0,     0,     0,     0,
       0,     0,   520,     0,     0,     0,     0,   521,     0,     0,
       0,   522,     0,     0,   523,     0,     0,   524,     0,     0,
       0,   525,   526,   527,   528,   642,     0,   529,     0,     0,
     530,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   531,   532,   533,   534,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   643,   535,     0,     0,     0,     0,
       0,     0,     0,     0,   536,     0,   537,     0,     0,     0,
       0,     0,   538,   539,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   540,   541,     0,
       0,   542,   543,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   544,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   545,
    2223,     0,     0,     0,   546,     0,   547,     0,     0,     0,
       0,     0,     0,     0,   548,     0,     0,   549,   550,   551,
     552,   553,   554,   555,     0,     0,     0,     0,   482,   556,
       0,   557,     0,   483,   484,     0,     0,   485,   486,   558,
       0,     0,     0,   559,   487,   488,     0,   489,   490,     0,
     491,     0,   492,     0,     0,     0,     0,     0,   493,     0,
       0,     0,     0,     0,     0,   494,     0,   495,   496,     0,
       0,     0,     0,     0,     0,     0,     0,   497,     0,   498,
       0,     0,     0,     0,     0,     0,   499,     0,   500,   501,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     502,   503,   504,   505,     0,   506,     0,   507,     0,   508,
     509,   510,     0,   511,     0,   641,   512,     0,     0,     0,
       0,   513,     0,     0,     0,     0,     0,     0,   514,     0,
       0,   515,     0,     0,     0,   516,   517,   518,   519,     0,
       0,     0,     0,     0,     0,   520,     0,     0,     0,     0,
     521,     0,     0,     0,   522,     0,     0,   523,     0,     0,
     524,     0,     0,     0,   525,   526,   527,   528,   642,     0,
     529,     0,     0,   530,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   531,   532,   533,   534,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   643,   535,     0,
       0,     0,     0,     0,     0,     0,     0,   536,     0,   537,
       0,     0,     0,     0,     0,   538,   539,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     540,   541,     0,     0,   542,   543,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     544,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   545,  2785,     0,     0,     0,   546,     0,   547,
       0,     0,     0,     0,     0,     0,     0,   548,     0,     0,
     549,   550,   551,   552,   553,   554,   555,     0,     0,     0,
       0,   482,   556,     0,   557,     0,   483,   484,     0,     0,
     485,   486,   558,     0,     0,     0,   559,   487,   488,     0,
     489,   490,     0,   491,     0,   492,     0,     0,     0,     0,
       0,   493,     0,     0,     0,     0,     0,     0,   494,     0,
     495,   496,     0,     0,     0,     0,     0,     0,     0,     0,
     497,     0,   498,     0,     0,     0,     0,     0,     0,   499,
       0,   500,   501,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   502,   503,   504,   505,     0,   506,     0,
     507,     0,   508,   509,   510,     0,   511,     0,   641,   512,
       0,     0,     0,     0,   513,     0,     0,     0,     0,     0,
       0,   514,     0,     0,   515,     0,     0,     0,   516,   517,
     518,   519,     0,     0,     0,     0,     0,     0,   520,     0,
       0,     0,     0,   521,     0,     0,     0,   522,     0,     0,
     523,     0,     0,   524,     0,     0,     0,   525,   526,   527,
     528,   642,     0,   529,     0,     0,   530,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   531,   532,   533,
     534,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     643,   535,     0,     0,     0,     0,     0,     0,     0,     0,
     536,     0,   537,     0,     0,     0,     0,     0,   538,   539,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   540,   541,     0,     0,   542,   543,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   544,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   545,     0,     0,     0,     0,
     546,     0,   547,     0,     0,     0,  2894,     0,     0,     0,
     548,     0,     0,   549,   550,   551,   552,   553,   554,   555,
       0,     0,     0,     0,   482,   556,     0,   557,     0,   483,
     484,     0,     0,   485,   486,   558,     0,     0,     0,   559,
     487,   488,     0,   489,   490,     0,   491,     0,   492,     0,
       0,     0,     0,     0,   493,     0,     0,     0,     0,     0,
       0,   494,     0,   495,   496,     0,     0,     0,     0,     0,
       0,     0,     0,   497,     0,   498,     0,     0,     0,     0,
       0,     0,   499,     0,   500,   501,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   502,   503,   504,   505,
       0,   506,     0,   507,     0,   508,   509,   510,     0,   511,
       0,   641,   512,     0,     0,     0,     0,   513,     0,     0,
       0,     0,     0,     0,   514,     0,     0,   515,     0,     0,
       0,   516,   517,   518,   519,     0,     0,     0,     0,     0,
       0,   520,     0,     0,     0,     0,   521,     0,     0,     0,
     522,     0,     0,   523,     0,     0,   524,     0,     0,     0,
     525,   526,   527,   528,   642,     0,   529,     0,     0,   530,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     531,   532,   533,   534,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   643,   535,     0,     0,     0,     0,     0,
       0,     0,     0,   536,     0,   537,     0,     0,     0,     0,
       0,   538,   539,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   540,   541,     0,     0,
     542,   543,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   544,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   545,     0,
       0,     0,     0,   546,     0,   547,     0,     0,     0,     0,
       0,     0,     0,   548,     0,     0,   549,   550,   551,   552,
     553,   554,   555,     0,     0,     0,     0,   482,   556,     0,
     557,     0,   483,   484,     0,     0,   485,   486,   558,     0,
       0,     0,   559,   487,   488,     0,   489,   490,     0,   491,
       0,   492,     0,     0,     0,     0,     0,   493,     0,     0,
       0,     0,     0,     0,   494,     0,   495,   496,     0,     0,
       0,     0,     0,     0,     0,     0,   497,     0,   498,     0,
       0,     0,     0,     0,     0,   499,     0,   500,   501,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   502,
     503,   504,   505,     0,   506,     0,   507,     0,   508,   509,
     510,     0,   511,     0,     0,   512,     0,     0,     0,     0,
     513,     0,     0,     0,     0,     0,     0,   514,     0,     0,
     515,     0,     0,     0,   516,   517,   518,   519,     0,     0,
       0,     0,     0,     0,   520,     0,     0,     0,     0,   521,
       0,     0,     0,   522,     0,     0,   523,     0,     0,   524,
       0,     0,     0,   525,   526,   527,   528,   642,     0,   529,
       0,     0,   530,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   531,   532,   533,   534,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   643,   535,     0,     0,
       0,     0,     0,     0,     0,     0,   536,     0,   537,     0,
       0,     0,     0,     0,   538,   539,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   540,
     541,     0,     0,   542,   543,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   544,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   545,     0,     0,     0,     0,   546,     0,   547,     0,
       0,     0,     0,     0,     0,     0,   548,     0,     0,   549,
     550,   551,   552,   553,   554,   555,     0,     0,     0,     0,
     482,   556,     0,   557,     0,   483,   484,     0,     0,   485,
     486,   558,     0,     0,     0,   559,   487,   488,     0,   489,
     490,     0,   491,     0,   492,     0,     0,     0,     0,     0,
     493,     0,     0,     0,     0,     0,     0,   494,     0,   495,
     496,     0,     0,     0,     0,     0,     0,     0,     0,   497,
       0,   498,     0,     0,     0,     0,     0,     0,   499,     0,
     500,   501,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   502,   503,   504,   505,     0,   506,     0,   507,
       0,   508,   509,   510,     0,   511,     0,     0,   512,     0,
       0,     0,     0,   513,     0,     0,     0,     0,     0,     0,
     514,     0,     0,   515,     0,     0,     0,   516,   517,   518,
     519,     0,     0,     0,     0,     0,     0,   520,     0,     0,
       0,     0,   521,     0,     0,     0,   522,     0,     0,   523,
       0,     0,   524,     0,     0,     0,   525,   526,   527,   528,
       0,     0,   529,     0,     0,   530,     0,     0,     0,     0,
     893,     0,     0,     0,     0,     0,   531,   532,   533,   534,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     535,     0,     0,     0,     0,     0,     0,     0,     0,   536,
       0,   537,     0,     0,     0,     0,     0,   538,   539,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   540,   541,     0,     0,   542,   543,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   544,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   545,     0,     0,     0,     0,   546,
       0,   547,     0,     0,     0,     0,     0,     0,     0,   548,
       0,     0,   549,   550,   551,   552,   553,   554,   555,     0,
       0,     0,     0,   482,   556,     0,   557,     0,   483,   484,
       0,     0,   485,   486,   558,     0,     0,     0,   559,   487,
     488,     0,   489,   490,     0,   491,     0,   492,     0,     0,
       0,     0,     0,   493,     0,     0,     0,     0,     0,     0,
     494,     0,   495,   496,     0,     0,     0,     0,     0,     0,
       0,     0,   497,     0,   498,     0,     0,     0,     0,     0,
       0,   499,     0,   500,   501,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   502,   503,   504,   505,     0,
     506,     0,   507,     0,   508,   509,   510,     0,   511,     0,
       0,   512,     0,     0,     0,     0,   513,     0,     0,     0,
       0,     0,     0,   514,     0,     0,   515,     0,     0,     0,
     516,   517,   518,   519,     0,     0,     0,     0,     0,     0,
     520,     0,     0,     0,     0,   521,     0,     0,     0,   522,
       0,     0,   523,     0,     0,   524,     0,     0,     0,   525,
     526,   527,   528,     0,     0,   529,     0,     0,   530,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   531,
     532,   533,   534,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   535,     0,     0,     0,     0,     0,     0,
       0,     0,   536,     0,   537,     0,     0,     0,     0,     0,
     538,   539,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   540,   541,     0,     0,   542,
     543,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   544,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   545,     0,     0,
       0,   932,   546,     0,   547,     0,     0,     0,     0,     0,
       0,     0,   548,     0,     0,   549,   550,   551,   552,   553,
     554,   555,     0,     0,     0,     0,   482,   556,     0,   557,
       0,   483,   484,     0,     0,   485,   486,   558,     0,     0,
       0,   559,   487,   488,     0,   489,   490,     0,   491,     0,
     492,     0,     0,     0,     0,     0,   493,     0,     0,     0,
       0,  2796,     0,   494,     0,   495,   496,     0,     0,     0,
       0,     0,     0,     0,     0,   497,     0,   498,     0,     0,
       0,     0,     0,     0,   499,     0,   500,   501,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   502,   503,
     504,   505,     0,   506,     0,   507,     0,   508,   509,   510,
       0,   511,     0,     0,   512,     0,     0,     0,     0,   513,
       0,     0,     0,     0,     0,     0,   514,     0,     0,   515,
       0,     0,     0,   516,   517,   518,   519,     0,     0,     0,
       0,     0,     0,   520,     0,     0,     0,     0,   521,     0,
       0,     0,   522,     0,     0,   523,     0,     0,   524,     0,
       0,     0,   525,   526,   527,   528,     0,     0,   529,     0,
       0,   530,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   531,   532,   533,   534,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   535,     0,     0,     0,
       0,     0,     0,     0,     0,   536,     0,   537,     0,     0,
       0,     0,     0,   538,   539,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   540,   541,
       0,     0,   542,   543,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   544,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     545,     0,     0,     0,     0,   546,     0,   547,     0,     0,
       0,     0,     0,     0,     0,   548,     0,     0,   549,   550,
     551,   552,   553,   554,   555,     0,     0,     0,     0,   482,
     556,     0,   557,     0,   483,   484,     0,     0,   485,   486,
     558,     0,     0,     0,   559,   487,   488,     0,   489,   490,
       0,   491,     0,   492,     0,     0,     0,     0,     0,   493,
       0,     0,     0,     0,  2848,     0,   494,     0,   495,   496,
       0,     0,     0,     0,     0,     0,     0,     0,   497,     0,
     498,     0,     0,     0,     0,     0,     0,   499,     0,   500,
     501,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   502,   503,   504,   505,     0,   506,     0,   507,     0,
     508,   509,   510,     0,   511,     0,     0,   512,     0,     0,
       0,     0,   513,     0,     0,     0,     0,     0,     0,   514,
       0,     0,   515,     0,     0,     0,   516,   517,   518,   519,
       0,     0,     0,     0,     0,     0,   520,     0,     0,     0,
       0,   521,     0,     0,     0,   522,     0,     0,   523,     0,
       0,   524,     0,     0,     0,   525,   526,   527,   528,     0,
       0,   529,     0,     0,   530,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   531,   532,   533,   534,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   535,
       0,     0,     0,     0,     0,     0,     0,     0,   536,     0,
     537,     0,     0,     0,     0,     0,   538,   539,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   540,   541,     0,     0,   542,   543,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   544,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   545,     0,     0,     0,     0,   546,     0,
     547,     0,     0,     0,     0,     0,     0,     0,   548,     0,
       0,   549,   550,   551,   552,   553,   554,   555,     0,     0,
       0,     0,   482,   556,     0,   557,     0,   483,   484,     0,
       0,   485,   486,   558,     0,     0,     0,   559,   487,   488,
       0,   489,   490,     0,   491,     0,   492,     0,     0,     0,
       0,     0,   493,     0,     0,     0,     0,     0,     0,   494,
       0,   495,   496,     0,     0,     0,     0,     0,     0,     0,
       0,   497,     0,   498,     0,     0,     0,     0,     0,     0,
     499,     0,   500,   501,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   502,   503,   504,   505,     0,   506,
       0,   507,     0,   508,   509,   510,     0,   511,     0,     0,
     512,     0,     0,     0,     0,   513,     0,     0,     0,     0,
       0,     0,   514,     0,     0,   515,     0,     0,     0,   516,
     517,   518,   519,     0,     0,     0,     0,     0,     0,   520,
       0,     0,     0,     0,   521,     0,     0,     0,   522,     0,
       0,   523,     0,     0,   524,     0,     0,     0,   525,   526,
     527,   528,     0,     0,   529,     0,     0,   530,     0,     0,
       0,     0,  2874,     0,     0,     0,     0,     0,   531,   532,
     533,   534,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   535,     0,     0,     0,     0,     0,     0,     0,
       0,   536,     0,   537,     0,     0,     0,     0,     0,   538,
     539,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   540,   541,     0,     0,   542,   543,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   544,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   545,     0,     0,     0,
       0,   546,     0,   547,     0,     0,     0,     0,     0,     0,
       0,   548,     0,     0,   549,   550,   551,   552,   553,   554,
     555,     0,     0,     0,     0,   482,   556,     0,   557,     0,
     483,   484,     0,     0,   485,   486,   558,     0,     0,     0,
     559,   487,   488,     0,   489,   490,     0,   491,     0,   492,
       0,     0,     0,     0,     0,   493,     0,     0,     0,     0,
    3253,     0,   494,     0,   495,   496,     0,     0,     0,     0,
       0,     0,     0,     0,   497,     0,   498,     0,     0,     0,
       0,     0,     0,   499,     0,   500,   501,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   502,   503,   504,
     505,     0,   506,     0,   507,     0,   508,   509,   510,     0,
     511,     0,     0,   512,     0,     0,     0,     0,   513,     0,
       0,     0,     0,     0,     0,   514,     0,     0,   515,     0,
       0,     0,   516,   517,   518,   519,     0,     0,     0,     0,
       0,     0,   520,     0,     0,     0,     0,   521,     0,     0,
       0,   522,     0,     0,   523,     0,     0,   524,     0,     0,
       0,   525,   526,   527,   528,     0,     0,   529,     0,     0,
     530,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   531,   532,   533,   534,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   535,     0,     0,     0,     0,
       0,     0,     0,     0,   536,     0,   537,     0,     0,     0,
       0,     0,   538,   539,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   540,   541,     0,
       0,   542,   543,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   544,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   545,
       0,     0,     0,     0,   546,     0,   547,     0,     0,     0,
       0,     0,     0,     0,   548,     0,     0,   549,   550,   551,
     552,   553,   554,   555,     0,     0,     0,     0,   482,   556,
       0,   557,     0,   483,   484,     0,     0,   485,   486,   558,
       0,     0,     0,   559,   487,   488,     0,   489,   490,     0,
     491,     0,   492,     0,     0,     0,     0,     0,   493,     0,
       0,     0,     0,     0,     0,   494,     0,   495,   496,     0,
       0,     0,     0,     0,     0,     0,     0,   497,     0,   498,
       0,     0,     0,     0,     0,     0,   499,     0,   500,   501,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     502,   503,   504,   505,     0,   506,     0,   507,     0,   508,
     509,   510,     0,   511,     0,     0,   512,     0,     0,     0,
       0,   513,     0,     0,     0,     0,     0,     0,   514,     0,
       0,   515,     0,     0,     0,   516,   517,   518,   519,     0,
       0,     0,     0,     0,     0,   520,     0,     0,     0,     0,
     521,     0,     0,     0,   522,     0,     0,   523,     0,     0,
     524,     0,     0,     0,   525,   526,   527,   528,     0,     0,
     529,     0,     0,   530,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   531,   532,   533,   534,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   535,     0,
       0,     0,     0,     0,     0,     0,     0,   536,     0,   537,
       0,     0,     0,     0,     0,   538,   539,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     540,   541,     0,     0,   542,   543,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     544,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   545,     0,     0,     0,  3613,   546,     0,   547,
       0,     0,     0,     0,     0,     0,     0,   548,     0,     0,
     549,   550,   551,   552,   553,   554,   555,     0,     0,     0,
       0,   482,   556,     0,   557,     0,   483,   484,     0,     0,
     485,   486,   558,     0,     0,     0,   559,   487,   488,     0,
     489,   490,     0,   491,     0,   492,     0,     0,     0,     0,
       0,   493,     0,     0,     0,     0,     0,     0,   494,     0,
     495,   496,     0,     0,     0,     0,     0,     0,     0,     0,
     497,     0,   498,     0,     0,     0,     0,     0,     0,   499,
       0,   500,   501,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   502,   503,   504,   505,     0,   506,     0,
     507,     0,   508,   509,   510,     0,   511,     0,     0,   512,
       0,     0,     0,     0,   513,     0,     0,     0,     0,     0,
       0,   514,     0,     0,   515,     0,     0,     0,   516,   517,
     518,   519,     0,     0,     0,     0,     0,     0,   520,     0,
       0,     0,     0,   521,     0,     0,     0,   522,     0,     0,
     523,     0,     0,   524,     0,     0,     0,   525,   526,   527,
     528,     0,     0,   529,     0,     0,   530,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   531,   532,   533,
     534,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   535,     0,     0,     0,     0,     0,     0,     0,     0,
     536,     0,   537,     0,     0,     0,     0,     0,   538,   539,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   540,   541,     0,     0,   542,   543,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   544,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   545,     0,     0,     0,     0,
     546,     0,   547,     0,     0,     0,     0,     0,     0,     0,
     548,     0,     0,   549,   550,   551,   552,   553,   554,   555,
       0,     0,     0,     0,   482,   556,     0,   557,     0,   483,
     484,     0,     0,   485,   486,   558,     0,     0,     0,   559,
     487,   488,     0,   489,   490,     0,   491,     0,   492,     0,
       0,     0,     0,     0,   493,     0,     0,     0,     0,     0,
       0,   494,     0,   495,   496,     0,     0,     0,     0,     0,
       0,     0,     0,   497,     0,   498,     0,     0,     0,     0,
       0,     0,   499,     0,   500,   501,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   502,   503,   504,   505,
       0,   506,     0,   507,     0,   508,   509,   510,     0,   511,
       0,     0,   512,     0,     0,     0,     0,   513,     0,     0,
       0,     0,     0,     0,   514,     0,     0,     0,     0,     0,
       0,   516,   517,   518,   519,     0,     0,     0,     0,     0,
       0,   520,     0,     0,     0,     0,   521,     0,     0,     0,
     522,     0,     0,   523,     0,     0,   524,     0,     0,     0,
     525,   526,   527,   528,     0,     0,   529,     0,     0,   530,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     531,   532,   533,   534,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   535,     0,     0,     0,     0,     0,
       0,     0,     0,   536,     0,   537,     0,     0,     0,     0,
       0,   538,   539,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   482,     0,     0,   540,   541,   483,   484,
     542,   543,   485,   486,     0,     0,     0,     0,     0,     0,
     899,     0,   489,   490,     0,   491,   544,   492,     0,     0,
       0,     0,     0,   493,     0,     0,     0,     0,   545,     0,
     494,     0,   495,   496,     0,     0,     0,     0,     0,     0,
       0,     0,   497,   548,   498,     0,   549,   550,   551,   552,
     553,   554,   555,   500,   501,     0,     0,     0,   556,     0,
     557,     0,     0,     0,     0,     0,     0,     0,   558,     0,
       0,     0,   559,     0,     0,   502,   503,     0,   505,     0,
     506,     0,   507,     0,   508,   509,   510,     0,   511,     0,
       0,  1523,     0,     0,     0,     0,   513,     0,     0,     0,
       0,     0,     0,   514,     0,     0,     0,     0,     0,     0,
     516,   517,   518,   519,     0,     0,     0,     0,     0,     0,
     520,     0,     0,     0,     0,   521,     0,     0,     0,   522,
       0,     0,   523,     0,     0,   524,     0,     0,     0,     0,
     526,   527,     0,     0,     0,   529,     0,     0,   530,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   531,
     532,   533,   534,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   535,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   537,     0,     0,     0,     0,     0,
       0,   539,     0,   482,     0,     0,     0,     0,   483,   484,
       0,     0,   485,   486,     0,   540,   541,     0,     0,     0,
     899,     0,   489,   490,     0,   491,     0,   492,     0,     0,
       0,     0,     0,   493,     0,     0,     0,     0,  1524,  1525,
     494,     0,   495,   496,     0,     0,     0,  1526,     0,     0,
       0,     0,   497,     0,   498,     0,     0,     0,     0,     0,
       0,     0,     0,   500,   501,  1527,  1528,   551,  1529,  1530,
     554,   555,     0,     0,     0,     0,     0,   556,     0,   557,
       0,     0,     0,     0,     0,   502,   503,   558,   505,     0,
     506,   559,   507,     0,   508,   509,   510,     0,   511,     0,
       0,     0,     0,     0,     0,     0,   513,     0,     0,     0,
       0,     0,     0,   514,     0,     0,     0,     0,     0,     0,
     516,   517,   518,   519,     0,     0,     0,     0,     0,     0,
     520,     0,     0,     0,  1358,   521,     0,     0,     0,   522,
       0,     0,   523,     0,     0,   524,     0,     0,     0,     0,
     526,   527,     0,     0,     0,   529,     0,     0,   530,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   531,
     532,   533,   534,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   535,     0,     0,     0,     0,     0,     0,
     482,     0,     0,     0,   537,   483,   484,     0,     0,   485,
     486,   539,     0,     0,     0,     0,     0,   899,     0,   489,
     490,     0,   491,     0,   492,   540,   541,     0,     0,     0,
     493,     0,     0,     0,     0,     0,     0,   494,     0,   495,
     496,     0,     0,     0,     0,     0,     0,     0,     0,   497,
       0,   498,     0,     0,     0,     0,     0,  1561,     0,     0,
     500,   501,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     554,   555,   502,   503,     0,   505,     0,   506,     0,   507,
       0,   508,   509,   510,     0,   511,     0,   558,     0,     0,
       0,   559,     0,   513,     0,     0,     0,     0,     0,     0,
     514,     0,     0,     0,     0,     0,     0,   516,   517,   518,
     519,     0,     0,     0,     0,     0,     0,   520,     0,     0,
       0,     0,   521,     0,     0,     0,   522,     0,     0,   523,
       0,     0,   524,     0,     0,     0,     0,   526,   527,     0,
       0,     0,   529,     0,     0,   530,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   531,   532,   533,   534,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     535,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   537,     0,     0,     0,     0,     0,     0,   539,     0,
     482,     0,     0,     0,     0,   483,   484,     0,     0,   485,
     486,     0,   540,   541,     0,     0,     0,   899,     0,   489,
     490,     0,   491,     0,   492,     0,  1684,     0,     0,     0,
     493,     0,     0,     0,     0,     0,     0,   494,     0,   495,
     496,     0,     0,     0,     0,     0,     0,     0,     0,   497,
       0,   498,     0,     0,     0,     0,     0,     0,     0,     0,
     500,   501,     0,  1790,     0,     0,     0,   554,   555,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   502,   503,   558,   505,     0,   506,   559,   507,
       0,   508,   509,   510,     0,   511,     0,     0,     0,     0,
       0,     0,     0,   513,     0,     0,     0,     0,     0,     0,
     514,     0,     0,     0,     0,     0,     0,   516,   517,   518,
     519,     0,     0,     0,     0,     0,     0,   520,     0,     0,
       0,     0,   521,     0,     0,     0,   522,     0,     0,   523,
       0,     0,   524,     0,     0,     0,     0,   526,   527,     0,
       0,     0,   529,     0,     0,   530,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   531,   532,   533,   534,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     535,     0,     0,     0,     0,     0,     0,   482,     0,     0,
       0,   537,   483,   484,     0,     0,   485,   486,   539,     0,
       0,     0,     0,     0,   899,     0,   489,   490,     0,   491,
       0,   492,   540,   541,     0,     0,     0,   493,     0,     0,
       0,     0,     0,     0,   494,     0,   495,   496,     0,     0,
       0,     0,     0,     0,     0,     0,   497,     0,   498,     0,
       0,     0,     0,     0,     0,     0,     0,   500,   501,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   554,   555,   502,
     503,     0,   505,     0,   506,     0,   507,     0,   508,   509,
     510,     0,   511,     0,   558,     0,     0,     0,   559,     0,
     513,     0,     0,     0,     0,     0,     0,   514,     0,     0,
       0,     0,     0,     0,   516,   517,   518,   519,     0,     0,
       0,     0,     0,     0,   520,     0,     0,     0,     0,   521,
       0,     0,     0,   522,     0,     0,   523,     0,     0,   524,
       0,     0,     0,     0,   526,   527,     0,     0,     0,   529,
       0,     0,   530,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   531,   532,   533,   534,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   535,     0,     0,
       0,     0,     0,     0,   482,     0,     0,     0,   537,   483,
     484,     0,     0,   485,   486,   539,     0,     0,     0,     0,
       0,   899,     0,   489,   490,     0,   491,     0,   492,   540,
     541,     0,     0,     0,   493,     0,     0,     0,     0,     0,
       0,   494,     0,   495,   496,     0,     0,     0,     0,     0,
       0,     0,     0,   497,     0,   498,     0,     0,     0,     0,
       0,     0,     0,     0,   500,   501,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,  1871,     0,   554,   555,   502,   503,     0,   505,
       0,   506,     0,   507,     0,   508,   509,   510,     0,   511,
       0,   558,     0,     0,     0,   559,     0,   513,     0,     0,
       0,     0,     0,     0,   514,     0,     0,     0,     0,     0,
       0,   516,   517,   518,   519,     0,     0,     0,     0,     0,
       0,   520,     0,     0,     0,     0,   521,     0,     0,     0,
     522,     0,     0,   523,     0,     0,   524,     0,     0,     0,
       0,   526,   527,     0,     0,     0,   529,     0,     0,   530,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     531,   532,   533,   534,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   535,     0,     0,     0,     0,     0,
       0,   482,     0,     0,     0,   537,   483,   484,     0,     0,
     485,   486,   539,     0,     0,     0,     0,     0,   899,     0,
     489,   490,     0,   491,     0,   492,   540,   541,     0,     0,
       0,   493,     0,     0,     0,     0,     0,     0,   494,     0,
     495,   496,     0,     0,     0,     0,     0,     0,     0,     0,
     497,     0,   498,     0,     0,     0,     0,     0,  2296,     0,
       0,   500,   501,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   554,   555,   502,   503,     0,   505,     0,   506,     0,
     507,     0,   508,   509,   510,     0,   511,     0,   558,     0,
       0,     0,   559,     0,   513,     0,     0,     0,     0,     0,
       0,   514,     0,     0,     0,     0,     0,     0,   516,   517,
     518,   519,     0,     0,     0,     0,     0,     0,   520,     0,
       0,     0,     0,   521,     0,     0,     0,   522,     0,     0,
     523,     0,     0,   524,     0,     0,     0,     0,   526,   527,
       0,     0,     0,   529,     0,     0,   530,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   531,   532,   533,
     534,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   535,     0,     0,     0,     0,     0,     0,   482,     0,
       0,     0,   537,   483,   484,     0,     0,   485,   486,   539,
       0,     0,     0,     0,     0,   899,     0,   489,   490,     0,
     491,     0,   492,   540,   541,     0,     0,     0,   493,     0,
       0,     0,     0,     0,     0,   494,     0,   495,   496,     0,
       0,     0,     0,     0,     0,     0,     0,   497,     0,   498,
       0,     0,     0,     0,     0,  3551,     0,     0,   500,   501,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   554,   555,
     502,   503,     0,   505,     0,   506,     0,   507,     0,   508,
     509,   510,     0,   511,     0,   558,     0,     0,     0,   559,
       0,   513,     0,     0,     0,     0,     0,     0,   514,     0,
       0,     0,     0,     0,     0,   516,   517,   518,   519,     0,
       0,     0,     0,     0,     0,   520,     0,     0,     0,     0,
     521,     0,     0,     0,   522,     0,     0,   523,     0,     0,
     524,     0,     0,     0,     0,   526,   527,     0,     0,     0,
     529,     0,     0,   530,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   531,   532,   533,   534,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   535,     0,
       0,     0,     0,     0,     0,   482,     0,     0,     0,   537,
     483,   484,     0,     0,   485,   486,   539,     0,     0,     0,
       0,     0,   899,     0,   489,   490,     0,   491,     0,   492,
     540,   541,     0,     0,     0,   493,     0,     0,     0,     0,
       0,     0,   494,     0,   495,   496,     0,     0,     0,     0,
       0,     0,     0,     0,   497,     0,   498,     0,     0,     0,
       0,     0,  3615,     0,     0,   500,   501,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   554,   555,   502,   503,     0,
     505,     0,   506,     0,   507,     0,   508,   509,   510,     0,
     511,     0,   558,     0,     0,     0,   559,     0,   513,     0,
       0,     0,     0,     0,     0,   514,     0,     0,     0,     0,
       0,     0,   516,   517,   518,   519,     0,     0,     0,     0,
       0,     0,   520,     0,     0,     0,     0,   521,     0,     0,
       0,   522,     0,     0,   523,     0,     0,   524,     0,     0,
       0,     0,   526,   527,     0,     0,     0,   529,     0,     0,
     530,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   531,   532,   533,   534,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   535,     0,     0,     0,     0,
       0,     0,   482,     0,     0,     0,   537,   483,   484,     0,
       0,   485,   486,   539,     0,     0,     0,     0,     0,   899,
       0,   489,   490,     0,   491,     0,   492,   540,   541,     0,
       0,     0,   493,     0,     0,     0,     0,     0,     0,   494,
       0,   495,   496,     0,     0,     0,     0,     0,     0,     0,
       0,   497,     0,   498,     0,     0,     0,     0,     0,  3755,
       0,     0,   500,   501,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   554,   555,   502,   503,     0,   505,     0,   506,
       0,   507,     0,   508,   509,   510,     0,   511,     0,   558,
       0,     0,     0,   559,     0,   513,     0,     0,     0,     0,
       0,     0,   514,     0,     0,     0,     0,     0,     0,   516,
     517,   518,   519,     0,     0,     0,     0,     0,     0,   520,
       0,   687,     0,     0,   521,     0,     0,     0,   522,     0,
       0,   523,     0,     0,   524,     0,     0,     0,     0,   526,
     527,   145,   688,     0,   529,     0,     0,   530,   689,     0,
       0,     0,     0,     0,   150,   151,   152,     0,   531,   532,
     533,   534,   154,     0,     0,     0,     0,     0,     0,     0,
     156,     0,   535,     0,     0,     0,     0,     0,     0,     0,
       0,   690,     0,   537,     0,     0,     0,   160,   687,     0,
     539,     0,     0,     0,   161,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   540,   541,     0,     0,   145,   688,
     162,   163,     0,     0,     0,   689,     0,     0,     0,     0,
       0,   150,   151,   152,     0,     0,     0,     0,     0,   154,
       0,     0,     0,     0,     0,     0,     0,   156,     0,     0,
       0,     0,     0,     0,   166,   167,     0,   691,   690,   169,
     170,     0,     0,   171,   160,     0,     0,     0,     0,   554,
     555,   161,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   173,     0,     0,     0,   558,   162,   163,     0,
     559,     0,   174,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   175,     0,     0,
       0,   166,   167,     0,   691,     0,   169,   170,   176,   177,
     171,     0,     0,     0,     0,   178,     0,     0,     0,     0,
       0,     0,     0,     0,   179,     0,     0,     0,     0,   173,
     180,   181,     0,     0,     0,  2603,     0,     0,     0,   174,
       0,     0,     0,     0,     0,  2604,     0,     0,     0,  2605,
       0,     0,  2606,   182,     0,     0,  2607,     0,     0,     0,
       0,  2608,     0,     0,   175,     0,     0,  2609,     0,   151,
       0,     0,     0,   692,     0,   176,   177,   183,     0,     0,
       0,     0,   178,     0,   156,     0,  2916,     0,     0,  2610,
       0,   179,  2611,     0,     0,  2612,     0,   180,   181,   189,
    2613,     0,     0,     0,  2614,     0,     0,     0,   161,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     182,     0,     0,  2615,   162,   163,     0,     0,     0,     0,
       0,  2616,     0,     0,     0,     0,  2617,     0,     0,     0,
     696,     0,     0,     0,   183,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,  2618,  2603,
       0,  2619,     0,   169,  1358,     0,   189,  2620,     0,  2604,
       0,     0,     0,  2605,     0,     0,  2606,     0,     0,     0,
    2607,     0,     0,     0,     0,  2608,   173,     0,     0,     0,
       0,  2609,     0,   151,     0,  2621,  1560,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,  3135,   156,     0,
       0,     0,     0,  2610,     0,     0,  2611,     0,     0,  2612,
    2622,     0,     0,     0,  2613,     0,     0,     0,  2614,     0,
       0,     0,   161,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,  2615,   162,   163,
       0,     0,     0,     0,     0,  2616,     0,     0,  2603,     0,
    2617,     0,     0,     0,     0,     0,     0,     0,  2604,     0,
       0,     0,  2605,     0,     0,  2606,     0,  1561,     0,  2607,
       0,     0,  2618,     0,  2608,  2619,     0,   169,  1358,  2623,
    2609,  2620,   151,     0,     0,     0,     0,     0,     0,     0,
     554,   555,     0,     0,     0,     0,     0,   156,     0,     0,
     173,     0,  2610,     0,     0,  2611,     0,     0,  2612,  2621,
    1560,     0,     0,  2613,     0,     0,     0,  2614,     0,     0,
       0,   161,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,  2622,     0,  2615,   162,   163,     0,
       0,     0,     0,     0,  2616,     0,     0,     0,     0,  2617,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,  2618,     0,     0,  2619,     0,   169,  1358,     0,     0,
    2620,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,  1561,     0,     0,     0,     0,     0,     0,     0,   173,
       0,     2,     0,  2623,     3,     4,     5,     0,  2621,  1560,
       6,     7,     8,     9,   554,   555,    10,    11,    12,     0,
      13,    14,     0,    15,    16,     0,    17,    18,    19,    20,
      21,     0,     0,  2622,     0,     0,     0,    23,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      24,    25,    26,    27,    28,    29,    30,    31,    32,    33,
      34,    35,    36,    37,    38,    39,    40,    41,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
    1561,     0,     3,     4,     5,     0,     0,     0,     6,     7,
       0,     9,  2623,     0,    10,    11,     0,     0,    13,    14,
       0,     0,    16,   554,   555,    18,    19,    20,    21,     0,
    1193,     0,     0,     0,     0,    23,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    24,    25,
      26,    27,    28,    29,    30,    31,    32,    33,    34,    35,
      36,    37,    38,    39,    40,    41,     3,     4,     5,     0,
       0,     0,     6,     7,     0,     9,     0,     0,    10,    11,
       0,     0,    13,    14,     0,     0,    16,     0,     0,    18,
      19,    20,    21,     0,     0,     0,     0,     0,     0,    23,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    24,    25,    26,    27,    28,    29,    30,    31,
      32,    33,    34,    35,    36,    37,    38,    39,    40,    41
};

static const yytype_int16 yycheck[] =
{
     150,   920,   168,  1105,   355,   968,   604,  1811,   927,  1906,
    1994,  1869,  1064,   364,  1735,   917,  1369,  1862,  1371,  1837,
     352,  1605,  1908,  1066,   728,   127,   175,  1120,  2101,  1243,
     910,   911,     0,  2230,  1052,  1861,  2573,  1058,  1117,     7,
    2324,   152,   128,    53,  1074,   156,  1736,  1393,   723,    52,
     161,   162,   163,  1743,   122,    58,   905,   733,  2107,   170,
    2076,  2110,   173,  1328,  2620,  1234,   617,   336,  2149,  2118,
    1325,     0,  1759,   872,  2715,  2807,  2639,  2809,  2965,  2996,
    2604,  1334,  2621,   963,    52,   349,    54,  1340,    66,  1953,
      58,  2615,   376,  2899,    62,   105,  2380,  1561,    66,  2693,
    2694,  2695,  1566,  2184,  1357,  2334,  2009,    53,  2392,   931,
    2956,  1436,   261,  2167,    82,  2263,  1360,   120,  2266,  2283,
    2284,  2494,  2486,  2450,  2188,    54,  2944,  2945,  2946,  3116,
      59,    60,  1123,  1330,  2545,    64,  3194,  2183,  2634,  2873,
    2856,   344,  1338,    39,  3131,    39,   854,   166,   297,   298,
     299,  1988,   120,    39,   122,    39,   364,   125,   422,    60,
      60,   108,  3127,    39,  3147,   133,   315,  3369,  1820,   115,
     116,   813,     7,     8,     9,    39,   105,  3115,    13,    14,
    2280,    16,   111,   167,    19,    20,   103,   136,    23,    24,
     115,  1013,    27,   122,   123,    30,    31,    32,    33,   200,
      39,   816,   104,   118,   126,    40,    61,    62,   131,   107,
    3520,   116,    39,   180,   205,   112,    64,  2773,    42,    43,
      44,    45,    46,    47,    48,    49,    50,    51,    63,    64,
      65,    66,    67,    68,    69,    70,    71,    72,    73,    74,
      75,    76,    77,    78,    79,    80,   221,   349,    83,    84,
      85,   142,   111,    52,    53,    54,    91,    92,    93,    94,
      95,    96,   175,   111,   138,   126,   165,   205,  3065,  2315,
    3187,   247,  2175,   182,   122,   123,   118,   266,   124,   104,
      97,  1360,   141,   276,   104,   227,   435,   436,   437,   438,
     237,   440,   441,   109,   443,   444,   260,   135,   159,   212,
    3557,   295,   311,   312,   110,  2988,   175,   269,   270,   247,
    2873,   206,   227,   262,   463,   464,   465,   466,   353,   182,
     422,  1565,  1690,   349,   143,   371,  2372,  2890,   396,   355,
    3317,   116,   335,   152,    81,  2874,   371,   487,   236,   342,
     209,   127,   112,  2216,   308,   115,  2219,   143,   104,  2430,
    3660,   107,   398,  1396,  2418,   317,   152,   248,   221,   257,
     180,   199,   371,   258,   353,   134,   212,   335,  3306,   266,
     231,   355,   129,   366,   342,  1743,  2573,   371,   371,  3095,
     371,   290,   198,   153,   352,   158,   354,   104,   355,   162,
     107,  2555,  2473,   339,  2239,   371,   371,  3335,  3336,   345,
     751,   402,   240,   220,   755,   415,   757,   758,   759,  2455,
     761,   129,   415,   342,   128,   353,  2827,   749,   364,   317,
     771,  2521,   371,  3733,   371,  3682,   131,   577,   396,   371,
    2288,   580,   581,  1485,   583,   584,   585,   586,   587,   588,
    2298,   590,   355,   592,  1487,  2309,   371,   415,   371,   371,
    2550,  3653,   420,   370,   371,   378,   371,  3767,  1488,  1489,
     120,  1380,  1381,   388,   733,  1016,   371,   396,   370,   371,
     295,   317,   371,   371,   371,  3272,   740,   809,   148,   131,
     371,   250,   238,   357,  1364,  2388,   415,  3474,   347,  1040,
    1041,  1042,  1043,  1044,  1045,  1046,  1047,  1048,  1049,  1050,
    1051,   770,   786,   787,   788,  3488,  2343,  3472,  3102,  1151,
     371,   353,  1431,  3319,   408,   784,   665,   666,   170,   668,
    2172,   238,   688,   407,   425,   691,   426,   388,  3211,  3587,
    3588,   407,   142,   371,   430,   738,   805,  1152,   414,  1419,
    3478,  3479,   745,   407,   430,   409,   160,   755,   396,  3395,
     814,  2454,  1817,   167,  1262,   169,   165,   171,   142,   148,
     174,   175,   368,   349,   370,   371,   421,   422,   407,   354,
     356,  3127,   368,  3289,  1770,   414,   362,   363,  3312,  2863,
     407,   366,   200,   175,  3571,   299,  3125,   414,   358,   428,
     376,   348,   252,   136,   429,  3119,   353,   228,   112,   426,
    3124,   370,   371,   427,   360,   361,  3523,  1526,   407,  3133,
     409,   212,   411,   386,  2598,   107,  3112,   209,  1815,   206,
    2984,  3508,  3509,  2496,  2497,   395,  2610,  1618,  3001,  2339,
     348,  2504,   292,   403,   404,   353,   422,   203,   740,   205,
     300,   101,  1967,   360,  2690,   243,   268,   261,   124,   192,
     203,   205,   205,   129,   138,  3642,  2820,  2821,  2806,  2986,
    3071,  3507,  1484,   150,   151,  2649,   371,   131,   203,  3586,
     205,  3729,  3730,   378,   124,  2756,  3232,   110,   332,  2752,
    2964,   749,  2585,   297,   298,   299,   146,  2741,  2533,  1226,
    2754,  2755,   307,   191,  1938,  2711,   167,   367,  1942,   153,
     138,   315,  2931,     0,   164,   319,   170,   377,   857,   379,
     370,   371,   814,   182,   412,   167,   865,   371,  2582,   371,
    3257,   138,   138,   161,   873,   412,    87,   219,    89,   117,
     428,   383,   128,   882,   205,  2422,   212,   886,   353,   240,
    1338,   809,   429,  1957,   353,   259,  3309,   161,   181,  3312,
     234,   135,   221,   371,   355,   765,   387,    54,  3321,   143,
     370,   371,   232,   209,   216,   257,   353,  2593,   152,   186,
     186,   269,   394,   371,   317,   367,  3594,   237,   367,  1316,
     240,   749,   266,   205,  3323,  3324,   370,   371,   377,  2692,
     379,  1142,  2618,   243,   943,  1594,   250,   389,  3322,   166,
     366,   769,  2266,   813,   772,   954,  1138,  3694,  3695,   755,
     813,   249,   961,   366,   312,   199,   139,   371,   371,   306,
     434,   435,   436,   437,   438,   317,   440,   441,   442,   443,
     444,   366,   446,   447,  1432,   249,   765,  3643,   816,  3657,
    3658,   809,   456,   811,   240,   813,   234,   120,   816,   463,
     464,   465,   466,   178,  1906,   202,   240,   471,   472,   394,
    1009,   475,  1941,   347,  1907,  1014,  1015,  1418,  2082,  1153,
     371,   347,  1136,   371,   893,   205,   193,  1027,   897,  1029,
    2226,  3687,   353,  1901,  1034,  1035,  1036,  1037,  1038,  1039,
    1565,   360,  2061,   216,  2604,  2248,  2249,  1990,   203,   182,
     205,   371,   371,   299,  1053,  2615,  2159,   371,  1058,   928,
    2620,   371,  1930,  2168,  3477,  2180,  3472,   142,  2173,   383,
     372,   367,  1952,  3517,   538,  3006,  2999,   222,  1192,  2639,
     347,   347,   384,   371,   219,   203,   236,   205,   221,   391,
     392,  1860,   237,   389,   169,   397,  2962,   237,   295,   371,
    1626,   240,  1101,   405,   740,   243,   139,   122,  1108,   282,
     203,   156,   205,   577,   578,   579,   580,   581,  3017,   583,
     584,   585,   586,   587,   588,   589,   590,   591,   592,   252,
     594,   595,  1884,  1885,  1864,   642,   643,   371,   241,   180,
     604,   179,   317,   218,  1843,   178,   291,   128,  3592,  3593,
     786,   787,   788,   789,   790,   791,   792,   793,   794,   795,
     796,   797,   798,   799,   800,   801,   802,   803,   804,   292,
    3752,  3662,   193,   248,   371,   128,   251,   300,   814,   346,
     347,   304,   349,   104,  1136,  3551,   109,   313,   355,  2865,
     113,   371,   115,   234,   179,   659,   243,   370,   371,   212,
     317,   665,   666,   667,   668,   669,   670,   271,   122,   132,
    3257,   384,   412,  2773,   252,   247,   371,   275,   391,   392,
    1138,   371,   237,   256,   145,   266,   371,   360,  2768,   429,
     153,   371,   371,   371,   243,   370,   371,    25,   371,   284,
    1192,  3732,   360,  1112,  1113,   371,  1245,   370,   371,   287,
      38,    39,  3386,    41,   371,  1191,     5,     6,   279,   234,
     269,    10,    11,    12,    52,    53,   150,   252,    17,    18,
     268,   374,  3763,    22,  2176,   198,    25,   258,   371,    28,
      81,   138,  1135,   347,   178,    34,   143,   390,    37,    38,
      39,  1151,    41,   234,   150,   152,   371,  2199,   401,  3222,
     223,  3792,   287,    52,    53,   258,   347,  3141,   366,  3675,
     348,  2514,  2515,  2873,  3022,  3023,   139,  1135,   212,  1318,
    1138,   353,  1770,   237,   371,   266,   125,   221,   545,   342,
    2890,   124,   104,    25,   347,   107,   129,    97,   272,   234,
    2211,   167,   355,  3051,   143,  1345,    38,    39,  1348,    41,
      99,   234,   178,   152,   153,   178,  2308,   178,   157,   158,
      52,    53,   371,   348,   104,  2294,  3753,   107,   291,   274,
     367,   266,   347,   371,   371,   104,   139,   217,   107,   263,
    3514,   104,  3516,  3672,   107,   394,   212,   244,   104,   264,
     237,   107,  2260,   857,   161,   221,   394,  1358,   321,  1399,
     864,   865,   272,    98,   868,   869,   347,   263,  3795,   873,
     142,  2279,   139,   347,   878,   178,   310,  1942,   882,   212,
     348,   885,   886,   317,   641,  1755,   283,   105,   106,   315,
     371,  1632,  1633,   256,  1635,   256,   283,   115,   278,   903,
     904,   240,   370,   371,   289,  3016,   206,   370,   371,   209,
    3739,   178,   347,   250,   247,  1637,  1638,  1639,   176,   219,
    3755,   366,    97,  3139,   347,   929,   238,   931,   146,  3303,
    3146,   189,   936,  3396,   938,   153,   323,   347,   353,   943,
    2232,  2233,   249,    97,   310,   371,   218,  3782,  1818,   250,
     954,   317,  3160,   256,  1363,  1896,   293,   961,   238,   385,
    1136,  1501,  1502,   235,   968,   138,   371,  1626,  1909,   238,
     143,   243,   180,  3802,  3803,   238,   248,  1153,   221,   152,
    2250,   347,   238,  1642,   371,   370,   371,   392,   179,   256,
     262,    97,   293,  1040,  1041,  1042,  1043,  1044,  1045,  1046,
    1047,  1048,  1049,  1050,  1051,  1009,  1010,  1011,   367,  1013,
    1014,  1015,   277,   256,   286,  2285,  1192,   208,     0,  3119,
     212,    97,   277,   281,  3124,   121,   298,  3127,   121,   221,
     389,   206,   371,  3133,   209,    98,  1040,  1041,  1042,  1043,
    1044,  1045,  1046,  1047,  1048,  1049,  1050,  1051,  1052,  1053,
     258,  2787,   206,  3027,  1058,   209,    97,  2800,  2801,  1063,
     276,   252,  1066,  1067,  1068,  1605,  1070,  1071,    97,   212,
     166,   244,    54,   291,    98,  2818,  2819,    59,    60,   407,
     313,  1620,  2686,   175,    97,   413,   414,   415,   416,   417,
     418,  2583,   317,   311,   312,   191,   287,  1101,   191,   371,
     206,  3475,   430,   209,   247,   370,   371,   200,   201,   243,
     283,   259,   285,   219,  1118,   370,   371,  2559,   407,   215,
     212,    25,   215,   348,   413,   414,   415,   416,   417,   418,
     206,   224,  3232,   209,    38,    39,   893,    41,   371,   428,
     897,   430,   375,   219,  2564,    25,   371,   384,    52,    53,
     366,   412,  1644,   232,   391,   371,   146,   253,    38,    39,
     253,    41,   201,   920,   243,   206,  3753,   428,   209,   317,
     927,   928,    52,    53,   276,   407,    64,   206,    25,   281,
     209,   413,   414,   415,   416,   417,   418,  1726,   322,   323,
    2673,    38,    39,   206,    41,  3569,   209,   429,   430,   367,
    2509,  1205,    25,   371,   111,    52,    53,   193,  3795,  3309,
     200,   201,  3312,   347,   269,    38,    39,   317,    41,   294,
     317,  3321,  3322,   111,  3500,   211,   111,  1767,  1768,    52,
      53,   179,   307,   288,   141,   123,   370,   371,   218,  2531,
    2532,  1245,   190,  2337,   347,   193,  2516,   237,   348,  1788,
    1789,   348,   355,   515,  1258,   235,   141,   312,   317,    25,
    2552,  2553,    39,   243,   366,  1022,  1023,   142,   248,   371,
    2258,   371,    38,    39,   371,    41,  2017,  2018,   353,  1637,
    1638,  1639,  3490,   235,   546,   547,    52,    53,   294,   348,
    1830,   243,  3403,  2015,  3037,  3038,   352,  2739,   354,   338,
     339,   307,  2243,  2244,   252,  3784,   286,  3542,   168,   430,
     366,  1315,   371,   163,  1318,  3689,   371,   177,   298,   127,
    1324,  1325,   347,   183,   168,   185,   365,   366,  3807,   368,
     369,   415,   416,   177,  1338,   143,    25,  1341,   188,   287,
     412,   185,    86,   218,   152,   370,   371,  3721,  1352,    38,
      39,   232,    41,   124,    98,  1112,  1113,   429,   129,    90,
     235,   205,  2854,    52,    53,   412,  2808,   242,   243,    90,
     230,  1911,  3472,   248,  1914,   256,   347,  3477,   412,  3666,
     176,   428,   412,  2825,   355,   181,   230,   262,   168,  3583,
     412,   371,  1396,   189,   428,  1399,  1400,   177,   428,   137,
    1404,   354,   197,   278,   200,   185,   428,   143,    25,   204,
    1414,   286,   382,   366,   412,   368,   152,   352,   371,   354,
     191,    38,    39,   298,    41,   205,   396,   137,  1432,  2871,
     428,   366,   352,   384,   354,    52,    53,  1977,   347,  3413,
     391,   212,     5,     6,   215,   358,   366,    10,    11,    12,
     230,   367,  1861,   358,    17,    18,   347,   370,   371,    22,
     102,   377,    25,   379,   355,    28,   190,   367,  1644,   193,
    2823,    34,   114,  1477,    37,    38,    39,   377,    41,   379,
    1484,   354,   253,  1487,   358,   281,   354,  3735,   143,    52,
      53,    25,   143,   366,   106,   358,   371,   152,   366,  2791,
    2792,   152,   339,   407,    38,    39,   118,    41,   345,   413,
     414,   415,   416,   417,   418,  3258,  3259,   346,    52,    53,
     347,   367,   370,   371,   428,   371,   355,   407,   355,   367,
    3778,  3779,   367,   413,   414,   415,   416,   417,   418,   377,
     430,   379,   377,   367,   379,  1549,   347,  1551,   428,   345,
     347,  2993,  2822,   377,   355,   379,  1560,   351,   355,  1968,
     407,   410,   356,   359,   360,   361,   413,   414,   415,   416,
     417,   418,   408,  1982,  1983,   367,   367,  1581,   370,   371,
     371,   428,    56,    57,   407,   377,   377,   379,   379,   232,
     413,   414,   415,   416,   417,   418,   427,   367,   241,   346,
     347,   371,   349,    89,  3096,   428,  1363,   377,   355,   379,
    1614,  1615,  1616,  1617,   347,   349,  1620,  2358,   347,    88,
     349,   355,   355,  1380,  1381,   347,   355,   349,   357,  2617,
    2169,  2353,  2354,   355,  2356,   143,   358,  2359,   366,  3072,
     368,   407,   408,   371,   152,   403,  3079,   413,   414,   415,
     416,   417,   418,  2062,  2194,  3075,  3076,  2015,  1041,  1042,
    1043,  1044,  1045,  1046,  1047,  1048,  1049,  1050,  1051,   346,
     347,  2211,   349,  1677,  1431,   205,   346,   347,   355,   349,
    1684,  1685,   346,   347,   366,   355,   214,  1691,  1692,   371,
     347,   355,  1696,   214,  1698,  2214,  2215,  1701,   355,  1703,
    1704,  1705,  2221,  1707,  1708,  1709,  1710,   385,   414,  1713,
    1714,   348,  1716,   419,   420,  1719,   353,  1721,   407,  1723,
     267,   410,  1726,    25,   413,   414,   415,   416,   417,   418,
    2382,    35,   367,  2273,  2274,    39,    38,    39,  2390,    41,
      25,   408,   371,   348,  1748,   412,   346,   347,   353,   349,
      52,    53,   371,    38,    39,   355,    41,   326,   327,   328,
     329,   330,   331,  3055,  3056,   364,  1770,    52,    53,  1526,
     366,   367,   348,  1777,   160,   371,   348,   353,   348,  1783,
     407,   353,   409,   353,  1788,  1789,   348,  1791,   408,   348,
     407,   353,   412,   410,   353,  1799,   413,   414,   415,   416,
     417,   418,   348,   348,   348,   160,   348,   353,   353,   353,
      25,   353,   348,   348,   353,  1819,   348,   353,   353,   358,
    2229,   353,   348,    38,    39,   348,    41,   353,  3261,   348,
     353,  3264,  3265,  1837,   353,  3268,  1840,    52,    53,   165,
     353,   348,   348,   348,   407,   358,   353,   353,   353,   243,
     413,   414,   415,   416,   417,   418,   348,   348,  1862,    58,
      59,   353,   353,   348,  1868,   428,   348,   430,   353,   243,
     348,   353,   348,   407,   408,   353,  1880,   353,   382,   413,
     414,   415,   416,   417,   418,  1889,   366,   348,   348,   348,
     348,   371,   353,   353,   353,   353,   348,  1901,  1902,   348,
     203,   353,   205,  1907,   353,     7,     8,     9,   348,   243,
     348,    13,    14,   353,    16,   353,   367,    19,    20,   370,
     371,    23,    24,   271,  1928,    27,  1930,   348,    30,    31,
      32,    33,   353,   408,  2922,   243,   348,   412,    40,  1943,
    1944,   353,  1946,   348,   348,   352,   243,   354,   353,   353,
    1954,  3260,  1956,   366,   367,   366,  2678,  1961,  2680,  2681,
     371,    63,    64,    65,    66,    67,    68,    69,    70,    71,
      72,    73,    74,    75,    76,    77,    78,    79,    80,   355,
     348,  1985,   358,   348,  1988,   353,   538,  2527,   353,  1993,
    1994,   203,  1996,   205,   348,  2353,  2354,   348,  2356,   353,
     243,  2359,   353,  3436,   366,  2009,   348,  2546,   348,   371,
     408,   353,   348,   353,   412,  3435,     4,   353,   348,     7,
       8,     9,   367,   353,   348,    13,    14,    15,    16,   353,
     371,    19,    20,    21,  2574,    23,    24,   368,    26,    27,
     371,    29,    30,    31,    32,    33,   366,   367,   348,   348,
     348,  2607,    40,   353,   353,   353,   348,  2061,   408,   348,
     273,   353,   412,  2619,   353,  2605,   348,  2071,   407,   348,
     409,   353,   358,  2077,   353,    63,    64,    65,    66,    67,
      68,    69,    70,    71,    72,    73,    74,    75,    76,    77,
      78,    79,    80,   348,   358,   348,   348,   348,   353,  2103,
     353,   353,   353,  1860,  1861,   407,   408,   203,   358,   205,
    3543,   413,   414,   415,   416,   417,   418,   157,   158,   351,
     348,   348,   407,   408,   356,   353,   353,   165,   413,   414,
     415,   416,   417,   418,   408,   408,   367,   408,   412,   412,
     371,   412,   201,  3131,   358,    25,  2150,   408,  2152,  3137,
     408,   412,   355,   107,   412,   358,   398,   206,    38,    39,
     209,    41,   398,   206,  2168,  2169,   209,  3476,   407,  2173,
     409,  2175,    52,    53,  2714,  2715,  1236,   200,   201,  1239,
    2589,   410,   358,   412,  2593,   793,   794,   795,   796,   203,
     201,   205,   407,   408,   358,   203,  2200,   205,   413,   414,
     415,   416,   417,   418,  2208,  2614,   203,  2211,   205,  2618,
     358,  1968,  2621,   352,  2936,   354,   370,   371,  2222,    82,
    2760,   296,   297,   175,   176,  1982,  1983,   274,   275,   274,
     275,   789,   790,    25,   427,  2239,  3669,  3670,  2778,   428,
     791,   792,   797,   798,  2380,  2381,    38,    39,   428,    41,
    3691,  3692,  3405,  3406,  2258,   426,  2260,  2774,  2775,  2263,
      52,    53,  2266,   407,   218,   219,   407,  2807,    41,  2809,
     407,    25,  3800,  3801,   407,  2279,  2830,  2831,  3711,   338,
     339,   235,   236,  2287,    38,    39,   413,    41,   242,   243,
    3527,  3528,  2296,   352,   248,   354,  3233,  3234,    52,    53,
     923,   924,   423,   257,   424,  2062,   365,   366,   262,   368,
     369,    55,   414,   426,   925,   926,  3576,  3577,   408,   412,
    2678,   410,  2680,  2681,   278,   408,   412,   338,   339,   112,
    2334,   358,  2336,   262,   360,  2339,   347,   371,   371,  2343,
     371,   352,   285,   354,   298,   317,   253,  3672,   292,   242,
     317,   903,   286,   262,   365,   366,   313,   368,   369,   262,
     136,   347,   355,   317,   367,   367,   347,  2371,   363,   346,
    2374,   101,   343,   167,   104,   105,   242,   929,  2382,   262,
    2384,   371,  3707,  3708,  2388,   136,  2390,   261,   371,   180,
    2394,   258,   371,   123,  2398,  2399,  2400,   167,   217,   107,
     107,   107,   107,   107,   107,   107,   180,   258,   180,  2413,
    2414,   367,   347,   143,  3739,   106,   146,   207,   346,   360,
     360,  2961,   152,  2427,   347,   247,   353,   167,   353,   355,
    2839,  2435,   371,   165,   367,   371,   358,   271,   205,   427,
     256,   266,   430,   104,   110,   133,   347,  2987,   136,  3774,
    2454,   426,   426,    34,   142,   428,  2865,   408,   371,  3784,
     371,   347,   136,   371,   194,  2874,   371,   371,   127,   371,
     127,   371,  2229,   136,  3799,  3800,  3801,  3802,  3803,   127,
     371,   169,  3807,   371,  2488,   136,   116,  3027,  1040,  1041,
    1042,  1043,  1044,  1045,  1046,  1047,  1048,  1049,  1050,  1051,
    3040,   371,   286,   191,   347,   165,  2510,   285,   277,   239,
    3050,   371,   234,   400,   263,  2519,  1068,   355,   367,   207,
     395,   353,  2526,   118,   180,   118,   315,   407,   165,  2533,
     218,   219,   220,   413,   414,   415,   416,   417,   418,   277,
     371,  2545,  2546,   165,   358,  2549,   358,   235,   357,   366,
    3090,   366,   242,   283,   242,   243,   244,   243,   242,   180,
     248,   291,   292,   251,   262,   253,   218,   243,  2572,   257,
     243,  2575,   243,   266,   262,   360,  3116,   136,  2936,   233,
     310,  2585,   187,  2587,   212,   110,   347,   212,   318,   247,
     278,  3131,   367,   353,  2598,   292,   266,   221,   286,   116,
    2604,   366,  2606,   371,   205,  2609,  2610,  2611,  2612,  2613,
     298,  2615,   258,  2617,   371,   407,  2620,   228,  2622,   250,
     368,   413,   414,   415,   416,   417,   418,   407,   410,   317,
     104,   105,   358,   348,   348,  2639,   347,   408,   247,   247,
    3049,   371,   408,   368,   408,  2649,   122,   368,   255,   266,
     277,   371,   371,   407,   371,   212,  3196,   286,   132,   413,
     414,   415,   416,   417,   418,   139,   360,   232,   360,   232,
     213,   212,   146,  3082,  3083,   212,   212,   366,   317,   317,
     116,   283,   212,   371,   158,   247,   371,   113,  2692,   371,
     371,   371,   371,   371,   371,  3235,  2700,   171,   113,   371,
     181,   181,   371,   371,   178,  3245,   355,   360,  2712,   277,
     262,   165,   367,   360,   243,  2719,  3125,   116,  2722,   116,
     271,   371,   165,   366,   134,   246,   371,   371,   368,   371,
    3139,   153,   110,   353,  2738,   367,   211,  3146,   348,  3279,
     167,   130,   153,   347,   350,   371,   153,   221,   389,   347,
     224,   347,  2509,   371,   367,   371,   353,   148,  2762,   262,
     234,   360,   262,  1315,   367,   262,   353,   353,   247,  2773,
     201,   155,   106,  1325,   167,   355,   277,  3317,   180,   348,
    2784,   347,   256,   347,   237,   114,   116,   175,   212,   371,
     367,   347,   353,   180,   268,   207,   202,   368,   410,   247,
    1352,   368,  2806,   394,   394,   110,   258,   247,   408,   243,
     196,   371,   286,   366,   360,  3354,   366,   366,   366,  2823,
    2824,   286,   184,  2827,   347,   253,   196,   347,   368,  2833,
    2834,  2835,  2589,   295,   366,   309,  2593,   371,   366,   312,
    2844,  2845,   366,   243,   371,   371,   269,  2851,  1400,   368,
     368,   366,  1404,  2857,   368,   368,   368,  2614,   366,   366,
     258,  2618,   247,   190,  2621,   371,  2870,   371,   110,  2873,
     110,  3280,   232,  3413,   232,   371,   240,   110,   153,   110,
     348,  3421,   348,   266,   355,   305,  2890,   347,   355,   279,
     264,   180,   353,   180,   259,   367,   370,   371,   367,  2903,
     358,   358,   247,   347,   155,   106,   167,   167,   371,   221,
     146,   288,   371,   348,  3323,  3324,   366,   221,  2922,   366,
    2924,  2925,   301,   209,   115,   346,   181,  2931,   225,    36,
     368,   368,   113,   347,  3474,   368,  2940,  2941,   368,   184,
     253,   366,   317,   184,   347,   317,   286,   366,   353,   247,
     258,  2955,   347,   247,   247,   205,   266,   353,   366,   353,
     175,   191,   366,   355,   310,   347,   110,   196,   196,   280,
     371,   165,  3381,   353,   258,  2979,   347,     4,  2982,   355,
       7,     8,     9,   172,   353,   366,    13,    14,    15,    16,
    2994,   347,    19,    20,    21,  3535,    23,    24,   371,    26,
      27,   367,    29,    30,    31,    32,    33,  3547,   367,   348,
     371,   366,   366,    40,   371,  3555,  3556,  3557,  3558,   371,
     371,   371,   371,  3432,   371,   347,   212,   347,   347,   350,
     190,  3571,   355,   371,   130,   367,    63,    64,    65,    66,
      67,    68,    69,    70,    71,    72,    73,    74,    75,    76,
      77,    78,    79,    80,   259,   347,   259,   259,   367,   258,
     355,   167,  1614,   353,   360,   348,   348,  3071,  3072,   291,
     355,   371,   175,   247,   288,  3079,   347,   353,   366,   358,
     165,   353,  2839,   192,   428,   348,  3495,   353,   237,   348,
     347,   347,   271,   110,   347,   347,   291,   371,   366,   368,
     285,   355,  3642,   237,   366,   237,   371,   110,  2865,   232,
     348,   258,   347,   371,   153,  3119,   266,  2874,  3122,   366,
    3124,   266,  3662,  3127,  3128,   371,   180,  3131,   371,  3133,
     116,   208,   371,  3137,   371,   347,  3140,  3141,  3678,  3143,
     367,   367,  3682,  3147,   355,   118,   171,   347,   371,  1701,
     347,   347,   247,   255,   347,  1707,  3160,   347,   288,   347,
     243,  3165,   191,   355,   346,   181,   353,   236,   202,   149,
     212,   347,   258,   371,   138,   358,   360,   357,  3182,  3183,
     358,   347,   359,   358,   347,   358,   358,   358,   358,   358,
     358,   358,  3732,   358,   358,  3199,  3200,   358,   353,   201,
     348,   138,   180,   258,   212,   348,   116,   353,   367,   371,
     367,   138,  3752,     7,     8,     9,   368,   371,   110,    13,
      14,   272,    16,  3763,   366,    19,    20,   110,  3232,    23,
      24,   366,   371,    27,   228,   366,    30,    31,    32,    33,
     348,   116,   371,   208,   395,    39,    40,   347,   353,   348,
     210,   183,  3792,   116,   266,   172,   371,  3261,  3262,   360,
    3264,  3265,   288,   347,  3268,  3269,  3270,   392,   237,    63,
      64,    65,    66,    67,    68,    69,    70,    71,    72,    73,
      74,    75,    76,    77,    78,    79,    80,   263,  3292,   371,
     150,   263,  3049,   115,   358,   263,   246,   133,   358,  3303,
     136,   358,   247,   193,   360,  3309,   142,   355,  3312,   358,
    1862,   358,   263,   153,   360,   358,   347,  3321,  3322,   110,
     358,   317,   166,   360,   286,  3082,  3083,   348,  1880,   272,
     348,   212,   368,   206,   347,  3339,   371,   110,  3342,   371,
     208,  3345,   353,   353,   348,   116,  3350,  3351,   347,   347,
    3354,   348,   353,   348,   116,   191,  3360,   348,   173,   193,
     240,   353,  3366,   347,  3368,   371,   371,   212,  3125,   181,
     153,   207,   153,   366,   348,   346,  1928,   366,   314,   346,
     193,   371,  3139,   219,   220,   286,   361,   263,  3392,  3146,
     110,   317,  1944,   212,  1946,   358,   224,   317,   180,   235,
     348,   180,   348,   430,   184,   366,   242,   243,   244,   367,
     366,   366,   248,   212,   347,   125,   116,   253,   353,   368,
     263,   257,   301,   347,   371,   243,   262,   371,   358,   348,
     348,   153,  3436,   250,   119,   358,  1988,  3441,   180,   344,
     153,   207,   278,   358,   358,   358,   358,   116,   347,  3453,
     286,   358,   361,   347,   358,   366,   347,  2009,   347,   247,
     355,   253,   298,   371,   201,   184,   348,   106,  3472,   371,
     348,  3475,   205,  3477,   348,   347,   347,   347,   347,   266,
     153,   317,   119,   344,  3488,   174,  3490,   353,   246,   176,
     348,   358,   247,   358,   371,   165,   355,   348,   205,   399,
     316,   347,   358,  3260,   358,   358,   355,  3511,  3512,  2061,
     348,   348,   347,   212,   371,   348,   317,   358,   245,  2071,
     237,   353,   353,  3280,   348,  2077,   176,   347,   212,   259,
     259,   358,   371,   347,   110,   371,   245,   348,  3542,  3543,
     347,   139,   348,   348,  3548,   347,   343,  3551,   358,    54,
     799,   335,   800,   116,   133,   111,  1135,   801,   420,   685,
    3564,   361,    97,    97,   656,  3569,  3323,  3324,   802,   707,
     396,   803,  1082,     7,     8,     9,   854,  3581,   611,    13,
      14,  2464,    16,   804,  1016,    19,    20,   854,  1902,    23,
      24,  2536,   167,    27,  1011,   820,    30,    31,    32,    33,
    2815,  2766,  2764,  1741,  2413,  1752,    40,  3290,  3563,  2452,
    1711,  3615,  3368,   407,   408,   409,  2168,  2951,  3622,  2699,
     414,  2173,  3182,  2175,  3381,  2940,  3458,  3296,  3362,    63,
      64,    65,    66,    67,    68,    69,    70,    71,    72,    73,
      74,    75,    76,    77,    78,    79,    80,   854,  1585,    83,
      84,    85,  3656,  1317,  1269,   875,  2208,    91,    92,    93,
      94,    95,    96,  3196,  3522,  3669,  3670,  3497,  2061,  2164,
    2222,  3675,  2173,  3209,  2741,  3432,  2444,  3211,  1946,  3683,
    3684,   154,  1828,  1485,  3040,  3689,  3048,  2239,  3703,  2844,
    2572,  3418,  2266,  3421,  1940,  1560,   193,  1840,  1947,  3703,
    1067,   721,  1842,  1399,  3549,  3739,  3622,  3711,  3773,  3799,
    3269,  2263,  3678,  3556,  2266,  3682,  3089,  3721,  3682,  3476,
    3558,  2841,  3087,  1977,  1023,   641,  1832,  3731,  3235,  3238,
    3535,  1022,  1346,  3529,  1843,  2664,  2924,  3741,  3495,   895,
    1985,  3149,  2609,  3345,  2296,  2878,  3118,  3117,  1996,  3641,
    1305,  3755,  1346,  2189,  1293,  2158,  2157,  2128,  3225,  2191,
    2421,  2181,  2350,  2926,   922,  2761,  3228,  3009,  1595,  3773,
    2495,    -1,  1100,  3499,    -1,    -1,    -1,    -1,  3782,    -1,
      -1,    -1,    -1,    -1,  2336,    -1,    -1,    -1,    -1,    -1,
      -1,  2343,     4,     5,     6,     7,     8,     9,    10,    11,
      12,    13,    14,    15,    16,    17,    18,    19,    20,    21,
      22,    23,    24,    25,    26,    27,    28,    29,    30,    31,
      32,    33,    34,    -1,    -1,    37,    38,    39,    40,    41,
    2382,    -1,    -1,    -1,    -1,    -1,  2388,    -1,  2390,    -1,
      52,    53,    -1,    -1,    -1,    -1,  2398,    -1,    -1,    -1,
      -1,    63,    64,    65,    66,    67,    68,    69,    70,    71,
      72,    73,    74,    75,    76,    77,    78,    79,    80,    -1,
    3627,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    99,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,  2454,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,     4,     5,     6,     7,
       8,     9,    10,    11,    12,    13,    14,    15,    16,    17,
      18,    19,    20,    21,    22,    23,    24,    25,    26,    27,
      28,    29,    30,    31,    32,    33,    34,    -1,    -1,    37,
      38,    39,    40,    41,    -1,    -1,    -1,    -1,  2510,    -1,
      -1,    -1,    -1,    -1,    52,    53,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,  2526,    63,    64,    65,    66,    67,
      68,    69,    70,    71,    72,    73,    74,    75,    76,    77,
      78,    79,    80,    -1,    -1,   429,    -1,  2549,    -1,    -1,
      -1,     7,     8,     9,    -1,    -1,    -1,    13,    14,    -1,
      16,    99,    -1,    19,    20,    -1,    -1,    23,    24,    -1,
    2572,    27,    -1,  2575,    30,    31,    32,    33,    -1,    -1,
      -1,    -1,    -1,  2585,    40,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    63,    64,    65,
      66,    67,    68,    69,    70,    71,    72,    73,    74,    75,
      76,    77,    78,    79,    80,    -1,    -1,    83,    84,    85,
      -1,    -1,    -1,    -1,    -1,    91,    92,    93,    94,    95,
      96,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,     7,     8,     9,    -1,    -1,
      -1,    13,    14,    -1,    16,    -1,    -1,    19,    20,    -1,
      -1,    23,    24,    -1,    -1,    27,    -1,    -1,    30,    31,
      32,    33,    -1,    -1,    -1,    -1,    -1,    -1,    40,    -1,
    2692,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
    2712,    63,    64,    65,    66,    67,    68,    69,    70,    71,
      72,    73,    74,    75,    76,    77,    78,    79,    80,    -1,
      -1,    83,    84,    85,    -1,    -1,    -1,    -1,    -1,    91,
      92,    93,    94,    95,    96,   407,    -1,    -1,    -1,    -1,
      -1,   413,   414,   415,   416,   417,   418,    -1,    -1,    -1,
      -1,    -1,    -1,     7,     8,     9,   428,   429,   430,    13,
      14,    -1,    16,    -1,    -1,    19,    20,    -1,    -1,    23,
      24,    -1,  2784,    27,    -1,    -1,    30,    31,    32,    33,
      -1,    -1,    -1,    -1,    -1,    39,    40,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,  2806,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    63,
      64,    65,    66,    67,    68,    69,    70,    71,    72,    73,
      74,    75,    76,    77,    78,    79,    80,    -1,    -1,     7,
       8,     9,  2844,    -1,    -1,    13,    14,    -1,    16,    -1,
      -1,    19,    20,    -1,    -1,    23,    24,    25,    -1,    27,
      -1,    -1,    30,    31,    32,    33,    -1,    -1,  2870,   407,
      38,    39,    40,    41,    -1,   413,   414,   415,   416,   417,
     418,    -1,    -1,    -1,    52,    53,    -1,    -1,    -1,    -1,
     428,   429,   430,    -1,    -1,    63,    64,    65,    66,    67,
      68,    69,    70,    71,    72,    73,    74,    75,    76,    77,
      78,    79,    80,    -1,    -1,    -1,    -1,    -1,     0,    -1,
    2922,     3,     4,  2925,    -1,     7,     8,     9,    -1,    -1,
      -1,    13,    14,    15,    16,    -1,    -1,    19,    20,    21,
      -1,    23,    24,    -1,    26,    27,    -1,    29,    30,    31,
      32,    33,    -1,  2955,    -1,    -1,    -1,    39,    40,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   429,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    63,    64,    65,    66,    67,    68,    69,    70,    71,
      72,    73,    74,    75,    76,    77,    78,    79,    80,    -1,
      -1,    83,    84,    85,    -1,    -1,    -1,    -1,    -1,    91,
      92,    93,    94,    95,    96,    -1,    -1,    99,     3,     4,
      -1,    -1,     7,     8,     9,    -1,    -1,    -1,    13,    14,
      15,    16,    -1,    -1,    19,    20,    21,    -1,    23,    24,
      -1,    26,    27,    -1,    29,    30,    31,    32,    33,    -1,
      -1,    -1,    -1,    -1,    39,    40,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   429,    63,    64,
      65,    66,    67,    68,    69,    70,    71,    72,    73,    74,
      75,    76,    77,    78,    79,    80,    -1,    -1,    83,    84,
      85,    -1,    -1,    -1,    -1,    -1,    91,    92,    93,    94,
      95,    96,    -1,    -1,    99,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,     7,     8,     9,
      -1,    -1,    -1,    13,    14,    -1,    16,    -1,  3140,    19,
      20,    -1,    -1,    23,    24,    -1,    -1,    27,    -1,    -1,
      30,    31,    32,    33,    -1,    -1,    -1,    -1,    -1,    39,
      40,    -1,    -1,   407,    -1,    -1,    -1,    -1,    -1,    -1,
     414,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   426,    63,    64,    65,    66,    67,    68,    69,
      70,    71,    72,    73,    74,    75,    76,    77,    78,    79,
      80,    -1,    -1,    -1,    -1,     7,     8,     9,    -1,    -1,
      -1,    13,    14,    -1,    16,    -1,    -1,    19,    20,    -1,
      -1,    23,    24,    -1,    -1,    27,    -1,    -1,    30,    31,
      32,    33,    -1,    -1,    -1,    -1,    -1,    -1,    40,   407,
      -1,    -1,    -1,    -1,    -1,   413,   414,   415,   416,   417,
     418,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
    3262,    63,    64,    65,    66,    67,    68,    69,    70,    71,
      72,    73,    74,    75,    76,    77,    78,    79,    80,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,     7,     8,
       9,    -1,    -1,    -1,    13,    14,    -1,    16,    -1,    -1,
      19,    20,    -1,    -1,    23,    24,    -1,    -1,    27,    -1,
      -1,    30,    31,    32,    33,    -1,    -1,    -1,    -1,    -1,
      -1,    40,    -1,    -1,    -1,   407,    -1,    -1,    -1,    -1,
      -1,    -1,   414,    -1,    -1,    -1,    -1,  3339,    -1,    -1,
      -1,    -1,    -1,  3345,    63,    64,    65,    66,    67,    68,
      69,    70,    71,    72,    73,    74,    75,    76,    77,    78,
      79,    80,    -1,    -1,    -1,    -1,  3368,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   136,    -1,     7,     8,     9,    -1,   142,    -1,
      13,    14,   136,    16,    -1,    -1,    19,    20,    -1,    -1,
      23,    24,    -1,    -1,    27,    -1,    -1,    30,    31,    32,
      33,    -1,    -1,    -1,    -1,   169,    -1,    40,    -1,    -1,
      -1,    -1,   407,    -1,    -1,   169,    -1,    -1,    -1,   414,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,  3441,
      63,    64,    65,    66,    67,    68,    69,    70,    71,    72,
      73,    74,    75,    76,    77,    78,    79,    80,    -1,    -1,
      -1,    -1,    -1,    -1,   218,   219,   220,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   218,    -1,   220,    -1,    -1,    -1,
      -1,   235,    -1,    -1,    -1,    -1,    -1,    -1,   242,   243,
      -1,   235,   236,    -1,   248,    -1,    -1,   251,    -1,   243,
      -1,    -1,    -1,   257,   248,    -1,    -1,    -1,   262,    -1,
      -1,    -1,    -1,   257,    -1,    -1,    -1,    -1,   262,    -1,
      -1,    -1,    -1,    -1,   278,    -1,    -1,   407,    -1,   409,
      -1,    -1,   286,    -1,   414,    -1,    -1,    -1,    -1,    -1,
    3542,    -1,   286,    -1,   298,    -1,  3548,    -1,    -1,  3551,
      -1,    -1,    -1,    -1,   298,    -1,    -1,    -1,    -1,   313,
      -1,    -1,    -1,   317,    -1,    -1,    -1,    -1,    -1,   313,
      -1,    -1,    -1,   317,    -1,    -1,    -1,    -1,    -1,   103,
      -1,    -1,    -1,    -1,   108,   109,    -1,    -1,   112,   113,
      -1,    -1,    -1,    -1,    -1,   119,   120,    -1,   122,   123,
      -1,   125,    -1,   127,    -1,   407,   408,   409,    -1,   133,
      -1,    -1,   414,  3615,    -1,    -1,   140,   371,   142,   143,
    3622,    -1,    -1,    -1,    -1,    -1,    -1,   371,   152,    -1,
     154,    -1,    -1,    -1,    -1,    -1,    -1,   161,    -1,   163,
     164,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   185,   186,   187,   188,    -1,   190,    -1,   192,    -1,
     194,   195,   196,  3675,   198,    -1,    -1,   201,    -1,    -1,
      -1,    -1,   206,    -1,    -1,    -1,    -1,    -1,   407,   213,
     409,    -1,   216,    -1,    -1,   414,   220,   221,   222,   223,
      -1,  3703,    -1,    -1,    -1,    -1,   230,    -1,    -1,    -1,
      -1,   235,    -1,    -1,    -1,   239,    -1,    -1,   242,    -1,
      -1,   245,    -1,    -1,    -1,   249,   250,   251,   252,    -1,
      -1,   255,    -1,    -1,   258,    -1,    -1,    -1,    -1,  3741,
      -1,    -1,    -1,    -1,    -1,   269,   270,   271,   272,    -1,
     136,    -1,    -1,  3755,    -1,    -1,    -1,    -1,    -1,   283,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   292,    -1,
     294,  3773,    -1,    -1,    -1,    -1,   300,   301,    -1,    -1,
    3782,    -1,    -1,   169,    -1,   408,    -1,    -1,    -1,    -1,
      -1,   315,   316,    -1,    -1,   319,   320,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   335,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   347,    -1,    -1,    -1,    -1,   352,    -1,
     354,    -1,   218,    -1,   220,    -1,    -1,    -1,   362,    -1,
      -1,   365,   366,   367,   368,   369,   370,   371,    -1,   235,
     236,    -1,    -1,   377,    -1,   379,   380,   243,    -1,    -1,
      -1,    -1,   248,   387,   103,    -1,    -1,   391,    -1,   108,
     109,   257,    -1,   112,   113,   399,   262,    -1,    -1,    -1,
     119,   120,   406,   122,   123,    -1,   125,    -1,   127,    -1,
      -1,    -1,    -1,    -1,   133,    -1,    -1,    -1,    -1,    -1,
     286,   140,    -1,   142,   143,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   298,   152,    -1,   154,    -1,    -1,    -1,    -1,
      -1,    -1,   161,    -1,   163,   164,    -1,   313,    -1,    -1,
      -1,   317,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   185,   186,   187,   188,
      -1,   190,    -1,   192,    -1,   194,   195,   196,    -1,   198,
      -1,    -1,   201,    -1,    -1,    -1,    -1,   206,    -1,    -1,
      -1,    -1,    -1,    -1,   213,    -1,    -1,   216,    -1,    -1,
      -1,   220,   221,   222,   223,   371,    -1,    -1,    -1,    -1,
      -1,   230,    -1,    -1,    -1,    -1,   235,    -1,    -1,    -1,
     239,    -1,    -1,   242,    -1,    -1,   245,    -1,    -1,    -1,
     249,   250,   251,   252,    -1,    -1,   255,    -1,    -1,   258,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     269,   270,   271,   272,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   283,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   292,    -1,   294,    -1,    -1,    -1,    -1,
      -1,   300,   301,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   315,   316,    -1,    -1,
     319,   320,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   335,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   347,    -1,
      -1,    -1,    -1,   352,    -1,   354,    -1,    -1,    -1,    -1,
      -1,    -1,   105,   362,    -1,    -1,   365,   366,   367,   368,
     369,   370,   371,    -1,    -1,    -1,    -1,    -1,   377,   122,
     379,   380,   125,   126,    -1,   128,    -1,    -1,   387,   132,
      -1,    -1,   391,    -1,   137,   138,   139,   140,    -1,    -1,
     399,   144,    -1,   146,   147,    -1,    -1,    -1,    -1,    -1,
      -1,   154,    -1,    -1,    -1,   158,    -1,    -1,    -1,   162,
      -1,    -1,   165,    -1,    -1,    -1,    -1,    -1,   171,    -1,
      -1,    -1,    -1,    -1,    -1,   178,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   194,   195,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   206,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   226,    -1,   228,   229,    -1,   231,    -1,
     233,   234,    -1,    -1,   237,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   254,    -1,   256,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   266,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   291,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   302,
     303,    -1,    -1,    -1,    -1,    -1,   309,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   318,    -1,    -1,    -1,    -1,
      -1,   324,   325,    -1,    -1,    -1,   103,    -1,    -1,    -1,
      -1,   108,   109,    -1,    -1,   112,   113,    -1,    -1,    -1,
      -1,    -1,    -1,   120,   347,   122,   123,    -1,   125,    -1,
     127,    -1,    -1,    -1,    -1,    -1,   133,    -1,    -1,    -1,
      -1,    -1,   139,   140,    -1,   142,   143,    -1,   371,    -1,
     373,    -1,   375,   376,    -1,   152,    -1,   154,   381,    -1,
      -1,    -1,    -1,   386,    -1,    -1,   163,   164,    -1,    -1,
     393,    -1,    -1,   396,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   185,   186,
      -1,   188,    -1,   190,    -1,   192,    -1,   194,   195,   196,
      -1,   198,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   206,
      -1,    -1,    -1,    -1,    -1,    -1,   213,    -1,    -1,   216,
      -1,    -1,    -1,   220,   221,   222,   223,    -1,    -1,    -1,
      -1,    -1,    -1,   230,    -1,    -1,    -1,    -1,   235,    -1,
      -1,    -1,   239,    -1,    -1,   242,    -1,    -1,   245,    -1,
      -1,    -1,    -1,   250,   251,    -1,    -1,    -1,   255,    -1,
      -1,   258,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   269,   270,   271,   272,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   282,   283,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   294,    -1,    -1,
      -1,    -1,    -1,    -1,   301,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   103,    -1,   315,   316,
      -1,   108,   109,    -1,    -1,   112,   113,    -1,    -1,    -1,
      -1,    -1,    -1,   120,    -1,   122,   123,    -1,   125,    -1,
     127,    -1,    -1,    -1,    -1,    -1,   133,    -1,    -1,    -1,
      -1,    -1,   139,   140,   351,   142,   143,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   152,    -1,   154,    -1,    -1,
      -1,    -1,    -1,   370,   371,    -1,   163,   164,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   384,    -1,    -1,
     387,    -1,    -1,    -1,   391,   392,    -1,    -1,   185,   186,
      -1,   188,    -1,   190,    -1,   192,    -1,   194,   195,   196,
      -1,   198,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   206,
      -1,    -1,    -1,    -1,    -1,    -1,   213,    -1,    -1,   216,
      -1,    -1,    -1,   220,   221,   222,   223,    -1,    -1,    -1,
      -1,    -1,    -1,   230,    -1,    -1,    -1,    -1,   235,    -1,
      -1,    -1,   239,    -1,    -1,   242,    -1,    -1,   245,    -1,
      -1,    -1,    -1,   250,   251,    -1,    -1,    -1,   255,    -1,
      -1,   258,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   269,   270,   271,   272,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   282,   283,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   294,    -1,    -1,
      -1,    -1,    -1,    -1,   301,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   103,    -1,   315,   316,
      -1,   108,   109,    -1,    -1,   112,   113,    -1,    -1,    -1,
      -1,    -1,    -1,   120,    -1,   122,   123,    -1,   125,    -1,
     127,    -1,    -1,    -1,    -1,    -1,   133,    -1,    -1,    -1,
      -1,    -1,   139,   140,   351,   142,   143,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   152,    -1,   154,    -1,    -1,
      -1,    -1,    -1,   370,   371,    -1,   163,   164,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   384,    -1,    -1,
     387,    -1,    -1,    -1,   391,   392,    -1,    -1,   185,   186,
      -1,   188,    -1,   190,    -1,   192,    -1,   194,   195,   196,
      -1,   198,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   206,
      -1,    -1,    -1,    -1,    -1,    -1,   213,    -1,    -1,   216,
      -1,    -1,    -1,   220,   221,   222,   223,    -1,    -1,    -1,
      -1,    -1,    -1,   230,    -1,    -1,    -1,    -1,   235,    -1,
      -1,    -1,   239,    -1,    -1,   242,    -1,    -1,   245,    -1,
      -1,    -1,    -1,   250,   251,    -1,    -1,    -1,   255,    -1,
      -1,   258,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   269,   270,   271,   272,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   282,   283,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   294,    -1,    -1,
      -1,    -1,    -1,    -1,   301,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   315,   316,
     103,   104,    -1,    -1,    -1,   108,   109,    -1,    -1,   112,
     113,    -1,    -1,    -1,    -1,    -1,   119,   120,    -1,   122,
     123,    -1,   125,    -1,   127,    -1,    -1,    -1,    -1,    -1,
     133,    -1,    -1,    -1,    -1,    -1,    -1,   140,    -1,   142,
     143,    -1,   145,    -1,    -1,    -1,    -1,    -1,    -1,   152,
      -1,   154,    -1,   370,   371,    -1,    -1,    -1,   161,    -1,
     163,   164,    -1,    -1,    -1,    -1,    -1,   384,    -1,    -1,
     387,    -1,    -1,    -1,   391,   392,    -1,    -1,    -1,    -1,
      -1,    -1,   185,   186,   187,   188,    -1,   190,    -1,   192,
      -1,   194,   195,   196,    -1,   198,    -1,   200,   201,    -1,
      -1,    -1,    -1,   206,    -1,    -1,    -1,    -1,    -1,    -1,
     213,    -1,    -1,   216,    -1,    -1,    -1,   220,   221,   222,
     223,    -1,    -1,    -1,    -1,    -1,    -1,   230,    -1,    -1,
      -1,    -1,   235,    -1,    -1,    -1,   239,    -1,    -1,   242,
      -1,    -1,   245,    -1,    -1,    -1,   249,   250,   251,   252,
     253,    -1,   255,    -1,    -1,   258,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   269,   270,   271,   272,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   282,
     283,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   292,
      -1,   294,    -1,    -1,    -1,    -1,    -1,   300,   301,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   315,   316,    -1,    -1,   319,   320,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   335,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   347,   348,    -1,    -1,   351,   352,
      -1,   354,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   362,
      -1,    -1,   365,   366,   367,   368,   369,   370,   371,    -1,
      -1,    -1,    -1,    -1,   377,    -1,   379,   103,   104,    -1,
      -1,    -1,   108,   109,   387,    -1,   112,   113,   391,    -1,
      -1,    -1,    -1,   119,   120,    -1,   122,   123,    -1,   125,
      -1,   127,    -1,    -1,    -1,    -1,    -1,   133,    -1,    -1,
      -1,    -1,    -1,    -1,   140,    -1,   142,   143,    -1,   145,
      -1,    -1,    -1,    -1,    -1,    -1,   152,    -1,   154,    -1,
      -1,    -1,    -1,    -1,    -1,   161,    -1,   163,   164,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   185,
     186,   187,   188,    -1,   190,    -1,   192,    -1,   194,   195,
     196,    -1,   198,    -1,   200,   201,    -1,    -1,    -1,    -1,
     206,    -1,    -1,    -1,    -1,    -1,    -1,   213,    -1,    -1,
     216,    -1,    -1,    -1,   220,   221,   222,   223,    -1,    -1,
      -1,    -1,    -1,    -1,   230,    -1,    -1,    -1,    -1,   235,
      -1,    -1,    -1,   239,    -1,    -1,   242,    -1,    -1,   245,
      -1,    -1,    -1,   249,   250,   251,   252,   253,    -1,   255,
      -1,    -1,   258,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   269,   270,   271,   272,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   282,   283,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   292,    -1,   294,    -1,
      -1,    -1,    -1,    -1,   300,   301,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   315,
     316,    -1,    -1,   319,   320,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   335,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   347,   348,    -1,    -1,   351,   352,    -1,   354,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   362,    -1,    -1,   365,
     366,   367,   368,   369,   370,   371,    -1,    -1,    -1,    -1,
     103,   377,    -1,   379,    -1,   108,   109,    -1,    -1,   112,
     113,   387,    -1,    -1,    -1,   391,   119,   120,    -1,   122,
     123,    -1,   125,    -1,   127,    -1,    -1,    -1,    -1,    -1,
     133,    -1,    -1,    -1,    -1,    -1,    -1,   140,    -1,   142,
     143,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   152,
      -1,   154,    -1,    -1,    -1,    -1,    -1,    -1,   161,    -1,
     163,   164,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   185,   186,   187,   188,    -1,   190,    -1,   192,
      -1,   194,   195,   196,    -1,   198,    -1,   200,   201,    -1,
      -1,    -1,    -1,   206,    -1,    -1,    -1,    -1,    -1,    -1,
     213,    -1,    -1,   216,    -1,    -1,    -1,   220,   221,   222,
     223,    -1,    -1,    -1,    -1,    -1,    -1,   230,    -1,    -1,
      -1,    -1,   235,    -1,    -1,    -1,   239,    -1,    -1,   242,
      -1,    -1,   245,    -1,    -1,    -1,   249,   250,   251,   252,
     253,    -1,   255,    -1,    -1,   258,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   266,    -1,    -1,   269,   270,   271,   272,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   282,
     283,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   292,
      -1,   294,    -1,    -1,    -1,    -1,    -1,   300,   301,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   315,   316,    -1,    -1,   319,   320,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   335,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   347,   348,    -1,    -1,    -1,   352,
      -1,   354,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   362,
      -1,    -1,   365,   366,   367,   368,   369,   370,   371,    -1,
      -1,    -1,    -1,   103,   377,    -1,   379,    -1,   108,   109,
      -1,    -1,   112,   113,   387,    -1,    -1,    -1,   391,   119,
     120,    -1,   122,   123,    -1,   125,    -1,   127,    -1,    -1,
      -1,    -1,    -1,   133,    -1,    -1,    -1,    -1,    -1,    -1,
     140,    -1,   142,   143,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   152,    -1,   154,    -1,    -1,    -1,    -1,    -1,
      -1,   161,    -1,   163,   164,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   185,   186,   187,   188,    -1,
     190,    -1,   192,    -1,   194,   195,   196,    -1,   198,    -1,
     200,   201,    -1,    -1,    -1,    -1,   206,    -1,    -1,    -1,
      -1,    -1,    -1,   213,    -1,    -1,   216,    -1,    -1,    -1,
     220,   221,   222,   223,    -1,    -1,    -1,    -1,    -1,    -1,
     230,    -1,    -1,    -1,    -1,   235,    -1,    -1,    -1,   239,
      -1,    -1,   242,    -1,    -1,   245,    -1,    -1,    -1,   249,
     250,   251,   252,   253,    -1,   255,    -1,    -1,   258,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   266,    -1,    -1,   269,
     270,   271,   272,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   282,   283,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   292,    -1,   294,    -1,    -1,    -1,    -1,    -1,
     300,   301,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   315,   316,    -1,    -1,   319,
     320,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   335,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   347,    -1,    -1,
      -1,    -1,   352,    -1,   354,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   362,    -1,    -1,   365,   366,   367,   368,   369,
     370,   371,    -1,    -1,    -1,    -1,   103,   377,    -1,   379,
      -1,   108,   109,    -1,    -1,   112,   113,   387,    -1,    -1,
      -1,   391,   119,   120,    -1,   122,   123,    -1,   125,    -1,
     127,    -1,    -1,    -1,    -1,    -1,   133,    -1,    -1,    -1,
      -1,    -1,    -1,   140,    -1,   142,   143,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   152,    -1,   154,    -1,    -1,
      -1,    -1,    -1,    -1,   161,    -1,   163,   164,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   185,   186,
     187,   188,    -1,   190,    -1,   192,    -1,   194,   195,   196,
      -1,   198,    -1,   200,   201,    -1,    -1,    -1,    -1,   206,
      -1,    -1,    -1,    -1,    -1,    -1,   213,    -1,    -1,   216,
      -1,    -1,    -1,   220,   221,   222,   223,    -1,    -1,    -1,
      -1,    -1,    -1,   230,    -1,    -1,    -1,    -1,   235,    -1,
      -1,    -1,   239,    -1,    -1,   242,    -1,    -1,   245,    -1,
      -1,    -1,   249,   250,   251,   252,   253,    -1,   255,    -1,
      -1,   258,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   269,   270,   271,   272,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   282,   283,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   292,    -1,   294,    -1,    -1,
      -1,    -1,    -1,   300,   301,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   315,   316,
      -1,    -1,   319,   320,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   335,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     347,   348,    -1,    -1,    -1,   352,    -1,   354,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   362,    -1,    -1,   365,   366,
     367,   368,   369,   370,   371,    -1,    -1,    -1,    -1,   103,
     377,    -1,   379,    -1,   108,   109,    -1,    -1,   112,   113,
     387,    -1,    -1,    -1,   391,   119,   120,    -1,   122,   123,
      -1,   125,    -1,   127,    -1,    -1,    -1,    -1,    -1,   133,
      -1,    -1,    -1,    -1,    -1,    -1,   140,    -1,   142,   143,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   152,    -1,
     154,    -1,    -1,    -1,    -1,    -1,    -1,   161,    -1,   163,
     164,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   185,   186,   187,   188,    -1,   190,    -1,   192,    -1,
     194,   195,   196,    -1,   198,    -1,   200,   201,    -1,    -1,
      -1,    -1,   206,    -1,    -1,    -1,    -1,    -1,    -1,   213,
      -1,    -1,   216,    -1,    -1,    -1,   220,   221,   222,   223,
      -1,    -1,    -1,    -1,    -1,    -1,   230,    -1,    -1,    -1,
      -1,   235,    -1,    -1,    -1,   239,    -1,    -1,   242,    -1,
      -1,   245,    -1,    -1,    -1,   249,   250,   251,   252,   253,
      -1,   255,    -1,    -1,   258,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   269,   270,   271,   272,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   282,   283,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   292,    -1,
     294,    -1,    -1,    -1,    -1,    -1,   300,   301,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   315,   316,    -1,    -1,   319,   320,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   335,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   347,    -1,    -1,    -1,   351,   352,    -1,
     354,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   362,    -1,
      -1,   365,   366,   367,   368,   369,   370,   371,    -1,    -1,
      -1,    -1,   103,   377,    -1,   379,    -1,   108,   109,    -1,
      -1,   112,   113,   387,    -1,    -1,    -1,   391,   119,   120,
      -1,   122,   123,    -1,   125,    -1,   127,    -1,    -1,    -1,
      -1,    -1,   133,    -1,    -1,    -1,    -1,    -1,    -1,   140,
      -1,   142,   143,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   152,    -1,   154,    -1,    -1,    -1,    -1,    -1,    -1,
     161,    -1,   163,   164,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   185,   186,   187,   188,    -1,   190,
      -1,   192,    -1,   194,   195,   196,    -1,   198,    -1,   200,
     201,    -1,    -1,    -1,    -1,   206,    -1,    -1,    -1,    -1,
      -1,    -1,   213,    -1,    -1,   216,    -1,    -1,    -1,   220,
     221,   222,   223,    -1,    -1,    -1,    -1,    -1,    -1,   230,
      -1,    -1,    -1,    -1,   235,    -1,    -1,    -1,   239,    -1,
      -1,   242,    -1,    -1,   245,    -1,    -1,    -1,   249,   250,
     251,   252,   253,    -1,   255,    -1,    -1,   258,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   269,   270,
     271,   272,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   282,   283,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   292,    -1,   294,    -1,    -1,    -1,    -1,    -1,   300,
     301,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   315,   316,    -1,    -1,   319,   320,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   335,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   347,   348,    -1,    -1,
      -1,   352,    -1,   354,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   362,    -1,    -1,   365,   366,   367,   368,   369,   370,
     371,    -1,    -1,    -1,    -1,   103,   377,    -1,   379,    -1,
     108,   109,    -1,    -1,   112,   113,   387,    -1,    -1,    -1,
     391,   119,   120,    -1,   122,   123,    -1,   125,    -1,   127,
      -1,    -1,    -1,    -1,    -1,   133,    -1,    -1,    -1,    -1,
      -1,    -1,   140,    -1,   142,   143,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   152,    -1,   154,    -1,    -1,    -1,
      -1,    -1,    -1,   161,    -1,   163,   164,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   185,   186,   187,
     188,    -1,   190,    -1,   192,    -1,   194,   195,   196,    -1,
     198,    -1,   200,   201,    -1,    -1,    -1,    -1,   206,    -1,
      -1,    -1,    -1,    -1,    -1,   213,    -1,    -1,   216,    -1,
      -1,    -1,   220,   221,   222,   223,    -1,    -1,    -1,    -1,
      -1,    -1,   230,    -1,    -1,    -1,    -1,   235,    -1,    -1,
      -1,   239,    -1,    -1,   242,    -1,    -1,   245,    -1,    -1,
      -1,   249,   250,   251,   252,   253,    -1,   255,    -1,    -1,
     258,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   269,   270,   271,   272,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   282,   283,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   292,    -1,   294,    -1,    -1,    -1,
      -1,    -1,   300,   301,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   315,   316,    -1,
      -1,   319,   320,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   335,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   347,
     348,    -1,    -1,    -1,   352,    -1,   354,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   362,    -1,    -1,   365,   366,   367,
     368,   369,   370,   371,    -1,    -1,    -1,    -1,   103,   377,
      -1,   379,    -1,   108,   109,    -1,    -1,   112,   113,   387,
      -1,    -1,    -1,   391,   119,   120,    -1,   122,   123,    -1,
     125,    -1,   127,    -1,    -1,    -1,    -1,    -1,   133,    -1,
      -1,    -1,    -1,    -1,    -1,   140,    -1,   142,   143,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   152,    -1,   154,
      -1,    -1,    -1,    -1,    -1,    -1,   161,    -1,   163,   164,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     185,   186,   187,   188,    -1,   190,    -1,   192,    -1,   194,
     195,   196,    -1,   198,    -1,   200,   201,    -1,    -1,    -1,
      -1,   206,    -1,    -1,    -1,    -1,    -1,    -1,   213,    -1,
      -1,   216,    -1,    -1,    -1,   220,   221,   222,   223,    -1,
      -1,    -1,    -1,    -1,    -1,   230,    -1,    -1,    -1,    -1,
     235,    -1,    -1,    -1,   239,    -1,    -1,   242,    -1,    -1,
     245,    -1,    -1,    -1,   249,   250,   251,   252,   253,    -1,
     255,    -1,    -1,   258,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   269,   270,   271,   272,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   282,   283,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   292,    -1,   294,
      -1,    -1,    -1,    -1,    -1,   300,   301,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     315,   316,    -1,    -1,   319,   320,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     335,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   347,   348,    -1,    -1,    -1,   352,    -1,   354,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   362,    -1,    -1,
     365,   366,   367,   368,   369,   370,   371,    -1,    -1,    -1,
      -1,   103,   377,    -1,   379,    -1,   108,   109,    -1,    -1,
     112,   113,   387,    -1,    -1,    -1,   391,   119,   120,    -1,
     122,   123,    -1,   125,    -1,   127,    -1,    -1,    -1,    -1,
      -1,   133,    -1,    -1,    -1,    -1,    -1,    -1,   140,    -1,
     142,   143,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     152,    -1,   154,    -1,    -1,    -1,    -1,    -1,    -1,   161,
      -1,   163,   164,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   185,   186,   187,   188,    -1,   190,    -1,
     192,    -1,   194,   195,   196,    -1,   198,    -1,   200,   201,
      -1,    -1,    -1,    -1,   206,    -1,    -1,    -1,    -1,    -1,
      -1,   213,    -1,    -1,   216,    -1,    -1,    -1,   220,   221,
     222,   223,    -1,    -1,    -1,    -1,    -1,    -1,   230,    -1,
      -1,    -1,    -1,   235,    -1,    -1,    -1,   239,    -1,    -1,
     242,    -1,    -1,   245,    -1,    -1,    -1,   249,   250,   251,
     252,   253,    -1,   255,    -1,    -1,   258,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   269,   270,   271,
     272,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     282,   283,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     292,    -1,   294,    -1,    -1,    -1,    -1,    -1,   300,   301,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   315,   316,    -1,    -1,   319,   320,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   335,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   347,    -1,    -1,    -1,    -1,
     352,    -1,   354,    -1,    -1,    -1,   358,    -1,    -1,    -1,
     362,    -1,    -1,   365,   366,   367,   368,   369,   370,   371,
      -1,    -1,    -1,    -1,   103,   377,    -1,   379,    -1,   108,
     109,    -1,    -1,   112,   113,   387,    -1,    -1,    -1,   391,
     119,   120,    -1,   122,   123,    -1,   125,    -1,   127,    -1,
      -1,    -1,    -1,    -1,   133,    -1,    -1,    -1,    -1,    -1,
      -1,   140,    -1,   142,   143,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   152,    -1,   154,    -1,    -1,    -1,    -1,
      -1,    -1,   161,    -1,   163,   164,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   185,   186,   187,   188,
      -1,   190,    -1,   192,    -1,   194,   195,   196,    -1,   198,
      -1,   200,   201,    -1,    -1,    -1,    -1,   206,    -1,    -1,
      -1,    -1,    -1,    -1,   213,    -1,    -1,   216,    -1,    -1,
      -1,   220,   221,   222,   223,    -1,    -1,    -1,    -1,    -1,
      -1,   230,    -1,    -1,    -1,    -1,   235,    -1,    -1,    -1,
     239,    -1,    -1,   242,    -1,    -1,   245,    -1,    -1,    -1,
     249,   250,   251,   252,   253,    -1,   255,    -1,    -1,   258,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     269,   270,   271,   272,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   282,   283,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   292,    -1,   294,    -1,    -1,    -1,    -1,
      -1,   300,   301,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   315,   316,    -1,    -1,
     319,   320,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   335,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   347,    -1,
      -1,    -1,    -1,   352,    -1,   354,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   362,    -1,    -1,   365,   366,   367,   368,
     369,   370,   371,    -1,    -1,    -1,    -1,   103,   377,    -1,
     379,    -1,   108,   109,    -1,    -1,   112,   113,   387,    -1,
      -1,    -1,   391,   119,   120,    -1,   122,   123,    -1,   125,
      -1,   127,    -1,    -1,    -1,    -1,    -1,   133,    -1,    -1,
      -1,    -1,    -1,    -1,   140,    -1,   142,   143,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   152,    -1,   154,    -1,
      -1,    -1,    -1,    -1,    -1,   161,    -1,   163,   164,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   185,
     186,   187,   188,    -1,   190,    -1,   192,    -1,   194,   195,
     196,    -1,   198,    -1,    -1,   201,    -1,    -1,    -1,    -1,
     206,    -1,    -1,    -1,    -1,    -1,    -1,   213,    -1,    -1,
     216,    -1,    -1,    -1,   220,   221,   222,   223,    -1,    -1,
      -1,    -1,    -1,    -1,   230,    -1,    -1,    -1,    -1,   235,
      -1,    -1,    -1,   239,    -1,    -1,   242,    -1,    -1,   245,
      -1,    -1,    -1,   249,   250,   251,   252,   253,    -1,   255,
      -1,    -1,   258,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   269,   270,   271,   272,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   282,   283,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   292,    -1,   294,    -1,
      -1,    -1,    -1,    -1,   300,   301,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   315,
     316,    -1,    -1,   319,   320,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   335,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   347,    -1,    -1,    -1,    -1,   352,    -1,   354,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   362,    -1,    -1,   365,
     366,   367,   368,   369,   370,   371,    -1,    -1,    -1,    -1,
     103,   377,    -1,   379,    -1,   108,   109,    -1,    -1,   112,
     113,   387,    -1,    -1,    -1,   391,   119,   120,    -1,   122,
     123,    -1,   125,    -1,   127,    -1,    -1,    -1,    -1,    -1,
     133,    -1,    -1,    -1,    -1,    -1,    -1,   140,    -1,   142,
     143,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   152,
      -1,   154,    -1,    -1,    -1,    -1,    -1,    -1,   161,    -1,
     163,   164,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   185,   186,   187,   188,    -1,   190,    -1,   192,
      -1,   194,   195,   196,    -1,   198,    -1,    -1,   201,    -1,
      -1,    -1,    -1,   206,    -1,    -1,    -1,    -1,    -1,    -1,
     213,    -1,    -1,   216,    -1,    -1,    -1,   220,   221,   222,
     223,    -1,    -1,    -1,    -1,    -1,    -1,   230,    -1,    -1,
      -1,    -1,   235,    -1,    -1,    -1,   239,    -1,    -1,   242,
      -1,    -1,   245,    -1,    -1,    -1,   249,   250,   251,   252,
      -1,    -1,   255,    -1,    -1,   258,    -1,    -1,    -1,    -1,
     263,    -1,    -1,    -1,    -1,    -1,   269,   270,   271,   272,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     283,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   292,
      -1,   294,    -1,    -1,    -1,    -1,    -1,   300,   301,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   315,   316,    -1,    -1,   319,   320,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   335,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   347,    -1,    -1,    -1,    -1,   352,
      -1,   354,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   362,
      -1,    -1,   365,   366,   367,   368,   369,   370,   371,    -1,
      -1,    -1,    -1,   103,   377,    -1,   379,    -1,   108,   109,
      -1,    -1,   112,   113,   387,    -1,    -1,    -1,   391,   119,
     120,    -1,   122,   123,    -1,   125,    -1,   127,    -1,    -1,
      -1,    -1,    -1,   133,    -1,    -1,    -1,    -1,    -1,    -1,
     140,    -1,   142,   143,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   152,    -1,   154,    -1,    -1,    -1,    -1,    -1,
      -1,   161,    -1,   163,   164,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   185,   186,   187,   188,    -1,
     190,    -1,   192,    -1,   194,   195,   196,    -1,   198,    -1,
      -1,   201,    -1,    -1,    -1,    -1,   206,    -1,    -1,    -1,
      -1,    -1,    -1,   213,    -1,    -1,   216,    -1,    -1,    -1,
     220,   221,   222,   223,    -1,    -1,    -1,    -1,    -1,    -1,
     230,    -1,    -1,    -1,    -1,   235,    -1,    -1,    -1,   239,
      -1,    -1,   242,    -1,    -1,   245,    -1,    -1,    -1,   249,
     250,   251,   252,    -1,    -1,   255,    -1,    -1,   258,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   269,
     270,   271,   272,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   283,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   292,    -1,   294,    -1,    -1,    -1,    -1,    -1,
     300,   301,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   315,   316,    -1,    -1,   319,
     320,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   335,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   347,    -1,    -1,
      -1,   351,   352,    -1,   354,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   362,    -1,    -1,   365,   366,   367,   368,   369,
     370,   371,    -1,    -1,    -1,    -1,   103,   377,    -1,   379,
      -1,   108,   109,    -1,    -1,   112,   113,   387,    -1,    -1,
      -1,   391,   119,   120,    -1,   122,   123,    -1,   125,    -1,
     127,    -1,    -1,    -1,    -1,    -1,   133,    -1,    -1,    -1,
      -1,   138,    -1,   140,    -1,   142,   143,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   152,    -1,   154,    -1,    -1,
      -1,    -1,    -1,    -1,   161,    -1,   163,   164,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   185,   186,
     187,   188,    -1,   190,    -1,   192,    -1,   194,   195,   196,
      -1,   198,    -1,    -1,   201,    -1,    -1,    -1,    -1,   206,
      -1,    -1,    -1,    -1,    -1,    -1,   213,    -1,    -1,   216,
      -1,    -1,    -1,   220,   221,   222,   223,    -1,    -1,    -1,
      -1,    -1,    -1,   230,    -1,    -1,    -1,    -1,   235,    -1,
      -1,    -1,   239,    -1,    -1,   242,    -1,    -1,   245,    -1,
      -1,    -1,   249,   250,   251,   252,    -1,    -1,   255,    -1,
      -1,   258,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   269,   270,   271,   272,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   283,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   292,    -1,   294,    -1,    -1,
      -1,    -1,    -1,   300,   301,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   315,   316,
      -1,    -1,   319,   320,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   335,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     347,    -1,    -1,    -1,    -1,   352,    -1,   354,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   362,    -1,    -1,   365,   366,
     367,   368,   369,   370,   371,    -1,    -1,    -1,    -1,   103,
     377,    -1,   379,    -1,   108,   109,    -1,    -1,   112,   113,
     387,    -1,    -1,    -1,   391,   119,   120,    -1,   122,   123,
      -1,   125,    -1,   127,    -1,    -1,    -1,    -1,    -1,   133,
      -1,    -1,    -1,    -1,   138,    -1,   140,    -1,   142,   143,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   152,    -1,
     154,    -1,    -1,    -1,    -1,    -1,    -1,   161,    -1,   163,
     164,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   185,   186,   187,   188,    -1,   190,    -1,   192,    -1,
     194,   195,   196,    -1,   198,    -1,    -1,   201,    -1,    -1,
      -1,    -1,   206,    -1,    -1,    -1,    -1,    -1,    -1,   213,
      -1,    -1,   216,    -1,    -1,    -1,   220,   221,   222,   223,
      -1,    -1,    -1,    -1,    -1,    -1,   230,    -1,    -1,    -1,
      -1,   235,    -1,    -1,    -1,   239,    -1,    -1,   242,    -1,
      -1,   245,    -1,    -1,    -1,   249,   250,   251,   252,    -1,
      -1,   255,    -1,    -1,   258,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   269,   270,   271,   272,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   283,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   292,    -1,
     294,    -1,    -1,    -1,    -1,    -1,   300,   301,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   315,   316,    -1,    -1,   319,   320,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   335,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   347,    -1,    -1,    -1,    -1,   352,    -1,
     354,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   362,    -1,
      -1,   365,   366,   367,   368,   369,   370,   371,    -1,    -1,
      -1,    -1,   103,   377,    -1,   379,    -1,   108,   109,    -1,
      -1,   112,   113,   387,    -1,    -1,    -1,   391,   119,   120,
      -1,   122,   123,    -1,   125,    -1,   127,    -1,    -1,    -1,
      -1,    -1,   133,    -1,    -1,    -1,    -1,    -1,    -1,   140,
      -1,   142,   143,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   152,    -1,   154,    -1,    -1,    -1,    -1,    -1,    -1,
     161,    -1,   163,   164,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   185,   186,   187,   188,    -1,   190,
      -1,   192,    -1,   194,   195,   196,    -1,   198,    -1,    -1,
     201,    -1,    -1,    -1,    -1,   206,    -1,    -1,    -1,    -1,
      -1,    -1,   213,    -1,    -1,   216,    -1,    -1,    -1,   220,
     221,   222,   223,    -1,    -1,    -1,    -1,    -1,    -1,   230,
      -1,    -1,    -1,    -1,   235,    -1,    -1,    -1,   239,    -1,
      -1,   242,    -1,    -1,   245,    -1,    -1,    -1,   249,   250,
     251,   252,    -1,    -1,   255,    -1,    -1,   258,    -1,    -1,
      -1,    -1,   263,    -1,    -1,    -1,    -1,    -1,   269,   270,
     271,   272,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   283,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   292,    -1,   294,    -1,    -1,    -1,    -1,    -1,   300,
     301,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   315,   316,    -1,    -1,   319,   320,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   335,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   347,    -1,    -1,    -1,
      -1,   352,    -1,   354,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   362,    -1,    -1,   365,   366,   367,   368,   369,   370,
     371,    -1,    -1,    -1,    -1,   103,   377,    -1,   379,    -1,
     108,   109,    -1,    -1,   112,   113,   387,    -1,    -1,    -1,
     391,   119,   120,    -1,   122,   123,    -1,   125,    -1,   127,
      -1,    -1,    -1,    -1,    -1,   133,    -1,    -1,    -1,    -1,
     138,    -1,   140,    -1,   142,   143,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   152,    -1,   154,    -1,    -1,    -1,
      -1,    -1,    -1,   161,    -1,   163,   164,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   185,   186,   187,
     188,    -1,   190,    -1,   192,    -1,   194,   195,   196,    -1,
     198,    -1,    -1,   201,    -1,    -1,    -1,    -1,   206,    -1,
      -1,    -1,    -1,    -1,    -1,   213,    -1,    -1,   216,    -1,
      -1,    -1,   220,   221,   222,   223,    -1,    -1,    -1,    -1,
      -1,    -1,   230,    -1,    -1,    -1,    -1,   235,    -1,    -1,
      -1,   239,    -1,    -1,   242,    -1,    -1,   245,    -1,    -1,
      -1,   249,   250,   251,   252,    -1,    -1,   255,    -1,    -1,
     258,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   269,   270,   271,   272,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   283,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   292,    -1,   294,    -1,    -1,    -1,
      -1,    -1,   300,   301,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   315,   316,    -1,
      -1,   319,   320,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   335,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   347,
      -1,    -1,    -1,    -1,   352,    -1,   354,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   362,    -1,    -1,   365,   366,   367,
     368,   369,   370,   371,    -1,    -1,    -1,    -1,   103,   377,
      -1,   379,    -1,   108,   109,    -1,    -1,   112,   113,   387,
      -1,    -1,    -1,   391,   119,   120,    -1,   122,   123,    -1,
     125,    -1,   127,    -1,    -1,    -1,    -1,    -1,   133,    -1,
      -1,    -1,    -1,    -1,    -1,   140,    -1,   142,   143,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   152,    -1,   154,
      -1,    -1,    -1,    -1,    -1,    -1,   161,    -1,   163,   164,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     185,   186,   187,   188,    -1,   190,    -1,   192,    -1,   194,
     195,   196,    -1,   198,    -1,    -1,   201,    -1,    -1,    -1,
      -1,   206,    -1,    -1,    -1,    -1,    -1,    -1,   213,    -1,
      -1,   216,    -1,    -1,    -1,   220,   221,   222,   223,    -1,
      -1,    -1,    -1,    -1,    -1,   230,    -1,    -1,    -1,    -1,
     235,    -1,    -1,    -1,   239,    -1,    -1,   242,    -1,    -1,
     245,    -1,    -1,    -1,   249,   250,   251,   252,    -1,    -1,
     255,    -1,    -1,   258,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   269,   270,   271,   272,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   283,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   292,    -1,   294,
      -1,    -1,    -1,    -1,    -1,   300,   301,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     315,   316,    -1,    -1,   319,   320,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     335,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   347,    -1,    -1,    -1,   351,   352,    -1,   354,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   362,    -1,    -1,
     365,   366,   367,   368,   369,   370,   371,    -1,    -1,    -1,
      -1,   103,   377,    -1,   379,    -1,   108,   109,    -1,    -1,
     112,   113,   387,    -1,    -1,    -1,   391,   119,   120,    -1,
     122,   123,    -1,   125,    -1,   127,    -1,    -1,    -1,    -1,
      -1,   133,    -1,    -1,    -1,    -1,    -1,    -1,   140,    -1,
     142,   143,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     152,    -1,   154,    -1,    -1,    -1,    -1,    -1,    -1,   161,
      -1,   163,   164,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   185,   186,   187,   188,    -1,   190,    -1,
     192,    -1,   194,   195,   196,    -1,   198,    -1,    -1,   201,
      -1,    -1,    -1,    -1,   206,    -1,    -1,    -1,    -1,    -1,
      -1,   213,    -1,    -1,   216,    -1,    -1,    -1,   220,   221,
     222,   223,    -1,    -1,    -1,    -1,    -1,    -1,   230,    -1,
      -1,    -1,    -1,   235,    -1,    -1,    -1,   239,    -1,    -1,
     242,    -1,    -1,   245,    -1,    -1,    -1,   249,   250,   251,
     252,    -1,    -1,   255,    -1,    -1,   258,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   269,   270,   271,
     272,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   283,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     292,    -1,   294,    -1,    -1,    -1,    -1,    -1,   300,   301,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   315,   316,    -1,    -1,   319,   320,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   335,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   347,    -1,    -1,    -1,    -1,
     352,    -1,   354,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     362,    -1,    -1,   365,   366,   367,   368,   369,   370,   371,
      -1,    -1,    -1,    -1,   103,   377,    -1,   379,    -1,   108,
     109,    -1,    -1,   112,   113,   387,    -1,    -1,    -1,   391,
     119,   120,    -1,   122,   123,    -1,   125,    -1,   127,    -1,
      -1,    -1,    -1,    -1,   133,    -1,    -1,    -1,    -1,    -1,
      -1,   140,    -1,   142,   143,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   152,    -1,   154,    -1,    -1,    -1,    -1,
      -1,    -1,   161,    -1,   163,   164,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   185,   186,   187,   188,
      -1,   190,    -1,   192,    -1,   194,   195,   196,    -1,   198,
      -1,    -1,   201,    -1,    -1,    -1,    -1,   206,    -1,    -1,
      -1,    -1,    -1,    -1,   213,    -1,    -1,    -1,    -1,    -1,
      -1,   220,   221,   222,   223,    -1,    -1,    -1,    -1,    -1,
      -1,   230,    -1,    -1,    -1,    -1,   235,    -1,    -1,    -1,
     239,    -1,    -1,   242,    -1,    -1,   245,    -1,    -1,    -1,
     249,   250,   251,   252,    -1,    -1,   255,    -1,    -1,   258,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     269,   270,   271,   272,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   283,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   292,    -1,   294,    -1,    -1,    -1,    -1,
      -1,   300,   301,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   103,    -1,    -1,   315,   316,   108,   109,
     319,   320,   112,   113,    -1,    -1,    -1,    -1,    -1,    -1,
     120,    -1,   122,   123,    -1,   125,   335,   127,    -1,    -1,
      -1,    -1,    -1,   133,    -1,    -1,    -1,    -1,   347,    -1,
     140,    -1,   142,   143,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   152,   362,   154,    -1,   365,   366,   367,   368,
     369,   370,   371,   163,   164,    -1,    -1,    -1,   377,    -1,
     379,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   387,    -1,
      -1,    -1,   391,    -1,    -1,   185,   186,    -1,   188,    -1,
     190,    -1,   192,    -1,   194,   195,   196,    -1,   198,    -1,
      -1,   201,    -1,    -1,    -1,    -1,   206,    -1,    -1,    -1,
      -1,    -1,    -1,   213,    -1,    -1,    -1,    -1,    -1,    -1,
     220,   221,   222,   223,    -1,    -1,    -1,    -1,    -1,    -1,
     230,    -1,    -1,    -1,    -1,   235,    -1,    -1,    -1,   239,
      -1,    -1,   242,    -1,    -1,   245,    -1,    -1,    -1,    -1,
     250,   251,    -1,    -1,    -1,   255,    -1,    -1,   258,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   269,
     270,   271,   272,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   283,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   294,    -1,    -1,    -1,    -1,    -1,
      -1,   301,    -1,   103,    -1,    -1,    -1,    -1,   108,   109,
      -1,    -1,   112,   113,    -1,   315,   316,    -1,    -1,    -1,
     120,    -1,   122,   123,    -1,   125,    -1,   127,    -1,    -1,
      -1,    -1,    -1,   133,    -1,    -1,    -1,    -1,   338,   339,
     140,    -1,   142,   143,    -1,    -1,    -1,   347,    -1,    -1,
      -1,    -1,   152,    -1,   154,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   163,   164,   365,   366,   367,   368,   369,
     370,   371,    -1,    -1,    -1,    -1,    -1,   377,    -1,   379,
      -1,    -1,    -1,    -1,    -1,   185,   186,   387,   188,    -1,
     190,   391,   192,    -1,   194,   195,   196,    -1,   198,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   206,    -1,    -1,    -1,
      -1,    -1,    -1,   213,    -1,    -1,    -1,    -1,    -1,    -1,
     220,   221,   222,   223,    -1,    -1,    -1,    -1,    -1,    -1,
     230,    -1,    -1,    -1,   234,   235,    -1,    -1,    -1,   239,
      -1,    -1,   242,    -1,    -1,   245,    -1,    -1,    -1,    -1,
     250,   251,    -1,    -1,    -1,   255,    -1,    -1,   258,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   269,
     270,   271,   272,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   283,    -1,    -1,    -1,    -1,    -1,    -1,
     103,    -1,    -1,    -1,   294,   108,   109,    -1,    -1,   112,
     113,   301,    -1,    -1,    -1,    -1,    -1,   120,    -1,   122,
     123,    -1,   125,    -1,   127,   315,   316,    -1,    -1,    -1,
     133,    -1,    -1,    -1,    -1,    -1,    -1,   140,    -1,   142,
     143,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   152,
      -1,   154,    -1,    -1,    -1,    -1,    -1,   347,    -1,    -1,
     163,   164,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     370,   371,   185,   186,    -1,   188,    -1,   190,    -1,   192,
      -1,   194,   195,   196,    -1,   198,    -1,   387,    -1,    -1,
      -1,   391,    -1,   206,    -1,    -1,    -1,    -1,    -1,    -1,
     213,    -1,    -1,    -1,    -1,    -1,    -1,   220,   221,   222,
     223,    -1,    -1,    -1,    -1,    -1,    -1,   230,    -1,    -1,
      -1,    -1,   235,    -1,    -1,    -1,   239,    -1,    -1,   242,
      -1,    -1,   245,    -1,    -1,    -1,    -1,   250,   251,    -1,
      -1,    -1,   255,    -1,    -1,   258,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   269,   270,   271,   272,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     283,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   294,    -1,    -1,    -1,    -1,    -1,    -1,   301,    -1,
     103,    -1,    -1,    -1,    -1,   108,   109,    -1,    -1,   112,
     113,    -1,   315,   316,    -1,    -1,    -1,   120,    -1,   122,
     123,    -1,   125,    -1,   127,    -1,   129,    -1,    -1,    -1,
     133,    -1,    -1,    -1,    -1,    -1,    -1,   140,    -1,   142,
     143,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   152,
      -1,   154,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     163,   164,    -1,   366,    -1,    -1,    -1,   370,   371,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   185,   186,   387,   188,    -1,   190,   391,   192,
      -1,   194,   195,   196,    -1,   198,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   206,    -1,    -1,    -1,    -1,    -1,    -1,
     213,    -1,    -1,    -1,    -1,    -1,    -1,   220,   221,   222,
     223,    -1,    -1,    -1,    -1,    -1,    -1,   230,    -1,    -1,
      -1,    -1,   235,    -1,    -1,    -1,   239,    -1,    -1,   242,
      -1,    -1,   245,    -1,    -1,    -1,    -1,   250,   251,    -1,
      -1,    -1,   255,    -1,    -1,   258,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   269,   270,   271,   272,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     283,    -1,    -1,    -1,    -1,    -1,    -1,   103,    -1,    -1,
      -1,   294,   108,   109,    -1,    -1,   112,   113,   301,    -1,
      -1,    -1,    -1,    -1,   120,    -1,   122,   123,    -1,   125,
      -1,   127,   315,   316,    -1,    -1,    -1,   133,    -1,    -1,
      -1,    -1,    -1,    -1,   140,    -1,   142,   143,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   152,    -1,   154,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   163,   164,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   370,   371,   185,
     186,    -1,   188,    -1,   190,    -1,   192,    -1,   194,   195,
     196,    -1,   198,    -1,   387,    -1,    -1,    -1,   391,    -1,
     206,    -1,    -1,    -1,    -1,    -1,    -1,   213,    -1,    -1,
      -1,    -1,    -1,    -1,   220,   221,   222,   223,    -1,    -1,
      -1,    -1,    -1,    -1,   230,    -1,    -1,    -1,    -1,   235,
      -1,    -1,    -1,   239,    -1,    -1,   242,    -1,    -1,   245,
      -1,    -1,    -1,    -1,   250,   251,    -1,    -1,    -1,   255,
      -1,    -1,   258,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   269,   270,   271,   272,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   283,    -1,    -1,
      -1,    -1,    -1,    -1,   103,    -1,    -1,    -1,   294,   108,
     109,    -1,    -1,   112,   113,   301,    -1,    -1,    -1,    -1,
      -1,   120,    -1,   122,   123,    -1,   125,    -1,   127,   315,
     316,    -1,    -1,    -1,   133,    -1,    -1,    -1,    -1,    -1,
      -1,   140,    -1,   142,   143,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   152,    -1,   154,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   163,   164,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   368,    -1,   370,   371,   185,   186,    -1,   188,
      -1,   190,    -1,   192,    -1,   194,   195,   196,    -1,   198,
      -1,   387,    -1,    -1,    -1,   391,    -1,   206,    -1,    -1,
      -1,    -1,    -1,    -1,   213,    -1,    -1,    -1,    -1,    -1,
      -1,   220,   221,   222,   223,    -1,    -1,    -1,    -1,    -1,
      -1,   230,    -1,    -1,    -1,    -1,   235,    -1,    -1,    -1,
     239,    -1,    -1,   242,    -1,    -1,   245,    -1,    -1,    -1,
      -1,   250,   251,    -1,    -1,    -1,   255,    -1,    -1,   258,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     269,   270,   271,   272,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   283,    -1,    -1,    -1,    -1,    -1,
      -1,   103,    -1,    -1,    -1,   294,   108,   109,    -1,    -1,
     112,   113,   301,    -1,    -1,    -1,    -1,    -1,   120,    -1,
     122,   123,    -1,   125,    -1,   127,   315,   316,    -1,    -1,
      -1,   133,    -1,    -1,    -1,    -1,    -1,    -1,   140,    -1,
     142,   143,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     152,    -1,   154,    -1,    -1,    -1,    -1,    -1,   347,    -1,
      -1,   163,   164,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   370,   371,   185,   186,    -1,   188,    -1,   190,    -1,
     192,    -1,   194,   195,   196,    -1,   198,    -1,   387,    -1,
      -1,    -1,   391,    -1,   206,    -1,    -1,    -1,    -1,    -1,
      -1,   213,    -1,    -1,    -1,    -1,    -1,    -1,   220,   221,
     222,   223,    -1,    -1,    -1,    -1,    -1,    -1,   230,    -1,
      -1,    -1,    -1,   235,    -1,    -1,    -1,   239,    -1,    -1,
     242,    -1,    -1,   245,    -1,    -1,    -1,    -1,   250,   251,
      -1,    -1,    -1,   255,    -1,    -1,   258,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   269,   270,   271,
     272,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   283,    -1,    -1,    -1,    -1,    -1,    -1,   103,    -1,
      -1,    -1,   294,   108,   109,    -1,    -1,   112,   113,   301,
      -1,    -1,    -1,    -1,    -1,   120,    -1,   122,   123,    -1,
     125,    -1,   127,   315,   316,    -1,    -1,    -1,   133,    -1,
      -1,    -1,    -1,    -1,    -1,   140,    -1,   142,   143,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   152,    -1,   154,
      -1,    -1,    -1,    -1,    -1,   347,    -1,    -1,   163,   164,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   370,   371,
     185,   186,    -1,   188,    -1,   190,    -1,   192,    -1,   194,
     195,   196,    -1,   198,    -1,   387,    -1,    -1,    -1,   391,
      -1,   206,    -1,    -1,    -1,    -1,    -1,    -1,   213,    -1,
      -1,    -1,    -1,    -1,    -1,   220,   221,   222,   223,    -1,
      -1,    -1,    -1,    -1,    -1,   230,    -1,    -1,    -1,    -1,
     235,    -1,    -1,    -1,   239,    -1,    -1,   242,    -1,    -1,
     245,    -1,    -1,    -1,    -1,   250,   251,    -1,    -1,    -1,
     255,    -1,    -1,   258,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   269,   270,   271,   272,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   283,    -1,
      -1,    -1,    -1,    -1,    -1,   103,    -1,    -1,    -1,   294,
     108,   109,    -1,    -1,   112,   113,   301,    -1,    -1,    -1,
      -1,    -1,   120,    -1,   122,   123,    -1,   125,    -1,   127,
     315,   316,    -1,    -1,    -1,   133,    -1,    -1,    -1,    -1,
      -1,    -1,   140,    -1,   142,   143,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   152,    -1,   154,    -1,    -1,    -1,
      -1,    -1,   347,    -1,    -1,   163,   164,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   370,   371,   185,   186,    -1,
     188,    -1,   190,    -1,   192,    -1,   194,   195,   196,    -1,
     198,    -1,   387,    -1,    -1,    -1,   391,    -1,   206,    -1,
      -1,    -1,    -1,    -1,    -1,   213,    -1,    -1,    -1,    -1,
      -1,    -1,   220,   221,   222,   223,    -1,    -1,    -1,    -1,
      -1,    -1,   230,    -1,    -1,    -1,    -1,   235,    -1,    -1,
      -1,   239,    -1,    -1,   242,    -1,    -1,   245,    -1,    -1,
      -1,    -1,   250,   251,    -1,    -1,    -1,   255,    -1,    -1,
     258,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   269,   270,   271,   272,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   283,    -1,    -1,    -1,    -1,
      -1,    -1,   103,    -1,    -1,    -1,   294,   108,   109,    -1,
      -1,   112,   113,   301,    -1,    -1,    -1,    -1,    -1,   120,
      -1,   122,   123,    -1,   125,    -1,   127,   315,   316,    -1,
      -1,    -1,   133,    -1,    -1,    -1,    -1,    -1,    -1,   140,
      -1,   142,   143,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   152,    -1,   154,    -1,    -1,    -1,    -1,    -1,   347,
      -1,    -1,   163,   164,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   370,   371,   185,   186,    -1,   188,    -1,   190,
      -1,   192,    -1,   194,   195,   196,    -1,   198,    -1,   387,
      -1,    -1,    -1,   391,    -1,   206,    -1,    -1,    -1,    -1,
      -1,    -1,   213,    -1,    -1,    -1,    -1,    -1,    -1,   220,
     221,   222,   223,    -1,    -1,    -1,    -1,    -1,    -1,   230,
      -1,   105,    -1,    -1,   235,    -1,    -1,    -1,   239,    -1,
      -1,   242,    -1,    -1,   245,    -1,    -1,    -1,    -1,   250,
     251,   125,   126,    -1,   255,    -1,    -1,   258,   132,    -1,
      -1,    -1,    -1,    -1,   138,   139,   140,    -1,   269,   270,
     271,   272,   146,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     154,    -1,   283,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   165,    -1,   294,    -1,    -1,    -1,   171,   105,    -1,
     301,    -1,    -1,    -1,   178,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   315,   316,    -1,    -1,   125,   126,
     194,   195,    -1,    -1,    -1,   132,    -1,    -1,    -1,    -1,
      -1,   138,   139,   140,    -1,    -1,    -1,    -1,    -1,   146,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   154,    -1,    -1,
      -1,    -1,    -1,    -1,   228,   229,    -1,   231,   165,   233,
     234,    -1,    -1,   237,   171,    -1,    -1,    -1,    -1,   370,
     371,   178,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   256,    -1,    -1,    -1,   387,   194,   195,    -1,
     391,    -1,   266,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   291,    -1,    -1,
      -1,   228,   229,    -1,   231,    -1,   233,   234,   302,   303,
     237,    -1,    -1,    -1,    -1,   309,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   318,    -1,    -1,    -1,    -1,   256,
     324,   325,    -1,    -1,    -1,   105,    -1,    -1,    -1,   266,
      -1,    -1,    -1,    -1,    -1,   115,    -1,    -1,    -1,   119,
      -1,    -1,   122,   347,    -1,    -1,   126,    -1,    -1,    -1,
      -1,   131,    -1,    -1,   291,    -1,    -1,   137,    -1,   139,
      -1,    -1,    -1,   367,    -1,   302,   303,   371,    -1,    -1,
      -1,    -1,   309,    -1,   154,    -1,   156,    -1,    -1,   159,
      -1,   318,   162,    -1,    -1,   165,    -1,   324,   325,   393,
     170,    -1,    -1,    -1,   174,    -1,    -1,    -1,   178,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     347,    -1,    -1,   193,   194,   195,    -1,    -1,    -1,    -1,
      -1,   201,    -1,    -1,    -1,    -1,   206,    -1,    -1,    -1,
     367,    -1,    -1,    -1,   371,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   228,   105,
      -1,   231,    -1,   233,   234,    -1,   393,   237,    -1,   115,
      -1,    -1,    -1,   119,    -1,    -1,   122,    -1,    -1,    -1,
     126,    -1,    -1,    -1,    -1,   131,   256,    -1,    -1,    -1,
      -1,   137,    -1,   139,    -1,   265,   266,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   153,   154,    -1,
      -1,    -1,    -1,   159,    -1,    -1,   162,    -1,    -1,   165,
     290,    -1,    -1,    -1,   170,    -1,    -1,    -1,   174,    -1,
      -1,    -1,   178,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   193,   194,   195,
      -1,    -1,    -1,    -1,    -1,   201,    -1,    -1,   105,    -1,
     206,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   115,    -1,
      -1,    -1,   119,    -1,    -1,   122,    -1,   347,    -1,   126,
      -1,    -1,   228,    -1,   131,   231,    -1,   233,   234,   359,
     137,   237,   139,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     370,   371,    -1,    -1,    -1,    -1,    -1,   154,    -1,    -1,
     256,    -1,   159,    -1,    -1,   162,    -1,    -1,   165,   265,
     266,    -1,    -1,   170,    -1,    -1,    -1,   174,    -1,    -1,
      -1,   178,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   290,    -1,   193,   194,   195,    -1,
      -1,    -1,    -1,    -1,   201,    -1,    -1,    -1,    -1,   206,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   228,    -1,    -1,   231,    -1,   233,   234,    -1,    -1,
     237,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   347,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   256,
      -1,     4,    -1,   359,     7,     8,     9,    -1,   265,   266,
      13,    14,    15,    16,   370,   371,    19,    20,    21,    -1,
      23,    24,    -1,    26,    27,    -1,    29,    30,    31,    32,
      33,    -1,    -1,   290,    -1,    -1,    -1,    40,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      63,    64,    65,    66,    67,    68,    69,    70,    71,    72,
      73,    74,    75,    76,    77,    78,    79,    80,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     347,    -1,     7,     8,     9,    -1,    -1,    -1,    13,    14,
      -1,    16,   359,    -1,    19,    20,    -1,    -1,    23,    24,
      -1,    -1,    27,   370,   371,    30,    31,    32,    33,    -1,
      35,    -1,    -1,    -1,    -1,    40,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    63,    64,
      65,    66,    67,    68,    69,    70,    71,    72,    73,    74,
      75,    76,    77,    78,    79,    80,     7,     8,     9,    -1,
      -1,    -1,    13,    14,    -1,    16,    -1,    -1,    19,    20,
      -1,    -1,    23,    24,    -1,    -1,    27,    -1,    -1,    30,
      31,    32,    33,    -1,    -1,    -1,    -1,    -1,    -1,    40,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    63,    64,    65,    66,    67,    68,    69,    70,
      71,    72,    73,    74,    75,    76,    77,    78,    79,    80
};

/* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
   symbol of state STATE-NUM.  */
static const yytype_uint16 yystos[] =
{
       0,     3,     4,     7,     8,     9,    13,    14,    15,    16,
      19,    20,    21,    23,    24,    26,    27,    29,    30,    31,
      32,    33,    39,    40,    63,    64,    65,    66,    67,    68,
      69,    70,    71,    72,    73,    74,    75,    76,    77,    78,
      79,    80,    83,    84,    85,    91,    92,    93,    94,    95,
      96,    99,   407,   414,   432,   433,   434,   456,   457,   461,
     462,   464,   467,   474,   477,   478,   481,   504,   506,   508,
     509,   510,   511,   512,   513,   514,   515,   516,   517,   518,
      81,    81,   430,   506,    87,    89,    86,    98,    98,    98,
      98,   112,   115,   153,   395,   403,   404,   519,   557,   558,
     559,   560,   477,   462,   481,   482,     0,   433,   428,   458,
     460,   477,   457,   457,   430,   465,   466,   506,   430,   456,
     457,   495,   498,   499,   505,   407,   409,   479,   480,   478,
     475,   476,   506,   430,    90,    90,   367,   371,   137,   137,
     347,   200,   402,   105,   122,   125,   126,   128,   132,   137,
     138,   139,   140,   144,   146,   147,   154,   158,   162,   165,
     171,   178,   194,   195,   206,   226,   228,   229,   231,   233,
     234,   237,   254,   256,   266,   291,   302,   303,   309,   318,
     324,   325,   347,   371,   373,   375,   376,   381,   386,   393,
     396,   520,   521,   522,   529,   530,   531,   535,   536,   537,
     538,   539,   540,   541,   543,   544,   545,   546,   548,   549,
     550,   551,   552,   553,   554,   556,   561,   562,   563,   564,
     565,   566,   567,   568,   572,   573,   576,   577,   578,   580,
     581,   582,   586,   587,   598,   599,   600,   601,   602,   603,
     609,   610,   611,   614,   615,   616,   637,   638,   639,   641,
     642,   643,   644,   645,   647,   653,   656,   674,   677,   681,
     685,   686,   698,   740,   741,   742,   748,   749,   750,   751,
     754,   755,   756,   757,   758,   759,   760,   762,   763,   770,
     771,   772,   773,   774,   784,   792,   807,   808,   809,   810,
     811,   814,   815,   892,   955,   956,   957,   958,   959,   960,
    1036,  1037,  1038,  1039,  1040,  1046,  1048,  1053,  1059,  1060,
    1061,  1062,  1064,  1065,  1083,  1084,  1085,  1086,  1102,  1103,
    1104,  1105,  1108,  1110,  1111,  1119,  1120,   358,   358,   358,
     408,   462,   481,   412,   428,   459,   427,   505,   429,   468,
     469,   470,   482,   508,   429,   468,   430,   477,     5,     6,
      10,    11,    12,    17,    18,    22,    25,    28,    34,    37,
      38,    41,    52,    53,   407,   413,   414,   415,   416,   417,
     418,   428,   429,   435,   436,   438,   439,   440,   441,   442,
     443,   444,   445,   446,   447,   448,   449,   450,   451,   452,
     454,   456,   493,   494,   495,   496,   497,   500,   501,   502,
     503,   506,   507,   518,   456,   495,   408,   483,   484,   506,
     410,   438,   451,   455,   506,   482,   485,   486,   487,   488,
     412,   429,   427,   475,    88,    89,   214,   214,   403,   385,
     131,   170,   371,   383,   136,   169,   218,   220,   235,   236,
     243,   248,   257,   262,   286,   298,   313,   317,   371,   371,
     398,   205,   267,   579,   367,   133,   136,   142,   169,   191,
     207,   218,   219,   220,   235,   242,   243,   244,   248,   251,
     253,   257,   278,   286,   298,   317,   371,   621,   622,   743,
     752,   371,   103,   108,   109,   112,   113,   119,   120,   122,
     123,   125,   127,   133,   140,   142,   143,   152,   154,   161,
     163,   164,   185,   186,   187,   188,   190,   192,   194,   195,
     196,   198,   201,   206,   213,   216,   220,   221,   222,   223,
     230,   235,   239,   242,   245,   249,   250,   251,   252,   255,
     258,   269,   270,   271,   272,   283,   292,   294,   300,   301,
     315,   316,   319,   320,   335,   347,   352,   354,   362,   365,
     366,   367,   368,   369,   370,   371,   377,   379,   387,   391,
     569,   571,   919,   921,   922,   923,   924,   925,   926,   929,
     936,   950,   974,  1121,  1124,   364,   816,   816,   136,   142,
     169,   218,   219,   220,   235,   242,   243,   248,   251,   257,
     262,   278,   286,   298,   313,   317,   371,   752,   117,   234,
     816,   115,   371,   388,   555,   167,   216,   372,   384,   391,
     392,   397,   405,   532,   533,   366,   367,   371,   104,   105,
     132,   139,   146,   158,   171,   178,   221,   224,   234,   256,
     268,   286,   309,   604,   605,  1121,   816,   816,   816,   371,
     160,   200,   253,   282,   896,   897,   898,   899,   900,   919,
     921,   604,   579,  1121,   816,   103,   371,  1121,   160,   816,
     812,   813,  1121,   584,  1121,   243,   243,   243,   243,   243,
     243,   810,   165,   382,   203,   205,   203,   205,   382,   396,
     271,   367,   371,   358,   371,   807,   358,   105,   126,   132,
     165,   231,   367,   562,   563,   566,   367,   562,   563,   273,
     358,   358,   762,   763,   770,   566,   158,   162,   386,   398,
     398,   584,   163,   188,   778,   165,   885,   179,   208,   252,
     287,   799,   879,   810,   358,   358,   358,   584,   584,   584,
     584,  1121,   460,   430,   452,   491,    82,   463,   429,   469,
     426,   471,   473,   477,   463,   429,   428,   455,   428,   426,
     493,   407,   506,   428,   454,   407,   438,   407,   407,   407,
     507,   407,   438,   438,   454,   482,   488,    52,    53,    54,
     407,   409,   411,    42,    43,    44,    45,    46,    47,    48,
      49,    50,    51,   427,   453,   440,   414,   419,   420,   415,
     416,    58,    59,    61,    62,   421,   422,    56,    57,   413,
     423,   424,    55,    60,   425,   412,   428,   429,   496,   426,
     408,   412,   410,   407,   409,   477,   481,   489,   490,   408,
     412,   476,   455,   429,   112,   358,   360,   131,   170,   371,
     383,   371,   131,   371,   378,   109,   113,   115,   132,   153,
     198,   223,   291,   321,  1049,  1121,   584,   584,   584,   584,
     122,   237,   584,   584,  1121,   584,   584,   262,  1121,  1121,
     108,   237,   371,   371,   124,   243,   371,   285,   317,   277,
    1121,   253,   292,   242,   584,   584,   584,   584,   317,   286,
    1121,  1121,   262,  1121,   371,   313,   262,   136,   241,   374,
     390,   401,   523,   263,   921,   934,   935,   347,   926,   120,
     569,   927,  1121,   355,   266,   801,   896,   949,   926,   926,
     175,   209,   389,  1134,  1135,  1136,   367,   367,   363,   928,
     347,   346,   343,   352,   354,   351,   356,   347,   349,   355,
     167,   817,   351,   820,   822,   823,   921,  1121,   277,  1121,
    1121,   584,   584,   242,   584,   584,   584,   584,   584,   584,
    1121,   584,  1121,   584,   262,  1121,  1121,   371,   136,   261,
     371,   180,   258,   367,   972,  1041,  1042,  1044,  1045,  1121,
    1131,   352,   354,   366,   534,   534,   371,   167,  1124,   217,
     107,   236,   257,   317,   371,   107,   218,   219,   235,   236,
     242,   243,   248,   257,   262,   278,   298,   317,   107,   107,
     219,   257,   317,   107,   107,   107,   107,   107,   107,   205,
     247,   353,   104,   180,   180,   180,   258,   367,   900,   347,
     950,   950,   207,   106,   346,   176,   181,   189,   200,   281,
     345,   359,   360,   361,   901,   902,   903,   904,   905,   906,
     907,   908,   909,   910,   911,   912,   913,   914,   915,   916,
     917,   918,   167,   205,   247,   371,   104,   145,   819,   360,
     182,   221,   583,   360,   347,   584,   761,   353,   347,   744,
     247,   355,   584,   584,  1121,   584,  1121,  1121,   348,   366,
     367,   367,   371,   367,   371,   371,   367,  1125,   353,   167,
     165,   526,   526,   236,   579,   207,   579,   358,   371,   532,
     271,   205,   295,   371,   886,   256,   266,   747,   116,   371,
     104,   814,   190,   193,   881,   883,   884,   879,   347,   962,
     962,   110,   181,   961,   102,   114,  1087,  1089,   110,   491,
     492,   463,   455,   412,   428,   472,   426,   463,   426,   493,
     518,    34,   428,   454,   428,   428,   488,   454,   454,   454,
     408,   407,   481,   408,   506,   408,   437,   452,   454,   506,
     452,   440,   440,   440,   441,   441,   442,   442,   443,   443,
     443,   443,   444,   444,   445,   446,   447,   448,   449,   450,
     452,   493,   518,    35,   506,   408,   485,   489,   410,   455,
     490,   407,   409,    35,   487,   371,   161,   249,   371,   131,
     371,   378,   347,   136,   192,   317,   371,   371,   371,   136,
     371,   240,   371,   236,   371,  1057,  1058,   127,   127,   371,
     135,   143,   152,   199,   240,   371,   654,   655,   673,   684,
     136,   313,   371,   375,   101,   104,   105,   123,   143,   146,
     152,   194,   239,   283,   291,   292,   310,   318,   371,   662,
     127,   143,   152,  1101,   138,   143,   152,   244,   283,   285,
     590,   591,   592,   593,   594,   595,   596,   127,   222,   237,
     291,   371,   678,   679,   584,   101,   146,   164,   237,   240,
     371,   105,   106,   115,   146,   153,   291,   311,   312,  1063,
    1078,   371,   371,   237,   283,   323,   371,   585,  1121,   584,
     136,   116,  1121,  1121,   371,  1047,   286,   142,   169,   218,
     248,   251,   371,   743,   584,   347,   654,   682,   165,   167,
     178,   212,   221,   310,   317,   347,   699,   701,   706,   707,
     738,  1140,  1121,   285,   584,   371,   157,   158,  1043,  1121,
     584,   277,   400,   524,   896,   263,   930,   931,   150,   933,
     935,   896,   355,   569,  1121,   803,   804,  1121,   234,   806,
     951,   952,   348,   353,   209,  1134,  1134,   367,   395,  1132,
    1133,  1132,   348,   949,   315,   923,   924,   924,   925,   925,
     104,   145,   348,   351,   949,   896,   970,   139,   216,   282,
     384,   391,   392,   569,   570,  1121,   761,   180,  1126,   353,
     110,   368,   824,  1121,   355,  1121,   584,   118,   652,   118,
     676,   584,   371,  1082,   277,   165,   165,   584,   148,   367,
     377,   379,   547,  1122,  1134,   358,   358,   120,   936,   971,
    1121,   347,   355,   357,   366,   366,   180,   258,   218,   235,
     243,   248,   286,   298,   371,   142,   218,   235,   243,   248,
     262,   286,   298,   371,   242,   262,   243,   142,   218,   235,
     242,   243,   248,   262,   278,   286,   298,   371,   242,   218,
     217,   278,   243,   235,   243,   243,   243,   142,   584,   219,
     606,   607,  1121,   605,   180,   764,   765,   761,   584,   584,
     547,  1123,  1124,   898,   899,   371,   385,   200,   201,   921,
     176,   189,   281,   921,   360,   104,   107,   238,   360,   361,
     104,   107,   238,   104,   107,   238,   360,   921,   921,   921,
     921,   921,   921,   201,   338,   339,   347,   365,   366,   368,
     369,   569,   856,   920,   950,  1121,  1124,   920,   920,   920,
     920,   920,   920,   920,   920,   920,   920,   920,   606,   142,
     584,   233,   136,   820,   143,   152,   187,   268,   394,  1121,
     266,   347,   796,   797,   798,   800,   802,   805,   806,   212,
    1144,   824,   813,   569,   745,   746,   110,  1121,  1121,   247,
     175,   355,  1144,   347,   212,   648,   266,   542,   542,   353,
     367,   221,   256,   237,   292,   258,   371,  1125,   584,   366,
     886,   221,   888,   889,   921,   116,   882,   896,   896,   883,
     348,   963,   964,  1121,   228,   961,   250,   981,   985,   990,
     205,   139,   178,   256,  1088,   368,   412,   429,   473,   455,
     493,   518,   407,   428,   454,   428,   408,   408,   408,   408,
     440,   408,   412,   410,   426,   408,   408,   410,   408,   485,
     410,   455,   358,   348,   348,   347,   247,   247,  1121,   368,
     255,  1054,   368,   371,   122,   126,   159,   231,   371,   388,
     371,   266,   116,   354,   366,   655,   371,   277,   360,   360,
     371,   583,   360,   124,   129,   212,   672,   675,   715,   716,
     286,   212,   317,   675,   212,   232,   129,   191,   212,   215,
     253,   675,   232,   213,   212,   212,  1140,   124,   129,   212,
     247,   584,   366,   212,   212,   661,   317,   371,   371,   317,
    1121,   116,   596,   212,   371,   679,   247,   203,   205,   360,
     680,   371,  1109,   243,   371,   243,   371,   104,   295,   631,
     269,   288,   312,   371,   112,   266,   371,   636,   243,   269,
     371,   394,   632,   633,   371,   371,   113,   371,   113,   371,
     240,   371,   575,   360,   125,   143,   152,   153,   240,   371,
    1043,  1114,  1116,  1118,   371,   371,   181,   355,   181,   277,
     367,   371,   128,   258,   753,   360,  1049,   371,   262,   165,
     366,   569,   718,   719,   683,   684,   584,   243,   371,   116,
     268,   371,   394,   366,  1121,   713,   714,   715,   719,   271,
     708,   709,   710,   143,   152,   700,   707,   706,   371,   116,
     744,   368,  1066,  1067,  1044,   165,   627,   744,  1121,   134,
     246,   932,   921,   931,   933,   921,   153,   110,   569,  1121,
     353,   744,   816,   799,   883,   896,  1134,  1135,   367,  1135,
     348,   351,   949,   949,   211,   940,   348,   348,   350,   928,
     347,   349,   355,   824,   367,   377,   379,  1127,   167,   775,
     823,   368,   569,   351,  1121,   130,   371,  1121,   371,   371,
     347,   730,  1124,  1134,   367,   367,   353,   153,   153,   355,
     348,   949,   972,  1121,   360,  1127,   148,   262,   262,   262,
    1121,   247,   353,   761,   765,   796,   824,  1144,  1144,   353,
     201,   155,   921,   921,   106,   104,   107,   238,   104,   107,
     238,   104,   107,   238,   104,   107,   238,   949,   355,  1121,
     167,  1121,   277,   180,  1128,   221,   371,   803,   800,   348,
     799,   879,   800,   347,   237,   348,   353,   347,   114,   232,
     371,   893,  1121,   175,   212,   646,   116,   700,   371,   367,
     394,   202,   295,   371,   527,   371,  1122,   180,   347,   326,
     327,   328,   329,   330,   331,   887,   394,   353,   111,   141,
     890,   888,   353,   371,   348,   353,   175,   209,   965,   569,
     977,   978,  1121,   134,   980,   982,   983,   984,   986,   987,
     988,   990,  1121,  1121,   966,  1121,   153,   990,   584,   202,
    1090,   207,   429,   491,   454,   408,   454,   428,   428,   454,
     493,   493,   493,   452,   451,   408,   410,   368,   368,   247,
     110,  1056,   371,   258,  1055,   247,   366,   366,   366,   354,
     366,   354,   366,   366,   243,   104,  1121,   138,   161,   249,
     371,   366,   368,   371,   360,   161,   249,  1121,  1121,   286,
     669,   347,   121,   166,   191,   215,   253,   673,  1121,  1121,
     271,   347,   196,  1121,   253,  1121,   184,   347,   569,   196,
    1121,  1121,  1121,   569,  1121,  1121,  1121,   661,  1121,  1121,
    1121,  1121,   203,   205,   597,  1121,  1121,   584,   203,   205,
     205,   371,  1107,   167,   355,   630,   368,   624,   625,   630,
     624,   295,   366,   366,   371,   621,   143,   152,   624,   673,
     371,   312,   634,   635,  1121,   366,   269,   633,   368,  1066,
     368,  1080,  1081,  1080,   354,   366,   368,   371,   368,   921,
     921,   366,  1044,   371,   368,  1121,   368,   206,   258,   366,
     247,  1121,   258,   366,  1076,   371,  1050,  1051,  1052,   584,
     584,   260,   308,   720,   726,   727,  1121,   348,   353,   371,
     190,  1121,   348,   353,   272,   347,   110,   710,   232,   232,
     706,  1066,  1121,   699,   371,  1068,  1069,   294,   307,   353,
    1071,  1072,  1074,  1075,   240,   332,   371,   110,   628,   110,
     258,   753,   266,   525,   921,   932,   153,   727,   355,   804,
     110,   819,   952,   348,   348,   348,   348,   347,   305,   953,
     279,   937,   355,   348,   949,   970,   569,   570,  1121,   264,
     895,  1134,   367,   367,   353,   776,   777,  1121,   895,   355,
     106,   118,   612,   180,   180,   569,   734,   259,  1132,  1132,
     367,   377,   379,   358,   358,   391,  1121,   348,   355,  1124,
     247,   606,   607,   347,   791,   138,   347,   791,   796,   824,
     730,  1124,   921,   155,   106,   921,   348,   569,  1121,   167,
     606,   367,   371,   377,   379,  1129,  1130,   167,   825,   371,
     221,   348,   805,   883,   879,  1121,   347,   569,   779,   780,
     781,  1121,   746,   808,   146,   371,   232,   256,   288,   175,
    1144,   893,  1121,   348,  1121,   651,  1140,   366,   528,  1121,
     366,   360,  1127,   732,   896,   889,   301,   891,   896,   896,
     964,   209,   977,   346,   961,   347,   355,  1121,   966,   115,
     984,   156,   284,   989,   181,   225,  1091,   734,   139,   178,
     256,   408,   493,   408,   408,   454,   408,   454,   428,    36,
     368,   368,   368,   113,   368,   366,   366,   203,   205,   366,
     394,   317,  1137,  1138,   347,   715,   717,   718,   896,   184,
     253,   184,   347,   731,   317,   286,   666,   272,   347,   569,
     347,   676,   569,   733,   353,   112,   259,  1140,   247,   247,
     247,   258,   657,   658,   126,   371,  1107,  1121,   353,   625,
     366,   266,   347,   353,   355,   366,   311,   312,   371,  1069,
    1079,   353,   247,   366,   371,   191,   574,   355,   367,   367,
     371,  1068,  1121,  1121,   371,   237,   371,   371,  1052,   371,
    1051,   744,   175,   721,   720,   371,   728,   729,   347,   738,
     739,   719,   584,   366,   347,   699,   714,   110,   734,   796,
     196,   196,  1073,  1074,   588,   589,   591,   594,   595,   596,
     708,   203,   205,   366,   371,  1068,   371,   366,  1077,  1067,
    1072,   921,   280,   371,   371,   617,   796,  1121,   258,   228,
     387,   165,   348,   569,   347,   820,   953,   953,   940,   212,
     941,   347,   940,   172,   953,   569,   348,   350,   928,   347,
     355,   896,   880,   881,  1132,  1132,   367,   377,   379,   355,
     351,   371,   130,  1124,  1124,   348,   353,   347,  1135,  1135,
    1134,   367,   367,   355,   972,  1121,   606,   266,   608,   781,
     790,   259,   259,   790,   259,   258,   167,   921,   921,   355,
     606,   612,  1129,  1129,  1134,   353,   243,   322,   323,   347,
     826,   827,   857,   860,  1121,   895,   883,   348,   569,   782,
     783,  1121,   353,   895,   360,   355,   348,   291,   640,   371,
     886,   893,   175,   288,   247,   347,   708,   353,   348,   353,
     890,   384,   391,   138,   357,   967,   250,   293,   980,   366,
     569,  1121,   962,   105,   115,   119,   122,   126,   131,   137,
     159,   162,   165,   170,   174,   193,   201,   206,   228,   231,
     237,   265,   290,   359,   576,   577,   578,   580,   762,   763,
     770,   771,   784,   792,   796,   972,   974,   975,   979,  1002,
    1003,  1004,  1005,  1006,  1007,  1008,  1009,  1013,  1020,  1021,
    1022,  1023,  1024,  1026,  1027,  1028,  1030,  1031,  1032,  1033,
    1034,  1035,  1121,   358,   977,   243,   371,   197,   204,  1092,
    1093,  1094,   165,  1098,  1090,   428,   493,   493,   408,   493,
     408,   408,   454,   493,   192,  1121,   708,   670,   671,  1121,
     348,   348,   347,   731,   890,   731,   733,   237,   688,  1121,
     347,   271,   663,   110,   734,   146,   200,   201,   237,   733,
     890,   348,   353,  1121,   347,   347,   569,  1121,  1121,   371,
     291,   659,   355,   366,   368,   620,   366,   635,  1121,  1077,
    1081,  1080,  1121,   206,   353,  1068,   285,  1121,   237,   110,
     232,   348,   721,   258,   366,  1137,   658,   734,   708,   347,
     348,   589,   371,  1106,   371,   392,   371,  1068,   371,   371,
     153,  1112,   191,   269,   312,   371,   618,   619,   266,   747,
    1121,   266,   797,   180,   821,  1126,   940,   940,   116,   208,
     944,   371,   347,   940,   355,   348,   949,   569,  1135,  1135,
    1134,   367,   367,  1121,   612,   569,   138,   766,   767,   921,
    1132,  1132,  1121,   608,   171,   348,   353,   347,   348,   347,
     768,   769,   827,   584,   569,   612,   371,   613,  1132,  1132,
     377,   379,  1129,   347,   347,   347,   796,   353,   168,   177,
     185,   230,   861,   342,   347,   355,  1144,   128,   240,   299,
     874,   875,   877,   348,   353,   355,   780,   880,   138,   921,
     569,   247,   255,   894,   288,   893,   886,   243,   734,  1121,
     191,   689,   690,   896,   970,   360,   966,   348,   353,   346,
     355,   181,   236,  1002,   263,   921,   968,  1014,  1015,  1121,
     579,   358,   982,   966,  1121,   976,  1121,  1121,   896,   969,
    1002,   358,   972,  1121,   358,   970,   579,   975,  1121,   969,
     358,  1001,  1121,   359,   358,   358,   358,   358,   358,   358,
     358,   358,   358,   358,   885,   357,   156,   995,   996,  1003,
     358,   966,   355,   967,   202,   347,   353,   232,   243,  1095,
     149,   961,  1100,   493,   493,   493,   408,  1139,  1140,   348,
     353,   212,   708,   734,   688,   688,   688,   348,   371,   258,
     694,   666,   667,   668,  1121,   347,   347,   348,   138,   201,
     348,   138,   348,   676,   569,   180,   704,   705,   921,   704,
    1121,   371,   371,   660,  1121,   258,   626,   353,   348,   212,
     367,   367,   116,  1121,   796,   366,   739,   138,   725,   368,
     348,   353,   348,   110,   317,   371,   711,   712,   272,   371,
    1107,   110,   371,  1077,   366,  1077,   371,  1068,   921,  1113,
    1114,  1121,   366,   620,   619,   371,   623,   624,   228,   348,
     973,   975,   825,   825,   921,   942,   943,   116,   371,   945,
     384,   391,   954,   208,   569,   348,   928,  1132,  1132,   348,
     353,  1135,  1135,   395,   781,   766,   796,   766,   353,   205,
     347,   793,   613,  1135,  1135,  1129,  1129,  1134,   304,   371,
     828,   936,  1121,  1121,   796,   348,   827,   210,   862,   862,
     862,   183,  1121,   858,   859,  1121,  1121,   296,   297,   829,
     830,   837,   116,   266,   896,   172,   818,   877,   371,   878,
     360,   783,  1121,  1121,   392,   886,   288,   894,  1121,   348,
     347,   371,   687,   890,   970,   352,   354,   366,   250,   293,
     569,  1121,   796,   237,   995,   969,   263,  1016,  1017,   150,
    1019,  1014,   355,   358,   115,   263,  1029,   180,   355,   175,
     176,  1145,   358,   246,  1012,   153,   358,   355,   358,   360,
     355,  1021,   358,   355,  1121,   358,   360,   263,   997,   998,
     999,   153,   358,   569,   972,  1121,   358,   978,   991,  1121,
     569,   993,   994,  1093,   110,  1096,   232,   241,   980,   493,
     671,  1121,   347,  1141,   348,   694,   694,   694,   360,   286,
     663,   348,   353,   317,   569,   664,   665,   711,   272,   348,
     348,   921,   676,   890,   672,   348,   353,   348,   166,   212,
     247,   371,   368,  1121,   206,  1121,   747,  1137,   921,   716,
     722,   723,   724,   352,   354,   366,   347,   796,  1121,   348,
     712,   110,   371,  1107,   797,   371,  1070,  1068,  1115,  1116,
     629,   630,   353,   895,   895,   353,   888,   276,   281,   366,
     371,   946,   948,   348,   208,   116,  1135,  1135,   767,   348,
     348,   769,   896,   138,   794,   795,   921,   895,  1132,  1132,
     347,   348,   355,   353,   348,   829,   827,   824,   348,   353,
     355,  1144,  1144,   347,   371,   838,   824,   896,   896,   116,
     173,   873,   878,   193,   875,   876,   921,   371,   894,   886,
     353,   347,   212,   691,   692,   181,   688,   366,   366,   348,
     346,   885,   371,   153,  1012,   968,  1018,  1019,  1016,  1002,
     153,  1121,  1002,   969,   358,   973,  1121,   314,   968,   972,
    1121,  1002,   150,   151,   306,  1010,  1011,   193,   972,   970,
    1121,   966,  1121,   361,   970,   289,  1000,  1001,   999,   355,
     286,   992,   355,   727,   348,   353,  1097,  1121,   263,  1099,
     317,   212,  1142,  1143,   224,   735,   203,   205,   269,   270,
     317,   695,   696,   697,   668,  1121,   317,   348,   353,   348,
     110,   348,   669,   180,   705,   180,   184,  1121,  1121,   366,
     367,   121,   191,   200,   201,   215,   253,   735,   724,   366,
     366,   348,   212,   702,   703,   347,   371,  1107,   371,  1070,
    1077,  1117,  1118,   353,   975,   874,   874,   943,   946,   274,
     366,   274,   275,   116,   921,   938,   939,   263,   785,   786,
     348,   353,   880,  1135,  1135,   949,   824,   569,  1121,   368,
     824,   824,   205,   824,   859,  1121,   829,   371,   831,   832,
     301,   347,   380,   399,   406,   863,   866,   870,   871,   921,
     896,   894,   646,   243,   649,   650,  1121,   348,   353,   371,
     695,   348,   348,   250,   358,   966,  1012,   153,   119,   358,
     995,   358,   180,   968,   344,  1021,   347,  1002,   969,   969,
     153,   358,   358,   358,   361,   358,  1012,  1012,   207,   569,
     116,   358,  1121,   358,   994,   347,  1121,  1121,   348,   353,
     584,   366,  1121,   697,  1121,   665,  1141,   347,   347,   347,
     371,   247,   355,   896,   253,   201,   184,   890,   348,   348,
    1121,   348,   353,   711,  1107,   371,   630,   818,   818,   106,
     371,   888,   111,   141,   348,   353,   200,   371,   787,   786,
     795,   348,   355,   348,   896,  1144,   824,   347,   165,   353,
     833,   347,   569,   839,   841,   347,   347,   347,   353,   266,
     872,  1121,   348,   353,   205,   692,   358,   119,   358,   153,
     973,   344,   968,   966,   348,   949,  1012,  1012,   174,  1001,
     727,   355,   896,   709,  1143,   730,   711,   672,   672,  1121,
    1121,   348,   890,   890,   688,   259,  1138,   703,   348,   873,
     873,   276,   371,   947,   948,   348,   939,   371,   371,   246,
     569,  1121,   824,   351,   921,   347,   569,   832,   176,   834,
     840,   841,   165,   864,   865,   921,   864,   347,   863,   867,
     868,   869,   921,   871,   399,   247,   650,  1121,   358,   966,
     358,   968,   316,  1025,   358,   348,  1010,  1010,   358,  1121,
     348,   205,   736,   348,   669,   669,   355,   688,   688,   694,
     138,   186,   347,   275,   274,   275,   178,   256,   788,   348,
     348,   569,   347,   348,   348,   353,   839,   348,   353,   348,
     348,   348,   353,   212,   317,   693,   358,  1025,   968,  1021,
     139,   178,   256,  1141,   353,   353,  1121,   694,   694,  1138,
     245,   704,   791,   237,   824,   824,   348,   352,   354,   835,
     836,   855,   856,   841,   176,   842,   865,   868,   869,  1121,
    1121,  1021,   966,   118,   227,   371,   737,   737,   737,   672,
     672,   212,   347,   348,   259,   779,   856,   856,   348,   353,
     824,   347,   348,   259,   966,   358,   371,   669,   669,  1121,
     704,  1138,   347,   895,   836,   347,   569,   843,   844,   845,
     847,   138,   186,   347,   358,   348,   348,   348,   766,   880,
     846,   847,   348,   353,   110,   245,   704,  1138,   348,   139,
     789,   348,   353,   844,   347,   848,   850,   851,   852,   853,
     854,   855,   347,   348,   895,   895,   847,   849,   850,   343,
     352,   354,   351,   356,   704,   880,   348,   353,   853,   854,
     854,   855,   855,   348,   850
};

#define yyerrok		(yyerrstatus = 0)
#define yyclearin	(yychar = YYEMPTY)
#define YYEMPTY		(-2)
#define YYEOF		0

#define YYACCEPT	goto yyacceptlab
#define YYABORT		goto yyabortlab
#define YYERROR		goto yyerrorlab


/* Like YYERROR except do call yyerror.  This remains here temporarily
   to ease the transition to the new meaning of YYERROR, for GCC.
   Once GCC version 2 has supplanted version 1, this can go.  */

#define YYFAIL		goto yyerrlab

#define YYRECOVERING()  (!!yyerrstatus)

#define YYBACKUP(Token, Value)					\
do								\
  if (yychar == YYEMPTY && yylen == 1)				\
    {								\
      yychar = (Token);						\
      yylval = (Value);						\
      yytoken = YYTRANSLATE (yychar);				\
      YYPOPSTACK (1);						\
      goto yybackup;						\
    }								\
  else								\
    {								\
      yyerror (YY_("syntax error: cannot back up")); \
      YYERROR;							\
    }								\
while (YYID (0))


#define YYTERROR	1
#define YYERRCODE	256


/* YYLLOC_DEFAULT -- Set CURRENT to span from RHS[1] to RHS[N].
   If N is 0, then set CURRENT to the empty location which ends
   the previous symbol: RHS[0] (always defined).  */

#define YYRHSLOC(Rhs, K) ((Rhs)[K])
#ifndef YYLLOC_DEFAULT
# define YYLLOC_DEFAULT(Current, Rhs, N)				\
    do									\
      if (YYID (N))                                                    \
	{								\
	  (Current).first_line   = YYRHSLOC (Rhs, 1).first_line;	\
	  (Current).first_column = YYRHSLOC (Rhs, 1).first_column;	\
	  (Current).last_line    = YYRHSLOC (Rhs, N).last_line;		\
	  (Current).last_column  = YYRHSLOC (Rhs, N).last_column;	\
	}								\
      else								\
	{								\
	  (Current).first_line   = (Current).last_line   =		\
	    YYRHSLOC (Rhs, 0).last_line;				\
	  (Current).first_column = (Current).last_column =		\
	    YYRHSLOC (Rhs, 0).last_column;				\
	}								\
    while (YYID (0))
#endif


/* YY_LOCATION_PRINT -- Print the location on the stream.
   This macro was not mandated originally: define only if we know
   we won't break user code: when these are the locations we know.  */

#ifndef YY_LOCATION_PRINT
# if YYLTYPE_IS_TRIVIAL
#  define YY_LOCATION_PRINT(File, Loc)			\
     fprintf (File, "%d.%d-%d.%d",			\
	      (Loc).first_line, (Loc).first_column,	\
	      (Loc).last_line,  (Loc).last_column)
# else
#  define YY_LOCATION_PRINT(File, Loc) ((void) 0)
# endif
#endif


/* YYLEX -- calling `yylex' with the right arguments.  */

#ifdef YYLEX_PARAM
# define YYLEX yylex (&yylval, YYLEX_PARAM)
#else
# define YYLEX yylex (&yylval)
#endif

/* Enable debugging if requested.  */
#if YYDEBUG

# ifndef YYFPRINTF
#  include <stdio.h> /* INFRINGES ON USER NAME SPACE */
#  define YYFPRINTF fprintf
# endif

# define YYDPRINTF(Args)			\
do {						\
  if (yydebug)					\
    YYFPRINTF Args;				\
} while (YYID (0))

# define YY_SYMBOL_PRINT(Title, Type, Value, Location)			  \
do {									  \
  if (yydebug)								  \
    {									  \
      YYFPRINTF (stderr, "%s ", Title);					  \
      yy_symbol_print (stderr,						  \
		  Type, Value); \
      YYFPRINTF (stderr, "\n");						  \
    }									  \
} while (YYID (0))


/*--------------------------------.
| Print this symbol on YYOUTPUT.  |
`--------------------------------*/

/*ARGSUSED*/
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_symbol_value_print (FILE *yyoutput, int yytype, YYSTYPE const * const yyvaluep)
#else
static void
yy_symbol_value_print (yyoutput, yytype, yyvaluep)
    FILE *yyoutput;
    int yytype;
    YYSTYPE const * const yyvaluep;
#endif
{
  if (!yyvaluep)
    return;
# ifdef YYPRINT
  if (yytype < YYNTOKENS)
    YYPRINT (yyoutput, yytoknum[yytype], *yyvaluep);
# else
  YYUSE (yyoutput);
# endif
  switch (yytype)
    {
      default:
	break;
    }
}


/*--------------------------------.
| Print this symbol on YYOUTPUT.  |
`--------------------------------*/

#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_symbol_print (FILE *yyoutput, int yytype, YYSTYPE const * const yyvaluep)
#else
static void
yy_symbol_print (yyoutput, yytype, yyvaluep)
    FILE *yyoutput;
    int yytype;
    YYSTYPE const * const yyvaluep;
#endif
{
  if (yytype < YYNTOKENS)
    YYFPRINTF (yyoutput, "token %s (", yytname[yytype]);
  else
    YYFPRINTF (yyoutput, "nterm %s (", yytname[yytype]);

  yy_symbol_value_print (yyoutput, yytype, yyvaluep);
  YYFPRINTF (yyoutput, ")");
}

/*------------------------------------------------------------------.
| yy_stack_print -- Print the state stack from its BOTTOM up to its |
| TOP (included).                                                   |
`------------------------------------------------------------------*/

#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_stack_print (yytype_int16 *yybottom, yytype_int16 *yytop)
#else
static void
yy_stack_print (yybottom, yytop)
    yytype_int16 *yybottom;
    yytype_int16 *yytop;
#endif
{
  YYFPRINTF (stderr, "Stack now");
  for (; yybottom <= yytop; yybottom++)
    {
      int yybot = *yybottom;
      YYFPRINTF (stderr, " %d", yybot);
    }
  YYFPRINTF (stderr, "\n");
}

# define YY_STACK_PRINT(Bottom, Top)				\
do {								\
  if (yydebug)							\
    yy_stack_print ((Bottom), (Top));				\
} while (YYID (0))


/*------------------------------------------------.
| Report that the YYRULE is going to be reduced.  |
`------------------------------------------------*/

#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_reduce_print (YYSTYPE *yyvsp, int yyrule)
#else
static void
yy_reduce_print (yyvsp, yyrule)
    YYSTYPE *yyvsp;
    int yyrule;
#endif
{
  int yynrhs = yyr2[yyrule];
  int yyi;
  unsigned long int yylno = yyrline[yyrule];
  YYFPRINTF (stderr, "Reducing stack by rule %d (line %lu):\n",
	     yyrule - 1, yylno);
  /* The symbols being reduced.  */
  for (yyi = 0; yyi < yynrhs; yyi++)
    {
      YYFPRINTF (stderr, "   $%d = ", yyi + 1);
      yy_symbol_print (stderr, yyrhs[yyprhs[yyrule] + yyi],
		       &(yyvsp[(yyi + 1) - (yynrhs)])
		       		       );
      YYFPRINTF (stderr, "\n");
    }
}

# define YY_REDUCE_PRINT(Rule)		\
do {					\
  if (yydebug)				\
    yy_reduce_print (yyvsp, Rule); \
} while (YYID (0))

/* Nonzero means print parse trace.  It is left uninitialized so that
   multiple parsers can coexist.  */
int yydebug;
#else /* !YYDEBUG */
# define YYDPRINTF(Args)
# define YY_SYMBOL_PRINT(Title, Type, Value, Location)
# define YY_STACK_PRINT(Bottom, Top)
# define YY_REDUCE_PRINT(Rule)
#endif /* !YYDEBUG */


/* YYINITDEPTH -- initial size of the parser's stacks.  */
#ifndef	YYINITDEPTH
# define YYINITDEPTH 200
#endif

/* YYMAXDEPTH -- maximum size the stacks can grow to (effective only
   if the built-in stack extension method is used).

   Do not make this value too large; the results are undefined if
   YYSTACK_ALLOC_MAXIMUM < YYSTACK_BYTES (YYMAXDEPTH)
   evaluated with infinite-precision integer arithmetic.  */

#ifndef YYMAXDEPTH
# define YYMAXDEPTH 10000
#endif



#if YYERROR_VERBOSE

# ifndef yystrlen
#  if defined __GLIBC__ && defined _STRING_H
#   define yystrlen strlen
#  else
/* Return the length of YYSTR.  */
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static YYSIZE_T
yystrlen (const char *yystr)
#else
static YYSIZE_T
yystrlen (yystr)
    const char *yystr;
#endif
{
  YYSIZE_T yylen;
  for (yylen = 0; yystr[yylen]; yylen++)
    continue;
  return yylen;
}
#  endif
# endif

# ifndef yystpcpy
#  if defined __GLIBC__ && defined _STRING_H && defined _GNU_SOURCE
#   define yystpcpy stpcpy
#  else
/* Copy YYSRC to YYDEST, returning the address of the terminating '\0' in
   YYDEST.  */
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static char *
yystpcpy (char *yydest, const char *yysrc)
#else
static char *
yystpcpy (yydest, yysrc)
    char *yydest;
    const char *yysrc;
#endif
{
  char *yyd = yydest;
  const char *yys = yysrc;

  while ((*yyd++ = *yys++) != '\0')
    continue;

  return yyd - 1;
}
#  endif
# endif

# ifndef yytnamerr
/* Copy to YYRES the contents of YYSTR after stripping away unnecessary
   quotes and backslashes, so that it's suitable for yyerror.  The
   heuristic is that double-quoting is unnecessary unless the string
   contains an apostrophe, a comma, or backslash (other than
   backslash-backslash).  YYSTR is taken from yytname.  If YYRES is
   null, do not copy; instead, return the length of what the result
   would have been.  */
static YYSIZE_T
yytnamerr (char *yyres, const char *yystr)
{
  if (*yystr == '"')
    {
      YYSIZE_T yyn = 0;
      char const *yyp = yystr;

      for (;;)
	switch (*++yyp)
	  {
	  case '\'':
	  case ',':
	    goto do_not_strip_quotes;

	  case '\\':
	    if (*++yyp != '\\')
	      goto do_not_strip_quotes;
	    /* Fall through.  */
	  default:
	    if (yyres)
	      yyres[yyn] = *yyp;
	    yyn++;
	    break;

	  case '"':
	    if (yyres)
	      yyres[yyn] = '\0';
	    return yyn;
	  }
    do_not_strip_quotes: ;
    }

  if (! yyres)
    return yystrlen (yystr);

  return yystpcpy (yyres, yystr) - yyres;
}
# endif

/* Copy into YYRESULT an error message about the unexpected token
   YYCHAR while in state YYSTATE.  Return the number of bytes copied,
   including the terminating null byte.  If YYRESULT is null, do not
   copy anything; just return the number of bytes that would be
   copied.  As a special case, return 0 if an ordinary "syntax error"
   message will do.  Return YYSIZE_MAXIMUM if overflow occurs during
   size calculation.  */
static YYSIZE_T
yysyntax_error (char *yyresult, int yystate, int yychar)
{
  int yyn = yypact[yystate];

  if (! (YYPACT_NINF < yyn && yyn <= YYLAST))
    return 0;
  else
    {
      int yytype = YYTRANSLATE (yychar);
      YYSIZE_T yysize0 = yytnamerr (0, yytname[yytype]);
      YYSIZE_T yysize = yysize0;
      YYSIZE_T yysize1;
      int yysize_overflow = 0;
      enum { YYERROR_VERBOSE_ARGS_MAXIMUM = 5 };
      char const *yyarg[YYERROR_VERBOSE_ARGS_MAXIMUM];
      int yyx;

# if 0
      /* This is so xgettext sees the translatable formats that are
	 constructed on the fly.  */
      YY_("syntax error, unexpected %s");
      YY_("syntax error, unexpected %s, expecting %s");
      YY_("syntax error, unexpected %s, expecting %s or %s");
      YY_("syntax error, unexpected %s, expecting %s or %s or %s");
      YY_("syntax error, unexpected %s, expecting %s or %s or %s or %s");
# endif
      char *yyfmt;
      char const *yyf;
      static char const yyunexpected[] = "syntax error, unexpected %s";
      static char const yyexpecting[] = ", expecting %s";
      static char const yyor[] = " or %s";
      char yyformat[sizeof yyunexpected
		    + sizeof yyexpecting - 1
		    + ((YYERROR_VERBOSE_ARGS_MAXIMUM - 2)
		       * (sizeof yyor - 1))];
      char const *yyprefix = yyexpecting;

      /* Start YYX at -YYN if negative to avoid negative indexes in
	 YYCHECK.  */
      int yyxbegin = yyn < 0 ? -yyn : 0;

      /* Stay within bounds of both yycheck and yytname.  */
      int yychecklim = YYLAST - yyn + 1;
      int yyxend = yychecklim < YYNTOKENS ? yychecklim : YYNTOKENS;
      int yycount = 1;

      yyarg[0] = yytname[yytype];
      yyfmt = yystpcpy (yyformat, yyunexpected);

      for (yyx = yyxbegin; yyx < yyxend; ++yyx)
	if (yycheck[yyx + yyn] == yyx && yyx != YYTERROR)
	  {
	    if (yycount == YYERROR_VERBOSE_ARGS_MAXIMUM)
	      {
		yycount = 1;
		yysize = yysize0;
		yyformat[sizeof yyunexpected - 1] = '\0';
		break;
	      }
	    yyarg[yycount++] = yytname[yyx];
	    yysize1 = yysize + yytnamerr (0, yytname[yyx]);
	    yysize_overflow |= (yysize1 < yysize);
	    yysize = yysize1;
	    yyfmt = yystpcpy (yyfmt, yyprefix);
	    yyprefix = yyor;
	  }

      yyf = YY_(yyformat);
      yysize1 = yysize + yystrlen (yyf);
      yysize_overflow |= (yysize1 < yysize);
      yysize = yysize1;

      if (yysize_overflow)
	return YYSIZE_MAXIMUM;

      if (yyresult)
	{
	  /* Avoid sprintf, as that infringes on the user's name space.
	     Don't have undefined behavior even if the translation
	     produced a string with the wrong number of "%s"s.  */
	  char *yyp = yyresult;
	  int yyi = 0;
	  while ((*yyp = *yyf) != '\0')
	    {
	      if (*yyp == '%' && yyf[1] == 's' && yyi < yycount)
		{
		  yyp += yytnamerr (yyp, yyarg[yyi++]);
		  yyf += 2;
		}
	      else
		{
		  yyp++;
		  yyf++;
		}
	    }
	}
      return yysize;
    }
}
#endif /* YYERROR_VERBOSE */


/*-----------------------------------------------.
| Release the memory associated to this symbol.  |
`-----------------------------------------------*/

/*ARGSUSED*/
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yydestruct (const char *yymsg, int yytype, YYSTYPE *yyvaluep)
#else
static void
yydestruct (yymsg, yytype, yyvaluep)
    const char *yymsg;
    int yytype;
    YYSTYPE *yyvaluep;
#endif
{
  YYUSE (yyvaluep);

  if (!yymsg)
    yymsg = "Deleting";
  YY_SYMBOL_PRINT (yymsg, yytype, yyvaluep, yylocationp);

  switch (yytype)
    {

      default:
	break;
    }
}

/* Prevent warnings from -Wmissing-prototypes.  */
#ifdef YYPARSE_PARAM
#if defined __STDC__ || defined __cplusplus
int yyparse (void *YYPARSE_PARAM);
#else
int yyparse ();
#endif
#else /* ! YYPARSE_PARAM */
#if defined __STDC__ || defined __cplusplus
int yyparse (void);
#else
int yyparse ();
#endif
#endif /* ! YYPARSE_PARAM */





/*-------------------------.
| yyparse or yypush_parse.  |
`-------------------------*/

#ifdef YYPARSE_PARAM
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
int
yyparse (void *YYPARSE_PARAM)
#else
int
yyparse (YYPARSE_PARAM)
    void *YYPARSE_PARAM;
#endif
#else /* ! YYPARSE_PARAM */
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
int
yyparse (void)
#else
int
yyparse ()

#endif
#endif
{
/* The lookahead symbol.  */
int yychar;

/* The semantic value of the lookahead symbol.  */
YYSTYPE yylval;

    /* Number of syntax errors so far.  */
    int yynerrs;

    int yystate;
    /* Number of tokens to shift before error messages enabled.  */
    int yyerrstatus;

    /* The stacks and their tools:
       `yyss': related to states.
       `yyvs': related to semantic values.

       Refer to the stacks thru separate pointers, to allow yyoverflow
       to reallocate them elsewhere.  */

    /* The state stack.  */
    yytype_int16 yyssa[YYINITDEPTH];
    yytype_int16 *yyss;
    yytype_int16 *yyssp;

    /* The semantic value stack.  */
    YYSTYPE yyvsa[YYINITDEPTH];
    YYSTYPE *yyvs;
    YYSTYPE *yyvsp;

    YYSIZE_T yystacksize;

  int yyn;
  int yyresult;
  /* Lookahead token as an internal (translated) token number.  */
  int yytoken;
  /* The variables used to return semantic value and location from the
     action routines.  */
  YYSTYPE yyval;

#if YYERROR_VERBOSE
  /* Buffer for error messages, and its allocated size.  */
  char yymsgbuf[128];
  char *yymsg = yymsgbuf;
  YYSIZE_T yymsg_alloc = sizeof yymsgbuf;
#endif

#define YYPOPSTACK(N)   (yyvsp -= (N), yyssp -= (N))

  /* The number of symbols on the RHS of the reduced rule.
     Keep to zero when no symbol should be popped.  */
  int yylen = 0;

  yytoken = 0;
  yyss = yyssa;
  yyvs = yyvsa;
  yystacksize = YYINITDEPTH;

  YYDPRINTF ((stderr, "Starting parse\n"));

  yystate = 0;
  yyerrstatus = 0;
  yynerrs = 0;
  yychar = YYEMPTY; /* Cause a token to be read.  */

  /* Initialize stack pointers.
     Waste one element of value and location stack
     so that they stay on the same level as the state stack.
     The wasted elements are never initialized.  */
  yyssp = yyss;
  yyvsp = yyvs;

/* User initialization code.  */

/* Line 1242 of yacc.c  */
#line 28 "ulpCompy.y"
{
    idlOS::memset(&yyval, 0, sizeof(yyval));

    /* BUG-28061 : preprocessing을마치면 marco table을 초기화하고, *
     *             ulpComp 에서 재구축한다.                       */
    switch ( gUlpProgOption.mOptParseInfo )
    {
        // 옵션 -parse none 에 해당하는 상태.
        case PARSE_NONE :
            gUlpCOMPStartCond = CP_ST_NONE;
            break;
        // 옵션 -parse partial 에 해당하는 상태.
        case PARSE_PARTIAL :
            gUlpCOMPStartCond = CP_ST_PARTIAL;
            break;
        // 옵션 -parse full 에 해당하는 상태.
        case PARSE_FULL :
            gUlpCOMPStartCond = CP_ST_C;
            break;
        default :
            break;
    }
}

/* Line 1242 of yacc.c  */
#line 7915 "ulpCompy.cpp"

  goto yysetstate;

/*------------------------------------------------------------.
| yynewstate -- Push a new state, which is found in yystate.  |
`------------------------------------------------------------*/
 yynewstate:
  /* In all cases, when you get here, the value and location stacks
     have just been pushed.  So pushing a state here evens the stacks.  */
  yyssp++;

 yysetstate:
  *yyssp = yystate;

  if (yyss + yystacksize - 1 <= yyssp)
    {
      /* Get the current used size of the three stacks, in elements.  */
      YYSIZE_T yysize = yyssp - yyss + 1;

#ifdef yyoverflow
      {
	/* Give user a chance to reallocate the stack.  Use copies of
	   these so that the &'s don't force the real ones into
	   memory.  */
	YYSTYPE *yyvs1 = yyvs;
	yytype_int16 *yyss1 = yyss;

	/* Each stack pointer address is followed by the size of the
	   data in use in that stack, in bytes.  This used to be a
	   conditional around just the two extra args, but that might
	   be undefined if yyoverflow is a macro.  */
	yyoverflow (YY_("memory exhausted"),
		    &yyss1, yysize * sizeof (*yyssp),
		    &yyvs1, yysize * sizeof (*yyvsp),
		    &yystacksize);

	yyss = yyss1;
	yyvs = yyvs1;
      }
#else /* no yyoverflow */
# ifndef YYSTACK_RELOCATE
      goto yyexhaustedlab;
# else
      /* Extend the stack our own way.  */
      if (YYMAXDEPTH <= yystacksize)
	goto yyexhaustedlab;
      yystacksize *= 2;
      if (YYMAXDEPTH < yystacksize)
	yystacksize = YYMAXDEPTH;

      {
	yytype_int16 *yyss1 = yyss;
	union yyalloc *yyptr =
	  (union yyalloc *) YYSTACK_ALLOC (YYSTACK_BYTES (yystacksize));
	if (! yyptr)
	  goto yyexhaustedlab;
	YYSTACK_RELOCATE (yyss_alloc, yyss);
	YYSTACK_RELOCATE (yyvs_alloc, yyvs);
#  undef YYSTACK_RELOCATE
	if (yyss1 != yyssa)
	  YYSTACK_FREE (yyss1);
      }
# endif
#endif /* no yyoverflow */

      yyssp = yyss + yysize - 1;
      yyvsp = yyvs + yysize - 1;

      YYDPRINTF ((stderr, "Stack size increased to %lu\n",
		  (unsigned long int) yystacksize));

      if (yyss + yystacksize - 1 <= yyssp)
	YYABORT;
    }

  YYDPRINTF ((stderr, "Entering state %d\n", yystate));

  if (yystate == YYFINAL)
    YYACCEPT;

  goto yybackup;

/*-----------.
| yybackup.  |
`-----------*/
yybackup:

  /* Do appropriate processing given the current state.  Read a
     lookahead token if we need one and don't already have one.  */

  /* First try to decide what to do without reference to lookahead token.  */
  yyn = yypact[yystate];
  if (yyn == YYPACT_NINF)
    goto yydefault;

  /* Not known => get a lookahead token if don't already have one.  */

  /* YYCHAR is either YYEMPTY or YYEOF or a valid lookahead symbol.  */
  if (yychar == YYEMPTY)
    {
      YYDPRINTF ((stderr, "Reading a token: "));
      yychar = YYLEX;
    }

  if (yychar <= YYEOF)
    {
      yychar = yytoken = YYEOF;
      YYDPRINTF ((stderr, "Now at end of input.\n"));
    }
  else
    {
      yytoken = YYTRANSLATE (yychar);
      YY_SYMBOL_PRINT ("Next token is", yytoken, &yylval, &yylloc);
    }

  /* If the proper action on seeing token YYTOKEN is to reduce or to
     detect an error, take that action.  */
  yyn += yytoken;
  if (yyn < 0 || YYLAST < yyn || yycheck[yyn] != yytoken)
    goto yydefault;
  yyn = yytable[yyn];
  if (yyn <= 0)
    {
      if (yyn == 0 || yyn == YYTABLE_NINF)
	goto yyerrlab;
      yyn = -yyn;
      goto yyreduce;
    }

  /* Count tokens shifted since error; after three, turn off error
     status.  */
  if (yyerrstatus)
    yyerrstatus--;

  /* Shift the lookahead token.  */
  YY_SYMBOL_PRINT ("Shifting", yytoken, &yylval, &yylloc);

  /* Discard the shifted token.  */
  yychar = YYEMPTY;

  yystate = yyn;
  *++yyvsp = yylval;

  goto yynewstate;


/*-----------------------------------------------------------.
| yydefault -- do the default action for the current state.  |
`-----------------------------------------------------------*/
yydefault:
  yyn = yydefact[yystate];
  if (yyn == 0)
    goto yyerrlab;
  goto yyreduce;


/*-----------------------------.
| yyreduce -- Do a reduction.  |
`-----------------------------*/
yyreduce:
  /* yyn is the number of a rule to reduce with.  */
  yylen = yyr2[yyn];

  /* If YYLEN is nonzero, implement the default value of the action:
     `$$ = $1'.

     Otherwise, the following line sets YYVAL to garbage.
     This behavior is undocumented and Bison
     users should not rely upon it.  Assigning to YYVAL
     unconditionally makes the parser a bit smaller, and it avoids a
     GCC warning that YYVAL may be used uninitialized.  */
  yyval = yyvsp[1-yylen];


  YY_REDUCE_PRINT (yyn);
  switch (yyn)
    {
        case 7:

/* Line 1455 of yacc.c  */
#line 571 "ulpCompy.y"
    {
        YYACCEPT;
    ;}
    break;

  case 84:

/* Line 1455 of yacc.c  */
#line 727 "ulpCompy.y"
    {
        /* BUG-28118 : system 헤더파일들도 파싱돼야함.            *
         * 2th. problem : 빈구조체 선언이 허용안됨. ex) struct A; */
        // <type> ; 이 올수 있다. ex> int; char; struct A; ...
        gUlpParseInfo.ulpInitHostInfo();
    ;}
    break;

  case 85:

/* Line 1455 of yacc.c  */
#line 734 "ulpCompy.y"
    {
        iduListNode *sIterator = NULL;
        iduListNode *sNextNode = NULL;
        ulpSymTElement *sSymNode;

        if( gUlpParseInfo.mFuncDecl == ID_TRUE )
        {
            gUlpScopeT.ulpSDelScope( gUlpCurDepth + 1 );
        }

        /* BUG-35518 Shared pointer should be supported in APRE */
        /* convert the sentence for shared pointer */
        if ( gUlpParseInfo.mIsSharedPtr == ID_TRUE)
        {
            WRITESTR2BUFCOMP ( (SChar *)" */\n" );
            IDU_LIST_ITERATE_SAFE(&(gUlpParseInfo.mSharedPtrVarList),
                                  sIterator, sNextNode )
            {
                sSymNode = (ulpSymTElement *)sIterator->mObj;
                if ( gDontPrint2file != ID_TRUE )
                {
                    gUlpCodeGen.ulpGenSharedPtr( sSymNode );
                }
                IDU_LIST_REMOVE(sIterator);
                idlOS::free(sIterator);
            }
            IDU_LIST_INIT( &(gUlpParseInfo.mSharedPtrVarList) );
        }

        // varchar 선언의 경우 해당 code를 주석처리 한후,
        // struct { char arr[...]; SQLLEN len; } 으로의 변환이 필요함.
        if ( (gUlpParseInfo.mHostValInfo[gUlpParseInfo.mStructDeclDepth]->mType == H_VARCHAR) ||
             (gUlpParseInfo.mHostValInfo[gUlpParseInfo.mStructDeclDepth]->mType == H_NVARCHAR) )
        {
            if ( gUlpParseInfo.mFuncDecl != ID_TRUE )
            {
                WRITESTR2BUFCOMP ( (SChar *)" */\n" );

                /* BUG-26375 valgrind bug */
                IDU_LIST_ITERATE_SAFE(&(gUlpParseInfo.mVarcharVarList),
                                      sIterator, sNextNode )
                {
                    sSymNode = (ulpSymTElement *)sIterator->mObj;
                    if ( gDontPrint2file != ID_TRUE )
                    {
                        gUlpCodeGen.ulpGenVarchar( sSymNode );
                    }
                    IDU_LIST_REMOVE(sIterator);
                    idlOS::free(sIterator);
                }
                IDU_LIST_INIT( &(gUlpParseInfo.mVarcharVarList) );
                gUlpParseInfo.mVarcharDecl = ID_FALSE;
            }
        }
   


        gUlpParseInfo.mFuncDecl = ID_FALSE;
        gUlpParseInfo.mHostValInfo4Typedef = NULL;
        // 하나의 선언구문이 처리되면 따로 저장하고 있던 호스트변수정보를 초기화함.
        gUlpParseInfo.ulpInitHostInfo();
    ;}
    break;

  case 90:

/* Line 1455 of yacc.c  */
#line 807 "ulpCompy.y"
    {
        SChar *sVarName;
        ulpSymTNode *sSymNode;
        iduListNode *sIterator = NULL;
        iduListNode *sVarcharListNode = NULL;
        /* BUG-35518 Shared pointer should be supported in APRE */
        iduListNode *sSharedPtrVarListNode = NULL;

        if( gUlpParseInfo.mFuncDecl != ID_TRUE )
        {

            if( gUlpParseInfo.mHostValInfo[gUlpParseInfo.mStructDeclDepth]->mIsTypedef != ID_TRUE )
            {   // typedef 정의가 아닐경우
                /* BUG-28118 : system 헤더파일들도 파싱돼야함.            *
                 * 5th. problem : 정의되지 않은 구조체 포인터 변수 선언안됨. *
                 * 8th. problem : can't resolve extern variable type at declaring section. */
                if ( (gUlpParseInfo.mHostValInfo[gUlpParseInfo.mStructDeclDepth]->mIsstruct  == ID_TRUE) &&
                     (gUlpParseInfo.mHostValInfo[gUlpParseInfo.mStructDeclDepth]->mStructName[0] != '\0') &&
                     (gUlpParseInfo.mHostValInfo[gUlpParseInfo.mStructDeclDepth]->mStructLink == NULL) )
                {   // struct 변수 선언의 경우.
                    // structure 변수 선언의경우 extern or pointer가 아니라면 struct table에서
                    // 해당 struct tag가 존재하는지 검사하며, extern or pointer일 경우에는 검사하지 않고
                    // 나중에 해당 변수를 사용할때 검사한다.
                    if ( (gUlpParseInfo.mHostValInfo[gUlpParseInfo.mStructDeclDepth]->mPointer  == 0) &&
                         (gUlpParseInfo.mHostValInfo[gUlpParseInfo.mStructDeclDepth]->mIsExtern == ID_FALSE) )
                    {   // it's not a pointer of struct and extern.
                        gUlpParseInfo.mStructPtr = gUlpStructT.ulpStructLookupAll(
                                            gUlpParseInfo.mHostValInfo[gUlpParseInfo.mStructDeclDepth]->mStructName,
                                            gUlpCurDepth );
                        if ( gUlpParseInfo.mStructPtr == NULL )
                        {
                            // error 처리

                            ulpSetErrorCode( &gUlpParseInfo.mErrorMgr,
                                             ulpERR_ABORT_COMP_C_Unknown_Structname_Error,
                                             gUlpParseInfo.mHostValInfo[gUlpParseInfo.mStructDeclDepth]->mStructName );
                            gUlpCOMPErrCode = ulpGetErrorSTATE( &gUlpParseInfo.mErrorMgr );
                            COMPerror( ulpGetErrorMSG(&gUlpParseInfo.mErrorMgr) );
                        }
                        else
                        {
                            gUlpParseInfo.mHostValInfo[gUlpParseInfo.mStructDeclDepth]->mStructLink
                                    = gUlpParseInfo.mStructPtr;
                        }
                    }
                    else
                    {   // it's a point or extern of struct
                        // do nothing
                    }
                }
            }
            else
            {
                if ( (gUlpParseInfo.mHostValInfo[gUlpParseInfo.mStructDeclDepth]->mIsstruct   == ID_TRUE) &&
                     (gUlpParseInfo.mHostValInfo[gUlpParseInfo.mStructDeclDepth]->mStructLink == NULL) )
                {   // structure 를 typedef 정의할 경우.
                    if (gUlpParseInfo.mHostValInfo[gUlpParseInfo.mStructDeclDepth]->mStructName[0] == '\0')
                    {   // no tag structure 를 typedef 정의할 경우.
                        // ex) typedef struct { ... } A;
                        gUlpParseInfo.mHostValInfo[gUlpParseInfo.mStructDeclDepth]->mStructLink
                                = gUlpParseInfo.mStructPtr;
                    }
                }
            }

            // char, varchar 변수의경우 -nchar_var 커맨드option에 포함된 변수인지 확인함.
            if ( (gUlpParseInfo.mHostValInfo[gUlpParseInfo.mStructDeclDepth]->mType == H_CHAR) ||
                 (gUlpParseInfo.mHostValInfo[gUlpParseInfo.mStructDeclDepth]->mType == H_VARCHAR) )
            {
                IDU_LIST_ITERATE(&gUlpProgOption.mNcharVarNameList, sIterator)
                {
                    sVarName = (SChar* )sIterator->mObj;
                    if ( idlOS::strcmp( sVarName,
                         gUlpParseInfo.mHostValInfo[gUlpParseInfo.mStructDeclDepth]->mName )
                         == 0 )
                    {
                        if ( gUlpParseInfo.mHostValInfo[gUlpParseInfo.mStructDeclDepth]->mType == H_CHAR )
                        {
                            gUlpParseInfo.mHostValInfo[gUlpParseInfo.mStructDeclDepth]->mType = H_NCHAR;
                        }
                        else
                        {
                            gUlpParseInfo.mHostValInfo[gUlpParseInfo.mStructDeclDepth]->mType = H_NVARCHAR;
                        }
                    }
                }
            }

            // scope table에 해당 symbol node를 추가한다.
            if( (sSymNode = gUlpScopeT.ulpSAdd ( gUlpParseInfo.mHostValInfo[gUlpParseInfo.mStructDeclDepth],
                                                 gUlpCurDepth ))
                == NULL )
            {
                // error 처리

                ulpSetErrorCode( &gUlpParseInfo.mErrorMgr,
                                ulpERR_ABORT_COMP_C_Add_Symbol_Error,
                                gUlpParseInfo.mHostValInfo[gUlpParseInfo.mStructDeclDepth]->mName );
                gUlpCOMPErrCode = ulpGetErrorSTATE( &gUlpParseInfo.mErrorMgr );
                COMPerror( ulpGetErrorMSG(&gUlpParseInfo.mErrorMgr) );
            }

            //varchar type의 경우, 나중 코드 변환을 위해 list에 따로 저장한다.
            if ( (gUlpParseInfo.mHostValInfo[gUlpParseInfo.mStructDeclDepth]->mType == H_VARCHAR) ||
                 (gUlpParseInfo.mHostValInfo[gUlpParseInfo.mStructDeclDepth]->mType == H_NVARCHAR)
               )
            {
                sVarcharListNode =
                    (iduListNode*)idlOS::malloc(ID_SIZEOF(iduListNode));
                if (sVarcharListNode == NULL)
                {
                    ulpSetErrorCode(&gUlpParseInfo.mErrorMgr, ulpERR_ABORT_Memory_Alloc_Error);
                    gUlpCOMPErrCode = ulpGetErrorSTATE(&gUlpParseInfo.mErrorMgr);
                    COMPerror(ulpGetErrorMSG(&gUlpParseInfo.mErrorMgr));
                }
                else
                {
                    IDU_LIST_INIT_OBJ(sVarcharListNode, &(sSymNode->mElement));
                    IDU_LIST_ADD_LAST(&(gUlpParseInfo.mVarcharVarList), sVarcharListNode);
                }
            }

            /* BUG-35518 Shared pointer should be supported in APRE
             * Store tokens for shared pointer to convert */

            if ( gUlpParseInfo.mIsSharedPtr == ID_TRUE )
            {
                sSharedPtrVarListNode = (iduListNode*)idlOS::malloc(ID_SIZEOF(iduListNode));
                if (sSharedPtrVarListNode == NULL)
                {
                    ulpSetErrorCode(&gUlpParseInfo.mErrorMgr, ulpERR_ABORT_Memory_Alloc_Error);
                    gUlpCOMPErrCode = ulpGetErrorSTATE(&gUlpParseInfo.mErrorMgr);
                    COMPerror(ulpGetErrorMSG(&gUlpParseInfo.mErrorMgr));
                }
                else
                {
                    IDU_LIST_INIT_OBJ(sSharedPtrVarListNode, &(sSymNode->mElement));
                    IDU_LIST_ADD_LAST(&(gUlpParseInfo.mSharedPtrVarList), sSharedPtrVarListNode);
                }

            }

        }
    ;}
    break;

  case 91:

/* Line 1455 of yacc.c  */
#line 952 "ulpCompy.y"
    {
        SChar *sVarName;
        ulpSymTNode *sSymNode;
        iduListNode *sIterator = NULL;
        iduListNode *sVarcharListNode = NULL;

        if( gUlpParseInfo.mFuncDecl != ID_TRUE )
        {

            if( gUlpParseInfo.mHostValInfo[gUlpParseInfo.mStructDeclDepth]->mIsTypedef != ID_TRUE )
            {   // typedef 정의가 아닐경우

                /* BUG-28118 : system 헤더파일들도 파싱돼야함.            *
                 * 5th. problem : 정의되지 않은 구조체 포인터 변수 선언안됨. *
                 * 8th. problem : can't resolve extern variable type at declaring section. */
                if ( (gUlpParseInfo.mHostValInfo[gUlpParseInfo.mStructDeclDepth]->mIsstruct  == ID_TRUE) &&
                     (gUlpParseInfo.mHostValInfo[gUlpParseInfo.mStructDeclDepth]->mStructName[0] != '\0') &&
                     (gUlpParseInfo.mHostValInfo[gUlpParseInfo.mStructDeclDepth]->mStructLink == NULL) )
                {   // struct 변수 선언의 경우.
                    // structure 변수 선언의경우 pointer가 아니라면 struct table에서
                    // 해당 struct tag가 존재하는지 검사하며, pointer일 경우에는 검사하지 않고
                    // 나중에 해당 변수를 사용할때 검사한다.
                    if ( (gUlpParseInfo.mHostValInfo[gUlpParseInfo.mStructDeclDepth]->mPointer  == 0) &&
                         (gUlpParseInfo.mHostValInfo[gUlpParseInfo.mStructDeclDepth]->mIsExtern == ID_FALSE) )
                    {   // it's not a pointer of struct and extern.

                        gUlpParseInfo.mStructPtr = gUlpStructT.ulpStructLookupAll(
                                            gUlpParseInfo.mHostValInfo[gUlpParseInfo.mStructDeclDepth]->mStructName,
                                            gUlpCurDepth );
                        if ( gUlpParseInfo.mStructPtr == NULL )
                        {
                            // error 처리

                            ulpSetErrorCode( &gUlpParseInfo.mErrorMgr,
                                             ulpERR_ABORT_COMP_C_Unknown_Structname_Error,
                                             gUlpParseInfo.mHostValInfo[gUlpParseInfo.mStructDeclDepth]->mStructName );
                            gUlpCOMPErrCode = ulpGetErrorSTATE( &gUlpParseInfo.mErrorMgr );
                            COMPerror( ulpGetErrorMSG(&gUlpParseInfo.mErrorMgr) );
                        }
                        else
                        {
                            gUlpParseInfo.mHostValInfo[gUlpParseInfo.mStructDeclDepth]->mStructLink
                                    = gUlpParseInfo.mStructPtr;
                        }
                    }
                    else
                    {   // it's a point of struct
                        // do nothing
                    }
                }
            }
            else
            {
                // no tag structure 를 typedef 할경우.
                if ( (gUlpParseInfo.mHostValInfo[gUlpParseInfo.mStructDeclDepth]->mIsstruct   == ID_TRUE) &&
                     (gUlpParseInfo.mHostValInfo[gUlpParseInfo.mStructDeclDepth]->mStructLink == NULL) )
                {   // structure 를 typedef 정의할 경우.
                    if (gUlpParseInfo.mHostValInfo[gUlpParseInfo.mStructDeclDepth]->mStructName[0] == '\0')
                    {   // no tag structure 를 typedef 정의할 경우.
                        // ex) typedef struct { ... } A;
                        gUlpParseInfo.mHostValInfo[gUlpParseInfo.mStructDeclDepth]->mStructLink = gUlpParseInfo.mStructPtr;
                    }
                }
            }

            if ( (gUlpParseInfo.mHostValInfo[gUlpParseInfo.mStructDeclDepth]->mType == H_CHAR) ||
                 (gUlpParseInfo.mHostValInfo[gUlpParseInfo.mStructDeclDepth]->mType == H_VARCHAR) )
            {
                IDU_LIST_ITERATE(&gUlpProgOption.mNcharVarNameList, sIterator)
                {
                    sVarName = (SChar* )sIterator->mObj;
                    if ( idlOS::strcmp( sVarName, gUlpParseInfo.mHostValInfo[gUlpParseInfo.mStructDeclDepth]->mName )
                         == 0 )
                    {
                        if ( gUlpParseInfo.mHostValInfo[gUlpParseInfo.mStructDeclDepth]->mType == H_CHAR )
                        {
                            gUlpParseInfo.mHostValInfo[gUlpParseInfo.mStructDeclDepth]->mType = H_NCHAR;
                        }
                        else
                        {
                            gUlpParseInfo.mHostValInfo[gUlpParseInfo.mStructDeclDepth]->mType = H_NVARCHAR;
                        }
                    }
                }
            }

            if( (sSymNode = gUlpScopeT.ulpSAdd ( gUlpParseInfo.mHostValInfo[gUlpParseInfo.mStructDeclDepth],
                                                 gUlpCurDepth ))
                == NULL )
            {
                // error 처리

                ulpSetErrorCode( &gUlpParseInfo.mErrorMgr,
                                ulpERR_ABORT_COMP_C_Add_Symbol_Error,
                                gUlpParseInfo.mHostValInfo[gUlpParseInfo.mStructDeclDepth]->mName );
                gUlpCOMPErrCode = ulpGetErrorSTATE( &gUlpParseInfo.mErrorMgr );
                COMPerror( ulpGetErrorMSG(&gUlpParseInfo.mErrorMgr) );
            }

            if ( (gUlpParseInfo.mHostValInfo[gUlpParseInfo.mStructDeclDepth]->mType == H_VARCHAR) ||
                 (gUlpParseInfo.mHostValInfo[gUlpParseInfo.mStructDeclDepth]->mType == H_NVARCHAR) )
            {
                sVarcharListNode =
                    (iduListNode*)idlOS::malloc(ID_SIZEOF(iduListNode));
                if (sVarcharListNode == NULL)
                {
                    ulpSetErrorCode(&gUlpParseInfo.mErrorMgr, ulpERR_ABORT_Memory_Alloc_Error);
                    gUlpCOMPErrCode = ulpGetErrorSTATE(&gUlpParseInfo.mErrorMgr);
                    COMPerror(ulpGetErrorMSG(&gUlpParseInfo.mErrorMgr));
                }
                else
                {
                    IDU_LIST_INIT_OBJ(sVarcharListNode, &(sSymNode->mElement));
                    IDU_LIST_ADD_LAST(&(gUlpParseInfo.mVarcharVarList), sVarcharListNode);
                }
            }
        }
    ;}
    break;

  case 92:

/* Line 1455 of yacc.c  */
#line 1074 "ulpCompy.y"
    {
        // , 를 사용한 동일 type을 여러개 선언할 경우 필요한 초기화.
        gUlpParseInfo.mSaveId = ID_TRUE;
        if ( gUlpParseInfo.mHostValInfo4Typedef != NULL )
        {
            if ( gUlpParseInfo.mHostValInfo4Typedef->mArraySize[0] != '\0' )
            {
                idlOS::strncpy( gUlpParseInfo.mHostValInfo[gUlpParseInfo.mStructDeclDepth]->mArraySize,
                                gUlpParseInfo.mHostValInfo4Typedef->mArraySize,
                                MAX_NUMBER_LEN - 1 );
            }
            else
            {
                gUlpParseInfo.mHostValInfo[gUlpParseInfo.mStructDeclDepth]->mArraySize[0]   = '\0';
            }

            if ( gUlpParseInfo.mHostValInfo4Typedef->mArraySize2[0] != '\0' )
            {
                idlOS::strncpy( gUlpParseInfo.mHostValInfo[gUlpParseInfo.mStructDeclDepth]->mArraySize2,
                                gUlpParseInfo.mHostValInfo4Typedef->mArraySize2,
                                MAX_NUMBER_LEN - 1 );
            }
            else
            {
                gUlpParseInfo.mHostValInfo[gUlpParseInfo.mStructDeclDepth]->mArraySize2[0]  = '\0';
            }

            gUlpParseInfo.mHostValInfo[gUlpParseInfo.mStructDeclDepth]->mPointer
                    = gUlpParseInfo.mHostValInfo4Typedef->mPointer;
            gUlpParseInfo.mHostValInfo[gUlpParseInfo.mStructDeclDepth]->mIsarray
                    = gUlpParseInfo.mHostValInfo4Typedef->mIsarray;
        }
        else
        {
            gUlpParseInfo.mHostValInfo[gUlpParseInfo.mStructDeclDepth]->mArraySize[0]   = '\0';
            gUlpParseInfo.mHostValInfo[gUlpParseInfo.mStructDeclDepth]->mArraySize2[0]  = '\0';
            gUlpParseInfo.mHostValInfo[gUlpParseInfo.mStructDeclDepth]->mPointer        = 0;
            gUlpParseInfo.mHostValInfo[gUlpParseInfo.mStructDeclDepth]->mIsarray        = ID_FALSE;
        }

    ;}
    break;

  case 95:

/* Line 1455 of yacc.c  */
#line 1124 "ulpCompy.y"
    {
        gUlpParseInfo.mHostValInfo[gUlpParseInfo.mStructDeclDepth]->mIsTypedef = ID_TRUE;
    ;}
    break;

  case 96:

/* Line 1455 of yacc.c  */
#line 1128 "ulpCompy.y"
    {
        /* BUG-28118 : system 헤더파일들도 파싱돼야함.                                 *
         * 8th. problem : can't resolve extern variable type at declaring section. */
        // extern 변수이고 standard type이 아니라면, 변수 선언시 type resolving을 하지않고,
        // 사용시 resolving을 하기위해 필요한 field.
        gUlpParseInfo.mHostValInfo[gUlpParseInfo.mStructDeclDepth]->mIsExtern = ID_TRUE;
    ;}
    break;

  case 100:

/* Line 1455 of yacc.c  */
#line 1142 "ulpCompy.y"
    {
        gUlpParseInfo.mSaveId = ID_TRUE;
        gUlpParseInfo.mHostValInfo[gUlpParseInfo.mStructDeclDepth]->mType = H_CHAR;
    ;}
    break;

  case 101:

/* Line 1455 of yacc.c  */
#line 1147 "ulpCompy.y"
    {
        /* BUG-31831 : An additional error message is needed to notify 
            the unacceptability of using varchar type in #include file. */
        if( gUlpCOMPIncludeIndex > 0 )
        {
            // error 처리
            ulpSetErrorCode( &gUlpParseInfo.mErrorMgr,
                             ulpERR_ABORT_COMP_Varchar_In_IncludeFile_Error );
            gUlpCOMPErrCode = ulpGetErrorSTATE( &gUlpParseInfo.mErrorMgr );
            COMPerror( ulpGetErrorMSG(&gUlpParseInfo.mErrorMgr) );
        }
        gUlpParseInfo.mSaveId = ID_TRUE;
        gUlpParseInfo.mHostValInfo[gUlpParseInfo.mStructDeclDepth]->mType = H_VARCHAR;
    ;}
    break;

  case 102:

/* Line 1455 of yacc.c  */
#line 1162 "ulpCompy.y"
    {
        gUlpParseInfo.mSaveId = ID_TRUE;
        gUlpParseInfo.mHostValInfo[gUlpParseInfo.mStructDeclDepth]->mType = H_NCHAR;
    ;}
    break;

  case 103:

/* Line 1455 of yacc.c  */
#line 1167 "ulpCompy.y"
    {
        /* BUG-31831 : An additional error message is needed to notify 
            the unacceptability of using varchar type in #include file. */
        if( gUlpCOMPIncludeIndex > 0 )
        {
            // error 처리
            ulpSetErrorCode( &gUlpParseInfo.mErrorMgr,
                             ulpERR_ABORT_COMP_Varchar_In_IncludeFile_Error );
            gUlpCOMPErrCode = ulpGetErrorSTATE( &gUlpParseInfo.mErrorMgr );
            COMPerror( ulpGetErrorMSG(&gUlpParseInfo.mErrorMgr) );
        }
        gUlpParseInfo.mSaveId = ID_TRUE;
        gUlpParseInfo.mHostValInfo[gUlpParseInfo.mStructDeclDepth]->mType = H_NVARCHAR;
    ;}
    break;

  case 104:

/* Line 1455 of yacc.c  */
#line 1182 "ulpCompy.y"
    {
        gUlpParseInfo.mSaveId = ID_TRUE;
        gUlpParseInfo.mHostValInfo[gUlpParseInfo.mStructDeclDepth]->mType = H_SHORT;
    ;}
    break;

  case 105:

/* Line 1455 of yacc.c  */
#line 1187 "ulpCompy.y"
    {
        gUlpParseInfo.mSaveId = ID_TRUE;

        switch ( gUlpParseInfo.mHostValInfo[gUlpParseInfo.mStructDeclDepth]->mType )
        {
            case H_SHORT:
            case H_LONG:
                break;
            default:
                gUlpParseInfo.mHostValInfo[gUlpParseInfo.mStructDeclDepth]->mType = H_INT;
                break;
        }
    ;}
    break;

  case 106:

/* Line 1455 of yacc.c  */
#line 1201 "ulpCompy.y"
    {
        gUlpParseInfo.mSaveId = ID_TRUE;

        if ( gUlpParseInfo.mHostValInfo[gUlpParseInfo.mStructDeclDepth]->mType == H_LONG )
        {
            gUlpParseInfo.mHostValInfo[gUlpParseInfo.mStructDeclDepth]->mType = H_LONGLONG;
        }
        else
        {
            gUlpParseInfo.mHostValInfo[gUlpParseInfo.mStructDeclDepth]->mType = H_LONG;
        }
    ;}
    break;

  case 107:

/* Line 1455 of yacc.c  */
#line 1214 "ulpCompy.y"
    {
        gUlpParseInfo.mHostValInfo[gUlpParseInfo.mStructDeclDepth]->mIssign = ID_TRUE;
    ;}
    break;

  case 108:

/* Line 1455 of yacc.c  */
#line 1218 "ulpCompy.y"
    {
        gUlpParseInfo.mHostValInfo[gUlpParseInfo.mStructDeclDepth]->mIssign = ID_FALSE;
    ;}
    break;

  case 109:

/* Line 1455 of yacc.c  */
#line 1222 "ulpCompy.y"
    {
        gUlpParseInfo.mSaveId = ID_TRUE;

        gUlpParseInfo.mHostValInfo[gUlpParseInfo.mStructDeclDepth]->mType = H_FLOAT;
    ;}
    break;

  case 110:

/* Line 1455 of yacc.c  */
#line 1228 "ulpCompy.y"
    {
        gUlpParseInfo.mSaveId = ID_TRUE;

        gUlpParseInfo.mHostValInfo[gUlpParseInfo.mStructDeclDepth]->mType = H_DOUBLE;
    ;}
    break;

  case 113:

/* Line 1455 of yacc.c  */
#line 1236 "ulpCompy.y"
    {
        gUlpParseInfo.mSaveId = ID_TRUE;

        gUlpParseInfo.mHostValInfo[gUlpParseInfo.mStructDeclDepth]->mType = H_UNKNOWN;
    ;}
    break;

  case 114:

/* Line 1455 of yacc.c  */
#line 1242 "ulpCompy.y"
    {
        gUlpParseInfo.mSaveId = ID_TRUE;

        gUlpParseInfo.mHostValInfo[gUlpParseInfo.mStructDeclDepth]->mType = H_USER_DEFINED;
        gUlpParseInfo.mHostValInfo[gUlpParseInfo.mStructDeclDepth]->mIsstruct = ID_TRUE;
    ;}
    break;

  case 115:

/* Line 1455 of yacc.c  */
#line 1249 "ulpCompy.y"
    {
        gUlpParseInfo.mSaveId = ID_TRUE;

        gUlpParseInfo.mHostValInfo[gUlpParseInfo.mStructDeclDepth]->mType = H_UNKNOWN;
    ;}
    break;

  case 116:

/* Line 1455 of yacc.c  */
#line 1255 "ulpCompy.y"
    {
        // In case of struct type or typedef type
        gUlpParseInfo.mSaveId = ID_TRUE;

        gUlpParseInfo.ulpCopyHostInfo4Typedef( gUlpParseInfo.mHostValInfo[gUlpParseInfo.mStructDeclDepth],
                                               gUlpParseInfo.mHostValInfo4Typedef );
    ;}
    break;

  case 117:

/* Line 1455 of yacc.c  */
#line 1263 "ulpCompy.y"
    {
        gUlpParseInfo.mSaveId = ID_TRUE;

        gUlpParseInfo.mHostValInfo[gUlpParseInfo.mStructDeclDepth]->mType = H_BINARY;
    ;}
    break;

  case 118:

/* Line 1455 of yacc.c  */
#line 1269 "ulpCompy.y"
    {
        gUlpParseInfo.mSaveId = ID_TRUE;

        gUlpParseInfo.mHostValInfo[gUlpParseInfo.mStructDeclDepth]->mType = H_BINARY2;
    ;}
    break;

  case 119:

/* Line 1455 of yacc.c  */
#line 1275 "ulpCompy.y"
    {
        gUlpParseInfo.mSaveId = ID_TRUE;

        gUlpParseInfo.mHostValInfo[gUlpParseInfo.mStructDeclDepth]->mType = H_BIT;
    ;}
    break;

  case 120:

/* Line 1455 of yacc.c  */
#line 1281 "ulpCompy.y"
    {
        gUlpParseInfo.mSaveId = ID_TRUE;

        gUlpParseInfo.mHostValInfo[gUlpParseInfo.mStructDeclDepth]->mType = H_BYTES;
    ;}
    break;

  case 121:

/* Line 1455 of yacc.c  */
#line 1287 "ulpCompy.y"
    {
        gUlpParseInfo.mSaveId = ID_TRUE;
        
        gUlpParseInfo.mHostValInfo[gUlpParseInfo.mStructDeclDepth]->mType = H_VARBYTE;
    ;}
    break;

  case 122:

/* Line 1455 of yacc.c  */
#line 1293 "ulpCompy.y"
    {
        gUlpParseInfo.mSaveId = ID_TRUE;

        gUlpParseInfo.mHostValInfo[gUlpParseInfo.mStructDeclDepth]->mType = H_NIBBLE;
    ;}
    break;

  case 123:

/* Line 1455 of yacc.c  */
#line 1299 "ulpCompy.y"
    {
        gUlpParseInfo.mSaveId = ID_TRUE;

        gUlpParseInfo.mHostValInfo[gUlpParseInfo.mStructDeclDepth]->mType = H_INTEGER;
    ;}
    break;

  case 124:

/* Line 1455 of yacc.c  */
#line 1305 "ulpCompy.y"
    {
        gUlpParseInfo.mSaveId = ID_TRUE;

        gUlpParseInfo.mHostValInfo[gUlpParseInfo.mStructDeclDepth]->mType = H_NUMERIC;
    ;}
    break;

  case 125:

/* Line 1455 of yacc.c  */
#line 1311 "ulpCompy.y"
    {
        gUlpParseInfo.mSaveId = ID_TRUE;

        gUlpParseInfo.mHostValInfo[gUlpParseInfo.mStructDeclDepth]->mType = H_BLOBLOCATOR;
    ;}
    break;

  case 126:

/* Line 1455 of yacc.c  */
#line 1317 "ulpCompy.y"
    {
        gUlpParseInfo.mSaveId = ID_TRUE;

        gUlpParseInfo.mHostValInfo[gUlpParseInfo.mStructDeclDepth]->mType = H_CLOBLOCATOR;
    ;}
    break;

  case 127:

/* Line 1455 of yacc.c  */
#line 1323 "ulpCompy.y"
    {
        gUlpParseInfo.mSaveId = ID_TRUE;

        gUlpParseInfo.mHostValInfo[gUlpParseInfo.mStructDeclDepth]->mType = H_BLOB;
    ;}
    break;

  case 128:

/* Line 1455 of yacc.c  */
#line 1329 "ulpCompy.y"
    {
        gUlpParseInfo.mSaveId = ID_TRUE;

        gUlpParseInfo.mHostValInfo[gUlpParseInfo.mStructDeclDepth]->mType = H_CLOB;
    ;}
    break;

  case 129:

/* Line 1455 of yacc.c  */
#line 1335 "ulpCompy.y"
    {
        gUlpParseInfo.mSaveId = ID_TRUE;

        if( ID_SIZEOF(SQLLEN) == 4 )
        {
            gUlpParseInfo.mHostValInfo[gUlpParseInfo.mStructDeclDepth]->mType = H_INT;
        }
        else
        {
            gUlpParseInfo.mHostValInfo[gUlpParseInfo.mStructDeclDepth]->mType = H_LONG;
        }
        // SQLLEN 은 무조건 signed
        gUlpParseInfo.mHostValInfo[gUlpParseInfo.mStructDeclDepth]->mIssign = ID_TRUE;
    ;}
    break;

  case 130:

/* Line 1455 of yacc.c  */
#line 1350 "ulpCompy.y"
    {
        gUlpParseInfo.mSaveId = ID_TRUE;

        gUlpParseInfo.mHostValInfo[gUlpParseInfo.mStructDeclDepth]->mType = H_TIMESTAMP;
    ;}
    break;

  case 131:

/* Line 1455 of yacc.c  */
#line 1356 "ulpCompy.y"
    {
        gUlpParseInfo.mSaveId = ID_TRUE;

        gUlpParseInfo.mHostValInfo[gUlpParseInfo.mStructDeclDepth]->mType = H_TIME;
    ;}
    break;

  case 132:

/* Line 1455 of yacc.c  */
#line 1362 "ulpCompy.y"
    {
        gUlpParseInfo.mSaveId = ID_TRUE;

        gUlpParseInfo.mHostValInfo[gUlpParseInfo.mStructDeclDepth]->mType = H_DATE;
    ;}
    break;

  case 133:

/* Line 1455 of yacc.c  */
#line 1369 "ulpCompy.y"
    {
        gUlpParseInfo.mSaveId = ID_TRUE;
        gUlpParseInfo.mHostValInfo[gUlpParseInfo.mStructDeclDepth]->mType = H_SQLDA;
    ;}
    break;

  case 134:

/* Line 1455 of yacc.c  */
#line 1374 "ulpCompy.y"
    {
        gUlpParseInfo.mSaveId = ID_TRUE;

        gUlpParseInfo.mHostValInfo[gUlpParseInfo.mStructDeclDepth]->mType = H_FAILOVERCB;
    ;}
    break;

  case 137:

/* Line 1455 of yacc.c  */
#line 1391 "ulpCompy.y"
    {
        if ( gUlpParseInfo.mStructDeclDepth > 0 )
        {
            idlOS::free( gUlpParseInfo.mHostValInfo[gUlpParseInfo.mStructDeclDepth] );
            gUlpParseInfo.mHostValInfo[gUlpParseInfo.mStructDeclDepth] = NULL;
        }
        gUlpParseInfo.mStructDeclDepth--;
        if( gUlpParseInfo.mStructPtr != NULL )
        {
            gUlpParseInfo.mHostValInfo[gUlpParseInfo.mStructDeclDepth]->mStructLink
                = gUlpParseInfo.mStructPtr;
        }
    ;}
    break;

  case 138:

/* Line 1455 of yacc.c  */
#line 1405 "ulpCompy.y"
    {
        /* BUG-28118 : system 헤더파일들도 파싱돼야함.                       *
         * 6th. problem : Nested structure 정의중 scope를 잘못 계산하는 문제 */
        if ( gUlpParseInfo.mStructDeclDepth > 0 )
        {
            idlOS::free( gUlpParseInfo.mHostValInfo[gUlpParseInfo.mStructDeclDepth] );
            gUlpParseInfo.mHostValInfo[gUlpParseInfo.mStructDeclDepth] = NULL;
        }

        gUlpParseInfo.mStructDeclDepth--;

        // typedef struct 의 경우 mStructLink가 설정되지 않는다.
        // 이 경우 mStructLink가가 설정되는 시점은 해당 type을 이용해 변수를 선언하는 시점이다.
        if( gUlpParseInfo.mStructPtr != NULL )
        {
            gUlpParseInfo.mHostValInfo[gUlpParseInfo.mStructDeclDepth]->mStructLink
                = gUlpParseInfo.mStructPtr;
        }
    ;}
    break;

  case 139:

/* Line 1455 of yacc.c  */
#line 1428 "ulpCompy.y"
    {
        if ( gUlpParseInfo.mStructDeclDepth > 0 )
        {
            idlOS::free( gUlpParseInfo.mHostValInfo[gUlpParseInfo.mStructDeclDepth] );
            gUlpParseInfo.mHostValInfo[gUlpParseInfo.mStructDeclDepth] = NULL;
        }
        gUlpParseInfo.mStructDeclDepth--;
        if( gUlpParseInfo.mStructPtr != NULL )
        {
            gUlpParseInfo.mHostValInfo[gUlpParseInfo.mStructDeclDepth]->mStructLink
                    = gUlpParseInfo.mStructPtr;
        }
    ;}
    break;

  case 140:

/* Line 1455 of yacc.c  */
#line 1442 "ulpCompy.y"
    {
        /* BUG-28118 : system 헤더파일들도 파싱돼야함.                       *
         * 6th. problem : Nested structure 정의중 scope를 잘못 계산하는 문제 */
        if ( gUlpParseInfo.mStructDeclDepth > 0 )
        {
            idlOS::free( gUlpParseInfo.mHostValInfo[gUlpParseInfo.mStructDeclDepth] );
            gUlpParseInfo.mHostValInfo[gUlpParseInfo.mStructDeclDepth] = NULL;
        }
        gUlpParseInfo.mStructDeclDepth--;
        if( gUlpParseInfo.mStructPtr != NULL )
        {
            gUlpParseInfo.mHostValInfo[gUlpParseInfo.mStructDeclDepth]->mStructLink
                    = gUlpParseInfo.mStructPtr;
        }
    ;}
    break;

  case 141:

/* Line 1455 of yacc.c  */
#line 1458 "ulpCompy.y"
    {
        /* BUG-28118 : system 헤더파일들도 파싱돼야함.                       *
         * 6th. problem : Nested structure 정의중 scope를 잘못 계산하는 문제 */
        if ( gUlpParseInfo.mStructDeclDepth > 0 )
        {
            idlOS::free( gUlpParseInfo.mHostValInfo[gUlpParseInfo.mStructDeclDepth] );
            gUlpParseInfo.mHostValInfo[gUlpParseInfo.mStructDeclDepth] = NULL;
        }

        gUlpParseInfo.mStructDeclDepth--;

        /* BUG-27875 : 구조체안의 typedef type인식못함. */
        gUlpParseInfo.mSkipTypedef = ID_FALSE;

        /* BUG-28118 : system 헤더파일들도 파싱돼야함.            *
         * 2th. problem : 빈구조체 선언이 허용안됨. ex) struct A; *
         * 5th. problem : 정의되지 않은 구조체 포인터 변수 선언안됨. */
        // structure 이름 정보 저장함.
        idlOS::strcpy( gUlpParseInfo.mHostValInfo[gUlpParseInfo.mStructDeclDepth]->mStructName,
                       (yyvsp[(2) - (2)].strval) );
    ;}
    break;

  case 142:

/* Line 1455 of yacc.c  */
#line 1483 "ulpCompy.y"
    {
        /* BUG-27875 : 구조체안의 typedef type인식못함. */
        gUlpParseInfo.mSkipTypedef = ID_FALSE;
        // id가 struct table에 있는지 확인한다.
        if ( gUlpStructT.ulpStructLookup( (yyvsp[(1) - (2)].strval), gUlpCurDepth - 1 )
             != NULL )
        {
            // error 처리
            ulpSetErrorCode( &gUlpParseInfo.mErrorMgr,
                             ulpERR_ABORT_COMP_C_Duplicate_Structname_Error,
                             (yyvsp[(1) - (2)].strval) );
            gUlpCOMPErrCode = ulpGetErrorSTATE( &gUlpParseInfo.mErrorMgr );
            COMPerror( ulpGetErrorMSG(&gUlpParseInfo.mErrorMgr) );
        }
        else
        {

            idlOS::strcpy( gUlpParseInfo.mHostValInfo[gUlpParseInfo.mStructDeclDepth - 1]->mStructName,
                           (yyvsp[(1) - (2)].strval) );
            // struct table에 저장한다.
            /* BUG-28118 : system 헤더파일들도 파싱돼야함.                       *
             * 6th. problem : Nested structure 정의중 scope를 잘못 계산하는 문제 */
            gUlpParseInfo.mHostValInfo[gUlpParseInfo.mStructDeclDepth - 1]->mStructLink
                    = gUlpStructT.ulpStructAdd ( (yyvsp[(1) - (2)].strval), gUlpCurDepth );

            if ( gUlpParseInfo.mHostValInfo[gUlpParseInfo.mStructDeclDepth - 1]->mStructLink
                 == NULL )
            {
                // error 처리
                ulpSetErrorCode( &gUlpParseInfo.mErrorMgr,
                                 ulpERR_ABORT_COMP_C_Duplicate_Structname_Error,
                                 (yyvsp[(1) - (2)].strval) );
                gUlpCOMPErrCode = ulpGetErrorSTATE( &gUlpParseInfo.mErrorMgr );
                COMPerror( ulpGetErrorMSG(&gUlpParseInfo.mErrorMgr) );
            }
        }
    ;}
    break;

  case 143:

/* Line 1455 of yacc.c  */
#line 1524 "ulpCompy.y"
    {
        /* BUG-27875 : 구조체안의 typedef type인식못함. */
        gUlpParseInfo.mSkipTypedef = ID_FALSE;
        gUlpParseInfo.mHostValInfo[gUlpParseInfo.mStructDeclDepth - 1]->mStructName[0] = '\0';
        // struct table에 저장한다.
        // no tag struct node는 hash table 마지막 bucket에 추가된다.
        /* BUG-28118 : system 헤더파일들도 파싱돼야함.                       *
         * 6th. problem : Nested structure 정의중 scope를 잘못 계산하는 문제 */
        gUlpParseInfo.mHostValInfo[gUlpParseInfo.mStructDeclDepth - 1]->mStructLink
                = gUlpStructT.ulpNoTagStructAdd ();
    ;}
    break;

  case 144:

/* Line 1455 of yacc.c  */
#line 1539 "ulpCompy.y"
    {
        /* BUG-27875 : 구조체안의 typedef type인식못함. */
        // 아래 구문을 처리하기위해 mSkipTypedef 변수 추가됨.
        // typedef struct Struct1 Struct1;
        // struct Struct1       <- mSkipTypedef = ID_TRUE  :
        //                          Struct1은 비록 이전에 typedef되어 있지만 렉서에서 C_TYPE_NAME이아닌
        // {                        C_IDENTIFIER로 인식되어야 한다.
        //    ...               <- mSkipTypedef = ID_FALSE :
        //    ...                   필드에 typedef 이름이 오면 C_TYPE_NAME으로 인식돼야한다.
        // };
        gUlpParseInfo.mSkipTypedef = ID_TRUE;
        gUlpParseInfo.mStructDeclDepth++;
        /* BUG-28118 : system 헤더파일들도 파싱돼야함.                       *
         * 6th. problem : Nested structure 정의중 scope를 잘못 계산하는 문제 */
        if( gUlpParseInfo.mStructDeclDepth >= MAX_NESTED_STRUCT_DEPTH )
        {
            ulpSetErrorCode( &gUlpParseInfo.mErrorMgr,
                             ulpERR_ABORT_COMP_Exceed_Max_Nested_Struct_Depth_Error );
            ulpPrintfErrorCode( stderr,
                                &gUlpParseInfo.mErrorMgr);
            IDE_ASSERT(0);
        }
        else
        {
            gUlpParseInfo.mHostValInfo[gUlpParseInfo.mStructDeclDepth]
                    = (ulpSymTElement *)idlOS::malloc(ID_SIZEOF( ulpSymTElement));
            if ( gUlpParseInfo.mHostValInfo[gUlpParseInfo.mStructDeclDepth] == NULL )
            {
                ulpSetErrorCode( &gUlpParseInfo.mErrorMgr,
                                 ulpERR_ABORT_Memory_Alloc_Error );
                ulpPrintfErrorCode( stderr,
                                    &gUlpParseInfo.mErrorMgr);
                IDE_ASSERT(0);
            }
            else
            {
                (void) gUlpParseInfo.ulpInitHostInfo();
            }
        }
        gUlpParseInfo.mSaveId      = ID_TRUE;
    ;}
    break;

  case 145:

/* Line 1455 of yacc.c  */
#line 1581 "ulpCompy.y"
    {
        /* BUG-27875 : 구조체안의 typedef type인식못함. */
        gUlpParseInfo.mSkipTypedef = ID_TRUE;
        gUlpParseInfo.mStructDeclDepth++;
        /* BUG-28118 : system 헤더파일들도 파싱돼야함.                       *
         * 6th. problem : Nested structure 정의중 scope를 잘못 계산하는 문제 */
        if( gUlpParseInfo.mStructDeclDepth >= MAX_NESTED_STRUCT_DEPTH )
        {
            ulpSetErrorCode( &gUlpParseInfo.mErrorMgr,
                             ulpERR_ABORT_COMP_Exceed_Max_Nested_Struct_Depth_Error );
            ulpPrintfErrorCode( stderr,
                                &gUlpParseInfo.mErrorMgr);
            IDE_ASSERT(0);
        }
        else
        {
            gUlpParseInfo.mHostValInfo[gUlpParseInfo.mStructDeclDepth]
                    = (ulpSymTElement *)idlOS::malloc(ID_SIZEOF( ulpSymTElement));
            if ( gUlpParseInfo.mHostValInfo[gUlpParseInfo.mStructDeclDepth] == NULL )
            {
                ulpSetErrorCode( &gUlpParseInfo.mErrorMgr,
                                 ulpERR_ABORT_Memory_Alloc_Error );
                ulpPrintfErrorCode( stderr,
                                    &gUlpParseInfo.mErrorMgr);
                IDE_ASSERT(0);
            }
            else
            {
                (void) gUlpParseInfo.ulpInitHostInfo();
            }
        }
        gUlpParseInfo.mSaveId      = ID_TRUE;
    ;}
    break;

  case 150:

/* Line 1455 of yacc.c  */
#line 1630 "ulpCompy.y"
    {
        iduListNode *sIterator = NULL;
        iduListNode *sNextNode = NULL;
        ulpSymTElement *sSymNode;

        if ( (gUlpParseInfo.mHostValInfo[gUlpParseInfo.mStructDeclDepth]->mType == H_VARCHAR) ||
             (gUlpParseInfo.mHostValInfo[gUlpParseInfo.mStructDeclDepth]->mType == H_NVARCHAR) )
        {
            if ( gUlpParseInfo.mFuncDecl != ID_TRUE )
            {
                WRITESTR2BUFCOMP ( (SChar *)" */\n" );

                /* BUG-26375 valgrind bug */
                IDU_LIST_ITERATE_SAFE(&(gUlpParseInfo.mVarcharVarList),
                                        sIterator, sNextNode)
                {
                    sSymNode = (ulpSymTElement *)sIterator->mObj;
                    if ( gDontPrint2file != ID_TRUE )
                    {
                        gUlpCodeGen.ulpGenVarchar( sSymNode );
                    }
                    IDU_LIST_REMOVE(sIterator);
                    idlOS::free(sIterator);
                }
                IDU_LIST_INIT( &(gUlpParseInfo.mVarcharVarList) );
                gUlpParseInfo.mVarcharDecl = ID_FALSE;
            }
        }

        gUlpParseInfo.mHostValInfo4Typedef = NULL;
        gUlpParseInfo.ulpInitHostInfo();
    ;}
    break;

  case 152:

/* Line 1455 of yacc.c  */
#line 1670 "ulpCompy.y"
    {
        SChar       *sVarName;
        ulpSymTNode *sSymNode;
        iduListNode *sIterator = NULL;
        iduListNode *sVarcharListNode = NULL;

        // field 이름 중복 검사함.
        if ( gUlpParseInfo.mHostValInfo[gUlpParseInfo.mStructDeclDepth - 1]->mStructLink->mChild->ulpSymLookup
             ( gUlpParseInfo.mHostValInfo[gUlpParseInfo.mStructDeclDepth]->mName ) != NULL )
        {
            // error 처리
            ulpSetErrorCode( &gUlpParseInfo.mErrorMgr,
                             ulpERR_ABORT_COMP_C_Duplicate_Structname_Error,
                             gUlpParseInfo.mHostValInfo[gUlpParseInfo.mStructDeclDepth]->mName );
            gUlpCOMPErrCode = ulpGetErrorSTATE( &gUlpParseInfo.mErrorMgr );
            COMPerror( ulpGetErrorMSG(&gUlpParseInfo.mErrorMgr) );
        }
        else
        {
            /* BUG-28118 : system 헤더파일들도 파싱돼야함.            *
             * 5th. problem : 정의되지 않은 구조체 포인터 변수 선언안됨. *
             * 8th. problem : can't resolve extern variable type at declaring section. */
            if ( (gUlpParseInfo.mHostValInfo[gUlpParseInfo.mStructDeclDepth]->mIsstruct  == ID_TRUE) &&
                 (gUlpParseInfo.mHostValInfo[gUlpParseInfo.mStructDeclDepth]->mStructName[0] != '\0') &&
                 (gUlpParseInfo.mHostValInfo[gUlpParseInfo.mStructDeclDepth]->mStructLink == NULL) )
            {   // struct 변수 선언의 경우.
                // structure 변수 선언의경우 pointer가 아니라면 struct table에서
                // 해당 struct tag가 존재하는지 검사하며, pointer일 경우에는 검사하지 않고
                // 나중에 해당 변수를 사용할때 검사한다.
                if ( (gUlpParseInfo.mHostValInfo[gUlpParseInfo.mStructDeclDepth]->mPointer  == 0) &&
                     (gUlpParseInfo.mHostValInfo[gUlpParseInfo.mStructDeclDepth]->mIsExtern == ID_FALSE) )
                {   // it's not a pointer of struct and extern.

                    gUlpParseInfo.mStructPtr = gUlpStructT.ulpStructLookupAll(
                                        gUlpParseInfo.mHostValInfo[gUlpParseInfo.mStructDeclDepth]->mStructName,
                                        gUlpCurDepth );
                    if ( gUlpParseInfo.mStructPtr == NULL )
                    {
                        // error 처리

                        ulpSetErrorCode( &gUlpParseInfo.mErrorMgr,
                                        ulpERR_ABORT_COMP_C_Unknown_Structname_Error,
                                        gUlpParseInfo.mHostValInfo[gUlpParseInfo.mStructDeclDepth]->mStructName );
                        gUlpCOMPErrCode = ulpGetErrorSTATE( &gUlpParseInfo.mErrorMgr );
                        COMPerror( ulpGetErrorMSG(&gUlpParseInfo.mErrorMgr) );
                    }
                    else
                    {
                        gUlpParseInfo.mHostValInfo[gUlpParseInfo.mStructDeclDepth]->mStructLink
                                = gUlpParseInfo.mStructPtr;
                    }
                }
                else
                {   // it's a point of struct
                    // do nothing
                }
            }

            if ( (gUlpParseInfo.mHostValInfo[gUlpParseInfo.mStructDeclDepth]->mType == H_CHAR) ||
                 (gUlpParseInfo.mHostValInfo[gUlpParseInfo.mStructDeclDepth]->mType == H_VARCHAR) )
            {
                IDU_LIST_ITERATE(&gUlpProgOption.mNcharVarNameList, sIterator)
                {
                    sVarName = (SChar* )sIterator->mObj;
                    if ( idlOS::strcmp( sVarName, gUlpParseInfo.mHostValInfo[gUlpParseInfo.mStructDeclDepth]->mName )
                         == 0 )
                    {
                        if ( gUlpParseInfo.mHostValInfo[gUlpParseInfo.mStructDeclDepth]->mType == H_CHAR )
                        {
                            gUlpParseInfo.mHostValInfo[gUlpParseInfo.mStructDeclDepth]->mType = H_NCHAR;
                        }
                        else
                        {
                            gUlpParseInfo.mHostValInfo[gUlpParseInfo.mStructDeclDepth]->mType = H_NVARCHAR;
                        }
                    }
                }
            }

            /* BUG-28118 : system 헤더파일들도 파싱돼야함.                       *
             * 6th. problem : Nested structure 정의중 scope를 잘못 계산하는 문제 */
            // struct 필드를 add하려 한다면, mHostValInfo의 이전 index에 저장된 struct node pointer 를 이용해야함.
            sSymNode =
                    gUlpParseInfo.mHostValInfo[gUlpParseInfo.mStructDeclDepth - 1]->mStructLink
                    ->mChild->ulpSymAdd(
                                           gUlpParseInfo.mHostValInfo[gUlpParseInfo.mStructDeclDepth]
                                       );

            if ( sSymNode != NULL )
            {
                if ( (gUlpParseInfo.mHostValInfo[gUlpParseInfo.mStructDeclDepth]->mType == H_VARCHAR) ||
                     (gUlpParseInfo.mHostValInfo[gUlpParseInfo.mStructDeclDepth]->mType == H_NVARCHAR) )
                {
                    sVarcharListNode =
                        (iduListNode*)idlOS::malloc(ID_SIZEOF(iduListNode));
                    if (sVarcharListNode == NULL)
                    {
                        ulpSetErrorCode(&gUlpParseInfo.mErrorMgr, ulpERR_ABORT_Memory_Alloc_Error);
                        gUlpCOMPErrCode = ulpGetErrorSTATE(&gUlpParseInfo.mErrorMgr);
                        COMPerror(ulpGetErrorMSG(&gUlpParseInfo.mErrorMgr));
                    }
                    else
                    {
                        IDU_LIST_INIT_OBJ(sVarcharListNode, &(sSymNode->mElement));
                        IDU_LIST_ADD_LAST(&(gUlpParseInfo.mVarcharVarList), sVarcharListNode);
                    }
                }
            }
        }
    ;}
    break;

  case 153:

/* Line 1455 of yacc.c  */
#line 1781 "ulpCompy.y"
    {
        SChar *sVarName;
        ulpSymTNode *sSymNode;
        iduListNode *sIterator = NULL;
        iduListNode *sVarcharListNode = NULL;

        // field 이름 중복 검사함.
        if ( gUlpParseInfo.mHostValInfo[gUlpParseInfo.mStructDeclDepth - 1]->mStructLink->mChild->ulpSymLookup
             ( gUlpParseInfo.mHostValInfo[gUlpParseInfo.mStructDeclDepth]->mName ) != NULL )
        {
            // error 처리
            ulpSetErrorCode( &gUlpParseInfo.mErrorMgr,
                             ulpERR_ABORT_COMP_C_Duplicate_Structname_Error,
                             gUlpParseInfo.mHostValInfo[gUlpParseInfo.mStructDeclDepth]->mName );
            gUlpCOMPErrCode = ulpGetErrorSTATE( &gUlpParseInfo.mErrorMgr );
            COMPerror( ulpGetErrorMSG(&gUlpParseInfo.mErrorMgr) );
        }
        else
        {
            /* BUG-28118 : system 헤더파일들도 파싱돼야함.            *
             * 5th. problem : 정의되지 않은 구조체 포인터 변수 선언안됨. *
             * 8th. problem : can't resolve extern variable type at declaring section. */
            if ( (gUlpParseInfo.mHostValInfo[gUlpParseInfo.mStructDeclDepth]->mIsstruct  == ID_TRUE) &&
                 (gUlpParseInfo.mHostValInfo[gUlpParseInfo.mStructDeclDepth]->mStructName[0] != '\0') &&
                 (gUlpParseInfo.mHostValInfo[gUlpParseInfo.mStructDeclDepth]->mStructLink == NULL) )
            {   // struct 변수 선언의 경우.
                // structure 변수 선언의경우 pointer가 아니라면 struct table에서
                // 해당 struct tag가 존재하는지 검사하며, pointer일 경우에는 검사하지 않고
                // 나중에 해당 변수를 사용할때 검사한다.
                if ( (gUlpParseInfo.mHostValInfo[gUlpParseInfo.mStructDeclDepth]->mPointer  == 0) &&
                     (gUlpParseInfo.mHostValInfo[gUlpParseInfo.mStructDeclDepth]->mIsExtern == ID_FALSE) )
                {   // it's not a pointer of struct and extern.

                    gUlpParseInfo.mStructPtr = gUlpStructT.ulpStructLookupAll(
                                        gUlpParseInfo.mHostValInfo[gUlpParseInfo.mStructDeclDepth]->mStructName,
                                        gUlpCurDepth );
                    if ( gUlpParseInfo.mStructPtr == NULL )
                    {
                        // error 처리

                        ulpSetErrorCode( &gUlpParseInfo.mErrorMgr,
                                        ulpERR_ABORT_COMP_C_Unknown_Structname_Error,
                                        gUlpParseInfo.mHostValInfo[gUlpParseInfo.mStructDeclDepth]->mStructName );
                        gUlpCOMPErrCode = ulpGetErrorSTATE( &gUlpParseInfo.mErrorMgr );
                        COMPerror( ulpGetErrorMSG(&gUlpParseInfo.mErrorMgr) );
                    }
                    else
                    {
                        gUlpParseInfo.mHostValInfo[gUlpParseInfo.mStructDeclDepth]->mStructLink
                                = gUlpParseInfo.mStructPtr;
                    }
                }
                else
                {   // it's a point of struct
                    // do nothing
                }
            }

            if ( (gUlpParseInfo.mHostValInfo[gUlpParseInfo.mStructDeclDepth]->mType == H_CHAR) ||
                 (gUlpParseInfo.mHostValInfo[gUlpParseInfo.mStructDeclDepth]->mType == H_VARCHAR) )
            {
                IDU_LIST_ITERATE(&gUlpProgOption.mNcharVarNameList, sIterator)
                {
                    sVarName = (SChar* )sIterator->mObj;
                    if ( idlOS::strcmp( sVarName, gUlpParseInfo.mHostValInfo[gUlpParseInfo.mStructDeclDepth]->mName )
                         == 0 )
                    {
                        if ( gUlpParseInfo.mHostValInfo[gUlpParseInfo.mStructDeclDepth]->mType == H_CHAR )
                        {
                            gUlpParseInfo.mHostValInfo[gUlpParseInfo.mStructDeclDepth]->mType = H_NCHAR;
                        }
                        else
                        {
                            gUlpParseInfo.mHostValInfo[gUlpParseInfo.mStructDeclDepth]->mType = H_NVARCHAR;
                        }
                    }
                }
            }

            /* BUG-28118 : system 헤더파일들도 파싱돼야함.                       *
             * 6th. problem : Nested structure 정의중 scope를 잘못 계산하는 문제 */
            // struct 필드를 add하려 한다면, mHostValInfo의 이전 index에 저장된 struct node pointer 를 이용해야함.
            sSymNode =
                  gUlpParseInfo.mHostValInfo[gUlpParseInfo.mStructDeclDepth - 1]->mStructLink
                  ->mChild->ulpSymAdd (
                                          gUlpParseInfo.mHostValInfo[gUlpParseInfo.mStructDeclDepth]
                                      );

            if ( sSymNode != NULL )
            {
                if ( (gUlpParseInfo.mHostValInfo[gUlpParseInfo.mStructDeclDepth]->mType == H_VARCHAR) ||
                     (gUlpParseInfo.mHostValInfo[gUlpParseInfo.mStructDeclDepth]->mType == H_NVARCHAR) )
                {
                    sVarcharListNode =
                        (iduListNode*)idlOS::malloc(ID_SIZEOF(iduListNode));
                    if (sVarcharListNode == NULL)
                    {
                        ulpSetErrorCode(&gUlpParseInfo.mErrorMgr, ulpERR_ABORT_Memory_Alloc_Error);
                        gUlpCOMPErrCode = ulpGetErrorSTATE(&gUlpParseInfo.mErrorMgr);
                        COMPerror(ulpGetErrorMSG(&gUlpParseInfo.mErrorMgr));
                    }
                    else
                    {
                        IDU_LIST_INIT_OBJ(sVarcharListNode, &(sSymNode->mElement));
                        IDU_LIST_ADD_LAST(&(gUlpParseInfo.mVarcharVarList), sVarcharListNode);
                    }
                }
            }
        }
    ;}
    break;

  case 154:

/* Line 1455 of yacc.c  */
#line 1895 "ulpCompy.y"
    {
        // , 를 사용한 동일 type을 여러개 선언할 경우 필요한 초기화.
        gUlpParseInfo.mSaveId = ID_TRUE;
        if ( gUlpParseInfo.mHostValInfo4Typedef != NULL )
        {
            if ( gUlpParseInfo.mHostValInfo4Typedef->mArraySize[0] != '\0' )
            {
                idlOS::strncpy( gUlpParseInfo.mHostValInfo[gUlpParseInfo.mStructDeclDepth]->mArraySize,
                                gUlpParseInfo.mHostValInfo4Typedef->mArraySize,
                                MAX_NUMBER_LEN - 1 );
            }
            else
            {
                gUlpParseInfo.mHostValInfo[gUlpParseInfo.mStructDeclDepth]->mArraySize[0]   = '\0';
            }

            if ( gUlpParseInfo.mHostValInfo4Typedef->mArraySize2[0] != '\0' )
            {
                idlOS::strncpy( gUlpParseInfo.mHostValInfo[gUlpParseInfo.mStructDeclDepth]->mArraySize2,
                                gUlpParseInfo.mHostValInfo4Typedef->mArraySize2,
                                MAX_NUMBER_LEN - 1 );
            }
            else
            {
                gUlpParseInfo.mHostValInfo[gUlpParseInfo.mStructDeclDepth]->mArraySize2[0]  = '\0';
            }

            gUlpParseInfo.mHostValInfo[gUlpParseInfo.mStructDeclDepth]->mPointer
                    = gUlpParseInfo.mHostValInfo4Typedef->mPointer;
            gUlpParseInfo.mHostValInfo[gUlpParseInfo.mStructDeclDepth]->mIsarray
                    = gUlpParseInfo.mHostValInfo4Typedef->mIsarray;
        }
        else
        {
            gUlpParseInfo.mHostValInfo[gUlpParseInfo.mStructDeclDepth]->mArraySize[0]   = '\0';
            gUlpParseInfo.mHostValInfo[gUlpParseInfo.mStructDeclDepth]->mArraySize2[0]  = '\0';
            gUlpParseInfo.mHostValInfo[gUlpParseInfo.mStructDeclDepth]->mPointer        = 0;
            gUlpParseInfo.mHostValInfo[gUlpParseInfo.mStructDeclDepth]->mIsarray        = ID_FALSE;
        }
    ;}
    break;

  case 165:

/* Line 1455 of yacc.c  */
#line 1961 "ulpCompy.y"
    {
        gUlpParseInfo.mConstantExprStr[0] = '\0';
        gUlpParseInfo.mArrDepth = 0;
    ;}
    break;

  case 166:

/* Line 1455 of yacc.c  */
#line 1966 "ulpCompy.y"
    {
        gUlpParseInfo.mConstantExprStr[0] = '\0';
        gUlpParseInfo.mArrDepth = 0;
    ;}
    break;

  case 169:

/* Line 1455 of yacc.c  */
#line 1976 "ulpCompy.y"
    {
        gUlpParseInfo.mHostValInfo[gUlpParseInfo.mStructDeclDepth]->mPointer ++;
        gUlpParseInfo.mHostValInfo[gUlpParseInfo.mStructDeclDepth]->mIsarray = ID_TRUE;
        if( gUlpParseInfo.mArrDepth == 0 )
        {
            if ( gUlpParseInfo.mHostValInfo[gUlpParseInfo.mStructDeclDepth]->mArraySize[0] == '\0' )
            {
                // do nothing
            }
            else
            {
                idlOS::strncpy( gUlpParseInfo.mHostValInfo[gUlpParseInfo.mStructDeclDepth]->mArraySize2,
                                gUlpParseInfo.mHostValInfo[gUlpParseInfo.mStructDeclDepth]->mArraySize,
                                MAX_NUMBER_LEN - 1 );
            }
        }
        else if ( gUlpParseInfo.mArrDepth == 1 )
        {
            // 2차 배열까지만 처리함.
            gUlpParseInfo.mHostValInfo[gUlpParseInfo.mStructDeclDepth]->mArraySize2[0] = '\0';
        }
        else
        {
            // 2차 배열까지만 처리함.
            // ignore
        }

        gUlpParseInfo.mArrDepth++;
    ;}
    break;

  case 170:

/* Line 1455 of yacc.c  */
#line 2006 "ulpCompy.y"
    {
        gUlpParseInfo.mHostValInfo[gUlpParseInfo.mStructDeclDepth]->mPointer ++;
        gUlpParseInfo.mHostValInfo[gUlpParseInfo.mStructDeclDepth]->mIsarray = ID_TRUE;

        if( gUlpParseInfo.mArrDepth == 0 )
        {
            if ( gUlpParseInfo.mHostValInfo[gUlpParseInfo.mStructDeclDepth]->mArraySize[0] == '\0' )
            {
                // 1차 배열의 expr
                idlOS::strncpy( gUlpParseInfo.mHostValInfo[gUlpParseInfo.mStructDeclDepth]->mArraySize,
                                gUlpParseInfo.mConstantExprStr,
                                MAX_NUMBER_LEN - 1 );
            }
            else
            {
                idlOS::strncpy( gUlpParseInfo.mHostValInfo[gUlpParseInfo.mStructDeclDepth]->mArraySize2,
                                gUlpParseInfo.mHostValInfo[gUlpParseInfo.mStructDeclDepth]->mArraySize,
                                MAX_NUMBER_LEN - 1 );
                idlOS::strncpy( gUlpParseInfo.mHostValInfo[gUlpParseInfo.mStructDeclDepth]->mArraySize,
                                gUlpParseInfo.mConstantExprStr,
                                MAX_NUMBER_LEN - 1 );
            }

        }
        else if ( gUlpParseInfo.mArrDepth == 1 )
        {
            if ( gUlpParseInfo.mHostValInfo[gUlpParseInfo.mStructDeclDepth]->mArraySize2[0] == '\0' )
            {
                // 2차 배열의 expr
                idlOS::strncpy( gUlpParseInfo.mHostValInfo[gUlpParseInfo.mStructDeclDepth]->mArraySize2,
                                gUlpParseInfo.mConstantExprStr,
                                MAX_NUMBER_LEN - 1 );
            }
            else
            {
                // do nothing
            }
        }

        gUlpParseInfo.mArrExpr = ID_FALSE;
        gUlpParseInfo.mArrDepth++;
    ;}
    break;

  case 172:

/* Line 1455 of yacc.c  */
#line 2050 "ulpCompy.y"
    {
        gUlpCurDepth--;
    ;}
    break;

  case 174:

/* Line 1455 of yacc.c  */
#line 2058 "ulpCompy.y"
    {
        // array [ expr ] => expr 의 시작이라는 것을 알림. expr을 저장하기 위함.
        // 물론 expr 문법 체크도 함.
        gUlpParseInfo.mConstantExprStr[0] = '\0';
        gUlpParseInfo.mArrExpr = ID_TRUE;
    ;}
    break;

  case 175:

/* Line 1455 of yacc.c  */
#line 2068 "ulpCompy.y"
    {
        gUlpCurDepth++;
        gUlpParseInfo.mFuncDecl = ID_TRUE;
    ;}
    break;

  case 176:

/* Line 1455 of yacc.c  */
#line 2076 "ulpCompy.y"
    {
        gUlpParseInfo.mHostValInfo[gUlpParseInfo.mStructDeclDepth]->mPointer ++;
        gUlpParseInfo.mHostValInfo[gUlpParseInfo.mStructDeclDepth]->mAlloc = ID_TRUE;
    ;}
    break;

  case 177:

/* Line 1455 of yacc.c  */
#line 2081 "ulpCompy.y"
    {
        gUlpParseInfo.mHostValInfo[gUlpParseInfo.mStructDeclDepth]->mPointer ++;
        gUlpParseInfo.mHostValInfo[gUlpParseInfo.mStructDeclDepth]->mAlloc = ID_TRUE;
    ;}
    break;

  case 178:

/* Line 1455 of yacc.c  */
#line 2086 "ulpCompy.y"
    {
        gUlpParseInfo.mHostValInfo[gUlpParseInfo.mStructDeclDepth]->mPointer ++;
        gUlpParseInfo.mHostValInfo[gUlpParseInfo.mStructDeclDepth]->mAlloc = ID_TRUE;
    ;}
    break;

  case 179:

/* Line 1455 of yacc.c  */
#line 2091 "ulpCompy.y"
    {
        gUlpParseInfo.mHostValInfo[gUlpParseInfo.mStructDeclDepth]->mPointer ++;
        gUlpParseInfo.mHostValInfo[gUlpParseInfo.mStructDeclDepth]->mAlloc = ID_TRUE;
    ;}
    break;

  case 190:

/* Line 1455 of yacc.c  */
#line 2125 "ulpCompy.y"
    {
        SChar *sVarName;
        iduListNode *sIterator = NULL;

        /* BUG-28118 : system 헤더파일들도 파싱돼야함.            *
         * 5th. problem : 정의되지 않은 구조체 포인터 변수 선언안됨. */
        if ( (gUlpParseInfo.mHostValInfo[gUlpParseInfo.mStructDeclDepth]->mIsstruct  == ID_TRUE) &&
             (gUlpParseInfo.mHostValInfo[gUlpParseInfo.mStructDeclDepth]->mStructName[0] != '\0') &&
             (gUlpParseInfo.mHostValInfo[gUlpParseInfo.mStructDeclDepth]->mStructLink == NULL) )
        {   // struct 변수 선언의 경우, type check rigidly.

            gUlpParseInfo.mStructPtr = gUlpStructT.ulpStructLookupAll(
                                gUlpParseInfo.mHostValInfo[gUlpParseInfo.mStructDeclDepth]->mStructName,
                                gUlpCurDepth );
            if ( gUlpParseInfo.mStructPtr == NULL )
            {
                // error 처리

                ulpSetErrorCode( &gUlpParseInfo.mErrorMgr,
                                 ulpERR_ABORT_COMP_C_Unknown_Structname_Error,
                                 gUlpParseInfo.mHostValInfo[gUlpParseInfo.mStructDeclDepth]->mStructName );
                gUlpCOMPErrCode = ulpGetErrorSTATE( &gUlpParseInfo.mErrorMgr );
                COMPerror( ulpGetErrorMSG(&gUlpParseInfo.mErrorMgr) );
            }
            else
            {
                gUlpParseInfo.mHostValInfo[gUlpParseInfo.mStructDeclDepth]->mStructLink
                        = gUlpParseInfo.mStructPtr;
            }

        }

        if ( (gUlpParseInfo.mHostValInfo[gUlpParseInfo.mStructDeclDepth]->mType == H_CHAR) ||
             (gUlpParseInfo.mHostValInfo[gUlpParseInfo.mStructDeclDepth]->mType == H_VARCHAR) )
        {
            IDU_LIST_ITERATE(&gUlpProgOption.mNcharVarNameList, sIterator)
            {
                sVarName = (SChar* )sIterator->mObj;
                if ( idlOS::strcmp( sVarName, gUlpParseInfo.mHostValInfo[gUlpParseInfo.mStructDeclDepth]->mName )
                     == 0 )
                {
                    if ( gUlpParseInfo.mHostValInfo[gUlpParseInfo.mStructDeclDepth]->mType == H_CHAR )
                    {
                        gUlpParseInfo.mHostValInfo[gUlpParseInfo.mStructDeclDepth]->mType = H_NCHAR;
                    }
                    else
                    {
                        gUlpParseInfo.mHostValInfo[gUlpParseInfo.mStructDeclDepth]->mType = H_NVARCHAR;
                    }
                }
            }
        }

        if( gUlpScopeT.ulpSAdd ( gUlpParseInfo.mHostValInfo[gUlpParseInfo.mStructDeclDepth]
                                 , gUlpCurDepth )
            == NULL )
        {
            // error 처리

            ulpSetErrorCode( &gUlpParseInfo.mErrorMgr,
                             ulpERR_ABORT_COMP_C_Add_Symbol_Error,
                             gUlpParseInfo.mHostValInfo[gUlpParseInfo.mStructDeclDepth]->mName );
            gUlpCOMPErrCode = ulpGetErrorSTATE( &gUlpParseInfo.mErrorMgr );
            COMPerror( ulpGetErrorMSG(&gUlpParseInfo.mErrorMgr) );
        }

        gUlpParseInfo.ulpInitHostInfo();
    ;}
    break;

  case 226:

/* Line 1455 of yacc.c  */
#line 2258 "ulpCompy.y"
    {
        /* BUG-29081 : 변수 선언부가 statement 중간에 들어오면 파싱 에러발생. */
        // statement 를 파싱한뒤 변수 type정보를 저장해두고 있는 자료구조를 초기화해줘야한다.
        // 저장 자체를 안하는게 이상적이나 type처리 문법을 선언부와 함께 공유하므로 어쩔수 없다.
        gUlpParseInfo.ulpInitHostInfo();
    ;}
    break;

  case 230:

/* Line 1455 of yacc.c  */
#line 2274 "ulpCompy.y"
    {
        if( gUlpParseInfo.mFuncDecl == ID_TRUE )
        {
            gUlpParseInfo.mFuncDecl = ID_FALSE;
        }
    ;}
    break;

  case 257:

/* Line 1455 of yacc.c  */
#line 2331 "ulpCompy.y"
    {
        if( idlOS::strlen((yyvsp[(1) - (1)].strval)) >= MAX_HOSTVAR_NAME_SIZE )
        {
            //error 처리

            ulpSetErrorCode( &gUlpParseInfo.mErrorMgr,
                             ulpERR_ABORT_COMP_C_Exceed_Max_Id_Length_Error,
                             (yyvsp[(1) - (1)].strval) );
            gUlpCOMPErrCode = ulpGetErrorSTATE( &gUlpParseInfo.mErrorMgr );
            COMPerror( ulpGetErrorMSG(&gUlpParseInfo.mErrorMgr) );
        }
        else
        {
            if( gUlpParseInfo.mSaveId == ID_TRUE )
            {
                idlOS::strcpy( gUlpParseInfo.mHostValInfo[gUlpParseInfo.mStructDeclDepth]->mName,
                               (yyvsp[(1) - (1)].strval) );
                gUlpParseInfo.mSaveId = ID_FALSE;
            }
        }
    ;}
    break;

  case 260:

/* Line 1455 of yacc.c  */
#line 2367 "ulpCompy.y"
    {
            /* BUG-28061 : preprocessing을마치면 marco table을 초기화하고, *
             *             ulpComp 에서 재구축한다.                       */
            gUlpCOMPStartCond = gUlpCOMPPrevCond;
        ;}
    break;

  case 261:

/* Line 1455 of yacc.c  */
#line 2373 "ulpCompy.y"
    {
            gUlpCOMPStartCond = gUlpCOMPPrevCond;
        ;}
    break;

  case 262:

/* Line 1455 of yacc.c  */
#line 2377 "ulpCompy.y"
    {
            gUlpCOMPStartCond = gUlpCOMPPrevCond;
        ;}
    break;

  case 263:

/* Line 1455 of yacc.c  */
#line 2381 "ulpCompy.y"
    {
            /* macro 조건문의 경우 참이면 C상태, 거짓이면 MACRO_IFSKIP 상태로
             * 전이 된다. */
        ;}
    break;

  case 264:

/* Line 1455 of yacc.c  */
#line 2386 "ulpCompy.y"
    {
            /* macro 조건문의 경우 참이면 C상태, 거짓이면 MACRO_IFSKIP 상태로
             * 전이 된다. */
        ;}
    break;

  case 265:

/* Line 1455 of yacc.c  */
#line 2391 "ulpCompy.y"
    {
            /* macro 조건문의 경우 참이면 C상태, 거짓이면 MACRO_IFSKIP 상태로
             * 전이 된다. */
        ;}
    break;

  case 266:

/* Line 1455 of yacc.c  */
#line 2396 "ulpCompy.y"
    {
            /* macro 조건문의 경우 참이면 C상태, 거짓이면 MACRO_IFSKIP 상태로
             * 전이 된다. */
        ;}
    break;

  case 267:

/* Line 1455 of yacc.c  */
#line 2401 "ulpCompy.y"
    {
            /* macro 조건문의 경우 참이면 C상태, 거짓이면 MACRO_IFSKIP 상태로
             * 전이 된다. */
        ;}
    break;

  case 269:

/* Line 1455 of yacc.c  */
#line 2410 "ulpCompy.y"
    {
            /* #include <...> */

            // 1. check exist header file in include paths
            if ( gUlpProgOption.ulpLookupHeader( (yyvsp[(3) - (4)].strval), ID_TRUE )
                 == IDE_FAILURE )
            {
                // do nothing
            }
            else
            {

                // 현재 #include 처리다.
                gDontPrint2file = ID_TRUE;
                /* BUG-27683 : iostream 사용 제거 */
                // 2. flex 버퍼 상태 저장.
                ulpCOMPSaveBufferState();
                // 3. doCOMPparse()를 재호출한다.
                doCOMPparse( gUlpProgOption.ulpGetIncList() );
                // 전에 #inlcude 처리중이었나? 확인함
                gDontPrint2file = gUlpProgOption.ulpIsHeaderCInclude();

                // 4. precompiler를 실행한 directory를 current path로 재setting
                idlOS::strcpy( gUlpProgOption.mCurrentPath, gUlpProgOption.mStartPath );
            }

        ;}
    break;

  case 270:

/* Line 1455 of yacc.c  */
#line 2438 "ulpCompy.y"
    {

            // 1. check exist header file in include paths
            if ( gUlpProgOption.ulpLookupHeader( (yyvsp[(3) - (4)].strval), ID_TRUE )
                 == IDE_FAILURE )
            {
                // do nothing
            }
            else
            {

                // 현재 #include 처리다.
                gDontPrint2file = ID_TRUE;
                /* BUG-27683 : iostream 사용 제거 */
                // 2. flex 버퍼 상태 저장.
                ulpCOMPSaveBufferState();
                // 3. doCOMPparse()를 재호출한다.
                doCOMPparse( gUlpProgOption.ulpGetIncList() );
                // 전에 #inlcude 처리중이었나? 확인함
                gDontPrint2file = gUlpProgOption.ulpIsHeaderCInclude();

                // 4. precompiler를 실행한 directory를 current path로 재setting
                idlOS::strcpy( gUlpProgOption.mCurrentPath, gUlpProgOption.mStartPath );
            }

        ;}
    break;

  case 271:

/* Line 1455 of yacc.c  */
#line 2470 "ulpCompy.y"
    {
            SChar sTmpDEFtext[ MAX_MACRO_DEFINE_CONTENT_LEN ];
            idlOS::memset(sTmpDEFtext,0,MAX_MACRO_DEFINE_CONTENT_LEN);

            ulpCOMPEraseBN4MacroText( sTmpDEFtext , ID_FALSE );

            if ( sTmpDEFtext[0] == '\0' )
            {
                // macro symbol table에 추가함.
                if( gUlpMacroT.ulpMDefine( (yyvsp[(2) - (2)].strval), NULL, ID_FALSE ) == IDE_FAILURE )
                {

                    ulpSetErrorCode( &gUlpParseInfo.mErrorMgr,
                                     ulpERR_ABORT_Memory_Alloc_Error );
                    gUlpCOMPErrCode = ulpGetErrorSTATE( &gUlpParseInfo.mErrorMgr );
                    COMPerror( ulpGetErrorMSG(&gUlpParseInfo.mErrorMgr) );
                }
            }
            else
            {
                // macro symbol table에 추가함.
                if( gUlpMacroT.ulpMDefine( (yyvsp[(2) - (2)].strval), sTmpDEFtext, ID_FALSE ) == IDE_FAILURE )
                {

                    ulpSetErrorCode( &gUlpParseInfo.mErrorMgr,
                                     ulpERR_ABORT_Memory_Alloc_Error );
                    gUlpCOMPErrCode = ulpGetErrorSTATE( &gUlpParseInfo.mErrorMgr );
                    COMPerror( ulpGetErrorMSG(&gUlpParseInfo.mErrorMgr) );
                }
            }

        ;}
    break;

  case 272:

/* Line 1455 of yacc.c  */
#line 2503 "ulpCompy.y"
    {
            // function macro의경우 인자 정보는 따로 저장되지 않는다.
            SChar sTmpDEFtext[ MAX_MACRO_DEFINE_CONTENT_LEN ];

            idlOS::memset(sTmpDEFtext,0,MAX_MACRO_DEFINE_CONTENT_LEN);
            ulpCOMPEraseBN4MacroText( sTmpDEFtext , ID_FALSE );

            // #define A() {...} 이면, macro id는 A이다.
            if ( sTmpDEFtext[0] == '\0' )
            {
                // macro symbol table에 추가함.
                if ( gUlpMacroT.ulpMDefine( (yyvsp[(2) - (2)].strval), NULL, ID_TRUE ) == IDE_FAILURE )
                {

                    ulpSetErrorCode( &gUlpParseInfo.mErrorMgr,
                                     ulpERR_ABORT_Memory_Alloc_Error );
                    gUlpCOMPErrCode = ulpGetErrorSTATE( &gUlpParseInfo.mErrorMgr );
                    COMPerror( ulpGetErrorMSG(&gUlpParseInfo.mErrorMgr) );
                }
            }
            else
            {
                // macro symbol table에 추가함.
                if ( gUlpMacroT.ulpMDefine( (yyvsp[(2) - (2)].strval), sTmpDEFtext, ID_TRUE ) == IDE_FAILURE )
                {

                    ulpSetErrorCode( &gUlpParseInfo.mErrorMgr,
                                     ulpERR_ABORT_Memory_Alloc_Error );
                    gUlpCOMPErrCode = ulpGetErrorSTATE( &gUlpParseInfo.mErrorMgr );
                    COMPerror( ulpGetErrorMSG(&gUlpParseInfo.mErrorMgr) );
                }
            }

        ;}
    break;

  case 273:

/* Line 1455 of yacc.c  */
#line 2541 "ulpCompy.y"
    {
            // $<strval>2 를 macro symbol table에서 삭제 한다.
            gUlpMacroT.ulpMUndef( (yyvsp[(2) - (2)].strval) );
        ;}
    break;

  case 274:

/* Line 1455 of yacc.c  */
#line 2549 "ulpCompy.y"
    {
            SInt  sVal;
            SChar sTmpExpBuf[MAX_MACRO_IF_EXPR_LEN];
            /* BUG-32413 APRE memory allocation failure should be fixed */
            idlOS::memset(sTmpExpBuf, 0, MAX_MACRO_IF_EXPR_LEN);

            switch( gUlpCOMPifstackMgr[gUlpCOMPifstackInd]->ulpPrevIfStatus() )
            {
                // 이전 상태가 PP_IF_IGNORE 이면 계속 무시함.
                case PP_IF_IGNORE :
                    // 단순히 token만 소모하는 역할이다. PPIFparse 호출하지 않는다.
                    ulpCOMPEraseBN4MacroText( sTmpExpBuf , ID_TRUE );
                    gUlpCOMPStartCond = CP_ST_MACRO_IF_SKIP;
                    // if stack manager 에 결과 정보 push
                    gUlpCOMPifstackMgr[gUlpCOMPifstackInd]->ulpIfpush( PP_IF, PP_IF_IGNORE );
                    break;
                // 이전 상태가 PP_IF_TRUE 이면 이번 #if <expr>파싱하여 값을 확인해봐야함.
                case PP_IF_TRUE :
                    // #if expression 을 복사해온다.
                    ulpCOMPEraseBN4MacroText( sTmpExpBuf , ID_TRUE );
                    gUlpPPIFbufptr = sTmpExpBuf;
                    gUlpPPIFbuflim = sTmpExpBuf + idlOS::strlen(sTmpExpBuf);

                    if ( PPIFparse( sTmpExpBuf, &sVal ) != 0 )
                    {
                        //error 처리

                        ulpSetErrorCode( &gUlpParseInfo.mErrorMgr,
                                         ulpERR_ABORT_COMP_IF_Macro_Syntax_Error );
                        gUlpCOMPErrCode = ulpGetErrorSTATE( &gUlpParseInfo.mErrorMgr );
                        COMPerror( ulpGetErrorMSG(&gUlpParseInfo.mErrorMgr) );
                    }
                    /* macro 조건문의 경우 참이면 MACRO상태, 거짓이면 MACRO_IFSKIP 상태로
                    * 전이 된다. */
                    if ( sVal != 0 )
                    {
                        // true
                        gUlpCOMPStartCond = gUlpCOMPPrevCond;
                        // if stack manager 에 결과 정보 push
                        gUlpCOMPifstackMgr[gUlpCOMPifstackInd]->ulpIfpush( PP_IF, PP_IF_TRUE );
                    }
                    else
                    {
                        // false
                        gUlpCOMPStartCond = CP_ST_MACRO_IF_SKIP;
                        // if stack manager 에 결과 정보 push
                        gUlpCOMPifstackMgr[gUlpCOMPifstackInd]->ulpIfpush( PP_IF, PP_IF_FALSE );
                    }
                    break;
                // 이전 상태가 PP_IF_FALSE 이면 무시함.
                case PP_IF_FALSE :
                    // 단순히 token만 소모하는 역할이다. PPIFparse 호출하지 않는다.
                    ulpCOMPEraseBN4MacroText( sTmpExpBuf , ID_TRUE );
                    gUlpCOMPStartCond = CP_ST_MACRO_IF_SKIP;
                    // if stack manager 에 결과 정보 push
                    gUlpCOMPifstackMgr[gUlpCOMPifstackInd]->ulpIfpush( PP_IF, PP_IF_IGNORE );
                    break;

                default:
                    IDE_ASSERT(0);
            }
        ;}
    break;

  case 275:

/* Line 1455 of yacc.c  */
#line 2615 "ulpCompy.y"
    {
            SInt  sVal;
            SChar sTmpExpBuf[MAX_MACRO_IF_EXPR_LEN];
            /* BUG-32413 APRE memory allocation failure should be fixed */
            idlOS::memset(sTmpExpBuf, 0, MAX_MACRO_IF_EXPR_LEN);

            // #elif 순서 문법 검사.
            if ( gUlpCOMPifstackMgr[gUlpCOMPifstackInd]->ulpIfCheckGrammar( PP_ELIF )
                 == ID_FALSE )
            {
                //error 처리

                ulpSetErrorCode( &gUlpParseInfo.mErrorMgr,
                                 ulpERR_ABORT_COMP_ELIF_Macro_Sequence_Error );
                gUlpCOMPErrCode = ulpGetErrorSTATE( &gUlpParseInfo.mErrorMgr );
                COMPerror( ulpGetErrorMSG(&gUlpParseInfo.mErrorMgr) );
            }

            switch( gUlpCOMPifstackMgr[gUlpCOMPifstackInd]->ulpPrevIfStatus() )
            {
                case PP_IF_IGNORE :
                    // 단순히 token만 소모하는 역할이다. PPIFparse 호출하지 않는다.
                    ulpCOMPEraseBN4MacroText( sTmpExpBuf , ID_TRUE );
                    gUlpCOMPStartCond = CP_ST_MACRO_IF_SKIP;
                    // if stack manager 에 결과 정보 push
                    gUlpCOMPifstackMgr[gUlpCOMPifstackInd]->ulpIfpush( PP_ELIF, PP_IF_IGNORE );
                    break;

                case PP_IF_TRUE :
                    // 단순히 token만 소모하는 역할이다. PPIFparse 호출하지 않는다.
                    ulpCOMPEraseBN4MacroText( sTmpExpBuf , ID_TRUE );
                    gUlpCOMPStartCond = CP_ST_MACRO_IF_SKIP;
                    // if stack manager 에 결과 정보 push
                    gUlpCOMPifstackMgr[gUlpCOMPifstackInd]->ulpIfpush( PP_ELIF, PP_IF_IGNORE );
                    break;

                case PP_IF_FALSE :
                    ulpCOMPEraseBN4MacroText( sTmpExpBuf , ID_TRUE );

                    gUlpPPIFbufptr = sTmpExpBuf;
                    gUlpPPIFbuflim = sTmpExpBuf + idlOS::strlen(sTmpExpBuf);

                    if ( PPIFparse( sTmpExpBuf, &sVal ) != 0 )
                    {
                        //error 처리

                        ulpSetErrorCode( &gUlpParseInfo.mErrorMgr,
                                         ulpERR_ABORT_COMP_ELIF_Macro_Syntax_Error );
                        gUlpCOMPErrCode = ulpGetErrorSTATE( &gUlpParseInfo.mErrorMgr );
                        COMPerror( ulpGetErrorMSG(&gUlpParseInfo.mErrorMgr) );
                    }

                    /* macro 조건문의 경우 참이면 MACRO상태, 거짓이면 MACRO_IFSKIP 상태로
                     * 전이 된다. */
                    if ( sVal != 0 )
                    {
                        gUlpCOMPStartCond = gUlpCOMPPrevCond;
                        // if stack manager 에 결과 정보 push
                        gUlpCOMPifstackMgr[gUlpCOMPifstackInd]->ulpIfpush( PP_ELIF, PP_IF_TRUE );
                    }
                    else
                    {
                        gUlpCOMPStartCond = CP_ST_MACRO_IF_SKIP;
                        // if stack manager 에 결과 정보 push
                        gUlpCOMPifstackMgr[gUlpCOMPifstackInd]->ulpIfpush( PP_ELIF, PP_IF_FALSE );
                    }
                    break;

                default:
                    IDE_ASSERT(0);
            }
        ;}
    break;

  case 276:

/* Line 1455 of yacc.c  */
#line 2691 "ulpCompy.y"
    {
            switch( gUlpCOMPifstackMgr[gUlpCOMPifstackInd]->ulpPrevIfStatus() )
            {
                case PP_IF_IGNORE :
                    gUlpCOMPStartCond = CP_ST_MACRO_IF_SKIP;
                    // if stack manager 에 결과 정보 push
                    gUlpCOMPifstackMgr[gUlpCOMPifstackInd]->ulpIfpush( PP_IFDEF, PP_IF_IGNORE );
                    break;

                case PP_IF_TRUE :
                    // $<strval>2 를 macro symbol table에 존재하는지 확인한다.
                    if ( gUlpMacroT.ulpMLookup((yyvsp[(2) - (2)].strval)) != NULL )
                    {
                        // 존재한다
                        gUlpCOMPStartCond = gUlpCOMPPrevCond;
                        // if stack manager 에 결과 정보 push
                        gUlpCOMPifstackMgr[gUlpCOMPifstackInd]->ulpIfpush( PP_IFDEF, PP_IF_TRUE );
                    }
                    else
                    {
                        // 존재안한다
                        gUlpCOMPStartCond = CP_ST_MACRO_IF_SKIP;
                        // if stack manager 에 결과 정보 push
                        gUlpCOMPifstackMgr[gUlpCOMPifstackInd]->ulpIfpush( PP_IFDEF, PP_IF_FALSE );
                    }
                    break;

                case PP_IF_FALSE :
                    gUlpCOMPStartCond = CP_ST_MACRO_IF_SKIP;
                    // if stack manager 에 결과 정보 push
                    gUlpCOMPifstackMgr[gUlpCOMPifstackInd]->ulpIfpush( PP_IFDEF, PP_IF_IGNORE );
                    break;

                default:
                    IDE_ASSERT(0);
            }
        ;}
    break;

  case 277:

/* Line 1455 of yacc.c  */
#line 2732 "ulpCompy.y"
    {
            switch( gUlpCOMPifstackMgr[gUlpCOMPifstackInd]->ulpPrevIfStatus() )
            {
                case PP_IF_IGNORE :
                    gUlpCOMPStartCond = CP_ST_MACRO_IF_SKIP;
                    // if stack manager 에 결과 정보 push
                    gUlpCOMPifstackMgr[gUlpCOMPifstackInd]->ulpIfpush( PP_IFNDEF, PP_IF_IGNORE );
                    break;

                case PP_IF_TRUE :
                    // $<strval>2 를 macro symbol table에 존재하는지 확인한다.
                    if ( gUlpMacroT.ulpMLookup((yyvsp[(2) - (2)].strval)) != NULL )
                    {
                        // 존재한다
                        gUlpCOMPStartCond = CP_ST_MACRO_IF_SKIP;
                        // if stack manager 에 결과 정보 push
                        gUlpCOMPifstackMgr[gUlpCOMPifstackInd]->ulpIfpush( PP_IFNDEF, PP_IF_FALSE );
                    }
                    else
                    {
                        // 존재안한다
                        gUlpCOMPStartCond = gUlpCOMPPrevCond;
                        // if stack manager 에 결과 정보 push
                        gUlpCOMPifstackMgr[gUlpCOMPifstackInd]->ulpIfpush( PP_IFNDEF, PP_IF_TRUE );
                    }
                    break;

                case PP_IF_FALSE :
                    gUlpCOMPStartCond = CP_ST_MACRO_IF_SKIP;
                    // if stack manager 에 결과 정보 push
                    gUlpCOMPifstackMgr[gUlpCOMPifstackInd]->ulpIfpush( PP_IFNDEF, PP_IF_IGNORE );
                    break;

                default:
                    IDE_ASSERT(0);
            }
        ;}
    break;

  case 278:

/* Line 1455 of yacc.c  */
#line 2773 "ulpCompy.y"
    {
            // #else 순서 문법 검사.
            if ( gUlpCOMPifstackMgr[gUlpCOMPifstackInd]->ulpIfCheckGrammar( PP_ELSE )
                 == ID_FALSE )
            {
                // error 처리

                ulpSetErrorCode( &gUlpParseInfo.mErrorMgr,
                                 ulpERR_ABORT_COMP_ELSE_Macro_Sequence_Error );
                gUlpCOMPErrCode = ulpGetErrorSTATE( &gUlpParseInfo.mErrorMgr );
                COMPerror( ulpGetErrorMSG(&gUlpParseInfo.mErrorMgr) );
            }

            switch( gUlpCOMPifstackMgr[gUlpCOMPifstackInd]->ulpPrevIfStatus() )
            {
                case PP_IF_IGNORE :
                case PP_IF_TRUE :
                    gUlpCOMPStartCond = CP_ST_MACRO_IF_SKIP;
                    // if stack manager 에 결과 정보 push
                    gUlpCOMPifstackMgr[gUlpCOMPifstackInd]->ulpIfpush( PP_ELSE, PP_IF_IGNORE );
                    break;

                case PP_IF_FALSE :
                    gUlpCOMPStartCond = gUlpCOMPPrevCond;
                    // if stack manager 에 결과 정보 push
                    gUlpCOMPifstackMgr[gUlpCOMPifstackInd]->ulpIfpush( PP_ELSE, PP_IF_TRUE );
                    break;

                default:
                    IDE_ASSERT(0);
            }
        ;}
    break;

  case 279:

/* Line 1455 of yacc.c  */
#line 2809 "ulpCompy.y"
    {
            // #endif 순서 문법 검사.
            if ( gUlpCOMPifstackMgr[gUlpCOMPifstackInd]->ulpIfCheckGrammar( PP_ENDIF )
                 == ID_FALSE )
            {
                // error 처리

                ulpSetErrorCode( &gUlpParseInfo.mErrorMgr,
                                 ulpERR_ABORT_COMP_ENDIF_Macro_Sequence_Error );
                gUlpCOMPErrCode = ulpGetErrorSTATE( &gUlpParseInfo.mErrorMgr );
                COMPerror( ulpGetErrorMSG(&gUlpParseInfo.mErrorMgr) );
            }
            // if stack 을 이전 조건문 까지 pop 한다.
            gUlpCOMPifstackMgr[gUlpCOMPifstackInd]->ulpIfpop4endif();

            /* BUG-27961 : preprocessor의 중첩 #if처리시 #endif 다음소스 무조건 출력하는 버그  */
            if( gUlpCOMPifstackMgr[gUlpCOMPifstackInd]->ulpIfneedSkip4Endif() == ID_TRUE )
            {
                // 출력 하지마라.
                gUlpCOMPStartCond = CP_ST_MACRO_IF_SKIP;
            }
            else
            {
                // 출력 해라.
                gUlpCOMPStartCond = gUlpCOMPPrevCond;
            }
        ;}
    break;

  case 280:

/* Line 1455 of yacc.c  */
#line 2847 "ulpCompy.y"
    {
        // 내장구문을 comment형태로 쓴다.
        gUlpCodeGen.ulpGenComment( gUlpCodeGen.ulpGetQueryBuf() );
        // 내장구문 관련 코드생성한다.
        if( gUlpStmttype > S_IGNORE )
        {
            gUlpCodeGen.ulpGenEmSQLFlush( gUlpStmttype, gUlpIsPrintStmt );
        }
        // ulpCodeGen class 의 query buffer 를 초기화한다.
        gUlpCodeGen.ulpGenInitQBuff();
        // ulpCodeGen class 의 mEmSQLInfo 를 초기화한다.
        gUlpCodeGen.ulpClearEmSQLInfo();
        // lexer의 상태를 embedded sql 구문을 파싱하기 이전상태로 되돌린다.
        gUlpCOMPStartCond = gUlpCOMPPrevCond;
        // init variables
        gUlpIsPrintStmt = ID_TRUE;
        gUlpStmttype    = S_UNKNOWN;
    ;}
    break;

  case 281:

/* Line 1455 of yacc.c  */
#line 2866 "ulpCompy.y"
    {

        gUlpCodeGen.ulpGenComment( gUlpCodeGen.ulpGetQueryBuf() );
        if( gUlpStmttype > S_IGNORE )
        {
            gUlpCodeGen.ulpGenEmSQLFlush( gUlpStmttype, gUlpIsPrintStmt );
        }
        gUlpCodeGen.ulpGenInitQBuff();
        gUlpCodeGen.ulpClearEmSQLInfo();
        gUlpCOMPStartCond = gUlpCOMPPrevCond;
        // init variables
        gUlpIsPrintStmt = ID_TRUE;
        gUlpStmttype    = S_UNKNOWN;
    ;}
    break;

  case 282:

/* Line 1455 of yacc.c  */
#line 2881 "ulpCompy.y"
    {

        gUlpCodeGen.ulpGenComment( gUlpCodeGen.ulpGetQueryBuf() );
        if( gUlpStmttype > S_IGNORE )
        {
            gUlpCodeGen.ulpGenEmSQLFlush( gUlpStmttype, gUlpIsPrintStmt );
        }
        gUlpCodeGen.ulpGenInitQBuff();
        gUlpCodeGen.ulpClearEmSQLInfo();
        gUlpCOMPStartCond = gUlpCOMPPrevCond;
        // init variables
        gUlpIsPrintStmt = ID_TRUE;
        gUlpStmttype    = S_UNKNOWN;
    ;}
    break;

  case 283:

/* Line 1455 of yacc.c  */
#line 2896 "ulpCompy.y"
    {

        gUlpCodeGen.ulpGenComment( gUlpCodeGen.ulpGetQueryBuf() );
        if( gUlpStmttype > S_IGNORE )
        {
            gUlpCodeGen.ulpGenEmSQLFlush( gUlpStmttype, gUlpIsPrintStmt );
        }
        gUlpCodeGen.ulpGenInitQBuff();
        gUlpCodeGen.ulpClearEmSQLInfo();
        gUlpCOMPStartCond = gUlpCOMPPrevCond;
        // init variables
        gUlpIsPrintStmt = ID_TRUE;
        gUlpStmttype    = S_UNKNOWN;
    ;}
    break;

  case 284:

/* Line 1455 of yacc.c  */
#line 2911 "ulpCompy.y"
    {

        gUlpCodeGen.ulpGenComment( gUlpCodeGen.ulpGetQueryBuf() );
        if( gUlpStmttype > S_IGNORE )
        {
            gUlpCodeGen.ulpGenEmSQLFlush( gUlpStmttype, gUlpIsPrintStmt );
        }
        gUlpCodeGen.ulpGenInitQBuff();
        gUlpCodeGen.ulpClearEmSQLInfo();
        gUlpCOMPStartCond = gUlpCOMPPrevCond;
        gUlpIsPrintStmt = ID_TRUE;
        gUlpStmttype    = S_UNKNOWN;
    ;}
    break;

  case 285:

/* Line 1455 of yacc.c  */
#line 2925 "ulpCompy.y"
    {
        gUlpCodeGen.ulpGenInitQBuff();
        gUlpCodeGen.ulpClearEmSQLInfo();
        gUlpCOMPStartCond = gUlpCOMPPrevCond;
    ;}
    break;

  case 286:

/* Line 1455 of yacc.c  */
#line 2931 "ulpCompy.y"
    {
        // whenever 구문을 comment로 쓴다.
        gUlpCodeGen.ulpGenComment( gUlpCodeGen.ulpGetQueryBuf() );

        gUlpCodeGen.ulpGenInitQBuff();
        gUlpCodeGen.ulpClearEmSQLInfo();
        gUlpCOMPStartCond = gUlpCOMPPrevCond;
    ;}
    break;

  case 287:

/* Line 1455 of yacc.c  */
#line 2940 "ulpCompy.y"
    {
        gUlpCodeGen.ulpGenComment( gUlpCodeGen.ulpGetQueryBuf() );
        if( gUlpStmttype > S_IGNORE )
        {
            gUlpCodeGen.ulpGenEmSQLFlush( gUlpStmttype, gUlpIsPrintStmt );
        }
        gUlpCodeGen.ulpGenInitQBuff();
        gUlpCodeGen.ulpClearEmSQLInfo();
        gUlpCOMPStartCond = gUlpCOMPPrevCond;
        gUlpIsPrintStmt = ID_TRUE;
        gUlpStmttype    = S_UNKNOWN;
    ;}
    break;

  case 288:

/* Line 1455 of yacc.c  */
#line 2954 "ulpCompy.y"
    {
        gUlpCodeGen.ulpGenComment( gUlpCodeGen.ulpGetQueryBuf() );
        gUlpCodeGen.ulpGenInitQBuff();
        gUlpCodeGen.ulpClearEmSQLInfo();
        gUlpIsPrintStmt = ID_TRUE;
        gUlpStmttype    = S_UNKNOWN;
    ;}
    break;

  case 289:

/* Line 1455 of yacc.c  */
#line 2965 "ulpCompy.y"
    {
        // do nothing
    ;}
    break;

  case 290:

/* Line 1455 of yacc.c  */
#line 2969 "ulpCompy.y"
    {
        if ( idlOS::strlen((yyvsp[(2) - (2)].strval)) > MAX_CONN_NAME_LEN )
        {
            // error 처리

            ulpSetErrorCode( &gUlpParseInfo.mErrorMgr,
                             ulpERR_ABORT_COMP_Exceed_Max_Connname_Error,
                             (yyvsp[(2) - (2)].strval) );
            gUlpCOMPErrCode = ulpGetErrorSTATE( &gUlpParseInfo.mErrorMgr );
            COMPerror( ulpGetErrorMSG(&gUlpParseInfo.mErrorMgr) );
        }
        else
        {
            gUlpCodeGen.ulpGenEmSQL( GEN_CONNNAME, (void *) (yyvsp[(2) - (2)].strval) );
            gUlpCodeGen.ulpGenRemoveQueryToken( (yyvsp[(2) - (2)].strval) );
            gUlpCodeGen.ulpGenRemoveQueryToken( (yyvsp[(1) - (2)].strval) );
        }

    ;}
    break;

  case 291:

/* Line 1455 of yacc.c  */
#line 2989 "ulpCompy.y"
    {
        gUlpCodeGen.ulpGenEmSQL( GEN_CONNNAME, (void *) (yyvsp[(2) - (2)].strval) );
        gUlpCodeGen.ulpGenRemoveQueryToken( (yyvsp[(2) - (2)].strval) );
        gUlpCodeGen.ulpGenRemoveQueryToken( (yyvsp[(1) - (2)].strval) );
    ;}
    break;

  case 292:

/* Line 1455 of yacc.c  */
#line 3007 "ulpCompy.y"
    {
        gUlpStmttype = S_DeclareCur;
    ;}
    break;

  case 293:

/* Line 1455 of yacc.c  */
#line 3011 "ulpCompy.y"
    {
        gUlpStmttype = S_DeclareStmt;
        gUlpIsPrintStmt = ID_FALSE;
    ;}
    break;

  case 294:

/* Line 1455 of yacc.c  */
#line 3016 "ulpCompy.y"
    {
        gUlpStmttype = S_Open;
        gUlpIsPrintStmt = ID_FALSE;
    ;}
    break;

  case 295:

/* Line 1455 of yacc.c  */
#line 3021 "ulpCompy.y"
    {
        gUlpStmttype = S_Fetch;
        gUlpIsPrintStmt = ID_FALSE;
    ;}
    break;

  case 296:

/* Line 1455 of yacc.c  */
#line 3026 "ulpCompy.y"
    {
        gUlpIsPrintStmt = ID_FALSE;
    ;}
    break;

  case 297:

/* Line 1455 of yacc.c  */
#line 3030 "ulpCompy.y"
    {
        gUlpStmttype = S_AutoCommit;
        gUlpIsPrintStmt = ID_FALSE;
    ;}
    break;

  case 298:

/* Line 1455 of yacc.c  */
#line 3035 "ulpCompy.y"
    {
        gUlpStmttype = S_Connect;
        gUlpIsPrintStmt = ID_FALSE;
    ;}
    break;

  case 299:

/* Line 1455 of yacc.c  */
#line 3040 "ulpCompy.y"
    {
        gUlpStmttype = S_Disconnect;
        gUlpIsPrintStmt = ID_FALSE;
    ;}
    break;

  case 300:

/* Line 1455 of yacc.c  */
#line 3045 "ulpCompy.y"
    {
        gUlpStmttype = S_FreeLob;
        gUlpIsPrintStmt = ID_FALSE;
    ;}
    break;

  case 301:

/* Line 1455 of yacc.c  */
#line 3063 "ulpCompy.y"
    {
        /* BUG-40939 */
        SChar* sTmpQueryBuf;
        UInt   sCurNameLength = 0;

        sCurNameLength = idlOS::strlen( gUlpCodeGen.ulpGenGetEmSQLInfo()->mCurName );
        sTmpQueryBuf = idlOS::strstr( gUlpCodeGen.ulpGetQueryBuf(), 
                                      gUlpCodeGen.ulpGenGetEmSQLInfo()->mCurName ) + sCurNameLength;
        gUlpCodeGen.ulpGenEmSQL( GEN_QUERYSTR, 
                                 idlOS::strstr(sTmpQueryBuf, (yyvsp[(2) - (3)].strval)) );
        gUlpIsPrintStmt = ID_TRUE;
    ;}
    break;

  case 302:

/* Line 1455 of yacc.c  */
#line 3076 "ulpCompy.y"
    {
        gUlpCodeGen.ulpGenEmSQL( GEN_STMTNAME, (void *) (yyvsp[(2) - (3)].strval) );
        gUlpIsPrintStmt = ID_FALSE;
    ;}
    break;

  case 303:

/* Line 1455 of yacc.c  */
#line 3085 "ulpCompy.y"
    {
        if ( idlOS::strlen((yyvsp[(2) - (7)].strval)) >= MAX_CUR_NAME_LEN)
        {

            ulpSetErrorCode( &gUlpParseInfo.mErrorMgr,
                             ulpERR_ABORT_COMP_Exceed_Max_Cursorname_Error,
                             (yyvsp[(2) - (7)].strval) );
            gUlpCOMPErrCode = ulpGetErrorSTATE( &gUlpParseInfo.mErrorMgr );
            COMPerror( ulpGetErrorMSG(&gUlpParseInfo.mErrorMgr) );
        }

        gUlpCodeGen.ulpGenEmSQL( GEN_CURNAME, (void *) (yyvsp[(2) - (7)].strval) );
    ;}
    break;

  case 304:

/* Line 1455 of yacc.c  */
#line 3103 "ulpCompy.y"
    {
        UInt sValue = SQL_INSENSITIVE;
        gUlpCodeGen.ulpGenEmSQL( GEN_CURSORSENSITIVITY, (void *)&sValue );
  ;}
    break;

  case 305:

/* Line 1455 of yacc.c  */
#line 3108 "ulpCompy.y"
    {
        UInt sValue = SQL_SENSITIVE;
        gUlpCodeGen.ulpGenEmSQL( GEN_CURSORSENSITIVITY, (void *)&sValue );
  ;}
    break;

  case 306:

/* Line 1455 of yacc.c  */
#line 3113 "ulpCompy.y"
    {
        UInt sValue = SQL_INSENSITIVE;
        gUlpCodeGen.ulpGenEmSQL( GEN_CURSORSENSITIVITY, (void *)&sValue );
  ;}
    break;

  case 307:

/* Line 1455 of yacc.c  */
#line 3118 "ulpCompy.y"
    {
        UInt sValue = SQL_INSENSITIVE;
        gUlpCodeGen.ulpGenEmSQL( GEN_CURSORSENSITIVITY, (void *)&sValue );
  ;}
    break;

  case 308:

/* Line 1455 of yacc.c  */
#line 3127 "ulpCompy.y"
    {
        UInt sValue = SQL_NONSCROLLABLE;
        gUlpCodeGen.ulpGenEmSQL( GEN_CURSORSCROLLABLE, (void *) &sValue );
;}
    break;

  case 309:

/* Line 1455 of yacc.c  */
#line 3132 "ulpCompy.y"
    {
        UInt sValue = SQL_SCROLLABLE;
        gUlpCodeGen.ulpGenEmSQL( GEN_CURSORSCROLLABLE, (void *) &sValue );
        gUlpCodeGen.ulpGenEmSQL( GEN_SCROLLCUR, (void *) "1" );
    ;}
    break;

  case 310:

/* Line 1455 of yacc.c  */
#line 3142 "ulpCompy.y"
    {
      UInt sValue = SQL_CURSOR_HOLD_OFF;
      gUlpCodeGen.ulpGenEmSQL( GEN_CURSORWITHHOLD, (void *) &sValue );
  ;}
    break;

  case 311:

/* Line 1455 of yacc.c  */
#line 3147 "ulpCompy.y"
    {
      UInt sValue = SQL_CURSOR_HOLD_ON;
      gUlpCodeGen.ulpGenEmSQL( GEN_CURSORWITHHOLD, (void *) &sValue );
  ;}
    break;

  case 312:

/* Line 1455 of yacc.c  */
#line 3152 "ulpCompy.y"
    {
      ulpSetErrorCode( &gUlpParseInfo.mErrorMgr,
                       ulpERR_ABORT_COMP_Not_Supported_WITH_RETURN_Error );
      gUlpCOMPErrCode = ulpGetErrorSTATE( &gUlpParseInfo.mErrorMgr );
      COMPerror( ulpGetErrorMSG(&gUlpParseInfo.mErrorMgr) );
  ;}
    break;

  case 313:

/* Line 1455 of yacc.c  */
#line 3159 "ulpCompy.y"
    {
      ulpSetErrorCode( &gUlpParseInfo.mErrorMgr,
                       ulpERR_ABORT_COMP_Not_Supported_WITH_RETURN_Error );
      gUlpCOMPErrCode = ulpGetErrorSTATE( &gUlpParseInfo.mErrorMgr );
      COMPerror( ulpGetErrorMSG(&gUlpParseInfo.mErrorMgr) );
  ;}
    break;

  case 315:

/* Line 1455 of yacc.c  */
#line 3170 "ulpCompy.y"
    {

      ulpSetErrorCode( &gUlpParseInfo.mErrorMgr,
                       ulpERR_ABORT_COMP_Not_Supported_READ_ONLY_Error );
      gUlpCOMPErrCode = ulpGetErrorSTATE( &gUlpParseInfo.mErrorMgr );
      COMPerror( ulpGetErrorMSG(&gUlpParseInfo.mErrorMgr) );
  ;}
    break;

  case 323:

/* Line 1455 of yacc.c  */
#line 3198 "ulpCompy.y"
    {
        gUlpCodeGen.ulpGenEmSQL( GEN_STMTNAME, (void *) (yyvsp[(2) - (3)].strval) );
    ;}
    break;

  case 324:

/* Line 1455 of yacc.c  */
#line 3209 "ulpCompy.y"
    {
        gUlpCodeGen.ulpGenEmSQL( GEN_CURNAME, (void *) (yyvsp[(2) - (2)].strval) );
    ;}
    break;

  case 325:

/* Line 1455 of yacc.c  */
#line 3213 "ulpCompy.y"
    {
        gUlpCodeGen.ulpGenEmSQL( GEN_CURNAME, (void *) (yyvsp[(2) - (4)].strval) );
    ;}
    break;

  case 326:

/* Line 1455 of yacc.c  */
#line 3218 "ulpCompy.y"
    {
        gUlpCodeGen.ulpGenEmSQL( GEN_CURNAME, (void *) (yyvsp[(2) - (4)].strval) );
    ;}
    break;

  case 327:

/* Line 1455 of yacc.c  */
#line 3229 "ulpCompy.y"
    {
        gUlpCodeGen.ulpGenEmSQL( GEN_CURNAME, (void *) (yyvsp[(3) - (5)].strval) );
    ;}
    break;

  case 328:

/* Line 1455 of yacc.c  */
#line 3234 "ulpCompy.y"
    {
        gUlpCodeGen.ulpGenEmSQL( GEN_CURNAME, (void *) (yyvsp[(4) - (6)].strval) );
    ;}
    break;

  case 329:

/* Line 1455 of yacc.c  */
#line 3239 "ulpCompy.y"
    {
        gUlpCodeGen.ulpGenEmSQL( GEN_CURNAME, (void *) (yyvsp[(3) - (6)].strval) );
    ;}
    break;

  case 334:

/* Line 1455 of yacc.c  */
#line 3253 "ulpCompy.y"
    {
        gUlpCodeGen.ulpGenEmSQL( GEN_SCROLLCUR, (void *) "1" /*F_FIRST*/ );
    ;}
    break;

  case 335:

/* Line 1455 of yacc.c  */
#line 3257 "ulpCompy.y"
    {
        gUlpCodeGen.ulpGenEmSQL( GEN_SCROLLCUR, (void *) "2" /*F_PRIOR*/ );
    ;}
    break;

  case 336:

/* Line 1455 of yacc.c  */
#line 3261 "ulpCompy.y"
    {
        gUlpCodeGen.ulpGenEmSQL( GEN_SCROLLCUR, (void *) "3" /*F_NEXT*/ );
    ;}
    break;

  case 337:

/* Line 1455 of yacc.c  */
#line 3265 "ulpCompy.y"
    {
        gUlpCodeGen.ulpGenEmSQL( GEN_SCROLLCUR, (void *) "4" /*F_LAST*/ );
    ;}
    break;

  case 338:

/* Line 1455 of yacc.c  */
#line 3269 "ulpCompy.y"
    {
        gUlpCodeGen.ulpGenEmSQL( GEN_SCROLLCUR, (void *) "5" /*F_CURRENT*/ );
    ;}
    break;

  case 339:

/* Line 1455 of yacc.c  */
#line 3273 "ulpCompy.y"
    {
        gUlpCodeGen.ulpGenEmSQL( GEN_SCROLLCUR, (void *) "6" /*F_RELATIVE*/ );
    ;}
    break;

  case 340:

/* Line 1455 of yacc.c  */
#line 3277 "ulpCompy.y"
    {
        gUlpCodeGen.ulpGenEmSQL( GEN_SCROLLCUR, (void *) "7" /*F_ABSOLUTE*/ );
    ;}
    break;

  case 341:

/* Line 1455 of yacc.c  */
#line 3284 "ulpCompy.y"
    {
        gUlpCodeGen.ulpGenEmSQL( GEN_SQLINFO, (void *) (yyvsp[(1) - (1)].strval) );
    ;}
    break;

  case 342:

/* Line 1455 of yacc.c  */
#line 3288 "ulpCompy.y"
    {
        gUlpCodeGen.ulpGenEmSQL( GEN_SQLINFO, (void *) (yyvsp[(2) - (2)].strval) );
    ;}
    break;

  case 343:

/* Line 1455 of yacc.c  */
#line 3292 "ulpCompy.y"
    {
        SChar sTmpStr[MAX_NUMBER_LEN];
        idlOS::snprintf( sTmpStr, MAX_NUMBER_LEN ,"-%s", (yyvsp[(2) - (2)].strval) );
        gUlpCodeGen.ulpGenEmSQL( GEN_SQLINFO, (void *) sTmpStr );
    ;}
    break;

  case 344:

/* Line 1455 of yacc.c  */
#line 3306 "ulpCompy.y"
    {
        gUlpStmttype = S_CloseRel;
        gUlpCodeGen.ulpGenEmSQL( GEN_CURNAME, (void *) (yyvsp[(3) - (3)].strval) );
    ;}
    break;

  case 345:

/* Line 1455 of yacc.c  */
#line 3311 "ulpCompy.y"
    {
        gUlpStmttype = S_Close;
        gUlpCodeGen.ulpGenEmSQL( GEN_CURNAME, (void *) (yyvsp[(2) - (2)].strval) );
    ;}
    break;

  case 346:

/* Line 1455 of yacc.c  */
#line 3323 "ulpCompy.y"
    {
        gUlpCodeGen.ulpGenEmSQL( GEN_SQLINFO, (void *) "1" /*ON*/);
    ;}
    break;

  case 347:

/* Line 1455 of yacc.c  */
#line 3327 "ulpCompy.y"
    {
        gUlpCodeGen.ulpGenEmSQL( GEN_SQLINFO, (void *) "0" /*OFF*/);
    ;}
    break;

  case 348:

/* Line 1455 of yacc.c  */
#line 3338 "ulpCompy.y"
    {
        ulpSymTElement *sSymNode;

        // User name
        if ( ( sSymNode = gUlpScopeT.ulpSLookupAll( (yyvsp[(2) - (5)].strval)+1, gUlpCurDepth ) ) == NULL )
        {
            //host 변수 못찾는 error
        }

        gUlpCodeGen.ulpGenAddHostVarList( (yyvsp[(2) - (5)].strval)+1, sSymNode ,
                                          gUlpIndName, NULL, NULL, HV_IN_TYPE);

        // Password name
        if ( ( sSymNode = gUlpScopeT.ulpSLookupAll( (yyvsp[(5) - (5)].strval)+1, gUlpCurDepth ) ) == NULL )
        {
            //host 변수 못찾는 error
        }

        gUlpCodeGen.ulpGenAddHostVarList( (yyvsp[(5) - (5)].strval)+1, sSymNode ,
                                          gUlpIndName, NULL, NULL, HV_IN_TYPE);

        gUlpCodeGen.ulpIncHostVarNum( 2 );
    ;}
    break;

  case 349:

/* Line 1455 of yacc.c  */
#line 3362 "ulpCompy.y"
    {
        ulpSymTElement *sSymNode;

        // User name
        if ( ( sSymNode = gUlpScopeT.ulpSLookupAll( (yyvsp[(2) - (7)].strval)+1, gUlpCurDepth ) ) == NULL )
        {
            //host 변수 못찾는 error
        }

        gUlpCodeGen.ulpGenAddHostVarList( (yyvsp[(2) - (7)].strval)+1, sSymNode ,
                                          gUlpIndName, NULL, NULL, HV_IN_TYPE);

        // Password name
        if ( ( sSymNode = gUlpScopeT.ulpSLookupAll( (yyvsp[(5) - (7)].strval)+1, gUlpCurDepth ) ) == NULL )
        {
            //host 변수 못찾는 error
        }

        gUlpCodeGen.ulpGenAddHostVarList( (yyvsp[(5) - (7)].strval)+1, sSymNode ,
                                          gUlpIndName, NULL, NULL, HV_IN_TYPE);

        /* using open :drivername */
        if ( ( sSymNode = gUlpScopeT.ulpSLookupAll( (yyvsp[(7) - (7)].strval)+1, gUlpCurDepth ) ) == NULL )
        {
            //host 변수 못찾는 error
        }

        // driver name이라고 표시함.
        //sSymNode -> mMoreInfo = 1;
        //gUlpCodeGen.ulpGenAddHostVarList( sSymNode );

        gUlpCodeGen.ulpIncHostVarNum( 2 );
    ;}
    break;

  case 350:

/* Line 1455 of yacc.c  */
#line 3396 "ulpCompy.y"
    {
        ulpSymTElement *sSymNode;

        // User name
        if ( ( sSymNode = gUlpScopeT.ulpSLookupAll( (yyvsp[(2) - (7)].strval)+1, gUlpCurDepth ) ) == NULL )
        {
            //host 변수 못찾는 error
        }

        gUlpCodeGen.ulpGenAddHostVarList( (yyvsp[(2) - (7)].strval)+1, sSymNode ,
                                          gUlpIndName, NULL, NULL, HV_IN_TYPE);

        // Password name
        if ( ( sSymNode = gUlpScopeT.ulpSLookupAll( (yyvsp[(5) - (7)].strval)+1, gUlpCurDepth ) ) == NULL )
        {
            //host 변수 못찾는 error
        }

        gUlpCodeGen.ulpGenAddHostVarList( (yyvsp[(5) - (7)].strval)+1, sSymNode ,
                                          gUlpIndName, NULL, NULL, HV_IN_TYPE);


        /* using :conn_opt1 */
        if ( ( sSymNode = gUlpScopeT.ulpSLookupAll( (yyvsp[(7) - (7)].strval)+1, gUlpCurDepth ) ) == NULL )
        {
            //host 변수 못찾는 error
        }

        // driver name이라고 표시함.
        gUlpCodeGen.ulpGenAddHostVarList( (yyvsp[(7) - (7)].strval)+1, sSymNode ,
                                          gUlpIndName, NULL, NULL, HV_IN_TYPE);

        gUlpCodeGen.ulpIncHostVarNum( 3 );
    ;}
    break;

  case 351:

/* Line 1455 of yacc.c  */
#line 3431 "ulpCompy.y"
    {
        ulpSymTElement *sSymNode;

        // User name
        if ( ( sSymNode = gUlpScopeT.ulpSLookupAll( (yyvsp[(2) - (9)].strval)+1, gUlpCurDepth ) ) == NULL )
        {
            //host 변수 못찾는 error
        }

        gUlpCodeGen.ulpGenAddHostVarList( (yyvsp[(2) - (9)].strval)+1, sSymNode ,
                                          gUlpIndName, NULL, NULL, HV_IN_TYPE);

        // Password name
        if ( ( sSymNode = gUlpScopeT.ulpSLookupAll( (yyvsp[(5) - (9)].strval)+1, gUlpCurDepth ) ) == NULL )
        {
            //host 변수 못찾는 error
        }

        gUlpCodeGen.ulpGenAddHostVarList( (yyvsp[(5) - (9)].strval)+1, sSymNode ,
                                          gUlpIndName, NULL, NULL, HV_IN_TYPE);

        /* using :conn_opt1, :conn_opt2 */
        if ( ( sSymNode = gUlpScopeT.ulpSLookupAll( (yyvsp[(7) - (9)].strval)+1, gUlpCurDepth ) ) == NULL )
        {
            //host 변수 못찾는 error
        }

        gUlpCodeGen.ulpGenAddHostVarList( (yyvsp[(7) - (9)].strval)+1, sSymNode ,
                                          gUlpIndName, NULL, NULL, HV_IN_TYPE);


        if ( ( sSymNode = gUlpScopeT.ulpSLookupAll( (yyvsp[(9) - (9)].strval)+1, gUlpCurDepth ) ) == NULL )
        {
            //host 변수 못찾는 error
        }

        gUlpCodeGen.ulpGenAddHostVarList( (yyvsp[(9) - (9)].strval)+1, sSymNode ,
                                          gUlpIndName, NULL, NULL, HV_IN_TYPE);

        gUlpCodeGen.ulpIncHostVarNum( 4 );
    ;}
    break;

  case 352:

/* Line 1455 of yacc.c  */
#line 3473 "ulpCompy.y"
    {
        ulpSymTElement *sSymNode;

        // User name
        if ( ( sSymNode = gUlpScopeT.ulpSLookupAll( (yyvsp[(2) - (11)].strval)+1, gUlpCurDepth ) ) == NULL )
        {
            //host 변수 못찾는 error
        }

        gUlpCodeGen.ulpGenAddHostVarList( (yyvsp[(2) - (11)].strval)+1, sSymNode ,
                                          gUlpIndName, NULL, NULL, HV_IN_TYPE);

        // Password name
        if ( ( sSymNode = gUlpScopeT.ulpSLookupAll( (yyvsp[(5) - (11)].strval)+1, gUlpCurDepth ) ) == NULL )
        {
            //host 변수 못찾는 error
        }

        gUlpCodeGen.ulpGenAddHostVarList( (yyvsp[(5) - (11)].strval)+1, sSymNode ,
                                          gUlpIndName, NULL, NULL, HV_IN_TYPE);

        /* using :conn_opt1, :conn_opt2 open :drivername */
        if ( ( sSymNode = gUlpScopeT.ulpSLookupAll( (yyvsp[(7) - (11)].strval)+1, gUlpCurDepth ) ) == NULL )
        {
            //host 변수 못찾는 error
        }

        gUlpCodeGen.ulpGenAddHostVarList( (yyvsp[(7) - (11)].strval)+1, sSymNode ,
                                          gUlpIndName, NULL, NULL, HV_IN_TYPE);

        if ( ( sSymNode = gUlpScopeT.ulpSLookupAll( (yyvsp[(9) - (11)].strval)+1, gUlpCurDepth ) ) == NULL )
        {
            //host 변수 못찾는 error
        }

        gUlpCodeGen.ulpGenAddHostVarList( (yyvsp[(9) - (11)].strval)+1, sSymNode ,
                                          gUlpIndName, NULL, NULL, HV_IN_TYPE);

        if ( ( sSymNode = gUlpScopeT.ulpSLookupAll( (yyvsp[(11) - (11)].strval)+1, gUlpCurDepth ) ) == NULL )
        {
            //host 변수 못찾는 error
        }

        // driver name이라고 표시함.
        //sSymNode -> mMoreInfo = 1;
        //gUlpCodeGen.ulpGenAddHostVarList( sSymNode );

        gUlpCodeGen.ulpIncHostVarNum( 4 );
    ;}
    break;

  case 353:

/* Line 1455 of yacc.c  */
#line 3523 "ulpCompy.y"
    {
        ulpSymTElement *sSymNode;

        // User name
        if ( ( sSymNode = gUlpScopeT.ulpSLookupAll( (yyvsp[(2) - (9)].strval)+1, gUlpCurDepth ) ) == NULL )
        {
            //host 변수 못찾는 error
        }

        gUlpCodeGen.ulpGenAddHostVarList( (yyvsp[(2) - (9)].strval)+1, sSymNode , gUlpIndName,
                                          NULL, NULL, HV_IN_TYPE);

        // Password name
        if ( ( sSymNode = gUlpScopeT.ulpSLookupAll( (yyvsp[(5) - (9)].strval)+1, gUlpCurDepth ) ) == NULL )
        {
            //host 변수 못찾는 error
        }

        gUlpCodeGen.ulpGenAddHostVarList( (yyvsp[(5) - (9)].strval)+1, sSymNode , gUlpIndName, NULL,
                                          NULL, HV_IN_TYPE);

        /* using :conn_opt1 open :drivername */
        if ( ( sSymNode = gUlpScopeT.ulpSLookupAll( (yyvsp[(7) - (9)].strval)+1, gUlpCurDepth ) ) == NULL )
        {
            //host 변수 못찾는 error
        }

        gUlpCodeGen.ulpGenAddHostVarList( (yyvsp[(7) - (9)].strval)+1, sSymNode , gUlpIndName, NULL,
                                          NULL, HV_IN_TYPE);

        if ( ( sSymNode = gUlpScopeT.ulpSLookupAll( (yyvsp[(9) - (9)].strval)+1, gUlpCurDepth ) ) == NULL )
        {
            //host 변수 못찾는 error
        }

        // driver name이라고 표시함.
        //sSymNode -> mMoreInfo = 1;
        //gUlpCodeGen.ulpGenAddHostVarList( sSymNode );

        gUlpCodeGen.ulpIncHostVarNum( 3 );
    ;}
    break;

  case 355:

/* Line 1455 of yacc.c  */
#line 3580 "ulpCompy.y"
    {

    ;}
    break;

  case 356:

/* Line 1455 of yacc.c  */
#line 3584 "ulpCompy.y"
    {

    ;}
    break;

  case 357:

/* Line 1455 of yacc.c  */
#line 3598 "ulpCompy.y"
    {
        gUlpStmttype    = S_IGNORE;
        gUlpIsPrintStmt = ID_FALSE;
    ;}
    break;

  case 358:

/* Line 1455 of yacc.c  */
#line 3603 "ulpCompy.y"
    {
        gUlpStmttype    = S_IGNORE;
        gUlpIsPrintStmt = ID_FALSE;
    ;}
    break;

  case 359:

/* Line 1455 of yacc.c  */
#line 3608 "ulpCompy.y"
    {
        gUlpStmttype = S_Prepare;
        gUlpIsPrintStmt = ID_TRUE;
    ;}
    break;

  case 360:

/* Line 1455 of yacc.c  */
#line 3613 "ulpCompy.y"
    {
        gUlpStmttype    = S_IGNORE;
        gUlpIsPrintStmt = ID_FALSE;
    ;}
    break;

  case 361:

/* Line 1455 of yacc.c  */
#line 3618 "ulpCompy.y"
    {
        gUlpStmttype    = S_ExecIm;
        gUlpIsPrintStmt = ID_TRUE;
    ;}
    break;

  case 362:

/* Line 1455 of yacc.c  */
#line 3623 "ulpCompy.y"
    {
        gUlpStmttype    = S_ExecDML;
        gUlpIsPrintStmt = ID_FALSE;
    ;}
    break;

  case 363:

/* Line 1455 of yacc.c  */
#line 3629 "ulpCompy.y"
    {
        gUlpStmttype    = S_BindVariables;
        gUlpIsPrintStmt = ID_FALSE;
    ;}
    break;

  case 364:

/* Line 1455 of yacc.c  */
#line 3634 "ulpCompy.y"
    {
        gUlpStmttype    = S_SetArraySize;
        gUlpIsPrintStmt = ID_FALSE;
    ;}
    break;

  case 365:

/* Line 1455 of yacc.c  */
#line 3639 "ulpCompy.y"
    {
        gUlpStmttype    = S_SelectList;
        gUlpIsPrintStmt = ID_FALSE;
    ;}
    break;

  case 369:

/* Line 1455 of yacc.c  */
#line 3653 "ulpCompy.y"
    {
      if(idlOS::strncasecmp("MAX", (yyvsp[(2) - (3)].strval), 3) != 0)
      {
          // error 처리

          ulpSetErrorCode( &gUlpParseInfo.mErrorMgr,
                           ulpERR_ABORT_COMP_Unterminated_String_Error );
          gUlpCOMPErrCode = ulpGetErrorSTATE( &gUlpParseInfo.mErrorMgr );
          COMPerror( ulpGetErrorMSG(&gUlpParseInfo.mErrorMgr) );
      }
  ;}
    break;

  case 372:

/* Line 1455 of yacc.c  */
#line 3673 "ulpCompy.y"
    {
        SChar sTmpStr[MAX_HOSTVAR_NAME_SIZE];
        ulpSymTElement *sSymNode;

        if ( (sSymNode = gUlpScopeT.ulpSLookupAll( (yyvsp[(2) - (2)].strval)+1, gUlpCurDepth )) == NULL )
        {
            //host 변수 못찾는 error
        }

        if( sSymNode != NULL )
        {
            if ( (sSymNode->mType == H_VARCHAR) ||
                 (sSymNode->mType == H_NVARCHAR) )
            {
                idlOS::snprintf( sTmpStr, MAX_HOSTVAR_NAME_SIZE,
                                "%s.arr",
                                (yyvsp[(2) - (2)].strval)+1 );
                gUlpCodeGen.ulpGenEmSQL( GEN_QUERYHV, (void *) sTmpStr );
            }
            else
            {
                gUlpCodeGen.ulpGenEmSQL( GEN_QUERYHV, (yyvsp[(2) - (2)].strval)+1 );
            }
        }
        else
        {
            gUlpCodeGen.ulpGenEmSQL( GEN_QUERYHV, (yyvsp[(2) - (2)].strval)+1 );
        }
    ;}
    break;

  case 373:

/* Line 1455 of yacc.c  */
#line 3703 "ulpCompy.y"
    {
        /* BUG-40939 */
        SChar* sTmpQueryBuf;
        UInt   sStmtNameLength = 0;
        sStmtNameLength = idlOS::strlen( gUlpCodeGen.ulpGenGetEmSQLInfo()->mStmtName );
        sTmpQueryBuf = idlOS::strstr( gUlpCodeGen.ulpGetQueryBuf(),
                                      gUlpCodeGen.ulpGenGetEmSQLInfo()->mStmtName ) + sStmtNameLength;
                                      
        gUlpCodeGen.ulpGenEmSQL( GEN_QUERYSTR,
                                 idlOS::strstr( sTmpQueryBuf, (yyvsp[(2) - (2)].strval)) );
    ;}
    break;

  case 374:

/* Line 1455 of yacc.c  */
#line 3715 "ulpCompy.y"
    {
        /* BUG-40939 */
        SChar* sTmpQueryBuf;
        SInt   sStmtNameLength = 0;
        sStmtNameLength = idlOS::strlen( gUlpCodeGen.ulpGenGetEmSQLInfo()->mStmtName );
        sTmpQueryBuf = idlOS::strstr( gUlpCodeGen.ulpGetQueryBuf(),
                                      gUlpCodeGen.ulpGenGetEmSQLInfo()->mStmtName ) + sStmtNameLength;
        gUlpCodeGen.ulpGenEmSQL( GEN_QUERYSTR,
                                 idlOS::strstr( sTmpQueryBuf, (yyvsp[(2) - (2)].strval)) );
    ;}
    break;

  case 375:

/* Line 1455 of yacc.c  */
#line 3729 "ulpCompy.y"
    {
        if ( idlOS::strlen((yyvsp[(2) - (3)].strval)) >= MAX_STMT_NAME_LEN )
        {
            // error 처리

            ulpSetErrorCode( &gUlpParseInfo.mErrorMgr,
                             ulpERR_ABORT_COMP_Exceed_Max_Stmtname_Error,
                             (yyvsp[(2) - (3)].strval) );
            gUlpCOMPErrCode = ulpGetErrorSTATE( &gUlpParseInfo.mErrorMgr );
            COMPerror( ulpGetErrorMSG(&gUlpParseInfo.mErrorMgr) );
        }
        gUlpCodeGen.ulpGenEmSQL( GEN_STMTNAME, (void *) (yyvsp[(2) - (3)].strval) );
    ;}
    break;

  case 378:

/* Line 1455 of yacc.c  */
#line 3768 "ulpCompy.y"
    {
        SChar sTmpStr[MAX_HOSTVAR_NAME_SIZE];
        ulpSymTElement *sSymNode;

        if ( (sSymNode = gUlpScopeT.ulpSLookupAll( (yyvsp[(2) - (2)].strval)+1, gUlpCurDepth )) == NULL )
        {
            //don't report error
        }

        if ( sSymNode != NULL )
        {
            if ( (sSymNode->mType == H_VARCHAR) ||
                 (sSymNode->mType == H_NVARCHAR) )
            {
                idlOS::snprintf( sTmpStr, MAX_HOSTVAR_NAME_SIZE,
                                 "%s.arr",
                                 (yyvsp[(2) - (2)].strval)+1 );
                gUlpCodeGen.ulpGenEmSQL( GEN_QUERYHV, (void *) sTmpStr );
            }
            else
            {
                gUlpCodeGen.ulpGenEmSQL( GEN_QUERYHV, (yyvsp[(2) - (2)].strval)+1 );
            }
        }
        else
        {
            gUlpCodeGen.ulpGenEmSQL( GEN_QUERYHV, (yyvsp[(2) - (2)].strval)+1 );
        }
    ;}
    break;

  case 379:

/* Line 1455 of yacc.c  */
#line 3798 "ulpCompy.y"
    {
        gUlpCodeGen.ulpGenEmSQL( GEN_QUERYSTR,
                                 idlOS::strstr( gUlpCodeGen.ulpGetQueryBuf(),
                                                (yyvsp[(2) - (2)].strval))
                               );
    ;}
    break;

  case 380:

/* Line 1455 of yacc.c  */
#line 3805 "ulpCompy.y"
    {
        gUlpCodeGen.ulpGenEmSQL( GEN_QUERYSTR,
                                 idlOS::strstr( gUlpCodeGen.ulpGetQueryBuf(),
                                                (yyvsp[(2) - (2)].strval))
                               );
    ;}
    break;

  case 382:

/* Line 1455 of yacc.c  */
#line 3819 "ulpCompy.y"
    {
      gUlpCodeGen.ulpGenEmSQL( GEN_STMTNAME, (void *) (yyvsp[(2) - (2)].strval) );
  ;}
    break;

  case 383:

/* Line 1455 of yacc.c  */
#line 3823 "ulpCompy.y"
    {
      gUlpCodeGen.ulpGenEmSQL( GEN_STMTNAME, (void *) (yyvsp[(3) - (5)].strval) );
  ;}
    break;

  case 384:

/* Line 1455 of yacc.c  */
#line 3827 "ulpCompy.y"
    {
      gUlpCodeGen.ulpGenEmSQL( GEN_STMTNAME, (void *) (yyvsp[(2) - (4)].strval) );
  ;}
    break;

  case 385:

/* Line 1455 of yacc.c  */
#line 3831 "ulpCompy.y"
    {
      gUlpCodeGen.ulpGenEmSQL( GEN_STMTNAME, (void *) (yyvsp[(2) - (4)].strval) );
  ;}
    break;

  case 386:

/* Line 1455 of yacc.c  */
#line 3839 "ulpCompy.y"
    {
      gUlpCodeGen.ulpGenEmSQL( GEN_STMTNAME, (void *) (yyvsp[(5) - (7)].strval) );
  ;}
    break;

  case 387:

/* Line 1455 of yacc.c  */
#line 3846 "ulpCompy.y"
    {
      gUlpCodeGen.ulpGenEmSQL( GEN_STMTNAME, (void *) (yyvsp[(2) - (3)].strval) );
  ;}
    break;

  case 388:

/* Line 1455 of yacc.c  */
#line 3854 "ulpCompy.y"
    {
      if(idlOS::strncasecmp("LIST", (yyvsp[(3) - (7)].strval), 4) != 0)
      {
          // error 처리
          ulpSetErrorCode( &gUlpParseInfo.mErrorMgr,
                           ulpERR_ABORT_COMP_Unterminated_String_Error );
          gUlpCOMPErrCode = ulpGetErrorSTATE( &gUlpParseInfo.mErrorMgr );
          COMPerror( ulpGetErrorMSG(&gUlpParseInfo.mErrorMgr) );
      }
      else
      {
          gUlpCodeGen.ulpGenEmSQL( GEN_STMTNAME, (void *) (yyvsp[(5) - (7)].strval) );
      }
  ;}
    break;

  case 389:

/* Line 1455 of yacc.c  */
#line 3882 "ulpCompy.y"
    {
        gUlpStmttype    = S_DirectOTH;
        gUlpIsPrintStmt = ID_TRUE;
        gUlpCodeGen.ulpGenCutQueryTail4PSM( ';' );

    ;}
    break;

  case 390:

/* Line 1455 of yacc.c  */
#line 3889 "ulpCompy.y"
    {
        gUlpStmttype    = S_DirectOTH;
        gUlpIsPrintStmt = ID_TRUE;
        gUlpCodeGen.ulpGenCutQueryTail4PSM( ';' );
    ;}
    break;

  case 391:

/* Line 1455 of yacc.c  */
#line 3895 "ulpCompy.y"
    {
        gUlpStmttype    = S_DirectOTH;
        gUlpIsPrintStmt = ID_TRUE;
        gUlpCodeGen.ulpGenCutQueryTail4PSM( ';' );
    ;}
    break;

  case 392:

/* Line 1455 of yacc.c  */
#line 3901 "ulpCompy.y"
    {
        idBool sTrue;
        sTrue = ID_TRUE;
        gUlpStmttype    = S_DirectPSM;
        gUlpIsPrintStmt = ID_TRUE;
        gUlpCodeGen.ulpGenEmSQL( GEN_QUERYSTR,
                                 idlOS::strstr( gUlpCodeGen.ulpGetQueryBuf(),
                                                (yyvsp[(3) - (6)].strval))
                               );
        gUlpCodeGen.ulpGenCutQueryTail4PSM( ';' );
        gUlpCodeGen.ulpGenCutQueryTail4PSM( ';' );
        gUlpCodeGen.ulpGenEmSQL( GEN_PSMEXEC, (void *) &sTrue );
    ;}
    break;

  case 393:

/* Line 1455 of yacc.c  */
#line 3915 "ulpCompy.y"
    {
        idBool sTrue;
        sTrue = ID_TRUE;
        gUlpStmttype    = S_DirectPSM;
        gUlpIsPrintStmt = ID_TRUE;
        gUlpCodeGen.ulpGenEmSQL( GEN_QUERYSTR,
                                 idlOS::strstr( gUlpCodeGen.ulpGetQueryBuf(),
                                                (yyvsp[(3) - (6)].strval))
                               );
        gUlpCodeGen.ulpGenCutQueryTail4PSM( ';' );
        gUlpCodeGen.ulpGenCutQueryTail4PSM( ';' );
        gUlpCodeGen.ulpGenEmSQL( GEN_PSMEXEC, (void *) &sTrue );
    ;}
    break;

  case 395:

/* Line 1455 of yacc.c  */
#line 3943 "ulpCompy.y"
    {
        gUlpStmttype    = S_Free;
        gUlpIsPrintStmt = ID_FALSE;
    ;}
    break;

  case 396:

/* Line 1455 of yacc.c  */
#line 3948 "ulpCompy.y"
    {
        gUlpStmttype    = S_Batch;
        gUlpIsPrintStmt = ID_FALSE;
    ;}
    break;

  case 397:

/* Line 1455 of yacc.c  */
#line 3953 "ulpCompy.y"
    {
        gUlpStmttype    = S_Batch;
        gUlpIsPrintStmt = ID_FALSE;
    ;}
    break;

  case 398:

/* Line 1455 of yacc.c  */
#line 3959 "ulpCompy.y"
    {
        gUlpStmttype = S_AutoCommit;
        gUlpIsPrintStmt = ID_FALSE;
        gUlpCodeGen.ulpGenEmSQL( GEN_SQLINFO, (void *) "1" /*ON*/);
    ;}
    break;

  case 399:

/* Line 1455 of yacc.c  */
#line 3966 "ulpCompy.y"
    {
        gUlpStmttype = S_AutoCommit;
        gUlpIsPrintStmt = ID_FALSE;
        gUlpCodeGen.ulpGenEmSQL( GEN_SQLINFO, (void *) "0" /*OFF*/);
    ;}
    break;

  case 400:

/* Line 1455 of yacc.c  */
#line 3973 "ulpCompy.y"
    {
        if(idlOS::strcasecmp("DEFAULT_DATE_FORMAT", (yyvsp[(4) - (6)].strval)) != 0 &&
           idlOS::strcasecmp("DATE_FORMAT", (yyvsp[(4) - (6)].strval)) != 0)
        {
            // error 처리

            ulpSetErrorCode( &gUlpParseInfo.mErrorMgr,
                             ulpERR_ABORT_COMP_Unterminated_String_Error );
            gUlpCOMPErrCode = ulpGetErrorSTATE( &gUlpParseInfo.mErrorMgr );
            COMPerror( ulpGetErrorMSG(&gUlpParseInfo.mErrorMgr) );
        }

        gUlpStmttype = S_AlterSession;
        gUlpIsPrintStmt = ID_FALSE;
        gUlpCodeGen.ulpGenEmSQL( GEN_EXTRASTRINFO, (yyvsp[(6) - (6)].strval) );
    ;}
    break;

  case 401:

/* Line 1455 of yacc.c  */
#line 3990 "ulpCompy.y"
    {
        ulpSymTElement *sSymNode;

        // failover var. name
        if ( ( sSymNode = gUlpScopeT.ulpSLookupAll( (yyvsp[(3) - (3)].strval)+1, gUlpCurDepth ) ) == NULL )
        {
            // error 처리.
        }
        else
        {
            gUlpCodeGen.ulpGenAddHostVarList( (yyvsp[(3) - (3)].strval)+1, sSymNode , gUlpIndName, NULL,
                                              NULL, HV_IN_TYPE);
        }

        gUlpCodeGen.ulpIncHostVarNum( 1 );

        gUlpCodeGen.ulpGenEmSQL( GEN_SQLINFO, (void *) "1" /*SET*/);
        gUlpStmttype = S_FailOver;
        gUlpIsPrintStmt = ID_FALSE;
    ;}
    break;

  case 402:

/* Line 1455 of yacc.c  */
#line 4011 "ulpCompy.y"
    {
        gUlpCodeGen.ulpGenEmSQL( GEN_SQLINFO, (void *) "0" /*UNSET*/);
        gUlpStmttype = S_FailOver;
        gUlpIsPrintStmt = ID_FALSE;
    ;}
    break;

  case 403:

/* Line 1455 of yacc.c  */
#line 4027 "ulpCompy.y"
    {
        idBool sTrue = ID_TRUE;
        gUlpCodeGen.ulpGenComment( gUlpCodeGen.ulpGetQueryBuf() );
        gUlpCodeGen.ulpGenEmSQL( GEN_MT, (void *)&sTrue );
    ;}
    break;

  case 404:

/* Line 1455 of yacc.c  */
#line 4033 "ulpCompy.y"
    {
        idBool sFalse = ID_FALSE;
        gUlpCodeGen.ulpGenComment( gUlpCodeGen.ulpGetQueryBuf() );
        gUlpCodeGen.ulpGenEmSQL( GEN_MT, (void *)&sFalse );
  ;}
    break;

  case 405:

/* Line 1455 of yacc.c  */
#line 4049 "ulpCompy.y"
    {
        gUlpCodeGen.ulpGenSetWhenever( gUlpCurDepth,
                                       GEN_WHEN_SQLERROR,
                                       GEN_WHEN_CONT,
                                       NULL );
    ;}
    break;

  case 406:

/* Line 1455 of yacc.c  */
#line 4056 "ulpCompy.y"
    {
        gUlpCodeGen.ulpGenSetWhenever( gUlpCurDepth,
                                       GEN_WHEN_SQLERROR,
                                       GEN_WHEN_GOTO,
                                       (yyvsp[(4) - (4)].strval) );
    ;}
    break;

  case 407:

/* Line 1455 of yacc.c  */
#line 4063 "ulpCompy.y"
    {
        gUlpCodeGen.ulpGenSetWhenever( gUlpCurDepth,
                                       GEN_WHEN_SQLERROR,
                                       GEN_WHEN_DO_BREAK,
                                       NULL );
    ;}
    break;

  case 408:

/* Line 1455 of yacc.c  */
#line 4070 "ulpCompy.y"
    {
        gUlpCodeGen.ulpGenSetWhenever( gUlpCurDepth,
                                       GEN_WHEN_SQLERROR,
                                       GEN_WHEN_DO_CONT,
                                       NULL );
    ;}
    break;

  case 409:

/* Line 1455 of yacc.c  */
#line 4077 "ulpCompy.y"
    {
        SChar  sTmpStr[MAX_EXPR_LEN];

        idlOS::snprintf( sTmpStr, MAX_EXPR_LEN , "%s(", (yyvsp[(4) - (5)].strval) );
        ulpCOMPWheneverFunc( sTmpStr );
        gUlpCodeGen.ulpGenSetWhenever( gUlpCurDepth,
                                       GEN_WHEN_SQLERROR,
                                       GEN_WHEN_DO_FUNC,
                                       sTmpStr );
    ;}
    break;

  case 410:

/* Line 1455 of yacc.c  */
#line 4088 "ulpCompy.y"
    {
        gUlpCodeGen.ulpGenSetWhenever( gUlpCurDepth,
                                       GEN_WHEN_SQLERROR,
                                       GEN_WHEN_STOP,
                                       NULL );
    ;}
    break;

  case 411:

/* Line 1455 of yacc.c  */
#line 4095 "ulpCompy.y"
    {
        gUlpCodeGen.ulpGenSetWhenever( gUlpCurDepth,
                                       GEN_WHEN_NOTFOUND,
                                       GEN_WHEN_CONT,
                                       NULL );
    ;}
    break;

  case 412:

/* Line 1455 of yacc.c  */
#line 4102 "ulpCompy.y"
    {
        gUlpCodeGen.ulpGenSetWhenever( gUlpCurDepth,
                                       GEN_WHEN_NOTFOUND,
                                       GEN_WHEN_GOTO,
                                       (yyvsp[(5) - (5)].strval) );
    ;}
    break;

  case 413:

/* Line 1455 of yacc.c  */
#line 4109 "ulpCompy.y"
    {
        gUlpCodeGen.ulpGenSetWhenever( gUlpCurDepth,
                                       GEN_WHEN_NOTFOUND,
                                       GEN_WHEN_DO_BREAK,
                                       NULL );
    ;}
    break;

  case 414:

/* Line 1455 of yacc.c  */
#line 4116 "ulpCompy.y"
    {
        gUlpCodeGen.ulpGenSetWhenever( gUlpCurDepth, 
                                       GEN_WHEN_NOTFOUND,
                                       GEN_WHEN_DO_CONT,
                                       NULL );
    ;}
    break;

  case 415:

/* Line 1455 of yacc.c  */
#line 4123 "ulpCompy.y"
    {
        SChar  sTmpStr[MAX_EXPR_LEN];

        idlOS::snprintf( sTmpStr, MAX_EXPR_LEN , "%s(", (yyvsp[(5) - (6)].strval) );
        ulpCOMPWheneverFunc( sTmpStr );
        gUlpCodeGen.ulpGenSetWhenever( gUlpCurDepth,
                                       GEN_WHEN_NOTFOUND,
                                       GEN_WHEN_DO_FUNC,
                                       sTmpStr );
    ;}
    break;

  case 416:

/* Line 1455 of yacc.c  */
#line 4134 "ulpCompy.y"
    {
        gUlpCodeGen.ulpGenSetWhenever( gUlpCurDepth,
                                       GEN_WHEN_NOTFOUND,
                                       GEN_WHEN_STOP,
                                       NULL );
    ;}
    break;

  case 418:

/* Line 1455 of yacc.c  */
#line 4146 "ulpCompy.y"
    {
        gUlpStmttype    = S_IGNORE;
        gUlpIsPrintStmt = ID_FALSE;
    ;}
    break;

  case 419:

/* Line 1455 of yacc.c  */
#line 4153 "ulpCompy.y"
    {
        gUlpSharedPtrPrevCond  = gUlpCOMPPrevCond;
        gUlpCOMPStartCond = CP_ST_C;
        idlOS::strcpy ( gUlpCodeGen.ulpGetSharedPtrName(), (yyvsp[(5) - (6)].strval) );
        gUlpParseInfo.mIsSharedPtr = ID_TRUE;

    ;}
    break;

  case 420:

/* Line 1455 of yacc.c  */
#line 4161 "ulpCompy.y"
    {
        gUlpCOMPStartCond = gUlpSharedPtrPrevCond;
        gUlpParseInfo.mIsSharedPtr = ID_FALSE;
        gUlpCodeGen.ulpClearSharedPtrInfo();
    ;}
    break;

  case 421:

/* Line 1455 of yacc.c  */
#line 4178 "ulpCompy.y"
    {
        gUlpStmttype = S_DirectOTH;
        gUlpIsPrintStmt = ID_TRUE;
    ;}
    break;

  case 422:

/* Line 1455 of yacc.c  */
#line 4184 "ulpCompy.y"
    {
        gUlpIsPrintStmt = ID_TRUE;
    ;}
    break;

  case 423:

/* Line 1455 of yacc.c  */
#line 4188 "ulpCompy.y"
    {
        gUlpStmttype = S_Commit;
        gUlpIsPrintStmt = ID_FALSE;
    ;}
    break;

  case 424:

/* Line 1455 of yacc.c  */
#line 4193 "ulpCompy.y"
    {
    ;}
    break;

  case 497:

/* Line 1455 of yacc.c  */
#line 4295 "ulpCompy.y"
    {
        gUlpStmttype = S_DirectDML;

        gUlpCodeGen.ulpGenEmSQL( GEN_QUERYSTR,
                                 idlOS::strstr( gUlpCodeGen.ulpGetQueryBuf(),
                                                (yyvsp[(2) - (2)].strval))
                               );

    ;}
    break;

  case 498:

/* Line 1455 of yacc.c  */
#line 4306 "ulpCompy.y"
    {
        gUlpStmttype = S_DirectDML;

        gUlpCodeGen.ulpGenEmSQL( GEN_QUERYSTR,
                                 idlOS::strstr( gUlpCodeGen.ulpGetQueryBuf(),
                                                (yyvsp[(1) - (1)].strval))
                               );
    ;}
    break;

  case 499:

/* Line 1455 of yacc.c  */
#line 4316 "ulpCompy.y"
    {
        gUlpStmttype = S_DirectDML;

        gUlpCodeGen.ulpGenEmSQL( GEN_QUERYSTR,
                                 idlOS::strstr( gUlpCodeGen.ulpGetQueryBuf(),
                                                (yyvsp[(2) - (2)].strval))
                               );

    ;}
    break;

  case 500:

/* Line 1455 of yacc.c  */
#line 4327 "ulpCompy.y"
    {
        gUlpStmttype = S_DirectDML;

        gUlpCodeGen.ulpGenEmSQL( GEN_QUERYSTR,
                                 idlOS::strstr( gUlpCodeGen.ulpGetQueryBuf(),
                                                (yyvsp[(1) - (1)].strval))
                               );

    ;}
    break;

  case 501:

/* Line 1455 of yacc.c  */
#line 4338 "ulpCompy.y"
    {
        gUlpStmttype = S_DirectDML;

        gUlpCodeGen.ulpGenEmSQL( GEN_QUERYSTR,
                                 idlOS::strstr( gUlpCodeGen.ulpGetQueryBuf(),
                                                (yyvsp[(2) - (2)].strval))
                               );

    ;}
    break;

  case 502:

/* Line 1455 of yacc.c  */
#line 4349 "ulpCompy.y"
    {
        gUlpStmttype = S_DirectDML;

        gUlpCodeGen.ulpGenEmSQL( GEN_QUERYSTR,
                                 idlOS::strstr( gUlpCodeGen.ulpGetQueryBuf(),
                                                (yyvsp[(1) - (1)].strval))
                               );

    ;}
    break;

  case 503:

/* Line 1455 of yacc.c  */
#line 4360 "ulpCompy.y"
    {
        gUlpStmttype = S_DirectDML;

        gUlpCodeGen.ulpGenEmSQL( GEN_QUERYSTR,
                                 idlOS::strstr( gUlpCodeGen.ulpGetQueryBuf(),
                                                (yyvsp[(1) - (1)].strval))
                               );

    ;}
    break;

  case 504:

/* Line 1455 of yacc.c  */
#line 4371 "ulpCompy.y"
    {
        gUlpStmttype = S_DirectDML;

        gUlpCodeGen.ulpGenEmSQL( GEN_QUERYSTR,
                                 idlOS::strstr( gUlpCodeGen.ulpGetQueryBuf(),
                                                (yyvsp[(1) - (1)].strval))
                               );

    ;}
    break;

  case 505:

/* Line 1455 of yacc.c  */
#line 4382 "ulpCompy.y"
    {
        gUlpStmttype = S_DirectSEL;

        gUlpCodeGen.ulpGenEmSQL( GEN_QUERYSTR,
                                 idlOS::strstr( gUlpCodeGen.ulpGetQueryBuf(),
                                                (yyvsp[(1) - (3)].strval))
                               );

    ;}
    break;

  case 506:

/* Line 1455 of yacc.c  */
#line 4393 "ulpCompy.y"
    {
        //is_select = sesTRUE;
        gUlpStmttype = S_DirectSEL;

        gUlpCodeGen.ulpGenEmSQL( GEN_QUERYSTR,
                                 idlOS::strstr( gUlpCodeGen.ulpGetQueryBuf(),
                                                (yyvsp[(1) - (3)].strval))
                               );
    ;}
    break;

  case 507:

/* Line 1455 of yacc.c  */
#line 4404 "ulpCompy.y"
    {
        gUlpStmttype = S_DirectDML;

        gUlpCodeGen.ulpGenEmSQL( GEN_QUERYSTR,
                                 idlOS::strstr( gUlpCodeGen.ulpGetQueryBuf(),
                                                (yyvsp[(1) - (1)].strval))
                               );
    ;}
    break;

  case 511:

/* Line 1455 of yacc.c  */
#line 4422 "ulpCompy.y"
    {
        gUlpCodeGen.ulpGenEmSQL( GEN_STATUSPTR, (yyvsp[(2) - (4)].strval)+1 );
        gUlpCodeGen.ulpGenEmSQL( GEN_ERRCODEPTR, (yyvsp[(4) - (4)].strval)+1 );
    ;}
    break;

  case 512:

/* Line 1455 of yacc.c  */
#line 4430 "ulpCompy.y"
    {
        SInt sNum;

        sNum = idlOS::atoi((yyvsp[(2) - (2)].strval));

        if ( sNum < 1 )
        {
            //The count of FOR clause must be greater than zero
            // error 처리

            ulpSetErrorCode( &gUlpParseInfo.mErrorMgr,
                             ulpERR_ABORT_COMP_FOR_iternum_Error );
            gUlpCOMPErrCode = ulpGetErrorSTATE( &gUlpParseInfo.mErrorMgr );
            COMPerror( ulpGetErrorMSG(&gUlpParseInfo.mErrorMgr) );
        }
        else
        {
            gUlpCodeGen.ulpGenEmSQL( GEN_ITERS, (void *) (yyvsp[(2) - (2)].strval) );
        }
    ;}
    break;

  case 513:

/* Line 1455 of yacc.c  */
#line 4451 "ulpCompy.y"
    {
        ulpSymTElement *sSymNode;

        if ( ( sSymNode = gUlpScopeT.ulpSLookupAll( (yyvsp[(2) - (2)].strval)+1, gUlpCurDepth ) ) == NULL )
        {
            //host 변수 못찾는 error
        }

        gUlpCodeGen.ulpGenEmSQL( GEN_ITERS, (yyvsp[(2) - (2)].strval)+1 );
    ;}
    break;

  case 514:

/* Line 1455 of yacc.c  */
#line 4462 "ulpCompy.y"
    {

        if(idlOS::strncasecmp("ATOMIC", (yyvsp[(1) - (3)].strval), 6) == 0)
        {
            if ( idlOS::atoi((yyvsp[(3) - (3)].strval)) < 1 )
            {
                //The count of FOR clause must be greater than zero
                // error 처리

                ulpSetErrorCode( &gUlpParseInfo.mErrorMgr,
                                 ulpERR_ABORT_COMP_FOR_iternum_Error );
                gUlpCOMPErrCode = ulpGetErrorSTATE( &gUlpParseInfo.mErrorMgr );
                COMPerror( ulpGetErrorMSG(&gUlpParseInfo.mErrorMgr) );
            }
            else
            {
                gUlpCodeGen.ulpGenEmSQL( GEN_ITERS, (void *) (yyvsp[(3) - (3)].strval) );
                gUlpCodeGen.ulpGenEmSQL( GEN_ATOMIC, (void *) "1" );
            }
        }
        else
        {
            // error 처리

            ulpSetErrorCode( &gUlpParseInfo.mErrorMgr,
                             ulpERR_ABORT_COMP_Unterminated_String_Error );
            gUlpCOMPErrCode = ulpGetErrorSTATE( &gUlpParseInfo.mErrorMgr );
            COMPerror( ulpGetErrorMSG(&gUlpParseInfo.mErrorMgr) );
        }
    ;}
    break;

  case 515:

/* Line 1455 of yacc.c  */
#line 4493 "ulpCompy.y"
    {
        ulpSymTElement *sSymNode;

        if( idlOS::strncasecmp("ATOMIC", (yyvsp[(1) - (3)].strval), 6) == 0 )
        {
            if ( ( sSymNode = gUlpScopeT.ulpSLookupAll( (yyvsp[(3) - (3)].strval)+1, gUlpCurDepth ) ) == NULL )
            {
                //host 변수 못찾는 error
            }

            gUlpCodeGen.ulpGenEmSQL( GEN_ITERS, (yyvsp[(3) - (3)].strval)+1 );

            gUlpCodeGen.ulpGenEmSQL( GEN_ATOMIC, (void *) "1" );
        }
        else
        {
            // error 처리

            ulpSetErrorCode( &gUlpParseInfo.mErrorMgr,
                             ulpERR_ABORT_COMP_Unterminated_String_Error );
            gUlpCOMPErrCode = ulpGetErrorSTATE( &gUlpParseInfo.mErrorMgr );
            COMPerror( ulpGetErrorMSG(&gUlpParseInfo.mErrorMgr) );
        }
    ;}
    break;

  case 516:

/* Line 1455 of yacc.c  */
#line 4521 "ulpCompy.y"
    {
        gUlpCodeGen.ulpGenEmSQL( GEN_SQLINFO, (void *) "0" );
    ;}
    break;

  case 517:

/* Line 1455 of yacc.c  */
#line 4525 "ulpCompy.y"
    {
        // release 구문이 오면 disconnect 해야한다.
        gUlpCodeGen.ulpGenEmSQL( GEN_SQLINFO, (void *) "1" );
    ;}
    break;

  case 518:

/* Line 1455 of yacc.c  */
#line 4533 "ulpCompy.y"
    {
        gUlpCodeGen.ulpGenEmSQL( GEN_SQLINFO, (void *) "2" );
    ;}
    break;

  case 519:

/* Line 1455 of yacc.c  */
#line 4537 "ulpCompy.y"
    {
        // release 구문이 오면 disconnect 해야한다.
        gUlpCodeGen.ulpGenEmSQL( GEN_SQLINFO, (void *) "3" );
        // 나중에 rollback 구문을 comment로 출력할때 release 토큰은 제거됨을 주의하자.
        gUlpCodeGen.ulpGenCutQueryTail( (yyvsp[(2) - (2)].strval) );
    ;}
    break;

  case 586:

/* Line 1455 of yacc.c  */
#line 4632 "ulpCompy.y"
    {
        if(idlOS::strncasecmp("EAGER", (yyvsp[(6) - (6)].strval), 5) != 0 &&
           idlOS::strncasecmp("LAZY", (yyvsp[(6) - (6)].strval), 4) != 0 &&
           idlOS::strncasecmp("ACKED", (yyvsp[(6) - (6)].strval), 5) != 0 &&
           idlOS::strncasecmp("NONE", (yyvsp[(6) - (6)].strval), 4) != 0)
        {
            // error 처리

            ulpSetErrorCode( &gUlpParseInfo.mErrorMgr,
                             ulpERR_ABORT_COMP_Unterminated_String_Error );
            gUlpCOMPErrCode = ulpGetErrorSTATE( &gUlpParseInfo.mErrorMgr );
            COMPerror( ulpGetErrorMSG(&gUlpParseInfo.mErrorMgr) );
        }
    ;}
    break;

  case 587:

/* Line 1455 of yacc.c  */
#line 4648 "ulpCompy.y"
    {
        if(idlOS::strncasecmp("EXPLAIN", (yyvsp[(4) - (7)].strval), 7) != 0 ||
           idlOS::strncasecmp("PLAN", (yyvsp[(5) - (7)].strval), 4) != 0)
        {
            // error 처리

            ulpSetErrorCode( &gUlpParseInfo.mErrorMgr,
                             ulpERR_ABORT_COMP_Unterminated_String_Error );
            gUlpCOMPErrCode = ulpGetErrorSTATE( &gUlpParseInfo.mErrorMgr );
            COMPerror( ulpGetErrorMSG(&gUlpParseInfo.mErrorMgr) );
        }
    ;}
    break;

  case 588:

/* Line 1455 of yacc.c  */
#line 4662 "ulpCompy.y"
    {
        if(idlOS::strncasecmp("EXPLAIN", (yyvsp[(4) - (7)].strval), 7) != 0 ||
           idlOS::strncasecmp("PLAN", (yyvsp[(5) - (7)].strval), 4) != 0)
        {
            // error 처리

            ulpSetErrorCode( &gUlpParseInfo.mErrorMgr,
                             ulpERR_ABORT_COMP_Unterminated_String_Error );
            gUlpCOMPErrCode = ulpGetErrorSTATE( &gUlpParseInfo.mErrorMgr );
            COMPerror( ulpGetErrorMSG(&gUlpParseInfo.mErrorMgr) );
        }
    ;}
    break;

  case 589:

/* Line 1455 of yacc.c  */
#line 4676 "ulpCompy.y"
    {
        if(idlOS::strncasecmp("EXPLAIN", (yyvsp[(4) - (7)].strval), 7) != 0 ||
           idlOS::strncasecmp("PLAN", (yyvsp[(5) - (7)].strval), 4) != 0)
        {
            // error 처리

            ulpSetErrorCode( &gUlpParseInfo.mErrorMgr,
                             ulpERR_ABORT_COMP_Unterminated_String_Error );
            gUlpCOMPErrCode = ulpGetErrorSTATE( &gUlpParseInfo.mErrorMgr );
            COMPerror( ulpGetErrorMSG(&gUlpParseInfo.mErrorMgr) );
        }
    ;}
    break;

  case 591:

/* Line 1455 of yacc.c  */
#line 4692 "ulpCompy.y"
    {
        if(idlOS::strncasecmp("STACK", (yyvsp[(4) - (7)].strval), 5) != 0 ||
           idlOS::strncasecmp("SIZE", (yyvsp[(5) - (7)].strval), 4) != 0)
        {
            // error 처리

            ulpSetErrorCode( &gUlpParseInfo.mErrorMgr,
                             ulpERR_ABORT_COMP_Unterminated_String_Error );
            gUlpCOMPErrCode = ulpGetErrorSTATE( &gUlpParseInfo.mErrorMgr );
            COMPerror( ulpGetErrorMSG(&gUlpParseInfo.mErrorMgr) );
        }
    ;}
    break;

  case 593:

/* Line 1455 of yacc.c  */
#line 4712 "ulpCompy.y"
    {
        if(idlOS::strncasecmp("SYSTEM", (yyvsp[(2) - (3)].strval), 6) != 0 ||
           idlOS::strncasecmp("CHECKPOINT", (yyvsp[(3) - (3)].strval), 10) != 0 &&
           idlOS::strncasecmp("REORGANIZE", (yyvsp[(3) - (3)].strval), 10) != 0 &&
           idlOS::strncasecmp("VERIFY", (yyvsp[(3) - (3)].strval), 6) != 0 &&
           idlOS::strncasecmp("COMPACT", (yyvsp[(3) - (3)].strval), 7) != 0)
        {
            // error 처리

            ulpSetErrorCode( &gUlpParseInfo.mErrorMgr,
                             ulpERR_ABORT_COMP_Unterminated_String_Error );
            gUlpCOMPErrCode = ulpGetErrorSTATE( &gUlpParseInfo.mErrorMgr );
            COMPerror( ulpGetErrorMSG(&gUlpParseInfo.mErrorMgr) );
        }
        if(idlOS::strncasecmp("COMPACT", (yyvsp[(3) - (3)].strval), 7) == 0)
        {
            // error 처리

            ulpSetErrorCode( &gUlpParseInfo.mErrorMgr,
                             ulpERR_ABORT_COMP_Not_Supported_ALTER_COMPACT_Error );
            gUlpCOMPErrCode = ulpGetErrorSTATE( &gUlpParseInfo.mErrorMgr );
            COMPerror( ulpGetErrorMSG(&gUlpParseInfo.mErrorMgr) );
        }
    ;}
    break;

  case 594:

/* Line 1455 of yacc.c  */
#line 4738 "ulpCompy.y"
    {
        if(idlOS::strncasecmp("SYSTEM", (yyvsp[(2) - (4)].strval), 6) != 0)
        {
            // error 처리

            ulpSetErrorCode( &gUlpParseInfo.mErrorMgr,
                             ulpERR_ABORT_COMP_Unterminated_String_Error );
            gUlpCOMPErrCode = ulpGetErrorSTATE( &gUlpParseInfo.mErrorMgr );
            COMPerror( ulpGetErrorMSG(&gUlpParseInfo.mErrorMgr) );
        }
        else
        {
            if(idlOS::strncasecmp("MEMORY", (yyvsp[(3) - (4)].strval), 6) == 0 &&
               idlOS::strncasecmp("COMPACT", (yyvsp[(4) - (4)].strval), 7) == 0)
            {
                // Nothing to do 
            }
            else if(idlOS::strncasecmp("SWITCH", (yyvsp[(3) - (4)].strval), 6) == 0 &&
                    idlOS::strncasecmp("LOGFILE", (yyvsp[(4) - (4)].strval), 7) == 0)
            {
                // Nothing to do 
            }
            else
            {
                // error 처리

                ulpSetErrorCode( &gUlpParseInfo.mErrorMgr,
                                 ulpERR_ABORT_COMP_Unterminated_String_Error );
                gUlpCOMPErrCode = ulpGetErrorSTATE( &gUlpParseInfo.mErrorMgr );
                COMPerror( ulpGetErrorMSG(&gUlpParseInfo.mErrorMgr) );
            }
        }
    ;}
    break;

  case 595:

/* Line 1455 of yacc.c  */
#line 4772 "ulpCompy.y"
    {
        if(idlOS::strncasecmp("SYSTEM", (yyvsp[(2) - (5)].strval), 6) != 0 ||
           idlOS::strncasecmp("LOG", (yyvsp[(4) - (5)].strval), 3) != 0)
        {
            // error 처리

            ulpSetErrorCode( &gUlpParseInfo.mErrorMgr,
                             ulpERR_ABORT_COMP_Unterminated_String_Error );
            gUlpCOMPErrCode = ulpGetErrorSTATE( &gUlpParseInfo.mErrorMgr );
            COMPerror( ulpGetErrorMSG(&gUlpParseInfo.mErrorMgr) );
        }
    ;}
    break;

  case 596:

/* Line 1455 of yacc.c  */
#line 4786 "ulpCompy.y"
    {
        if(idlOS::strncasecmp("SYSTEM", (yyvsp[(2) - (6)].strval), 6) != 0)
        {
            // error 처리

            ulpSetErrorCode( &gUlpParseInfo.mErrorMgr,
                             ulpERR_ABORT_COMP_Unterminated_String_Error );
            gUlpCOMPErrCode = ulpGetErrorSTATE( &gUlpParseInfo.mErrorMgr );
            COMPerror( ulpGetErrorMSG(&gUlpParseInfo.mErrorMgr) );
        }
    ;}
    break;

  case 597:

/* Line 1455 of yacc.c  */
#line 4799 "ulpCompy.y"
    {
        if(idlOS::strncasecmp("SYSTEM", (yyvsp[(2) - (7)].strval), 6) != 0)
        {
            // error 처리

            ulpSetErrorCode( &gUlpParseInfo.mErrorMgr,
                             ulpERR_ABORT_COMP_Unterminated_String_Error );
            gUlpCOMPErrCode = ulpGetErrorSTATE( &gUlpParseInfo.mErrorMgr );
            COMPerror( ulpGetErrorMSG(&gUlpParseInfo.mErrorMgr) );
        }
    ;}
    break;

  case 598:

/* Line 1455 of yacc.c  */
#line 4812 "ulpCompy.y"
    {
        if(idlOS::strncasecmp("SYSTEM", (yyvsp[(2) - (6)].strval), 6) != 0)
        {
            // error 처리

            ulpSetErrorCode( &gUlpParseInfo.mErrorMgr,
                             ulpERR_ABORT_COMP_Unterminated_String_Error );
            gUlpCOMPErrCode = ulpGetErrorSTATE( &gUlpParseInfo.mErrorMgr );
            COMPerror( ulpGetErrorMSG(&gUlpParseInfo.mErrorMgr) );
        }
    ;}
    break;

  case 599:

/* Line 1455 of yacc.c  */
#line 4825 "ulpCompy.y"
    {
        if(idlOS::strncasecmp("SYSTEM", (yyvsp[(2) - (6)].strval), 6) != 0)
        {
            // error 처리

            ulpSetErrorCode( &gUlpParseInfo.mErrorMgr,
                             ulpERR_ABORT_COMP_Unterminated_String_Error );
            gUlpCOMPErrCode = ulpGetErrorSTATE( &gUlpParseInfo.mErrorMgr );
            COMPerror( ulpGetErrorMSG(&gUlpParseInfo.mErrorMgr) );
        }
    ;}
    break;

  case 600:

/* Line 1455 of yacc.c  */
#line 4838 "ulpCompy.y"
    {
        if (( idlOS::strncasecmp("SYSTEM", (yyvsp[(2) - (5)].strval), 6) != 0 ) ||
            ( idlOS::strncasecmp("RELOAD", (yyvsp[(3) - (5)].strval), 6) != 0 ) ||
            ( idlOS::strncasecmp("LIST", (yyvsp[(5) - (5)].strval), 4) != 0 ))
        {
            // error 처리
            ulpSetErrorCode( &gUlpParseInfo.mErrorMgr,
                             ulpERR_ABORT_COMP_Unterminated_String_Error );
            gUlpCOMPErrCode = ulpGetErrorSTATE( &gUlpParseInfo.mErrorMgr );
            COMPerror( ulpGetErrorMSG(&gUlpParseInfo.mErrorMgr) );
        }
    ;}
    break;

  case 601:

/* Line 1455 of yacc.c  */
#line 4852 "ulpCompy.y"
    {
        if ( ( idlOS::strncasecmp( "SYSTEM", (yyvsp[(2) - (7)].strval), 6 ) != 0 ) ||
             ( idlOS::strncasecmp( "RELOAD", (yyvsp[(3) - (7)].strval), 6 ) != 0 ) ||
             ( idlOS::strncasecmp( "META",   (yyvsp[(5) - (7)].strval), 4 ) != 0 ) ||
             ( idlOS::strncasecmp( "NUMBER", (yyvsp[(6) - (7)].strval), 6 ) != 0 ) )
        {
            // error 처리
            ulpSetErrorCode( &gUlpParseInfo.mErrorMgr,
                             ulpERR_ABORT_COMP_Unterminated_String_Error );
            gUlpCOMPErrCode = ulpGetErrorSTATE( &gUlpParseInfo.mErrorMgr );
            COMPerror( ulpGetErrorMSG( &gUlpParseInfo.mErrorMgr ) );
        }
    ;}
    break;

  case 605:

/* Line 1455 of yacc.c  */
#line 4875 "ulpCompy.y"
    {
        if(idlOS::strncasecmp("STOP", (yyvsp[(1) - (1)].strval), 4) != 0)
        {
            // error 처리

            ulpSetErrorCode( &gUlpParseInfo.mErrorMgr,
                             ulpERR_ABORT_COMP_Unterminated_String_Error );
            gUlpCOMPErrCode = ulpGetErrorSTATE( &gUlpParseInfo.mErrorMgr );
            COMPerror( ulpGetErrorMSG(&gUlpParseInfo.mErrorMgr) );
        }
    ;}
    break;

  case 608:

/* Line 1455 of yacc.c  */
#line 4898 "ulpCompy.y"
    {
        gUlpStmttype    = S_Commit;
        gUlpIsPrintStmt = ID_FALSE;
    ;}
    break;

  case 609:

/* Line 1455 of yacc.c  */
#line 4903 "ulpCompy.y"
    {
        gUlpStmttype    = S_DirectRB;
        gUlpIsPrintStmt = ID_TRUE;
        gUlpCodeGen.ulpGenEmSQL( GEN_QUERYSTR,
                                 idlOS::strstr( gUlpCodeGen.ulpGetQueryBuf(),
                                                (yyvsp[(1) - (5)].strval))
                               );
    ;}
    break;

  case 612:

/* Line 1455 of yacc.c  */
#line 4920 "ulpCompy.y"
    {
        if(idlOS::strncasecmp("TRANSACTION", (yyvsp[(2) - (3)].strval), 11) != 0)
        {
            // error 처리

            ulpSetErrorCode( &gUlpParseInfo.mErrorMgr,
                             ulpERR_ABORT_COMP_Unterminated_String_Error );
            gUlpCOMPErrCode = ulpGetErrorSTATE( &gUlpParseInfo.mErrorMgr );
            COMPerror( ulpGetErrorMSG(&gUlpParseInfo.mErrorMgr) );
        }
    ;}
    break;

  case 613:

/* Line 1455 of yacc.c  */
#line 4932 "ulpCompy.y"
    {
        if(idlOS::strncasecmp("TRANSACTION", (yyvsp[(4) - (5)].strval), 11) != 0)
        {
            // error 처리

            ulpSetErrorCode( &gUlpParseInfo.mErrorMgr,
                             ulpERR_ABORT_COMP_Unterminated_String_Error );
            gUlpCOMPErrCode = ulpGetErrorSTATE( &gUlpParseInfo.mErrorMgr );
            COMPerror( ulpGetErrorMSG(&gUlpParseInfo.mErrorMgr) );
        }
    ;}
    break;

  case 614:

/* Line 1455 of yacc.c  */
#line 4948 "ulpCompy.y"
    {
        if ( idlOS::strncasecmp( "FORCE", (yyvsp[(3) - (5)].strval), 5 ) != 0 )
        {
            // error 처리

            ulpSetErrorCode( &gUlpParseInfo.mErrorMgr,
                             ulpERR_ABORT_COMP_Unterminated_String_Error );
            gUlpCOMPErrCode = ulpGetErrorSTATE( &gUlpParseInfo.mErrorMgr );
            COMPerror( ulpGetErrorMSG(&gUlpParseInfo.mErrorMgr) );
        }
    ;}
    break;

  case 615:

/* Line 1455 of yacc.c  */
#line 4964 "ulpCompy.y"
    {
        if ( idlOS::strncasecmp( "FORCE", (yyvsp[(3) - (5)].strval), 5 ) != 0 )
        {
            // error 처리

            ulpSetErrorCode( &gUlpParseInfo.mErrorMgr,
                             ulpERR_ABORT_COMP_Unterminated_String_Error );
            gUlpCOMPErrCode = ulpGetErrorSTATE( &gUlpParseInfo.mErrorMgr );
            COMPerror( ulpGetErrorMSG(&gUlpParseInfo.mErrorMgr) );
        }
    ;}
    break;

  case 618:

/* Line 1455 of yacc.c  */
#line 4984 "ulpCompy.y"
    {
        if(idlOS::strncasecmp("COMMITTED", (yyvsp[(4) - (4)].strval), 9) != 0 &&
           idlOS::strncasecmp("UNCOMMITTED", (yyvsp[(4) - (4)].strval), 11) != 0)
        {
            // error 처리

            ulpSetErrorCode( &gUlpParseInfo.mErrorMgr,
                             ulpERR_ABORT_COMP_Unterminated_String_Error );
            gUlpCOMPErrCode = ulpGetErrorSTATE( &gUlpParseInfo.mErrorMgr );
            COMPerror( ulpGetErrorMSG(&gUlpParseInfo.mErrorMgr) );
        }
    ;}
    break;

  case 619:

/* Line 1455 of yacc.c  */
#line 4998 "ulpCompy.y"
    {
        if(idlOS::strncasecmp("REPEATABLE", (yyvsp[(3) - (4)].strval), 10) != 0)
        {
            // error 처리

            ulpSetErrorCode( &gUlpParseInfo.mErrorMgr,
                             ulpERR_ABORT_COMP_Unterminated_String_Error );
            gUlpCOMPErrCode = ulpGetErrorSTATE( &gUlpParseInfo.mErrorMgr );
            COMPerror( ulpGetErrorMSG(&gUlpParseInfo.mErrorMgr) );
        }
    ;}
    break;

  case 620:

/* Line 1455 of yacc.c  */
#line 5011 "ulpCompy.y"
    {
        if(idlOS::strncasecmp("SERIALIZABLE", (yyvsp[(3) - (3)].strval), 12) != 0)
        {
            // error 처리

            ulpSetErrorCode( &gUlpParseInfo.mErrorMgr,
                             ulpERR_ABORT_COMP_Unterminated_String_Error );
            gUlpCOMPErrCode = ulpGetErrorSTATE( &gUlpParseInfo.mErrorMgr );
            COMPerror( ulpGetErrorMSG(&gUlpParseInfo.mErrorMgr) );
        }
    ;}
    break;

  case 659:

/* Line 1455 of yacc.c  */
#line 5139 "ulpCompy.y"
    {
        if(idlOS::strncasecmp("SYSTEM", (yyvsp[(2) - (2)].strval), 6) != 0)
        {
            // error 처리

            ulpSetErrorCode( &gUlpParseInfo.mErrorMgr,
                             ulpERR_ABORT_COMP_Unterminated_String_Error );
            gUlpCOMPErrCode = ulpGetErrorSTATE( &gUlpParseInfo.mErrorMgr );
            COMPerror( ulpGetErrorMSG(&gUlpParseInfo.mErrorMgr) );
        }
    ;}
    break;

  case 711:

/* Line 1455 of yacc.c  */
#line 5207 "ulpCompy.y"
    {
        if(idlOS::strncasecmp("JOB", (yyvsp[(3) - (3)].strval), 3) != 0)
        {
            // error 처리

            ulpSetErrorCode( &gUlpParseInfo.mErrorMgr,
                             ulpERR_ABORT_COMP_Unterminated_String_Error );
            gUlpCOMPErrCode = ulpGetErrorSTATE( &gUlpParseInfo.mErrorMgr );
            COMPerror( ulpGetErrorMSG(&gUlpParseInfo.mErrorMgr) );
        }
    ;}
    break;

  case 712:

/* Line 1455 of yacc.c  */
#line 5219 "ulpCompy.y"
    {
        if(idlOS::strncasecmp("JOB", (yyvsp[(3) - (3)].strval), 3) != 0)
        {
            // error 처리

            ulpSetErrorCode( &gUlpParseInfo.mErrorMgr,
                             ulpERR_ABORT_COMP_Unterminated_String_Error );
            gUlpCOMPErrCode = ulpGetErrorSTATE( &gUlpParseInfo.mErrorMgr );
            COMPerror( ulpGetErrorMSG(&gUlpParseInfo.mErrorMgr) );
        }
    ;}
    break;

  case 713:

/* Line 1455 of yacc.c  */
#line 5231 "ulpCompy.y"
    {
        if(idlOS::strncasecmp("JOB", (yyvsp[(3) - (3)].strval), 3) != 0)
        {
            // error 처리

            ulpSetErrorCode( &gUlpParseInfo.mErrorMgr,
                             ulpERR_ABORT_COMP_Unterminated_String_Error );
            gUlpCOMPErrCode = ulpGetErrorSTATE( &gUlpParseInfo.mErrorMgr );
            COMPerror( ulpGetErrorMSG(&gUlpParseInfo.mErrorMgr) );
        }
    ;}
    break;

  case 741:

/* Line 1455 of yacc.c  */
#line 5306 "ulpCompy.y"
    {
        if(idlOS::strncasecmp("FORCE", (yyvsp[(1) - (1)].strval), 5) != 0)
        {
            // error 처리

            ulpSetErrorCode( &gUlpParseInfo.mErrorMgr,
                             ulpERR_ABORT_COMP_Unterminated_String_Error );
            gUlpCOMPErrCode = ulpGetErrorSTATE( &gUlpParseInfo.mErrorMgr );
            COMPerror( ulpGetErrorMSG(&gUlpParseInfo.mErrorMgr) );
        }
    ;}
    break;

  case 749:

/* Line 1455 of yacc.c  */
#line 5357 "ulpCompy.y"
    {
        if(idlOS::strncasecmp("HOST", (yyvsp[(5) - (6)].strval), 4) != 0)
        {
            // error 처리

            ulpSetErrorCode( &gUlpParseInfo.mErrorMgr,
                             ulpERR_ABORT_COMP_Unterminated_String_Error );
            gUlpCOMPErrCode = ulpGetErrorSTATE( &gUlpParseInfo.mErrorMgr );
            COMPerror( ulpGetErrorMSG(&gUlpParseInfo.mErrorMgr) );
        }
    ;}
    break;

  case 750:

/* Line 1455 of yacc.c  */
#line 5370 "ulpCompy.y"
    {
        if(idlOS::strncasecmp("HOST", (yyvsp[(5) - (6)].strval), 4) != 0)
        {
            // error 처리

            ulpSetErrorCode( &gUlpParseInfo.mErrorMgr,
                             ulpERR_ABORT_COMP_Unterminated_String_Error );
            gUlpCOMPErrCode = ulpGetErrorSTATE( &gUlpParseInfo.mErrorMgr );
            COMPerror( ulpGetErrorMSG(&gUlpParseInfo.mErrorMgr) );
        }
    ;}
    break;

  case 751:

/* Line 1455 of yacc.c  */
#line 5383 "ulpCompy.y"
    {
        if(idlOS::strncasecmp("HOST", (yyvsp[(5) - (6)].strval), 4) != 0)
        {
            // error 처리

            ulpSetErrorCode( &gUlpParseInfo.mErrorMgr,
                             ulpERR_ABORT_COMP_Unterminated_String_Error );
            gUlpCOMPErrCode = ulpGetErrorSTATE( &gUlpParseInfo.mErrorMgr );
            COMPerror( ulpGetErrorMSG(&gUlpParseInfo.mErrorMgr) );
        }
    ;}
    break;

  case 753:

/* Line 1455 of yacc.c  */
#line 5402 "ulpCompy.y"
    {
        if ( ( idlOS::strncasecmp("RECOVERY", (yyvsp[(5) - (6)].strval), 8 ) != 0 ) &&
             ( idlOS::strncasecmp("GAPLESS", (yyvsp[(5) - (6)].strval), 7 ) != 0 ) &&
             ( idlOS::strncasecmp("GROUPING", (yyvsp[(5) - (6)].strval), 8 ) != 0 ) &&
             ( idlOS::strncasecmp("DDL_REPLICATE", (yyvsp[(5) - (6)].strval), 13 ) != 0 ) ) // BUG-46525
        {
            // error 처리

            ulpSetErrorCode( &gUlpParseInfo.mErrorMgr,
                             ulpERR_ABORT_COMP_Unterminated_String_Error );
            gUlpCOMPErrCode = ulpGetErrorSTATE( &gUlpParseInfo.mErrorMgr );
            COMPerror( ulpGetErrorMSG(&gUlpParseInfo.mErrorMgr) );
        }
    ;}
    break;

  case 760:

/* Line 1455 of yacc.c  */
#line 5434 "ulpCompy.y"
    {
        if(idlOS::strncasecmp("SYNC", (yyvsp[(4) - (5)].strval), 4) != 0 &&
           idlOS::strncasecmp("QUICKSTART", (yyvsp[(4) - (5)].strval), 10) != 0 &&
           idlOS::strncasecmp("STOP", (yyvsp[(4) - (5)].strval), 4) != 0 &&
           idlOS::strncasecmp("RESET", (yyvsp[(4) - (5)].strval), 5) != 0)
        {
            // error 처리

            ulpSetErrorCode( &gUlpParseInfo.mErrorMgr,
                             ulpERR_ABORT_COMP_Unterminated_String_Error );
            gUlpCOMPErrCode = ulpGetErrorSTATE( &gUlpParseInfo.mErrorMgr );
            COMPerror( ulpGetErrorMSG(&gUlpParseInfo.mErrorMgr) );
        }
    ;}
    break;

  case 761:

/* Line 1455 of yacc.c  */
#line 5451 "ulpCompy.y"
    {
        if(idlOS::strncasecmp("SYNC", (yyvsp[(4) - (6)].strval), 4) != 0 &&
           idlOS::strncasecmp("QUICKSTART", (yyvsp[(4) - (6)].strval), 10) != 0)
        {
            // error 처리

            ulpSetErrorCode( &gUlpParseInfo.mErrorMgr,
                             ulpERR_ABORT_COMP_Unterminated_String_Error );
            gUlpCOMPErrCode = ulpGetErrorSTATE( &gUlpParseInfo.mErrorMgr );
            COMPerror( ulpGetErrorMSG(&gUlpParseInfo.mErrorMgr) );
        }
    ;}
    break;

  case 764:

/* Line 1455 of yacc.c  */
#line 5470 "ulpCompy.y"
    {
        if(idlOS::strncasecmp("OPTIONS", (yyvsp[(1) - (2)].strval), 7) != 0)
        {
            // error 처리

            ulpSetErrorCode( &gUlpParseInfo.mErrorMgr,
                             ulpERR_ABORT_COMP_Unterminated_String_Error );
            gUlpCOMPErrCode = ulpGetErrorSTATE( &gUlpParseInfo.mErrorMgr );
            COMPerror( ulpGetErrorMSG(&gUlpParseInfo.mErrorMgr) );
        }
    ;}
    break;

  case 767:

/* Line 1455 of yacc.c  */
#line 5490 "ulpCompy.y"
    {
        if ( ( idlOS::strncasecmp("RECOVERY", (yyvsp[(1) - (1)].strval), 8 ) != 0 ) &&
             ( idlOS::strncasecmp("GAPLESS", (yyvsp[(1) - (1)].strval), 7 ) != 0 ) &&
             ( idlOS::strncasecmp("GROUPING", (yyvsp[(1) - (1)].strval), 8 ) != 0 ) &&
             ( idlOS::strncasecmp("DDL_REPLICATE", (yyvsp[(1) - (1)].strval), 13 ) != 0 ) && // BUG-46525
             /* BUG-46528 Apply __REPLICATION_USE_V6_PROTOCOL to each replication. */
             ( idlOS::strncasecmp("V6_PROTOCOL", (yyvsp[(1) - (1)].strval), 11 ) != 0 ) )            

        {
            // error 처리

            ulpSetErrorCode( &gUlpParseInfo.mErrorMgr,
                             ulpERR_ABORT_COMP_Unterminated_String_Error );
            gUlpCOMPErrCode = ulpGetErrorSTATE( &gUlpParseInfo.mErrorMgr );
            COMPerror( ulpGetErrorMSG(&gUlpParseInfo.mErrorMgr) );
        }
    ;}
    break;

  case 773:

/* Line 1455 of yacc.c  */
#line 5519 "ulpCompy.y"
    {
        if(idlOS::strncasecmp("LAZY", (yyvsp[(1) - (1)].strval), 4) != 0 &&
           idlOS::strncasecmp("EAGER", (yyvsp[(1) - (1)].strval), 5) != 0)
        {
            // error 처리

            ulpSetErrorCode( &gUlpParseInfo.mErrorMgr,
                             ulpERR_ABORT_COMP_Unterminated_String_Error );
            gUlpCOMPErrCode = ulpGetErrorSTATE( &gUlpParseInfo.mErrorMgr );
            COMPerror( ulpGetErrorMSG(&gUlpParseInfo.mErrorMgr) );
        }
    ;}
    break;

  case 777:

/* Line 1455 of yacc.c  */
#line 5541 "ulpCompy.y"
    {
        if(idlOS::strncasecmp("UNIX_DOMAIN", (yyvsp[(1) - (1)].strval), 11) != 0)
        {
            // error 처리

            ulpSetErrorCode( &gUlpParseInfo.mErrorMgr,
                             ulpERR_ABORT_COMP_Unterminated_String_Error );
            gUlpCOMPErrCode = ulpGetErrorSTATE( &gUlpParseInfo.mErrorMgr );
            COMPerror( ulpGetErrorMSG(&gUlpParseInfo.mErrorMgr) );
        }
    ;}
    break;

  case 782:

/* Line 1455 of yacc.c  */
#line 5567 "ulpCompy.y"
    {
        if( idlOS::strncasecmp("TCP", (yyvsp[(2) - (2)].strval), 3) != 0 && 
            idlOS::strncasecmp("IB", (yyvsp[(2) - (2)].strval), 2) != 0 )
        {
            // error 처리

            ulpSetErrorCode( &gUlpParseInfo.mErrorMgr,
                             ulpERR_ABORT_COMP_Unterminated_String_Error );
            gUlpCOMPErrCode = ulpGetErrorSTATE( &gUlpParseInfo.mErrorMgr );
            COMPerror( ulpGetErrorMSG(&gUlpParseInfo.mErrorMgr) );
        }
    ;}
    break;

  case 783:

/* Line 1455 of yacc.c  */
#line 5580 "ulpCompy.y"
    {
        if( idlOS::strncasecmp("IB", (yyvsp[(2) - (3)].strval), 2) != 0 ) 
        {
            // error 처리

            ulpSetErrorCode( &gUlpParseInfo.mErrorMgr,
                             ulpERR_ABORT_COMP_Unterminated_String_Error );
            gUlpCOMPErrCode = ulpGetErrorSTATE( &gUlpParseInfo.mErrorMgr );
            COMPerror( ulpGetErrorMSG(&gUlpParseInfo.mErrorMgr) );
        }
    ;}
    break;

  case 786:

/* Line 1455 of yacc.c  */
#line 5598 "ulpCompy.y"
    {
        if( (idlOS::strncasecmp("ANALYSIS", (yyvsp[(2) - (2)].strval), 8) != 0) &&
            (idlOS::strncasecmp("PROPAGATION", (yyvsp[(2) - (2)].strval), 11) != 0) )
        {
            // error 처리

            ulpSetErrorCode( &gUlpParseInfo.mErrorMgr,
                             ulpERR_ABORT_COMP_Unterminated_String_Error );
            gUlpCOMPErrCode = ulpGetErrorSTATE( &gUlpParseInfo.mErrorMgr );
            COMPerror( ulpGetErrorMSG(&gUlpParseInfo.mErrorMgr) );
        }
    ;}
    break;

  case 787:

/* Line 1455 of yacc.c  */
#line 5611 "ulpCompy.y"
    {
        if( idlOS::strncasecmp("PROPAGABLE", (yyvsp[(2) - (3)].strval), 10) != 0 )
        {
            // error 처리

            ulpSetErrorCode( &gUlpParseInfo.mErrorMgr,
                             ulpERR_ABORT_COMP_Unterminated_String_Error );
            gUlpCOMPErrCode = ulpGetErrorSTATE( &gUlpParseInfo.mErrorMgr );
            COMPerror( ulpGetErrorMSG(&gUlpParseInfo.mErrorMgr) );
        }
    ;}
    break;

  case 789:

/* Line 1455 of yacc.c  */
#line 5627 "ulpCompy.y"
    {
        if(idlOS::strncasecmp("MASTER", (yyvsp[(2) - (2)].strval), 6) != 0 &&
           idlOS::strncasecmp("SLAVE", (yyvsp[(2) - (2)].strval), 5) != 0)
        {
            // error 처리

            ulpSetErrorCode( &gUlpParseInfo.mErrorMgr,
                             ulpERR_ABORT_COMP_Unterminated_String_Error );
            gUlpCOMPErrCode = ulpGetErrorSTATE( &gUlpParseInfo.mErrorMgr );
            COMPerror( ulpGetErrorMSG(&gUlpParseInfo.mErrorMgr) );
        }
    ;}
    break;

  case 801:

/* Line 1455 of yacc.c  */
#line 5665 "ulpCompy.y"
    {
        if(idlOS::strncasecmp("RETRY", (yyvsp[(1) - (1)].strval), 5) != 0)
        {
            // error 처리

            ulpSetErrorCode( &gUlpParseInfo.mErrorMgr,
                             ulpERR_ABORT_COMP_Unterminated_String_Error );
            gUlpCOMPErrCode = ulpGetErrorSTATE( &gUlpParseInfo.mErrorMgr );
            COMPerror( ulpGetErrorMSG(&gUlpParseInfo.mErrorMgr) );
        }
    ;}
    break;

  case 810:

/* Line 1455 of yacc.c  */
#line 5697 "ulpCompy.y"
    {
        if(idlOS::strncasecmp("RETRY", (yyvsp[(1) - (1)].strval), 5) != 0)
        {
            // error 처리

            ulpSetErrorCode( &gUlpParseInfo.mErrorMgr,
                             ulpERR_ABORT_COMP_Unterminated_String_Error );
            gUlpCOMPErrCode = ulpGetErrorSTATE( &gUlpParseInfo.mErrorMgr );
            COMPerror( ulpGetErrorMSG(&gUlpParseInfo.mErrorMgr) );
        }
    ;}
    break;

  case 811:

/* Line 1455 of yacc.c  */
#line 5710 "ulpCompy.y"
    {
        if(idlOS::strncasecmp("SN", (yyvsp[(2) - (5)].strval), 2) != 0)
        {
            // error 처리

            ulpSetErrorCode( &gUlpParseInfo.mErrorMgr,
                             ulpERR_ABORT_COMP_Unterminated_String_Error );
            gUlpCOMPErrCode = ulpGetErrorSTATE( &gUlpParseInfo.mErrorMgr );
            COMPerror( ulpGetErrorMSG(&gUlpParseInfo.mErrorMgr) );
        }
    ;}
    break;

  case 842:

/* Line 1455 of yacc.c  */
#line 5813 "ulpCompy.y"
    {
        if(idlOS::strncasecmp("INCREMENT", (yyvsp[(1) - (3)].strval), 9) != 0)
        {
            // error 처리

            ulpSetErrorCode( &gUlpParseInfo.mErrorMgr,
                             ulpERR_ABORT_COMP_Unterminated_String_Error );
            gUlpCOMPErrCode = ulpGetErrorSTATE( &gUlpParseInfo.mErrorMgr );
            COMPerror( ulpGetErrorMSG(&gUlpParseInfo.mErrorMgr) );
        }
    ;}
    break;

  case 843:

/* Line 1455 of yacc.c  */
#line 5826 "ulpCompy.y"
    {
        if(idlOS::strncasecmp("INCREMENT", (yyvsp[(1) - (4)].strval), 9) != 0)
        {
            // error 처리

            ulpSetErrorCode( &gUlpParseInfo.mErrorMgr,
                             ulpERR_ABORT_COMP_Unterminated_String_Error );
            gUlpCOMPErrCode = ulpGetErrorSTATE( &gUlpParseInfo.mErrorMgr );
            COMPerror( ulpGetErrorMSG(&gUlpParseInfo.mErrorMgr) );
        }
    ;}
    break;

  case 844:

/* Line 1455 of yacc.c  */
#line 5841 "ulpCompy.y"
    {
        if(idlOS::strncasecmp("CACHE", (yyvsp[(1) - (2)].strval), 5) != 0 &&
           idlOS::strncasecmp("MAXVALUE", (yyvsp[(1) - (2)].strval), 8) != 0 &&
           idlOS::strncasecmp("MINVALUE", (yyvsp[(1) - (2)].strval), 8) != 0)
        {
            // error 처리

            ulpSetErrorCode( &gUlpParseInfo.mErrorMgr,
                             ulpERR_ABORT_COMP_Unterminated_String_Error );
            gUlpCOMPErrCode = ulpGetErrorSTATE( &gUlpParseInfo.mErrorMgr );
            COMPerror( ulpGetErrorMSG(&gUlpParseInfo.mErrorMgr) );
        }
    ;}
    break;

  case 845:

/* Line 1455 of yacc.c  */
#line 5855 "ulpCompy.y"
    {
        if(idlOS::strncasecmp("MAXVALUE", (yyvsp[(1) - (3)].strval), 8) != 0 &&
           idlOS::strncasecmp("MINVALUE", (yyvsp[(1) - (3)].strval), 8) != 0)
        {
            // error 처리

            ulpSetErrorCode( &gUlpParseInfo.mErrorMgr,
                             ulpERR_ABORT_COMP_Unterminated_String_Error );
            gUlpCOMPErrCode = ulpGetErrorSTATE( &gUlpParseInfo.mErrorMgr );
            COMPerror( ulpGetErrorMSG(&gUlpParseInfo.mErrorMgr) );
        }
    ;}
    break;

  case 846:

/* Line 1455 of yacc.c  */
#line 5871 "ulpCompy.y"
    {
        if(idlOS::strncasecmp("NOCACHE", (yyvsp[(1) - (1)].strval), 7) != 0 &&
           idlOS::strncasecmp("NOMAXVALUE", (yyvsp[(1) - (1)].strval), 10) != 0 &&
           idlOS::strncasecmp("NOMINVALUE", (yyvsp[(1) - (1)].strval), 10) != 0)
        {
            // error 처리

            ulpSetErrorCode( &gUlpParseInfo.mErrorMgr,
                             ulpERR_ABORT_COMP_Unterminated_String_Error );
            gUlpCOMPErrCode = ulpGetErrorSTATE( &gUlpParseInfo.mErrorMgr );
            COMPerror( ulpGetErrorMSG(&gUlpParseInfo.mErrorMgr) );
        }
    ;}
    break;

  case 859:

/* Line 1455 of yacc.c  */
#line 5920 "ulpCompy.y"
    {
        if ( ( idlOS::strncasecmp("COMPACT", (yyvsp[(4) - (5)].strval), 7) != 0 ) &&
             ( idlOS::strncasecmp("AGING", (yyvsp[(4) - (5)].strval), 5) != 0 ) &&
             ( idlOS::strncasecmp("TOUCH", (yyvsp[(4) - (5)].strval), 5) != 0 ) )
        {
            // error 처리

            ulpSetErrorCode( &gUlpParseInfo.mErrorMgr,
                             ulpERR_ABORT_COMP_Unterminated_String_Error );
            gUlpCOMPErrCode = ulpGetErrorSTATE( &gUlpParseInfo.mErrorMgr );
            COMPerror( ulpGetErrorMSG(&gUlpParseInfo.mErrorMgr) );
        }
    ;}
    break;

  case 939:

/* Line 1455 of yacc.c  */
#line 6158 "ulpCompy.y"
    {
        if(idlOS::strncasecmp("INDEXTYPE", (yyvsp[(1) - (3)].strval), 9) != 0 ||
           idlOS::strncasecmp("BTREE", (yyvsp[(3) - (3)].strval), 5) != 0 &&
           idlOS::strncasecmp("RTREE", (yyvsp[(3) - (3)].strval), 5) != 0 &&
           // Altibase Spatio-Temporal DBMS
           idlOS::strncasecmp("TDRTREE", (yyvsp[(3) - (3)].strval), 7) != 0)
        {
            // error 처리

            ulpSetErrorCode( &gUlpParseInfo.mErrorMgr,
                             ulpERR_ABORT_COMP_Unterminated_String_Error );
            gUlpCOMPErrCode = ulpGetErrorSTATE( &gUlpParseInfo.mErrorMgr );
            COMPerror( ulpGetErrorMSG(&gUlpParseInfo.mErrorMgr) );
        }
    ;}
    break;

  case 941:

/* Line 1455 of yacc.c  */
#line 6178 "ulpCompy.y"
    {
        if(idlOS::strncasecmp("PERSISTENT", (yyvsp[(2) - (4)].strval), 10) != 0)
        {
            // error 처리

            ulpSetErrorCode( &gUlpParseInfo.mErrorMgr,
                           ulpERR_ABORT_COMP_Unterminated_String_Error );
            gUlpCOMPErrCode = ulpGetErrorSTATE( &gUlpParseInfo.mErrorMgr );
            COMPerror( ulpGetErrorMSG(&gUlpParseInfo.mErrorMgr) );
        }
    ;}
    break;

  case 942:

/* Line 1455 of yacc.c  */
#line 6190 "ulpCompy.y"
    {
        if(idlOS::strncasecmp("PERSISTENT", (yyvsp[(2) - (4)].strval), 10) != 0)
        {
            // error 처리

            ulpSetErrorCode( &gUlpParseInfo.mErrorMgr,
                           ulpERR_ABORT_COMP_Unterminated_String_Error );
            gUlpCOMPErrCode = ulpGetErrorSTATE( &gUlpParseInfo.mErrorMgr );
            COMPerror( ulpGetErrorMSG(&gUlpParseInfo.mErrorMgr) );
        }
    ;}
    break;

  case 988:

/* Line 1455 of yacc.c  */
#line 6328 "ulpCompy.y"
    {
      // if (strMatch : HIGH,2)
      // else if ( strMatch : LOW, 2)
      if(idlOS::strncasecmp("HIGH", (yyvsp[(2) - (4)].strval), 4) != 0 &&
          idlOS::strncasecmp("LOW", (yyvsp[(2) - (4)].strval), 3) != 0)
      {
          // error 처리

          ulpSetErrorCode( &gUlpParseInfo.mErrorMgr,
                           ulpERR_ABORT_COMP_Unterminated_String_Error );
          gUlpCOMPErrCode = ulpGetErrorSTATE( &gUlpParseInfo.mErrorMgr );
          COMPerror( ulpGetErrorMSG(&gUlpParseInfo.mErrorMgr) );
      }
  ;}
    break;

  case 998:

/* Line 1455 of yacc.c  */
#line 6370 "ulpCompy.y"
    {
        if(idlOS::strncasecmp("LOGGING", (yyvsp[(1) - (1)].strval), 7) != 0 &&
           idlOS::strncasecmp("NOLOGGING", (yyvsp[(1) - (1)].strval), 9) != 0 &&
           idlOS::strncasecmp("BUFFER", (yyvsp[(1) - (1)].strval), 6) != 0 &&
           idlOS::strncasecmp("NOBUFFER", (yyvsp[(1) - (1)].strval), 8) != 0)
        {
            // error 처리

            ulpSetErrorCode( &gUlpParseInfo.mErrorMgr,
                           ulpERR_ABORT_COMP_Unterminated_String_Error );
            gUlpCOMPErrCode = ulpGetErrorSTATE( &gUlpParseInfo.mErrorMgr );
            COMPerror( ulpGetErrorMSG(&gUlpParseInfo.mErrorMgr) );
        }
    ;}
    break;

  case 1059:

/* Line 1455 of yacc.c  */
#line 6550 "ulpCompy.y"
    {
      if(idlOS::strncasecmp("NO", (yyvsp[(1) - (2)].strval), 2) != 0 ||
         idlOS::strncasecmp("ACTION", (yyvsp[(2) - (2)].strval), 6) != 0)
      {
            // error 처리

            ulpSetErrorCode( &gUlpParseInfo.mErrorMgr,
                           ulpERR_ABORT_COMP_Unterminated_String_Error );
            gUlpCOMPErrCode = ulpGetErrorSTATE( &gUlpParseInfo.mErrorMgr );
            COMPerror( ulpGetErrorMSG(&gUlpParseInfo.mErrorMgr) );
      }
  ;}
    break;

  case 1069:

/* Line 1455 of yacc.c  */
#line 6613 "ulpCompy.y"
    {
        if(idlOS::strncasecmp("NO", (yyvsp[(1) - (2)].strval), 2) != 0 ||
           idlOS::strncasecmp("FORCE", (yyvsp[(2) - (2)].strval), 5) != 0)
        {
            // error 처리

            ulpSetErrorCode( &gUlpParseInfo.mErrorMgr,
                           ulpERR_ABORT_COMP_Unterminated_String_Error );
            gUlpCOMPErrCode = ulpGetErrorSTATE( &gUlpParseInfo.mErrorMgr );
            COMPerror( ulpGetErrorMSG(&gUlpParseInfo.mErrorMgr) );
        }
    ;}
    break;

  case 1070:

/* Line 1455 of yacc.c  */
#line 6626 "ulpCompy.y"
    {
        if(idlOS::strncasecmp("FORCE", (yyvsp[(1) - (1)].strval), 5) != 0)
        {
            // error 처리

            ulpSetErrorCode( &gUlpParseInfo.mErrorMgr,
                           ulpERR_ABORT_COMP_Unterminated_String_Error );
            gUlpCOMPErrCode = ulpGetErrorSTATE( &gUlpParseInfo.mErrorMgr );
            COMPerror( ulpGetErrorMSG(&gUlpParseInfo.mErrorMgr) );
        }
    ;}
    break;

  case 1087:

/* Line 1455 of yacc.c  */
#line 6686 "ulpCompy.y"
    {
      if ( idlOS::strncasecmp( "PRIVATE", (yyvsp[(1) - (1)].strval), 7 ) != 0 )
      {
          // error 처리
          
          ulpSetErrorCode( &gUlpParseInfo.mErrorMgr,
                           ulpERR_ABORT_COMP_Unterminated_String_Error );
          gUlpCOMPErrCode = ulpGetErrorSTATE( &gUlpParseInfo.mErrorMgr );
          COMPerror( ulpGetErrorMSG(&gUlpParseInfo.mErrorMgr) );
      }
  ;}
    break;

  case 1090:

/* Line 1455 of yacc.c  */
#line 6708 "ulpCompy.y"
    {
      if ( idlOS::strncasecmp( "STOP", (yyvsp[(4) - (4)].strval), 4 ) != 0 )
      {
          // error 처리

          ulpSetErrorCode( &gUlpParseInfo.mErrorMgr,
                           ulpERR_ABORT_COMP_Unterminated_String_Error );
          gUlpCOMPErrCode = ulpGetErrorSTATE( &gUlpParseInfo.mErrorMgr );
          COMPerror( ulpGetErrorMSG(&gUlpParseInfo.mErrorMgr) );
      }      
  ;}
    break;

  case 1091:

/* Line 1455 of yacc.c  */
#line 6720 "ulpCompy.y"
    {
      if ( idlOS::strncasecmp( "STOP", (yyvsp[(4) - (5)].strval), 4 ) != 0 )
      {
          // error 처리

          ulpSetErrorCode( &gUlpParseInfo.mErrorMgr,
                           ulpERR_ABORT_COMP_Unterminated_String_Error );
          gUlpCOMPErrCode = ulpGetErrorSTATE( &gUlpParseInfo.mErrorMgr );
          COMPerror( ulpGetErrorMSG(&gUlpParseInfo.mErrorMgr) );
      }
      if ( idlOS::strncasecmp( "FORCE", (yyvsp[(5) - (5)].strval), 5 ) != 0 )
      {
          // error 처리

          ulpSetErrorCode( &gUlpParseInfo.mErrorMgr,
                           ulpERR_ABORT_COMP_Unterminated_String_Error );
          gUlpCOMPErrCode = ulpGetErrorSTATE( &gUlpParseInfo.mErrorMgr );
          COMPerror( ulpGetErrorMSG(&gUlpParseInfo.mErrorMgr) );
      }
  ;}
    break;

  case 1096:

/* Line 1455 of yacc.c  */
#line 6760 "ulpCompy.y"
    {
        if ( idlOS::strncasecmp( "COMPACT", (yyvsp[(4) - (4)].strval), 7 ) != 0 )
        {
            /* error 처리 */
            ulpSetErrorCode( &gUlpParseInfo.mErrorMgr,
                             ulpERR_ABORT_COMP_Unterminated_String_Error );
            gUlpCOMPErrCode = ulpGetErrorSTATE( &gUlpParseInfo.mErrorMgr );
            COMPerror( ulpGetErrorMSG(&gUlpParseInfo.mErrorMgr) );
        }
    ;}
    break;

  case 1097:

/* Line 1455 of yacc.c  */
#line 6771 "ulpCompy.y"
    {
        if ( ( idlOS::strncasecmp( "MSGID", (yyvsp[(4) - (5)].strval), 5 ) != 0 ) ||
             ( idlOS::strncasecmp( "RESET", (yyvsp[(5) - (5)].strval), 5 ) != 0 ) )
        {
            /* error 처리 */
            ulpSetErrorCode( &gUlpParseInfo.mErrorMgr,
                             ulpERR_ABORT_COMP_Unterminated_String_Error );
            gUlpCOMPErrCode = ulpGetErrorSTATE( &gUlpParseInfo.mErrorMgr );
            COMPerror( ulpGetErrorMSG(&gUlpParseInfo.mErrorMgr) );
        }
    ;}
    break;

  case 1337:

/* Line 1455 of yacc.c  */
#line 7465 "ulpCompy.y"
    {
        if(idlOS::strncasecmp("IGNORE", (yyvsp[(1) - (2)].strval), 6) != 0)
        {
            // error 처리

            ulpSetErrorCode( &gUlpParseInfo.mErrorMgr,
                           ulpERR_ABORT_COMP_Unterminated_String_Error );
            gUlpCOMPErrCode = ulpGetErrorSTATE( &gUlpParseInfo.mErrorMgr );
            COMPerror( ulpGetErrorMSG(&gUlpParseInfo.mErrorMgr) );
        }
    ;}
    break;

  case 1340:

/* Line 1455 of yacc.c  */
#line 7482 "ulpCompy.y"
    {
      if ( idlOS::strncasecmp("SIBLINGS", (yyvsp[(2) - (4)].strval), 8 ) != 0 )
      {
          ulpSetErrorCode( &gUlpParseInfo.mErrorMgr,
                           ulpERR_ABORT_COMP_Unterminated_String_Error );
          gUlpCOMPErrCode = ulpGetErrorSTATE( &gUlpParseInfo.mErrorMgr );
          COMPerror( ulpGetErrorMSG(&gUlpParseInfo.mErrorMgr) );
      }
      else
      {
          /* Nothing to do */
      }
  ;}
    break;

  case 1345:

/* Line 1455 of yacc.c  */
#line 7509 "ulpCompy.y"
    {
      if ( idlOS::strncasecmp("OFFSET", (yyvsp[(2) - (3)].strval), 6 ) != 0 )
      {
          ulpSetErrorCode( &gUlpParseInfo.mErrorMgr,
                           ulpERR_ABORT_COMP_Unterminated_String_Error );
          gUlpCOMPErrCode = ulpGetErrorSTATE( &gUlpParseInfo.mErrorMgr );
          COMPerror( ulpGetErrorMSG(&gUlpParseInfo.mErrorMgr) );
      }
      else
      {
          /* Nothing to do */
      }

    ;}
    break;

  case 1354:

/* Line 1455 of yacc.c  */
#line 7573 "ulpCompy.y"
    {
        if(idlOS::strncasecmp("NOWAIT", (yyvsp[(1) - (1)].strval), 6) != 0)
        {
            // error 처리

            ulpSetErrorCode( &gUlpParseInfo.mErrorMgr,
                             ulpERR_ABORT_COMP_Unterminated_String_Error );
            gUlpCOMPErrCode = ulpGetErrorSTATE( &gUlpParseInfo.mErrorMgr );
            COMPerror( ulpGetErrorMSG(&gUlpParseInfo.mErrorMgr) );
        }

    ;}
    break;

  case 1376:

/* Line 1455 of yacc.c  */
#line 7638 "ulpCompy.y"
    {
        if(idlOS::strncasecmp("SHARE", (yyvsp[(2) - (2)].strval), 5) != 0 &&
           idlOS::strncasecmp("EXCLUSIVE", (yyvsp[(2) - (2)].strval), 9) != 0)
        {
            // error 처리

            ulpSetErrorCode( &gUlpParseInfo.mErrorMgr,
                             ulpERR_ABORT_COMP_Unterminated_String_Error );
            gUlpCOMPErrCode = ulpGetErrorSTATE( &gUlpParseInfo.mErrorMgr );
            COMPerror( ulpGetErrorMSG(&gUlpParseInfo.mErrorMgr) );
        }
    ;}
    break;

  case 1377:

/* Line 1455 of yacc.c  */
#line 7652 "ulpCompy.y"
    {
        if(idlOS::strncasecmp("SHARE", (yyvsp[(1) - (2)].strval), 5) != 0)
        {
            // error 처리

            ulpSetErrorCode( &gUlpParseInfo.mErrorMgr,
                             ulpERR_ABORT_COMP_Unterminated_String_Error );
            gUlpCOMPErrCode = ulpGetErrorSTATE( &gUlpParseInfo.mErrorMgr );
            COMPerror( ulpGetErrorMSG(&gUlpParseInfo.mErrorMgr) );
        }
    ;}
    break;

  case 1378:

/* Line 1455 of yacc.c  */
#line 7665 "ulpCompy.y"
    {
        if(idlOS::strncasecmp("SHARE", (yyvsp[(1) - (3)].strval), 5) != 0 ||
           idlOS::strncasecmp("EXCLUSIVE", (yyvsp[(3) - (3)].strval), 9) != 0)
        {
            // error 처리

            ulpSetErrorCode( &gUlpParseInfo.mErrorMgr,
                             ulpERR_ABORT_COMP_Unterminated_String_Error );
            gUlpCOMPErrCode = ulpGetErrorSTATE( &gUlpParseInfo.mErrorMgr );
            COMPerror( ulpGetErrorMSG(&gUlpParseInfo.mErrorMgr) );
        }
    ;}
    break;

  case 1379:

/* Line 1455 of yacc.c  */
#line 7680 "ulpCompy.y"
    {
        if(idlOS::strncasecmp("SHARE", (yyvsp[(1) - (1)].strval), 5) != 0 &&
           idlOS::strncasecmp("EXCLUSIVE", (yyvsp[(1) - (1)].strval), 9) != 0)
        {
            // error 처리

            ulpSetErrorCode( &gUlpParseInfo.mErrorMgr,
                             ulpERR_ABORT_COMP_Unterminated_String_Error );
            gUlpCOMPErrCode = ulpGetErrorSTATE( &gUlpParseInfo.mErrorMgr );
            COMPerror( ulpGetErrorMSG(&gUlpParseInfo.mErrorMgr) );
        }
    ;}
    break;

  case 1381:

/* Line 1455 of yacc.c  */
#line 7698 "ulpCompy.y"
    {
        if ( idlOS::strncasecmp( "DDL",  (yyvsp[(3) - (3)].strval), 3 ) != 0 )
        {
            // error 처리
            ulpSetErrorCode( &gUlpParseInfo.mErrorMgr,
                             ulpERR_ABORT_COMP_Unterminated_String_Error );
            gUlpCOMPErrCode = ulpGetErrorSTATE( &gUlpParseInfo.mErrorMgr );
            COMPerror( ulpGetErrorMSG( &gUlpParseInfo.mErrorMgr ) );
        }
        else
        {
            /* Nothing to do */
        }
    ;}
    break;

  case 1419:

/* Line 1455 of yacc.c  */
#line 7783 "ulpCompy.y"
    {
        if(idlOS::strncasecmp("ISOPEN", (yyvsp[(3) - (3)].strval), 6) != 0 &&
           idlOS::strncasecmp("NOTFOUND", (yyvsp[(3) - (3)].strval), 8) != 0)
        {
            // error 처리

            ulpSetErrorCode( &gUlpParseInfo.mErrorMgr,
                             ulpERR_ABORT_COMP_Unterminated_String_Error );
            gUlpCOMPErrCode = ulpGetErrorSTATE( &gUlpParseInfo.mErrorMgr );
            COMPerror( ulpGetErrorMSG(&gUlpParseInfo.mErrorMgr) );
        }
    ;}
    break;

  case 1644:

/* Line 1455 of yacc.c  */
#line 8524 "ulpCompy.y"
    {
        if(idlOS::strncasecmp("RECORD", (yyvsp[(4) - (8)].strval), 6) != 0)
        {
            // error 처리

            ulpSetErrorCode( &gUlpParseInfo.mErrorMgr,
                             ulpERR_ABORT_COMP_Unterminated_String_Error );
            gUlpCOMPErrCode = ulpGetErrorSTATE( &gUlpParseInfo.mErrorMgr );
            COMPerror( ulpGetErrorMSG(&gUlpParseInfo.mErrorMgr) );
        }
    ;}
    break;

  case 1764:

/* Line 1455 of yacc.c  */
#line 8969 "ulpCompy.y"
    {
        // strMatch : INITSIZE, 1
        if(idlOS::strncasecmp("INITSIZE", (yyvsp[(1) - (3)].strval), 8) != 0)
        {
            // error 처리

            ulpSetErrorCode( &gUlpParseInfo.mErrorMgr,
                             ulpERR_ABORT_COMP_Unterminated_String_Error );
            gUlpCOMPErrCode = ulpGetErrorSTATE( &gUlpParseInfo.mErrorMgr );
            COMPerror( ulpGetErrorMSG(&gUlpParseInfo.mErrorMgr) );
        }
    ;}
    break;

  case 1770:

/* Line 1455 of yacc.c  */
#line 9007 "ulpCompy.y"
    {
          if(idlOS::strncasecmp("CHARACTER", (yyvsp[(1) - (3)].strval), 9) != 0)
          {
            // error 처리

            ulpSetErrorCode( &gUlpParseInfo.mErrorMgr,
                             ulpERR_ABORT_COMP_Unterminated_String_Error );
            gUlpCOMPErrCode = ulpGetErrorSTATE( &gUlpParseInfo.mErrorMgr );
            COMPerror( ulpGetErrorMSG(&gUlpParseInfo.mErrorMgr) );
          }
      ;}
    break;

  case 1771:

/* Line 1455 of yacc.c  */
#line 9025 "ulpCompy.y"
    {
          if( (idlOS::strncasecmp("NATIONAL", (yyvsp[(1) - (4)].strval), 8) != 0) &&
              (idlOS::strncasecmp("CHARACTER", (yyvsp[(2) - (4)].strval), 9) != 0) )
          {
            // error 처리

            ulpSetErrorCode( &gUlpParseInfo.mErrorMgr,
                             ulpERR_ABORT_COMP_Unterminated_String_Error );
            gUlpCOMPErrCode = ulpGetErrorSTATE( &gUlpParseInfo.mErrorMgr );
            COMPerror( ulpGetErrorMSG(&gUlpParseInfo.mErrorMgr) );
          }
      ;}
    break;

  case 1778:

/* Line 1455 of yacc.c  */
#line 9047 "ulpCompy.y"
    {
      // strMatch : DATAFILE, 4
      if(idlOS::strncasecmp("DATAFILE", (yyvsp[(4) - (6)].strval), 8) != 0)
      {
          // error 처리

          ulpSetErrorCode( &gUlpParseInfo.mErrorMgr,
                             ulpERR_ABORT_COMP_Unterminated_String_Error );
          gUlpCOMPErrCode = ulpGetErrorSTATE( &gUlpParseInfo.mErrorMgr );
          COMPerror( ulpGetErrorMSG(&gUlpParseInfo.mErrorMgr) );
      }
  ;}
    break;

  case 1779:

/* Line 1455 of yacc.c  */
#line 9060 "ulpCompy.y"
    {
      // strMatch : DATAFILE, 4
      if(idlOS::strncasecmp("DATAFILE", (yyvsp[(4) - (7)].strval), 8) != 0)
      {
          // error 처리

          ulpSetErrorCode( &gUlpParseInfo.mErrorMgr,
                           ulpERR_ABORT_COMP_Unterminated_String_Error );
          gUlpCOMPErrCode = ulpGetErrorSTATE( &gUlpParseInfo.mErrorMgr );
          COMPerror( ulpGetErrorMSG(&gUlpParseInfo.mErrorMgr) );
      }
  ;}
    break;

  case 1781:

/* Line 1455 of yacc.c  */
#line 9075 "ulpCompy.y"
    {
      if(idlOS::strncasecmp("SNAPSHOT", (yyvsp[(4) - (4)].strval), 8) != 0)
      {
          // error 처리

          ulpSetErrorCode( &gUlpParseInfo.mErrorMgr,
                           ulpERR_ABORT_COMP_Unterminated_String_Error );
          gUlpCOMPErrCode = ulpGetErrorSTATE( &gUlpParseInfo.mErrorMgr );
          COMPerror( ulpGetErrorMSG(&gUlpParseInfo.mErrorMgr) );
      }
  ;}
    break;

  case 1782:

/* Line 1455 of yacc.c  */
#line 9088 "ulpCompy.y"
    {
      if(idlOS::strncasecmp("SNAPSHOT", (yyvsp[(4) - (4)].strval), 8) != 0)
      {
          // error 처리
          ulpSetErrorCode( &gUlpParseInfo.mErrorMgr,
                           ulpERR_ABORT_COMP_Unterminated_String_Error );
          gUlpCOMPErrCode = ulpGetErrorSTATE( &gUlpParseInfo.mErrorMgr );
          COMPerror( ulpGetErrorMSG(&gUlpParseInfo.mErrorMgr) );
      }
  ;}
    break;

  case 1784:

/* Line 1455 of yacc.c  */
#line 9103 "ulpCompy.y"
    {
      // strMatch : CANCEL, 2
      if(idlOS::strncasecmp("CANCEL", (yyvsp[(2) - (2)].strval), 6) != 0)
      {
          // error 처리

          ulpSetErrorCode( &gUlpParseInfo.mErrorMgr,
                           ulpERR_ABORT_COMP_Unterminated_String_Error );
          gUlpCOMPErrCode = ulpGetErrorSTATE( &gUlpParseInfo.mErrorMgr );
          COMPerror( ulpGetErrorMSG(&gUlpParseInfo.mErrorMgr) );
      }
  ;}
    break;

  case 1785:

/* Line 1455 of yacc.c  */
#line 9116 "ulpCompy.y"
    {
      // strMatch : TIME, 2
      if(idlOS::strncasecmp("TIME", (yyvsp[(2) - (3)].strval), 4) != 0)
      {
          // error 처리

          ulpSetErrorCode( &gUlpParseInfo.mErrorMgr,
                           ulpERR_ABORT_COMP_Unterminated_String_Error );
          gUlpCOMPErrCode = ulpGetErrorSTATE( &gUlpParseInfo.mErrorMgr );
          COMPerror( ulpGetErrorMSG(&gUlpParseInfo.mErrorMgr) );
      }
  ;}
    break;

  case 1790:

/* Line 1455 of yacc.c  */
#line 9142 "ulpCompy.y"
    {
    // strMatch : 1) PROCESS
    //            2) CONTROL
    //            3) SERVICE
    //            4) META    , 1
    if(idlOS::strncasecmp("PROCESS", (yyvsp[(1) - (1)].strval), 7) != 0 &&
       idlOS::strncasecmp("CONTROL", (yyvsp[(1) - (1)].strval), 7) != 0 &&
       idlOS::strncasecmp("SERVICE", (yyvsp[(1) - (1)].strval), 7) != 0 &&
       idlOS::strncasecmp("META", (yyvsp[(1) - (1)].strval), 4) != 0)
    {
          // error 처리

          ulpSetErrorCode( &gUlpParseInfo.mErrorMgr,
                           ulpERR_ABORT_COMP_Unterminated_String_Error );
          gUlpCOMPErrCode = ulpGetErrorSTATE( &gUlpParseInfo.mErrorMgr );
          COMPerror( ulpGetErrorMSG(&gUlpParseInfo.mErrorMgr) );
    }
  ;}
    break;

  case 1791:

/* Line 1455 of yacc.c  */
#line 9161 "ulpCompy.y"
    {
    // strMatch : 1) META  UPGRADE
    //            2) META RESETLOGS
    //            3) META RESETUNDO
    //            4) SHUTDOWN NORMAL
    if(idlOS::strncasecmp("META", (yyvsp[(1) - (2)].strval), 4) == 0 &&
       idlOS::strncasecmp("UPGRADE", (yyvsp[(2) - (2)].strval), 7) == 0)
    {
    }
    else if(idlOS::strncasecmp("META", (yyvsp[(1) - (2)].strval), 4) == 0 &&
            idlOS::strncasecmp("RESETLOGS", (yyvsp[(2) - (2)].strval), 9) == 0)
    {
    }
    else if(idlOS::strncasecmp("META", (yyvsp[(1) - (2)].strval), 4) == 0 &&
            idlOS::strncasecmp("RESETUNDO", (yyvsp[(2) - (2)].strval), 9) == 0)
    {
    }
    else if(idlOS::strncasecmp("SHUTDOWN", (yyvsp[(1) - (2)].strval), 8) == 0 &&
            idlOS::strncasecmp("NORMAL", (yyvsp[(2) - (2)].strval), 6) == 0)
    {
    }
    else
    {
          // error 처리

          ulpSetErrorCode( &gUlpParseInfo.mErrorMgr,
                           ulpERR_ABORT_COMP_Unterminated_String_Error );
          gUlpCOMPErrCode = ulpGetErrorSTATE( &gUlpParseInfo.mErrorMgr );
          COMPerror( ulpGetErrorMSG(&gUlpParseInfo.mErrorMgr) );
    }
  ;}
    break;

  case 1792:

/* Line 1455 of yacc.c  */
#line 9193 "ulpCompy.y"
    {
    // strMatch : SHUTDOWN, 1
    if(idlOS::strncasecmp("SHUTDOWN", (yyvsp[(1) - (2)].strval), 8) != 0)
    {
          // error 처리

          ulpSetErrorCode( &gUlpParseInfo.mErrorMgr,
                           ulpERR_ABORT_COMP_Unterminated_String_Error );
          gUlpCOMPErrCode = ulpGetErrorSTATE( &gUlpParseInfo.mErrorMgr );
          COMPerror( ulpGetErrorMSG(&gUlpParseInfo.mErrorMgr) );
    }
  ;}
    break;

  case 1793:

/* Line 1455 of yacc.c  */
#line 9206 "ulpCompy.y"
    {
    // strMatch : SHUTDOWN, 1
    if(idlOS::strncasecmp("SHUTDOWN", (yyvsp[(1) - (2)].strval), 8) != 0)
    {
          // error 처리

          ulpSetErrorCode( &gUlpParseInfo.mErrorMgr,
                           ulpERR_ABORT_COMP_Unterminated_String_Error );
          gUlpCOMPErrCode = ulpGetErrorSTATE( &gUlpParseInfo.mErrorMgr );
          COMPerror( ulpGetErrorMSG(&gUlpParseInfo.mErrorMgr) );
    }
  ;}
    break;

  case 1795:

/* Line 1455 of yacc.c  */
#line 9223 "ulpCompy.y"
    {
    // strMatch : DTX, 1
    if(idlOS::strncasecmp("DTX", (yyvsp[(1) - (3)].strval), 3) != 0)
    {
          // error 처리

          ulpSetErrorCode( &gUlpParseInfo.mErrorMgr,
                           ulpERR_ABORT_COMP_Unterminated_String_Error );
          gUlpCOMPErrCode = ulpGetErrorSTATE( &gUlpParseInfo.mErrorMgr );
          COMPerror( ulpGetErrorMSG(&gUlpParseInfo.mErrorMgr) );
    }
  ;}
    break;

  case 1796:

/* Line 1455 of yacc.c  */
#line 9236 "ulpCompy.y"
    {
    // strMatch : DTX, 1
    if(idlOS::strncasecmp("DTX", (yyvsp[(1) - (3)].strval), 3) != 0)
    {
          // error 처리

          ulpSetErrorCode( &gUlpParseInfo.mErrorMgr,
                           ulpERR_ABORT_COMP_Unterminated_String_Error );
          gUlpCOMPErrCode = ulpGetErrorSTATE( &gUlpParseInfo.mErrorMgr );
          COMPerror( ulpGetErrorMSG(&gUlpParseInfo.mErrorMgr) );
    }
  ;}
    break;

  case 1798:

/* Line 1455 of yacc.c  */
#line 9258 "ulpCompy.y"
    {
    // strMatch : DATAFILE, 4
    if(idlOS::strncasecmp("DATAFILE", (yyvsp[(4) - (6)].strval), 8) != 0)
    {
          // error 처리

          ulpSetErrorCode( &gUlpParseInfo.mErrorMgr,
                           ulpERR_ABORT_COMP_Unterminated_String_Error );
          gUlpCOMPErrCode = ulpGetErrorSTATE( &gUlpParseInfo.mErrorMgr );
          COMPerror( ulpGetErrorMSG(&gUlpParseInfo.mErrorMgr) );
    }
  ;}
    break;

  case 1799:

/* Line 1455 of yacc.c  */
#line 9277 "ulpCompy.y"
    {
      // strMatch : SIZE 5,
      if (idlOS::strncasecmp("SIZE", (yyvsp[(5) - (7)].strval), 4) != 0)
      {
          // error 처리

          ulpSetErrorCode( &gUlpParseInfo.mErrorMgr,
                           ulpERR_ABORT_COMP_Unterminated_String_Error );
          gUlpCOMPErrCode = ulpGetErrorSTATE( &gUlpParseInfo.mErrorMgr );
          COMPerror( ulpGetErrorMSG(&gUlpParseInfo.mErrorMgr) );
      }
  ;}
    break;

  case 1800:

/* Line 1455 of yacc.c  */
#line 9297 "ulpCompy.y"
    {
      // strMatch : SIZE 5,
      if (idlOS::strncasecmp("SIZE", (yyvsp[(5) - (8)].strval), 4) != 0)
      {
          // error 처리

          ulpSetErrorCode( &gUlpParseInfo.mErrorMgr,
                           ulpERR_ABORT_COMP_Unterminated_String_Error );
          gUlpCOMPErrCode = ulpGetErrorSTATE( &gUlpParseInfo.mErrorMgr );
          COMPerror( ulpGetErrorMSG(&gUlpParseInfo.mErrorMgr) );
      }
      if(idlOS::strncasecmp("K", (yyvsp[(7) - (8)].strval), 1) != 0 &&
         idlOS::strncasecmp("M", (yyvsp[(7) - (8)].strval), 1) != 0 &&
         idlOS::strncasecmp("G", (yyvsp[(7) - (8)].strval), 1) != 0)
      {
          // error 처리

          ulpSetErrorCode( &gUlpParseInfo.mErrorMgr,
                           ulpERR_ABORT_COMP_Unterminated_String_Error );
          gUlpCOMPErrCode = ulpGetErrorSTATE( &gUlpParseInfo.mErrorMgr );
          COMPerror( ulpGetErrorMSG(&gUlpParseInfo.mErrorMgr) );
      }
  ;}
    break;

  case 1801:

/* Line 1455 of yacc.c  */
#line 9326 "ulpCompy.y"
    {
    // strMatch : TEMPFILE, 5
    if(idlOS::strncasecmp("TEMPFILE", (yyvsp[(5) - (7)].strval), 8) != 0)
    {
          // error 처리

          ulpSetErrorCode( &gUlpParseInfo.mErrorMgr,
                           ulpERR_ABORT_COMP_Unterminated_String_Error );
          gUlpCOMPErrCode = ulpGetErrorSTATE( &gUlpParseInfo.mErrorMgr );
          COMPerror( ulpGetErrorMSG(&gUlpParseInfo.mErrorMgr) );
    }
  ;}
    break;

  case 1803:

/* Line 1455 of yacc.c  */
#line 9346 "ulpCompy.y"
    {
    // strMatch : 1) DATAFILE, 5
    //            2) TEMPFILE, 5
    if(idlOS::strncasecmp("DATAFILE", (yyvsp[(5) - (8)].strval), 8) != 0 &&
       idlOS::strncasecmp("TEMPFILE", (yyvsp[(5) - (8)].strval), 8) != 0)
    {
          // error 처리

          ulpSetErrorCode( &gUlpParseInfo.mErrorMgr,
                           ulpERR_ABORT_COMP_Unterminated_String_Error );
          gUlpCOMPErrCode = ulpGetErrorSTATE( &gUlpParseInfo.mErrorMgr );
          COMPerror( ulpGetErrorMSG(&gUlpParseInfo.mErrorMgr) );
    }
  ;}
    break;

  case 1804:

/* Line 1455 of yacc.c  */
#line 9362 "ulpCompy.y"
    {
    // strMatch : 1) DATAFILE, 5
    //            2) TEMPFILE, 5
    if(idlOS::strncasecmp("DATAFILE", (yyvsp[(5) - (7)].strval), 8) != 0 &&
       idlOS::strncasecmp("TEMPFILE", (yyvsp[(5) - (7)].strval), 8) != 0)
    {
          // error 처리

          ulpSetErrorCode( &gUlpParseInfo.mErrorMgr,
                           ulpERR_ABORT_COMP_Unterminated_String_Error );
          gUlpCOMPErrCode = ulpGetErrorSTATE( &gUlpParseInfo.mErrorMgr );
          COMPerror( ulpGetErrorMSG(&gUlpParseInfo.mErrorMgr) );
    }
  ;}
    break;

  case 1808:

/* Line 1455 of yacc.c  */
#line 9387 "ulpCompy.y"
    {
    // strMatch : 1) DATAFILE, 5
    //            2) TEMPFILE, 5
    if(idlOS::strncasecmp("DATAFILE", (yyvsp[(5) - (6)].strval), 8) != 0 &&
       idlOS::strncasecmp("TEMPFILE", (yyvsp[(5) - (6)].strval), 8) != 0)
    {
          // error 처리

          ulpSetErrorCode( &gUlpParseInfo.mErrorMgr,
                           ulpERR_ABORT_COMP_Unterminated_String_Error );
          gUlpCOMPErrCode = ulpGetErrorSTATE( &gUlpParseInfo.mErrorMgr );
          COMPerror( ulpGetErrorMSG(&gUlpParseInfo.mErrorMgr) );
    }
  ;}
    break;

  case 1809:

/* Line 1455 of yacc.c  */
#line 9403 "ulpCompy.y"
    {
    // strMatch : 1) DATAFILE, 5
    //            2) TEMPFILE, 5
    if(idlOS::strncasecmp("DATAFILE", (yyvsp[(5) - (6)].strval), 8) != 0 &&
       idlOS::strncasecmp("TEMPFILE", (yyvsp[(5) - (6)].strval), 8) != 0)
    {
          // error 처리

          ulpSetErrorCode( &gUlpParseInfo.mErrorMgr,
                           ulpERR_ABORT_COMP_Unterminated_String_Error );
          gUlpCOMPErrCode = ulpGetErrorSTATE( &gUlpParseInfo.mErrorMgr );
          COMPerror( ulpGetErrorMSG(&gUlpParseInfo.mErrorMgr) );
    }
  ;}
    break;

  case 1810:

/* Line 1455 of yacc.c  */
#line 9419 "ulpCompy.y"
    {
    // strMatch : 1) DATAFILE, 5
    //            2) TEMPFILE, 5
    if(idlOS::strncasecmp("DATAFILE", (yyvsp[(5) - (7)].strval), 8) != 0 &&
       idlOS::strncasecmp("TEMPFILE", (yyvsp[(5) - (7)].strval), 8) != 0)
    {
          // error 처리

          ulpSetErrorCode( &gUlpParseInfo.mErrorMgr,
                           ulpERR_ABORT_COMP_Unterminated_String_Error );
          gUlpCOMPErrCode = ulpGetErrorSTATE( &gUlpParseInfo.mErrorMgr );
          COMPerror( ulpGetErrorMSG(&gUlpParseInfo.mErrorMgr) );
    }
  ;}
    break;

  case 1811:

/* Line 1455 of yacc.c  */
#line 9436 "ulpCompy.y"
    {
    // strMatch : 1) DATAFILE, 5 && SIZE, 7
    //            2) TEMPFILE, 5 && SIZE, 7
    if(idlOS::strncasecmp("DATAFILE", (yyvsp[(5) - (8)].strval), 8) == 0 &&
       idlOS::strncasecmp("SIZE", (yyvsp[(7) - (8)].strval), 4) == 0)
    {
    }
    else if (idlOS::strncasecmp("TEMPFILE", (yyvsp[(5) - (8)].strval), 8) == 0 &&
             idlOS::strncasecmp("SIZE", (yyvsp[(7) - (8)].strval), 4) == 0)
    {
    }
    else
    {
          // error 처리

          ulpSetErrorCode( &gUlpParseInfo.mErrorMgr,
                           ulpERR_ABORT_COMP_Unterminated_String_Error );
          gUlpCOMPErrCode = ulpGetErrorSTATE( &gUlpParseInfo.mErrorMgr );
          COMPerror( ulpGetErrorMSG(&gUlpParseInfo.mErrorMgr) );
    }
  ;}
    break;

  case 1816:

/* Line 1455 of yacc.c  */
#line 9474 "ulpCompy.y"
    {
    // strMatch : SIZE, 2
    if(idlOS::strncasecmp("SIZE", (yyvsp[(2) - (4)].strval), 4) != 0)
    {
          // error 처리

          ulpSetErrorCode( &gUlpParseInfo.mErrorMgr,
                           ulpERR_ABORT_COMP_Unterminated_String_Error );
          gUlpCOMPErrCode = ulpGetErrorSTATE( &gUlpParseInfo.mErrorMgr );
          COMPerror( ulpGetErrorMSG(&gUlpParseInfo.mErrorMgr) );
    }
  ;}
    break;

  case 1817:

/* Line 1455 of yacc.c  */
#line 9489 "ulpCompy.y"
    {
    // if ( strMatch : SIZE, 2)
    // {
    //    if ( strMatch : REUSE, 4)
    //    else if ( strMatch : K, 4)
    //    else if ( strMatch : M, 4)
    //    else if ( strMatch : G, 4)
    // }
    if(idlOS::strncasecmp("SIZE", (yyvsp[(2) - (5)].strval), 4) != 0 ||
       idlOS::strncasecmp("REUSE", (yyvsp[(4) - (5)].strval), 5) != 0 &&
       idlOS::strncasecmp("K", (yyvsp[(4) - (5)].strval), 1) != 0 &&
       idlOS::strncasecmp("M", (yyvsp[(4) - (5)].strval), 1) != 0 &&
       idlOS::strncasecmp("G", (yyvsp[(4) - (5)].strval), 1) != 0)
    {
          // error 처리

          ulpSetErrorCode( &gUlpParseInfo.mErrorMgr,
                           ulpERR_ABORT_COMP_Unterminated_String_Error );
          gUlpCOMPErrCode = ulpGetErrorSTATE( &gUlpParseInfo.mErrorMgr );
          COMPerror( ulpGetErrorMSG(&gUlpParseInfo.mErrorMgr) );
    }
  ;}
    break;

  case 1818:

/* Line 1455 of yacc.c  */
#line 9513 "ulpCompy.y"
    {
    // strMatch : REUSE, 2
    if(idlOS::strncasecmp("REUSE", (yyvsp[(2) - (3)].strval), 5) != 0)
    {
          // error 처리

          ulpSetErrorCode( &gUlpParseInfo.mErrorMgr,
                           ulpERR_ABORT_COMP_Unterminated_String_Error );
          gUlpCOMPErrCode = ulpGetErrorSTATE( &gUlpParseInfo.mErrorMgr );
          COMPerror( ulpGetErrorMSG(&gUlpParseInfo.mErrorMgr) );
    }
  ;}
    break;

  case 1819:

/* Line 1455 of yacc.c  */
#line 9527 "ulpCompy.y"
    {
    // if ( strMatch : SIZE, 2 && REUSE, 5)
    // {
    //    if ( strMatch : K, 4)
    //    else if ( strMatch : M, 4)
    //    else if ( strMatch : G, 4)
    // }
    if(idlOS::strncasecmp("SIZE", (yyvsp[(2) - (6)].strval), 4) != 0 ||
       idlOS::strncasecmp("REUSE", (yyvsp[(5) - (6)].strval), 5) != 0 ||
       idlOS::strncasecmp("K", (yyvsp[(4) - (6)].strval), 1) != 0 &&
       idlOS::strncasecmp("M", (yyvsp[(4) - (6)].strval), 1) != 0 &&
       idlOS::strncasecmp("G", (yyvsp[(4) - (6)].strval), 1) != 0)
    {
          // error 처리

          ulpSetErrorCode( &gUlpParseInfo.mErrorMgr,
                           ulpERR_ABORT_COMP_Unterminated_String_Error );
          gUlpCOMPErrCode = ulpGetErrorSTATE( &gUlpParseInfo.mErrorMgr );
          COMPerror( ulpGetErrorMSG(&gUlpParseInfo.mErrorMgr) );
    }
  ;}
    break;

  case 1822:

/* Line 1455 of yacc.c  */
#line 9558 "ulpCompy.y"
    {
    // strMatch : AUTOEXTEND, 1
    if(idlOS::strncasecmp("AUTOEXTEND", (yyvsp[(1) - (2)].strval), 10) != 0)
    {
          // error 처리

          ulpSetErrorCode( &gUlpParseInfo.mErrorMgr,
                           ulpERR_ABORT_COMP_Unterminated_String_Error );
          gUlpCOMPErrCode = ulpGetErrorSTATE( &gUlpParseInfo.mErrorMgr );
          COMPerror( ulpGetErrorMSG(&gUlpParseInfo.mErrorMgr) );
    }
  ;}
    break;

  case 1823:

/* Line 1455 of yacc.c  */
#line 9572 "ulpCompy.y"
    {
    // strMatch : AUTOEXTEND, 1
    if(idlOS::strncasecmp("AUTOEXTEND", (yyvsp[(1) - (2)].strval), 10) != 0)
    {
          // error 처리

          ulpSetErrorCode( &gUlpParseInfo.mErrorMgr,
                           ulpERR_ABORT_COMP_Unterminated_String_Error );
          gUlpCOMPErrCode = ulpGetErrorSTATE( &gUlpParseInfo.mErrorMgr );
          COMPerror( ulpGetErrorMSG(&gUlpParseInfo.mErrorMgr) );
    }
  ;}
    break;

  case 1824:

/* Line 1455 of yacc.c  */
#line 9587 "ulpCompy.y"
    {
    // strMatch : AUTOEXTEND, 1
    if(idlOS::strncasecmp("AUTOEXTEND", (yyvsp[(1) - (4)].strval), 10) != 0)
    {
          // error 처리

          ulpSetErrorCode( &gUlpParseInfo.mErrorMgr,
                           ulpERR_ABORT_COMP_Unterminated_String_Error );
          gUlpCOMPErrCode = ulpGetErrorSTATE( &gUlpParseInfo.mErrorMgr );
          COMPerror( ulpGetErrorMSG(&gUlpParseInfo.mErrorMgr) );
    }
  ;}
    break;

  case 1825:

/* Line 1455 of yacc.c  */
#line 9602 "ulpCompy.y"
    {
    // strMatch : AUTOEXTEND, 1
    // strMatch : MAXSIZE, 3
    if(idlOS::strncasecmp("AUTOEXTEND", (yyvsp[(1) - (4)].strval), 10) != 0 ||
       idlOS::strncasecmp("MAXSIZE", (yyvsp[(3) - (4)].strval), 7) != 0)
    {
          // error 처리

          ulpSetErrorCode( &gUlpParseInfo.mErrorMgr,
                           ulpERR_ABORT_COMP_Unterminated_String_Error );
          gUlpCOMPErrCode = ulpGetErrorSTATE( &gUlpParseInfo.mErrorMgr );
          COMPerror( ulpGetErrorMSG(&gUlpParseInfo.mErrorMgr) );
    }
  ;}
    break;

  case 1826:

/* Line 1455 of yacc.c  */
#line 9618 "ulpCompy.y"
    {
    // strMatch : AUTOEXTEND, 1
    if(idlOS::strncasecmp("AUTOEXTEND", (yyvsp[(1) - (5)].strval), 10) != 0)
    {
          // error 처리

          ulpSetErrorCode( &gUlpParseInfo.mErrorMgr,
                           ulpERR_ABORT_COMP_Unterminated_String_Error );
          gUlpCOMPErrCode = ulpGetErrorSTATE( &gUlpParseInfo.mErrorMgr );
          COMPerror( ulpGetErrorMSG(&gUlpParseInfo.mErrorMgr) );
    }
  ;}
    break;

  case 1827:

/* Line 1455 of yacc.c  */
#line 9632 "ulpCompy.y"
    {
    // strMatch : AUTOEXTEND, 1
    // strMatch : K|M|G, 5
    if(idlOS::strncasecmp("AUTOEXTEND", (yyvsp[(1) - (6)].strval), 10) != 0 ||
       idlOS::strncasecmp("K", (yyvsp[(5) - (6)].strval), 1) != 0 &&
       idlOS::strncasecmp("M", (yyvsp[(5) - (6)].strval), 1) != 0 &&
       idlOS::strncasecmp("G", (yyvsp[(5) - (6)].strval), 1) != 0)
    {
          // error 처리

          ulpSetErrorCode( &gUlpParseInfo.mErrorMgr,
                           ulpERR_ABORT_COMP_Unterminated_String_Error );
          gUlpCOMPErrCode = ulpGetErrorSTATE( &gUlpParseInfo.mErrorMgr );
          COMPerror( ulpGetErrorMSG(&gUlpParseInfo.mErrorMgr) );
    }
  ;}
    break;

  case 1828:

/* Line 1455 of yacc.c  */
#line 9650 "ulpCompy.y"
    {
    // strMatch : AUTOEXTEND, 1
    // strMatch : MAXSIZE, 3
    // strMatch : UNLIMITED, 4
    if(idlOS::strncasecmp("AUTOEXTEND", (yyvsp[(1) - (4)].strval), 10) != 0 ||
       idlOS::strncasecmp("MAXSIZE", (yyvsp[(3) - (4)].strval), 7) != 0 ||
       idlOS::strncasecmp("UNLIMITED", (yyvsp[(4) - (4)].strval), 9) != 0)
    {
          // error 처리

          ulpSetErrorCode( &gUlpParseInfo.mErrorMgr,
                           ulpERR_ABORT_COMP_Unterminated_String_Error );
          gUlpCOMPErrCode = ulpGetErrorSTATE( &gUlpParseInfo.mErrorMgr );
          COMPerror( ulpGetErrorMSG(&gUlpParseInfo.mErrorMgr) );
    }
  ;}
    break;

  case 1829:

/* Line 1455 of yacc.c  */
#line 9671 "ulpCompy.y"
    {
    // if( strMatch : MAXSIZE, 1 && UNLIMITED, 2)
    if(idlOS::strncasecmp("MAXSIZE", (yyvsp[(1) - (2)].strval), 7) != 0 ||
       idlOS::strncasecmp("UNLIMITED", (yyvsp[(2) - (2)].strval), 9) != 0)
    {
          // error 처리

          ulpSetErrorCode( &gUlpParseInfo.mErrorMgr,
                           ulpERR_ABORT_COMP_Unterminated_String_Error );
          gUlpCOMPErrCode = ulpGetErrorSTATE( &gUlpParseInfo.mErrorMgr );
          COMPerror( ulpGetErrorMSG(&gUlpParseInfo.mErrorMgr) );
    }
  ;}
    break;

  case 1830:

/* Line 1455 of yacc.c  */
#line 9687 "ulpCompy.y"
    {
    // if( strMatch : MAXSIZE, 1)
    if(idlOS::strncasecmp("MAXSIZE", (yyvsp[(1) - (2)].strval), 7) != 0)
    {
          // error 처리

          ulpSetErrorCode( &gUlpParseInfo.mErrorMgr,
                           ulpERR_ABORT_COMP_Unterminated_String_Error );
          gUlpCOMPErrCode = ulpGetErrorSTATE( &gUlpParseInfo.mErrorMgr );
          COMPerror( ulpGetErrorMSG(&gUlpParseInfo.mErrorMgr) );
    }
  ;}
    break;

  case 1841:

/* Line 1455 of yacc.c  */
#line 9728 "ulpCompy.y"
    {
    // if ( strMatch : K, 2 )
    // else if ( strMatch : M, 2)
    // else if ( strMatch : G, 2)
    if(idlOS::strncasecmp("K", (yyvsp[(2) - (2)].strval), 1) != 0 &&
       idlOS::strncasecmp("M", (yyvsp[(2) - (2)].strval), 1) != 0 &&
       idlOS::strncasecmp("G", (yyvsp[(2) - (2)].strval), 1) != 0)
    {
          // error 처리

          ulpSetErrorCode( &gUlpParseInfo.mErrorMgr,
                           ulpERR_ABORT_COMP_Unterminated_String_Error );
          gUlpCOMPErrCode = ulpGetErrorSTATE( &gUlpParseInfo.mErrorMgr );
          COMPerror( ulpGetErrorMSG(&gUlpParseInfo.mErrorMgr) );
    }
  ;}
    break;

  case 1843:

/* Line 1455 of yacc.c  */
#line 9749 "ulpCompy.y"
    {
    // if ( strMatch : K, 2 )
    // else if ( strMatch : M, 2)
    // else if ( strMatch : G, 2)
    if(idlOS::strncasecmp("K", (yyvsp[(2) - (2)].strval), 1) != 0 &&
       idlOS::strncasecmp("M", (yyvsp[(2) - (2)].strval), 1) != 0 &&
       idlOS::strncasecmp("G", (yyvsp[(2) - (2)].strval), 1) != 0)
    {
          // error 처리

          ulpSetErrorCode( &gUlpParseInfo.mErrorMgr,
                           ulpERR_ABORT_COMP_Unterminated_String_Error );
          gUlpCOMPErrCode = ulpGetErrorSTATE( &gUlpParseInfo.mErrorMgr );
          COMPerror( ulpGetErrorMSG(&gUlpParseInfo.mErrorMgr) );
    }
  ;}
    break;

  case 1852:

/* Line 1455 of yacc.c  */
#line 9790 "ulpCompy.y"
    {
    // if ( strMatch: INCLUDING, 1 && CONTENTS, 2 )
    if(idlOS::strncasecmp("INCLUDING", (yyvsp[(1) - (3)].strval), 9) != 0 ||
       idlOS::strncasecmp("CONTENTS", (yyvsp[(2) - (3)].strval), 8) != 0)
    {
          // error 처리

          ulpSetErrorCode( &gUlpParseInfo.mErrorMgr,
                           ulpERR_ABORT_COMP_Unterminated_String_Error );
          gUlpCOMPErrCode = ulpGetErrorSTATE( &gUlpParseInfo.mErrorMgr );
          COMPerror( ulpGetErrorMSG(&gUlpParseInfo.mErrorMgr) );
    }
  ;}
    break;

  case 1853:

/* Line 1455 of yacc.c  */
#line 9805 "ulpCompy.y"
    {
    // if ( strMatch: INCLUDING, 1 && CONTENTS, 2 && DATAFILES, 4 )
    if(idlOS::strncasecmp("INCLUDING", (yyvsp[(1) - (5)].strval), 9) != 0 ||
       idlOS::strncasecmp("CONTENTS", (yyvsp[(2) - (5)].strval), 8) != 0 ||
       idlOS::strncasecmp("DATAFILES", (yyvsp[(4) - (5)].strval), 9) != 0)
    {
          // error 처리

          ulpSetErrorCode( &gUlpParseInfo.mErrorMgr,
                           ulpERR_ABORT_COMP_Unterminated_String_Error );
          gUlpCOMPErrCode = ulpGetErrorSTATE( &gUlpParseInfo.mErrorMgr );
          COMPerror( ulpGetErrorMSG(&gUlpParseInfo.mErrorMgr) );
    }
  ;}
    break;

  case 1898:

/* Line 1455 of yacc.c  */
#line 9958 "ulpCompy.y"
    {
    /* BUILD [IMMEDIATE | DEFERRED] */
    /* REFRESH [COMPLETE | FAST | FORCE] */
    /* NEVER REFRESH */
    idBool sPassFlag = ID_FALSE;

    if ( idlOS::strncasecmp( "BUILD", (yyvsp[(1) - (2)].strval), 5 ) == 0 )
    {
        if ( ( idlOS::strncasecmp( "IMMEDIATE", (yyvsp[(2) - (2)].strval), 9 ) == 0 ) ||
             ( idlOS::strncasecmp( "DEFERRED", (yyvsp[(2) - (2)].strval), 8 ) == 0 ) )
        {
            sPassFlag = ID_TRUE;
        }
        else
        {
            /* Nothing to do */
        }
    }
    else if ( idlOS::strncasecmp( "REFRESH", (yyvsp[(1) - (2)].strval), 7 ) == 0 )
    {
        if ( ( idlOS::strncasecmp( "FORCE", (yyvsp[(2) - (2)].strval), 5 ) == 0 ) ||
             ( idlOS::strncasecmp( "COMPLETE", (yyvsp[(2) - (2)].strval), 8 ) == 0 ) ||
             ( idlOS::strncasecmp( "FAST", (yyvsp[(2) - (2)].strval), 4 ) == 0 ) )
        {
            sPassFlag = ID_TRUE;
        }
        else
        {
            /* Nothing to do */
        }
    }
    else if ( idlOS::strncasecmp( "NEVER", (yyvsp[(1) - (2)].strval), 5 ) == 0 )
    {
        if ( idlOS::strncasecmp( "REFRESH", (yyvsp[(2) - (2)].strval), 7 ) == 0 )
        {
            sPassFlag = ID_TRUE;
        }
        else
        {
            /* Nothing to do */
        }
    }
    else
    {
        /* Nothing to do */
    }

    if ( sPassFlag != ID_TRUE )
    {
        ulpSetErrorCode( &gUlpParseInfo.mErrorMgr,
                         ulpERR_ABORT_COMP_Unterminated_String_Error );
        gUlpCOMPErrCode = ulpGetErrorSTATE( &gUlpParseInfo.mErrorMgr );
        COMPerror( ulpGetErrorMSG( &gUlpParseInfo.mErrorMgr ) );
    }
    else
    {
        /* Nothing to do */
    }
  ;}
    break;

  case 1899:

/* Line 1455 of yacc.c  */
#line 10018 "ulpCompy.y"
    {
    /* REFRESH [ON DEMAND | ON COMMIT] */
    if ( idlOS::strncasecmp( "REFRESH", (yyvsp[(1) - (2)].strval), 7 ) == 0 )
    {
        /* Nothing to do */
    }
    else
    {
        ulpSetErrorCode( &gUlpParseInfo.mErrorMgr,
                         ulpERR_ABORT_COMP_Unterminated_String_Error );
        gUlpCOMPErrCode = ulpGetErrorSTATE( &gUlpParseInfo.mErrorMgr );
        COMPerror( ulpGetErrorMSG( &gUlpParseInfo.mErrorMgr ) );
    }
  ;}
    break;

  case 1900:

/* Line 1455 of yacc.c  */
#line 10033 "ulpCompy.y"
    {
    /* REFRESH [COMPLETE | FAST | FORCE] [ON DEMAND | ON COMMIT] */
    idBool sPassFlag = ID_FALSE;

    if ( idlOS::strncasecmp( "REFRESH", (yyvsp[(1) - (3)].strval), 7 ) == 0 )
    {
        if ( ( idlOS::strncasecmp( "FORCE", (yyvsp[(2) - (3)].strval), 5 ) == 0 ) ||
             ( idlOS::strncasecmp( "COMPLETE", (yyvsp[(2) - (3)].strval), 8 ) == 0 ) ||
             ( idlOS::strncasecmp( "FAST", (yyvsp[(2) - (3)].strval), 4 ) == 0 ) )
        {
            sPassFlag = ID_TRUE;
        }
        else
        {
            /* Nothing to do */
        }
    }
    else
    {
        /* Nothing to do */
    }

    if ( sPassFlag != ID_TRUE )
    {
        ulpSetErrorCode( &gUlpParseInfo.mErrorMgr,
                         ulpERR_ABORT_COMP_Unterminated_String_Error );
        gUlpCOMPErrCode = ulpGetErrorSTATE( &gUlpParseInfo.mErrorMgr );
        COMPerror( ulpGetErrorMSG( &gUlpParseInfo.mErrorMgr ) );
    }
    else
    {
        /* Nothing to do */
    }
  ;}
    break;

  case 1901:

/* Line 1455 of yacc.c  */
#line 10068 "ulpCompy.y"
    {
    /* BUILD [IMMEDIATE | DEFERRED] REFRESH [COMPLETE | FAST | FORCE] */
    /* BUILD [IMMEDIATE | DEFERRED] NEVER REFRESH */
    idBool sPassFlag = ID_FALSE;

    if ( idlOS::strncasecmp( "BUILD", (yyvsp[(1) - (4)].strval), 5 ) == 0 )
    {
        if ( ( idlOS::strncasecmp( "IMMEDIATE", (yyvsp[(2) - (4)].strval), 9 ) == 0 ) ||
             ( idlOS::strncasecmp( "DEFERRED", (yyvsp[(2) - (4)].strval), 8 ) == 0 ) )
        {
            if ( idlOS::strncasecmp( "REFRESH", (yyvsp[(3) - (4)].strval), 7 ) == 0 )
            {
                if ( ( idlOS::strncasecmp( "FORCE", (yyvsp[(4) - (4)].strval), 5 ) == 0 ) ||
                     ( idlOS::strncasecmp( "COMPLETE", (yyvsp[(4) - (4)].strval), 8 ) == 0 ) ||
                     ( idlOS::strncasecmp( "FAST", (yyvsp[(4) - (4)].strval), 4 ) == 0 ) )
                {
                    sPassFlag = ID_TRUE;
                }
                else
                {
                    /* Nothing to do */
                }
            }
            else if ( idlOS::strncasecmp( "NEVER", (yyvsp[(3) - (4)].strval), 5 ) == 0 )
            {
                if ( idlOS::strncasecmp( "REFRESH", (yyvsp[(4) - (4)].strval), 7 ) == 0 )
                {
                    sPassFlag = ID_TRUE;
                }
                else
                {
                    /* Nothing to do */
                }
            }
            else
            {
                /* Nothing to do */
            }
        }
        else
        {
            /* Nothing to do */
        }
    }
    else
    {
        /* Nothing to do */
    }

    if ( sPassFlag != ID_TRUE )
    {
        ulpSetErrorCode( &gUlpParseInfo.mErrorMgr,
                         ulpERR_ABORT_COMP_Unterminated_String_Error );
        gUlpCOMPErrCode = ulpGetErrorSTATE( &gUlpParseInfo.mErrorMgr );
        COMPerror( ulpGetErrorMSG( &gUlpParseInfo.mErrorMgr ) );
    }
    else
    {
        /* Nothing to do */
    }
  ;}
    break;

  case 1902:

/* Line 1455 of yacc.c  */
#line 10130 "ulpCompy.y"
    {
    /* BUILD [IMMEDIATE | DEFERRED] REFRESH [ON DEMAND | ON COMMIT] */
    idBool sPassFlag = ID_FALSE;

    if ( idlOS::strncasecmp( "BUILD", (yyvsp[(1) - (4)].strval), 5 ) == 0 )
    {
        if ( ( idlOS::strncasecmp( "IMMEDIATE", (yyvsp[(2) - (4)].strval), 9 ) == 0 ) ||
             ( idlOS::strncasecmp( "DEFERRED", (yyvsp[(2) - (4)].strval), 8 ) == 0 ) )
        {
            if ( idlOS::strncasecmp( "REFRESH", (yyvsp[(3) - (4)].strval), 7 ) == 0 )
            {
                sPassFlag = ID_TRUE;
            }
            else
            {
                /* Nothing to do */
            }
        }
        else
        {
            /* Nothing to do */
        }
    }
    else
    {
        /* Nothing to do */
    }

    if ( sPassFlag != ID_TRUE )
    {
        ulpSetErrorCode( &gUlpParseInfo.mErrorMgr,
                         ulpERR_ABORT_COMP_Unterminated_String_Error );
        gUlpCOMPErrCode = ulpGetErrorSTATE( &gUlpParseInfo.mErrorMgr );
        COMPerror( ulpGetErrorMSG( &gUlpParseInfo.mErrorMgr ) );
    }
    else
    {
        /* Nothing to do */
    }
  ;}
    break;

  case 1903:

/* Line 1455 of yacc.c  */
#line 10171 "ulpCompy.y"
    {
    /* BUILD [IMMEDIATE | DEFERRED] REFRESH [COMPLETE | FAST | FORCE] [ON DEMAND | ON COMMIT] */
    idBool sPassFlag = ID_FALSE;

    if ( idlOS::strncasecmp( "BUILD", (yyvsp[(1) - (5)].strval), 5 ) == 0 )
    {
        if ( ( idlOS::strncasecmp( "IMMEDIATE", (yyvsp[(2) - (5)].strval), 9 ) == 0 ) ||
             ( idlOS::strncasecmp( "DEFERRED", (yyvsp[(2) - (5)].strval), 8 ) == 0 ) )
        {
            if ( idlOS::strncasecmp( "REFRESH", (yyvsp[(3) - (5)].strval), 7 ) == 0 )
            {
                if ( ( idlOS::strncasecmp( "FORCE", (yyvsp[(4) - (5)].strval), 5 ) == 0 ) ||
                     ( idlOS::strncasecmp( "COMPLETE", (yyvsp[(4) - (5)].strval), 8 ) == 0 ) ||
                     ( idlOS::strncasecmp( "FAST", (yyvsp[(4) - (5)].strval), 4 ) == 0 ) )
                {
                    sPassFlag = ID_TRUE;
                }
                else
                {
                    /* Nothing to do */
                }
            }
            else
            {
                /* Nothing to do */
            }
        }
        else
        {
            /* Nothing to do */
        }
    }
    else
    {
        /* Nothing to do */
    }

    if ( sPassFlag != ID_TRUE )
    {
        ulpSetErrorCode( &gUlpParseInfo.mErrorMgr,
                         ulpERR_ABORT_COMP_Unterminated_String_Error );
        gUlpCOMPErrCode = ulpGetErrorSTATE( &gUlpParseInfo.mErrorMgr );
        COMPerror( ulpGetErrorMSG( &gUlpParseInfo.mErrorMgr ) );
    }
    else
    {
        /* Nothing to do */
    }
  ;}
    break;

  case 1904:

/* Line 1455 of yacc.c  */
#line 10224 "ulpCompy.y"
    {
    if ( idlOS::strncasecmp( "DEMAND", (yyvsp[(2) - (2)].strval), 6 ) == 0 )
    {
        /* Nothing to do */
    }
    else
    {
        ulpSetErrorCode( &gUlpParseInfo.mErrorMgr,
                         ulpERR_ABORT_COMP_Unterminated_String_Error );
        gUlpCOMPErrCode = ulpGetErrorSTATE( &gUlpParseInfo.mErrorMgr );
        COMPerror( ulpGetErrorMSG( &gUlpParseInfo.mErrorMgr ) );
    }
  ;}
    break;

  case 1907:

/* Line 1455 of yacc.c  */
#line 10247 "ulpCompy.y"
    {
    /* REFRESH [COMPLETE | FAST | FORCE] */
    idBool sPassFlag = ID_FALSE;

    if ( idlOS::strncasecmp( "REFRESH", (yyvsp[(1) - (2)].strval), 7 ) == 0 )
    {
        if ( ( idlOS::strncasecmp( "FORCE", (yyvsp[(2) - (2)].strval), 5 ) == 0 ) ||
             ( idlOS::strncasecmp( "COMPLETE", (yyvsp[(2) - (2)].strval), 8 ) == 0 ) ||
             ( idlOS::strncasecmp( "FAST", (yyvsp[(2) - (2)].strval), 4 ) == 0 ) )
        {
            sPassFlag = ID_TRUE;
        }
        else
        {
            /* Nothing to do */
        }
    }
    else
    {
        /* Nothing to do */
    }

    if ( sPassFlag != ID_TRUE )
    {
        ulpSetErrorCode( &gUlpParseInfo.mErrorMgr,
                         ulpERR_ABORT_COMP_Unterminated_String_Error );
        gUlpCOMPErrCode = ulpGetErrorSTATE( &gUlpParseInfo.mErrorMgr );
        COMPerror( ulpGetErrorMSG( &gUlpParseInfo.mErrorMgr ) );
    }
    else
    {
        /* Nothing to do */
    }
  ;}
    break;

  case 1908:

/* Line 1455 of yacc.c  */
#line 10282 "ulpCompy.y"
    {
    /* REFRESH [ON DEMAND | ON COMMIT] */
    if ( idlOS::strncasecmp( "REFRESH", (yyvsp[(1) - (2)].strval), 7 ) == 0 )
    {
        /* Nothing to do */
    }
    else
    {
        ulpSetErrorCode( &gUlpParseInfo.mErrorMgr,
                         ulpERR_ABORT_COMP_Unterminated_String_Error );
        gUlpCOMPErrCode = ulpGetErrorSTATE( &gUlpParseInfo.mErrorMgr );
        COMPerror( ulpGetErrorMSG( &gUlpParseInfo.mErrorMgr ) );
    }
  ;}
    break;

  case 1909:

/* Line 1455 of yacc.c  */
#line 10297 "ulpCompy.y"
    {
    /* REFRESH [COMPLETE | FAST | FORCE] [ON DEMAND | ON COMMIT] */
    idBool sPassFlag = ID_FALSE;

    if ( idlOS::strncasecmp( "REFRESH", (yyvsp[(1) - (3)].strval), 7 ) == 0 )
    {
        if ( ( idlOS::strncasecmp( "FORCE", (yyvsp[(2) - (3)].strval), 5 ) == 0 ) ||
             ( idlOS::strncasecmp( "COMPLETE", (yyvsp[(2) - (3)].strval), 8 ) == 0 ) ||
             ( idlOS::strncasecmp( "FAST", (yyvsp[(2) - (3)].strval), 4 ) == 0 ) )
        {
            sPassFlag = ID_TRUE;
        }
        else
        {
            /* Nothing to do */
        }
    }
    else
    {
        /* Nothing to do */
    }

    if ( sPassFlag != ID_TRUE )
    {
        ulpSetErrorCode( &gUlpParseInfo.mErrorMgr,
                         ulpERR_ABORT_COMP_Unterminated_String_Error );
        gUlpCOMPErrCode = ulpGetErrorSTATE( &gUlpParseInfo.mErrorMgr );
        COMPerror( ulpGetErrorMSG( &gUlpParseInfo.mErrorMgr ) );
    }
    else
    {
        /* Nothing to do */
    }
  ;}
    break;

  case 1911:

/* Line 1455 of yacc.c  */
#line 10348 "ulpCompy.y"
    {
      if(idlOS::strncasecmp("JOB", (yyvsp[(2) - (11)].strval), 3) != 0)
      {
          // error 처리

          ulpSetErrorCode( &gUlpParseInfo.mErrorMgr,
                           ulpERR_ABORT_COMP_Unterminated_String_Error );
          gUlpCOMPErrCode = ulpGetErrorSTATE( &gUlpParseInfo.mErrorMgr );
          COMPerror( ulpGetErrorMSG(&gUlpParseInfo.mErrorMgr) );
      }
  ;}
    break;

  case 1924:

/* Line 1455 of yacc.c  */
#line 10400 "ulpCompy.y"
    {
      if(idlOS::strncasecmp("JOB", (yyvsp[(2) - (6)].strval), 3) != 0)
      {
          // error 처리

          ulpSetErrorCode( &gUlpParseInfo.mErrorMgr,
                           ulpERR_ABORT_COMP_Unterminated_String_Error );
          gUlpCOMPErrCode = ulpGetErrorSTATE( &gUlpParseInfo.mErrorMgr );
          COMPerror( ulpGetErrorMSG(&gUlpParseInfo.mErrorMgr) );
      }

  ;}
    break;

  case 1925:

/* Line 1455 of yacc.c  */
#line 10413 "ulpCompy.y"
    {
      if(idlOS::strncasecmp("JOB", (yyvsp[(2) - (6)].strval), 3) != 0)
      {
          // error 처리

          ulpSetErrorCode( &gUlpParseInfo.mErrorMgr,
                           ulpERR_ABORT_COMP_Unterminated_String_Error );
          gUlpCOMPErrCode = ulpGetErrorSTATE( &gUlpParseInfo.mErrorMgr );
          COMPerror( ulpGetErrorMSG(&gUlpParseInfo.mErrorMgr) );
      }

  ;}
    break;

  case 1926:

/* Line 1455 of yacc.c  */
#line 10426 "ulpCompy.y"
    {
      if(idlOS::strncasecmp("JOB", (yyvsp[(2) - (6)].strval), 3) != 0)
      {
          // error 처리

          ulpSetErrorCode( &gUlpParseInfo.mErrorMgr,
                           ulpERR_ABORT_COMP_Unterminated_String_Error );
          gUlpCOMPErrCode = ulpGetErrorSTATE( &gUlpParseInfo.mErrorMgr );
          COMPerror( ulpGetErrorMSG(&gUlpParseInfo.mErrorMgr) );
      }

  ;}
    break;

  case 1927:

/* Line 1455 of yacc.c  */
#line 10439 "ulpCompy.y"
    {
      if(idlOS::strncasecmp("JOB", (yyvsp[(2) - (5)].strval), 3) != 0)
      {
          // error 처리

          ulpSetErrorCode( &gUlpParseInfo.mErrorMgr,
                           ulpERR_ABORT_COMP_Unterminated_String_Error );
          gUlpCOMPErrCode = ulpGetErrorSTATE( &gUlpParseInfo.mErrorMgr );
          COMPerror( ulpGetErrorMSG(&gUlpParseInfo.mErrorMgr) );
      }
  ;}
    break;

  case 1928:

/* Line 1455 of yacc.c  */
#line 10451 "ulpCompy.y"
    {
      // BUG-41713 each job enable disable
      if(idlOS::strncasecmp("JOB", (yyvsp[(2) - (5)].strval), 3) != 0)
      {
          // error 처리

          ulpSetErrorCode( &gUlpParseInfo.mErrorMgr,
                           ulpERR_ABORT_COMP_Unterminated_String_Error );
          gUlpCOMPErrCode = ulpGetErrorSTATE( &gUlpParseInfo.mErrorMgr );
          COMPerror( ulpGetErrorMSG(&gUlpParseInfo.mErrorMgr) );
      }
  ;}
    break;

  case 1929:

/* Line 1455 of yacc.c  */
#line 10464 "ulpCompy.y"
    {
      // BUG-41713 each job enable disable
      if(idlOS::strncasecmp("JOB", (yyvsp[(2) - (5)].strval), 3) != 0)
      {
          // error 처리

          ulpSetErrorCode( &gUlpParseInfo.mErrorMgr,
                           ulpERR_ABORT_COMP_Unterminated_String_Error );
          gUlpCOMPErrCode = ulpGetErrorSTATE( &gUlpParseInfo.mErrorMgr );
          COMPerror( ulpGetErrorMSG(&gUlpParseInfo.mErrorMgr) );
      }
  ;}
    break;

  case 1930:

/* Line 1455 of yacc.c  */
#line 10480 "ulpCompy.y"
    {
      if(idlOS::strncasecmp("JOB", (yyvsp[(2) - (3)].strval), 3) != 0)
      {
          // error 처리

          ulpSetErrorCode( &gUlpParseInfo.mErrorMgr,
                           ulpERR_ABORT_COMP_Unterminated_String_Error );
          gUlpCOMPErrCode = ulpGetErrorSTATE( &gUlpParseInfo.mErrorMgr );
          COMPerror( ulpGetErrorMSG(&gUlpParseInfo.mErrorMgr) );
      }
  ;}
    break;

  case 1933:

/* Line 1455 of yacc.c  */
#line 10504 "ulpCompy.y"
    {
        // yyvsp is a internal variable for bison,
        // host value in/out type = HV_IN_TYPE,
        // host value file type = HV_FILE_NONE,
        // Does it need to transform the query? = TRUE
        // num of tokens = 2,
        // index of host value token = 1,
        // indexs of remove token = 0 (it means none)
        ulpValidateHostValue( yyvsp,
                              HV_IN_TYPE,
                              HV_FILE_NONE,
                              ID_TRUE,
                              (SInt)2,
                              (SInt)1,
                              (SInt)0
                            );
    ;}
    break;

  case 1934:

/* Line 1455 of yacc.c  */
#line 10522 "ulpCompy.y"
    {
        ulpValidateHostValue( yyvsp,
                              HV_IN_TYPE,
                              HV_FILE_BLOB,
                              ID_FALSE,
                              (SInt)4,
                              (SInt)2,
                              (SInt)1
                            );
    ;}
    break;

  case 1935:

/* Line 1455 of yacc.c  */
#line 10533 "ulpCompy.y"
    {
        ulpValidateHostValue( yyvsp,
                              HV_IN_TYPE,
                              HV_FILE_CLOB,
                              ID_FALSE,
                              (SInt)4,
                              (SInt)2,
                              (SInt)1
                            );
    ;}
    break;

  case 1936:

/* Line 1455 of yacc.c  */
#line 10544 "ulpCompy.y"
    {
        ulpValidateHostValue( yyvsp,
                              HV_IN_TYPE,
                              HV_FILE_NONE,
                              ID_TRUE,
                              (SInt)4,
                              (SInt)3,
                              (SInt)0
                            );
    ;}
    break;

  case 1937:

/* Line 1455 of yacc.c  */
#line 10555 "ulpCompy.y"
    {
        ulpValidateHostValue( yyvsp,
                              HV_IN_TYPE,
                              HV_FILE_BLOB,
                              ID_FALSE,
                              (SInt)6,
                              (SInt)4,
                              (SInt)3
                            );
    ;}
    break;

  case 1938:

/* Line 1455 of yacc.c  */
#line 10566 "ulpCompy.y"
    {
        ulpValidateHostValue( yyvsp,
                              HV_IN_TYPE,
                              HV_FILE_CLOB,
                              ID_FALSE,
                              (SInt)6,
                              (SInt)4,
                              (SInt)3
                            );
    ;}
    break;

  case 1941:

/* Line 1455 of yacc.c  */
#line 10585 "ulpCompy.y"
    {
        ulpValidateHostValue( yyvsp,
                              HV_IN_TYPE,
                              HV_FILE_NONE,
                              ID_TRUE,
                              (SInt)3,
                              (SInt)1,
                              (SInt)2
                            );
    ;}
    break;

  case 1942:

/* Line 1455 of yacc.c  */
#line 10596 "ulpCompy.y"
    {
        ulpValidateHostValue( yyvsp,
                              HV_OUT_PSM_TYPE,
                              HV_FILE_NONE,
                              ID_TRUE,
                              (SInt)3,
                              (SInt)1,
                              (SInt)2
                            );
    ;}
    break;

  case 1943:

/* Line 1455 of yacc.c  */
#line 10607 "ulpCompy.y"
    {
        ulpValidateHostValue( yyvsp,
                              HV_INOUT_TYPE,
                              HV_FILE_NONE,
                              ID_TRUE,
                              (SInt)4,
                              (SInt)1,
                              (SInt)32
                            );
    ;}
    break;

  case 1944:

/* Line 1455 of yacc.c  */
#line 10618 "ulpCompy.y"
    {
        ulpValidateHostValue( yyvsp,
                              HV_IN_TYPE,
                              HV_FILE_NONE,
                              ID_TRUE,
                              (SInt)2,
                              (SInt)1,
                              (SInt)0
                            );
    ;}
    break;

  case 1945:

/* Line 1455 of yacc.c  */
#line 10629 "ulpCompy.y"
    {
        ulpValidateHostValue( yyvsp,
                              HV_IN_TYPE,
                              HV_FILE_BLOB,
                              ID_TRUE,
                              (SInt)4,
                              (SInt)2,
                              (SInt)1
                            );
    ;}
    break;

  case 1946:

/* Line 1455 of yacc.c  */
#line 10640 "ulpCompy.y"
    {
        ulpValidateHostValue( yyvsp,
                              HV_IN_TYPE,
                              HV_FILE_CLOB,
                              ID_TRUE,
                              (SInt)4,
                              (SInt)2,
                              (SInt)1
                            );
    ;}
    break;

  case 1947:

/* Line 1455 of yacc.c  */
#line 10654 "ulpCompy.y"
    {
        ulpSymTElement *sSymNode;

        // in host variable
        if ( ( sSymNode = gUlpScopeT.ulpSLookupAll( (yyvsp[(3) - (3)].strval)+1, gUlpCurDepth ) ) == NULL )
        {
            //host 변수 못찾는 error
            // error 처리

            ulpSetErrorCode( &gUlpParseInfo.mErrorMgr,
                             ulpERR_ABORT_COMP_Unknown_Hostvar_Error,
                             (yyvsp[(3) - (3)].strval)+1 );
            gUlpCOMPErrCode = ulpGetErrorSTATE( &gUlpParseInfo.mErrorMgr );
            COMPerror( ulpGetErrorMSG(&gUlpParseInfo.mErrorMgr) );
        }
        else
        {
            if( (sSymNode -> mType != H_BLOBLOCATOR) &&
                (sSymNode -> mType != H_CLOBLOCATOR) )
            {
                //host 변수 type error
                // error 처리

                ulpSetErrorCode( &gUlpParseInfo.mErrorMgr,
                                 ulpERR_ABORT_COMP_Lob_Locator_Error );
                gUlpCOMPErrCode = ulpGetErrorSTATE( &gUlpParseInfo.mErrorMgr );
                COMPerror( ulpGetErrorMSG(&gUlpParseInfo.mErrorMgr) );
            }

            gUlpCodeGen.ulpGenAddHostVarList( (yyvsp[(3) - (3)].strval)+1, sSymNode ,
                                              gUlpIndName, NULL, NULL, HV_IN_TYPE);

            gUlpCodeGen.ulpIncHostVarNum( 1 );
            gUlpCodeGen.ulpGenAddHostVarArr( 1 );
        }

    ;}
    break;

  case 1948:

/* Line 1455 of yacc.c  */
#line 10692 "ulpCompy.y"
    {
        ulpSymTElement *sSymNode;

        // in host variable
        if ( ( sSymNode = gUlpScopeT.ulpSLookupAll( (yyvsp[(1) - (1)].strval)+1, gUlpCurDepth ) ) == NULL )
        {
            //host 변수 못찾는 error
            // error 처리

            ulpSetErrorCode( &gUlpParseInfo.mErrorMgr,
                             ulpERR_ABORT_COMP_Unknown_Hostvar_Error,
                             (yyvsp[(1) - (1)].strval)+1 );
            gUlpCOMPErrCode = ulpGetErrorSTATE( &gUlpParseInfo.mErrorMgr );
            COMPerror( ulpGetErrorMSG(&gUlpParseInfo.mErrorMgr) );
        }
        else
        {
            gUlpCodeGen.ulpGenAddHostVarList( (yyvsp[(1) - (1)].strval)+1, sSymNode ,
                                              gUlpIndName, NULL, NULL, HV_IN_TYPE);

            gUlpCodeGen.ulpIncHostVarNum( 1 );
            gUlpCodeGen.ulpGenAddHostVarArr( 1 );
        }
    ;}
    break;

  case 1950:

/* Line 1455 of yacc.c  */
#line 10721 "ulpCompy.y"
    {
        gUlpCodeGen.ulpGenRemoveQueryToken( (yyvsp[(1) - (2)].strval) );
    ;}
    break;

  case 1951:

/* Line 1455 of yacc.c  */
#line 10728 "ulpCompy.y"
    {
        ulpValidateHostValue( yyvsp,
                              HV_OUT_TYPE,
                              HV_FILE_NONE,
                              ID_FALSE,
                              (SInt)2,
                              (SInt)1,
                              (SInt)1
                            );
    ;}
    break;

  case 1952:

/* Line 1455 of yacc.c  */
#line 10739 "ulpCompy.y"
    {
        ulpValidateHostValue( yyvsp,
                              HV_OUT_TYPE,
                              HV_FILE_NONE,
                              ID_FALSE,
                              (SInt)4,
                              (SInt)3,
                              (SInt)23
                            );
    ;}
    break;

  case 1953:

/* Line 1455 of yacc.c  */
#line 10750 "ulpCompy.y"
    {
        ulpValidateHostValue( yyvsp,
                              HV_OUT_TYPE,
                              HV_FILE_BLOB,
                              ID_FALSE,
                              (SInt)4,
                              (SInt)2,
                              (SInt)12
                            );
    ;}
    break;

  case 1954:

/* Line 1455 of yacc.c  */
#line 10761 "ulpCompy.y"
    {
        ulpValidateHostValue( yyvsp,
                              HV_OUT_TYPE,
                              HV_FILE_CLOB,
                              ID_FALSE,
                              (SInt)4,
                              (SInt)2,
                              (SInt)12
                            );
    ;}
    break;

  case 1955:

/* Line 1455 of yacc.c  */
#line 10772 "ulpCompy.y"
    {
        ulpValidateHostValue( yyvsp,
                              HV_OUT_TYPE,
                              HV_FILE_BLOB,
                              ID_FALSE,
                              (SInt)6,
                              (SInt)4,
                              (SInt)234
                            );
    ;}
    break;

  case 1956:

/* Line 1455 of yacc.c  */
#line 10783 "ulpCompy.y"
    {
        ulpValidateHostValue( yyvsp,
                              HV_OUT_TYPE,
                              HV_FILE_CLOB,
                              ID_FALSE,
                              (SInt)6,
                              (SInt)4,
                              (SInt)234
                            );
    ;}
    break;

  case 1958:

/* Line 1455 of yacc.c  */
#line 10798 "ulpCompy.y"
    {
        gUlpCodeGen.ulpGenRemoveQueryToken( (yyvsp[(1) - (2)].strval) );
    ;}
    break;

  case 1961:

/* Line 1455 of yacc.c  */
#line 10810 "ulpCompy.y"
    {
        ulpValidateHostValue( yyvsp,
                              HV_OUT_TYPE,
                              HV_FILE_NONE,
                              ID_FALSE,
                              (SInt)2,
                              (SInt)1,
                              (SInt)1
                            );
    ;}
    break;

  case 1962:

/* Line 1455 of yacc.c  */
#line 10821 "ulpCompy.y"
    {
        ulpValidateHostValue( yyvsp,
                              HV_OUT_TYPE,
                              HV_FILE_NONE,
                              ID_FALSE,
                              (SInt)4,
                              (SInt)3,
                              (SInt)23
                            );
    ;}
    break;

  case 1963:

/* Line 1455 of yacc.c  */
#line 10832 "ulpCompy.y"
    {
        ulpValidateHostValue( yyvsp,
                              HV_OUT_TYPE,
                              HV_FILE_BLOB,
                              ID_FALSE,
                              (SInt)4,
                              (SInt)2,
                              (SInt)12
                            );
    ;}
    break;

  case 1964:

/* Line 1455 of yacc.c  */
#line 10843 "ulpCompy.y"
    {
        ulpValidateHostValue( yyvsp,
                              HV_OUT_TYPE,
                              HV_FILE_CLOB,
                              ID_FALSE,
                              (SInt)4,
                              (SInt)2,
                              (SInt)12
                            );
    ;}
    break;

  case 1965:

/* Line 1455 of yacc.c  */
#line 10854 "ulpCompy.y"
    {
        ulpValidateHostValue( yyvsp,
                              HV_OUT_TYPE,
                              HV_FILE_BLOB,
                              ID_FALSE,
                              (SInt)6,
                              (SInt)4,
                              (SInt)234
                            );
    ;}
    break;

  case 1966:

/* Line 1455 of yacc.c  */
#line 10865 "ulpCompy.y"
    {
        ulpValidateHostValue( yyvsp,
                              HV_OUT_TYPE,
                              HV_FILE_CLOB,
                              ID_FALSE,
                              (SInt)6,
                              (SInt)4,
                              (SInt)234
                            );
    ;}
    break;

  case 1967:

/* Line 1455 of yacc.c  */
#line 10879 "ulpCompy.y"
    {
        ulpValidateHostValue( yyvsp,
                              HV_OUT_PSM_TYPE,
                              HV_FILE_NONE,
                              ID_TRUE,
                              (SInt)2,
                              (SInt)1,
                              (SInt)0
                            );
    ;}
    break;

  case 1968:

/* Line 1455 of yacc.c  */
#line 10894 "ulpCompy.y"
    {
        ulpSymTElement *sSymNode;

        gUlpCodeGen.ulpGenRemoveQueryToken( (yyvsp[(2) - (2)].strval) );

        // out host variable
        if ( ( sSymNode = gUlpScopeT.ulpSLookupAll( (yyvsp[(2) - (2)].strval)+1, gUlpCurDepth ) ) == NULL )
        {
            //host 변수 못찾는 error
            // error 처리

            ulpSetErrorCode( &gUlpParseInfo.mErrorMgr,
                             ulpERR_ABORT_COMP_Unknown_Hostvar_Error,
                             (yyvsp[(2) - (2)].strval)+1 );
            gUlpCOMPErrCode = ulpGetErrorSTATE( &gUlpParseInfo.mErrorMgr );
            COMPerror( ulpGetErrorMSG(&gUlpParseInfo.mErrorMgr) );
        }
        else
        {
            idlOS::snprintf( gUlpFileOptName, MAX_HOSTVAR_NAME_SIZE * 2,
                             "%s", (yyvsp[(2) - (2)].strval)+1);
        }
    ;}
    break;

  case 1970:

/* Line 1455 of yacc.c  */
#line 10922 "ulpCompy.y"
    {
        gUlpCodeGen.ulpGenRemoveQueryToken( (yyvsp[(1) - (1)].strval) );
    ;}
    break;

  case 1973:

/* Line 1455 of yacc.c  */
#line 10934 "ulpCompy.y"
    {
        ulpSymTNode *sFieldSymNode;

        gUlpCodeGen.ulpGenRemoveQueryToken( (yyvsp[(2) - (2)].strval) );
        if ( ( gUlpIndNode = gUlpScopeT.ulpSLookupAll( (yyvsp[(2) - (2)].strval)+1, gUlpCurDepth ) ) == NULL )
        {
            //host 변수 못찾는 error
            // error 처리

            ulpSetErrorCode( &gUlpParseInfo.mErrorMgr,
                             ulpERR_ABORT_COMP_Unknown_Hostvar_Error,
                             (yyvsp[(2) - (2)].strval)+1 );
            gUlpCOMPErrCode = ulpGetErrorSTATE( &gUlpParseInfo.mErrorMgr );
            COMPerror( ulpGetErrorMSG(&gUlpParseInfo.mErrorMgr) );
        }
        else
        {
            /* BUG-28566: The indicator must be the type of SQLLEN or int or long(32bit). */
            if( (gUlpIndNode->mIsstruct   == ID_TRUE) &&
                (gUlpIndNode->mStructLink != NULL) )
            {   // indicator가 struct type이라면 모든 필드들은 int/long or SQLLEN type이어야한다.
                // indicator symbol node(gUlpIndNode)안의 struct node pointer(mStructLink)
                // 를 따라가 field hash table(mChild)의 symbol node(mInOrderList)를
                // 얻어온다.
                sFieldSymNode = gUlpIndNode->mStructLink->mChild->mInOrderList;

                // struct 안의 각 필드들의 type을 검사한다.
                while ( sFieldSymNode != NULL )
                {
                    switch ( sFieldSymNode->mElement.mType )
                    {
                        case H_INT:
                        case H_BLOBLOCATOR:
                        case H_CLOBLOCATOR:
                            break;
                        case H_LONG:
                            // indicator는 무조건 4byte이어야함.
                            if( ID_SIZEOF(long) != 4 )
                            {
                                // 잘못된 indicator type error 처리
                                ulpSetErrorCode( &gUlpParseInfo.mErrorMgr,
                                                 ulpERR_ABORT_COMP_Wrong_IndicatorType_Error,
                                                 sFieldSymNode->mElement.mName );
                                gUlpCOMPErrCode = ulpGetErrorSTATE( &gUlpParseInfo.mErrorMgr );
                                COMPerror( ulpGetErrorMSG(&gUlpParseInfo.mErrorMgr) );
                            }
                            break;
                        default:
                            // 잘못된 indicator type error 처리
                            ulpSetErrorCode( &gUlpParseInfo.mErrorMgr,
                                             ulpERR_ABORT_COMP_Wrong_IndicatorType_Error,
                                             sFieldSymNode->mElement.mName );
                            gUlpCOMPErrCode = ulpGetErrorSTATE( &gUlpParseInfo.mErrorMgr );
                            COMPerror( ulpGetErrorMSG(&gUlpParseInfo.mErrorMgr) );
                            break;
                    }
                    // 다음 field symbol node를 가리킨다.
                    sFieldSymNode = sFieldSymNode->mInOrderNext;
                }
            }
            else
            {   // struct type이 아니다.
                switch( gUlpIndNode->mType )
                {   // must be the type of SQLLEN or int or long(32bit).
                    case H_INT:
                    case H_BLOBLOCATOR:
                    case H_CLOBLOCATOR:
                        break;
                    case H_LONG:
                        // indicator는 무조건 4byte이어야함.
                        if( ID_SIZEOF(long) != 4 )
                        {
                            // 잘못된 indicator type error 처리
                            ulpSetErrorCode( &gUlpParseInfo.mErrorMgr,
                                             ulpERR_ABORT_COMP_Wrong_IndicatorType_Error,
                                             sFieldSymNode->mElement.mName );
                            gUlpCOMPErrCode = ulpGetErrorSTATE( &gUlpParseInfo.mErrorMgr );
                            COMPerror( ulpGetErrorMSG(&gUlpParseInfo.mErrorMgr) );
                        }
                        break;
                    default:
                        // 잘못된 indicator type error 처리
                        ulpSetErrorCode( &gUlpParseInfo.mErrorMgr,
                                         ulpERR_ABORT_COMP_Wrong_IndicatorType_Error,
                                         gUlpIndNode->mName );
                        gUlpCOMPErrCode = ulpGetErrorSTATE( &gUlpParseInfo.mErrorMgr );
                        COMPerror( ulpGetErrorMSG(&gUlpParseInfo.mErrorMgr) );
                        break;
                }
            }

            idlOS::snprintf( gUlpIndName, MAX_HOSTVAR_NAME_SIZE * 2,
                             "%s", (yyvsp[(2) - (2)].strval)+1);
        }
    ;}
    break;

  case 1975:

/* Line 1455 of yacc.c  */
#line 11034 "ulpCompy.y"
    {
        gUlpCodeGen.ulpGenRemoveQueryToken( (yyvsp[(1) - (1)].strval) );
    ;}
    break;

  case 1983:

/* Line 1455 of yacc.c  */
#line 11056 "ulpCompy.y"
    {
      if ( idlOS::strncasecmp( "APPEND", (yyvsp[(2) - (2)].strval), 6 ) != 0 )
      {
          ulpSetErrorCode( &gUlpParseInfo.mErrorMgr,
                           ulpERR_ABORT_COMP_Unterminated_String_Error );
          gUlpCOMPErrCode = ulpGetErrorSTATE( &gUlpParseInfo.mErrorMgr );
          COMPerror( ulpGetErrorMSG( &gUlpParseInfo.mErrorMgr ) );
      }
      else
      {
          /* Nothing to do */
      }
  ;}
    break;



/* Line 1455 of yacc.c  */
#line 15805 "ulpCompy.cpp"
      default: break;
    }
  YY_SYMBOL_PRINT ("-> $$ =", yyr1[yyn], &yyval, &yyloc);

  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);

  *++yyvsp = yyval;

  /* Now `shift' the result of the reduction.  Determine what state
     that goes to, based on the state we popped back to and the rule
     number reduced by.  */

  yyn = yyr1[yyn];

  yystate = yypgoto[yyn - YYNTOKENS] + *yyssp;
  if (0 <= yystate && yystate <= YYLAST && yycheck[yystate] == *yyssp)
    yystate = yytable[yystate];
  else
    yystate = yydefgoto[yyn - YYNTOKENS];

  goto yynewstate;


/*------------------------------------.
| yyerrlab -- here on detecting error |
`------------------------------------*/
yyerrlab:
  /* If not already recovering from an error, report this error.  */
  if (!yyerrstatus)
    {
      ++yynerrs;
#if ! YYERROR_VERBOSE
      yyerror (YY_("syntax error"));
#else
      {
	YYSIZE_T yysize = yysyntax_error (0, yystate, yychar);
	if (yymsg_alloc < yysize && yymsg_alloc < YYSTACK_ALLOC_MAXIMUM)
	  {
	    YYSIZE_T yyalloc = 2 * yysize;
	    if (! (yysize <= yyalloc && yyalloc <= YYSTACK_ALLOC_MAXIMUM))
	      yyalloc = YYSTACK_ALLOC_MAXIMUM;
	    if (yymsg != yymsgbuf)
	      YYSTACK_FREE (yymsg);
	    yymsg = (char *) YYSTACK_ALLOC (yyalloc);
	    if (yymsg)
	      yymsg_alloc = yyalloc;
	    else
	      {
		yymsg = yymsgbuf;
		yymsg_alloc = sizeof yymsgbuf;
	      }
	  }

	if (0 < yysize && yysize <= yymsg_alloc)
	  {
	    (void) yysyntax_error (yymsg, yystate, yychar);
	    yyerror (yymsg);
	  }
	else
	  {
	    yyerror (YY_("syntax error"));
	    if (yysize != 0)
	      goto yyexhaustedlab;
	  }
      }
#endif
    }



  if (yyerrstatus == 3)
    {
      /* If just tried and failed to reuse lookahead token after an
	 error, discard it.  */

      if (yychar <= YYEOF)
	{
	  /* Return failure if at end of input.  */
	  if (yychar == YYEOF)
	    YYABORT;
	}
      else
	{
	  yydestruct ("Error: discarding",
		      yytoken, &yylval);
	  yychar = YYEMPTY;
	}
    }

  /* Else will try to reuse lookahead token after shifting the error
     token.  */
  goto yyerrlab1;


/*---------------------------------------------------.
| yyerrorlab -- error raised explicitly by YYERROR.  |
`---------------------------------------------------*/
yyerrorlab:

  /* Pacify compilers like GCC when the user code never invokes
     YYERROR and the label yyerrorlab therefore never appears in user
     code.  */
  if (/*CONSTCOND*/ 0)
     goto yyerrorlab;

  /* Do not reclaim the symbols of the rule which action triggered
     this YYERROR.  */
  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);
  yystate = *yyssp;
  goto yyerrlab1;


/*-------------------------------------------------------------.
| yyerrlab1 -- common code for both syntax error and YYERROR.  |
`-------------------------------------------------------------*/
yyerrlab1:
  yyerrstatus = 3;	/* Each real token shifted decrements this.  */

  for (;;)
    {
      yyn = yypact[yystate];
      if (yyn != YYPACT_NINF)
	{
	  yyn += YYTERROR;
	  if (0 <= yyn && yyn <= YYLAST && yycheck[yyn] == YYTERROR)
	    {
	      yyn = yytable[yyn];
	      if (0 < yyn)
		break;
	    }
	}

      /* Pop the current state because it cannot handle the error token.  */
      if (yyssp == yyss)
	YYABORT;


      yydestruct ("Error: popping",
		  yystos[yystate], yyvsp);
      YYPOPSTACK (1);
      yystate = *yyssp;
      YY_STACK_PRINT (yyss, yyssp);
    }

  *++yyvsp = yylval;


  /* Shift the error token.  */
  YY_SYMBOL_PRINT ("Shifting", yystos[yyn], yyvsp, yylsp);

  yystate = yyn;
  goto yynewstate;


/*-------------------------------------.
| yyacceptlab -- YYACCEPT comes here.  |
`-------------------------------------*/
yyacceptlab:
  yyresult = 0;
  goto yyreturn;

/*-----------------------------------.
| yyabortlab -- YYABORT comes here.  |
`-----------------------------------*/
yyabortlab:
  yyresult = 1;
  goto yyreturn;

#if !defined(yyoverflow) || YYERROR_VERBOSE
/*-------------------------------------------------.
| yyexhaustedlab -- memory exhaustion comes here.  |
`-------------------------------------------------*/
yyexhaustedlab:
  yyerror (YY_("memory exhausted"));
  yyresult = 2;
  /* Fall through.  */
#endif

yyreturn:
  if (yychar != YYEMPTY)
     yydestruct ("Cleanup: discarding lookahead",
		 yytoken, &yylval);
  /* Do not reclaim the symbols of the rule which action triggered
     this YYABORT or YYACCEPT.  */
  YYPOPSTACK (yylen);
  YY_STACK_PRINT (yyss, yyssp);
  while (yyssp != yyss)
    {
      yydestruct ("Cleanup: popping",
		  yystos[*yyssp], yyvsp);
      YYPOPSTACK (1);
    }
#ifndef yyoverflow
  if (yyss != yyssa)
    YYSTACK_FREE (yyss);
#endif
#if YYERROR_VERBOSE
  if (yymsg != yymsgbuf)
    YYSTACK_FREE (yymsg);
#endif
  /* Make sure YYID is used.  */
  return YYID (yyresult);
}



/* Line 1675 of yacc.c  */
#line 11098 "ulpCompy.y"



int doCOMPparse( SChar *aFilename )
{
/***********************************************************************
 *
 * Description :
 *      COMPparse precompiling을 시작되게 하는 initial 함수.
 *
 ***********************************************************************/
    int sRes;

    ulpCOMPInitialize( aFilename );

    gUlpCOMPifstackMgr[++gUlpCOMPifstackInd] = new ulpPPifstackMgr();

    if( gDontPrint2file != ID_TRUE )
    {
        gUlpCodeGen.ulpGenInitPrint();

        /* BUG-42357 */
        if (gUlpProgOption.mOptLineMacro == ID_TRUE)
        {
            if (gUlpProgOption.mOptParseInfo == PARSE_FULL)
            {
                gUlpCodeGen.ulpGenSetCurFileInfo( COMPlineno, 0, gUlpProgOption.mInFile );
            }
            else
            {
                gUlpCodeGen.ulpGenSetCurFileInfo( COMPlineno, -1, gUlpProgOption.mInFile );
            }

            gUlpCodeGen.ulpGenPrintLineMacro();
        }
    }

    sRes = COMPparse( );

    gUlpCodeGen.ulpGenWriteFile();

    delete gUlpCOMPifstackMgr[gUlpCOMPifstackInd];

    gUlpCOMPifstackInd--;

    ulpCOMPFinalize();

    return sRes;
}


idBool ulpCOMPCheckArray( ulpSymTElement *aSymNode )
{
/***********************************************************************
 *
 * Description :
 *      array binding을 해야할지(isarr을 true로 set할지) 여부를 체크하기 위한 함수.
 *      struct A { int a[10]; } sA; 의 경우 isarr를 true로 set하고, isstruct를 
 *      false로 set하기 위해 사용됨.
 *
 ***********************************************************************/
    ulpSymTNode *sFieldSymNode;

    sFieldSymNode = aSymNode->mStructLink->mChild->mInOrderList;

    IDE_TEST( sFieldSymNode == NULL );

    while ( sFieldSymNode != NULL )
    {
        switch ( sFieldSymNode->mElement.mType )
        {
            case H_CLOB:
            case H_BLOB:
            case H_NUMERIC:
            case H_NIBBLE:
            case H_BIT:
            case H_BYTES:
            case H_BINARY:
            case H_BINARY2:  /* BUG-46418 */
            case H_CHAR:
            case H_NCHAR:
            case H_CLOB_FILE:
            case H_BLOB_FILE:
                IDE_TEST( sFieldSymNode->mElement.mArraySize2[0] == '\0' );
                break;
            case H_VARCHAR:
            case H_NVARCHAR:
                IDE_TEST( 1 );
                break;
            default:
                IDE_TEST( sFieldSymNode->mElement.mIsarray != ID_TRUE );
                break;
        }
        sFieldSymNode = sFieldSymNode->mInOrderNext;
    }

    return ID_TRUE;

    IDE_EXCEPTION_END;

    return ID_FALSE;
}


void ulpValidateHostValue( void         *yyvsp,
                           ulpHVarType   aInOutType,
                           ulpHVFileType aFileType,
                           idBool        aTransformQuery,
                           SInt          aNumofTokens,
                           SInt          aHostValIndex,
                           SInt          aRemoveTokIndexs )
{
/***********************************************************************
 *
 * Description :
 *      host 변수가 유효한지 확인하며, 유효하다면 ulpGenHostVarList 에 추가한다.
 *      aNumofTokens는 총 토큰들의 수,
 *      aHostValIndex 는 호스트 변수가 몇번째 토큰에 위치하는지를 나타내며,
 *      aRemoveTokIndexs는 SQL쿼리변환시 몇번째 토큰에 위치하는 토큰들을 제거할지를 나타내준다.
 *      ex> aRemoveTokIndexs이 123이면 1,2,3 에 위치하는 토큰들을 제거해준다.
 *
 ***********************************************************************/
    SInt            sIndexs, sMod;
    SChar          *sFileOptName;
    ulpSymTElement *sSymNode;
    ulpGENhvType    sHVType;
    ulpGENhvType    sArrayStructType;

    switch( aInOutType )
    {
        case HV_IN_TYPE:
            sArrayStructType = IN_GEN_ARRAYSTRUCT;
            break;
        case HV_OUT_TYPE:
        case HV_OUT_PSM_TYPE:
            sArrayStructType = OUT_GEN_ARRAYSTRUCT;
            break;
        case HV_INOUT_TYPE:
            sArrayStructType = INOUT_GEN_ARRAYSTRUCT;
            break;
        default:
            sArrayStructType = GEN_GENERAL;
            break;
    }

    if ( sArrayStructType == INOUT_GEN_ARRAYSTRUCT )
    {
        if ( ((gUlpCodeGen.ulpGenGetEmSQLInfo()->mHostValueType) == IN_GEN_ARRAYSTRUCT) ||
             ((gUlpCodeGen.ulpGenGetEmSQLInfo()->mHostValueType) == OUT_GEN_ARRAYSTRUCT))
        {
            // error 처리
            // array struct type은 다른 host 변수와 같이 올수 없다.

            ulpSetErrorCode( &gUlpParseInfo.mErrorMgr,
                             ulpERR_ABORT_COMP_Repeat_Array_Struct_Error,
                             (*(((YYSTYPE *)yyvsp)[aHostValIndex - aNumofTokens].strval)==':')?
                             (((YYSTYPE *)yyvsp)[aHostValIndex - aNumofTokens].strval)+1:
                             (((YYSTYPE *)yyvsp)[aHostValIndex - aNumofTokens].strval)
                           );
            gUlpCOMPErrCode = ulpGetErrorSTATE( &gUlpParseInfo.mErrorMgr );
            COMPerror( ulpGetErrorMSG(&gUlpParseInfo.mErrorMgr) );
        }
    }
    else
    {
        if ( (gUlpCodeGen.ulpGenGetEmSQLInfo()->mHostValueType)
             == sArrayStructType )
        {
            // error 처리
            // array struct type은 다른 host 변수와 같이 올수 없다.

            ulpSetErrorCode( &gUlpParseInfo.mErrorMgr,
                             ulpERR_ABORT_COMP_Repeat_Array_Struct_Error,
                             (*(((YYSTYPE *)yyvsp)[aHostValIndex - aNumofTokens].strval)==':')?
                             (((YYSTYPE *)yyvsp)[aHostValIndex - aNumofTokens].strval)+1:
                             (((YYSTYPE *)yyvsp)[aHostValIndex - aNumofTokens].strval)
                           );
            gUlpCOMPErrCode = ulpGetErrorSTATE( &gUlpParseInfo.mErrorMgr );
            COMPerror( ulpGetErrorMSG(&gUlpParseInfo.mErrorMgr) );
        }
    }

    // lookup host variable
    if ( ( sSymNode = gUlpScopeT.ulpSLookupAll(
                                    (((YYSTYPE *)yyvsp)[aHostValIndex - aNumofTokens].strval),
                                    gUlpCurDepth )
         ) == NULL
       )
    {
        //host 변수 못찾는 error
        // error 처리

        ulpSetErrorCode( &gUlpParseInfo.mErrorMgr,
                         ulpERR_ABORT_COMP_Unknown_Hostvar_Error,
                         (*(((YYSTYPE *)yyvsp)[aHostValIndex - aNumofTokens].strval)==':')?
                         (((YYSTYPE *)yyvsp)[aHostValIndex - aNumofTokens].strval)+1:
                         (((YYSTYPE *)yyvsp)[aHostValIndex - aNumofTokens].strval)
                       );
        gUlpCOMPErrCode = ulpGetErrorSTATE( &gUlpParseInfo.mErrorMgr );
        COMPerror( ulpGetErrorMSG(&gUlpParseInfo.mErrorMgr) );
    }
    else
    {
        /* BUG-28788 : FOR절을 이용하여 struct pointer type의 array insert가 안됨  */
        if ( (gUlpCodeGen.ulpGenGetEmSQLInfo()->mIters[0] != '\0') && 
             (sSymNode->mPointer <= 0) )
        {
            /* BUG-44577 array or pointer type이 아닌데 FOR절이 왔다면 error를 report함. 
             * array or pointer type이 아니지만 struct type일 경우 struct안의 변수를 체크한다. */
            if ( sSymNode->mIsstruct == ID_TRUE )
            {
                /* BUG-44577 struct안에 배열 변수가 있는지 확인 */
                if ( ulpValidateFORStructArray(sSymNode) != IDE_SUCCESS)
                {
                    ulpSetErrorCode( &gUlpParseInfo.mErrorMgr, ulpERR_ABORT_FORstmt_Invalid_usage_Error );
                    gUlpCOMPErrCode = ulpGetErrorSTATE( &gUlpParseInfo.mErrorMgr );
                    COMPerror( ulpGetErrorMSG(&gUlpParseInfo.mErrorMgr) );
                }
            }
            else
            {
                ulpSetErrorCode( &gUlpParseInfo.mErrorMgr,
                                 ulpERR_ABORT_FORstmt_Invalid_usage_Error );
                gUlpCOMPErrCode = ulpGetErrorSTATE( &gUlpParseInfo.mErrorMgr );
                COMPerror( ulpGetErrorMSG(&gUlpParseInfo.mErrorMgr) );
            }
        }
        else
        {
            /* pointer type */
        }

        // 호스트 변수들에 대해 struct,arraystruct type 설정.
        if ( sSymNode->mIsstruct == ID_TRUE )
        {
            if ( sSymNode->mArraySize[0] != '\0' )
            {
                // array struct

                /* BUG-32100 An indicator of arraystructure type should not be used for a hostvariable. */
                if (gUlpIndNode != NULL)
                {
                    // 구조체 배열을 호스트 변수로 사용하면, 인디케이터를 가질 수 없다.
                    ulpSetErrorCode( &gUlpParseInfo.mErrorMgr,
                                     ulpERR_ABORT_COMP_Invalid_Indicator_Usage_Error );
                    gUlpCOMPErrCode = ulpGetErrorSTATE( &gUlpParseInfo.mErrorMgr );
                    COMPerror( ulpGetErrorMSG( &gUlpParseInfo.mErrorMgr ) );
                }
                else
                {
                    sHVType = sArrayStructType;
                    gUlpCodeGen.ulpGenEmSQL( GEN_HVTYPE, (void *) &sHVType );
                }
            }
            else
            {
                if( ulpCOMPCheckArray ( sSymNode ) == ID_TRUE )
                {
                    // array
                    sHVType = GEN_ARRAY;
                    gUlpCodeGen.ulpGenEmSQL( GEN_HVTYPE, (void *) &sHVType );
                }
                else
                {
                    // struct
                    sHVType = GEN_STRUCT;
                    gUlpCodeGen.ulpGenEmSQL( GEN_HVTYPE, (void *) &sHVType );
                }
            }
        }

        // remove some tokens
        for( sIndexs = aRemoveTokIndexs; sIndexs > 0 ; sIndexs/=10 )
        {
            sMod = sIndexs%10;
            if (sMod > 0)
            {
                gUlpCodeGen.ulpGenRemoveQueryToken(
                        (((YYSTYPE *)yyvsp)[sMod - aNumofTokens].strval)
                                                  );
            }
        }

        // set value type for file mode
        switch(aFileType)
        {
            case HV_FILE_CLOB:
                sSymNode->mType = H_CLOB_FILE;
                sFileOptName = gUlpFileOptName;
                break;
            case HV_FILE_BLOB:
                sSymNode->mType = H_BLOB_FILE;
                sFileOptName = gUlpFileOptName;
                break;
            default:
                sFileOptName = NULL;
                break;
        }

        gUlpCodeGen.ulpGenAddHostVarList(
                        (((YYSTYPE *)yyvsp)[aHostValIndex- aNumofTokens].strval),
                        sSymNode ,
                        gUlpIndName,
                        gUlpIndNode,
                        sFileOptName,
                        aInOutType      );

        if ( sSymNode->mIsstruct == ID_TRUE )
        {
            IDE_ASSERT(sSymNode->mStructLink->mChild != NULL);
            gUlpCodeGen.ulpIncHostVarNum( sSymNode->mStructLink->mChild->mCnt );
            if( aTransformQuery == ID_TRUE )
            {
                gUlpCodeGen.ulpGenAddHostVarArr( sSymNode->mStructLink->mChild->mCnt );
            }
        }
        else
        {
            gUlpCodeGen.ulpIncHostVarNum( 1 );
            if( aTransformQuery == ID_TRUE )
            {
                gUlpCodeGen.ulpGenAddHostVarArr( 1 );
            }
        }
    }

    gUlpIndName[0] = '\0';
    gUlpIndNode    = NULL;

    switch(aFileType)
    {
        case HV_FILE_CLOB:
        case HV_FILE_BLOB:
            gUlpFileOptName[0] = '\0';
            break;
        default:
            break;
    }
}

/* =========================================================
 *  ulpValidateFORStructArray
 *
 *  Description :
 *     ulpValidateHostValue에서 호출되는 함수들로, 
       FOR절에 사용되는 host 변수를 체크한다.
 *
 *  Parameters :  
 *     ulpSymTElement *aElement : 체크해야될 host 변수 정보
 * ========================================================*/
IDE_RC ulpValidateFORStructArray(ulpSymTElement *aElement)
{
    ulpStructTNode *sStructTNode;
    ulpSymTNode    *sSymTNode;
    ulpSymTElement *sFirstFieldNode;
    ulpSymTElement *sFieldNode;
    SInt            i;
        
    sStructTNode    = (ulpStructTNode*)aElement->mStructLink;
    
    /* BUG-44577 struct안에 변수가 없음 */
    IDE_TEST( sStructTNode->mChild->mCnt <= 0 );
    
    sSymTNode       = sStructTNode->mChild->mInOrderList;
    sFirstFieldNode = &(sSymTNode->mElement);
                
    IDE_TEST( (sFirstFieldNode->mIsstruct == ID_TRUE) || (sFirstFieldNode->mIsarray == ID_FALSE));
        
    /* BUG-44577 char type일 경우 무조건 2차원 배열이 와야 한다. */
    if ( (sFirstFieldNode->mType == H_CHAR)    ||
         (sFirstFieldNode->mType == H_VARCHAR) ||
         (sFirstFieldNode->mType == H_NCHAR)   ||
         (sFirstFieldNode->mType == H_NVARCHAR) )
    {
        IDE_TEST( (sFirstFieldNode->mArraySize[0] == '\0') || (sFirstFieldNode->mArraySize2[0] == '\0') );
    }
                
    for (i = 1; i < sStructTNode->mChild->mCnt; i++)
    {
        sSymTNode = sSymTNode->mInOrderNext;
        sFieldNode = &(sSymTNode->mElement);
        
        IDE_TEST( (sFieldNode->mIsstruct == ID_TRUE) || (sFieldNode->mIsarray == ID_FALSE));
        
        /* BUG-44577 char type일 경우 무조건 2차원 배열이 와야 한다. */
        if ( (sFirstFieldNode->mType == H_CHAR)    ||
             (sFirstFieldNode->mType == H_VARCHAR) ||
             (sFirstFieldNode->mType == H_NCHAR)   ||
             (sFirstFieldNode->mType == H_NVARCHAR) )
        {
            IDE_TEST( (sFirstFieldNode->mArraySize[0] == '\0') || (sFirstFieldNode->mArraySize2[0] == '\0') );
        }
        
        IDE_TEST( idlOS::strcmp(sFirstFieldNode->mArraySize, sFieldNode->mArraySize) != 0 );
    }

    return IDE_SUCCESS;
        
    IDE_EXCEPTION_END;
    
    return IDE_FAILURE;
}


/*
 *  Member functions of the ulpParseInfo.
 *
 */

ulpParseInfo::ulpParseInfo()
{
    mSaveId              = ID_FALSE;
    mFuncDecl            = ID_FALSE;
    mStructDeclDepth     = 0;
    mArrDepth            = 0;
    mArrExpr             = ID_FALSE;
    mConstantExprStr[0]  = '\0';
    mStructPtr           = NULL;
    mHostValInfo4Typedef = NULL;
    mVarcharDecl         = ID_FALSE;
    /* BUG-27875 : 구조체안의 typedef type인식못함. */
    mSkipTypedef         = ID_FALSE;

    /* BUG-35518 Shared pointer should be supported in APRE */
    mIsSharedPtr         = ID_FALSE;
    IDU_LIST_INIT( &mSharedPtrVarList );

    IDU_LIST_INIT( &mVarcharVarList );

    /* BUG-28118 : system 헤더파일들도 파싱돼야함.                       *
     * 6th. problem : Nested structure 정의중 scope를 잘못 계산하는 문제 */
    mHostValInfo[mStructDeclDepth]
            = (ulpSymTElement *) idlOS::malloc( ID_SIZEOF( ulpSymTElement ) );

    if ( mHostValInfo[mStructDeclDepth] == NULL )
    {
        ulpSetErrorCode( &gUlpParseInfo.mErrorMgr,
                         ulpERR_ABORT_Memory_Alloc_Error );
        ulpPrintfErrorCode( stderr,
                            &gUlpParseInfo.mErrorMgr);
        IDE_ASSERT(0);
    }
    else
    {
        (void) ulpInitHostInfo();
    }
}


/* BUG-28118 : system 헤더파일들도 파싱돼야함.                       *
 * 6th. problem : Nested structure 정의중 scope를 잘못 계산하는 문제 */
void ulpParseInfo::ulpFinalize(void)
{
/***********************************************************************
 *
 * Description :
 *    malloc 된 mHostValInfo 배열이 free되지 않았다면 free해준다.
 *
 * Implementation :
 *
 ***********************************************************************/

    for( ; mStructDeclDepth >= 0 ; mStructDeclDepth-- )
    {
        idlOS::free( mHostValInfo[mStructDeclDepth] );
        mHostValInfo[mStructDeclDepth] = NULL;
    }
}


/* BUG-28118 : system 헤더파일들도 파싱돼야함.                       *
 * 6th. problem : Nested structure 정의중 scope를 잘못 계산하는 문제 */
void ulpParseInfo::ulpInitHostInfo( void )
{
/***********************************************************************
 *
 * Description :
 *    host 변수 정보 초기화 함수로 특정 host 변수를 파싱하면서 setting된
 *    변수에대한 정보를 파싱을 마친후 본함수가 호출되어 다시 초기화 해준다.
 * Implementation :
 *
 ***********************************************************************/
    mHostValInfo[mStructDeclDepth]->mName[0]       = '\0';
    mHostValInfo[mStructDeclDepth]->mType          = H_UNKNOWN;
    mHostValInfo[mStructDeclDepth]->mIsTypedef     = ID_FALSE;
    mHostValInfo[mStructDeclDepth]->mIsarray       = ID_FALSE;
    mHostValInfo[mStructDeclDepth]->mArraySize[0]  = '\0';
    mHostValInfo[mStructDeclDepth]->mArraySize2[0] = '\0';
    mHostValInfo[mStructDeclDepth]->mIsstruct      = ID_FALSE;
    mHostValInfo[mStructDeclDepth]->mStructName[0] = '\0';
    mHostValInfo[mStructDeclDepth]->mStructLink    = NULL;
    mHostValInfo[mStructDeclDepth]->mIssign        = ID_TRUE;
    mHostValInfo[mStructDeclDepth]->mPointer       = 0;
    mHostValInfo[mStructDeclDepth]->mAlloc         = ID_FALSE;
    mHostValInfo[mStructDeclDepth]->mMoreInfo      = 0;
    mHostValInfo[mStructDeclDepth]->mIsExtern      = ID_FALSE;
}


void ulpParseInfo::ulpCopyHostInfo4Typedef( ulpSymTElement *aD,
                                            ulpSymTElement *aS )
{
/***********************************************************************
 *
 * Description :
 *    typedef 구문 처리를 위한 ulpSymTElement copy 함수로, typedef 된 특정 type을
 *    선언시 사용할때 호출되어 해당 type에 대한 정보를 복사해줌.
 *   예)  typedef struct A { int a; };
          A sA;           <----   이경우 type A에 대한 정보를 변수 sA 정보에 복사해줌.
 * Implementation :
 *
 ***********************************************************************/
    // mIsTypedef, mName은 복사 대상이 아님.
    aD->mType     = aS->mType;
    aD->mIsarray  = aS->mIsarray;
    aD->mIsstruct = aS->mIsstruct;
    aD->mIssign   = aS->mIssign;
    aD->mPointer  = aS->mPointer;
    aD->mAlloc    = aS->mAlloc;
    aD->mIsExtern = aS->mIsExtern;
    if ( aS->mArraySize[0] != '\0' )
    {
        idlOS::strncpy( aD->mArraySize, aS->mArraySize, MAX_NUMBER_LEN - 1 );
    }
    if ( aS->mArraySize2[0] != '\0' )
    {
        idlOS::strncpy( aD->mArraySize2, aS->mArraySize2, MAX_NUMBER_LEN - 1 );
    }
    if ( aS->mStructName[0] != '\0' )
    {
        idlOS::strncpy( aD->mStructName, aS->mStructName, MAX_HOSTVAR_NAME_SIZE - 1 );
    }
    if ( aS->mStructLink != NULL )
    {
        aD->mStructLink  = aS->mStructLink;
    }
}


// for debug : print host variable info.
void ulpParseInfo::ulpPrintHostInfo(void)
{
    idlOS::printf( "\n=== hostvar info ===\n"
                   "mName       =[%s]\n"
                   "mType       =[%d]\n"
                   "mIsTypedef  =[%d]\n"
                   "mIsarray    =[%d]\n"
                   "mArraySize  =[%s]\n"
                   "mArraySize2 =[%s]\n"
                   "mIsstruct   =[%d]\n"
                   "mStructName =[%s]\n"
                   "mStructLink =[%d]\n"
                   "mIssign     =[%d]\n"
                   "mPointer    =[%d]\n"
                   "mAlloc      =[%d]\n"
                   "mIsExtern   =[%d]\n"
                   "====================\n",
                   mHostValInfo[mStructDeclDepth]->mName,
                   mHostValInfo[mStructDeclDepth]->mType,
                   mHostValInfo[mStructDeclDepth]->mIsTypedef,
                   mHostValInfo[mStructDeclDepth]->mIsarray,
                   mHostValInfo[mStructDeclDepth]->mArraySize,
                   mHostValInfo[mStructDeclDepth]->mArraySize2,
                   mHostValInfo[mStructDeclDepth]->mIsstruct,
                   mHostValInfo[mStructDeclDepth]->mStructName,
                   mHostValInfo[mStructDeclDepth]->mStructLink,
                   mHostValInfo[mStructDeclDepth]->mIssign,
                   mHostValInfo[mStructDeclDepth]->mPointer,
                   mHostValInfo[mStructDeclDepth]->mAlloc,
                   mHostValInfo[mStructDeclDepth]->mIsExtern );
}

