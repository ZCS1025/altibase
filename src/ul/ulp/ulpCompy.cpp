
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
     C_SQL_NUMERIC_STRUCT = 334,
     C_SQL_DA_STRUCT = 335,
     C_FAILOVERCB = 336,
     C_NCHAR_CS = 337,
     C_ATTRIBUTE = 338,
     M_INCLUDE = 339,
     M_DEFINE = 340,
     M_UNDEF = 341,
     M_FUNCTION = 342,
     M_LBRAC = 343,
     M_RBRAC = 344,
     M_DQUOTE = 345,
     M_FILENAME = 346,
     M_IF = 347,
     M_ELIF = 348,
     M_ELSE = 349,
     M_ENDIF = 350,
     M_IFDEF = 351,
     M_IFNDEF = 352,
     M_CONSTANT = 353,
     M_IDENTIFIER = 354,
     EM_SQLSTART = 355,
     EM_ERROR = 356,
     TR_ADD = 357,
     TR_AFTER = 358,
     TR_AGER = 359,
     TR_ALL = 360,
     TR_ALTER = 361,
     TR_AND = 362,
     TR_ANY = 363,
     TR_ARCHIVE = 364,
     TR_ARCHIVELOG = 365,
     TR_AS = 366,
     TR_ASC = 367,
     TR_AT = 368,
     TR_BACKUP = 369,
     TR_BEFORE = 370,
     TR_BEGIN = 371,
     TR_BY = 372,
     TR_BIND = 373,
     TR_CASCADE = 374,
     TR_CASE = 375,
     TR_CAST = 376,
     TR_CHECK_OPENING_PARENTHESIS = 377,
     TR_CLOSE = 378,
     TR_COALESCE = 379,
     TR_COLUMN = 380,
     TR_COMMENT = 381,
     TR_COMMIT = 382,
     TR_COMPILE = 383,
     TR_CONNECT = 384,
     TR_CONSTRAINT = 385,
     TR_CONSTRAINTS = 386,
     TR_CONTINUE = 387,
     TR_CREATE = 388,
     TR_VOLATILE = 389,
     TR_CURSOR = 390,
     TR_CYCLE = 391,
     TR_DATABASE = 392,
     TR_DECLARE = 393,
     TR_DEFAULT = 394,
     TR_DELETE = 395,
     TR_DEQUEUE = 396,
     TR_DESC = 397,
     TR_DIRECTORY = 398,
     TR_DISABLE = 399,
     TR_DISCONNECT = 400,
     TR_DISTINCT = 401,
     TR_DROP = 402,
     TR_DESCRIBE = 403,
     TR_DESCRIPTOR = 404,
     TR_EACH = 405,
     TR_ELSE = 406,
     TR_ELSEIF = 407,
     TR_ENABLE = 408,
     TR_END = 409,
     TR_ENQUEUE = 410,
     TR_ESCAPE = 411,
     TR_EXCEPTION = 412,
     TR_EXEC = 413,
     TR_EXECUTE = 414,
     TR_EXIT = 415,
     TR_FAILOVERCB = 416,
     TR_FALSE = 417,
     TR_FETCH = 418,
     TR_FIFO = 419,
     TR_FLUSH = 420,
     TR_FOR = 421,
     TR_FOREIGN = 422,
     TR_FROM = 423,
     TR_FULL = 424,
     TR_FUNCTION = 425,
     TR_GOTO = 426,
     TR_GRANT = 427,
     TR_GROUP = 428,
     TR_HAVING = 429,
     TR_IF = 430,
     TR_IN = 431,
     TR_IN_BF_LPAREN = 432,
     TR_INNER = 433,
     TR_INSERT = 434,
     TR_INTERSECT = 435,
     TR_INTO = 436,
     TR_IS = 437,
     TR_ISOLATION = 438,
     TR_JOIN = 439,
     TR_KEY = 440,
     TR_LEFT = 441,
     TR_LESS = 442,
     TR_LEVEL = 443,
     TR_LIFO = 444,
     TR_LIKE = 445,
     TR_LIMIT = 446,
     TR_LOCAL = 447,
     TR_LOGANCHOR = 448,
     TR_LOOP = 449,
     TR_MERGE = 450,
     TR_MOVE = 451,
     TR_MOVEMENT = 452,
     TR_NEW = 453,
     TR_NOARCHIVELOG = 454,
     TR_NOCYCLE = 455,
     TR_NOT = 456,
     TR_NULL = 457,
     TR_OF = 458,
     TR_OFF = 459,
     TR_OLD = 460,
     TR_ON = 461,
     TR_OPEN = 462,
     TR_OR = 463,
     TR_ORDER = 464,
     TR_OUT = 465,
     TR_OUTER = 466,
     TR_OVER = 467,
     TR_PARTITION = 468,
     TR_PARTITIONS = 469,
     TR_POINTER = 470,
     TR_PRIMARY = 471,
     TR_PRIOR = 472,
     TR_PRIVILEGES = 473,
     TR_PROCEDURE = 474,
     TR_PUBLIC = 475,
     TR_QUEUE = 476,
     TR_READ = 477,
     TR_REBUILD = 478,
     TR_RECOVER = 479,
     TR_REFERENCES = 480,
     TR_REFERENCING = 481,
     TR_REGISTER = 482,
     TR_RESTRICT = 483,
     TR_RETURN = 484,
     TR_REVOKE = 485,
     TR_RIGHT = 486,
     TR_ROLLBACK = 487,
     TR_ROW = 488,
     TR_SAVEPOINT = 489,
     TR_SELECT = 490,
     TR_SEQUENCE = 491,
     TR_SESSION = 492,
     TR_SET = 493,
     TR_SOME = 494,
     TR_SPLIT = 495,
     TR_START = 496,
     TR_STATEMENT = 497,
     TR_SYNONYM = 498,
     TR_TABLE = 499,
     TR_TEMPORARY = 500,
     TR_THAN = 501,
     TR_THEN = 502,
     TR_TO = 503,
     TR_TRIGGER = 504,
     TR_TRUE = 505,
     TR_TYPE = 506,
     TR_TYPESET = 507,
     TR_UNION = 508,
     TR_UNIQUE = 509,
     TR_UNREGISTER = 510,
     TR_UNTIL = 511,
     TR_UPDATE = 512,
     TR_USER = 513,
     TR_USING = 514,
     TR_VALUES = 515,
     TR_VARIABLE = 516,
     TR_VARIABLE_LARGE = 517,
     TR_VARIABLES = 518,
     TR_VIEW = 519,
     TR_WHEN = 520,
     TR_WHERE = 521,
     TR_WHILE = 522,
     TR_WITH = 523,
     TR_WORK = 524,
     TR_WRITE = 525,
     TR_PARALLEL = 526,
     TR_NOPARALLEL = 527,
     TR_LOB = 528,
     TR_STORE = 529,
     TR_ENDEXEC = 530,
     TR_PRECEDING = 531,
     TR_FOLLOWING = 532,
     TR_CURRENT_ROW = 533,
     TR_LINK = 534,
     TR_ROLE = 535,
     TR_WITHIN = 536,
     TR_LOGGING = 537,
     TK_BETWEEN = 538,
     TK_EXISTS = 539,
     TO_ACCESS = 540,
     TO_CONSTANT = 541,
     TO_IDENTIFIED = 542,
     TO_INDEX = 543,
     TO_MINUS = 544,
     TO_MODE = 545,
     TO_OTHERS = 546,
     TO_RAISE = 547,
     TO_RENAME = 548,
     TO_REPLACE = 549,
     TO_ROWTYPE = 550,
     TO_SEGMENT = 551,
     TO_WAIT = 552,
     TO_PIVOT = 553,
     TO_UNPIVOT = 554,
     TO_MATERIALIZED = 555,
     TO_CONNECT_BY_NOCYCLE = 556,
     TO_CONNECT_BY_ROOT = 557,
     TO_NULLS = 558,
     TO_PURGE = 559,
     TO_FLASHBACK = 560,
     TO_VC2COLL = 561,
     TO_KEEP = 562,
     TA_ELSIF = 563,
     TA_EXTENTSIZE = 564,
     TA_FIXED = 565,
     TA_LOCK = 566,
     TA_MAXROWS = 567,
     TA_ONLINE = 568,
     TA_OFFLINE = 569,
     TA_REPLICATION = 570,
     TA_REVERSE = 571,
     TA_ROWCOUNT = 572,
     TA_STEP = 573,
     TA_TABLESPACE = 574,
     TA_TRUNCATE = 575,
     TA_SQLCODE = 576,
     TA_SQLERRM = 577,
     TA_LINKER = 578,
     TA_REMOTE_TABLE = 579,
     TA_SHARD = 580,
     TA_NODE_META = 581,
     TA_NODE_DATA = 582,
     TA_DISJOIN = 583,
     TA_CONJOIN = 584,
     TA_SEC = 585,
     TA_MSEC = 586,
     TA_USEC = 587,
     TA_SECOND = 588,
     TA_MILLISECOND = 589,
     TA_MICROSECOND = 590,
     TA_ANALYSIS_PROPAGATION = 591,
     TI_NONQUOTED_IDENTIFIER = 592,
     TI_QUOTED_IDENTIFIER = 593,
     TI_HOSTVARIABLE = 594,
     TL_TYPED_LITERAL = 595,
     TL_LITERAL = 596,
     TL_NCHAR_LITERAL = 597,
     TL_UNICODE_LITERAL = 598,
     TL_INTEGER = 599,
     TL_NUMERIC = 600,
     TS_AT_SIGN = 601,
     TS_CONCATENATION_SIGN = 602,
     TS_DOUBLE_PERIOD = 603,
     TS_EXCLAMATION_POINT = 604,
     TS_PERCENT_SIGN = 605,
     TS_OPENING_PARENTHESIS = 606,
     TS_CLOSING_PARENTHESIS = 607,
     TS_OPENING_BRACKET = 608,
     TS_CLOSING_BRACKET = 609,
     TS_ASTERISK = 610,
     TS_PLUS_SIGN = 611,
     TS_COMMA = 612,
     TS_MINUS_SIGN = 613,
     TS_PERIOD = 614,
     TS_SLASH = 615,
     TS_COLON = 616,
     TS_SEMICOLON = 617,
     TS_LESS_THAN_SIGN = 618,
     TS_EQUAL_SIGN = 619,
     TS_GREATER_THAN_SIGN = 620,
     TS_QUESTION_MARK = 621,
     TS_OUTER_JOIN_OPERATOR = 622,
     TX_HINTS = 623,
     SES_V_NUMERIC = 624,
     SES_V_INTEGER = 625,
     SES_V_HOSTVARIABLE = 626,
     SES_V_LITERAL = 627,
     SES_V_TYPED_LITERAL = 628,
     SES_V_DQUOTE_LITERAL = 629,
     SES_V_IDENTIFIER = 630,
     SES_V_ABSOLUTE = 631,
     SES_V_ALLOCATE = 632,
     SES_V_ASENSITIVE = 633,
     SES_V_AUTOCOMMIT = 634,
     SES_V_BATCH = 635,
     SES_V_BLOB_FILE = 636,
     SES_V_BREAK = 637,
     SES_V_CLOB_FILE = 638,
     SES_V_CUBE = 639,
     SES_V_DEALLOCATE = 640,
     SES_V_DESCRIPTOR = 641,
     SES_V_DO = 642,
     SES_V_FIRST = 643,
     SES_V_FOUND = 644,
     SES_V_FREE = 645,
     SES_V_HOLD = 646,
     SES_V_IMMEDIATE = 647,
     SES_V_INDICATOR = 648,
     SES_V_INSENSITIVE = 649,
     SES_V_LAST = 650,
     SES_V_NEXT = 651,
     SES_V_ONERR = 652,
     SES_V_ONLY = 653,
     APRE_V_OPTION = 654,
     SES_V_PREPARE = 655,
     SES_V_RELATIVE = 656,
     SES_V_RELEASE = 657,
     SES_V_ROLLUP = 658,
     SES_V_SCROLL = 659,
     SES_V_SENSITIVE = 660,
     SES_V_SQLERROR = 661,
     SES_V_THREADS = 662,
     SES_V_WHENEVER = 663,
     SES_V_CURRENT = 664,
     SES_V_GROUPING_SETS = 665,
     SES_V_WITH_ROLLUP = 666,
     SES_V_GET = 667,
     SES_V_DIAGNOSTICS = 668,
     SES_V_CONDITION = 669,
     SES_V_NUMBER = 670,
     SES_V_ROW_COUNT = 671,
     SES_V_RETURNED_SQLCODE = 672,
     SES_V_RETURNED_SQLSTATE = 673,
     SES_V_MESSAGE_TEXT = 674,
     SES_V_ROW_NUMBER = 675,
     SES_V_COLUMN_NUMBER = 676
   };
#endif



#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
typedef union YYSTYPE
{

/* Line 214 of yacc.c  */
#line 24 "ulpCompy.y"

    int   intval;
    char *strval;



/* Line 214 of yacc.c  */
#line 551 "ulpCompy.cpp"
} YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
#endif


/* Copy the second part of user declarations.  */

/* Line 264 of yacc.c  */
#line 54 "ulpCompy.y"


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

/* BUG-46824 anonymous block 에서 object_name이 해석된 횟수 */
UInt                 gUlpProcObjCount = 0;
/* BUG-446824 anonymous block에서 첫번째 object_name을 저장하는 변수 */
SChar               *gUlpPSMObjName;

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
#line 652 "ulpCompy.cpp"

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
#define YYFINAL  109
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   18267

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  446
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  744
/* YYNRULES -- Number of rules.  */
#define YYNRULES  2083
/* YYNRULES -- Number of states.  */
#define YYNSTATES  3990

/* YYTRANSLATE(YYLEX) -- Bison symbol number corresponding to YYLEX.  */
#define YYUNDEFTOK  2
#define YYMAXUTOK   676

#define YYTRANSLATE(YYX)						\
  ((unsigned int) (YYX) <= YYMAXUTOK ? yytranslate[YYX] : YYUNDEFTOK)

/* YYTRANSLATE[YYLEX] -- Bison symbol number corresponding to YYLEX.  */
static const yytype_uint16 yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,   433,     2,     2,     2,   435,   428,     2,
     422,   423,   429,   430,   427,   431,   426,   434,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,   441,   443,
     436,   442,   437,   440,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,   424,     2,   425,   438,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,   445,   439,   444,   432,     2,     2,     2,
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
     405,   406,   407,   408,   409,   410,   411,   412,   413,   414,
     415,   416,   417,   418,   419,   420,   421
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
     343,   345,   347,   349,   351,   353,   355,   356,   358,   363,
     369,   374,   380,   383,   386,   388,   390,   392,   394,   397,
     399,   401,   405,   406,   408,   412,   414,   416,   419,   423,
     428,   434,   437,   439,   443,   445,   449,   451,   454,   456,
     460,   464,   469,   473,   478,   483,   485,   487,   489,   492,
     495,   499,   501,   504,   506,   510,   512,   516,   518,   522,
     524,   528,   531,   533,   535,   538,   540,   542,   545,   549,
     552,   556,   560,   565,   568,   572,   576,   581,   583,   587,
     592,   594,   598,   600,   602,   604,   606,   608,   610,   614,
     618,   623,   627,   632,   636,   639,   643,   645,   647,   649,
     651,   654,   656,   658,   661,   663,   666,   672,   680,   686,
     692,   700,   707,   715,   723,   732,   740,   749,   758,   768,
     772,   775,   778,   781,   785,   788,   792,   794,   797,   799,
     801,   804,   806,   808,   810,   812,   814,   816,   818,   820,
     822,   827,   832,   835,   838,   841,   843,   845,   848,   851,
     853,   855,   860,   865,   870,   876,   881,   885,   889,   893,
     896,   900,   901,   904,   907,   909,   911,   913,   915,   917,
     919,   921,   923,   925,   929,   933,   941,   942,   944,   946,
     948,   949,   951,   952,   955,   958,   963,   964,   968,   972,
     976,   981,   982,   985,   987,   991,   995,   998,  1003,  1008,
    1014,  1021,  1028,  1029,  1031,  1033,  1036,  1038,  1040,  1042,
    1044,  1046,  1049,  1052,  1054,  1057,  1060,  1064,  1067,  1070,
    1073,  1079,  1087,  1095,  1105,  1117,  1127,  1129,  1134,  1138,
    1140,  1142,  1144,  1146,  1148,  1150,  1152,  1154,  1156,  1161,
    1166,  1167,  1171,  1175,  1179,  1182,  1185,  1188,  1192,  1196,
    1199,  1202,  1205,  1208,  1211,  1214,  1220,  1225,  1230,  1238,
    1242,  1250,  1253,  1256,  1259,  1266,  1270,  1272,  1275,  1278,
    1285,  1292,  1299,  1303,  1306,  1313,  1320,  1324,  1329,  1334,
    1339,  1345,  1349,  1354,  1360,  1366,  1372,  1379,  1384,  1385,
    1387,  1394,  1399,  1404,  1411,  1412,  1414,  1418,  1420,  1424,
    1426,  1428,  1430,  1432,  1436,  1438,  1443,  1445,  1447,  1449,
    1451,  1453,  1455,  1457,  1459,  1461,  1463,  1466,  1468,  1471,
    1473,  1475,  1477,  1479,  1481,  1483,  1485,  1487,  1489,  1491,
    1493,  1495,  1497,  1499,  1501,  1503,  1505,  1507,  1509,  1511,
    1513,  1515,  1517,  1519,  1521,  1523,  1525,  1527,  1529,  1531,
    1533,  1535,  1537,  1539,  1541,  1543,  1545,  1547,  1549,  1551,
    1553,  1555,  1557,  1559,  1561,  1563,  1565,  1567,  1569,  1571,
    1573,  1575,  1577,  1579,  1581,  1583,  1585,  1587,  1589,  1591,
    1593,  1595,  1597,  1599,  1601,  1603,  1605,  1607,  1609,  1611,
    1614,  1616,  1620,  1623,  1626,  1628,  1632,  1635,  1638,  1640,
    1644,  1647,  1649,  1651,  1655,  1660,  1664,  1666,  1668,  1670,
    1673,  1678,  1681,  1684,  1688,  1692,  1694,  1697,  1699,  1702,
    1704,  1706,  1708,  1710,  1712,  1714,  1716,  1718,  1720,  1722,
    1724,  1726,  1728,  1730,  1732,  1734,  1736,  1738,  1740,  1742,
    1744,  1746,  1748,  1750,  1752,  1754,  1756,  1758,  1760,  1762,
    1764,  1766,  1768,  1770,  1772,  1774,  1776,  1778,  1780,  1782,
    1784,  1786,  1788,  1790,  1792,  1794,  1796,  1798,  1800,  1802,
    1804,  1806,  1808,  1810,  1812,  1814,  1816,  1818,  1820,  1822,
    1824,  1826,  1828,  1835,  1842,  1849,  1856,  1864,  1872,  1880,
    1887,  1895,  1902,  1906,  1911,  1917,  1924,  1932,  1939,  1946,
    1952,  1960,  1961,  1963,  1965,  1967,  1970,  1973,  1976,  1982,
    1983,  1985,  1989,  1995,  2001,  2007,  2010,  2013,  2018,  2023,
    2027,  2029,  2033,  2037,  2043,  2050,  2058,  2063,  2066,  2068,
    2070,  2072,  2074,  2076,  2078,  2080,  2082,  2084,  2086,  2089,
    2092,  2094,  2097,  2101,  2105,  2109,  2113,  2115,  2117,  2122,
    2126,  2130,  2132,  2134,  2139,  2147,  2156,  2160,  2162,  2165,
    2169,  2173,  2177,  2180,  2184,  2188,  2192,  2196,  2199,  2203,
    2207,  2211,  2214,  2218,  2222,  2226,  2229,  2233,  2237,  2241,
    2245,  2248,  2251,  2254,  2258,  2262,  2266,  2270,  2274,  2278,
    2282,  2286,  2289,  2292,  2295,  2298,  2302,  2306,  2310,  2313,
    2316,  2319,  2323,  2327,  2331,  2336,  2341,  2346,  2349,  2353,
    2357,  2361,  2365,  2369,  2371,  2373,  2375,  2377,  2379,  2381,
    2383,  2385,  2387,  2390,  2392,  2394,  2396,  2400,  2402,  2404,
    2406,  2407,  2411,  2413,  2415,  2420,  2429,  2439,  2440,  2443,
    2444,  2446,  2452,  2459,  2463,  2468,  2479,  2486,  2493,  2500,
    2507,  2514,  2521,  2528,  2535,  2544,  2551,  2555,  2561,  2568,
    2574,  2581,  2587,  2588,  2591,  2594,  2596,  2598,  2601,  2604,
    2607,  2611,  2613,  2615,  2616,  2618,  2620,  2622,  2625,  2627,
    2632,  2633,  2636,  2640,  2641,  2644,  2647,  2651,  2652,  2655,
    2659,  2661,  2670,  2683,  2685,  2686,  2689,  2691,  2695,  2699,
    2701,  2703,  2706,  2707,  2710,  2714,  2716,  2720,  2726,  2727,
    2729,  2735,  2739,  2744,  2752,  2753,  2757,  2761,  2765,  2770,
    2774,  2781,  2789,  2795,  2803,  2813,  2817,  2819,  2831,  2841,
    2849,  2850,  2852,  2853,  2855,  2858,  2863,  2868,  2871,  2873,
    2877,  2882,  2886,  2891,  2895,  2900,  2903,  2907,  2909,  2911,
    2913,  2916,  2918,  2919,  2921,  2923,  2934,  2946,  2957,  2968,
    2978,  2994,  3007,  3014,  3021,  3027,  3033,  3040,  3049,  3055,
    3060,  3069,  3079,  3090,  3099,  3100,  3102,  3106,  3107,  3110,
    3111,  3116,  3117,  3120,  3124,  3127,  3131,  3140,  3146,  3151,
    3167,  3183,  3187,  3191,  3195,  3203,  3204,  3209,  3213,  3215,
    3219,  3220,  3225,  3229,  3231,  3235,  3236,  3241,  3245,  3247,
    3251,  3257,  3261,  3271,  3275,  3277,  3279,  3285,  3292,  3300,
    3310,  3311,  3313,  3314,  3316,  3321,  3327,  3329,  3331,  3335,
    3339,  3341,  3344,  3346,  3348,  3351,  3354,  3360,  3361,  3363,
    3364,  3366,  3370,  3382,  3385,  3389,  3394,  3395,  3399,  3400,
    3405,  3410,  3411,  3413,  3415,  3420,  3424,  3426,  3432,  3433,
    3436,  3437,  3441,  3442,  3444,  3447,  3449,  3452,  3455,  3457,
    3466,  3477,  3485,  3494,  3495,  3497,  3499,  3502,  3505,  3509,
    3511,  3512,  3516,  3520,  3530,  3534,  3536,  3546,  3550,  3558,
    3564,  3568,  3570,  3572,  3575,  3577,  3580,  3585,  3586,  3588,
    3591,  3593,  3603,  3610,  3613,  3615,  3618,  3620,  3624,  3626,
    3628,  3630,  3636,  3643,  3651,  3658,  3665,  3670,  3671,  3674,
    3676,  3678,  3682,  3684,  3691,  3692,  3694,  3696,  3698,  3699,
    3703,  3704,  3706,  3709,  3711,  3714,  3718,  3723,  3729,  3736,
    3739,  3746,  3747,  3750,  3751,  3754,  3756,  3761,  3768,  3776,
    3784,  3785,  3787,  3791,  3792,  3796,  3800,  3805,  3808,  3813,
    3816,  3820,  3822,  3827,  3828,  3832,  3836,  3840,  3844,  3847,
    3849,  3852,  3853,  3855,  3866,  3875,  3884,  3895,  3896,  3899,
    3901,  3902,  3906,  3910,  3912,  3914,  3915,  3919,  3924,  3928,
    3935,  3943,  3951,  3960,  3965,  3971,  3973,  3975,  3982,  3987,
    3992,  3998,  4005,  4012,  4015,  4018,  4023,  4029,  4033,  4040,
    4047,  4050,  4054,  4058,  4060,  4067,  4075,  4082,  4090,  4098,
    4107,  4114,  4122,  4132,  4143,  4149,  4152,  4154,  4162,  4166,
    4168,  4170,  4172,  4176,  4178,  4182,  4186,  4188,  4191,  4193,
    4200,  4208,  4218,  4220,  4222,  4229,  4232,  4234,  4238,  4240,
    4241,  4243,  4245,  4249,  4251,  4255,  4259,  4265,  4267,  4271,
    4275,  4277,  4279,  4283,  4295,  4298,  4300,  4305,  4307,  4310,
    4313,  4320,  4328,  4329,  4333,  4337,  4339,  4340,  4344,  4356,
    4357,  4361,  4365,  4367,  4369,  4371,  4373,  4375,  4379,  4384,
    4386,  4388,  4392,  4397,  4399,  4402,  4404,  4406,  4410,  4412,
    4416,  4418,  4419,  4422,  4425,  4429,  4431,  4438,  4442,  4444,
    4448,  4450,  4461,  4472,  4483,  4485,  4487,  4491,  4496,  4500,
    4502,  4505,  4509,  4511,  4518,  4522,  4524,  4535,  4536,  4538,
    4539,  4543,  4544,  4546,  4548,  4550,  4552,  4553,  4556,  4557,
    4560,  4563,  4567,  4569,  4573,  4579,  4582,  4583,  4585,  4588,
    4590,  4593,  4596,  4600,  4602,  4609,  4614,  4620,  4628,  4634,
    4639,  4641,  4647,  4649,  4651,  4653,  4656,  4658,  4662,  4667,
    4671,  4673,  4675,  4677,  4682,  4684,  4690,  4694,  4695,  4697,
    4699,  4706,  4710,  4712,  4718,  4724,  4729,  4732,  4737,  4741,
    4743,  4746,  4755,  4756,  4759,  4763,  4765,  4769,  4771,  4773,
    4778,  4782,  4784,  4788,  4790,  4794,  4796,  4800,  4802,  4804,
    4808,  4810,  4814,  4816,  4818,  4820,  4824,  4826,  4830,  4834,
    4836,  4840,  4844,  4846,  4849,  4852,  4854,  4856,  4858,  4860,
    4862,  4864,  4866,  4868,  4874,  4878,  4880,  4883,  4888,  4895,
    4896,  4898,  4901,  4904,  4907,  4908,  4910,  4915,  4920,  4924,
    4926,  4928,  4933,  4937,  4939,  4943,  4945,  4948,  4950,  4952,
    4956,  4958,  4960,  4962,  4965,  4966,  4968,  4969,  4972,  4973,
    4977,  4981,  4985,  4986,  4988,  4992,  4995,  4996,  4999,  5000,
    5004,  5009,  5010,  5012,  5015,  5019,  5023,  5027,  5029,  5030,
    5032,  5034,  5037,  5038,  5042,  5043,  5045,  5049,  5050,  5052,
    5054,  5056,  5058,  5060,  5062,  5066,  5068,  5072,  5073,  5075,
    5077,  5078,  5081,  5084,  5094,  5106,  5109,  5112,  5116,  5118,
    5119,  5123,  5124,  5127,  5129,  5133,  5135,  5139,  5141,  5144,
    5146,  5152,  5159,  5163,  5168,  5174,  5181,  5185,  5189,  5193,
    5197,  5201,  5205,  5209,  5213,  5217,  5221,  5225,  5229,  5233,
    5237,  5241,  5245,  5249,  5253,  5257,  5262,  5265,  5268,  5272,
    5276,  5278,  5280,  5283,  5286,  5288,  5291,  5293,  5296,  5299,
    5303,  5307,  5310,  5313,  5317,  5320,  5324,  5327,  5330,  5332,
    5336,  5340,  5344,  5348,  5351,  5354,  5358,  5362,  5365,  5368,
    5372,  5376,  5378,  5382,  5386,  5388,  5394,  5398,  5400,  5402,
    5404,  5406,  5410,  5412,  5416,  5420,  5422,  5426,  5430,  5432,
    5435,  5438,  5441,  5443,  5445,  5447,  5449,  5451,  5453,  5457,
    5459,  5461,  5463,  5465,  5467,  5469,  5478,  5485,  5490,  5493,
    5497,  5499,  5501,  5505,  5507,  5509,  5511,  5517,  5521,  5523,
    5524,  5526,  5532,  5537,  5540,  5542,  5546,  5549,  5550,  5553,
    5556,  5558,  5562,  5570,  5577,  5582,  5588,  5595,  5603,  5611,
    5618,  5627,  5635,  5640,  5644,  5651,  5654,  5655,  5663,  5667,
    5669,  5671,  5674,  5677,  5678,  5685,  5686,  5690,  5694,  5696,
    5698,  5699,  5703,  5704,  5710,  5713,  5716,  5718,  5721,  5724,
    5727,  5729,  5732,  5735,  5737,  5741,  5745,  5747,  5753,  5757,
    5759,  5761,  5762,  5771,  5773,  5775,  5784,  5791,  5797,  5800,
    5805,  5808,  5813,  5816,  5821,  5823,  5825,  5826,  5829,  5833,
    5837,  5839,  5844,  5845,  5847,  5849,  5852,  5853,  5855,  5856,
    5860,  5863,  5865,  5867,  5869,  5871,  5875,  5877,  5879,  5883,
    5888,  5892,  5894,  5899,  5906,  5913,  5922,  5924,  5928,  5934,
    5936,  5938,  5942,  5948,  5952,  5958,  5966,  5968,  5970,  5974,
    5980,  5985,  5992,  6000,  6008,  6017,  6024,  6030,  6033,  6034,
    6036,  6039,  6041,  6043,  6045,  6047,  6049,  6052,  6054,  6062,
    6066,  6072,  6073,  6075,  6084,  6093,  6095,  6097,  6101,  6107,
    6111,  6115,  6117,  6120,  6121,  6123,  6126,  6127,  6129,  6132,
    6134,  6138,  6142,  6146,  6148,  6150,  6154,  6157,  6159,  6161,
    6163,  6165,  6167,  6169,  6171,  6173,  6175,  6177,  6179,  6181,
    6183,  6185,  6187,  6189,  6191,  6197,  6201,  6206,  6209,  6213,
    6216,  6220,  6223,  6227,  6230,  6233,  6236,  6239,  6242,  6245,
    6248,  6251,  6253,  6259,  6265,  6271,  6279,  6287,  6288,  6291,
    6293,  6298,  6303,  6306,  6313,  6321,  6324,  6326,  6330,  6333,
    6335,  6339,  6341,  6342,  6345,  6347,  6349,  6351,  6353,  6358,
    6364,  6368,  6379,  6391,  6392,  6395,  6403,  6407,  6413,  6418,
    6419,  6422,  6426,  6429,  6433,  6439,  6443,  6446,  6449,  6453,
    6456,  6461,  6466,  6470,  6474,  6478,  6481,  6484,  6486,  6494,
    6497,  6503,  6509,  6511,  6513,  6515,  6519,  6525,  6529,  6534,
    6539,  6544,  6548,  6555,  6557,  6559,  6562,  6565,  6569,  6574,
    6579,  6584,  6588,  6595,  6603,  6610,  6617,  6625,  6632,  6637,
    6642,  6643,  6646,  6650,  6651,  6655,  6656,  6659,  6661,  6664,
    6667,  6670,  6674,  6678,  6682,  6686,  6693,  6701,  6710,  6718,
    6723,  6732,  6740,  6745,  6748,  6751,  6758,  6765,  6773,  6782,
    6787,  6791,  6793,  6796,  6801,  6807,  6811,  6818,  6819,  6821,
    6824,  6827,  6832,  6837,  6843,  6850,  6855,  6858,  6861,  6862,
    6864,  6867,  6869,  6871,  6872,  6874,  6877,  6881,  6883,  6886,
    6888,  6891,  6893,  6895,  6897,  6899,  6903,  6905,  6907,  6908,
    6912,  6918,  6927,  6930,  6935,  6940,  6944,  6947,  6951,  6955,
    6960,  6962,  6964,  6967,  6969,  6971,  6972,  6975,  6976,  6979,
    6983,  6985,  6990,  6992,  6994,  6995,  6997,  6999,  7000,  7002,
    7004,  7005,  7010,  7014,  7015,  7020,  7023,  7025,  7027,  7029,
    7034,  7037,  7042,  7046,  7057,  7058,  7061,  7064,  7068,  7073,
    7078,  7084,  7087,  7090,  7096,  7099,  7102,  7106,  7111,  7123,
    7124,  7127,  7128,  7130,  7134,  7135,  7137,  7139,  7141,  7142,
    7144,  7147,  7154,  7161,  7168,  7174,  7180,  7186,  7190,  7192,
    7194,  7196,  7198,  7200,  7202,  7204,  7206,  7208,  7210,  7212,
    7214,  7216,  7219,  7224,  7229,  7234,  7241,  7248,  7250,  7254,
    7258,  7262,  7267,  7270,  7275,  7280,  7284,  7286,  7289,  7290,
    7293,  7296,  7301,  7306,  7311,  7318,  7325,  7326,  7329,  7331,
    7333,  7336,  7341,  7346,  7351,  7358,  7365,  7367,  7371,  7374,
    7377,  7378,  7380,  7381,  7383,  7386,  7387,  7389,  7390,  7393,
    7397,  7398,  7400,  7403,  7406,  7409,  7410,  7414,  7418,  7420,
    7424,  7425,  7430,  7432
};

/* YYRHS -- A `-1'-separated list of the rules' RHS.  */
static const yytype_int16 yyrhs[] =
{
     447,     0,    -1,   448,    -1,   447,   448,    -1,   449,    -1,
     533,    -1,   523,    -1,     3,    -1,   519,    -1,   471,    -1,
     521,    -1,    38,    -1,   522,    -1,   422,   469,   423,    -1,
     450,    -1,   451,   424,   469,   425,    -1,   451,   422,   423,
      -1,   451,   422,   452,   423,    -1,   451,   426,   521,    -1,
     451,    54,   521,    -1,   451,    52,    -1,   451,    53,    -1,
     467,    -1,   452,   427,   467,    -1,   451,    -1,    52,   453,
      -1,    53,   453,    -1,   454,   455,    -1,    25,   453,    -1,
      25,   422,   503,   423,    -1,   428,    -1,   429,    -1,   430,
      -1,   431,    -1,   432,    -1,   433,    -1,   453,    -1,   422,
     503,   423,   455,    -1,   455,    -1,   456,   429,   455,    -1,
     456,   434,   455,    -1,   456,   435,   455,    -1,   456,    -1,
     457,   430,   456,    -1,   457,   431,   456,    -1,   457,    -1,
     458,    59,   457,    -1,   458,    58,   457,    -1,   458,    -1,
     459,   436,   458,    -1,   459,   437,   458,    -1,   459,    61,
     458,    -1,   459,    62,   458,    -1,   459,    -1,   460,    56,
     459,    -1,   460,    57,   459,    -1,   460,    -1,   461,   428,
     460,    -1,   461,    -1,   462,   438,   461,    -1,   462,    -1,
     463,   439,   462,    -1,   463,    -1,   464,    55,   463,    -1,
     464,    -1,   465,    60,   464,    -1,   465,    -1,   465,   440,
     465,   441,   466,    -1,   466,    -1,   453,   468,   467,    -1,
     442,    -1,    46,    -1,    47,    -1,    48,    -1,    44,    -1,
      45,    -1,    43,    -1,    42,    -1,    49,    -1,    50,    -1,
      51,    -1,   467,    -1,   469,   427,   467,    -1,   466,    -1,
     472,   443,    -1,   472,   473,   443,    -1,   476,    -1,   476,
     472,    -1,   477,    -1,   477,   472,    -1,   475,    -1,   473,
     474,   475,    -1,   427,    -1,   492,    -1,   492,   442,   506,
      -1,    29,    -1,    15,    -1,    26,    -1,     4,    -1,    21,
      -1,     7,    -1,     8,    -1,     7,    82,    -1,     8,    82,
      -1,    23,    -1,    19,    -1,    20,    -1,    24,    -1,    31,
      -1,    16,    -1,    13,    -1,     9,    -1,    33,    -1,    32,
      -1,   479,    -1,   489,    -1,    40,    -1,    63,    -1,    64,
      -1,    65,    -1,    66,    -1,    67,    -1,    68,    -1,    69,
      -1,    70,    -1,    71,    -1,    72,    -1,    73,    -1,    74,
      -1,    75,    -1,    76,    -1,    77,    -1,    78,    -1,    79,
      -1,    80,    -1,    81,    -1,    -1,    83,    -1,   482,   480,
     444,   478,    -1,   482,   480,   483,   444,   478,    -1,   482,
     481,   444,   478,    -1,   482,   481,   483,   444,   478,    -1,
     482,   521,    -1,   521,   445,    -1,   445,    -1,    27,    -1,
      30,    -1,   484,    -1,   483,   484,    -1,   485,    -1,   523,
      -1,   497,   486,   443,    -1,    -1,   488,    -1,   486,   487,
     488,    -1,   427,    -1,   492,    -1,   441,   470,    -1,   492,
     441,   470,    -1,    14,   445,   490,   444,    -1,    14,   521,
     445,   490,   444,    -1,    14,   521,    -1,   491,    -1,   490,
     427,   491,    -1,   521,    -1,   521,   442,   470,    -1,   493,
      -1,   496,   493,    -1,   521,    -1,   422,   492,   423,    -1,
     493,   424,   425,    -1,   493,   494,   470,   425,    -1,   493,
     422,   423,    -1,   493,   495,   500,   423,    -1,   493,   422,
     498,   423,    -1,   424,    -1,   422,    -1,   429,    -1,   429,
     497,    -1,   429,   496,    -1,   429,   497,   496,    -1,   477,
      -1,   497,   477,    -1,   499,    -1,   499,   427,    35,    -1,
     521,    -1,   499,   427,   521,    -1,   501,    -1,   501,   427,
      35,    -1,   502,    -1,   501,   427,   502,    -1,   497,   492,
      -1,   503,    -1,   497,    -1,   497,   504,    -1,   496,    -1,
     505,    -1,   496,   505,    -1,   422,   504,   423,    -1,   424,
     425,    -1,   424,   470,   425,    -1,   505,   424,   425,    -1,
     505,   424,   470,   425,    -1,   422,   423,    -1,   422,   500,
     423,    -1,   505,   422,   423,    -1,   505,   422,   500,   423,
      -1,   467,    -1,   445,   507,   444,    -1,   445,   507,   427,
     444,    -1,   506,    -1,   507,   427,   506,    -1,   509,    -1,
     510,    -1,   515,    -1,   516,    -1,   517,    -1,   518,    -1,
     521,   441,   508,    -1,   521,   441,   533,    -1,     6,   470,
     441,   508,    -1,    11,   441,   508,    -1,     6,   470,   441,
     533,    -1,    11,   441,   533,    -1,   513,   444,    -1,   513,
     512,   444,    -1,   471,    -1,   508,    -1,   533,    -1,   511,
      -1,   512,   511,    -1,   445,    -1,   471,    -1,   514,   471,
      -1,   443,    -1,   469,   443,    -1,    37,   422,   469,   423,
     508,    -1,    37,   422,   469,   423,   508,    36,   508,    -1,
      28,   422,   469,   423,   508,    -1,    34,   422,   469,   423,
     508,    -1,    12,   508,    34,   422,   469,   423,   443,    -1,
      17,   422,   443,   443,   423,   508,    -1,    17,   422,   443,
     443,   469,   423,   508,    -1,    17,   422,   443,   469,   443,
     423,   508,    -1,    17,   422,   443,   469,   443,   469,   423,
     508,    -1,    17,   422,   469,   443,   443,   423,   508,    -1,
      17,   422,   469,   443,   443,   469,   423,   508,    -1,    17,
     422,   469,   443,   469,   443,   423,   508,    -1,    17,   422,
     469,   443,   469,   443,   469,   423,   508,    -1,    18,   521,
     443,    -1,    10,   443,    -1,     5,   443,    -1,    22,   443,
      -1,    22,   469,   443,    -1,   492,   520,    -1,   472,   492,
     520,    -1,   510,    -1,   514,   510,    -1,    39,    -1,    41,
      -1,    41,   522,    -1,   524,    -1,   525,    -1,   526,    -1,
     529,    -1,   530,    -1,   527,    -1,   528,    -1,   531,    -1,
     532,    -1,    84,    88,    91,    89,    -1,    84,    90,    91,
      90,    -1,    85,    99,    -1,    85,    87,    -1,    86,    99,
      -1,    92,    -1,    93,    -1,    96,    99,    -1,    97,    99,
      -1,    94,    -1,    95,    -1,   100,   534,   584,   362,    -1,
     100,   534,   535,   362,    -1,   100,   534,   555,   362,    -1,
     100,   534,   569,   275,   362,    -1,   100,   534,   570,   362,
      -1,   100,   571,   362,    -1,   100,   572,   362,    -1,   100,
     573,   362,    -1,   100,   574,    -1,   100,   575,   362,    -1,
      -1,   113,   375,    -1,   113,   371,    -1,   536,    -1,   544,
      -1,   545,    -1,   546,    -1,   550,    -1,   551,    -1,   552,
      -1,   553,    -1,   554,    -1,   537,   842,   541,    -1,   537,
     375,   541,    -1,   138,   375,   538,   539,   135,   540,   166,
      -1,    -1,   405,    -1,   394,    -1,   378,    -1,    -1,   404,
      -1,    -1,   268,   391,    -1,   268,   229,    -1,   268,   391,
     268,   229,    -1,    -1,   166,   222,   398,    -1,   166,   257,
     542,    -1,   166,   257,   375,    -1,   166,   257,   297,   370,
      -1,    -1,   203,   543,    -1,  1162,    -1,   543,   357,  1162,
      -1,   138,   375,   242,    -1,   207,   375,    -1,   207,   375,
     259,  1165,    -1,   207,   375,   259,   562,    -1,   163,   547,
     375,   181,  1170,    -1,   589,   163,   547,   375,   181,  1170,
      -1,   163,   547,   375,   259,   149,  1166,    -1,    -1,   168,
      -1,   548,    -1,   548,   168,    -1,   388,    -1,   217,    -1,
     396,    -1,   395,    -1,   409,    -1,   401,   549,    -1,   376,
     549,    -1,   370,    -1,   356,   370,    -1,   358,   370,    -1,
     123,   402,   375,    -1,   123,   375,    -1,   379,   206,    -1,
     379,   204,    -1,   129,   371,   287,   117,   371,    -1,   129,
     371,   287,   117,   371,   207,   371,    -1,   129,   371,   287,
     117,   371,   259,   371,    -1,   129,   371,   287,   117,   371,
     259,   371,   357,   371,    -1,   129,   371,   287,   117,   371,
     259,   371,   357,   371,   207,   371,    -1,   129,   371,   287,
     117,   371,   259,   371,   207,   371,    -1,   145,    -1,   589,
     390,   273,  1167,    -1,   390,   273,  1167,    -1,   556,    -1,
     558,    -1,   559,    -1,   561,    -1,   563,    -1,   565,    -1,
     566,    -1,   567,    -1,   568,    -1,   377,   386,   371,   557,
      -1,   377,   386,   375,   557,    -1,    -1,   268,   375,   370,
      -1,   385,   386,   371,    -1,   385,   386,   375,    -1,   560,
     371,    -1,   560,   585,    -1,   560,   586,    -1,   400,   375,
     168,    -1,   385,   400,   375,    -1,   149,  1166,    -1,   564,
     371,    -1,   564,   585,    -1,   564,   586,    -1,   159,   392,
      -1,   159,   375,    -1,   589,   159,   375,   259,  1164,    -1,
     159,   375,   259,  1164,    -1,   159,   375,   259,   562,    -1,
     148,   118,   263,   166,   375,   181,  1166,    -1,   166,   375,
    1166,    -1,   148,   235,   375,   166,   375,   181,  1166,    -1,
     995,   362,    -1,   994,   362,    -1,   996,   362,    -1,   159,
     116,  1080,   362,   154,   362,    -1,   159,  1081,   362,    -1,
     390,    -1,   380,   206,    -1,   380,   204,    -1,   106,   237,
     238,   379,   364,   250,    -1,   106,   237,   238,   379,   364,
     162,    -1,   106,   237,   238,   375,   364,   372,    -1,   227,
     161,   371,    -1,   255,   161,    -1,   399,   351,   407,   364,
     250,   352,    -1,   399,   351,   407,   364,   162,   352,    -1,
     408,   406,   132,    -1,   408,   406,   171,   375,    -1,   408,
     406,   387,   382,    -1,   408,   406,   387,   132,    -1,   408,
     406,   387,   375,   351,    -1,   408,   406,   375,    -1,   408,
     201,   389,   132,    -1,   408,   201,   389,   171,   375,    -1,
     408,   201,   389,   387,   382,    -1,   408,   201,   389,   387,
     132,    -1,   408,   201,   389,   387,   375,   351,    -1,   408,
     201,   389,   375,    -1,    -1,   407,    -1,   116,   138,   215,
     113,   375,   362,    -1,   154,   138,   215,   362,    -1,   412,
     576,   413,   577,    -1,   412,   576,   413,   414,   580,   581,
      -1,    -1,   409,    -1,   577,   357,   578,    -1,   578,    -1,
     371,   364,   579,    -1,   415,    -1,   416,    -1,   370,    -1,
     371,    -1,   581,   357,   582,    -1,   582,    -1,   371,  1178,
     364,   583,    -1,   417,    -1,   418,    -1,   419,    -1,   420,
      -1,   421,    -1,   585,    -1,   586,    -1,   590,    -1,   591,
      -1,   595,    -1,   863,   595,    -1,   596,    -1,   863,   596,
      -1,  1087,    -1,   600,    -1,   603,    -1,   604,    -1,   605,
      -1,   609,    -1,   610,    -1,   624,    -1,   632,    -1,   639,
      -1,   723,    -1,   781,    -1,   782,    -1,   680,    -1,   699,
      -1,   661,    -1,   662,    -1,   668,    -1,   670,    -1,   660,
      -1,   710,    -1,   706,    -1,   766,    -1,   767,    -1,   676,
      -1,   702,    -1,   773,    -1,   666,    -1,   667,    -1,   665,
      -1,   664,    -1,   621,    -1,   774,    -1,   775,    -1,   776,
      -1,   779,    -1,   780,    -1,  1143,    -1,  1145,    -1,   931,
      -1,  1075,    -1,  1076,    -1,  1077,    -1,  1078,    -1,  1079,
      -1,  1089,    -1,  1094,    -1,  1100,    -1,  1101,    -1,  1102,
      -1,  1105,    -1,  1103,    -1,  1106,    -1,  1124,    -1,  1126,
      -1,  1127,    -1,   637,    -1,   783,    -1,   638,    -1,   765,
      -1,   784,    -1,  1146,    -1,  1149,    -1,  1151,    -1,   785,
      -1,  1152,    -1,  1160,    -1,  1161,    -1,   623,    -1,   622,
      -1,   587,   788,    -1,   788,    -1,   587,   863,   788,    -1,
     863,   788,    -1,   587,   789,    -1,   789,    -1,   587,   863,
     789,    -1,   863,   789,    -1,   587,   799,    -1,   799,    -1,
     587,   863,   799,    -1,   863,   799,    -1,   813,    -1,   821,
      -1,   842,   924,   772,    -1,   863,   842,   924,   772,    -1,
     801,   807,   925,    -1,   800,    -1,   589,    -1,   588,    -1,
     588,   589,    -1,   397,   371,   357,   371,    -1,   166,   370,
      -1,   166,   371,    -1,   375,   166,   370,    -1,   375,   166,
     371,    -1,   599,    -1,   599,   402,    -1,   601,    -1,   601,
     402,    -1,  1162,    -1,   104,    -1,   109,    -1,   110,    -1,
     114,    -1,   121,    -1,   128,    -1,   143,    -1,   144,    -1,
     153,    -1,   165,    -1,   391,    -1,   395,    -1,   186,    -1,
     191,    -1,   193,    -1,   196,    -1,   199,    -1,   222,    -1,
     224,    -1,   231,    -1,   317,    -1,   243,    -1,   256,    -1,
     259,    -1,   271,    -1,   155,    -1,   221,    -1,   141,    -1,
     164,    -1,   189,    -1,   272,    -1,   318,    -1,   236,    -1,
     251,    -1,   252,    -1,   273,    -1,   274,    -1,   113,    -1,
     187,    -1,   246,    -1,   197,    -1,   124,    -1,   195,    -1,
     214,    -1,   240,    -1,   223,    -1,   134,    -1,   207,    -1,
     123,    -1,   126,    -1,   296,    -1,   285,    -1,   303,    -1,
    1162,    -1,   140,    -1,   284,    -1,   217,    -1,   388,    -1,
     395,    -1,   396,    -1,   253,    -1,   294,    -1,   106,   237,
     238,   315,   364,   250,    -1,   106,   237,   238,   315,   364,
     162,    -1,   106,   237,   238,   315,   364,   139,    -1,   106,
     237,   238,   315,   364,   375,    -1,   106,   237,   238,   375,
     375,   364,   398,    -1,   106,   237,   238,   375,   375,   364,
     206,    -1,   106,   237,   238,   375,   375,   364,   204,    -1,
     106,   237,   238,   375,   364,   370,    -1,   106,   237,   238,
     375,   375,   364,   370,    -1,   106,   237,   238,   375,   364,
     375,    -1,   106,   375,   375,    -1,   106,   375,   375,   375,
      -1,   106,   375,   109,   375,   598,    -1,   106,   375,   238,
     375,   364,   370,    -1,   106,   375,   238,   375,   364,   358,
     370,    -1,   106,   375,   238,   375,   364,   375,    -1,   106,
     375,   238,   375,   364,   372,    -1,   106,   375,   375,   285,
     375,    -1,   106,   375,   375,   325,   375,   375,   597,    -1,
      -1,   192,    -1,   241,    -1,   375,    -1,   127,   602,    -1,
     234,  1162,    -1,   232,   602,    -1,   232,   602,   248,   234,
    1162,    -1,    -1,   269,    -1,   238,   375,   606,    -1,   106,
     237,   238,   375,   606,    -1,   127,   602,   375,   137,   279,
      -1,   232,   602,   375,   137,   279,    -1,   222,   398,    -1,
     222,   270,    -1,   183,   188,   222,   375,    -1,   183,   188,
     375,   222,    -1,   183,   188,   375,    -1,  1162,    -1,  1162,
     359,  1162,    -1,  1162,   359,  1162,    -1,  1162,   359,  1162,
     359,  1162,    -1,   133,   258,  1162,   287,   117,  1162,    -1,
     133,   258,  1162,   287,   117,  1162,   611,    -1,   106,   258,
    1162,   613,    -1,   611,   612,    -1,   612,    -1,   617,    -1,
     618,    -1,   619,    -1,   614,    -1,   616,    -1,   617,    -1,
     618,    -1,   615,    -1,   614,    -1,   153,   375,    -1,   144,
     375,    -1,   619,    -1,   615,   619,    -1,   287,   117,  1162,
      -1,   245,   319,  1162,    -1,   139,   319,  1162,    -1,   285,
    1162,   620,    -1,   206,    -1,   204,    -1,   147,   258,  1162,
     701,    -1,   147,   280,  1162,    -1,   133,   280,  1162,    -1,
     625,    -1,   626,    -1,   172,   627,   248,   629,    -1,   172,
     627,   206,   607,   248,   629,   631,    -1,   172,   627,   206,
     143,  1162,   248,   629,   631,    -1,   627,   357,   628,    -1,
     628,    -1,   106,   375,    -1,   133,   108,   288,    -1,   106,
     108,   288,    -1,   147,   108,   288,    -1,   133,   219,    -1,
     133,   108,   219,    -1,   106,   108,   219,    -1,   147,   108,
     219,    -1,   159,   108,   219,    -1,   133,   249,    -1,   133,
     108,   249,    -1,   106,   108,   249,    -1,   147,   108,   249,
      -1,   133,   243,    -1,   133,   220,   243,    -1,   147,   108,
     243,    -1,   147,   220,   243,    -1,   133,   236,    -1,   133,
     108,   236,    -1,   106,   108,   236,    -1,   147,   108,   236,
      -1,   235,   108,   236,    -1,   133,   237,    -1,   106,   237,
      -1,   133,   244,    -1,   133,   108,   244,    -1,   106,   108,
     244,    -1,   140,   108,   244,    -1,   147,   108,   244,    -1,
     179,   108,   244,    -1,   311,   108,   244,    -1,   235,   108,
     244,    -1,   257,   108,   244,    -1,   133,   258,    -1,   106,
     258,    -1,   147,   258,    -1,   133,   264,    -1,   133,   108,
     264,    -1,   147,   108,   264,    -1,   172,   108,   218,    -1,
     133,   319,    -1,   106,   319,    -1,   147,   319,    -1,   133,
     108,   143,    -1,   147,   108,   143,    -1,   133,   300,   264,
      -1,   133,   108,   300,   264,    -1,   106,   108,   300,   264,
      -1,   147,   108,   300,   264,    -1,   133,   280,    -1,   147,
     108,   280,    -1,   172,   108,   280,    -1,   133,   108,   375,
      -1,   106,   108,   375,    -1,   147,   108,   375,    -1,   106,
      -1,   140,    -1,   159,    -1,   288,    -1,   179,    -1,   225,
      -1,   235,    -1,   257,    -1,   105,    -1,   105,   218,    -1,
     222,    -1,   270,    -1,  1162,    -1,   629,   357,   630,    -1,
     630,    -1,  1162,    -1,   220,    -1,    -1,   268,   172,   399,
      -1,   633,    -1,   634,    -1,   230,   627,   168,   629,    -1,
     230,   627,   206,   607,   168,   629,   635,   636,    -1,   230,
     627,   206,   143,  1162,   168,   629,   635,   636,    -1,    -1,
     119,   131,    -1,    -1,   375,    -1,   133,   243,   607,   166,
     607,    -1,   133,   220,   243,   607,   166,   607,    -1,   147,
     243,   607,    -1,   147,   220,   243,   607,    -1,   133,   645,
     315,  1162,   650,   651,   640,   268,   646,   652,    -1,   106,
     315,  1162,   102,   244,   653,    -1,   106,   315,  1162,   147,
     244,   653,    -1,   106,   315,  1162,   102,   375,   647,    -1,
     106,   315,  1162,   147,   375,   647,    -1,   106,   315,  1162,
     238,   375,   647,    -1,   106,   315,  1162,   238,   290,   644,
      -1,   106,   315,  1162,   238,   375,   698,    -1,   106,   315,
    1162,   238,   271,   370,    -1,   106,   315,  1162,   238,   314,
     153,   268,   643,    -1,   106,   315,  1162,   238,   314,   144,
      -1,   147,   315,  1162,    -1,   106,   315,  1162,   241,   659,
      -1,   106,   315,  1162,   241,   268,   314,    -1,   106,   315,
    1162,   375,   656,    -1,   106,   315,  1162,   375,   655,   656,
      -1,   106,   315,  1162,   165,   654,    -1,    -1,   375,   641,
      -1,   641,   642,    -1,   642,    -1,   375,    -1,   271,   370,
      -1,   314,   643,    -1,   192,  1162,    -1,   643,   357,   372,
      -1,   372,    -1,   375,    -1,    -1,   644,    -1,   647,    -1,
     375,    -1,   647,   648,    -1,   648,    -1,   372,   357,   370,
     649,    -1,    -1,   259,   375,    -1,   259,   375,   370,    -1,
      -1,   166,   336,    -1,   166,   375,    -1,   166,   375,   282,
      -1,    -1,   111,   375,    -1,   652,   357,   653,    -1,   653,
      -1,   168,  1162,   359,  1162,   248,  1162,   359,  1162,    -1,
     168,  1162,   359,  1162,   213,  1162,   248,  1162,   359,  1162,
     213,  1162,    -1,   359,    -1,    -1,   297,   370,    -1,   105,
      -1,   105,   297,   370,    -1,   398,   271,   370,    -1,   398,
      -1,   375,    -1,   271,   370,    -1,    -1,   244,   657,    -1,
     657,   357,   658,    -1,   658,    -1,  1162,   359,  1162,    -1,
    1162,   359,  1162,   213,  1162,    -1,    -1,   375,    -1,   113,
     375,   351,   370,   352,    -1,   320,   244,   607,    -1,   293,
     607,   248,  1162,    -1,   305,   244,   607,   248,   115,   147,
     663,    -1,    -1,   293,   248,  1162,    -1,   147,   236,   607,
      -1,   147,   288,   607,    -1,   147,   244,   607,   675,    -1,
     304,   244,   607,    -1,   328,   244,  1162,   351,   669,   352,
      -1,   213,  1162,   248,   244,  1162,   357,   669,    -1,   213,
    1162,   248,   244,  1162,    -1,   329,   244,  1162,   671,   725,
     674,   733,    -1,   213,   117,  1162,   351,   759,   352,   351,
     672,   352,    -1,   672,   357,   673,    -1,   673,    -1,   244,
    1162,   248,   213,  1162,   260,   187,   246,   351,   729,   352,
      -1,   244,  1162,   248,   213,  1162,   260,   351,   729,   352,
      -1,   244,  1162,   248,   213,  1162,   260,   139,    -1,    -1,
    1184,    -1,    -1,   119,    -1,   119,   131,    -1,   106,   236,
     607,   677,    -1,   106,   236,   607,   709,    -1,   677,   678,
      -1,   678,    -1,   241,   268,   370,    -1,   241,   268,   358,
     370,    -1,   375,   117,   370,    -1,   375,   117,   358,   370,
      -1,   375,   268,   370,    -1,   375,   268,   358,   370,    -1,
     375,   370,    -1,   375,   358,   370,    -1,   375,    -1,   136,
      -1,   200,    -1,   325,   679,    -1,   192,    -1,    -1,   310,
      -1,   261,    -1,   106,   244,   607,   102,   700,   351,   742,
     352,   733,  1185,    -1,   106,   244,   607,   106,   700,   351,
     592,   238,   139,   960,   352,    -1,   106,   244,   607,   106,
     700,   351,   592,   147,   139,   352,    -1,   106,   244,   607,
     106,   700,   351,   592,   201,   202,   352,    -1,   106,   244,
     607,   106,   700,   351,   592,   202,   352,    -1,   106,   244,
     607,   106,   700,   273,   351,   759,   352,   274,   111,   351,
     736,   352,  1185,    -1,   106,   244,   607,   106,   700,   273,
     274,   111,   351,   736,   352,  1185,    -1,   106,   244,   607,
     147,   700,   592,    -1,   106,   244,   607,   293,   248,  1162,
      -1,   106,   244,   607,   312,   370,    -1,   106,   244,   607,
     375,   685,    -1,   106,   244,   607,   105,   288,   698,    -1,
     106,   244,   607,   293,   125,   592,   248,   592,    -1,   106,
     244,   607,   285,  1184,    -1,   106,   244,   607,   686,    -1,
     106,   244,   607,   106,   319,  1162,   690,   687,    -1,   106,
     244,   607,   294,   607,   685,   681,   683,   684,    -1,   106,
     244,   607,   147,   192,   254,   351,   758,   352,   701,    -1,
     106,   244,   607,   293,   130,  1162,   248,  1162,    -1,    -1,
     682,    -1,   259,   375,  1162,    -1,    -1,   293,   375,    -1,
      -1,   375,   167,   185,   375,    -1,    -1,   213,  1162,    -1,
     102,   696,   693,    -1,   124,   213,    -1,   147,   213,  1162,
      -1,   195,   214,  1162,   357,  1162,   181,   697,   693,    -1,
     293,   213,  1162,   248,  1162,    -1,   285,   213,  1162,  1184,
      -1,   240,   213,  1162,   113,   351,   729,   352,   181,   351,
     697,   693,   357,   697,   693,   352,    -1,   240,   213,  1162,
     260,   351,   729,   352,   181,   351,   697,   693,   357,   697,
     693,   352,    -1,   320,   213,  1162,    -1,   153,   233,   197,
      -1,   144,   233,   197,    -1,   106,   213,  1162,   319,  1162,
     690,   687,    -1,    -1,   273,   351,   688,   352,    -1,   688,
     357,   689,    -1,   689,    -1,   592,   319,  1162,    -1,    -1,
     288,   351,   691,   352,    -1,   691,   357,   692,    -1,   692,
      -1,  1162,   319,  1162,    -1,    -1,   288,   351,   694,   352,
      -1,   694,   357,   695,    -1,   695,    -1,  1162,   213,  1162,
      -1,  1162,   213,  1162,   319,  1162,    -1,   213,  1162,  1182,
      -1,   213,  1162,   260,   187,   246,   351,   729,   352,  1182,
      -1,   213,  1162,  1182,    -1,   153,    -1,   144,    -1,   106,
     244,   607,   102,   740,    -1,   106,   244,   607,   147,   130,
    1162,    -1,   106,   244,   607,   147,   216,   185,   701,    -1,
     106,   244,   607,   147,   254,   351,   758,   352,   701,    -1,
      -1,   125,    -1,    -1,   119,    -1,   106,   288,   607,   703,
      -1,   106,   288,   607,   238,   704,    -1,   375,    -1,   223,
      -1,   223,   213,  1162,    -1,   293,   248,   607,    -1,   704,
      -1,   375,   705,    -1,   206,    -1,   204,    -1,   364,   206,
      -1,   364,   204,    -1,   133,   236,   607,   707,   708,    -1,
      -1,   677,    -1,    -1,   709,    -1,   698,   375,   244,    -1,
     711,   607,   206,   607,   351,   757,   352,   714,   712,   713,
     720,    -1,   133,   288,    -1,   133,   254,   288,    -1,   133,
     192,   254,   288,    -1,    -1,   375,   182,   375,    -1,    -1,
     238,   375,   364,   206,    -1,   238,   375,   364,   204,    -1,
      -1,   715,    -1,   192,    -1,   192,   351,   716,   352,    -1,
     716,   357,   717,    -1,   717,    -1,   213,  1162,   206,  1162,
     718,    -1,    -1,   319,  1162,    -1,    -1,   259,   288,   720,
      -1,    -1,   721,    -1,   721,   722,    -1,   722,    -1,   319,
    1162,    -1,   271,   370,    -1,   272,    -1,   133,   244,   607,
     351,   738,   352,   724,   733,    -1,   133,   244,   607,   351,
     738,   352,   724,   733,   111,   828,    -1,   133,   244,   607,
     724,   733,   111,   828,    -1,   133,   244,   607,   168,   244,
     375,   607,   682,    -1,    -1,   763,    -1,   731,    -1,   763,
     731,    -1,   726,   725,    -1,   726,   725,   731,    -1,  1184,
      -1,    -1,   153,   233,   197,    -1,   144,   233,   197,    -1,
     213,   117,  1162,   351,   759,   352,   351,   727,   352,    -1,
     727,   357,   728,    -1,   728,    -1,   213,  1162,   260,   187,
     246,   351,   729,   352,  1182,    -1,   213,  1162,  1182,    -1,
     213,  1162,   260,   351,   729,   352,  1182,    -1,   213,  1162,
     260,   139,  1182,    -1,   729,   357,   730,    -1,   730,    -1,
     960,    -1,   731,   732,    -1,   732,    -1,   319,  1162,    -1,
     179,   375,   191,   370,    -1,    -1,   734,    -1,   734,   735,
      -1,   735,    -1,   273,   351,   759,   352,   274,   111,   351,
     736,   352,    -1,   273,   274,   111,   351,   736,   352,    -1,
     736,   737,    -1,   737,    -1,   319,  1162,    -1,   375,    -1,
     738,   357,   739,    -1,   739,    -1,   740,    -1,   744,    -1,
     741,   254,   756,   713,   719,    -1,   741,   216,   185,   756,
     713,   719,    -1,   741,   167,   185,   351,   759,   352,   760,
      -1,   741,   192,   254,   756,   713,   719,    -1,   741,   192,
     254,   929,   713,   719,    -1,   741,   122,   935,   352,    -1,
      -1,   130,  1162,    -1,   743,    -1,   740,    -1,   743,   357,
     744,    -1,   744,    -1,   592,   751,   745,   746,   750,   747,
      -1,    -1,   310,    -1,   261,    -1,   262,    -1,    -1,   176,
     233,   370,    -1,    -1,   748,    -1,   748,   749,    -1,   749,
      -1,   741,   202,    -1,   741,   201,   202,    -1,   741,   122,
     935,   352,    -1,   741,   254,   929,   713,   719,    -1,   741,
     216,   185,   929,   713,   719,    -1,   741,   760,    -1,   741,
     192,   254,   929,   713,   719,    -1,    -1,   139,   960,    -1,
      -1,   752,   753,    -1,  1162,    -1,  1162,   351,   370,   352,
      -1,  1162,   351,   370,   357,   370,   352,    -1,  1162,   351,
     370,   357,   356,   370,   352,    -1,  1162,   351,   370,   357,
     358,   370,   352,    -1,    -1,   754,    -1,   375,   259,   372,
      -1,    -1,   351,   759,   352,    -1,   351,   758,   352,    -1,
     757,   357,   935,   929,    -1,   935,   929,    -1,   758,   357,
     592,   929,    -1,   592,   929,    -1,   759,   357,   592,    -1,
     592,    -1,   225,   607,   755,   761,    -1,    -1,   206,   179,
     762,    -1,   206,   257,   762,    -1,   206,   140,   762,    -1,
     206,   140,   119,    -1,   375,   375,    -1,   228,    -1,   312,
     370,    -1,    -1,   763,    -1,   133,   221,   607,   351,   370,
     745,   746,   352,   764,  1181,    -1,   133,   221,   607,   351,
     743,   352,   764,  1181,    -1,   133,   768,   264,   607,   769,
     111,   828,   772,    -1,   133,   208,   294,   768,   264,   607,
     769,   111,   828,   772,    -1,    -1,   375,   375,    -1,   375,
      -1,    -1,   351,   770,   352,    -1,   770,   357,   771,    -1,
     771,    -1,   592,    -1,    -1,   268,   222,   398,    -1,   106,
     264,   607,   128,    -1,   147,   264,   607,    -1,   133,   137,
     279,  1162,   259,  1162,    -1,   133,   777,   137,   279,  1162,
     259,  1162,    -1,   133,   137,   279,  1162,   778,   259,  1162,
      -1,   133,   777,   137,   279,  1162,   778,   259,  1162,    -1,
     147,   137,   279,  1162,    -1,   147,   777,   137,   279,  1162,
      -1,   220,    -1,   375,    -1,   129,   248,  1162,   287,   117,
    1162,    -1,   106,   137,   323,   241,    -1,   106,   137,   323,
     375,    -1,   106,   137,   323,   375,   375,    -1,   106,   237,
     123,   137,   279,   105,    -1,   106,   237,   123,   137,   279,
    1162,    -1,   139,   960,    -1,   229,   935,    -1,   106,   221,
     607,   375,    -1,   106,   221,   607,   375,   375,    -1,   147,
     221,   607,    -1,   126,   206,   244,   607,   182,   372,    -1,
     126,   206,   125,   608,   182,   372,    -1,   607,  1188,    -1,
     351,   828,   352,    -1,   375,   357,   375,    -1,   375,    -1,
     140,   851,   786,   859,   934,   919,    -1,   140,   851,   168,
     786,   859,   934,   919,    -1,   140,   851,   787,   168,   796,
     934,    -1,   140,   851,   168,   787,   259,   796,   934,    -1,
     179,   851,   181,   786,   859,   139,   260,    -1,   179,   851,
     181,   786,   859,   820,   260,   794,    -1,   179,   851,   181,
     786,   859,   828,    -1,   179,   851,   181,   786,   859,   863,
     828,    -1,   179,   851,   181,   786,   859,   351,   819,   352,
     828,    -1,   179,   851,   181,   786,   859,   351,   819,   352,
     863,   828,    -1,   179,   851,   105,   790,   828,    -1,   790,
     791,    -1,   791,    -1,   181,   786,   820,   260,   351,   792,
     352,    -1,   792,   357,   793,    -1,   793,    -1,   960,    -1,
     139,    -1,   794,   357,   795,    -1,   795,    -1,   351,   792,
     352,    -1,   796,   357,   797,    -1,   797,    -1,   786,   859,
      -1,   798,    -1,   797,   900,   184,   797,   206,   935,    -1,
     257,   851,   796,   238,   808,   934,   919,    -1,   155,   851,
     181,   607,   755,   260,   351,   792,   352,    -1,   802,    -1,
     803,    -1,   141,   851,   854,  1169,   804,   934,    -1,   168,
     805,    -1,   806,    -1,  1162,   359,  1162,    -1,  1162,    -1,
      -1,   164,    -1,   189,    -1,   808,   357,   809,    -1,   809,
      -1,   810,   364,   960,    -1,   810,   364,   139,    -1,   351,
     811,   352,   364,   960,    -1,   592,    -1,  1162,   359,   592,
      -1,   811,   357,   812,    -1,   812,    -1,   592,    -1,  1162,
     359,  1162,    -1,   195,   851,   181,   607,  1188,   859,   259,
     862,   206,   935,   814,    -1,   814,   815,    -1,   815,    -1,
     265,   816,   247,   817,    -1,   375,    -1,   201,   375,    -1,
     375,   375,    -1,   257,   238,   808,   934,   919,   818,    -1,
     179,   820,   260,   351,   792,   352,   934,    -1,    -1,   140,
     934,   919,    -1,   819,   357,   810,    -1,   810,    -1,    -1,
     351,   819,   352,    -1,   196,   851,   181,   607,  1188,   755,
     168,   607,   822,   934,   919,    -1,    -1,   351,   823,   352,
      -1,   823,   357,   824,    -1,   824,    -1,   960,    -1,   139,
      -1,   826,    -1,   827,    -1,   832,   918,   922,    -1,   835,
     832,   918,   922,    -1,   829,    -1,   830,    -1,   833,   918,
     922,    -1,   835,   833,   918,   922,    -1,   253,    -1,   253,
     105,    -1,   180,    -1,   289,    -1,   832,   831,   838,    -1,
     838,    -1,   833,   831,   839,    -1,   839,    -1,    -1,   268,
     836,    -1,   268,   836,    -1,   836,   357,   837,    -1,   837,
      -1,  1162,   769,   111,   351,   829,   352,    -1,   351,   832,
     352,    -1,   840,    -1,   351,   833,   352,    -1,   841,    -1,
     235,   851,   853,   854,   855,   860,   934,   913,   852,   912,
      -1,   235,   851,   853,   854,  1168,   860,   934,   913,   852,
     912,    -1,   235,   851,   853,   854,   856,   860,   934,   913,
     852,   912,    -1,   843,    -1,   844,    -1,   845,   918,   922,
      -1,   846,   845,   918,   922,    -1,   845,   831,   849,    -1,
     849,    -1,   268,   847,    -1,   847,   357,   848,    -1,   848,
      -1,  1162,   769,   111,   351,   843,   352,    -1,   351,   845,
     352,    -1,   850,    -1,   235,   851,   853,   854,  1171,   860,
     934,   913,   852,   912,    -1,    -1,   368,    -1,    -1,   173,
     117,   909,    -1,    -1,   105,    -1,   146,    -1,   355,    -1,
     857,    -1,    -1,   181,  1012,    -1,    -1,   181,  1012,    -1,
     181,  1170,    -1,   857,   357,   858,    -1,   858,    -1,  1162,
     359,   355,    -1,  1162,   359,  1162,   359,   355,    -1,   960,
     859,    -1,    -1,  1162,    -1,   111,   592,    -1,   372,    -1,
     111,   372,    -1,   168,   861,    -1,   861,   357,   862,    -1,
     862,    -1,  1162,   359,  1162,  1188,   868,   859,    -1,  1162,
    1188,   868,   859,    -1,   351,   828,   352,   868,   859,    -1,
     324,   351,  1162,   357,   372,   352,   859,    -1,   863,   351,
     828,   352,   859,    -1,  1162,   346,  1162,   859,    -1,   899,
      -1,   244,   351,   867,   352,   859,    -1,   896,    -1,   325,
      -1,   326,    -1,   327,   864,    -1,   354,    -1,   351,   352,
     354,    -1,   351,   865,   352,   354,    -1,   865,   357,   866,
      -1,   866,    -1,   372,    -1,   975,    -1,   306,   351,   988,
     352,    -1,   375,    -1,  1162,   359,  1162,   359,   592,    -1,
    1162,   359,   592,    -1,    -1,   869,    -1,   876,    -1,   298,
     351,   870,   872,   873,   352,    -1,   870,   357,   871,    -1,
     871,    -1,   375,   351,   960,   352,   859,    -1,   375,   351,
     355,   352,   859,    -1,   166,   351,   592,   352,    -1,   166,
     592,    -1,   177,   351,   874,   352,    -1,   874,   357,   875,
      -1,   875,    -1,   894,   859,    -1,   299,   877,   351,   878,
     166,   878,   881,   352,    -1,    -1,   375,   303,    -1,   351,
     879,   352,    -1,   880,    -1,   879,   357,   880,    -1,   880,
      -1,   592,    -1,   177,   351,   882,   352,    -1,   882,   357,
     883,    -1,   883,    -1,   884,   111,   887,    -1,   884,    -1,
     351,   885,   352,    -1,   886,    -1,   885,   357,   886,    -1,
     886,    -1,   592,    -1,   351,   888,   352,    -1,   889,    -1,
     888,   357,   889,    -1,   889,    -1,   890,    -1,   891,    -1,
     891,   347,   892,    -1,   892,    -1,   892,   356,   893,    -1,
     892,   358,   893,    -1,   893,    -1,   893,   355,   894,    -1,
     893,   360,   894,    -1,   894,    -1,   356,   895,    -1,   358,
     895,    -1,   895,    -1,   202,    -1,   370,    -1,   369,    -1,
     372,    -1,   373,    -1,   342,    -1,   343,    -1,  1162,   351,
     897,   352,   859,    -1,   897,   357,   898,    -1,   898,    -1,
    1162,  1188,    -1,  1162,   359,  1162,  1188,    -1,   862,   900,
     184,   862,   206,   935,    -1,    -1,   178,    -1,   186,   901,
      -1,   231,   901,    -1,   169,   901,    -1,    -1,   211,    -1,
     403,   351,   903,   352,    -1,   384,   351,   903,   352,    -1,
     903,   357,   904,    -1,   904,    -1,   960,    -1,   410,   351,
     906,   352,    -1,   906,   357,   908,    -1,   908,    -1,   906,
     357,   907,    -1,   907,    -1,   351,   352,    -1,   902,    -1,
     960,    -1,   909,   357,   910,    -1,   910,    -1,   902,    -1,
     905,    -1,   960,   911,    -1,    -1,   411,    -1,    -1,   174,
     935,    -1,    -1,   914,   916,   917,    -1,   916,   917,   915,
      -1,   241,   268,   935,    -1,    -1,   914,    -1,   129,   117,
     935,    -1,   301,   935,    -1,    -1,   375,   194,    -1,    -1,
     209,   117,   927,    -1,   209,   375,   117,   927,    -1,    -1,
     920,    -1,   191,   921,    -1,   921,   357,   935,    -1,   921,
     375,   935,    -1,   166,   325,   935,    -1,   935,    -1,    -1,
     920,    -1,   923,    -1,   194,   935,    -1,    -1,   166,   257,
     925,    -1,    -1,   375,    -1,   297,   370,   926,    -1,    -1,
     330,    -1,   331,    -1,   332,    -1,   333,    -1,   334,    -1,
     335,    -1,   927,   357,   928,    -1,   928,    -1,   960,   929,
     930,    -1,    -1,   112,    -1,   142,    -1,    -1,   303,   388,
      -1,   303,   395,    -1,   311,   244,  1162,  1188,   176,   932,
     290,   925,   933,    -1,   311,   244,  1162,   359,  1162,  1188,
     176,   932,   290,   925,   933,    -1,   233,   375,    -1,   375,
     257,    -1,   375,   233,   375,    -1,   375,    -1,    -1,   256,
     396,   375,    -1,    -1,   266,   935,    -1,   936,    -1,   936,
     208,   937,    -1,   937,    -1,   937,   107,   938,    -1,   938,
      -1,   201,   939,    -1,   939,    -1,   960,   283,   960,   107,
     960,    -1,   960,   201,   283,   960,   107,   960,    -1,   960,
     190,   960,    -1,   960,   201,   190,   960,    -1,   960,   190,
     960,   156,   960,    -1,   960,   201,   190,   960,   156,   960,
      -1,   960,   940,   960,    -1,   960,   941,   960,    -1,   960,
     942,   960,    -1,   960,   943,   960,    -1,   960,   944,   960,
      -1,   960,   945,   960,    -1,   960,   946,   959,    -1,   960,
     947,   959,    -1,   960,   948,   959,    -1,   960,   949,   959,
      -1,   960,   950,   959,    -1,   960,   951,   959,    -1,   960,
     952,   959,    -1,   960,   953,   959,    -1,   960,   954,   959,
      -1,   960,   955,   959,    -1,   960,   956,   959,    -1,   960,
     957,   959,    -1,   960,   182,   202,    -1,   960,   182,   201,
     202,    -1,   284,   989,    -1,   254,   989,    -1,   958,   350,
     375,    -1,   958,   350,   389,    -1,   960,    -1,   364,    -1,
     363,   365,    -1,   349,   364,    -1,   363,    -1,   363,   364,
      -1,   365,    -1,   365,   364,    -1,   364,   105,    -1,   363,
     365,   105,    -1,   349,   364,   105,    -1,   201,   177,    -1,
     363,   105,    -1,   363,   364,   105,    -1,   365,   105,    -1,
     365,   364,   105,    -1,   364,   108,    -1,   364,   239,    -1,
     177,    -1,   363,   365,   108,    -1,   363,   365,   239,    -1,
     349,   364,   108,    -1,   349,   364,   239,    -1,   363,   108,
      -1,   363,   239,    -1,   363,   364,   108,    -1,   363,   364,
     239,    -1,   365,   108,    -1,   365,   239,    -1,   365,   364,
     108,    -1,   365,   364,   239,    -1,  1162,    -1,  1162,   359,
    1162,    -1,   351,   988,   352,    -1,   989,    -1,  1162,   359,
    1162,   359,   592,    -1,  1162,   359,   592,    -1,   592,    -1,
    1166,    -1,   895,    -1,   961,    -1,   961,   347,   962,    -1,
     962,    -1,   962,   356,   963,    -1,   962,   358,   963,    -1,
     963,    -1,   963,   355,   964,    -1,   963,   360,   964,    -1,
     964,    -1,   356,   965,    -1,   358,   965,    -1,   217,   965,
      -1,   965,    -1,   202,    -1,   250,    -1,   162,    -1,   321,
      -1,   322,    -1,   958,   350,   317,    -1,   370,    -1,   369,
      -1,   372,    -1,   373,    -1,   366,    -1,  1166,    -1,  1162,
     359,  1162,   359,  1162,   359,   592,   967,    -1,  1162,   359,
    1162,   359,   592,   967,    -1,  1162,   359,   592,   967,    -1,
     592,   967,    -1,   339,   359,   592,    -1,   188,    -1,   975,
      -1,   351,   988,   352,    -1,   989,    -1,  1013,    -1,   968,
      -1,  1162,   359,  1162,   359,   592,    -1,  1162,   359,   592,
      -1,   592,    -1,    -1,   367,    -1,   120,   960,   969,   972,
     154,    -1,   120,   973,   972,   154,    -1,   969,   970,    -1,
     970,    -1,   265,   960,   971,    -1,   247,   960,    -1,    -1,
     151,   960,    -1,   973,   974,    -1,   974,    -1,   265,   935,
     971,    -1,  1162,   351,   988,   352,   976,   992,   979,    -1,
    1162,   359,   593,   351,   988,   352,    -1,  1162,   351,   352,
     979,    -1,  1162,   359,   593,   351,   352,    -1,  1162,   351,
     355,   352,   992,   979,    -1,  1162,   351,   105,   355,   352,
     992,   979,    -1,  1162,   351,   105,   988,   352,   992,   979,
      -1,  1162,   351,   146,   988,   352,   979,    -1,  1162,   359,
    1162,   359,   593,   351,   988,   352,    -1,  1162,   359,  1162,
     359,   593,   351,   352,    -1,   594,   351,   988,   352,    -1,
     594,   351,   352,    -1,   121,   351,   935,   111,   752,   352,
      -1,   302,   966,    -1,    -1,   281,   173,   351,   209,   117,
     977,   352,    -1,   977,   357,   978,    -1,   978,    -1,   960,
      -1,   960,   112,    -1,   960,   142,    -1,    -1,   212,   351,
     980,   983,   984,   352,    -1,    -1,   213,   117,   981,    -1,
     981,   357,   982,    -1,   982,    -1,   960,    -1,    -1,   209,
     117,   927,    -1,    -1,   375,   283,   985,   107,   986,    -1,
     375,   985,    -1,   375,   276,    -1,   278,    -1,   987,   276,
      -1,   987,   277,    -1,   375,   277,    -1,   278,    -1,   987,
     276,    -1,   987,   277,    -1,   370,    -1,   375,   370,   375,
      -1,   988,   357,   935,    -1,   935,    -1,   351,   834,   990,
     922,   352,    -1,   990,   831,   991,    -1,   991,    -1,   841,
      -1,    -1,   307,   351,   375,   993,   209,   117,   927,   352,
      -1,   388,    -1,   395,    -1,   997,   607,  1001,   229,  1016,
    1000,  1019,  1005,    -1,   998,   607,  1001,  1000,  1019,  1005,
      -1,   999,   607,  1000,  1020,  1005,    -1,   133,   170,    -1,
     133,   208,   294,   170,    -1,   133,   219,    -1,   133,   208,
     294,   219,    -1,   133,   252,    -1,   133,   208,   294,   252,
      -1,   111,    -1,   182,    -1,    -1,   351,   352,    -1,   351,
    1002,   352,    -1,  1002,   357,  1003,    -1,  1003,    -1,  1162,
    1004,  1016,  1006,    -1,    -1,   176,    -1,   210,    -1,   176,
     210,    -1,    -1,  1162,    -1,    -1,   361,   364,  1009,    -1,
     139,  1009,    -1,   960,    -1,   935,    -1,   935,    -1,  1162,
      -1,  1162,   359,  1162,    -1,   975,    -1,  1162,    -1,  1162,
     351,   352,    -1,  1162,   351,   988,   352,    -1,  1012,   357,
    1014,    -1,  1014,    -1,  1162,   353,  1009,   354,    -1,  1162,
     359,  1162,   353,  1009,   354,    -1,  1162,   353,  1009,   354,
     359,   592,    -1,  1162,   359,  1162,   353,  1009,   354,   359,
     592,    -1,  1162,    -1,  1162,   359,   592,    -1,  1162,   359,
    1162,   359,   592,    -1,  1013,    -1,  1162,    -1,   592,   350,
     295,    -1,  1162,   359,   592,   350,   295,    -1,   592,   350,
     251,    -1,  1162,   359,   592,   350,   251,    -1,  1162,   359,
    1162,   359,   592,   350,   251,    -1,  1017,    -1,  1162,    -1,
    1162,   359,  1162,    -1,  1162,   359,  1162,   359,  1162,    -1,
    1162,   351,   370,   352,    -1,  1162,   351,   370,   357,   370,
     352,    -1,  1162,   351,   370,   357,   356,   370,   352,    -1,
    1162,   351,   370,   357,   358,   370,   352,    -1,   138,  1021,
     116,  1041,  1034,   154,  1005,   362,    -1,   116,  1041,  1034,
     154,  1005,   362,    -1,  1021,   116,  1041,  1034,   154,    -1,
    1024,   154,    -1,    -1,  1022,    -1,  1022,  1023,    -1,  1023,
      -1,  1025,    -1,  1026,    -1,  1027,    -1,  1029,    -1,  1024,
    1029,    -1,  1029,    -1,   135,  1162,  1001,   182,   828,   924,
     362,    -1,  1162,   157,   362,    -1,  1162,  1028,  1016,  1006,
     362,    -1,    -1,   286,    -1,   251,  1162,   182,   375,   351,
    1032,   352,   362,    -1,   251,  1162,   182,   244,   203,  1030,
    1031,   362,    -1,  1017,    -1,  1162,    -1,  1162,   359,  1162,
      -1,  1162,   359,  1162,   359,  1162,    -1,   288,   117,   752,
      -1,  1032,   357,  1033,    -1,  1033,    -1,   592,   752,    -1,
      -1,  1035,    -1,   157,  1036,    -1,    -1,  1037,    -1,  1037,
    1038,    -1,  1038,    -1,   265,  1039,  1051,    -1,   265,   291,
    1051,    -1,  1040,   208,  1040,    -1,  1040,    -1,  1162,    -1,
    1162,   359,  1162,    -1,  1041,  1042,    -1,  1042,    -1,  1043,
      -1,  1046,    -1,  1066,    -1,  1067,    -1,  1047,    -1,  1069,
      -1,  1048,    -1,  1052,    -1,  1059,    -1,  1070,    -1,  1071,
      -1,  1018,    -1,  1072,    -1,  1073,    -1,  1074,    -1,  1044,
      -1,   363,   363,  1162,   365,   365,    -1,   825,   924,   362,
      -1,   863,   825,   924,   362,    -1,   789,   362,    -1,   863,
     789,   362,    -1,   799,   362,    -1,   863,   799,   362,    -1,
     788,   362,    -1,   863,   788,   362,    -1,   813,   362,    -1,
     821,   362,    -1,   800,   362,    -1,   603,   362,    -1,   600,
     362,    -1,   599,   362,    -1,   601,   362,    -1,  1045,   362,
      -1,  1085,    -1,  1014,   361,   364,  1009,   362,    -1,   238,
    1014,   364,  1009,   362,    -1,   163,  1162,   181,  1012,   362,
      -1,   163,  1162,   359,  1162,   181,  1012,   362,    -1,   175,
    1008,  1051,  1049,   154,   175,   362,    -1,    -1,   151,  1041,
      -1,  1050,    -1,   308,  1008,  1051,  1049,    -1,   152,  1008,
    1051,  1049,    -1,   247,  1041,    -1,   120,  1053,  1058,   154,
     120,   362,    -1,   120,  1007,  1055,  1058,   154,   120,   362,
      -1,  1054,  1053,    -1,  1054,    -1,   265,  1008,  1051,    -1,
    1056,  1055,    -1,  1056,    -1,   265,  1057,  1051,    -1,  1007,
      -1,    -1,   151,  1041,    -1,  1062,    -1,  1061,    -1,  1063,
      -1,  1065,    -1,   194,  1041,   154,   194,    -1,   267,  1008,
    1060,  1005,   362,    -1,  1060,  1005,   362,    -1,   166,  1015,
    1189,  1007,   348,  1007,  1064,  1060,  1005,   362,    -1,   166,
    1015,   176,   316,  1007,   348,  1007,  1064,  1060,  1005,   362,
      -1,    -1,   318,  1007,    -1,   166,  1015,  1189,  1011,  1060,
    1005,   362,    -1,   123,  1162,   362,    -1,   123,  1162,   359,
    1162,   362,    -1,   160,  1005,  1068,   362,    -1,    -1,   265,
    1008,    -1,   171,  1162,   362,    -1,   202,   362,    -1,   207,
    1011,   362,    -1,   207,  1162,   359,  1011,   362,    -1,   292,
    1040,   362,    -1,   292,   362,    -1,   229,   362,    -1,   229,
    1009,   362,    -1,   132,   362,    -1,   106,   219,   607,   128,
      -1,   106,   170,   607,   128,    -1,   147,   219,   607,    -1,
     147,   170,   607,    -1,   147,   252,   607,    -1,  1086,  1010,
      -1,  1082,  1083,    -1,  1083,    -1,   363,   363,  1162,   365,
     365,   138,  1021,    -1,   138,  1021,    -1,   363,   363,  1162,
     365,   365,    -1,   116,  1041,  1034,   154,  1005,    -1,   158,
      -1,   159,    -1,  1011,    -1,  1162,   359,  1011,    -1,  1162,
     359,  1162,   359,  1011,    -1,  1175,   361,   364,    -1,   238,
    1162,   364,  1162,    -1,   238,   104,   364,   153,    -1,   238,
     104,   364,   144,    -1,   375,   364,  1117,    -1,   133,   137,
    1162,  1088,  1090,  1091,    -1,   110,    -1,   199,    -1,  1092,
    1093,    -1,  1093,  1092,    -1,   375,   238,  1162,    -1,   375,
     375,   238,  1162,    -1,   106,   137,  1162,  1098,    -1,   106,
     137,  1162,  1099,    -1,   106,   137,  1090,    -1,   106,   137,
     114,   193,   248,   372,    -1,   106,   137,   114,   319,  1162,
     248,   372,    -1,   106,   137,   114,   137,   248,   372,    -1,
     106,   137,   133,   375,   372,  1097,    -1,   106,   137,   293,
     375,   372,   248,   372,    -1,   106,   137,   224,   137,  1095,
    1096,    -1,   106,   137,   116,   375,    -1,   106,   137,   154,
     375,    -1,    -1,   256,   375,    -1,   256,   375,   372,    -1,
      -1,   259,   114,   193,    -1,    -1,   111,   372,    -1,   375,
      -1,   375,   375,    -1,   375,   392,    -1,   375,   160,    -1,
     237,   123,   370,    -1,   375,   127,   370,    -1,   375,   232,
     370,    -1,   147,   137,  1162,    -1,   133,   319,  1162,   375,
    1107,  1112,    -1,   133,   134,   319,  1162,   375,   370,  1109,
      -1,   133,   134,   319,  1162,   375,   370,   375,  1109,    -1,
     133,   245,   319,  1162,   375,  1107,  1114,    -1,   106,   319,
    1162,  1119,    -1,   106,   319,  1162,   293,   375,  1121,   248,
    1121,    -1,   106,   319,  1162,   106,   375,   372,  1120,    -1,
     106,   319,  1162,  1104,    -1,   116,   114,    -1,   154,   114,
      -1,   106,   319,  1162,   107,   375,  1107,    -1,   106,   319,
    1162,   147,   375,  1121,    -1,   106,   319,  1162,   106,   375,
     372,  1110,    -1,   106,   319,  1162,   106,   375,   372,   375,
    1118,    -1,   147,   319,  1162,  1123,    -1,  1107,   357,  1108,
      -1,  1108,    -1,   372,  1109,    -1,   372,   375,   370,  1109,
      -1,   372,   375,   370,   375,  1109,    -1,   372,   375,  1109,
      -1,   372,   375,   370,   375,   375,  1109,    -1,    -1,  1110,
      -1,   375,   204,    -1,   375,   206,    -1,   375,   206,   396,
    1118,    -1,   375,   206,   375,  1118,    -1,   375,   206,   396,
     370,  1111,    -1,   375,   206,   396,   370,   375,  1111,    -1,
     375,   206,   375,   375,    -1,   375,   375,    -1,   375,  1118,
      -1,    -1,  1113,    -1,  1113,  1113,    -1,  1115,    -1,  1116,
      -1,    -1,  1115,    -1,   309,  1118,    -1,   296,   375,   375,
      -1,   370,    -1,   370,   375,    -1,   370,    -1,   370,   375,
      -1,   313,    -1,   314,    -1,   313,    -1,   314,    -1,  1121,
     357,  1122,    -1,  1122,    -1,   372,    -1,    -1,   375,   375,
     635,    -1,   375,   375,   107,   375,   635,    -1,  1125,   607,
    1128,   206,   607,  1132,  1139,  1141,    -1,   133,   249,    -1,
     133,   208,   294,   249,    -1,   106,   249,   607,  1142,    -1,
     147,   249,   607,    -1,  1130,  1129,    -1,  1129,   208,   140,
      -1,  1129,   208,   179,    -1,  1129,   208,   257,  1131,    -1,
     179,    -1,   140,    -1,   257,  1131,    -1,   115,    -1,   103,
      -1,    -1,   203,   759,    -1,    -1,   226,  1133,    -1,  1133,
     357,  1134,    -1,  1134,    -1,  1135,  1136,  1137,  1138,    -1,
     205,    -1,   198,    -1,    -1,   233,    -1,   244,    -1,    -1,
     111,    -1,  1162,    -1,    -1,   166,   150,   233,  1140,    -1,
     166,   150,   242,    -1,    -1,   265,   351,   935,   352,    -1,
    1000,  1019,    -1,   153,    -1,   144,    -1,   128,    -1,  1144,
    1162,   111,   372,    -1,   133,   143,    -1,   133,   208,   294,
     143,    -1,   147,   143,  1162,    -1,   133,   300,   264,   607,
     769,   724,   733,  1147,   111,   829,    -1,    -1,   375,   375,
      -1,   375,  1148,    -1,   375,   375,  1148,    -1,   375,   375,
     375,   375,    -1,   375,   375,   375,  1148,    -1,   375,   375,
     375,   375,  1148,    -1,   206,   375,    -1,   206,   127,    -1,
     106,   300,   264,   607,  1150,    -1,   375,   375,    -1,   375,
    1148,    -1,   375,   375,  1148,    -1,   147,   300,   264,   607,
      -1,   133,   375,   375,  1084,  1085,   241,   960,  1153,  1154,
    1156,  1158,    -1,    -1,   154,   960,    -1,    -1,  1155,    -1,
     375,   370,   375,    -1,    -1,  1157,    -1,   153,    -1,   144,
      -1,    -1,  1159,    -1,   126,   372,    -1,   106,   375,   375,
     238,  1084,  1085,    -1,   106,   375,   375,   238,   241,   960,
      -1,   106,   375,   375,   238,   154,   960,    -1,   106,   375,
     375,   238,  1155,    -1,   106,   375,   375,   238,  1157,    -1,
     106,   375,   375,   238,  1159,    -1,   147,   375,   375,    -1,
     375,    -1,   374,    -1,  1163,    -1,   412,    -1,   413,    -1,
     414,    -1,   415,    -1,   416,    -1,   417,    -1,   418,    -1,
     419,    -1,   420,    -1,   421,    -1,   371,  1178,    -1,   381,
     371,  1176,  1179,    -1,   383,   371,  1176,  1179,    -1,  1164,
     357,   371,  1178,    -1,  1164,   357,   381,   371,  1176,  1179,
      -1,  1164,   357,   383,   371,  1176,  1179,    -1,  1166,    -1,
    1165,   357,  1166,    -1,   371,   176,  1178,    -1,   371,   210,
    1178,    -1,   371,   176,   210,  1178,    -1,   371,  1178,    -1,
     381,   371,  1176,  1179,    -1,   383,   371,  1176,  1179,    -1,
    1167,   357,   371,    -1,   371,    -1,   181,  1174,    -1,    -1,
     181,  1170,    -1,   371,  1178,    -1,  1170,   357,   371,  1178,
      -1,   381,   371,  1176,  1179,    -1,   383,   371,  1176,  1179,
      -1,  1170,   357,   381,   371,  1176,  1179,    -1,  1170,   357,
     383,   371,  1176,  1179,    -1,    -1,   181,  1173,    -1,   371,
      -1,   375,    -1,  1172,  1178,    -1,  1173,   357,  1172,  1178,
      -1,   381,  1172,  1176,  1179,    -1,   383,  1172,  1176,  1179,
      -1,  1173,   357,   381,  1172,  1176,  1179,    -1,  1173,   357,
     383,  1172,  1176,  1179,    -1,  1175,    -1,  1174,   357,  1175,
      -1,   371,  1178,    -1,  1177,   371,    -1,    -1,   399,    -1,
      -1,  1179,    -1,  1180,   371,    -1,    -1,   393,    -1,    -1,
     319,  1162,    -1,  1181,   733,  1183,    -1,    -1,  1184,    -1,
     222,   398,    -1,   222,   270,    -1,   222,   375,    -1,    -1,
     351,  1186,   352,    -1,  1186,   357,  1187,    -1,  1187,    -1,
     213,  1162,   734,    -1,    -1,   213,   351,  1162,   352,    -1,
     177,    -1,   176,    -1
};

/* YYRLINE[YYN] -- source line where rule number YYN was defined.  */
static const yytype_uint16 yyrline[] =
{
       0,   584,   584,   585,   589,   590,   591,   592,   606,   607,
     611,   612,   613,   614,   618,   619,   620,   621,   622,   623,
     624,   625,   629,   630,   634,   635,   636,   637,   638,   639,
     643,   644,   645,   646,   647,   648,   652,   653,   657,   658,
     659,   660,   664,   665,   666,   670,   671,   672,   676,   677,
     678,   679,   680,   684,   685,   686,   690,   691,   695,   696,
     700,   701,   705,   706,   710,   711,   715,   716,   720,   721,
     725,   726,   727,   728,   729,   730,   731,   732,   733,   734,
     735,   739,   740,   744,   748,   755,   821,   822,   823,   824,
     828,   973,  1095,  1140,  1141,  1145,  1149,  1157,  1158,  1159,
    1163,  1168,  1183,  1188,  1203,  1208,  1222,  1235,  1239,  1243,
    1249,  1255,  1256,  1257,  1263,  1270,  1276,  1284,  1290,  1296,
    1302,  1308,  1314,  1320,  1326,  1332,  1338,  1344,  1350,  1356,
    1371,  1377,  1383,  1390,  1397,  1402,  1410,  1412,  1419,  1433,
    1456,  1470,  1486,  1511,  1552,  1567,  1609,  1648,  1649,  1653,
    1654,  1658,  1694,  1698,  1809,  1923,  1967,  1968,  1969,  1973,
    1974,  1975,  1979,  1980,  1984,  1985,  1989,  1994,  2002,  2003,
    2004,  2034,  2077,  2078,  2082,  2086,  2096,  2104,  2109,  2114,
    2119,  2127,  2128,  2132,  2133,  2137,  2138,  2142,  2143,  2147,
    2148,  2153,  2222,  2226,  2227,  2231,  2232,  2233,  2237,  2238,
    2239,  2240,  2241,  2242,  2243,  2244,  2245,  2249,  2250,  2251,
    2255,  2256,  2260,  2261,  2262,  2263,  2264,  2265,  2269,  2271,
    2272,  2273,  2274,  2275,  2279,  2280,  2285,  2286,  2293,  2297,
    2298,  2302,  2312,  2313,  2317,  2318,  2322,  2323,  2324,  2328,
    2329,  2330,  2331,  2332,  2333,  2334,  2335,  2336,  2337,  2341,
    2342,  2343,  2344,  2345,  2349,  2350,  2354,  2355,  2359,  2384,
    2385,  2395,  2401,  2405,  2409,  2414,  2419,  2424,  2429,  2434,
    2438,  2466,  2498,  2531,  2569,  2577,  2643,  2719,  2760,  2801,
    2837,  2875,  2894,  2909,  2924,  2942,  2956,  2962,  2971,  2985,
    2994,  3012,  3015,  3035,  3053,  3057,  3062,  3067,  3072,  3076,
    3081,  3086,  3091,  3109,  3122,  3130,  3150,  3154,  3159,  3164,
    3174,  3178,  3189,  3193,  3198,  3205,  3214,  3216,  3224,  3225,
    3226,  3229,  3231,  3235,  3236,  3244,  3255,  3259,  3264,  3275,
    3280,  3285,  3291,  3293,  3294,  3295,  3299,  3303,  3307,  3311,
    3315,  3319,  3323,  3330,  3334,  3338,  3352,  3357,  3369,  3373,
    3384,  3408,  3442,  3477,  3519,  3569,  3618,  3626,  3630,  3644,
    3649,  3654,  3659,  3664,  3669,  3675,  3680,  3685,  3693,  3694,
    3697,  3699,  3714,  3715,  3719,  3749,  3761,  3775,  3792,  3810,
    3814,  3844,  3851,  3861,  3865,  3869,  3873,  3877,  3885,  3892,
    3900,  3928,  3935,  3941,  3947,  3961,  4004,  4009,  4014,  4019,
    4026,  4033,  4051,  4072,  4088,  4094,  4110,  4117,  4124,  4131,
    4138,  4149,  4156,  4163,  4170,  4177,  4184,  4195,  4205,  4207,
    4214,  4222,  4231,  4236,  4243,  4245,  4249,  4250,  4254,  4270,
    4274,  4281,  4302,  4318,  4319,  4323,  4339,  4343,  4347,  4351,
    4355,  4370,  4376,  4380,  4385,  4392,  4393,  4395,  4396,  4398,
    4400,  4401,  4402,  4403,  4405,  4406,  4408,  4409,  4411,  4413,
    4414,  4415,  4416,  4417,  4418,  4419,  4420,  4421,  4422,  4423,
    4424,  4425,  4426,  4427,  4428,  4429,  4430,  4431,  4432,  4433,
    4434,  4435,  4436,  4437,  4438,  4439,  4441,  4442,  4443,  4444,
    4445,  4446,  4447,  4449,  4451,  4452,  4453,  4455,  4456,  4457,
    4458,  4459,  4461,  4462,  4463,  4465,  4466,  4467,  4469,  4470,
    4472,  4473,  4474,  4476,  4478,  4479,  4480,  4482,  4483,  4489,
    4500,  4510,  4520,  4530,  4541,  4552,  4563,  4574,  4585,  4596,
    4607,  4618,  4629,  4640,  4651,  4662,  4673,  4685,  4686,  4687,
    4691,  4699,  4720,  4731,  4762,  4790,  4794,  4802,  4806,  4820,
    4821,  4822,  4823,  4824,  4825,  4826,  4827,  4828,  4829,  4830,
    4831,  4832,  4833,  4834,  4835,  4836,  4837,  4838,  4839,  4840,
    4841,  4842,  4843,  4844,  4847,  4848,  4849,  4850,  4851,  4852,
    4853,  4854,  4855,  4856,  4857,  4858,  4859,  4860,  4861,  4862,
    4863,  4864,  4865,  4866,  4867,  4868,  4869,  4870,  4871,  4872,
    4873,  4874,  4875,  4882,  4883,  4884,  4885,  4886,  4887,  4888,
    4893,  4894,  4898,  4899,  4900,  4901,  4916,  4930,  4944,  4958,
    4960,  4974,  4979,  5006,  5041,  5054,  5067,  5080,  5093,  5106,
    5120,  5137,  5139,  5143,  5144,  5159,  5163,  5167,  5172,  5183,
    5185,  5189,  5201,  5217,  5233,  5248,  5250,  5251,  5266,  5279,
    5295,  5296,  5300,  5301,  5305,  5306,  5310,  5314,  5315,  5319,
    5320,  5321,  5322,  5326,  5327,  5328,  5329,  5330,  5334,  5335,
    5339,  5340,  5344,  5348,  5352,  5356,  5360,  5361,  5365,  5370,
    5374,  5378,  5379,  5383,  5389,  5394,  5402,  5403,  5408,  5420,
    5421,  5422,  5423,  5424,  5425,  5426,  5427,  5429,  5430,  5431,
    5432,  5434,  5435,  5436,  5437,  5438,  5439,  5440,  5441,  5442,
    5443,  5444,  5445,  5446,  5447,  5448,  5449,  5450,  5451,  5452,
    5453,  5454,  5455,  5456,  5457,  5458,  5459,  5460,  5461,  5462,
    5463,  5465,  5466,  5468,  5469,  5470,  5471,  5473,  5474,  5475,
    5476,  5488,  5500,  5513,  5514,  5515,  5516,  5517,  5518,  5519,
    5520,  5521,  5522,  5524,  5525,  5527,  5531,  5532,  5536,  5537,
    5540,  5542,  5546,  5547,  5551,  5556,  5561,  5568,  5570,  5573,
    5575,  5591,  5596,  5605,  5608,  5618,  5621,  5623,  5625,  5638,
    5651,  5664,  5666,  5686,  5689,  5692,  5695,  5696,  5697,  5699,
    5720,  5735,  5739,  5741,  5756,  5757,  5761,  5779,  5780,  5781,
    5785,  5786,  5790,  5805,  5807,  5811,  5812,  5827,  5828,  5832,
    5836,  5838,  5851,  5866,  5868,  5869,  5882,  5896,  5898,  5914,
    5915,  5919,  5921,  5923,  5926,  5928,  5929,  5930,  5934,  5935,
    5936,  5948,  5951,  5953,  5957,  5958,  5962,  5963,  5966,  5968,
    5980,  5999,  6003,  6007,  6010,  6012,  6016,  6020,  6024,  6028,
    6032,  6036,  6037,  6041,  6045,  6049,  6050,  6054,  6055,  6056,
    6059,  6061,  6064,  6066,  6067,  6071,  6072,  6076,  6077,  6081,
    6082,  6083,  6096,  6109,  6123,  6137,  6154,  6167,  6186,  6187,
    6188,  6189,  6192,  6194,  6195,  6199,  6205,  6208,  6211,  6214,
    6217,  6221,  6225,  6226,  6227,  6228,  6243,  6245,  6248,  6249,
    6251,  6254,  6259,  6263,  6268,  6270,  6275,  6279,  6281,  6285,
    6287,  6290,  6292,  6296,  6297,  6298,  6299,  6302,  6304,  6305,
    6311,  6317,  6318,  6319,  6320,  6324,  6326,  6331,  6332,  6336,
    6339,  6341,  6346,  6347,  6351,  6354,  6356,  6361,  6362,  6366,
    6367,  6373,  6374,  6380,  6384,  6385,  6389,  6390,  6391,  6393,
    6399,  6401,  6404,  6406,  6410,  6411,  6415,  6416,  6417,  6418,
    6419,  6423,  6427,  6428,  6429,  6430,  6434,  6437,  6439,  6442,
    6444,  6448,  6452,  6467,  6468,  6469,  6472,  6474,  6493,  6495,
    6507,  6521,  6523,  6527,  6528,  6532,  6533,  6537,  6541,  6543,
    6546,  6548,  6551,  6553,  6557,  6558,  6562,  6565,  6566,  6570,
    6573,  6577,  6580,  6585,  6587,  6588,  6589,  6590,  6591,  6593,
    6597,  6599,  6600,  6605,  6611,  6612,  6617,  6620,  6621,  6624,
    6629,  6630,  6634,  6639,  6640,  6644,  6645,  6662,  6664,  6668,
    6669,  6673,  6676,  6681,  6682,  6686,  6687,  6705,  6706,  6710,
    6711,  6716,  6720,  6724,  6727,  6731,  6735,  6738,  6740,  6744,
    6745,  6749,  6750,  6754,  6758,  6760,  6761,  6762,  6765,  6767,
    6770,  6772,  6776,  6777,  6781,  6782,  6783,  6784,  6788,  6792,
    6793,  6797,  6799,  6802,  6804,  6808,  6809,  6810,  6812,  6814,
    6818,  6820,  6824,  6827,  6829,  6833,  6839,  6840,  6844,  6845,
    6849,  6850,  6854,  6858,  6860,  6861,  6862,  6863,  6868,  6881,
    6885,  6888,  6890,  6895,  6905,  6916,  6923,  6929,  6931,  6944,
    6958,  6960,  6964,  6965,  6969,  6972,  6974,  6979,  6984,  6989,
    6990,  6991,  6992,  6997,  6998,  7003,  7004,  7020,  7025,  7026,
    7038,  7063,  7064,  7069,  7073,  7079,  7090,  7105,  7109,  7110,
    7118,  7119,  7123,  7124,  7128,  7129,  7130,  7131,  7135,  7136,
    7138,  7140,  7142,  7145,  7148,  7153,  7154,  7158,  7164,  7165,
    7169,  7170,  7174,  7175,  7178,  7182,  7183,  7187,  7188,  7192,
    7196,  7203,  7215,  7219,  7223,  7230,  7234,  7238,  7239,  7242,
    7244,  7245,  7249,  7250,  7254,  7255,  7256,  7261,  7262,  7266,
    7267,  7271,  7272,  7279,  7287,  7288,  7292,  7296,  7297,  7298,
    7302,  7304,  7309,  7311,  7315,  7316,  7319,  7321,  7328,  7334,
    7336,  7340,  7341,  7345,  7346,  7354,  7355,  7359,  7363,  7367,
    7368,  7372,  7376,  7380,  7381,  7382,  7383,  7388,  7389,  7393,
    7394,  7397,  7399,  7403,  7407,  7408,  7412,  7417,  7418,  7422,
    7423,  7428,  7435,  7445,  7455,  7456,  7460,  7464,  7469,  7470,
    7474,  7478,  7479,  7483,  7487,  7488,  7492,  7501,  7503,  7506,
    7508,  7511,  7513,  7514,  7518,  7519,  7522,  7524,  7527,  7529,
    7530,  7537,  7538,  7542,  7543,  7544,  7547,  7549,  7550,  7551,
    7552,  7556,  7560,  7561,  7565,  7566,  7567,  7568,  7569,  7570,
    7571,  7572,  7573,  7577,  7578,  7579,  7583,  7584,  7585,  7589,
    7590,  7594,  7598,  7599,  7600,  7601,  7602,  7605,  7607,  7608,
    7612,  7616,  7617,  7621,  7622,  7626,  7627,  7631,  7635,  7636,
    7640,  7644,  7647,  7649,  7653,  7654,  7658,  7659,  7663,  7667,
    7671,  7672,  7676,  7677,  7681,  7682,  7686,  7687,  7691,  7695,
    7696,  7700,  7701,  7705,  7709,  7713,  7714,  7718,  7719,  7720,
    7724,  7725,  7726,  7730,  7731,  7732,  7736,  7737,  7738,  7739,
    7740,  7741,  7742,  7746,  7751,  7752,  7756,  7757,  7762,  7766,
    7768,  7769,  7770,  7771,  7774,  7776,  7780,  7782,  7787,  7788,
    7792,  7796,  7801,  7802,  7803,  7804,  7808,  7812,  7813,  7817,
    7818,  7822,  7823,  7824,  7827,  7829,  7832,  7834,  7837,  7839,
    7842,  7848,  7851,  7853,  7857,  7858,  7861,  7863,  7877,  7879,
    7880,  7896,  7898,  7902,  7906,  7907,  7923,  7924,  7928,  7930,
    7931,  7935,  7938,  7940,  7943,  7945,  7959,  7963,  7965,  7966,
    7967,  7968,  7969,  7970,  7974,  7975,  7979,  7982,  7984,  7985,
    7988,  7990,  7991,  7998,  7999,  8003,  8018,  8031,  8045,  8063,
    8065,  8085,  8087,  8091,  8095,  8096,  8100,  8101,  8105,  8106,
    8110,  8112,  8114,  8116,  8118,  8120,  8122,  8124,  8126,  8128,
    8130,  8132,  8134,  8135,  8136,  8137,  8138,  8139,  8140,  8141,
    8142,  8143,  8144,  8145,  8146,  8147,  8148,  8149,  8150,  8163,
    8164,  8168,  8172,  8173,  8177,  8181,  8185,  8189,  8193,  8197,
    8198,  8199,  8203,  8207,  8211,  8215,  8219,  8220,  8221,  8225,
    8226,  8227,  8228,  8232,  8233,  8237,  8238,  8242,  8243,  8247,
    8248,  8252,  8253,  8257,  8258,  8259,  8260,  8261,  8262,  8263,
    8267,  8271,  8272,  8276,  8277,  8278,  8282,  8283,  8284,  8288,
    8289,  8290,  8291,  8295,  8296,  8297,  8298,  8299,  8300,  8301,
    8302,  8303,  8304,  8305,  8306,  8308,  8309,  8310,  8311,  8312,
    8313,  8314,  8315,  8316,  8317,  8318,  8323,  8324,  8325,  8328,
    8330,  8341,  8353,  8360,  8361,  8365,  8371,  8375,  8377,  8382,
    8383,  8387,  8395,  8400,  8402,  8405,  8407,  8411,  8415,  8419,
    8422,  8424,  8426,  8428,  8430,  8432,  8436,  8438,  8446,  8447,
    8451,  8452,  8453,  8457,  8459,  8467,  8469,  8474,  8475,  8480,
    8483,  8485,  8488,  8490,  8491,  8495,  8496,  8497,  8498,  8502,
    8503,  8504,  8505,  8509,  8510,  8514,  8515,  8519,  8523,  8524,
    8528,  8531,  8533,  8543,  8544,  8551,  8562,  8572,  8583,  8584,
    8588,  8589,  8593,  8594,  8598,  8599,  8605,  8607,  8608,  8614,
    8615,  8619,  8625,  8627,  8628,  8629,  8635,  8637,  8640,  8642,
    8643,  8650,  8654,  8659,  8666,  8667,  8668,  8675,  8676,  8677,
    8684,  8685,  8689,  8693,  8699,  8705,  8716,  8717,  8718,  8723,
    8727,  8734,  8737,  8742,  8745,  8750,  8757,  8758,  8759,  8762,
    8773,  8774,  8776,  8778,  8786,  8794,  8803,  8813,  8820,  8822,
    8826,  8827,  8831,  8832,  8833,  8834,  8841,  8842,  8849,  8862,
    8869,  8876,  8878,  8885,  8903,  8912,  8913,  8914,  8915,  8919,
    8923,  8924,  8928,  8934,  8936,  8945,  8948,  8950,  8954,  8955,
    8959,  8962,  8968,  8969,  8973,  8974,  8981,  8987,  8996,  8997,
    8998,  8999,  9000,  9001,  9002,  9003,  9004,  9005,  9006,  9007,
    9008,  9009,  9010,  9011,  9015,  9023,  9024,  9025,  9026,  9027,
    9028,  9029,  9030,  9031,  9032,  9033,  9034,  9035,  9036,  9037,
    9038,  9051,  9058,  9063,  9074,  9079,  9092,  9099,  9101,  9102,
    9106,  9110,  9117,  9125,  9129,  9137,  9138,  9142,  9148,  9149,
    9153,  9159,  9162,  9164,  9171,  9172,  9173,  9174,  9178,  9188,
    9199,  9206,  9216,  9229,  9231,  9238,  9251,  9252,  9260,  9266,
    9268,  9272,  9276,  9280,  9281,  9289,  9290,  9295,  9296,  9300,
    9307,  9312,  9317,  9321,  9326,  9333,  9338,  9340,  9345,  9348,
    9350,  9354,  9362,  9363,  9367,  9368,  9370,  9374,  9378,  9379,
    9380,  9384,  9401,  9410,  9411,  9415,  9416,  9420,  9437,  9456,
    9457,  9458,  9459,  9460,  9461,  9462,  9475,  9488,  9490,  9503,
    9516,  9518,  9531,  9546,  9548,  9551,  9553,  9557,  9576,  9608,
    9621,  9637,  9638,  9651,  9667,  9671,  9686,  9705,  9739,  9757,
    9760,  9776,  9792,  9796,  9797,  9801,  9817,  9833,  9849,  9876,
    9881,  9882,  9886,  9888,  9902,  9927,  9941,  9966,  9968,  9972,
    9986, 10000, 10015, 10032, 10046, 10064, 10085, 10100, 10117, 10119,
   10120, 10124, 10125, 10128, 10130, 10134, 10138, 10142, 10143, 10163,
   10164, 10184, 10185, 10189, 10190, 10194, 10195, 10199, 10202, 10204,
   10219, 10241, 10251, 10252, 10256, 10260, 10264, 10268, 10269, 10270,
   10271, 10272, 10273, 10277, 10278, 10281, 10283, 10287, 10289, 10294,
   10295, 10299, 10306, 10307, 10310, 10312, 10313, 10316, 10318, 10322,
   10325, 10327, 10328, 10331, 10333, 10337, 10342, 10343, 10344, 10348,
   10354, 10355, 10359, 10363, 10371, 10373, 10433, 10448, 10483, 10545,
   10586, 10639, 10653, 10657, 10662, 10697, 10712, 10750, 10755, 10777,
   10779, 10782, 10784, 10788, 10792, 10794, 10799, 10800, 10804, 10806,
   10811, 10815, 10828, 10841, 10854, 10866, 10879, 10895, 10910, 10911,
   10912, 10916, 10917, 10918, 10919, 10920, 10921, 10922, 10923, 10924,
   10925, 10933, 10951, 10962, 10973, 10984, 10995, 11009, 11010, 11014,
   11025, 11036, 11047, 11058, 11069, 11083, 11121, 11150, 11153, 11155,
   11162, 11173, 11184, 11195, 11206, 11217, 11230, 11232, 11239, 11240,
   11244, 11255, 11266, 11277, 11288, 11299, 11314, 11315, 11319, 11334,
   11360, 11362, 11368, 11370, 11374, 11472, 11474, 11480, 11482, 11486,
   11490, 11492, 11496, 11497, 11498, 11514, 11516, 11522, 11523, 11527,
   11530, 11532, 11537, 11538
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
  "C_SQL_TIME_STRUCT", "C_SQL_DATE_STRUCT", "C_SQL_NUMERIC_STRUCT",
  "C_SQL_DA_STRUCT", "C_FAILOVERCB", "C_NCHAR_CS", "C_ATTRIBUTE",
  "M_INCLUDE", "M_DEFINE", "M_UNDEF", "M_FUNCTION", "M_LBRAC", "M_RBRAC",
  "M_DQUOTE", "M_FILENAME", "M_IF", "M_ELIF", "M_ELSE", "M_ENDIF",
  "M_IFDEF", "M_IFNDEF", "M_CONSTANT", "M_IDENTIFIER", "EM_SQLSTART",
  "EM_ERROR", "TR_ADD", "TR_AFTER", "TR_AGER", "TR_ALL", "TR_ALTER",
  "TR_AND", "TR_ANY", "TR_ARCHIVE", "TR_ARCHIVELOG", "TR_AS", "TR_ASC",
  "TR_AT", "TR_BACKUP", "TR_BEFORE", "TR_BEGIN", "TR_BY", "TR_BIND",
  "TR_CASCADE", "TR_CASE", "TR_CAST", "TR_CHECK_OPENING_PARENTHESIS",
  "TR_CLOSE", "TR_COALESCE", "TR_COLUMN", "TR_COMMENT", "TR_COMMIT",
  "TR_COMPILE", "TR_CONNECT", "TR_CONSTRAINT", "TR_CONSTRAINTS",
  "TR_CONTINUE", "TR_CREATE", "TR_VOLATILE", "TR_CURSOR", "TR_CYCLE",
  "TR_DATABASE", "TR_DECLARE", "TR_DEFAULT", "TR_DELETE", "TR_DEQUEUE",
  "TR_DESC", "TR_DIRECTORY", "TR_DISABLE", "TR_DISCONNECT", "TR_DISTINCT",
  "TR_DROP", "TR_DESCRIBE", "TR_DESCRIPTOR", "TR_EACH", "TR_ELSE",
  "TR_ELSEIF", "TR_ENABLE", "TR_END", "TR_ENQUEUE", "TR_ESCAPE",
  "TR_EXCEPTION", "TR_EXEC", "TR_EXECUTE", "TR_EXIT", "TR_FAILOVERCB",
  "TR_FALSE", "TR_FETCH", "TR_FIFO", "TR_FLUSH", "TR_FOR", "TR_FOREIGN",
  "TR_FROM", "TR_FULL", "TR_FUNCTION", "TR_GOTO", "TR_GRANT", "TR_GROUP",
  "TR_HAVING", "TR_IF", "TR_IN", "TR_IN_BF_LPAREN", "TR_INNER",
  "TR_INSERT", "TR_INTERSECT", "TR_INTO", "TR_IS", "TR_ISOLATION",
  "TR_JOIN", "TR_KEY", "TR_LEFT", "TR_LESS", "TR_LEVEL", "TR_LIFO",
  "TR_LIKE", "TR_LIMIT", "TR_LOCAL", "TR_LOGANCHOR", "TR_LOOP", "TR_MERGE",
  "TR_MOVE", "TR_MOVEMENT", "TR_NEW", "TR_NOARCHIVELOG", "TR_NOCYCLE",
  "TR_NOT", "TR_NULL", "TR_OF", "TR_OFF", "TR_OLD", "TR_ON", "TR_OPEN",
  "TR_OR", "TR_ORDER", "TR_OUT", "TR_OUTER", "TR_OVER", "TR_PARTITION",
  "TR_PARTITIONS", "TR_POINTER", "TR_PRIMARY", "TR_PRIOR", "TR_PRIVILEGES",
  "TR_PROCEDURE", "TR_PUBLIC", "TR_QUEUE", "TR_READ", "TR_REBUILD",
  "TR_RECOVER", "TR_REFERENCES", "TR_REFERENCING", "TR_REGISTER",
  "TR_RESTRICT", "TR_RETURN", "TR_REVOKE", "TR_RIGHT", "TR_ROLLBACK",
  "TR_ROW", "TR_SAVEPOINT", "TR_SELECT", "TR_SEQUENCE", "TR_SESSION",
  "TR_SET", "TR_SOME", "TR_SPLIT", "TR_START", "TR_STATEMENT",
  "TR_SYNONYM", "TR_TABLE", "TR_TEMPORARY", "TR_THAN", "TR_THEN", "TR_TO",
  "TR_TRIGGER", "TR_TRUE", "TR_TYPE", "TR_TYPESET", "TR_UNION",
  "TR_UNIQUE", "TR_UNREGISTER", "TR_UNTIL", "TR_UPDATE", "TR_USER",
  "TR_USING", "TR_VALUES", "TR_VARIABLE", "TR_VARIABLE_LARGE",
  "TR_VARIABLES", "TR_VIEW", "TR_WHEN", "TR_WHERE", "TR_WHILE", "TR_WITH",
  "TR_WORK", "TR_WRITE", "TR_PARALLEL", "TR_NOPARALLEL", "TR_LOB",
  "TR_STORE", "TR_ENDEXEC", "TR_PRECEDING", "TR_FOLLOWING",
  "TR_CURRENT_ROW", "TR_LINK", "TR_ROLE", "TR_WITHIN", "TR_LOGGING",
  "TK_BETWEEN", "TK_EXISTS", "TO_ACCESS", "TO_CONSTANT", "TO_IDENTIFIED",
  "TO_INDEX", "TO_MINUS", "TO_MODE", "TO_OTHERS", "TO_RAISE", "TO_RENAME",
  "TO_REPLACE", "TO_ROWTYPE", "TO_SEGMENT", "TO_WAIT", "TO_PIVOT",
  "TO_UNPIVOT", "TO_MATERIALIZED", "TO_CONNECT_BY_NOCYCLE",
  "TO_CONNECT_BY_ROOT", "TO_NULLS", "TO_PURGE", "TO_FLASHBACK",
  "TO_VC2COLL", "TO_KEEP", "TA_ELSIF", "TA_EXTENTSIZE", "TA_FIXED",
  "TA_LOCK", "TA_MAXROWS", "TA_ONLINE", "TA_OFFLINE", "TA_REPLICATION",
  "TA_REVERSE", "TA_ROWCOUNT", "TA_STEP", "TA_TABLESPACE", "TA_TRUNCATE",
  "TA_SQLCODE", "TA_SQLERRM", "TA_LINKER", "TA_REMOTE_TABLE", "TA_SHARD",
  "TA_NODE_META", "TA_NODE_DATA", "TA_DISJOIN", "TA_CONJOIN", "TA_SEC",
  "TA_MSEC", "TA_USEC", "TA_SECOND", "TA_MILLISECOND", "TA_MICROSECOND",
  "TA_ANALYSIS_PROPAGATION", "TI_NONQUOTED_IDENTIFIER",
  "TI_QUOTED_IDENTIFIER", "TI_HOSTVARIABLE", "TL_TYPED_LITERAL",
  "TL_LITERAL", "TL_NCHAR_LITERAL", "TL_UNICODE_LITERAL", "TL_INTEGER",
  "TL_NUMERIC", "TS_AT_SIGN", "TS_CONCATENATION_SIGN", "TS_DOUBLE_PERIOD",
  "TS_EXCLAMATION_POINT", "TS_PERCENT_SIGN", "TS_OPENING_PARENTHESIS",
  "TS_CLOSING_PARENTHESIS", "TS_OPENING_BRACKET", "TS_CLOSING_BRACKET",
  "TS_ASTERISK", "TS_PLUS_SIGN", "TS_COMMA", "TS_MINUS_SIGN", "TS_PERIOD",
  "TS_SLASH", "TS_COLON", "TS_SEMICOLON", "TS_LESS_THAN_SIGN",
  "TS_EQUAL_SIGN", "TS_GREATER_THAN_SIGN", "TS_QUESTION_MARK",
  "TS_OUTER_JOIN_OPERATOR", "TX_HINTS", "SES_V_NUMERIC", "SES_V_INTEGER",
  "SES_V_HOSTVARIABLE", "SES_V_LITERAL", "SES_V_TYPED_LITERAL",
  "SES_V_DQUOTE_LITERAL", "SES_V_IDENTIFIER", "SES_V_ABSOLUTE",
  "SES_V_ALLOCATE", "SES_V_ASENSITIVE", "SES_V_AUTOCOMMIT", "SES_V_BATCH",
  "SES_V_BLOB_FILE", "SES_V_BREAK", "SES_V_CLOB_FILE", "SES_V_CUBE",
  "SES_V_DEALLOCATE", "SES_V_DESCRIPTOR", "SES_V_DO", "SES_V_FIRST",
  "SES_V_FOUND", "SES_V_FREE", "SES_V_HOLD", "SES_V_IMMEDIATE",
  "SES_V_INDICATOR", "SES_V_INSENSITIVE", "SES_V_LAST", "SES_V_NEXT",
  "SES_V_ONERR", "SES_V_ONLY", "APRE_V_OPTION", "SES_V_PREPARE",
  "SES_V_RELATIVE", "SES_V_RELEASE", "SES_V_ROLLUP", "SES_V_SCROLL",
  "SES_V_SENSITIVE", "SES_V_SQLERROR", "SES_V_THREADS", "SES_V_WHENEVER",
  "SES_V_CURRENT", "SES_V_GROUPING_SETS", "SES_V_WITH_ROLLUP", "SES_V_GET",
  "SES_V_DIAGNOSTICS", "SES_V_CONDITION", "SES_V_NUMBER",
  "SES_V_ROW_COUNT", "SES_V_RETURNED_SQLCODE", "SES_V_RETURNED_SQLSTATE",
  "SES_V_MESSAGE_TEXT", "SES_V_ROW_NUMBER", "SES_V_COLUMN_NUMBER", "'('",
  "')'", "'['", "']'", "'.'", "','", "'&'", "'*'", "'+'", "'-'", "'~'",
  "'!'", "'/'", "'%'", "'<'", "'>'", "'^'", "'|'", "'?'", "':'", "'='",
  "';'", "'}'", "'{'", "$accept", "program", "combined_grammar",
  "C_grammar", "primary_expr", "postfix_expr", "argument_expr_list",
  "unary_expr", "unary_operator", "cast_expr", "multiplicative_expr",
  "additive_expr", "shift_expr", "relational_expr", "equality_expr",
  "and_expr", "exclusive_or_expr", "inclusive_or_expr", "logical_and_expr",
  "logical_or_expr", "conditional_expr", "assignment_expr",
  "assignment_operator", "expr", "constant_expr", "declaration",
  "declaration_specifiers", "init_declarator_list", "var_decl_list_begin",
  "init_declarator", "storage_class_specifier", "type_specifier",
  "attribute_specifier", "struct_or_union_specifier", "struct_decl_begin",
  "no_tag_struct_decl_begin", "struct_or_union",
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
  "select_list_stmt", "proc_stmt", "etc_stmt", "option_stmt",
  "exception_stmt", "threads_stmt", "sharedptr_stmt", "getdiag_stmt",
  "current_opt", "statement_information_list",
  "statement_information_item", "statement_information_item_name",
  "condition_number", "condition_information_list",
  "condition_information_item", "condition_information_item_name",
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
  "sequence_option", "opt_fixed", "alter_table_statement",
  "opt_using_prefix", "using_prefix", "opt_rename_force",
  "opt_ignore_foreign_key_child", "opt_partition",
  "alter_table_partitioning", "opt_lob_storage", "lob_storage_list",
  "lob_storage_element", "opt_index_storage", "index_storage_list",
  "index_storage_element", "opt_index_part_attr_list",
  "index_part_attr_list", "index_part_attr", "add_partition_spec",
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
  "dml_table_reference", "name_list", "delete_statement",
  "insert_statement", "multi_insert_value_list", "multi_insert_value",
  "insert_atom_commalist", "insert_atom", "multi_rows_list", "one_row",
  "dml_table_commalist", "dml_table", "dml_joined_table",
  "update_statement", "enqueue_statement", "dequeue_statement",
  "dequeue_query_term", "dequeue_query_spec", "dequeue_from_clause",
  "dequeue_from_table_reference_commalist", "dequeue_from_table_reference",
  "opt_fifo", "assignment_commalist", "assignment", "set_column_def",
  "assignment_column_comma_list", "assignment_column", "merge_statement",
  "merge_actions_list", "merge_actions", "when_condition", "then_action",
  "opt_delete_where_clause", "table_column_commalist",
  "opt_table_column_commalist", "move_statement",
  "opt_move_expression_commalist", "move_expression_commalist",
  "move_expression", "SP_select_or_with_select_statement",
  "SP_select_statement", "SP_with_select_statement",
  "select_or_with_select_statement", "select_statement",
  "with_select_statement", "set_op", "SP_query_exp", "query_exp",
  "opt_subquery_factoring_clause", "subquery_factoring_clause",
  "subquery_factoring_clause_list", "subquery_factoring_element",
  "SP_query_term", "query_term", "SP_query_spec", "query_spec",
  "select_or_with_select_statement_4emsql", "select_statement_4emsql",
  "with_select_statement_4emsql", "query_exp_4emsql",
  "subquery_factoring_clause_4emsql",
  "subquery_factoring_clause_list_4emsql",
  "subquery_factoring_element_4emsql", "query_term_4emsql",
  "query_spec_4emsql", "opt_hints", "opt_groupby_clause", "opt_quantifier",
  "target_list", "opt_into_list", "opt_into_list_host_var",
  "select_sublist_commalist", "select_sublist", "opt_as_name",
  "from_clause", "sel_from_table_reference_commalist",
  "sel_from_table_reference", "shard_stmt_spec", "node_data_spec",
  "shard_node_commalist", "shard_node_element", "table_func_argument",
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
  "SP_drop_typeset_statement", "exec_func_stmt",
  "SP_anonymous_block_statement", "SP_anoymous_block_declare_block",
  "SP_anonymous_block_first_block", "SP_exec_or_execute",
  "SP_ident_opt_simple_arglist", "assign_return_value", "set_statement",
  "initsize_spec", "create_database_statement", "archivelog_option",
  "character_set_option", "db_character_set", "national_character_set",
  "alter_database_statement", "until_option", "usinganchor_option",
  "filespec_option", "alter_database_options", "alter_database_options2",
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
  "allowed_keywords_for_object_name", "in_host_var_list", "host_var_list",
  "host_variable", "free_lob_loc_list", "SP_into_host_var",
  "opt_into_host_var", "out_host_var_list", "opt_into_ses_host_var_4emsql",
  "out_1hostvariable_4emsql", "out_host_var_list_4emsql",
  "out_psm_host_var_list", "out_psm_host_var", "file_option",
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
     655,   656,   657,   658,   659,   660,   661,   662,   663,   664,
     665,   666,   667,   668,   669,   670,   671,   672,   673,   674,
     675,   676,    40,    41,    91,    93,    46,    44,    38,    42,
      43,    45,   126,    33,    47,    37,    60,    62,    94,   124,
      63,    58,    61,    59,   125,   123
};
# endif

/* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_uint16 yyr1[] =
{
       0,   446,   447,   447,   448,   448,   448,   448,   449,   449,
     450,   450,   450,   450,   451,   451,   451,   451,   451,   451,
     451,   451,   452,   452,   453,   453,   453,   453,   453,   453,
     454,   454,   454,   454,   454,   454,   455,   455,   456,   456,
     456,   456,   457,   457,   457,   458,   458,   458,   459,   459,
     459,   459,   459,   460,   460,   460,   461,   461,   462,   462,
     463,   463,   464,   464,   465,   465,   466,   466,   467,   467,
     468,   468,   468,   468,   468,   468,   468,   468,   468,   468,
     468,   469,   469,   470,   471,   471,   472,   472,   472,   472,
     473,   473,   474,   475,   475,   476,   476,   476,   476,   476,
     477,   477,   477,   477,   477,   477,   477,   477,   477,   477,
     477,   477,   477,   477,   477,   477,   477,   477,   477,   477,
     477,   477,   477,   477,   477,   477,   477,   477,   477,   477,
     477,   477,   477,   477,   477,   477,   478,   478,   479,   479,
     479,   479,   479,   480,   481,   482,   482,   483,   483,   484,
     484,   485,   486,   486,   486,   487,   488,   488,   488,   489,
     489,   489,   490,   490,   491,   491,   492,   492,   493,   493,
     493,   493,   493,   493,   493,   494,   495,   496,   496,   496,
     496,   497,   497,   498,   498,   499,   499,   500,   500,   501,
     501,   502,   502,   503,   503,   504,   504,   504,   505,   505,
     505,   505,   505,   505,   505,   505,   505,   506,   506,   506,
     507,   507,   508,   508,   508,   508,   508,   508,   509,   509,
     509,   509,   509,   509,   510,   510,   511,   511,   511,   512,
     512,   513,   514,   514,   515,   515,   516,   516,   516,   517,
     517,   517,   517,   517,   517,   517,   517,   517,   517,   518,
     518,   518,   518,   518,   519,   519,   520,   520,   521,   522,
     522,   523,   523,   523,   523,   523,   523,   523,   523,   523,
     524,   524,   525,   525,   526,   527,   528,   529,   530,   531,
     532,   533,   533,   533,   533,   533,   533,   533,   533,   533,
     533,   534,   534,   534,   535,   535,   535,   535,   535,   535,
     535,   535,   535,   536,   536,   537,   538,   538,   538,   538,
     539,   539,   540,   540,   540,   540,   541,   541,   541,   541,
     541,   542,   542,   543,   543,   544,   545,   545,   545,   546,
     546,   546,   547,   547,   547,   547,   548,   548,   548,   548,
     548,   548,   548,   549,   549,   549,   550,   550,   551,   551,
     552,   552,   552,   552,   552,   552,   553,   554,   554,   555,
     555,   555,   555,   555,   555,   555,   555,   555,   556,   556,
     557,   557,   558,   558,   559,   559,   559,   560,   561,   562,
     563,   563,   563,   564,   565,   565,   565,   565,   566,   567,
     568,   569,   569,   569,   569,   569,   570,   570,   570,   570,
     570,   570,   570,   570,   571,   571,   572,   572,   572,   572,
     572,   572,   572,   572,   572,   572,   572,   572,   573,   573,
     574,   574,   575,   575,   576,   576,   577,   577,   578,   579,
     579,   580,   580,   581,   581,   582,   583,   583,   583,   583,
     583,   584,   584,   584,   584,   585,   585,   585,   585,   585,
     585,   585,   585,   585,   585,   585,   585,   585,   585,   585,
     585,   585,   585,   585,   585,   585,   585,   585,   585,   585,
     585,   585,   585,   585,   585,   585,   585,   585,   585,   585,
     585,   585,   585,   585,   585,   585,   585,   585,   585,   585,
     585,   585,   585,   585,   585,   585,   585,   585,   585,   585,
     585,   585,   585,   585,   585,   585,   585,   585,   585,   585,
     585,   585,   585,   585,   585,   585,   585,   585,   585,   586,
     586,   586,   586,   586,   586,   586,   586,   586,   586,   586,
     586,   586,   586,   586,   586,   586,   586,   587,   587,   587,
     588,   589,   589,   589,   589,   590,   590,   591,   591,   592,
     592,   592,   592,   592,   592,   592,   592,   592,   592,   592,
     592,   592,   592,   592,   592,   592,   592,   592,   592,   592,
     592,   592,   592,   592,   592,   592,   592,   592,   592,   592,
     592,   592,   592,   592,   592,   592,   592,   592,   592,   592,
     592,   592,   592,   592,   592,   592,   592,   592,   592,   592,
     592,   592,   592,   593,   593,   593,   593,   593,   593,   593,
     594,   594,   595,   595,   595,   595,   595,   595,   595,   595,
     595,   595,   596,   596,   596,   596,   596,   596,   596,   596,
     596,   597,   597,   598,   598,   599,   600,   601,   601,   602,
     602,   603,   603,   604,   605,   606,   606,   606,   606,   606,
     607,   607,   608,   608,   609,   609,   610,   611,   611,   612,
     612,   612,   612,   613,   613,   613,   613,   613,   614,   614,
     615,   615,   616,   617,   618,   619,   620,   620,   621,   622,
     623,   624,   624,   625,   626,   626,   627,   627,   628,   628,
     628,   628,   628,   628,   628,   628,   628,   628,   628,   628,
     628,   628,   628,   628,   628,   628,   628,   628,   628,   628,
     628,   628,   628,   628,   628,   628,   628,   628,   628,   628,
     628,   628,   628,   628,   628,   628,   628,   628,   628,   628,
     628,   628,   628,   628,   628,   628,   628,   628,   628,   628,
     628,   628,   628,   628,   628,   628,   628,   628,   628,   628,
     628,   628,   628,   628,   628,   628,   629,   629,   630,   630,
     631,   631,   632,   632,   633,   634,   634,   635,   635,   636,
     636,   637,   637,   638,   638,   639,   639,   639,   639,   639,
     639,   639,   639,   639,   639,   639,   639,   639,   639,   639,
     639,   639,   640,   640,   641,   641,   642,   642,   642,   642,
     643,   643,   644,   645,   645,   646,   646,   647,   647,   648,
     649,   649,   649,   650,   650,   650,   650,   651,   651,   652,
     652,   653,   653,   653,   654,   654,   654,   654,   655,   655,
     655,   655,   656,   656,   657,   657,   658,   658,   659,   659,
     659,   660,   661,   662,   663,   663,   664,   665,   666,   667,
     668,   669,   669,   670,   671,   672,   672,   673,   673,   673,
     674,   674,   675,   675,   675,   676,   676,   677,   677,   678,
     678,   678,   678,   678,   678,   678,   678,   678,   678,   678,
     678,   678,   679,   679,   679,   680,   680,   680,   680,   680,
     680,   680,   680,   680,   680,   680,   680,   680,   680,   680,
     680,   680,   680,   680,   681,   681,   682,   683,   683,   684,
     684,   685,   685,   686,   686,   686,   686,   686,   686,   686,
     686,   686,   686,   686,   686,   687,   687,   688,   688,   689,
     690,   690,   691,   691,   692,   693,   693,   694,   694,   695,
     695,   696,   696,   697,   698,   698,   699,   699,   699,   699,
     700,   700,   701,   701,   702,   702,   703,   703,   703,   703,
     703,   704,   705,   705,   705,   705,   706,   707,   707,   708,
     708,   709,   710,   711,   711,   711,   712,   712,   713,   713,
     713,   714,   714,   715,   715,   716,   716,   717,   718,   718,
     719,   719,   720,   720,   721,   721,   722,   722,   722,   723,
     723,   723,   723,   724,   724,   724,   724,   724,   724,   724,
     725,   725,   725,   726,   727,   727,   728,   728,   728,   728,
     729,   729,   730,   731,   731,   732,   732,   733,   733,   734,
     734,   735,   735,   736,   736,   737,   737,   738,   738,   739,
     739,   740,   740,   740,   740,   740,   740,   741,   741,   742,
     742,   743,   743,   744,   745,   745,   745,   745,   746,   746,
     747,   747,   748,   748,   749,   749,   749,   749,   749,   749,
     749,   750,   750,   751,   751,   752,   752,   752,   752,   752,
     753,   753,   754,   755,   755,   756,   757,   757,   758,   758,
     759,   759,   760,   761,   761,   761,   761,   761,   762,   762,
     763,   764,   764,   765,   765,   766,   767,   768,   768,   768,
     769,   769,   770,   770,   771,   772,   772,   773,   774,   775,
     775,   775,   775,   776,   776,   777,   777,   778,   779,   779,
     779,   780,   780,   781,   782,   783,   783,   784,   785,   785,
     786,   786,   787,   787,   788,   788,   788,   788,   789,   789,
     789,   789,   789,   789,   789,   790,   790,   791,   792,   792,
     793,   793,   794,   794,   795,   796,   796,   797,   797,   798,
     799,   800,   801,   802,   803,   804,   805,   806,   806,   807,
     807,   807,   808,   808,   809,   809,   809,   810,   810,   811,
     811,   812,   812,   813,   814,   814,   815,   816,   816,   816,
     817,   817,   818,   818,   819,   819,   820,   820,   821,   822,
     822,   823,   823,   824,   824,   825,   825,   826,   827,   828,
     828,   829,   830,   831,   831,   831,   831,   832,   832,   833,
     833,   834,   834,   835,   836,   836,   837,   838,   838,   839,
     839,   840,   840,   841,   842,   842,   843,   844,   845,   845,
     846,   847,   847,   848,   849,   849,   850,   851,   851,   852,
     852,   853,   853,   853,   854,   854,   855,   855,   856,   856,
     856,   857,   857,   858,   858,   858,   859,   859,   859,   859,
     859,   860,   861,   861,   862,   862,   862,   862,   862,   862,
     862,   862,   862,   863,   863,   863,   864,   864,   864,   865,
     865,   866,   867,   867,   867,   867,   867,   868,   868,   868,
     869,   870,   870,   871,   871,   872,   872,   873,   874,   874,
     875,   876,   877,   877,   878,   878,   879,   879,   880,   881,
     882,   882,   883,   883,   884,   884,   885,   885,   886,   887,
     887,   888,   888,   889,   890,   891,   891,   892,   892,   892,
     893,   893,   893,   894,   894,   894,   895,   895,   895,   895,
     895,   895,   895,   896,   897,   897,   898,   898,   899,   900,
     900,   900,   900,   900,   901,   901,   902,   902,   903,   903,
     904,   905,   906,   906,   906,   906,   907,   908,   908,   909,
     909,   910,   910,   910,   911,   911,   912,   912,   913,   913,
     913,   914,   915,   915,   916,   916,   917,   917,   918,   918,
     918,   919,   919,   920,   921,   921,   921,   921,   922,   922,
     922,   923,   924,   924,   925,   925,   925,   926,   926,   926,
     926,   926,   926,   926,   927,   927,   928,   929,   929,   929,
     930,   930,   930,   931,   931,   932,   932,   932,   932,   933,
     933,   934,   934,   935,   936,   936,   937,   937,   938,   938,
     939,   939,   939,   939,   939,   939,   939,   939,   939,   939,
     939,   939,   939,   939,   939,   939,   939,   939,   939,   939,
     939,   939,   939,   939,   939,   939,   939,   939,   939,   939,
     939,   940,   941,   941,   942,   943,   944,   945,   946,   947,
     947,   947,   948,   949,   950,   951,   952,   952,   952,   953,
     953,   953,   953,   954,   954,   955,   955,   956,   956,   957,
     957,   958,   958,   959,   959,   959,   959,   959,   959,   959,
     960,   961,   961,   962,   962,   962,   963,   963,   963,   964,
     964,   964,   964,   965,   965,   965,   965,   965,   965,   965,
     965,   965,   965,   965,   965,   965,   965,   965,   965,   965,
     965,   965,   965,   965,   965,   965,   966,   966,   966,   967,
     967,   968,   968,   969,   969,   970,   971,   972,   972,   973,
     973,   974,   975,   975,   975,   975,   975,   975,   975,   975,
     975,   975,   975,   975,   975,   975,   976,   976,   977,   977,
     978,   978,   978,   979,   979,   980,   980,   981,   981,   982,
     983,   983,   984,   984,   984,   985,   985,   985,   985,   986,
     986,   986,   986,   987,   987,   988,   988,   989,   990,   990,
     991,   992,   992,   993,   993,   994,   995,   996,   997,   997,
     998,   998,   999,   999,  1000,  1000,  1001,  1001,  1001,  1002,
    1002,  1003,  1004,  1004,  1004,  1004,  1005,  1005,  1006,  1006,
    1006,  1007,  1008,  1009,  1010,  1010,  1010,  1011,  1011,  1011,
    1012,  1012,  1013,  1013,  1013,  1013,  1014,  1014,  1014,  1014,
    1015,  1016,  1016,  1016,  1016,  1016,  1016,  1016,  1016,  1016,
    1017,  1017,  1017,  1017,  1018,  1018,  1019,  1020,  1021,  1021,
    1022,  1022,  1023,  1023,  1023,  1023,  1024,  1024,  1025,  1026,
    1027,  1028,  1028,  1029,  1029,  1030,  1030,  1030,  1030,  1031,
    1032,  1032,  1033,  1034,  1034,  1035,  1036,  1036,  1037,  1037,
    1038,  1038,  1039,  1039,  1040,  1040,  1041,  1041,  1042,  1042,
    1042,  1042,  1042,  1042,  1042,  1042,  1042,  1042,  1042,  1042,
    1042,  1042,  1042,  1042,  1043,  1044,  1044,  1044,  1044,  1044,
    1044,  1044,  1044,  1044,  1044,  1044,  1044,  1044,  1044,  1044,
    1044,  1045,  1046,  1046,  1047,  1047,  1048,  1049,  1049,  1049,
    1050,  1050,  1051,  1052,  1052,  1053,  1053,  1054,  1055,  1055,
    1056,  1057,  1058,  1058,  1059,  1059,  1059,  1059,  1060,  1061,
    1062,  1063,  1063,  1064,  1064,  1065,  1066,  1066,  1067,  1068,
    1068,  1069,  1070,  1071,  1071,  1072,  1072,  1073,  1073,  1074,
    1075,  1076,  1077,  1078,  1079,  1080,  1081,  1081,  1082,  1082,
    1082,  1083,  1084,  1084,  1085,  1085,  1085,  1086,  1087,  1087,
    1087,  1088,  1089,  1090,  1090,  1091,  1091,  1092,  1093,  1094,
    1094,  1094,  1094,  1094,  1094,  1094,  1094,  1094,  1094,  1094,
    1095,  1095,  1095,  1096,  1096,  1097,  1097,  1098,  1098,  1098,
    1098,  1099,  1099,  1099,  1100,  1101,  1101,  1101,  1102,  1103,
    1103,  1103,  1103,  1104,  1104,  1105,  1105,  1105,  1105,  1106,
    1107,  1107,  1108,  1108,  1108,  1108,  1108,  1109,  1109,  1110,
    1110,  1110,  1110,  1110,  1110,  1110,  1111,  1111,  1112,  1112,
    1112,  1113,  1113,  1114,  1114,  1115,  1116,  1117,  1117,  1118,
    1118,  1119,  1119,  1120,  1120,  1121,  1121,  1122,  1123,  1123,
    1123,  1124,  1125,  1125,  1126,  1127,  1128,  1129,  1129,  1129,
    1129,  1129,  1129,  1130,  1130,  1131,  1131,  1132,  1132,  1133,
    1133,  1134,  1135,  1135,  1136,  1136,  1136,  1137,  1137,  1138,
    1139,  1139,  1139,  1140,  1140,  1141,  1142,  1142,  1142,  1143,
    1144,  1144,  1145,  1146,  1147,  1147,  1147,  1147,  1147,  1147,
    1147,  1148,  1148,  1149,  1150,  1150,  1150,  1151,  1152,  1153,
    1153,  1154,  1154,  1155,  1156,  1156,  1157,  1157,  1158,  1158,
    1159,  1160,  1160,  1160,  1160,  1160,  1160,  1161,  1162,  1162,
    1162,  1163,  1163,  1163,  1163,  1163,  1163,  1163,  1163,  1163,
    1163,  1164,  1164,  1164,  1164,  1164,  1164,  1165,  1165,  1166,
    1166,  1166,  1166,  1166,  1166,  1167,  1167,  1168,  1169,  1169,
    1170,  1170,  1170,  1170,  1170,  1170,  1171,  1171,  1172,  1172,
    1173,  1173,  1173,  1173,  1173,  1173,  1174,  1174,  1175,  1176,
    1177,  1177,  1178,  1178,  1179,  1180,  1180,  1181,  1181,  1182,
    1183,  1183,  1184,  1184,  1184,  1185,  1185,  1186,  1186,  1187,
    1188,  1188,  1189,  1189
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
       1,     1,     1,     1,     1,     1,     0,     1,     4,     5,
       4,     5,     2,     2,     1,     1,     1,     1,     2,     1,
       1,     3,     0,     1,     3,     1,     1,     2,     3,     4,
       5,     2,     1,     3,     1,     3,     1,     2,     1,     3,
       3,     4,     3,     4,     4,     1,     1,     1,     2,     2,
       3,     1,     2,     1,     3,     1,     3,     1,     3,     1,
       3,     2,     1,     1,     2,     1,     1,     2,     3,     2,
       3,     3,     4,     2,     3,     3,     4,     1,     3,     4,
       1,     3,     1,     1,     1,     1,     1,     1,     3,     3,
       4,     3,     4,     3,     2,     3,     1,     1,     1,     1,
       2,     1,     1,     2,     1,     2,     5,     7,     5,     5,
       7,     6,     7,     7,     8,     7,     8,     8,     9,     3,
       2,     2,     2,     3,     2,     3,     1,     2,     1,     1,
       2,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       4,     4,     2,     2,     2,     1,     1,     2,     2,     1,
       1,     4,     4,     4,     5,     4,     3,     3,     3,     2,
       3,     0,     2,     2,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     3,     3,     7,     0,     1,     1,     1,
       0,     1,     0,     2,     2,     4,     0,     3,     3,     3,
       4,     0,     2,     1,     3,     3,     2,     4,     4,     5,
       6,     6,     0,     1,     1,     2,     1,     1,     1,     1,
       1,     2,     2,     1,     2,     2,     3,     2,     2,     2,
       5,     7,     7,     9,    11,     9,     1,     4,     3,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     4,     4,
       0,     3,     3,     3,     2,     2,     2,     3,     3,     2,
       2,     2,     2,     2,     2,     5,     4,     4,     7,     3,
       7,     2,     2,     2,     6,     3,     1,     2,     2,     6,
       6,     6,     3,     2,     6,     6,     3,     4,     4,     4,
       5,     3,     4,     5,     5,     5,     6,     4,     0,     1,
       6,     4,     4,     6,     0,     1,     3,     1,     3,     1,
       1,     1,     1,     3,     1,     4,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     2,     1,     2,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     2,
       1,     3,     2,     2,     1,     3,     2,     2,     1,     3,
       2,     1,     1,     3,     4,     3,     1,     1,     1,     2,
       4,     2,     2,     3,     3,     1,     2,     1,     2,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     6,     6,     6,     6,     7,     7,     7,     6,
       7,     6,     3,     4,     5,     6,     7,     6,     6,     5,
       7,     0,     1,     1,     1,     2,     2,     2,     5,     0,
       1,     3,     5,     5,     5,     2,     2,     4,     4,     3,
       1,     3,     3,     5,     6,     7,     4,     2,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     2,     2,
       1,     2,     3,     3,     3,     3,     1,     1,     4,     3,
       3,     1,     1,     4,     7,     8,     3,     1,     2,     3,
       3,     3,     2,     3,     3,     3,     3,     2,     3,     3,
       3,     2,     3,     3,     3,     2,     3,     3,     3,     3,
       2,     2,     2,     3,     3,     3,     3,     3,     3,     3,
       3,     2,     2,     2,     2,     3,     3,     3,     2,     2,
       2,     3,     3,     3,     4,     4,     4,     2,     3,     3,
       3,     3,     3,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     2,     1,     1,     1,     3,     1,     1,     1,
       0,     3,     1,     1,     4,     8,     9,     0,     2,     0,
       1,     5,     6,     3,     4,    10,     6,     6,     6,     6,
       6,     6,     6,     6,     8,     6,     3,     5,     6,     5,
       6,     5,     0,     2,     2,     1,     1,     2,     2,     2,
       3,     1,     1,     0,     1,     1,     1,     2,     1,     4,
       0,     2,     3,     0,     2,     2,     3,     0,     2,     3,
       1,     8,    12,     1,     0,     2,     1,     3,     3,     1,
       1,     2,     0,     2,     3,     1,     3,     5,     0,     1,
       5,     3,     4,     7,     0,     3,     3,     3,     4,     3,
       6,     7,     5,     7,     9,     3,     1,    11,     9,     7,
       0,     1,     0,     1,     2,     4,     4,     2,     1,     3,
       4,     3,     4,     3,     4,     2,     3,     1,     1,     1,
       2,     1,     0,     1,     1,    10,    11,    10,    10,     9,
      15,    12,     6,     6,     5,     5,     6,     8,     5,     4,
       8,     9,    10,     8,     0,     1,     3,     0,     2,     0,
       4,     0,     2,     3,     2,     3,     8,     5,     4,    15,
      15,     3,     3,     3,     7,     0,     4,     3,     1,     3,
       0,     4,     3,     1,     3,     0,     4,     3,     1,     3,
       5,     3,     9,     3,     1,     1,     5,     6,     7,     9,
       0,     1,     0,     1,     4,     5,     1,     1,     3,     3,
       1,     2,     1,     1,     2,     2,     5,     0,     1,     0,
       1,     3,    11,     2,     3,     4,     0,     3,     0,     4,
       4,     0,     1,     1,     4,     3,     1,     5,     0,     2,
       0,     3,     0,     1,     2,     1,     2,     2,     1,     8,
      10,     7,     8,     0,     1,     1,     2,     2,     3,     1,
       0,     3,     3,     9,     3,     1,     9,     3,     7,     5,
       3,     1,     1,     2,     1,     2,     4,     0,     1,     2,
       1,     9,     6,     2,     1,     2,     1,     3,     1,     1,
       1,     5,     6,     7,     6,     6,     4,     0,     2,     1,
       1,     3,     1,     6,     0,     1,     1,     1,     0,     3,
       0,     1,     2,     1,     2,     3,     4,     5,     6,     2,
       6,     0,     2,     0,     2,     1,     4,     6,     7,     7,
       0,     1,     3,     0,     3,     3,     4,     2,     4,     2,
       3,     1,     4,     0,     3,     3,     3,     3,     2,     1,
       2,     0,     1,    10,     8,     8,    10,     0,     2,     1,
       0,     3,     3,     1,     1,     0,     3,     4,     3,     6,
       7,     7,     8,     4,     5,     1,     1,     6,     4,     4,
       5,     6,     6,     2,     2,     4,     5,     3,     6,     6,
       2,     3,     3,     1,     6,     7,     6,     7,     7,     8,
       6,     7,     9,    10,     5,     2,     1,     7,     3,     1,
       1,     1,     3,     1,     3,     3,     1,     2,     1,     6,
       7,     9,     1,     1,     6,     2,     1,     3,     1,     0,
       1,     1,     3,     1,     3,     3,     5,     1,     3,     3,
       1,     1,     3,    11,     2,     1,     4,     1,     2,     2,
       6,     7,     0,     3,     3,     1,     0,     3,    11,     0,
       3,     3,     1,     1,     1,     1,     1,     3,     4,     1,
       1,     3,     4,     1,     2,     1,     1,     3,     1,     3,
       1,     0,     2,     2,     3,     1,     6,     3,     1,     3,
       1,    10,    10,    10,     1,     1,     3,     4,     3,     1,
       2,     3,     1,     6,     3,     1,    10,     0,     1,     0,
       3,     0,     1,     1,     1,     1,     0,     2,     0,     2,
       2,     3,     1,     3,     5,     2,     0,     1,     2,     1,
       2,     2,     3,     1,     6,     4,     5,     7,     5,     4,
       1,     5,     1,     1,     1,     2,     1,     3,     4,     3,
       1,     1,     1,     4,     1,     5,     3,     0,     1,     1,
       6,     3,     1,     5,     5,     4,     2,     4,     3,     1,
       2,     8,     0,     2,     3,     1,     3,     1,     1,     4,
       3,     1,     3,     1,     3,     1,     3,     1,     1,     3,
       1,     3,     1,     1,     1,     3,     1,     3,     3,     1,
       3,     3,     1,     2,     2,     1,     1,     1,     1,     1,
       1,     1,     1,     5,     3,     1,     2,     4,     6,     0,
       1,     2,     2,     2,     0,     1,     4,     4,     3,     1,
       1,     4,     3,     1,     3,     1,     2,     1,     1,     3,
       1,     1,     1,     2,     0,     1,     0,     2,     0,     3,
       3,     3,     0,     1,     3,     2,     0,     2,     0,     3,
       4,     0,     1,     2,     3,     3,     3,     1,     0,     1,
       1,     2,     0,     3,     0,     1,     3,     0,     1,     1,
       1,     1,     1,     1,     3,     1,     3,     0,     1,     1,
       0,     2,     2,     9,    11,     2,     2,     3,     1,     0,
       3,     0,     2,     1,     3,     1,     3,     1,     2,     1,
       5,     6,     3,     4,     5,     6,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     4,     2,     2,     3,     3,
       1,     1,     2,     2,     1,     2,     1,     2,     2,     3,
       3,     2,     2,     3,     2,     3,     2,     2,     1,     3,
       3,     3,     3,     2,     2,     3,     3,     2,     2,     3,
       3,     1,     3,     3,     1,     5,     3,     1,     1,     1,
       1,     3,     1,     3,     3,     1,     3,     3,     1,     2,
       2,     2,     1,     1,     1,     1,     1,     1,     3,     1,
       1,     1,     1,     1,     1,     8,     6,     4,     2,     3,
       1,     1,     3,     1,     1,     1,     5,     3,     1,     0,
       1,     5,     4,     2,     1,     3,     2,     0,     2,     2,
       1,     3,     7,     6,     4,     5,     6,     7,     7,     6,
       8,     7,     4,     3,     6,     2,     0,     7,     3,     1,
       1,     2,     2,     0,     6,     0,     3,     3,     1,     1,
       0,     3,     0,     5,     2,     2,     1,     2,     2,     2,
       1,     2,     2,     1,     3,     3,     1,     5,     3,     1,
       1,     0,     8,     1,     1,     8,     6,     5,     2,     4,
       2,     4,     2,     4,     1,     1,     0,     2,     3,     3,
       1,     4,     0,     1,     1,     2,     0,     1,     0,     3,
       2,     1,     1,     1,     1,     3,     1,     1,     3,     4,
       3,     1,     4,     6,     6,     8,     1,     3,     5,     1,
       1,     3,     5,     3,     5,     7,     1,     1,     3,     5,
       4,     6,     7,     7,     8,     6,     5,     2,     0,     1,
       2,     1,     1,     1,     1,     1,     2,     1,     7,     3,
       5,     0,     1,     8,     8,     1,     1,     3,     5,     3,
       3,     1,     2,     0,     1,     2,     0,     1,     2,     1,
       3,     3,     3,     1,     1,     3,     2,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     5,     3,     4,     2,     3,     2,
       3,     2,     3,     2,     2,     2,     2,     2,     2,     2,
       2,     1,     5,     5,     5,     7,     7,     0,     2,     1,
       4,     4,     2,     6,     7,     2,     1,     3,     2,     1,
       3,     1,     0,     2,     1,     1,     1,     1,     4,     5,
       3,    10,    11,     0,     2,     7,     3,     5,     4,     0,
       2,     3,     2,     3,     5,     3,     2,     2,     3,     2,
       4,     4,     3,     3,     3,     2,     2,     1,     7,     2,
       5,     5,     1,     1,     1,     3,     5,     3,     4,     4,
       4,     3,     6,     1,     1,     2,     2,     3,     4,     4,
       4,     3,     6,     7,     6,     6,     7,     6,     4,     4,
       0,     2,     3,     0,     3,     0,     2,     1,     2,     2,
       2,     3,     3,     3,     3,     6,     7,     8,     7,     4,
       8,     7,     4,     2,     2,     6,     6,     7,     8,     4,
       3,     1,     2,     4,     5,     3,     6,     0,     1,     2,
       2,     4,     4,     5,     6,     4,     2,     2,     0,     1,
       2,     1,     1,     0,     1,     2,     3,     1,     2,     1,
       2,     1,     1,     1,     1,     3,     1,     1,     0,     3,
       5,     8,     2,     4,     4,     3,     2,     3,     3,     4,
       1,     1,     2,     1,     1,     0,     2,     0,     2,     3,
       1,     4,     1,     1,     0,     1,     1,     0,     1,     1,
       0,     4,     3,     0,     4,     2,     1,     1,     1,     4,
       2,     4,     3,    10,     0,     2,     2,     3,     4,     4,
       5,     2,     2,     5,     2,     2,     3,     4,    11,     0,
       2,     0,     1,     3,     0,     1,     1,     1,     0,     1,
       2,     6,     6,     6,     5,     5,     5,     3,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     2,     4,     4,     4,     6,     6,     1,     3,     3,
       3,     4,     2,     4,     4,     3,     1,     2,     0,     2,
       2,     4,     4,     4,     6,     6,     0,     2,     1,     1,
       2,     4,     4,     4,     6,     6,     1,     3,     2,     2,
       0,     1,     0,     1,     2,     0,     1,     0,     2,     3,
       0,     1,     2,     2,     2,     0,     3,     3,     1,     3,
       0,     4,     1,     1
};

/* YYDEFACT[STATE-NAME] -- Default rule to reduce with in state
   STATE-NUM when YYTABLE doesn't specify something else to do.  Zero
   means the default is an error.  */
static const yytype_uint16 yydefact[] =
{
       0,     7,    98,   100,   101,   111,   110,     0,    96,   109,
     105,   106,    99,   104,   107,    97,   145,    95,   146,   108,
     113,   112,   258,   116,   117,   118,   119,   120,   121,   122,
     123,   124,   125,   126,   127,   128,   129,   130,   131,   132,
     133,   134,   135,     0,     0,     0,   275,   276,   279,   280,
       0,     0,   291,     0,   177,     0,     2,     4,     9,     0,
      86,    88,   114,     0,   115,     0,   166,     0,     8,   168,
       6,   261,   262,   263,   266,   267,   264,   265,   268,   269,
       5,   102,   103,     0,   161,     0,     0,   273,   272,   274,
     277,   278,     0,     0,     0,     0,   419,     0,   424,     0,
       0,     0,     0,   289,     0,     0,   181,   179,   178,     1,
       3,    84,     0,    90,    93,    87,    89,   144,     0,     0,
     142,   231,   232,     0,   256,     0,     0,   254,   176,   175,
       0,     0,   167,     0,   162,   164,     0,     0,     0,   293,
     292,     0,     0,     0,     0,     0,   425,     0,     0,     0,
       0,   639,     0,   803,     0,     0,  1257,  1257,   356,     0,
       0,  1257,     0,   332,     0,     0,  1257,  1257,  1257,     0,
       0,     0,     0,   639,     0,  1257,     0,     0,  1257,     0,
       0,     0,     0,     0,     0,  1293,  1294,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   396,     0,     0,     0,
     294,     0,   295,   296,   297,   298,   299,   300,   301,   302,
       0,   359,   360,   361,     0,   362,   363,     0,   364,   365,
     366,   367,     0,     0,     0,   441,   442,     0,   538,   537,
     443,   444,   445,   447,   545,   450,   547,   451,   452,   453,
     454,   455,   480,   518,   517,   456,   681,   682,   457,   762,
     763,   505,   507,   458,   468,   464,   465,   479,   478,   476,
     477,   466,   467,   473,   462,   463,   474,   470,   469,     0,
     459,   508,   471,   472,   475,   481,   482,   483,   484,   485,
     460,   461,   506,   509,   513,   520,   524,   528,   536,  1179,
    1172,  1173,   531,   532,  1422,  1244,  1245,  1408,     0,  1249,
    1255,     0,   488,     0,     0,     0,     0,     0,     0,   489,
     490,   491,   492,   493,   449,   494,   495,   496,   497,   498,
     500,   499,   501,   502,     0,   503,   504,   486,     0,   487,
     510,   511,   512,   514,   515,   516,   286,   287,   288,   290,
     169,   182,   180,    92,    85,     0,     0,   255,   136,     0,
     147,   149,   152,   150,   136,     0,   143,    93,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      11,   259,     0,     0,     0,    30,    31,    32,    33,    34,
      35,   234,   224,    14,    24,    36,     0,    38,    42,    45,
      48,    53,    56,    58,    60,    62,    64,    66,    68,    81,
       0,   226,   227,   212,   213,   229,     0,   214,   215,   216,
     217,    10,    12,   228,   233,   257,   172,     0,   183,   185,
     170,    36,    83,     0,    10,   193,     0,   187,   189,   192,
       0,   159,     0,     0,   270,   271,     0,     0,     0,     0,
     406,     0,   411,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     347,     0,     0,   640,   635,     0,     0,     0,  1970,  1638,
       0,     0,  1640,  1125,     0,     0,     0,     0,     0,  1932,
    1642,     0,     0,     0,   973,     0,     0,   802,   804,     0,
       0,     0,   306,   550,   551,   552,   587,   553,     0,   554,
     598,   591,   599,   555,   596,   577,   556,   557,   558,   575,
    1545,   578,   559,   562,   588,  1560,   579,   563,   564,   592,
     565,   590,   566,  1543,   597,   593,     0,   576,   567,   595,
     568,   569,   582,   594,   571,   589,  1544,   583,   584,   610,
     572,   573,   574,   580,   585,   586,   601,   611,   600,     0,
     602,   570,   581,  1546,  1547,     0,  1231,     0,     0,  1553,
    1550,  1549,  2062,  1551,  1552,  2009,  2008,     0,     0,   560,
     561,  2011,  2012,  2013,  2014,  2015,  2016,  2017,  2018,  2019,
    2020,  1569,     0,     0,  1133,  1530,  1532,  1535,  1538,  1542,
    1565,  1561,  1563,  1564,   549,  2010,  1554,  1258,     0,     0,
       0,     0,     0,     0,  1125,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,  1126,     0,
       0,     0,     0,     0,  1698,     0,   384,   383,     0,     0,
    1827,   333,   337,     0,   336,   339,   338,     0,   340,     0,
     334,   541,   542,     0,   751,   743,     0,   744,     0,   745,
       0,   747,   753,   748,   749,   750,   754,   746,     0,     0,
     687,   755,     0,     0,     0,   326,     0,     0,     0,     0,
    1134,  1453,  1455,  1457,  1459,     0,  1490,     0,   637,   636,
    1261,     0,  2008,     0,   403,     0,  1250,  1252,  1110,     0,
     650,     0,     0,     0,     0,     0,  1296,  1295,     0,     0,
       0,     0,     0,   349,   348,   398,   397,     0,     0,     0,
       0,     0,   282,   316,   316,   283,     0,   639,   803,     0,
     639,   374,   375,   376,   537,   380,   381,   382,     0,   285,
     281,   519,   523,   527,     0,   539,     0,   332,     0,   546,
     548,     0,  1180,  1181,  1424,     0,  1115,  1225,     0,  1223,
    1226,     0,  1418,  1408,     0,   446,   448,   522,   526,   530,
    1422,   392,   391,   393,  1646,  1646,     0,     0,     0,    91,
       0,   207,    94,   137,   138,   136,   148,     0,     0,   153,
     156,   140,   136,   251,     0,   250,     0,     0,     0,     0,
     252,     0,     0,    28,     0,     0,     0,   260,     0,    25,
      26,     0,   193,     0,    20,    21,     0,     0,     0,     0,
      77,    76,    74,    75,    71,    72,    73,    78,    79,    80,
      70,     0,    27,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   235,   225,   230,     0,   174,     0,   171,
       0,     0,   191,   195,   194,   196,   173,     0,   163,   165,
     160,     0,   421,     0,   412,     0,   417,     0,   407,   409,
       0,   408,     0,     0,   422,   427,  1843,     0,     0,     0,
       0,  1844,     0,     0,     0,  1851,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   622,   346,     0,     0,     0,     0,     0,
       0,     0,     0,  1107,     0,     0,   967,     0,  1003,     0,
     974,     0,   680,     0,     0,  1108,     0,     0,     0,   325,
     309,   308,   307,   310,     0,     0,  1577,  1580,     0,  1541,
     554,  1568,  1595,   549,     0,     0,     0,  1626,     0,  1539,
    1540,  2062,  2062,  2066,  2032,  2063,     0,  2060,  2060,  1570,
    1558,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,  2008,  2080,  1276,     0,  1264,  2038,  1265,
    1272,  1276,   549,     0,  1874,  1972,  1823,  1822,     0,  1137,
     846,   773,   862,  1935,  1824,   952,  1118,   679,   847,     0,
     786,  1928,  2007,     0,     0,     0,     0,     0,     0,     0,
       0,   639,     0,  1698,  1656,     0,     0,     0,     0,     0,
       0,     0,     0,   639,  1257,     0,     0,     0,     0,     0,
       0,  2062,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,  1422,  1215,  1216,  1408,     0,  1228,  1238,     0,
    1834,  1679,     0,  1749,  1723,  1737,  1738,  1753,     0,  1739,
    1742,  1744,  1745,  1746,  1656,  1795,  1794,  1796,  1797,  1740,
    1741,  1743,  1747,  1748,  1750,  1751,  1752,     0,  1771,     0,
    1667,     0,     0,     0,  1829,  1699,  1701,  1702,  1703,  1704,
    1705,  1711,     0,     0,   395,     0,  1826,     0,     0,   343,
     342,   341,     0,   335,   389,   752,     0,   711,   722,   729,
     688,     0,   692,     0,   705,   710,   701,   712,   697,   721,
     724,   737,     0,   728,     0,     0,     0,   723,   730,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   402,  1458,  1231,  1487,  1486,     0,     0,
       0,  1508,     0,     0,     0,     0,     0,  1494,  1491,  1496,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,  1262,  1263,     0,     0,     0,     0,   641,     0,
    1276,     0,  1166,  1168,     0,     0,     0,     0,     0,   849,
       0,  2080,   841,     0,  1301,     0,  1300,     0,     0,  1254,
     543,   544,   370,   370,   372,   373,   378,  2036,   358,     0,
     377,     0,   304,   303,     0,     0,     0,     0,   284,   521,
     525,   529,     0,     0,     0,     0,     0,  1425,   535,  1424,
       0,   533,     0,     0,  1224,  1248,     0,     0,  1419,  1246,
    1420,  1418,     0,     0,  1115,     0,     0,     0,  1644,  1645,
       0,  1944,  1943,     0,     0,     0,   210,     0,   139,   157,
     155,   151,     0,     0,   141,     0,   221,   223,     0,     0,
       0,   249,   253,     0,     0,     0,     0,    13,     0,   195,
       0,    19,    16,     0,    22,     0,    18,    69,    39,    40,
      41,    43,    44,    47,    46,    51,    52,    49,    50,    54,
      55,    57,    59,    61,    63,    65,     0,    82,   218,   219,
     184,   186,   203,     0,     0,   199,     0,   197,     0,     0,
     188,   190,     0,     0,     0,   413,   415,     0,   414,   410,
       0,   431,   432,     0,     0,     0,     0,     0,  1858,     0,
    1859,  1860,     0,  1128,  1129,     0,  1867,  1849,  1850,  1821,
    1820,  1135,   878,   945,   944,   881,   879,     0,   882,   877,
     865,   868,     0,   866,     0,     0,     0,     0,  1047,     0,
     950,     0,     0,   950,     0,     0,     0,     0,     0,     0,
       0,     0,   911,   899,  1968,  1967,  1966,  1934,     0,     0,
       0,     0,     0,     0,   656,   667,   666,   663,   664,   665,
     670,  1117,   957,     0,     0,   956,   954,   960,     0,     0,
       0,   824,     0,   838,   832,     0,     0,     0,     0,     0,
       0,  1921,  1922,  1882,  1879,     0,     0,     0,     0,     0,
     623,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     975,  1971,  1639,  1641,  1933,  1643,  1109,     0,     0,     0,
     968,   969,     0,     0,     0,     0,     0,     0,     0,  1047,
    1027,  1010,  1005,  1024,  1004,  1009,     0,     0,  1110,     0,
    1832,  1833,     0,   813,  1110,     0,   311,     0,     0,     0,
    1577,  1574,     0,     0,  1579,     0,     0,  1559,   549,  1232,
    1235,  1110,  1257,  1630,  1418,  1629,  1562,     0,  2062,  2029,
    2030,  2064,  2061,  2065,     0,  2065,  1593,     0,  1548,  1531,
    1533,  1534,  1536,  1537,     0,     0,  1603,     0,     0,  1663,
       0,   604,   606,   605,   607,   561,   609,  1569,     0,   549,
    1276,     0,     0,     0,  1219,  1220,  1408,     0,  1230,  1240,
       0,     0,  1140,     0,  1279,  1451,  1277,     0,     0,     0,
       0,  1275,     0,  1123,   774,   863,   848,   953,   678,  1987,
       0,  1889,     0,     0,     0,  1083,     0,  1723,     0,  1661,
       0,  1792,  1786,     0,   635,  1819,     0,  1809,  1657,     0,
       0,  1680,     0,  1662,     0,     0,  1812,     0,  1667,  1817,
       0,   637,  1261,     0,  1676,     0,  1233,  1816,     0,  1734,
       0,     0,  2058,  1768,  1767,  1769,  1766,  1761,  1757,  1759,
    1765,  1763,  1764,     0,     0,  1418,  1408,     0,     0,     0,
    1422,     0,  1726,     0,  1724,  1736,  1770,     0,     0,     0,
    1666,  1825,  1664,     0,     0,     0,  1646,     0,  1700,     0,
    1712,     0,     0,     0,  2062,     0,     0,   387,   386,   344,
     345,     0,     0,   694,   707,   714,   699,   690,     0,   741,
     731,   693,   706,   713,   698,   725,   689,     0,   740,   702,
     733,   715,   732,   695,   708,   703,   716,   700,   726,   738,
     691,     0,   742,   704,   696,   727,   739,   717,   709,   719,
     720,   718,     0,     0,   759,   683,   757,   758,   686,     0,
       0,  1156,  1276,  2080,  2080,   328,   327,  2027,  1454,  1456,
    1488,  1489,     0,  1484,  1462,  1501,     0,     0,     0,  1493,
    1502,  1513,  1514,  1495,  1492,  1498,  1506,  1507,  1504,  1517,
    1518,  1497,  1466,  1467,  1468,  1469,  1470,  1471,  1356,  1361,
    1362,  1231,  1358,  1357,  1359,  1360,  1527,  1529,  1472,  1524,
     549,  1528,  1473,  1474,  1475,  1476,  1477,  1478,  1479,  1480,
    1481,  1482,  1483,   764,     0,     0,     0,     0,  2046,  1840,
    1839,     0,   646,   645,  1838,  1167,     0,     0,  1374,  1370,
    1374,  1374,     0,  1251,  1114,     0,  1113,     0,   842,   651,
       0,     0,     0,  1297,     0,     0,     0,     0,  1010,     0,
     368,   369,     0,   540,     0,   321,     0,  1107,     0,     0,
     357,     0,  1427,  1423,     0,  1409,  1435,  1437,     0,     0,
    1413,  1417,  1421,  1247,     0,   622,   534,  1647,     0,  1650,
    1652,     0,  1698,  1656,     0,  1707,     0,  1941,  1940,  1945,
    1936,  1969,     0,   208,   154,   158,   220,   222,     0,     0,
       0,     0,    29,     0,     0,     0,    37,    17,     0,    15,
       0,   204,   198,   200,   205,     0,   201,     0,   420,   405,
     404,   416,   429,   430,   428,  2062,   423,   434,   426,     0,
       0,     0,  1865,     0,  1863,     0,  1130,     0,     0,  1870,
       0,  1868,  1869,  1136,     0,   884,   883,   880,     0,     0,
       0,   875,   867,     0,     0,     0,     0,     0,   642,     0,
     951,     0,     0,   935,     0,   946,     0,     0,     0,     0,
       0,   914,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   898,     0,     0,     0,     0,   911,   894,
       0,     0,   895,     0,   669,   668,     0,     0,     0,   671,
       0,     0,   955,     0,   963,   962,     0,   961,     0,  1983,
       0,     0,     0,     0,   826,     0,   791,     0,     0,     0,
       0,     0,     0,   839,   787,     0,     0,   830,   829,   832,
     789,     0,     0,  1883,     0,  1884,     0,   633,   634,   624,
       0,     0,  1997,  1996,     0,     0,     0,     0,  2004,  2005,
    2006,   629,     0,     0,     0,     0,   643,   350,     0,     0,
       0,     0,     0,     0,  1108,     0,     0,  1054,  1073,     0,
    1052,   966,   970,   771,     0,     0,     0,  2073,  2074,  2072,
    1100,  1025,     0,  1038,  1039,  1040,     0,     0,  1028,  1030,
       0,     0,  1007,  1023,  1006,     0,     0,  1003,  1897,  1908,
    1891,     0,  1667,     0,   817,     0,     0,   312,     0,  1581,
       0,  1573,     0,  1578,  1572,     0,  1567,   549,     0,     0,
    1261,     0,     0,  1625,  2031,  2033,  2059,  2034,  1592,     0,
       0,     0,     0,  1584,  1631,  1596,  1672,  1557,     0,     0,
       0,  1451,     0,     0,  1141,     0,  1418,  1408,  1142,     0,
    1280,  1278,     0,  1411,  1451,  2062,     0,     0,  2039,     0,
    1451,  1271,  1273,   549,   864,   767,  1124,     0,     0,     0,
       0,     0,     0,     0,     0,  1792,  1789,     0,     0,  1785,
       0,  1806,     0,     0,     0,     0,     0,  2083,  2082,     0,
    1811,     0,  1777,     0,  1813,     0,  1818,     0,     0,     0,
    1656,  1815,     0,  1237,     0,  1755,  1227,  1217,  1418,  1762,
    1758,  1760,     0,     0,     0,  1725,  1727,  1729,  1656,  1800,
       0,     0,  1668,     0,  1677,  1835,   549,  1837,     0,     0,
    1709,     0,  1658,  1686,  1687,     0,   379,  2021,  2060,  2060,
       0,   329,     0,   735,   734,   736,     0,     0,     0,  1206,
    1155,  1154,  1206,  1276,  1083,     0,  1485,     0,  1463,     0,
       0,  1500,  1511,  1512,  1503,  1515,  1516,  1499,  1509,  1510,
    1505,  1519,  1520,     0,     0,     0,     0,   638,   644,     0,
       0,     0,   649,     0,  1187,  1451,  1183,     0,   549,  1165,
    1375,  1373,  1371,  1372,     0,  1111,     0,     0,     0,  2080,
       0,  1298,  1299,     0,     0,     0,   860,     0,  2035,   317,
       0,     0,   319,   318,     0,   385,     0,     0,  1428,  1429,
    1430,  1431,  1432,  1433,  1426,  1116,     0,  1438,  1439,  1440,
    1410,     0,     0,     0,     0,  1648,     0,  1653,  1654,     0,
       0,  1656,     0,  1637,  1697,  1706,  1947,     0,  1942,     0,
     209,   211,     0,     0,     0,     0,     0,     0,   238,   239,
     236,    23,    67,   206,   202,     0,     0,  1854,  1852,     0,
       0,  1855,  1861,     0,  1857,     0,  1871,  1872,  1873,     0,
     869,     0,   871,     0,   873,   876,   971,  1131,  1132,   614,
     613,   612,   615,   619,   401,   621,     0,   400,   399,  1048,
    2067,     0,   913,  1047,     0,     0,     0,     0,     0,   896,
       0,   930,     0,     0,   923,   947,     0,   915,   952,     0,
     892,   922,     0,     0,     0,     0,     0,     0,   893,   904,
     921,   912,   674,   673,   677,   676,   675,   672,   958,   959,
     965,   964,     0,  1984,  1985,     0,   823,   776,     0,   778,
     808,   777,   779,     0,   825,   783,   802,   781,   785,     0,
     780,   782,     0,   788,   833,   835,     0,   831,     0,   790,
       0,  1885,  1927,  1886,  1926,     0,     0,   625,   628,   627,
    2000,  2003,  2002,     0,  2001,   631,  1139,   652,  1138,     0,
       0,  1897,     0,  1119,     0,  1917,  1841,     0,  1842,     0,
       0,  1110,   772,  1056,  1057,  1055,  1058,  1054,  1080,  1075,
    1101,     0,     0,     0,     0,  1003,  1047,     0,     0,     0,
    1029,     0,     0,  1008,  1913,   654,  1027,  1897,  1892,  1898,
       0,     0,     0,  1875,  1909,  1911,  1912,     0,     0,   814,
     815,     0,   792,     0,     0,     0,     0,     0,  1576,  1575,
    1571,     0,     0,  1234,     0,     0,  1628,  1627,  1631,  1631,
    1603,  1605,     0,  1603,     0,  1631,     0,  1585,     0,     0,
    1569,     0,   549,  1411,  1451,  1239,  1229,  1221,  1418,     0,
    1452,  1144,  1412,  1146,  2040,  2060,  2060,     0,  1175,  1176,
    1178,  1174,     0,     0,     0,  1929,     0,     0,  1091,     0,
       0,     0,  1656,  1787,  1791,     0,     0,  1788,  1793,     0,
       0,  1723,  1810,  1808,     0,  1671,     0,     0,     0,     0,
     549,  1782,     0,     0,     0,     0,  1779,  1798,     0,  1667,
    1266,     0,   549,     0,  1735,     0,  1218,  1756,     0,     0,
       0,  1733,  1728,  1831,   394,   608,   603,  1669,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,  1830,  2065,
    2065,  2062,     0,     0,   331,     0,   760,   756,     0,     0,
       0,     0,     0,  1150,     0,     0,     0,  2028,  1464,     0,
       0,  1460,  1523,  1526,   549,     0,   767,  2048,  2049,     0,
       0,  2062,  2047,     0,  1451,   647,   648,  1191,     0,  1190,
     549,     0,  1411,     0,     0,  1369,  1112,     0,   844,     0,
       0,  1448,     0,     0,   850,     0,  1027,   861,   371,   322,
     323,   320,     0,   330,     0,  1437,  1434,     0,  1436,  1416,
    1414,  1415,  1649,  1655,  1658,  1698,  1636,     0,     0,  1960,
    1946,  1937,  1938,  1945,     0,   241,     0,     0,     0,     0,
       0,     0,     0,     0,   433,  1853,  1866,  1862,     0,  1856,
     870,   872,   874,   618,   617,   620,   616,     0,     0,  1027,
     941,     0,  1050,     0,  1049,     0,     0,  1437,     0,     0,
     978,     0,     0,   925,     0,     0,     0,     0,   948,  1437,
       0,     0,     0,     0,   918,     0,     0,     0,     0,   907,
     905,  1982,  1981,  1986,     0,     0,   807,   827,     0,     0,
       0,     0,   828,  1923,  1924,     0,  1887,  1881,     0,     0,
     626,  1993,   632,   630,     0,   351,   352,  1897,  1876,     0,
    1121,  1918,     0,     0,     0,  1845,     0,  1846,     0,     0,
       0,  1058,     0,  1074,  1081,     0,  1102,  2067,  1051,     0,
    1026,     0,  1027,  1037,     0,     0,  1001,  1012,  1011,  1878,
    1914,   655,   658,   662,   659,   660,   661,  1974,  1899,  1900,
    1897,     0,  1895,     0,  1919,  1915,  1890,  1910,  1989,  1667,
     816,   818,     0,     0,  1115,  1120,     0,   314,   313,   305,
    1594,  1566,     0,  1268,  1603,  1603,  1589,     0,  1610,     0,
    1586,     0,  1603,  1674,  1583,  1673,  1556,     0,     0,  1145,
    1147,  1222,  2081,  2065,  2065,  2062,     0,     0,     0,  1274,
     767,   768,   388,   390,  1084,     0,     0,     0,  1790,     0,
       0,  1807,     0,     0,  1774,     0,     0,     0,  1656,     0,
    1778,     0,     0,     0,  1814,     0,     0,     0,  1773,     0,
    1799,  1754,  1772,  1731,  1730,     0,     0,  1678,  1836,   549,
    1422,     0,     0,  1683,  1681,  1660,     0,  1710,     0,     0,
    1688,  1698,  2022,  2023,  2024,  2060,  2060,   760,     0,   684,
    1205,     0,     0,  1148,     0,     0,  1151,     0,     0,  1465,
    1461,     0,   767,   769,  2060,  2060,  2050,     0,     0,     0,
       0,  1281,  1283,     0,  1292,  1290,  2080,  1398,     0,     0,
       0,  1182,  1170,  1185,  1184,  1188,     0,  1253,     0,   843,
       0,  1445,     0,  1446,  1424,     0,     0,   853,     0,   981,
       0,  1087,  1441,  1442,  1651,  1656,  1723,  1953,  1952,  1948,
    1950,  1954,     0,     0,  1939,   240,   242,   243,     0,   245,
       0,     0,     0,   237,   436,   437,   438,   439,   440,   435,
    1864,     0,  2068,  2070,     0,   938,     0,  1027,  1046,     0,
     978,   978,   978,     0,     0,   990,   930,     0,     0,   900,
       0,     0,     0,     0,     0,     0,     0,  1089,   952,     0,
       0,     0,     0,   897,   903,   917,     0,     0,   909,     0,
     810,   801,   784,     0,   834,   836,  1888,  1925,  1880,   653,
       0,     0,  1877,     0,  1847,     0,     0,     0,  1101,  1071,
       0,     0,  1104,  1002,     0,   999,     0,     0,   657,     0,
       0,     0,     0,  1897,  1893,  1916,  1920,     0,  1991,     0,
       0,     0,     0,   796,   793,   795,     0,  1105,  1122,     0,
       0,     0,     0,  1587,  1588,     0,     0,  1612,     0,     0,
    1582,     0,  1591,     0,  1569,  2042,  2043,  2041,  2060,  2060,
    1177,  1930,  1090,  1161,     0,  1159,  1160,  1695,     0,  1783,
    1656,  1670,     0,     0,  1803,     0,  1603,     0,  1777,  1777,
       0,  1267,  2037,  2056,  1451,  1451,  1732,   603,     0,  1715,
       0,  1716,     0,     0,  1721,  1659,  1690,     0,     0,     0,
    1828,  2065,  2065,   685,     0,  1207,     0,     0,  1207,     0,
    1149,  1163,  1369,  1209,  1525,   769,   770,   765,  2065,  2065,
       0,     0,  2062,     0,     0,     0,     0,     0,     0,     0,
       0,     0,  1307,     0,     0,     0,  1259,     0,  1406,     0,
    1189,  1192,  1169,     0,     0,  1447,  1449,     0,     0,   324,
     983,   976,   982,  1437,  1635,     0,     0,  1955,  1956,  1957,
       0,  1698,  1931,   244,   246,   247,     0,     0,  2069,  2071,
     936,     0,     0,  2075,     0,   990,   990,   990,  1085,     0,
       0,  1041,   925,     0,   933,     0,     0,     0,     0,     0,
       0,   889,     0,   952,   949,  1437,     0,     0,  1021,  1022,
       0,   906,   908,     0,   901,     0,     0,   809,     0,   840,
       0,   355,   353,     0,  1848,  1115,  1059,  2067,     0,  1047,
    1082,  1076,     0,     0,     0,     0,  1036,     0,  1034,     0,
    1975,  1976,     0,  1905,  1902,  1919,  1901,  1897,  1894,  1990,
    1994,  1992,   799,   797,   798,   794,   806,     0,   805,   315,
    1236,  1269,  1270,  1451,  1609,  1606,  1608,     0,     0,     0,
    1633,  1634,     0,     0,  1675,  1590,  1555,  2065,  2065,  1171,
       0,  1784,     0,  1775,  1803,     0,     0,  1805,  1596,  1781,
    1780,  1776,     0,  1398,  1398,  1708,     0,     0,     0,  1722,
       0,     0,     0,     0,     0,  1684,  1682,     0,  1689,  2025,
    2026,   761,  1204,     0,  1152,     0,     0,     0,     0,     0,
    1451,   766,  2052,  2053,  2060,  2060,  2051,     0,  2008,     0,
    1302,     0,     0,  1307,  1282,     0,     0,  1276,     0,  1365,
    2080,  2080,     0,  1322,  1276,  1308,  1309,     0,     0,  1405,
       0,  1396,  1406,     0,  1402,  1186,   845,  1424,     0,  1443,
     852,     0,     0,     0,   978,  1086,  1696,  1949,  1958,     0,
    1963,  1962,  1965,   248,     0,   937,   939,     0,   885,     0,
    1044,  1045,  1042,     0,   992,   924,   931,     0,     0,     0,
       0,   928,     0,     0,   887,   888,     0,   902,  1088,     0,
     935,     0,     0,     0,     0,     0,     0,   811,   800,   837,
       0,  1127,  1106,  1103,  1072,     0,  1053,  1047,  1063,     0,
       0,     0,     0,  1000,  1035,  1032,  1033,     0,     0,  1977,
    1973,  1920,  1903,  1896,  1998,  1995,   775,   820,  1398,     0,
    1611,  1616,     0,  1623,     0,  1614,     0,  1604,     0,     0,
    2044,  2045,  1158,  1694,     0,  1804,  1656,  2057,  1259,  1259,
       0,  1714,  1717,  1713,  1720,     0,     0,  1691,     0,  1157,
    1153,  1164,  1162,     0,  1214,     0,  1212,  1213,  1411,  2065,
    2065,     0,  1276,     0,     0,  1276,  1369,  1276,  1289,  1276,
       0,     0,  1366,  1307,     0,     0,     0,  1285,  1404,  1401,
       0,     0,  1256,  1399,  1407,  1403,  1400,  1449,     0,     0,
       0,     0,     0,   986,     0,   992,  1951,  1959,     0,  1961,
       0,     0,     0,     0,  2078,     0,  1043,   980,   979,     0,
     998,     0,   991,   993,   995,   932,   934,     0,   926,     0,
    2075,     0,   886,  2067,   916,     0,  1020,     0,     0,     0,
       0,   812,   354,     0,     0,     0,  1064,     0,  1437,  1069,
    1062,     0,     0,  1077,     0,     0,  1015,     0,  1978,  1979,
    1906,  1904,  1907,  1988,  1999,     0,  1259,  1607,     0,  1615,
       0,  1617,  1618,     0,  1600,     0,  1599,  1656,     0,  1396,
    1396,  1719,     0,  1692,  1693,  1685,     0,  1193,  1195,  1210,
       0,  1208,  2054,  2055,     0,  1291,  1306,   549,     0,  1286,
       0,  1288,  1363,  1364,  2080,  1276,     0,     0,  1312,  1323,
       0,     0,     0,     0,  1391,  1392,  1260,  1390,  1394,  1397,
    1444,  1450,   851,     0,     0,   856,     0,   984,     0,   977,
     972,     0,  2067,   940,     0,  2076,     0,  1083,   997,   996,
     994,   929,   927,   891,     0,   943,     0,     0,   910,     0,
       0,     0,  1437,  1065,  1437,   978,  1078,  1079,  2067,  1013,
       0,     0,  1980,  1906,   819,  1396,     0,  1624,     0,  1601,
    1602,  1597,     0,     0,  1801,  1241,  1242,  1718,     0,  1197,
       0,  1194,  1211,  1303,     0,  1276,  1368,  1367,  1284,     0,
       0,     0,     0,     0,  1328,     0,  1325,     0,     0,     0,
       0,  1395,  1393,     0,   854,     0,     0,   985,     0,   942,
    2079,  2077,  1093,     0,   935,   935,     0,   821,  1066,   978,
     978,   990,     0,  1017,  1014,  1031,  1243,  1620,     0,  1613,
       0,  1632,  1598,  1802,  1198,  1199,     0,  1305,   549,  1287,
       0,     0,     0,  1316,  1311,     0,     0,     0,  1327,     0,
       0,  1379,  1380,     0,  1231,  1387,     0,  1385,  1383,  1388,
    1389,     0,   855,   988,  1964,     0,  1092,  2075,     0,     0,
       0,   990,   990,  1067,  2067,     0,     0,  1619,  1621,  1622,
    1206,     0,  1196,  1276,  1276,     0,     0,  1310,  1324,     0,
       0,  1377,     0,  1376,  1386,  1381,     0,     0,     0,   987,
       0,     0,     0,   890,     0,     0,     0,  1070,  1068,  1019,
       0,     0,     0,     0,  1314,  1313,  1315,     0,     0,     0,
    1319,  1276,  1355,  1326,     0,     0,  1378,  1384,  1382,     0,
     989,  1097,  1099,     0,  1096,  1094,  1095,   935,   935,     0,
       0,  2067,     0,  1451,  1353,  1354,  1317,     0,  1320,     0,
    1321,     0,  1098,     0,     0,   822,     0,  1018,     0,  1411,
    1318,     0,  1338,     0,  1331,  1333,  1335,   859,     0,     0,
     919,   920,  2067,     0,  1202,     0,  1337,  1329,     0,     0,
       0,     0,  1016,  1451,  1451,  1200,  1334,     0,  1330,     0,
    1332,  1340,  1343,  1344,  1346,  1349,  1352,     0,   858,  1201,
    1411,  1336,     0,  1342,     0,     0,     0,     0,     0,     0,
    1203,  1339,     0,  1345,  1347,  1348,  1350,  1351,   857,  1341
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
      -1,    55,    56,    57,   383,   384,  1293,   385,   386,   387,
     388,   389,   390,   391,   392,   393,   394,   395,   396,   397,
     398,   399,   821,   400,   423,    58,   123,   112,   345,   113,
      60,   106,   774,    62,   118,   119,    63,   349,   350,   351,
     778,  1272,   779,    64,   133,   134,    65,    66,   130,   131,
      67,   425,   417,   418,  1323,   427,   428,   429,   854,   855,
     772,  1267,   402,   403,   404,   405,   406,   125,   126,   407,
     408,   409,   410,    68,   127,   424,   412,   353,    71,    72,
      73,    74,    75,    76,    77,    78,    79,    80,    99,   199,
     200,   201,   933,  1487,  2507,  1222,  2273,  2679,   202,   203,
     204,   639,   640,  1100,   205,   206,   207,   208,   209,   210,
     211,  1810,   212,   213,   214,   215,  1657,   216,   217,   218,
     219,   220,   221,   222,   223,   100,   101,   102,   103,   104,
     147,   874,   875,  1884,  1343,  1886,  1887,  3019,   224,   225,
     226,   227,   228,   724,   230,   231,   581,  1538,   582,   232,
     233,  2783,  1999,  1032,  1033,  1034,   464,  1035,   238,   239,
    1918,   974,  1441,   240,   241,  2821,  2822,  1404,  2823,  1406,
    1407,  2824,  2825,  2826,  2396,   242,   243,   244,   245,   246,
     247,   659,   660,  1705,  1706,  2939,   248,   249,   250,  2555,
    3187,   251,   252,   253,  2843,  3104,  3105,  3062,   488,   489,
    3317,  2409,  2410,  3277,  2064,  2502,  3496,  2407,  1976,  1989,
    1990,  2424,  2425,  1984,   254,   255,   256,  2979,   257,   258,
     259,   260,   261,  2264,   262,  1808,  3694,  3695,  2676,  1566,
     263,  1370,  1371,  1907,   264,  2759,  2760,  3058,  3274,  1952,
    1393,  3039,  3450,  3451,  2743,  3253,  3254,  2362,  3024,  3025,
    1923,  3460,  1372,   265,  1924,  1568,   266,  1416,  1417,  1967,
     267,  1461,  2031,  1373,   268,   269,  3424,  3035,  3221,  3222,
    3572,  3573,  3869,  3251,  3592,  3593,  3594,   270,  1470,  2052,
    1471,  3625,  3626,  3267,  3268,  1472,  1473,  2047,  2048,  2049,
    3297,  3298,  2042,  2043,  2044,  1926,  2733,  2029,  2030,  2466,
    2800,  3476,  3477,  3478,  3289,  2467,  2468,  2803,  2804,  2130,
    2740,  2684,  2750,  2559,  3586,  3836,  3904,  1474,  2807,   271,
     272,   273,   490,  1196,  1795,  1796,  1241,   274,   275,   276,
     277,   491,  2021,   278,   279,   280,   281,   282,   283,   284,
    1190,   976,  1036,  1037,  1710,  1711,  3134,  3135,  3180,  3181,
    1191,  1192,  1193,  1038,  1039,   289,   290,   291,  2120,  2548,
    2549,   744,  2245,  2246,  2247,  2658,  2659,  1040,  3657,  3658,
    3750,  3852,  3955,  2941,  2629,  1041,  3380,  3535,  3536,  1042,
    1043,  1044,  1543,  1544,  1545,   751,  1045,  1546,   946,  1046,
    1499,  1500,  1047,  1548,  1048,  1549,   294,   295,   296,   297,
     298,   686,   687,   299,   300,   598,  3411,  1184,   978,  2906,
    3112,   979,   980,  1555,  2654,  2961,  2962,  1049,   697,  1205,
    1206,  3389,  3404,  3405,  3677,  3678,  3762,  3816,  3889,  3890,
    3406,  3556,  3765,  3817,  3766,  3895,  3933,  3934,  3935,  3945,
    3936,  3960,  3972,  3961,  3962,  3963,  3964,  3965,  3966,  1757,
    2964,  3398,  3399,  2965,  3197,  2251,  3684,  3820,  3821,  3685,
    3826,  3827,  3828,  3686,  3687,  3772,  3562,  3206,  3207,  3566,
    3208,  3414,   752,  2541,  1248,  1830,  1249,  1250,   746,  1238,
    2284,  1825,  1826,  2289,  2688,   302,  2672,  3419,  2113,   947,
     671,   672,   673,   674,  1160,  1161,  1162,  1163,  1164,  1165,
    1166,  1167,  1168,  1169,  1170,  1171,  1172,  1173,  1174,  1175,
    1176,  1177,   583,  1758,   676,   585,   586,   587,   588,   589,
     942,   960,   590,  1490,  1491,  2069,  1493,   936,   937,   591,
    2525,  3645,  3646,  2093,  2858,  3325,  3326,  3117,  3329,  3505,
    3799,  3506,   948,   592,  1504,  1505,  2523,  3332,   303,   304,
     305,   306,   307,   308,  1260,  1256,  1838,  1839,  2299,  1587,
    2615,  1580,  1594,  1530,  1641,  1050,  2574,   593,  1052,  1590,
    2192,  2193,  1053,  2301,  1843,  2302,  1085,  1086,  1844,  1087,
    1088,  1089,  1651,  1090,  3160,  3357,  3163,  3164,  1633,  1634,
    2175,  2176,  2177,  2600,  1608,  1054,  1055,  1056,  1057,  1058,
    1059,  1060,  1061,  2585,  2586,  2152,  1062,  1581,  1582,  2135,
    2136,  2565,  2138,  1063,  1064,  1065,  1066,  1067,  3346,  1068,
    1069,  1070,  2144,  1071,  1072,  1073,  1074,  1075,  1076,   309,
     310,   311,   312,   313,  1077,   628,   629,   630,  1482,  1078,
    1079,   314,  1449,   315,   885,  2458,  2459,  2460,   316,  1894,
    2334,  2331,  1357,  1358,   317,   318,   319,   320,  1433,   321,
     322,  2059,  2060,  2488,  2489,  3492,  2493,  2494,  2819,  2495,
    2496,  2456,  3632,  1434,  2777,  2433,  2434,  1571,   323,   324,
     325,   326,  1263,  1850,  1264,  2308,  2699,  2999,  3000,  3001,
    3229,  3429,  3576,  3003,  3579,  3232,  1397,   327,   328,   329,
     330,  3090,  2404,   331,  1969,   332,   333,  3098,  3310,  2008,
    3494,  2009,  3633,  2010,   334,   335,   594,   595,  1658,  1716,
     596,  1218,  2907,  1559,  2118,  2240,  2651,  2652,  3152,  1081,
    1513,  1514,   954,   955,   956,  2729,  2730,  3238,  1475,  3438,
    3583,  3584,  1552,  2149
};

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
#define YYPACT_NINF -3634
static const yytype_int16 yypact[] =
{
    5803, -3634, -3634,   551,   838, -3634, -3634,   117, -3634, -3634,
   -3634, -3634, -3634, -3634, -3634, -3634, -3634, -3634, -3634, -3634,
   -3634, -3634, -3634, -3634, -3634, -3634, -3634, -3634, -3634, -3634,
   -3634, -3634, -3634, -3634, -3634, -3634, -3634, -3634, -3634, -3634,
   -3634, -3634, -3634,  1624,  1735,   957, -3634, -3634, -3634, -3634,
    1062,  1189,  1129,   114,  3794,  5705, -3634, -3634, -3634,   184,
   18108, 18108, -3634,   120, -3634,  4575,  1714,   144, -3634, -3634,
   -3634, -3634, -3634, -3634, -3634, -3634, -3634, -3634, -3634, -3634,
   -3634, -3634, -3634,  1272,   598,  1490,  1517, -3634, -3634, -3634,
   -3634, -3634,   309,  1349,  1518,  1285, -3634,   -12,  1476, 17560,
    1319,  1452,  1539, -3634,  1586,  1494, -3634, -3634,  3794, -3634,
   -3634, -3634,  1341, -3634,  2647, -3634, -3634, -3634,  4674,  5396,
    1525, -3634, -3634,   184, -3634,  5224,  4575, -3634,   140,  1567,
    1886, 17561,  1714,  1143, -3634,  1538,  1272,  1937,  1969, -3634,
   -3634,  1861,  1922,  1756,  1789,    37, -3634,  1781,  2925,   900,
    2041,  1918,  1864,  3339,  1880, 13048,  1898,  1898, -3634,  4024,
     407,  1898,    60,   582,  1777,  4751,  1898,  1898,  1898,  1919,
    2112,  9550,  4751,  1918,  4206,  1898,   733,  2143,  1898,  4206,
    4206,  2079,  2096,  2103,  2120, -3634, -3634,   -13,  2145,  2219,
     472,  2249,  2110,  2004,  2050,  1377,  2234,  2172,  2155,  2151,
   -3634,  1082, -3634, -3634, -3634, -3634, -3634, -3634, -3634, -3634,
    2207, -3634, -3634, -3634, 17696, -3634, -3634, 17840, -3634, -3634,
   -3634, -3634,  2363,  2284,  2303, -3634, -3634,   796,    12,   239,
   -3634, -3634, -3634, -3634,  2273, -3634,  2290, -3634, -3634, -3634,
   -3634, -3634, -3634, -3634, -3634, -3634, -3634, -3634, -3634, -3634,
   -3634, -3634, -3634, -3634, -3634, -3634, -3634, -3634, -3634, -3634,
   -3634, -3634, -3634, -3634, -3634, -3634, -3634, -3634, -3634,  4206,
   -3634, -3634, -3634, -3634, -3634, -3634, -3634, -3634, -3634, -3634,
   -3634, -3634, -3634, -3634, -3634, -3634, -3634, -3634, -3634,  1471,
   -3634, -3634, -3634, -3634,  2516, -3634, -3634,  1297,   472, -3634,
   -3634,  1266, -3634,  2336,  2386,  2388,  4206,  4206,  4206, -3634,
   -3634, -3634, -3634, -3634, -3634, -3634, -3634, -3634, -3634, -3634,
   -3634, -3634, -3634, -3634,  4206, -3634, -3634, -3634,  4206, -3634,
   -3634, -3634, -3634, -3634, -3634, -3634, -3634, -3634, -3634, -3634,
   -3634, -3634, -3634, -3634, -3634,   114,   218, -3634,  2671,  5487,
   -3634, -3634,  1351, -3634,  2671,  5607, -3634,  2335,  2328,  1886,
    2355,  2341,  2105,  2385,  1272,   358,  2160,  2387,  2400,  2402,
   -3634,  2762,  2211,  2211,  3905, -3634, -3634, -3634, -3634, -3634,
   -3634, -3634, -3634, -3634,   197,   822,  1886, -3634,  1706,  1405,
    2298,   153,  2274,  2380,  2390,  2396,  2770,   124, -3634, -3634,
    1388, -3634, -3634, -3634, -3634, -3634,  5321, -3634, -3634, -3634,
   -3634,  2398, -3634, -3634, -3634, -3634, -3634,  2406,  2404, -3634,
   -3634, -3634, -3634,  2411, -3634,  5028,  2414,  2413, -3634, -3634,
    1272, -3634,  1886,  1383, -3634, -3634,  2728,  2481,  2480,   121,
   -3634,  2473, -3634,    68,   678,  2189,  4206,  4206,  4206,  4206,
     135,  4206,  4206,  4206,  4206,  4206,  2591,  4206,  4206,    76,
   -3634,  2482,   780, -3634,  2484,  2569,  2539,  3618, -3634, -3634,
    2606,  2567, -3634,  2619,  4206,  4206,  4206,  4206,  2548, -3634,
   -3634,  2581,  4206,  4206, -3634,  2607,  4206,    82, -3634,  2555,
    2608,  2737,   691, -3634, -3634, -3634, -3634, -3634, 10822,  2525,
   -3634, -3634, -3634, -3634, -3634, -3634, -3634, -3634, -3634, -3634,
   -3634, -3634, -3634, -3634, -3634, -3634, -3634, -3634, -3634, -3634,
   -3634, -3634, -3634, -3634, -3634, -3634, 13366, -3634, -3634, -3634,
   -3634, -3634, -3634, -3634, -3634, -3634, -3634, -3634, -3634, -3634,
   -3634, -3634, -3634, -3634, -3634, -3634, -3634, -3634, -3634, 16619,
   -3634, -3634, -3634, -3634, -3634,  2518,  7006, 13366, 13366, -3634,
   -3634, -3634,   467, -3634, -3634, -3634, -3634,  2508,  2512, -3634,
   -3634, -3634, -3634, -3634, -3634, -3634, -3634, -3634, -3634, -3634,
   -3634,  2511,  2536,  2538, -3634,  2542,  1934,  1764, -3634, -3634,
   -3634, -3634, -3634, -3634,  1743, -3634, -3634, -3634,  2898, 11140,
    3630,  4206,  4206,  4206,  2648,  4206,  4206,  4206,  4206,  4206,
    4206,  4206,  4206,  4206,  4206,  2628,  4206,  4206,  2519,  2756,
    2632,  2523,  2718, 16789,  2549,  2540,  2642, -3634,  2546,  2788,
   -3634, -3634, -3634,  1533, -3634, -3634, -3634,  1533, -3634,  2530,
    2741, -3634, -3634,  1615,  2692,    66,  3217,  2805,   669,  2807,
    2808,  2809, -3634, -3634,  2810,  2811, -3634, -3634,  2812,   681,
   -3634, -3634,  1334,  2744,  2745,  2668,  2557, 10186,  2579,  2579,
   -3634,  2723,  2825, -3634, -3634,  2583,  1430,   111,   628, -3634,
    1486,  2570,  1258,  2571, -3634,  3877,  2582, -3634,  2585,  2690,
    2595,  4206,  4206,  4206,  4206,   345, -3634, -3634,  4206,  4206,
     659,  2081,  1635, -3634, -3634, -3634, -3634,  1774,  2565,  2572,
    2598,  2789, -3634,  2790,  2790, -3634,  3528,  1918,  3386,  2153,
    1918, -3634, -3634, -3634, -3634, -3634, -3634, -3634,  2596, -3634,
   -3634, -3634, -3634, -3634,   986, -3634,  2599,   582,  2686, -3634,
   -3634,  2765, -3634, -3634,   688,  2719,  2710, -3634,    53,  2870,
   -3634,   472,  2119,  1297,   290, -3634, -3634, -3634, -3634, -3634,
    2516, -3634, -3634, -3634,  2629,  2629,   698,  1793,  2873, -3634,
     218, -3634, -3634, -3634, -3634,  2671, -3634,  1886,  1396, -3634,
    2545, -3634,  2671, -3634,  2551, -3634,  1197,  2947,   767,  2552,
   -3634,  1399,  3905, -3634,  1886,  1886,  1886, -3634,  1886, -3634,
   -3634,  1618,  5989,  2562, -3634, -3634,  1272,   979,  1886,  1272,
   -3634, -3634, -3634, -3634, -3634, -3634, -3634, -3634, -3634, -3634,
   -3634,  1886, -3634,  1886,  1886,  1886,  1886,  1886,  1886,  1886,
    1886,  1886,  1886,  1886,  1886,  1886,  1886,  1886,  1886,  1886,
    1886,  1886,  1886, -3634, -3634, -3634,  1197, -3634,  2165, -3634,
    4056,  1134, -3634,   151, -3634,  2009, -3634, 18186, -3634, -3634,
   -3634,  2612, -3634,  1323, -3634,  2614, -3634,   107, -3634, -3634,
    2643, -3634,  2633,  2287,  2639, -3634, -3634,   389,  2630,  2635,
    2638, -3634,  2871,  2640,   438, -3634,   461,  2879,  2886,  2641,
    1146,  2880,  1210,  3053,  1192,  1524,  2890,   944,  4206,   163,
     667,  2644,  2645,   734, -3634,  4206,  4206,  2884,  2906,  4206,
    4206,  2650,  2738,   683,  4206,  2676,   584,  2872,   869,  4206,
   -3634,  2752, -3634,  4206,  2665,  2544,  4206,  4206,  2763, -3634,
   -3634, -3634, -3634,  2637,  9550,  2778,   104, -3634,  9550, -3634,
   -3634, -3634, -3634,  2688, 16619,  4206,  2813, -3634,  1328, -3634,
   -3634,   329,  1337, -3634, -3634, -3634,  2674,  2651,  2651, -3634,
   -3634,  7324,  2732, 13048, 13048, 13048, 13048, 13048,  6051,  9550,
   14476,  4005,  1168,   443,  2838,  1665,  2885, -3634,  2874,  2695,
   -3634,  1665,  1852,  4206, -3634, -3634, -3634, -3634,  4206, -3634,
   -3634, -3634,  2939, -3634, -3634,  2940, -3634, -3634, -3634,  4206,
   -3634,  2685, -3634,  2782,  2897,  2899,  4206,  2827, 17329, 11458,
    4206,  1918,  2705,  2549,  4206,  4206,  4206,  4206,  9550, 17329,
    2706,  4206,  7642,  1918,  1898,  4382,  9550,  4206,  3693,  1041,
    2709,  1337,  2708,  2711,  2712,  2713,  2714,  2724,  2725,  2726,
    2729,  2731,  2516, -3634, -3634,  1297,  1041, -3634, -3634,   955,
   -3634, -3634,  2733, -3634, 16990, -3634, -3634, -3634,  2734, -3634,
   -3634, -3634, -3634, -3634,  4206, -3634, -3634, -3634, -3634, -3634,
   -3634, -3634, -3634, -3634, -3634, -3634, -3634,  2735, -3634,  2532,
    1800,  2739,  4206,  4206, -3634,  2549, -3634, -3634, -3634, -3634,
   -3634,   235,  4206,   254, -3634, 17329, -3634,  2699,  2715, -3634,
   -3634, -3634,  1045, -3634, -3634, -3634,  1361, -3634, -3634, -3634,
   -3634,  2114, -3634,  2847, -3634, -3634, -3634, -3634, -3634, -3634,
   -3634, -3634,  2834, -3634,  2855,  2602,  2860, -3634, -3634,  2891,
      86,  2861,  1546,  2865,  2887,  1954,  3314,  4751,  2923,  3877,
    4206,  4206,   330, -3634, -3634,  2846, -3634, -3634,  9550,  9550,
    1282, -3634,  2507, 13048,  1257, 13048,  2751,   331,   607,   178,
   13048, 13048, 13048, 13048, 13048, 13048, 13684, 13684, 13684, 13684,
   13684, 13684, 13684, 13684, 13684, 13684, 13684, 13684,  3314,  2707,
    2883,  2981, -3634, -3634, 11140,  1664,  2941,   664, -3634,  4206,
    1665,   693,  1597, -3634,  4206, 16619,  3019,  4206,  4206, -3634,
    2888,   413, -3634,  2780, -3634,  1670, -3634,  2781,  2922, -3634,
   -3634, -3634,  2869,  2869, -3634, -3634, -3634, -3634,  2783,  2767,
   -3634,   103, -3634, -3634,   756,  2484,  2845,  2766, -3634, -3634,
   -3634, -3634,  2889,  2768,  2572,  4206,  2775, -3634, -3634,   688,
    2927, -3634, 13048,  3030, -3634, -3634,  7960,  9550, -3634, -3634,
   -3634,  2119,  2912,    97,  2710,  3988,  2924,   698, -3634, -3634,
    2900, -3634, -3634,  2946,  1119,  2785, -3634,  1385, -3634, -3634,
   -3634, -3634,   187,  1886, -3634,  1197, -3634, -3634,  2742,   899,
    1417, -3634, -3634,  2740,  1733,  1821,  1914, -3634,  5897,  2055,
    1886, -3634, -3634,  1960, -3634,  2319, -3634, -3634, -3634, -3634,
   -3634,  1706,  1706,  1405,  1405,  2298,  2298,  2298,  2298,   153,
     153,  2274,  2380,  2390,  2396,  2770,   128, -3634, -3634, -3634,
   -3634, -3634, -3634,  2743,  2747, -3634,  2746,  2009,  6072,  1271,
   -3634, -3634,  2792,  2815,  2816, -3634, -3634,  2814, -3634, -3634,
    2342, -3634, -3634,  2802,  2804,  2928,  2930,  4206, -3634,  2821,
   -3634,  2904,  2822, -3634,  2806,  3056,   142, -3634, -3634, -3634,
   -3634,  2820, -3634, -3634, -3634, -3634, -3634,  2918,    58,   527,
     584, -3634,  2826, -3634,  2901,  2823,   575,  2824,   240,  2908,
     127,  2989,  2971,  1102,  2974,  2995,  3005,  1268,   981,  4206,
    2849,  3009,  3014, -3634, -3634, -3634, -3634, -3634,  2910,  2856,
    2857,  2915,  4206,  3118, -3634, -3634,  2951, -3634, -3634, -3634,
   -3634, -3634,  3024,  2864,  2993,   819, -3634, -3634,  2867,   538,
     580,    91,  1204,    62,   887,  2868,  2875,  3131,  2877,  3132,
    2878, -3634, -3634, -3634, -3634,   458,  2893,   122,  2892,  2894,
   -3634,  3072,  2903,  3077,  2984,  2895,  2896,   118,  2911,  1096,
   -3634, -3634, -3634, -3634, -3634, -3634,  2907,  2996,  3098, 14952,
     584,  1796,  4206,  3021,  2913,  3151,   719,  2917,  4206, 15189,
    2997,  1823,   155, -3634,   155, -3634,  2914,  3157,  2585,  2920,
   -3634, -3634,  4206,  3110,  2585,  4206, -3634,  3143,  3036, 13048,
     862, -3634, 13048,  3145, -3634,  3186, 16619, -3634, -3634,  2945,
   -3634,  2585,  1898, -3634,  1498, -3634, -3634,  9550,  1337, -3634,
   -3634, -3634, -3634,  2916,  2932,  2916, -3634,  1810, -3634,  1934,
    1764,  1764, -3634, -3634,  8278,  9550,  3092,  2956,  1834, -3634,
    2966, -3634, -3634, -3634, -3634,  2970, -3634,  2511,  2972,  1909,
    1665,  3063,  1104,  2975, -3634, -3634,  1297,  1104, -3634, -3634,
    2949,  2977, -3634, 15426, -3634,  3064, -3634,  3877,  1636,  3161,
   13048, -3634, 14002, -3634, -3634,  3200, -3634, -3634, -3634, -3634,
    2957, -3634,  4206,  2958,  2959,  2985,  3097, 16990,  9550, -3634,
    3074,  3189,  3076,  1454, -3634, -3634,  3221,  3078, -3634,   134,
    2584, -3634,  2980, -3634,  3102, 17191, -3634,  2988,   -15, -3634,
    2990,  3103,  1486,  2991,  1704,  3159,  2945, -3634,  2994,  2998,
     708,  4206, -3634, -3634, -3634, -3634, -3634, -3634, -3634, -3634,
   -3634, -3634, -3634,  2999,  1041,  2119,  1297,  3000,  3001,  3002,
    2516,  3003,  3093,  3205, -3634, -3634, -3634,  3004,  3206,  2525,
   -3634, -3634,  1264,  8596, 16619,  3007,  2629,  3192, -3634,  3006,
   -3634, 16619,  3011,  1615,  1337,  3010,  3012, -3634,  3023, -3634,
   -3634,  1636,  3229, -3634, -3634, -3634, -3634, -3634,  3120, -3634,
   -3634, -3634, -3634, -3634, -3634, -3634, -3634,  3122, -3634, -3634,
   -3634, -3634, -3634, -3634, -3634, -3634, -3634, -3634, -3634, -3634,
   -3634,  3123, -3634, -3634, -3634, -3634, -3634, -3634, -3634, -3634,
   -3634, -3634,  4206,  3134, -3634,  3031, -3634, -3634, -3634,  3877,
     594, -3634,  1665,  2838,  2838, -3634,  3033, -3634,  2825, -3634,
   -3634, -3634,  3190, -3634,  3235, -3634, 13048, 13048,  3286,   841,
   -3634, -3634, -3634,  1113,  1150, -3634, -3634, -3634, -3634, -3634,
   -3634,  1155, -3634, -3634, -3634, -3634, -3634, -3634, -3634, -3634,
   -3634,  7006, -3634, -3634, -3634, -3634, -3634, -3634, -3634, -3634,
    3035, -3634, -3634, -3634, -3634, -3634, -3634, -3634, -3634, -3634,
   -3634, -3634, -3634,  3031,  4206,  3227,  4206,  3117,  3216, -3634,
   -3634,     0, -3634, -3634, -3634, -3634, 15663,  3877,  3187, -3634,
    3187,  3187,  3218, -3634, -3634,  1838, -3634,  3049, -3634, -3634,
    3288,  4206,  3225, -3634,  3050,  3034,  3195,  3292,  1823,  3038,
   -3634, -3634,  3040, -3634,  3020,   482,   -14,    74,  1649,  3236,
    2783,  3069,  2066, -3634,  3025,  3065, -3634,  1593, 13048,  3099,
    1312, -3634, -3634, -3634,  1123,   916, -3634, -3634,  1867, -3634,
    1090, 16619,  2549,  4206,   919, -3634,  4206, -3634, -3634,  3223,
    3230, -3634,   259, -3634, -3634, -3634, -3634, -3634,  1886,  1215,
    1418,  1036, -3634,  2105,  2105,  2105, -3634, -3634,  1886, -3634,
    1886, -3634, -3634, -3634, -3634,  3008, -3634,  3015, -3634, -3634,
   -3634, -3634, -3634, -3634, -3634,  1337,  3085, -3634, -3634,  3061,
    3071,  3197,  3324,  3075,  3188,  3198, -3634,  3079,  3082, -3634,
    3086, -3634, -3634, -3634,  1565, -3634, -3634, -3634,  1583,  1588,
    3087, -3634, -3634,  3211,  1654,   233,  1802,  3094, -3634,  1338,
   -3634,  4206,  4206,  3171,  3111, -3634,  1315,  1796,  4206,  4206,
    1187, -3634,  3266,  4206,  3210,  4206,  3280,  3116, 16619,  3271,
    4206,  4206,  4206, -3634, 16619,  4206,  4206,  4206,  3014, -3634,
    4206,  4206, -3634,  4206, -3634, -3634,  4206,  2381,  4206, -3634,
    4206,   819, -3634,  4206, -3634, -3634,  2392, -3634,    14, -3634,
      27,  3100,    27,  3100,  3172,  3101, -3634,  3104,  3095,  1860,
     301,  3105,  3165, -3634, -3634,  4206,  3114, -3634,  3214,  3243,
   -3634,  3121,  2920, -3634,  3124, -3634,  3124, -3634, -3634, -3634,
    1416,  3127, -3634, -3634, 13048, 13048,  3125,  4206, -3634, -3634,
   -3634, -3634,  3115,  3128,  4206,  3129, -3634,  1350,  3133,  3244,
    4206,  3248,  3138,  3119, -3634,  4206,  4206,  1445,  4206,  1868,
   -3634, -3634, -3634, -3634,  3135,  3311,  4206, -3634, -3634, -3634,
   -3634, -3634,  1881, -3634, -3634, -3634,   954,  3400,  2997, -3634,
    3279,  3281,   155, -3634,   155,  2920,  4206,  1067,  3141,  1305,
   -3634,  3277,  1507,  1138,  3408,  3410,   455,  3254, 13048, -3634,
    3036, -3634,  3371, -3634, -3634,  4206, -3634,  3168,  4206,  3417,
    1486,  2813,  3178, -3634, -3634, -3634, -3634, -3634, -3634,  3180,
    1882,  1913,  3191, -3634,  3231,  3256,  3182, -3634,  8914,  9550,
   14476,  3064,  3877,   726, -3634,  1104,  2119,  1297, -3634,  4206,
   -3634, -3634,  9550,  3353,   635,  1337,  3174,  3175,  3193,  4206,
    3064, -3634, -3634,  1985, -3634,  1807, -3634,  3368,  3370, 16619,
    3293,  3179,  3401,  3102, 13048,  3189,  3074, 17329,  3402, -3634,
    4206, -3634, 17329,  9550,  3199,  4206,  4206,  3241, -3634, 13048,
   -3634, 17329,   170,  3369, -3634,  4206, -3634, 11140,  9550, 16619,
    4206, -3634,  4206, -3634,  3202, -3634, -3634, -3634,  2119, -3634,
   -3634, -3634,  3203,  9550,  3934, -3634,  3093, -3634,  4206, -3634,
    3208,  2616, -3634,  1915, -3634, -3634,  1702, -3634,  3380,   588,
   -3634,  3222,    48, -3634,  1506,  3215, -3634, -3634,  2651,  2651,
    1667,  3193,  1615, -3634, -3634, -3634,  3316,  3314,  3314,  3234,
   -3634, -3634,   639,  1665,  2985,  1615, -3634, 13048,  3412,  3466,
   13048, -3634, -3634, -3634, -3634, -3634, -3634, -3634, -3634, -3634,
   -3634, -3634, -3634,  1925, 16619,  3418,  3314, -3634, -3634,  1472,
    3421,  3201,  3373, 16619, -3634,   820, -3634,  3226,  3233,  1597,
   -3634, -3634, -3634, -3634,  3877, -3634, 16619,   472,  3449,  2838,
     421, -3634, -3634,  4206,  3246,  4206,  3378,  3232, -3634, -3634,
    4206,  3238, -3634, -3634,   676,  3023,  1636,  9550, -3634, -3634,
   -3634, -3634, -3634, -3634, -3634, -3634, 13048, -3634, -3634,  3298,
    3065,  9550,  9550,  9550,  1499, -3634,  4206,  3395, -3634, 16619,
     698,  4206,  3493, -3634, -3634, -3634,  3384, 16619, -3634,  1221,
   -3634, -3634,  1981,  2105,  2099,  1310,  1799,  1419, -3634, -3634,
    3575, -3634, -3634, -3634, -3634,  3253,  2802, -3634, -3634,  3249,
    3251, -3634,  3252,  3504, -3634,  3260, -3634, -3634, -3634,  3250,
   -3634,  3255, -3634,  3258, -3634, -3634, -3634, -3634, -3634, -3634,
   -3634, -3634, -3634, -3634, -3634, -3634,   460, -3634, -3634, -3634,
    1021,  3275, -3634, 15189,  9550,  3451,  3383,  3453,  3283, -3634,
    3322,  3354,   996, 16619, -3634, -3634,  3294, -3634,  2940, 16619,
   -3634, -3634,  3289,   521,  3378,  3399,  3403,  3404, -3634,  3389,
   -3634, -3634, -3634, -3634, -3634, -3634, -3634, -3634, -3634, -3634,
   -3634, -3634,    45,  3447, -3634,  4206, -3634, -3634,  3297,  3100,
   -3634, -3634,  3100,  3285, -3634, -3634, -3634, -3634, -3634,  3388,
    3100, -3634,  3306, -3634,  3303, -3634,  3302, -3634,  3299, -3634,
     958,  3305, -3634,  3307, -3634,   779,  3300, -3634, -3634, -3634,
   -3634, -3634, -3634,  3296, -3634,  3471, -3634,  3308, -3634,  3301,
    3309,  3310,  4206, -3634,  4206,  3312, -3634,    47, -3634,  3315,
    3317,  2585, -3634, -3634, -3634, -3634,  3492,  1445,  3318,  3325,
    3361, 16619,  4206,  3313,  3326,  1067, 15189,  3567, 16619,  1168,
   -3634,  3484,  3485,   155,   303,  1108,  2997,   465, -3634, -3634,
    3319,  3329,  2920, -3634,    36, -3634, -3634, 13048,  4206, -3634,
    3413,  3321,  3327,  1168,  4206,  3425,   -20,  3534, -3634, -3634,
   -3634,  3349, 16619, -3634,  3352, 11140, -3634, -3634,  3231,  3231,
    3092,  3491,  3355,  3092,  3535,  3231, 16619, -3634,  1936,  3356,
    2511,  3358,  1640,  3353,   635, -3634, -3634, -3634,  2119,  3360,
   -3634, -3634, -3634, -3634, -3634,  2651,  2651,  1673, -3634, -3634,
    3357, -3634, 14239,  3338,  3584, -3634,  1615,  1615, -3634,  1944,
    3366,  1258,  4206, -3634, -3634,  3102,  3564, -3634, 17329,  3599,
    3359, 16990, -3634, -3634,  1949, -3634,  3539, 13048,  3375,  3159,
     963, 17329, 17329,  9550,  9550,  3571, -3634, -3634,  3374,  3387,
    3556,  3377,  1775,  3381, -3634,  3379, -3634, -3634,  3390,  3102,
    3102,  3532, -3634, -3634, -3634, -3634,  2018, -3634, 16619,  1168,
    3542,  3405,  1225,  9550,  3393,  3391,  3392, 16619,  3612,  2916,
    2916,  1337,  3396,  3398, -3634,  3314,  1039, -3634, 16619,  3494,
    3498, 14713,  3499, -3634,  1168,  3501,  3595, -3634, -3634, 13048,
   13048, -3634, -3634, -3634,  3407,  3314,   108, -3634, -3634,  1904,
    1904,  1337,  3414,  4041,  3064, -3634, -3634, -3634,  1968, -3634,
    3411, 15663,  3353, 11776, 16619,  1787, -3634,  3423,  3483,  3602,
    3406,  1359,  3489,  3540, -3634,  3429,  2997, -3634, -3634,  3426,
   -3634, -3634,  1951,  3193,  1975,  1593, -3634,  1700, -3634, -3634,
   -3634, -3634, -3634, -3634,    48,  2549, -3634, 17329,  1927,  3621,
    3432, -3634, -3634,  3223,  3347, -3634,  2105,  2105,  2148,  2105,
    2156,  1962,  2105,  2316, -3634, -3634, -3634, -3634,  3598, -3634,
   -3634, -3634, -3634, -3634, -3634, -3634, -3634,  3606,  4206,  2997,
   -3634,  4206, -3634,  3442,  3438,  3445,  3454,    65,  3283, 16619,
    3560,  4206,  3458,  3527,  3700, 16619,  1352, 16619, -3634,  1593,
    2002,  4206,  3464,  3468, -3634, 16619,  4206,  4206,  3448,  3511,
   -3634, -3634, -3634, -3634,  3461,  3459, -3634, -3634,  3463,  3460,
    4206,  4206, -3634, -3634, -3634,   765, -3634, -3634,  3124,  3124,
   -3634, -3634, -3634, -3634,  4206, -3634,   446,   515, -3634,  3544,
   -3634, -3634,  4206,  3601,  3462, -3634,  3604, -3634,  3722,  3603,
    3488,  3492,  3585, -3634, -3634,  3475, -3634,  3529, -3634,  3389,
   -3634, 16619,  2997, -3634,  3495,  2029, -3634, -3634, -3634, -3634,
   -3634,  1108, -3634, -3634, -3634, -3634, -3634,  3474, -3634,   680,
    3478,  2537, -3634,  3479,  3480, -3634, -3634, -3634,  3696,  1674,
   -3634, -3634,   343,  3588,  2710, -3634,  4206, -3634,  3608, -3634,
   -3634, -3634,  1104,  3698,  3092,  3092, -3634,  3760,  3671,  3506,
   -3634,  3531,  3092, -3634, -3634,  3524, -3634,  9232, 16619, -3634,
   -3634, -3634, -3634,  2916,  2916,  1337,  3513,  3514,  4206, -3634,
    3769, -3634, -3634, -3634, -3634, 16619, 12094,  3530, -3634,  3770,
    3537, -3634,  3735,  4206, -3634,  4206,  3543, 13048,  4206,  6370,
   17329,  3102,  3102,  3718, -3634,  4102,  3421,  3421, -3634, 16619,
   -3634, -3634, -3634, -3634, -3634,  4206,  2616, -3634, -3634,  1558,
    2516,  4206, 16619, -3634, -3634, -3634,  9550, -3634,  2060,  3546,
    1699,  2549, -3634, -3634, -3634,  2651,  2651,  1039,  3728, -3634,
   -3634,  2067,  3551, -3634,  2069,  3553, -3634,  4041,  4206, -3634,
   -3634, 16619,   108,  3519,  2651,  2651, -3634,  1576,  3554,  3555,
    1168,  3550,  1597,  3557, -3634, -3634,   695,   404,  3547, 16619,
    4206, -3634, -3634, -3634, -3634, -3634,  9550, -3634,  3662, -3634,
     421, -3634,  3541, -3634,   688,  3673, 16619, -3634,  4206,  3730,
    9550, -3634, -3634, -3634, -3634,  4206, 16990, -3634, -3634,  3563,
   -3634,  1710,  3776,   698, -3634, -3634, -3634, -3634,  2105, -3634,
    2105,  2105,  2272, -3634, -3634, -3634, -3634, -3634, -3634, -3634,
   -3634,  3681, -3634,  3378,  2073, -3634,  3720,  2997, -3634, 16619,
    3560,  3560,  3560,  2075,  3565,  3680,  3354,  4206,  3590, -3634,
    3591,  2077,  3808,  3746,  3597,  3811,  2098, -3634,  2940, 16619,
    3771, 13048, 13048, -3634, -3634, -3634,  4206,  3576,  3578,  4206,
    3695, -3634,  3605,  3607, -3634,  3742, -3634, -3634,  3307, -3634,
    3589,  3592, -3634,  3839, -3634,  4206,  1168,  3594,  3361,  3822,
    3615,  2101, -3634, -3634,  2117,  3855,  1320,  3714, -3634,   123,
    3878,  2048,  3620,   737, -3634, -3634, -3634, 13048,  3619,  4206,
    4206,  3625,  3463, -3634,   343, -3634,  2125, -3634, -3634,  3767,
    3645,  4117,  3421, -3634, -3634, 13048,  3882,  3627,  1719,  3791,
   -3634, 16619, -3634,  2118,  2511, -3634, -3634, -3634,  2651,  2651,
   -3634, -3634, -3634, -3634,  2124, -3634, -3634, -3634,  3641, -3634,
    4206, -3634,  2121, 13048,  3688,  3646,  1555,  2135,   170,   170,
    3648,  3650,  3655, -3634,  3064,  3064, -3634, -3634,  3651, -3634,
    3726,  1701,  4206,  2136, -3634, -3634, -3634,  1548,  1260, 16619,
   -3634,  2916,  2916, -3634,  3617, -3634, 16619, 12094,  1325, 12094,
    3661, -3634,  1923,  3668, -3634,  3519, -3634, -3634,  2916,  2916,
    1904,  1904,  1337,  2399,  4206,  3670,  4041,  3836,  1168,  4206,
    4206,  4206,  2464,  3906,  3756,  9550,  3856,    89,  3653, 13048,
   -3634, -3634, -3634,  4206,  3762, -3634,  3797,  4206,  2138, -3634,
    3705,  3682, -3634,  1593, -3634,  3904,  1927, -3634, -3634,  3948,
    1858,  2549, -3634, -3634, -3634, -3634,  2105,  3709, -3634, -3634,
   -3634,  4206,  4206,  3710,  2142,  3680,  3680,  3680, -3634,  3702,
    3783, -3634,  3527,  2146, -3634,  3754, 16619,  1320,  3800,  3725,
    3732, -3634, 13048,  2940, -3634,  1593,  3865,  2152, -3634, -3634,
    2154, -3634, -3634,  3915, -3634,   100,  3715, -3634,  3713, -3634,
    4206, -3634,  3884,  4206, -3634,  2710, -3634,  3529, 13048,    56,
   -3634, -3634,  1549,  3741,  1168,  4206, -3634,  1322, -3634,  3982,
     281, -3634,  1104, -3634, -3634,  3719, -3634,   515, -3634, -3634,
    1959, -3634, -3634, -3634,  3605, -3634, -3634,    27,  3100, -3634,
   -3634,  3650,  3193,  3064, -3634,  3740, -3634, 13048,  1176,  3747,
   -3634, -3634,  3889,  3983, -3634, -3634, -3634,  2916,  2916, -3634,
   12094, -3634,  3739, -3634,  3688, 13048,  3159, -3634,   832, -3634,
   -3634, -3634,  3731,   404,   404, -3634,  3986,  3753,  4206, -3634,
    3755, 16619,  3734,  3748,  3787, -3634, -3634,  3790,  3792, -3634,
   -3634, -3634, -3634,  2162, -3634,  1168,  2164,  3553,  9550, 12412,
    3064, -3634, -3634, -3634,  2651,  2651, -3634,  3793,  3789,  3795,
   -3634,  1747,  3786,  2464,  1597,  4041,  3798,  1665,  2187, -3634,
     535,  2838,  3801,  3774,  1665, -3634, -3634,  9550,  9550, -3634,
    4028,  3972,  3653,  3957,  3912, -3634, -3634,   688,  3758, -3634,
    3799,  3806,  3945,  3977,  3560, -3634, -3634, -3634, -3634,  4206,
    3895, -3634, -3634, -3634, 13048, -3634,  3843,  3951, -3634,  3941,
   -3634, -3634, -3634,  2543,  1438, -3634, -3634,  4206,  4206,  3850,
    2188, -3634,  1336,  4060, -3634, -3634,  3820, -3634, -3634,  4206,
    3171,  3993, 13048,  3995,  3992,  4206,  4206,  3810, -3634, -3634,
    3807, -3634, -3634, -3634, -3634,  1696, -3634,   138, -3634,  3812,
    3813,  3829,  3971, -3634, -3634, -3634, -3634,  3837,   286, -3634,
   -3634,  2171, -3634, -3634,  4061, -3634,  3832, -3634,   404, 13048,
    3065, -3634,   933, -3634,   711, -3634,  2488, -3634,  4073, 13048,
   -3634, -3634, -3634, -3634,  3159, -3634,  4206, -3634,  3856,  3856,
    4206, -3634,  3834, -3634, -3634,  3844,  3845, -3634,  3944, -3634,
   -3634, -3634, -3634,  3933, -3634,  2190, -3634, -3634,  3353,  2916,
    2916,  9550,  1665, 14476,  3827,  1665,  2040,  1665, -3634,  1665,
    4206,  4206, -3634,  2464,  3825,  3899,  3852, -3634, -3634, -3634,
    9868,  9550, -3634, -3634, -3634, -3634, -3634,  3797,  3830,  3195,
    3960,  4206,  2197, -3634,  3831,  1438, -3634, -3634,  3857, -3634,
    2199,  4206,  4206,  2200, -3634,  4206, -3634, -3634, -3634,  3840,
   -3634,  4206, -3634,  1438, -3634, -3634, -3634,  4206, -3634, 16619,
    3710,  3858, -3634,  3529, -3634,  3860, -3634,  3861,  3838,  3959,
    3863, -3634, -3634,  9550,  3961,  4012, -3634,  4031,  1593, -3634,
   -3634,  3867,  3868, -3634,  4206,  2201, -3634,  1320,  3447, -3634,
    2185, -3634, -3634, -3634, -3634,    27,  3856, -3634,  4119, -3634,
    3849, -3634, -3634, 13048,  1622,  2209, -3634,  4206,  3869,  3972,
    3972, -3634,  4206, -3634, -3634, -3634,    -8,  3933, -3634, -3634,
   12412, -3634, -3634, -3634,  2210, -3634, -3634,  1754,  3875, -3634,
    9550, -3634, -3634, -3634,  2838,  1665,  3879,   112, -3634, -3634,
   15902,  3881,  3883,  3885, -3634, -3634,  3876, -3634,  3824, -3634,
   -3634, -3634, -3634,  4206,  2213, -3634,  4032, -3634,  3945, -3634,
   -3634,  9550,  3529, -3634,  2997, -3634,  3951,  2985, -3634, -3634,
   -3634, -3634, -3634, -3634,  1320, -3634,  3865,  3865, -3634,  4206,
    4206,  3890,  1593, -3634,  1593,  3560, -3634, -3634,  1223, -3634,
    3971,  1379, -3634, -3634, -3634,  3972,  1283, -3634,  2220, -3634,
   -3634, -3634, 13048,  3886, -3634, -3634, -3634, -3634,  3862,  3866,
    3999, -3634, -3634, -3634, 14476,  1665, -3634, -3634, -3634, 12730,
   16141,  3825,  4070, 16619, -3634,  4083, -3634, 13048, 13048, 10504,
    9868, -3634, -3634,  4002, -3634,  3960,  4206, -3634,  3901, -3634,
    2997, -3634,  4048,  1410,  3171,  3171,  3896, -3634, -3634,  3560,
    3560,  3680,   150, -3634, -3634, -3634, -3634, -3634,   574, -3634,
    2491, -3634, -3634, -3634, -3634, -3634,  1326, -3634,  3908, -3634,
    3909,  3910, 16619, -3634, -3634,  3913,  3911,  2229, -3634, 15902,
    2242, -3634, -3634,  2243,  6688, -3634,  2259, -3634, -3634, -3634,
   -3634,  4044, -3634,  3950, -3634,  1273, -3634,  3710,  3914,  3917,
    4206,  3680,  3680, -3634,  3529,  4026, 13048, -3634, -3634, -3634,
    3234,  4037, -3634,  1665,  1665,  3918,  1639, -3634, -3634, 16619,
    4100, -3634, 13048, -3634, -3634, -3634, 10504,  4206,  4206, -3634,
      80,   442,   442, -3634,  3865,  3865,  4065, -3634, -3634, -3634,
    3928,  2277,  4020, 15663, -3634, -3634, -3634,   760,   760,  2278,
   -3634,  1665, -3634, -3634,  3930,  3931, -3634, -3634, -3634,  4027,
   -3634, -3634, -3634,  3925, -3634, -3634, -3634,  3171,  3171,  4206,
   13048,  3529,  3935,   820, -3634, -3634, -3634,  1639, -3634, 16380,
   -3634,   152, -3634,  3932,  3949, -3634,  2279, -3634, 12094,  3353,
   -3634, 16619, -3634,  2280, -3634,  4191, -3634, -3634,  4057, 13048,
   -3634, -3634,  3529,  2312,  4165,  2329, -3634, -3634, 16380,  2159,
    3955,  2331, -3634,  3064,  3064, -3634, -3634, 16619, -3634,  1639,
   -3634, -3634, -3634,  3964,  2395,  2330, -3634, 13048, -3634, -3634,
    3353, -3634,  2337, -3634,  1639,  1639,  1639,  1639,  1639,  2339,
   -3634, -3634,  1639,  2395,  2330,  2330, -3634, -3634, -3634, -3634
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
   -3634, -3634,  4258, -3634, -3634, -3634, -3634,   -99, -3634,  -295,
    1943,  1955,  1782,  1945,  3481,  3477,  3482,  3476,  3490,  3487,
    -105,  -284, -3634,  -332,  -277,    99,  1604, -3634, -3634,  3973,
   -3634,   180,  -197, -3634, -3634, -3634, -3634,  4197,  1833, -3634,
   -3634, -3634,  3051, -3634,  4185,  3892,     2,    31, -3634, -3634,
      13,    85, -3634, -3634,   -63, -3634,  3500,  -216,  -700,  -706,
    -697, -3634,  -361, -3634,  1556,  3919, -3634, -3634, -3634, -3634,
   -3634, -3634, -3634, -3634,  4217,    69,  3970,   307, -3634, -3634,
   -3634, -3634, -3634, -3634, -3634, -3634, -3634,   -42, -3634, -3634,
   -3634, -3634, -3634, -3634, -3634,  3628, -3634, -3634, -3634, -3634,
   -3634,  3622, -3634,  3707, -3634, -3634, -3634, -3634, -3634, -3634,
   -3634,  3147, -3634, -3634, -3634, -3634,  3219, -3634, -3634, -3634,
   -3634, -3634, -3634, -3634, -3634, -3634, -3634, -3634, -3634, -3634,
   -3634, -3634,  3027, -3634, -3634, -3634,  2006, -3634, -3634,  2431,
    2483, -3634, -3634,   519, -3634, -3634,   630, -2068, -3634,  4063,
    4071, -3634, -3634,  4259,   999,  4274,   -81,  1140, -3634, -3634,
    3692,   -96, -3634, -3634, -3634, -3634,  1554, -3634,  3502, -3634,
   -3634,  3503,  3516,  -765, -3634, -3634, -3634, -3634, -3634, -3634,
   -3634,  4204,  3240, -1137,  2170,  1446, -3634, -3634, -3634, -2475,
    1200, -3634, -3634, -3634, -3634, -3634,  1284,  1280,  2408, -3634,
   -3634, -1902, -2319, -3634, -3634, -3634, -3634, -1944, -3634, -3634,
    2405, -3634,  1617, -3634, -3634, -3634, -3634, -3634, -3634, -3634,
   -3634, -3634, -3634,   826, -3634, -3634, -3634,   614, -3634, -3634,
   -3634,  3496,    83, -3634, -3634, -3634,  1587, -3634, -3634,  2465,
   -3634,  1162, -3634,   828,  1392, -3634,   982, -3314, -3634,  1190,
   -3634, -2682,  -405, -3634,  1324, -2289, -3634, -3634,  3017, -3634,
   -3634, -3634, -3634,  2973, -3634, -3634, -3634, -2864, -3634, -3634,
   -3634,   735, -3634, -3085,   860, -3634,   843, -3634, -1894,  2636,
   -3634, -3634,   707, -2953,   976, -1359, -1386, -2345,   736, -2022,
   -3112, -3159, -3634,  1965, -1291, -3059, -3634,  2080, -1392,  1978,
    1646, -3634, -3634,   971, -3634, -3634, -2024, -3634, -3634, -2174,
      50, -3634,  -773, -2201,   975, -3634, -1081, -2362,  1373, -3634,
   -3634, -3634,  -816, -1390, -3634,  2196, -1238, -3634, -3634, -3634,
   -3634,  4306,  2401, -3634, -3634, -3634, -3634, -3634, -3634, -3634,
    -524,  3497,    15,    17, -3634,  2764, -3072,  1132, -3634,  1098,
   -1440, -1650, -3634,    19,  1226, -3634, -3634, -3634, -3634, -3634,
   -3634, -3634,   599,  1820, -2502, -3634,  1515,  1256, -3634,   829,
   -3634, -3634, -3634,  1859, -2178,  1278, -3634, -3634,   833,  3446,
   -3634, -3634, -1681, -2710, -3634,  -943,   732, -1462, -3634,  -955,
    3467,  2418,  2881,  2394, -3634,  -876,   715,  2240, -3634,   950,
   -3634, -3634,  3330,  3751, -3634,  -103, -2669, -1458, -1127, -3634,
   -3634, -3634,  2943,  -969, -2705, -3634, -2796,   -94, -3634, -3634,
    2701, -3634, -3177, -3634, -3634,   743, -3634, -3634, -3634,   590,
   -3634, -3634,   689, -3634, -3502, -3634, -3634,   561, -3634, -3634,
   -2447, -3634, -3634, -3633, -3634, -3634,   536, -1183, -2700,  -976,
   -3634, -3634,   977, -3634, -1156,  1004, -2593,   757,   666, -3634,
   -3634,   673,   674, -3634,   771, -3634, -2188, -3116,  1128, -3634,
    1339,  1131,  -703, -2481, -2075, -3634, -1195, -3634,  -747, -1236,
   -3634, -1813,  2258, -2606, -3634, -3634,  1568,   978, -2054,  -122,
   -3634,  3409,  3415,  3880, -3634, -3634, -3634, -3634, -3634, -3634,
   -3634, -3634, -3634, -3634, -3634, -3634, -3634, -3634, -3634, -3634,
   -3634, -3634,  -127,  1451,  -155, -3634,  3593,  1832,  1835,  1263,
   -3634, -1502, -3634, -3634,  3062,  2485,  3068, -3634,  3623, -1073,
   -3634, -3634,   809, -2224, -3634, -3634,  1055, -3634, -3634,  1058,
   -3634,   827,  -957,  -406, -3634,  2486,  -603, -3634, -3634, -3634,
   -3634, -3634, -3634, -3634, -1233,  -670, -3634,  2266, -3634, -1050,
    1872, -2053,  -981,  -995, -3634, -1012, -2623,  -593, -1003, -3634,
   -1692,  1647, -3634, -2576, -3634,  -622, -3634,  3486, -3634, -3634,
   -3634, -3634, -3634, -1150, -3634, -3634, -3634,  1208, -1524, -3634,
   -3634, -3634,  2397, -3634, -2070,  -971, -1036, -3634, -3634, -3634,
   -3634, -3634, -3634,  -343, -3634, -2020, -3634,  2992, -3634,  2434,
   -3634, -3634,  2437, -3634, -1586, -3634, -3634, -3634,  1231, -3634,
   -3634, -3634, -3634, -3634, -3634, -3634, -3634, -3634, -3634, -3634,
   -3634, -3634, -3634, -3634, -3634, -3634, -3634,  3947,  3140, -1355,
   -3634, -3634, -3634, -3634,  3136, -3634,  2126,  2133, -3634, -3634,
   -3634, -3634, -3634, -3634, -3634, -3634, -3634, -3634, -3634, -3634,
   -3634,  -950,  2108, -2339,  2163,  1106, -3634,  2109, -3634,  2127,
   -3634, -3634, -2382, -3634, -3634, -1893,  1831, -3634, -3634, -3634,
   -3634, -3634, -3634, -3634, -3634,  1907, -3634, -3634,  1386, -3634,
   -3634, -3634, -3634, -3634, -3634, -3634, -3634, -3634, -3634, -3634,
   -3634, -3634, -2344, -3634, -3634, -3634, -3634, -3634, -3634,  1516,
   -3634,  1303, -3634,  1122, -3634, -3634,  1270, -3634,  2799, -3634,
    -620,  3424, -3634, -3634, -1618, -3634, -2515, -3634, -3634, -2751,
    -912, -3634,  -931, -1505, -3634, -2667, -3438, -3634, -1348, -3418,
   -3634,   923, -1194, -3634
};

/* YYTABLE[YYPACT[STATE-NUM]].  What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule which
   number is the opposite.  If zero, do what YYDEFACT says.
   If YYTABLE_NINF, syntax error.  */
#define YYTABLE_NINF -2066
static const yytype_int16 yytable[] =
{
     584,   787,  1084,  1823,  1517,   301,  1640,  1802,  2085,  1597,
    2087,  1528,  1561,  1254,  1637,  2290,  1836,  1547,  1635,  2160,
    1509,  1510,  1603,  1104,  1842,   422,  2480,  1600,  2411,  2211,
    1051,   421,  2531,   791,  2632,  2097,  1792,  1577,  2542,  1943,
    2636,  1773,   801,  2201,   675,  1605,  1515,  2533,  1595,   670,
    1251,  2511,  2869,  2132,   599,   105,  1833,  1778,   622,  2763,
    2543,   114,   771,   662,   663,   664,  2551,   107,   426,    69,
    1503,  2412,   680,  1266,   975,   685,    84,  2045,  2420,  2991,
    2103,  2564,   784,   413,   689,  2107,  2053,  1925,  2057,  2748,
    2766,   822,   678,  2766,  2065,  1257,  2578,  1457,   132,  3270,
    1612,  2766,  1624,  2435,  2601,  3373,  2700,  3376,  2806,  2835,
    1845,  2079,  2788,  2563,   285,  2054,   286,  2114,   287,  2995,
     301,   342,    69,   301,    69,   357,  2940,  2061,    69,  2940,
    1410,  3031,   120,   734,  2954,  2955,    69,  2249,  3486,   108,
    3082,  2827,  3110,  3047,  2157,  3452,  3604,  1327,  2832,  2300,
    1324,  3182,   135,    22,  3153,   859,    22,   781,   803,    22,
    3440,  3441,  3442,  2486,   122,  3715,  3245,  3246,  3247,   440,
    1242,  2953,  2761,   741,  1106,  1981,   623,  2287,   719,    22,
      61,  2972,  3713,    22,   840,   901,  1921,  2613,   840,   144,
      22,  2662,    69,  3748,   411,  2405,  1974,   419,   624,  3901,
     869,  3154,  3155,   352,   352,   135,   901,  2288,   441,  2847,
     764,   765,   766,   122,   830,   831,  3545,  1451,  3203, -1126,
    2402,  1785,  2241,    22,   401,   414,    22,  2554,   767,   285,
    3475,   286,   285,   287,   286,    61,   287,  3518,  3519,  1336,
      61,    61,   731,   366,   732,    61,   733,  2019,  2001,   804,
     805,   806,  1920,   864,   422,  1492,   370,    22,   891,   371,
     421,  3818,  1146,  1147,  3779,  1419,  2002,   793,  1921,  1898,
     372,   373,  3142,   799,   800,  2003,  2004,  2815,  3760,  1178,
    1480,  1481,  3151,  1738,   366,  2792,  1739,   421,   341,  3844,
    3793,  3937,   865,  3486,    61,  1623,  2856,   370,    22,  2860,
     371,  1375,  1899,  1107,  1695,    61,    61,    70,  3902,  2082,
    1420,   372,   373,  3465,   902,  2145,   757,  1179,   758,  1905,
     759,  2582,  2583,  1454,  1108,  1814,  3973,   422,  1421,  2402,
    1982,  2987,  2490,   421,  1464,   902,  1643,  3845,   695,  3938,
    1928,   696,  1625,   935,  2155,  2491, -1109,   357,  3466,  3989,
     887,   888,   889,   890,   780,   893,   894,  3893,   896,   897,
    1815,  2274,    70,  2005,   413,  1920,  1696,  3749,  1906,   934,
    1921,  2848,  2349,   892,  1900,  2242,  3675,  2020,   915,   916,
     917,   918,  3636,   366,  3023,  1109,  2406,   191,  1975,  2403,
    3205,  1586,  1649,  3066,   145,  2350,   370,    22,   736,   371,
    3394,  1422,   737,  1653,  1423,  3131,  3879,  3323, -1060,  2614,
     372,   373,   442, -1060,    69,  1051,  2739,  1740,  3475,  3873,
    2762,    69,  2793,   625,   443,  1276,  1051,   852,  1243,   675,
    2167,   411,  1051,   789,   352,   626,  1730,  1983,   853,  1731,
     352,  1110,  3192,   870,   981,  1363,  1929,  1540,  3072,  1456,
     871,   903,   627,  1922,  1364,  3903,  1280,   925,  2542,   802,
     801,  1051,  1284,  1285,  1286,  2208,   801,  3085,  1137,  3761,
    3838,  3839,  1835,  3927,  1468,   411,  1295,  3185,  2584,  1653,
    2870,  3580,  1337,  2351,  2531,  1318,   771,  2402,  3321,  1338,
   -1061,  3094,  2402,  2146,    69, -1061,   866,  2006,  3300,   135,
    1269,  3846,  1051,  3939,  3952,   401,   986,   987,   867,   989,
     990,   991,   992,   993,   994,  3731,   996,  1901,   998,  2213,
    2214,  1650,  1717,  1294,  2896,   620,  1345,  1252,  1298,  1299,
    1300,  2633,   341,  3203,  1902,  3100,    53,  1297,  1424,  1508,
     675,  1635,  1741,    54,  3041,  2888,  1761,  1761,  1761,  1761,
    1761,  1761,  1761,  1761,  1761,  1761,  1761,  1761,  1317,  1635,
    3575,  2081,    83,   416,   841,   117,    53,  2090,  2091,  1870,
    1732,  2101,  3486,   850,  1326,   851,  1283,  2084,  1268,  2913,
    2914,  2812,  1346,  1327,  2019,  1274,    61,  2542,  1324,   832,
     833,  -950,  3490,  3923,  3924,  1199,  1200,  2133,  1202,  3546,
    2967,  3517,  3783,  2105,  2665,   341,    53,  2694,  2352,    53,
    3084, -1143,  2491,    54,  3101,  1712,    54,  3425,   229,   807,
    3473,   808,  2515,   809,  3486,  1654,  1551,   111,   777,   738,
    3113,  3114,  2185,    81,  2752,  1655,  1225,  1656,  3120,  1227,
     374,  1959,   621,   951,  1908,  3204,   375,   376,   377,   378,
     379,   380,  2444,  3070,  2670,  3432,  3488,  3102,  2683,  3458,
    2492,  3628,  2534,   770,  2723,  1253,  2724,  1624,  2053,  2828,
    3902,  2829,   422,  2408,  3372,  3384,  3385,   952,   421,  1353,
     139,   374,  3243,  1624,   140,  2270,  2183,   375,   376,   377,
     378,   379,   380,  2483,  2305,  1733,  1734,  1203,  1355,  1997,
   -2065,   562, -1143,  2310,   770,  3205,  3843,   175,  1347,  3304,
    3306,   567,  1735,   568,  2504,  1736,  2806,  1204,  3103,  2828,
    1362,  2829,   953,  2197,   421,   421,   421,   421,   421,   421,
     421,   421,   421,   421,   421,   421,   421,   421,   421,   421,
     421,   421,   421,  2212,  1277,  3301,   422,   735,  1551,  1229,
     631,  1230,   421,  1231,  3308,  1547,  3877,  3878,  1186,  3264,
    1759,  1759,  1759,  1759,  1759,  1759,  1759,  1759,  1759,  1759,
    1759,  1759,  1801,  1425,  1426,  1709,  1365,  1125,  2630,  2271,
     374,  2753,  1970,  1427,  1366,  3218,   375,   376,   377,   378,
     379,   380,   366,  2303,  2233,  1909,  2671,  1187,  2816,   632,
    1550,   790,  1418,  3071,  1319,   370,    22,   675,   371,  1258,
    1443,   675,  1488,  1354,  1428,  1289,  1495,  3903,  1458,   372,
     373,  1429,  2844,   190,  1972,  1367,  1451,  1478,  3244,  1502,
    2725,  1484,  2610,  1998,   675,  2830,  1356,   681, -2065,   747,
    2831,   675,   675,  2106,  3144,  3156,  1737,  1529,  2531,  3649,
    3650,  3847,   105,  1452,  1579,   411,  3943,  2272,  2726,  1186,
     953,  3791,  1027,   853,   810,   811,   812,   813,   814,   815,
     816,   817,   818,   819,  1502,  1291,  1180,   802,  1296,   891,
    1259,  3148,  3149,  2172,   132,  1910,  3068,  1135,   747,  1126,
    2831,   675,  1564,  3881,  3551,   675,  1593,  1911,  1187,   675,
    1529,  2112,  1453,  1569,  1593,   905,   747,  1027,  1551,  1368,
    1575,  2537,   749,  1971,  1856,   411,   714,  1321,  2677,    69,
      82,  1602,    69,  2168,   366,  3841,  3842,  1127,  2920,  1136,
    1584,  1786,  1454,   929,  1782,  1455,   156,   370,    22,  1916,
     371,  2828,  1601,  2829,  3640,  1542,  2221,  1860,   750,  2222,
    1917,   372,   373,  2946,  2325,  1973,  3489,  3926,   633,  1369,
    1430,   749,  1748,  2611,   185,   186,   187,  3735,  3493,  2828,
     634,  2829,  1437,  2596,  3457,   166,  2188,   635,   636,   749,
    1431,  1432,   341,   637,  1051,  1236,  3951,  3639,  1128,  2037,
    2631,   638,  1787,  2898,  1816,  1866,  1855,   750,  1724,  2766,
    1728,  1457,  1051,  1181,   366,  1742,  1743,  1744,  1745,  1746,
    1747,  1209,  3725,  1492,  3979,   750,   760,   370,    22,  1438,
     371,   675,   675,  1964,   906,  1965, -1669,  2779,  2866,   981,
    2590,   372,   373,  2196,  3784,  3785,  2754,  1463,  1137,  1703,
    2682,  3199,  2431,   136,  1713,  1714,  3200,  2892,  1464,   872,
    1787,  1917,  1877,   178,  3201,  3091,    89,  3661,  1456,  1439,
    2163,   366,  1783,  1237,  1627,  2669,  1628,  2695,  1629,   930,
    2626,  2798,  2732,  2304,   370,    22,  3092,   371,  2535,  2808,
    2223,  3640,  1465,  1775,  2045,   931,  2112,  1827,   372,   373,
    3344,  1466,   873,  1792,  2038,   156,   932,  2053,   235,  2646,
    3353,  3354,  1749,  1750,  2529,  2484,  1944,   565,   682,  1440,
    2593,  1945,  3307,  2524,  2932,  2933,  3789,  2039,  3790,   675,
     675,   185,   186,   187,  1831,  1832,   156,  1489,  2603,  1752,
    1753,  1985,  1754,  1755,   166,  2834,  2778,  2579,  3359,  1821,
     700,  2528,  2575,  2588,  3629,   571,   572,   573,   574,   575,
     576,   577,   578,   579,   580,  2311,  3891, -1667,  1986,   366,
    2105,    90,  2572,  2591,  2105,   166,  2568,  1412,   422,  2103,
    1083,  2571,   370,    22,   421,   371,  3825,  2661,  2598,   941,
    2581,  1467,  1413,  1966,  2544,  2209,   372,   373,  1468,   374,
    1024,   421,  3907,  3908,  1946,   375,   376,   377,   378,   379,
     380,  1438,   358,   359,  3318,  1503,   876,   360,   361,   362,
    1279,  3501,   178,   235,   363,   364,   235,  3891,  2224,   365,
    1469,  2225,   366,  1027,   422,   367,  1661,  1920,  2477,  1947,
     421,   368,  1933,  1857,   369,   370,    22,  1414,   371,   237,
     366,  1439,    92,   178,  2635,    93,  1464,  1398,   753,   372,
     373,  2696,  1399,   370,    22,  2227,   371,  1547,  2228,  1847,
    2230,  1400,  1987,  2231,   820,  1875,  2297,   372,   373,  3498,
    2744,  2773,  2774,  3825,   780,   460,  1024,  3986,  3987,  3195,
    1465,  2727,  1362,    94,  3732,  1988,  2619,  2620,    91,  1466,
    1363,  1440,  3515,  1948,  1934,   881,   366,    52,  1848,  1364,
    2298,  1289,   461,  3503,  1662,  2478,  1029,  2938,  3504,   370,
      22,    22,   371, -1521,  2899,  1935,   969,   175,  1936,  1415,
    1394,   374,   970,   372,   373,   288,  3538,   375,   376,   377,
     378,   379,   380,  2775,  2070,   366,  1395,  2073,  1365,  1502,
    2728,    69,  1859,  2871,   411,  1396,  1366,  2745,   370,    22,
     179,   371,  2226,  1401,   237,   292,  1937,   237,     3,     4,
       5,  2701,   372,   373,     6,     7,  2033,     9,  3125,  3126,
      10,    11,   754,  3497,    13,    14,  1849,   293,    16,  1467,
     675,    18,    19,    20,    21,  2083,  1468,  1367,  2853,  2229,
      22,    23,  1029,  1402,  2232,  3285,  2208,   675,   675,  2080,
    2702,   374,  1292,  1502,  2538,   981,   156,   375,   376,   377,
     378,   379,   380,  3870,    24,    25,    26,    27,    28,    29,
      30,    31,    32,    33,    34,    35,    36,    37,    38,    39,
      40,    41,    42,   190,  1725,   661,  1027,  2364,  1375,  1138,
     288,  1186,   661,   288,   679,   166,   683,  1726,  3944,   688,
     690,   675,  3871,  1912,  3501,  1542,  1593,   713,   374,  3502,
    2372,  3745,  3746,  2542,   375,   376,   377,   378,   379,   380,
     292,  1368,  3225,   292,  2499,  1977,  2923,   747,  2703,  2316,
    1187,  1942,  2365,  3792,  3946,  1333,  2185,   141,  2937,  3980,
    1466,  -418,   293,  3322,  1978,   293,  3651,  3374,  2294,  3042,
    2357,   175,  2318,  2319,  2320,  3850,   748,  2366,  2952,  1792,
    3971,  3365,  2887,  2500,  3500,  1139,   675,  3396,  1979,  1542,
    2924,  1369,  2369,   178,  1547,  1375,  2312,  2314,    95,  2317,
    3872,  2367,  1635,  3782,   179,  1635,    96,    97,  2373,   690,
    1727,    98,  2728,  1912,  1051,  1635,  3503,  3796,  1547,  1051,
     749,  3504,  1051,  3043,  3044,  3366,   374,  2449,  1051,  1325,
    1502,  3797,   375,   376,   377,   378,   379,   380,   771,  2368,
     430,  2218,  2219,  1334,  1497,  2421,   690,   690,   690,  1980,
    1663,   137,  2624,  3851,  2321,  1376,   750,   431,  2358,  1377,
    3045,  1182,  2982,  1027,   690,  2637,  2918,  1664,   768,  1518,
    1537,  2490,  2901,  2902,    59,  1665,  3107,  1151,   138,  2450,
    1666,  2900,  1152,  3483,  2491,   968,  2983,   190,  2925,   374,
    1153,   124,  3336,  2181,   675,   375,   376,   377,   378,   379,
     380,  1154,  1183,  2873,  2874,   742,   143,   374,  2313,  3295,
     381,  3295,   121,   375,   376,   377,   378,   379,   380,  1667,
     185,   186,   187,  3503,  1547,  3295,   142,  1720,  3798,    59,
     743,  1668,  2492,  1398,   115,   116,  3369,  3370,  1399,  2292,
     124,  1721,  3882,  1827,  3485,  3239,  1542,  1400,   747,  1547,
    1506,   336,   415,  3382,  3383,  1507,  2531,  2293,  3600,  1246,
    2934,  3734,  1247,   374,  3530,  3296,  1876,  3296,  3295,   375,
     376,   377,   378,   379,   380,  2287,  2463,  2464, -2065,  3589,
    3590,  3296,    85,  1155,    86,   886,   690,   690,   690,   690,
    2956,   690,   690,   895,   690,   690,  2996,   899,   900,  3295,
     953,  3795,   374,  2707,  3739,  2288,  1669,   911,   375,   376,
     377,   378,   379,   380,   690,   690,   690,   690,  3216, -1668,
    2306,   749,   921,   922,  3296,  2465,   924,  3591,  2480,  2347,
    3516,  1610,  3837,   707,  3740,  2322,  1788,  2092,   343,  1401,
    3231,   421,  3202,    53,  2436,  1789,  1553,   708,  1626,  1156,
      54, -1369,  1698,  1790,   344,  3296,  2437,   750,  2438,   939,
    1699,  2439,   777,  1157,  1158,  1159,  1756,  1756,  1756,  1756,
    1756,  1756,  1756,  1756,  1756,  1756,  1756,  1756,  1779,  1402,
     430,  1403,  1852,  2140,   337,   842,  2141,  1780,  3613,   943,
     949,   950,    87,  1270,   366,  1794,   842,   860,  1791,  1853,
    3738,   843,  3510,  3511,    88,   826,   827,   370,    22,  1271,
     371,  1748,  1282,  2647,   842,   842,   842,  2648,  3145,  2441,
    2442,   372,   373,  2649,  2542,  2650,  -549,  2616,  1643,  3929,
    1861,  2315,  2711,  2682,  1635,  2617,  2498,  2399,   690,   982,
     984,   985,   690,   690,  1917,   690,   690,   690,   690,   690,
     690,   995,   690,   997,   690,   146,  1000,  1001,  3614,  1097,
    3141,  1098,  2575,  1080,  1091,  2542,  1261,  3615,  3616,  3969,
    3970,   338,  2575,  1099,  3362,  3479,  3363,  3480,  1262,  1643,
    3123,   366,  3617,  2508,  2553,  2854,  2855,   340,  3364,  3481,
   -1667,  3585,  2862,  2339,   370,    22,  2554,   371,  3647,  2461,
    2462,  3165,   411,   411,   411,  2340,  2882,  2883,   372,   373,
    1363,  2341,  3147,  3227,  3127,  3224,  2343,  2647,   339,  1364,
    3618,  2648,  2705,  2342,  3228,   690,  1788,  3190,  2344,  3191,
    1635,   690,   690,  1201,   690,  1789,  3033,  2050,  1207,  1208,
     356,   675,   675,  1790,  3046,  1051,  2051,  1529,  1051,  1579,
     432,  1749,  1750,  2708,  2710,   675,   562,   366,  1051,  1051,
    2540,  -603,   420,  2976,  1579,  3887,   567,  3888,   568,  2868,
     370,    22,   981,   371,  2418,  1547,  1212,  2115,  1752,  1753,
    1213,  1754,  1755,  2419,   372,   373,   675,  2116,  1791,  2117,
    1654,  1593,  1804,  3171,  3172,  1643,   434,  1805,   565,   566,
    1655,   675,  1656,  3099,  3662,  3663,  1529,  1554,  2621,   565,
     566,  1287,  3188,  3189,  2875,   842,   675,  3472,  2622,  -549,
    2623,  1529,  2616,  1643,  2876,  2099,  2877,   969,  3169,   435,
    3358,  2608,  2638,  2159, -1667,  2641,   571,   572,   573,   574,
     575,   576,   577,   578,   579,   580,   436,   571,   572,   573,
     574,   575,   576,   577,   578,   579,   580,  2918,  2992,  2028,
    3342,  3430,  1788, -1521,   968,  2993,   969,  1702,   968,  2028,
    3431,  1789,   970,  2002,  1051,  -603,  3543,  3330,  2575,  1790,
     358,   359,  2003,  3754,  3331,   360,   361,   362,  2634,   966,
    3390,  1547,   363,   364,   967,  2997,  2076,   365,  2099,  3378,
     366,  1827,  2998,   367,  2909,   823,   128,   437,   129,   368,
     824,   825,   369,   370,    22,  1214,   371,   641,   642,  1215,
     675,  1643,   643,   969,  1791,  2685,  1863,   372,   373,  1644,
     842, -1676,  2088,   438,   675,   675,   675,  1507,   690,  2689,
    2690,  2691,  2353,  3158,  2354,  1442,   690,  2355,   439,  1446,
    1447,  3567,   776,  2111,   690,   366,  2095,   463,   776,  1476,
    2255,  1507,  1537,   690,   444,  2256,  1483,   690,   370,    22,
    1320,   371, -1521,   968,    22,   969,  3552,  3553,   703,  1788,
     704,  1562,   372,   373,  1498,  1501,  3337,  3338,  1789,  2295,
    2470,   374,  2709,  1547,  2296,  2471,  1790,   375,   376,   377,
     378,   379,   380,  2475,  2519,   465,   366,   675,  2476,  1507,
    1539,   690,  2735,  1547,  1864,  1556,  3670,   462,   842,   370,
      22,  1556,   371,  1563,   705,   492,   706,  1670,   690, -1522,
    -603,  3386,  2099,   372,   373,  2520,   597,  2607,  2100,   690,
    1507,  1791,  1507,   666,  2184,  2647,   690,  2642,  1080,  2648,
    1583,  2191,  1507,  1091,  1588,  1589,  1591,  1592,  2864,  1080,
     964,  1598,   965,  1507,   665,  1604,  2884,  1501,  1609,   876,
    1051,  2885,  1051,   877,   684,   878,  2893,  1051,   374,  3170,
    1246,  2894,  1051,  1247,   375,   376,   377,   378,   379,   380,
    2968,  2353,   879,   691,  1080,  2969,  2355,  2989,   565,   566,
     834,   835,  2990,  1671,  1588, -1522,  -603,  1865,  2099,  1547,
     692,   842,  2838,   880,  2552,  3006,  3007,   693,  3009,  1642,
    1672,  3013,  1646,  1647,  3048,  1091,   828,   829,  1673,  3049,
     981,  1748,  1652,  1674,   694,  1080,   571,   572,   573,   574,
     575,   576,   577,   578,   579,   580,  2809,  2916,  1675,  3012,
   -1665,  3087,   411,  1867,   374,  3011,  2885,  1868,   881,   698,
     375,   376,   377,   378,   379,   380,  2278,  2279,  2280,  2281,
    2282,  2283,  1676,  1051,  2704,   690,  1707,   661,   842,   690,
     690,   690,  3166,   882,  1677,   701,  2244,  3167,  2834,  3175,
    1547,  3178,  1579,  3303,  3176,  3240,  3176,  3248,  3548,  3258,
    3241,  1328,  3049,  1329,  2885,  3557,  1760,  1760,  1760,  1760,
    1760,  1760,  1760,  1760,  1760,  1760,  1760,  1760,  1707,   690,
    3263,  1210,  1211,  3291,   982,  3049,   675,   675,  3292,  1784,
    1556,  1593,  1593,   699,   688,  1498,  3648,  1798,  1799,  3293,
    3335,  2191,  3539,  3540,  2885,  1507,  3339,  1288,  2893,   851,
    3757,  3340,   883,  3343,  2949,  2950,   675,  3348,  3360,  1678,
    3421,  1529,  1507,  3361,  3439,  2885,   702,  2408,  3446,  2885,
    3316,  1749,  1750,  3447,  3461,   690,  3463,   709,  2974,  3462,
    3959,  3462,   884,   712,  3529,  3887,  3531,  3888,  1051,  3340,
    1639,  3340,  2706,   641,   642,  1840,   842,   374,  1752,  1753,
     711,  1754,  1755,   375,   376,   377,   378,   379,   380,  3549,
    3598,  2834,  3659,   710,  3550,  3599,  3630,  3660,   381,  3697,
     121,  3702,  3705,  3729,  3698,  2834,  3462,  3706,  3730,  2963,
    3733,  3741,  3753,   565,   566,  3774,  3742,  1507,  2380,   715,
    3775,  3008,  3801,  3665,  2385,   842,  3669,  2286,  3671,  3010,
    3672,  3858,   792,   842,  3664,  2394,  3859,  2395,   375,   376,
     377,   378,   379,   380,  3861,  3863,  2400,  3743,  2401,  3862,
    3862,   571,   572,   573,   574,   575,   576,   577,   578,   579,
     580,  3865,  1305,  1306,  1307,  1308,  3866,  1891,  1762,  1763,
    1764,  1765,  1766,  1767,  1768,  1769,  1770,  1771,  1772,  3911,
    3916,  3942,  3947,   798,  3462,  3917,  3462,  3948,   728,   375,
     376,   377,   378,   379,   380,   722,   729,  3233,   726,  3234,
    3235,     2,   539,  1639,     3,     4,     5,  1341,  1342,   690,
       6,     7,     8,     9,  3953,   730,    10,    11,    12,  3340,
      13,    14,  1957,    15,    16,   739,    17,    18,    19,    20,
      21,  3956,   745,  3968,  1082,  3977,  3957,    23,  3462,  3981,
    3978,  3988,   740,   547,  3982,  3236,  3462,   723,   761,   842,
     727,   549,  1480,  1481,  1930,  3387,  3758,  1938,  1722,  1723,
      24,    25,    26,    27,    28,    29,    30,    31,    32,    33,
      34,    35,    36,    37,    38,    39,    40,    41,    42,  1498,
    2530,  3136,   690,  3014,  3015,  3016,  3017,  3018,  2041,  1498,
     675,  2828,  1579,  2829,  1869,  1682,   842,  3587,   762,  3588,
     763,  3975,  2062,  3976,   773,  2066,  1531,  1882,  1883,  2558,
    2147,  2148,  3402,  3403,  3641,  3642,  2077,  3848,  3849,  1301,
    1302,   783,   675,   565,  3388,   411,   411,   346,   411,  1309,
    1310,   411,   786,  1303,  1304,   539,  3809,  3030,  3032,  2184,
    3905,  3906,  3984,  3985,  2252,  2253,  1520,  1521,   785,   675,
    1083,  1522,  1523,   371,  1529,  3349,  3350,   788,   836,   794,
    1556,   571,   572,   573,   574,   575,   576,   577,   578,   579,
     580,  1683,   795,  1498,   796,   839,   547,   690,   837,   847,
     982,   848,  2123,  1532,   549,   838,   849,   856,  1684,   846,
     857,   861,  2126,   862,   863,  1685,  1686,  1080,   868,   675,
    1774,  1687,  3183,  2963,  3212,   898,   908,   904,   909,   907,
     912,   913,   914,   675,  2643,  1080,  1688,   919,  3223,   920,
     926,   923,   927,  2657,   928,  3433,   938,   944,   959,   957,
    3892,  2164,  1689,   958,  3884,  3885,  1794,   961,   962,   963,
    1690,   988,   999,  1003,  1002,  1004,  3269,  3269,  1005,  1006,
    1533,  1093,  1691,  1092,  1095,  1102,   565,   566,  1094,  1103,
    1105,  3914,  3915,  1124,  2186,  1129,  1130,  1131,  1132,  1133,
    1134,  2194,  3918,   565,   566,  1140,  1141,  1142,  1143,  2191,
    1145,  1148,  1149,  1150,  1185,  1189,  1195,  2558,  1197,  1194,
    1216,  3892,  3309,  1217,   571,   572,   573,   574,   575,   576,
     577,   578,   579,   580,  1198,  1219,  1221,  1220,  1228,  1234,
    3324,   571,   572,   573,   574,   575,   576,   577,   578,   579,
     580,  1235,  2206,  3892,  1232,  1244,  1239,  1692,  1240,   690,
    1255,  1278,  1556,  3892,  1265,  1290,  1273,  1332,  1579,  1335,
     565,   566,  1275,  2028,  1339,  1281,  1344,  1340,  3892,  3892,
    3892,  3892,  3892,  2746,  1534,  1348,  3892,  1359,  1351,  2749,
    1349,  2605,  1536,  1350,  1360,  1352,  1361,  1374,  1411,  1435,
    1436,  1444,  3136,  1445,  3136,  1448,  1450,  1459,   571,   572,
     573,   574,   575,   576,   577,   578,   579,   580,  1462,  1477,
    1479,  1486,  1485,  1489,  2235,  1511,  2237,  1496,  1502,  1518,
    1512,  1551,  1560,  1557,  3415,  1558,  2248,   690,  1565,  1567,
    1570,  1572,   445,  1573,  1576,  1574,   971,  1585,  1596,  1659,
    1613,  2259,  1611,  1614,  1615,  1616,  1617,   411,   675,   411,
     411,   565,   566,  3409,  3375,  1660,  1618,  1619,  1620,   346,
    1679,  1621,   121,  1622,  1631,   446,  1636,  1638,  1680,  1681,
    1645,  2028,  2963,  1693,  1709,  1697,  2028,  3456,  2558,  1700,
    1694,  2194,  1091,  1588,   945,  1729,   690,  1776,  1777,   571,
     572,   573,   574,   575,   576,   577,   578,   579,   580,  1781,
    1797,  1701,  1806,  3474,  1803,  1807,  1800,  1809,  1813,  1817,
    1812,  1181,  2851,  1819,   447,  1822,   448,  1828,  1818,  1824,
    1834,  1083,  1846,  1841,  1878,  1378,  2863,  1851,  1379,  1380,
    1893,   449,   450,  1862,  1858,  1881,  1871,  1879,  1880,   451,
    1872,  1873,  1827,  1885,   452,   872,  1889,  1381,  1890,  1897,
    1914,  1896,  2530,   453,  2348,  3136,  1904,  1915,  1919,   454,
    1579,  2359,  2360,  1892,  1895,  1903,  1927,  1382,  2370,  2371,
    1383,  1913,  1931,  2375,  1932,  2377,  1384,  1939,  1498,  1940,
    2382,  2383,  2384,   455,  1498,  2386,  2387,  2388,  1941,  1949,
    2390,  2391,  1950,  2392,  3537,   456,  2393,  1951,  2397,  1953,
    2398,  1954,  1955,   690,  1956,  1958,  1402,  1960,  2917,  1961,
     457,  1963,  1968,  1991,   458,  1993,  1995,  2929,  1385,   972,
    1992,   675,  1994,  1996,  2013,  2426,  3533,  2000,  2244,  2015,
    2025,  2244,  2014,  2016,  2026,  2034,  2017,  2011,  2036,  2012,
    2046,  2018,   565,   973,  2056,  2022,  2063,  2062,  2067,  3269,
     675,   675,  2024,  2068,  2447,  3558,  3559,  2040,  2035,  2055,
    2453,  2244,  2058,  1386,  2975,   690,   690,  2075,  2469,  2074,
     459,  2963,  2078,  2086,  2092,   411,  2474,  3269,  2094,   953,
     571,   572,   573,   574,   575,   576,   577,   578,   579,   580,
    2096,  -608,  2102,  2098,  2108,  1111,  2485,  2104,  2109,  2119,
    2112,  2124,  2125,  2127,  2128,  2131,  2129,  2142,  1387,  2134,
    2137,  1578,  2150,  2143,  3324,  2469,  1388,  1389,  1501,  2151,
    2154,  1180,  2156,  1019,  3644,  2158,  2161,  2162,  2174,  2178,
    2180,  2165,  2169,  2170,  2171,  1390,  2179,  2173,  2190,  2749,
    2532,  2187,   690,  1391,  2189,  2558,  2195,  2749,  2202,  2539,
    2200,  2198,  2207,  2199,  2203,  3053,  2204,  2205,  2208,  2550,
    2215,  2217,  2216,  2220,  2234,  2236,  2238,  2239,  2250,  1498,
    2257,  2260,  2254,  2258,  2261,  3688,  1204,  1080,  2263,  2265,
    2570,  2268,  1080,  2267,   675,  1604,  2576,  2276,  2269,  2580,
    2277,  1080,  2286,  2285,  2291,  2589,  2307,   982,  1392,  2592,
    1588,  2323,  2594,  2327,   675,  2330,  1112,  1113,  2309,  3689,
    2324,  2558,  2326,  2328,  1609,  2329,  2335,  2333,  1588,  2336,
    2332,  2606,  2337,  1114,  1115,  2346,  2338,  2345,  2356,  2361,
    1116,  1117,  2363,  2374,  2376,  2378,  1118,  2379,  2381,  2413,
    2416,  2414,  2408,   466,  2415,  1119,   467,  1707,  1707,  2423,
    2422,  1120,   468,  1556,  2427,  2428,   675,  1985,  1827,  3707,
    2445,  3721,  2452,  2430,  2457,  2443,  2432,  1121,  3124,  2440,
    2446,  2448,  2473,  2451,  2644,  3537,  1707,  2454,  2455,   469,
    2472,  2479,  2481,  2660,  2482,  3132,  2487,  1122,  2497,  2501,
     466,  2503,  2506,   467,   690,  2510,  1498,  2512,  2514,   468,
    2517,   470,  2518,  2673,  1704,  2675,  1123,  2524,  2522,  2917,
    2680,  2526,  2521,   675,  1246,  2545,  2546,   471,  3756,  2556,
    2547,  2557,  3162,  2560,  2561,  2562,  2569,  2577,   472,   473,
     474,  2573,  2609,  2587,  2625,  2597,  1840,  2595,  2639,  2194,
    2604,  1588,  2612,  2640,   675,   475,  2655,  1498,   470,  3778,
    2618,  3184,   476,   477,   478,  2628,  2645,  3644,   479,  2653,
    2663,   480,  2664,   481,  1226,  2656,  2668,   482,  2674,  2657,
    1466,  2687,  2678, -1107,  3811,  2693,   473,   474,  2681,  2697,
    2698,  2712,  3822,  3822,  3829,  3688,  2558,  2713,  2718,   483,
    2720,  2715,   475,  2716,  2717,  2721,  2731,   484,  2722,   476,
     477,   478,  2719,  1498,  2739,   479,  2736,  2737,  2738,   485,
     481,  2741,  2742,  1498,   482,  2747,  2751,  2755,  2758,  1498,
   -1107,  2756,  2757,  2402,  2765,  2767,  2768,  2769,   486,  2558,
    2770,  2771,  2492,  2782,  2778,   445,   483,  2784,  2799,  2772,
    2780,  2781,  2785,  1467,   484,  2764,  2805,  2811,  2814,  3265,
    2786,  2817,  2818,  2810,  2846,  2787,   485,  2791,   565,   566,
    2794,  3269,  2796,  2802,  2833,  2840,  2841,   675,   446,  2834,
    2849,  2850,  2842,  2852,  2857,   486,  2859,  3822,  2861,  2867,
    2865,  3829,  2872,  2880,   487,  2881,  2878,  2886,  2889,  2890,
    2895,  2891,  2789,  2897,  2790,  2903,   571,   572,   573,   574,
     575,   576,   577,   578,   579,   580,  2904,  2905,  1643,  2908,
    2915,  1498,   690,  2910,  2911,  2921,  1498,   447,  1498,   448,
    2931,  3334,  2912,  2927,  2942,  3269,  2922,  2926,  2943,  2945,
    2947,   487,  2928,  2948,   449,  1224,  2951,  2935,  2839,  2936,
    2970,  2957,   451,  3136,  2845,  2977,  2978,   452,  2980,  2984,
    2986,  2981,  1498,  2988,  3269,   982,   453,  3002,  2985,  2885,
    3005,  3020,   454,  3021,  3027,  2471,  1498,  3028,  3034,  3367,
    3038,     3,     4,     5,  3057,  3029,  2244,     6,     7,  3037,
       9,  3040,  3269,    10,    11,  3051,   455,    13,    14,  3052,
    3059,    16,  2532,  3056,    18,    19,    20,    21,   456,  3060,
    3063,  3073,  1588,  3076,    23,  3061,  3077,  2793,  1080,  3075,
    3078,  1080,  2792,   457,  3080,  3081,  3086,   458,  2728,  3089,
    3097,  1080,  1080,  3093,  3095,  3096,  3106,    24,    25,    26,
      27,    28,    29,    30,    31,    32,    33,    34,    35,    36,
      37,    38,    39,    40,    41,    42,  3109,  3115,  2919,  3111,
    3116,  3118,  3119,  3121,  3128,  3129,  3449,  2930,  2554,  3140,
    3138,  3143,  3137,  3150,  3186,  1707,  3168,   910,  2248,  3139,
    3174,  2248,  3177,   459,  3179,  3193,  3194,  3196,  3198,   983,
    3213,  3209,     3,     4,     5,  1707,  3215,  3217,     6,     7,
    3226,     9,  3220,  2966,    10,    11,  3230,  3237,    13,    14,
     366,  2248,    16,  3242,  1498,    18,    19,    20,    21,  3250,
    3249,  3256,  3257,   370,    22,    23,   371,  3259,  3260,  3261,
    3262,  3272,  3266,  3273,  3276,  3280,  3283,   372,   373,  3279,
    3281,  3288,  3278,  3282,  3286,  1091,  3294,  1080,    24,    25,
      26,    27,    28,    29,    30,    31,    32,    33,    34,    35,
      36,    37,    38,    39,    40,    41,    42,  3290,  3299,  3302,
    3305,  3162,   565,   566,  2006,  3313,  3319,  3320,  3022,  3327,
    3333,  3026,  3328,  3341,   565,   566,  3345,  2893,  3347,  1498,
    3351,  3036,  3352,  3355,  3356,  1498,  3371,  1498,  3377,  3379,
    3395,  3050,  3393,  3407,  3408,  1498,  3054,  3055,  3413,  3410,
     571,   572,   573,   574,   575,   576,   577,   578,   579,   580,
    2426,  3065,   571,   572,   573,   574,   575,   576,   577,   578,
     579,   580,  3417,  3418,  3069,  1607,  3422,  3423,  3426,  3428,
    3434,  3437,  3074,     3,     4,     5,  3443,   565,   566,     6,
       7,  3444,     9,  3448,  3453,    10,    11,  3454,  3459,    13,
      14,  1498,  3464,    16,  3455,  3468,    18,    19,    20,    21,
    3467,  3470,  3482,  3487,  3491,    22,    23,  3499,  3508,  3507,
    3509,  3513,  1031,  3520,  3525,   571,   572,   573,   574,   575,
     576,   577,   578,   579,   580,  3521,  3108,  3523,  3526,    24,
      25,    26,    27,    28,    29,    30,    31,    32,    33,    34,
      35,    36,    37,    38,    39,    40,    41,    42,  1498,  3527,
    3528, -1304,  -549,  3544,  3541,  3560,  3561,  3542,  3130,  3555,
    3547,  3564,  3554,  3204,  3568,  1498,  3569,  3570,  3571,  3574,
    3578,   600,  3581,  1604,  3582,  1604,  3585,   601,  1588,  3597,
    1080,  3601,  3602,  3666,  3605,  1604,  3607,  3608,  3612,  1498,
    3611,  3623,  3621,  3622,  3624,  1609,  3157,  2001,  3627,  3635,
    3643,  3161,  1498,  3652,   602,  3655,  3653,  3654,  3656,  3668,
    3676,  1091,  3679,  3680,  3693,  3691,  3699,  3719,  3701,  3714,
    3708,  3716,  3717,  3718,  3723,  3722,  3724,  2966,   690,  3726,
    3727,  1498,  3720,    54,  3737,  2599,  3736,  3755,   972,  3449,
    3759,  3744,  3767,  3770,  3768,  3771,  3769,  3804,  3776,  2660,
    3211,  3805,  3788,   603,   604,   605,  3806,  3815,  3803,  3819,
    3831,   565,   566,  3834,  3835,  3840,  1498,  3867,  3219,  -603,
     606,  3853,  3854,  3857,  3856,  1588,  1080,   607,   608,  3868,
    3886,  3874,  3880,   609,  3875,  3883,   610,  3894,  3909,  3910,
    3912,  3919,   611,  3920,  3940,  2958,  3928,  3921,   612,   571,
     572,   573,   574,   575,   576,   577,   578,   579,   580,  1498,
    3922,  3941,  3949,  3950,   613,  3954,  3967,  3255,   565,   566,
    3764,  3974,   614,   110,  1312,  1314,   355,  1311,   769,  1498,
    1313,   433,   858,  1854,   615,   845,  3271,   374,  1316,  3275,
    1315,   347,  2714,   375,   376,   377,   378,   379,   380,   616,
    1837,   797,  1223,   617,  1101,  3284,   571,   572,   573,   574,
     575,   576,   577,   578,   579,   580,   972,  1331,   234,  1233,
    1811,  1715,   565,   566,   755,  2959,   185,   186,   187,  2589,
    3312,  1888,   756,   236,  1188,  3088,   677,  1708,  2627,   565,
     973,  1604,  3314,  3173,  3807,  3381,  2417,  3064,  3315,  3832,
    3813,  1498,  2960,  3764,  2429,  3692,  3083,  1405,  1408,   618,
     571,   572,   573,   574,   575,   576,   577,   578,   579,   580,
    1588,  1409,  1460,  2389,  3445,   565,   566,   571,   572,   573,
     574,   575,   576,   577,   578,   579,   580,  3712,  3252,  3595,
    1962,  3435,  2469,  3777,  2032,  3700,  3710,  3794,  3606,  3368,
    3780,  2813,  3855,  2734,  2266,  2801,  2248,  3079,  3620,  3764,
    3619,  3287,  2666,   571,   572,   573,   574,   575,   576,   577,
     578,   579,   580,  3391,  3392,   619,  2966,  2505,  1541,  3397,
    3400,  3401,  3512,  1031,  2210,  3532,   565,   566,   850,  1322,
     851,  2971,  3913,  3416,  3210,    54,  3751,  3420,  2115,  3764,
    2944,   565,   566,  3752,  1606,  1630,  2513,  2667,  2116,  2536,
    2117,  1091,  1245,  2121,  3814,  2166,  2262,  3930,  3860,  3958,
    3983,  3026,  3436,  2244,   571,   572,   573,   574,   575,   576,
     577,   578,   579,   580,  1793,  3823,  1498,  3673,  3896,   571,
     572,   573,   574,   575,   576,   577,   578,   579,   580,  3897,
    3898,  3830,  3565,  3563,  2686,  3690,  3412,  1144,  3214,  3932,
    3469,  3802,  2071,  3471,  3637,  2509,  1519,  1718,  2072,  1494,
    3638,  3932,  2692,  3800,  1719,  3484,  2994,  2516,  3159,  3524,
    2567,  1648,  2566,  2602,  2139,  3514,  1096,  2007,  3932,     2,
     565,   566,     3,     4,     5,  2023,  2797,  3932,     6,     7,
       8,     9,  2795,  2776,    10,    11,    12,  3631,    13,    14,
    2836,    15,    16,  2837,    17,    18,    19,    20,    21,  3067,
    3004,  2820,  3427,  3495,  3311,    23,  3634,  2275,   571,   572,
     573,   574,   575,   576,   577,   578,   579,   580,  3522,  3781,
       0,  1498,     0,     0,     0,     0,     0,     0,    24,    25,
      26,    27,    28,    29,    30,    31,    32,    33,    34,    35,
      36,    37,    38,    39,    40,    41,    42,     0,  1820,     0,
       0,     0,     0,     0,     0,  2966,     0,  1556,     0,     0,
       0,     0,     0,     0,  1556,     0,     0,     0,     0,     0,
       0,     3,     4,     5,     0,     0,     0,     6,     7,     0,
       9,     0,     0,    10,    11,     0,     0,    13,    14,  3577,
       0,    16,     0,     0,    18,    19,    20,    21,     0,     0,
       0,     0,     0,     0,    23,     0,     0,  3255,  3596,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,  3603,
       0,     0,     0,     0,     0,  3609,  3610,    24,    25,    26,
      27,    28,    29,    30,    31,    32,    33,    34,    35,    36,
      37,    38,    39,    40,    41,    42,   565,   682,    43,    44,
      45,     0,     0,     0,     0,     0,    46,    47,    48,    49,
      50,    51,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,  1588,     0,     0,     0,
    2469,     0,     0,     0,   571,   572,   573,   574,   575,   576,
     577,   578,   579,   580,     0,     0,     0,     0,     0,     0,
       0,     0,  1556,  3667,     0,  1556,     0,  1556,     0,  1556,
    3400,  3674,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,  3696,     0,     0,     0,     0,     0,     0,     0,     0,
       0,  3703,  3704,     0,     0,   690,   644,   645,     0,     0,
       0,  3709,     0,     0,     0,     0,     0,  3711,     0,  1498,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   646,     0,     0,     0,     0,     0,
       0,   647,     0,     0,  3728,     0,     0,     0,   648,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     649,     0,     0,     0,     0,     0,     0,  1588,     0,     0,
       0,     0,  3747,   650,     0,     0,     0,     0,     0,     0,
     651,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,  1556,     0,     0,     0,     0,
    1498,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,  3773,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   652,     0,     0,   653,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   654,     0,     0,  3786,
    3787,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   655,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     121,   656,     0,     0,  3808,  1556,     0,     0,     0,     0,
    1498,     0,     0,  1498,     0,     3,     4,     5,     0,   657,
       0,     6,     7,     0,     9,     0,  3833,    10,    11,     0,
       0,    13,    14,     0,     0,    16,     0,     0,    18,    19,
      20,    21,   658,     0,     0,     0,     0,    22,    23,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,  1498,     0,     0,     0,     0,     0,     0,  1498,
       0,    24,    25,    26,    27,    28,    29,    30,    31,    32,
      33,    34,    35,    36,    37,    38,    39,    40,    41,    42,
    3876,     0,     0,     0,     0,     0,     0,     0,   348,     0,
       0,     0,     0,  1556,  1556,   565,   566,     0,     0,  1498,
       0,     0,     0,     0,     0,     0,     0,  3899,  3900,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,  2248,     0,     0,     0,     0,     0,     0,
       0,  1556,     0,   571,   572,   573,   574,   575,   576,   577,
     578,   579,   580,     0,     0,     0,     0,     0,     0,  3925,
       0,     0,     0,     0,     0,     0,     0,     0,     0,  1498,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,  1498,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,  1498,     0,
       0,     0,     0,     0,     0,     0,     0,  1498,     2,   358,
     359,     3,     4,     5,   360,   361,   362,     6,     7,     8,
       9,   363,   364,    10,    11,    12,   365,    13,    14,   366,
      15,    16,   367,    17,    18,    19,    20,    21,   368,     0,
       0,   369,   370,    22,    23,   371,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   372,   373,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    24,    25,    26,
      27,    28,    29,    30,    31,    32,    33,    34,    35,    36,
      37,    38,    39,    40,    41,    42,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    52,     2,   358,   359,     3,     4,
       5,   360,   361,   362,     6,     7,     8,     9,   363,   364,
      10,    11,    12,   365,    13,    14,   366,    15,    16,   367,
      17,    18,    19,    20,    21,   368,     0,     0,   369,   370,
      22,    23,   371,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   372,   373,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    24,    25,    26,    27,    28,    29,
      30,    31,    32,    33,    34,    35,    36,    37,    38,    39,
      40,    41,    42,     3,     4,     5,     0,     0,     0,     6,
       7,     0,     9,     0,     0,    10,    11,     0,     0,    13,
      14,    52,     0,    16,     0,     0,    18,    19,    20,    21,
       0,     0,     0,     0,     0,     0,    23,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     850,     0,   851,     0,     0,     0,     0,    54,     0,    24,
      25,    26,    27,    28,    29,    30,    31,    32,    33,    34,
      35,    36,    37,    38,    39,    40,    41,    42,     0,     0,
      43,    44,    45,     0,     0,     0,     0,     0,    46,    47,
      48,    49,    50,    51,     3,     4,     5,     0,     0,     0,
       6,     7,     0,     9,     0,     0,    10,    11,     0,     0,
      13,    14,     0,     0,    16,     0,     0,    18,    19,    20,
      21,     0,     0,     0,     0,     0,     0,    23,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      24,    25,    26,    27,    28,    29,    30,    31,    32,    33,
      34,    35,    36,    37,    38,    39,    40,    41,    42,     0,
       0,    43,    44,    45,     0,     0,     0,     0,     0,    46,
      47,    48,    49,    50,    51,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     3,     4,     5,     0,     0,     0,
       6,     7,     0,     9,     0,     0,    10,    11,     0,     0,
      13,    14,     0,     0,    16,     0,     0,    18,    19,    20,
      21,     0,     0,     0,     0,     0,   374,    23,     0,     0,
       0,     0,   375,   376,   377,   378,   379,   380,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   381,   382,   121,
      24,    25,    26,    27,    28,    29,    30,    31,    32,    33,
      34,    35,    36,    37,    38,    39,    40,    41,    42,     0,
       0,    43,    44,    45,     0,     0,     0,     0,     0,    46,
      47,    48,    49,    50,    51,   109,     0,     0,     1,     2,
       0,     0,     3,     4,     5,     0,     0,     0,     6,     7,
       8,     9,     0,     0,    10,    11,    12,     0,    13,    14,
       0,    15,    16,     0,    17,    18,    19,    20,    21,     0,
       0,     0,     0,   374,    22,    23,     0,     0,     0,   375,
     376,   377,   378,   379,   380,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   381,   844,   121,     0,    24,    25,
      26,    27,    28,    29,    30,    31,    32,    33,    34,    35,
      36,    37,    38,    39,    40,    41,    42,     0,     0,    43,
      44,    45,     0,     0,     0,     0,     0,    46,    47,    48,
      49,    50,    51,     0,     0,    52,     1,     2,     0,     0,
       3,     4,     5,     0,     0,     0,     6,     7,     8,     9,
       0,     0,    10,    11,    12,     0,    13,    14,     0,    15,
      16,     0,    17,    18,    19,    20,    21,     0,     0,     0,
     354,     0,    22,    23,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    24,    25,    26,    27,
      28,    29,    30,    31,    32,    33,    34,    35,    36,    37,
      38,    39,    40,    41,    42,     0,     0,    43,    44,    45,
       0,     0,     0,     0,     0,    46,    47,    48,    49,    50,
      51,     0,     0,    52,     3,     4,     5,     0,     0,     0,
       6,     7,     0,     9,     0,     0,    10,    11,     0,     0,
      13,    14,     0,     0,    16,     0,     0,    18,    19,    20,
      21,   775,     0,     0,     0,     0,     0,    23,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      24,    25,    26,    27,    28,    29,    30,    31,    32,    33,
      34,    35,    36,    37,    38,    39,    40,    41,    42,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     3,     4,     5,     0,
       0,     0,     6,     7,     0,     9,     0,     0,    10,    11,
       0,     0,    13,    14,     0,     0,    16,     0,     0,    18,
      19,    20,    21,     0,     0,     0,     0,     0,     0,    23,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   782,    24,    25,    26,    27,    28,    29,    30,    31,
      32,    33,    34,    35,    36,    37,    38,    39,    40,    41,
      42,     0,     0,     0,     0,     0,     0,     0,     0,     3,
       4,     5,     0,     0,     0,     6,     7,     0,     9,     0,
       0,    10,    11,     0,     0,    13,    14,     0,     0,    16,
       0,     0,    18,    19,    20,    21,     0,     0,     0,     0,
       0,     0,    23,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    53,     0,     0,
       0,     0,     0,     0,    54,    24,    25,    26,    27,    28,
      29,    30,    31,    32,    33,    34,    35,    36,    37,    38,
      39,    40,    41,    42,     0,   493,  1524,     0,     0,     0,
     494,   495,     0,     0,   496,   497,     0,     0,     0,     0,
       0,   498,   499,     0,   500,   501,     0,   502,     0,   503,
       0,     0,     0,     0,     0,   504,     0,     0,     0,     0,
       0,     0,   505,     0,   506,   507,     0,  1525,     0,     0,
       0,     0,     0,     0,   508,     0,   509,     0,     0,     0,
       0,     0,     0,   510,     0,   511,   512,     0,     0,     0,
       0,     0,     0,     0,     0,    53,     0,     0,     0,     0,
       0,     0,    54,     0,     0,     0,     0,   513,   514,   515,
     516,     0,   517,     0,   518,     0,   519,   520,   521,     0,
     522,     0,   667,   523,     0,     0,     0,     0,   524,     0,
       0,     0,     0,     0,     0,   525,     0,     0,   526,     0,
       0,     0,   527,   528,   529,   530,     0,     0,     0,     0,
       0,     0,   531,     0,     0,     0,     0,   532,     0,     0,
       0,   533,     0,     0,   534,     0,     0,   535,     0,     0,
       0,   536,   537,   538,   539,   668,     0,   540,     0,     0,
     541,     0,     0,     0,     0,     0,     0,     0,     0,  1288,
    1322,   851,   542,   543,   544,   545,    54,     0,     0,     0,
       0,     0,     0,     0,     0,   669,   546,     0,     0,     0,
       0,     0,     0,     0,     0,   547,     0,   548,     0,     0,
       0,     0,     0,   549,   550,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   551,   552,
       0,     0,   553,   554,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     555,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   556,  1526,     0,     0,  1527,   557,     0,   558,
       0,  1288,     0,   851,     0,     0,     0,   559,    54,     0,
     560,   561,   562,   563,   564,   565,   566,     0,     0,     0,
       0,     0,   567,     0,   568,     0,     0,     0,     0,     0,
       0,     0,   569,     0,     0,     0,   570,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   571,   572,   573,   574,   575,   576,   577,
     578,   579,   580,     0,   493,  1524,     0,     0,     0,   494,
     495,     0,     0,   496,   497,     0,     0,     0,     0,     0,
     498,   499,     0,   500,   501,  1874,   502,     0,   503,     0,
       0,     0,     0,     0,   504,     0,     0,     0,     0,     0,
       0,   505,     0,   506,   507,     0,  1525,     0,     0,     0,
       0,     0,     0,   508,     0,   509,     0,     0,     0,     0,
       0,     0,   510,     0,   511,   512,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   513,   514,   515,   516,
       0,   517,     0,   518,     0,   519,   520,   521,     0,   522,
       0,   667,   523,     0,     0,     0,     0,   524,     0,     0,
       0,     0,     0,     0,   525,     0,     0,   526,     0,     0,
       0,   527,   528,   529,   530,     0,     0,     0,     0,     0,
       0,   531,     0,     0,     0,     0,   532,     0,     0,     0,
     533,     0,     0,   534,     0,     0,   535,     0,     0,     0,
     536,   537,   538,   539,   668,     0,   540,     0,     0,   541,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   542,   543,   544,   545,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   669,   546,     0,     0,     0,     0,
       0,     0,     0,     0,   547,     0,   548,     0,     0,     0,
       0,     0,   549,   550,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   551,   552,     0,
       0,   553,   554,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   555,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   556,  3146,     0,     0,  1527,   557,     0,   558,     0,
       0,     0,     0,     0,     0,     0,   559,     0,     0,   560,
     561,   562,   563,   564,   565,   566,     0,     0,     0,     0,
       0,   567,     0,   568,     0,     0,     0,     0,     0,     0,
       0,   569,     0,     0,     0,   570,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   571,   572,   573,   574,   575,   576,   577,   578,
     579,   580,   493,     0,     0,     0,     0,   494,   495,     0,
       0,   496,   497,     0,     0,     0,     0,     0,   498,   499,
       0,   500,   501,     0,   502,     0,   503,     0,     0,     0,
       0,     0,   504,     0,     0,     0,     0,     0,     0,   505,
       0,   506,   507,     0,     0,     0,     0,     0,     0,     0,
       0,   508,     0,   509,     0,     0,     0,     0,     0,     0,
     510,     0,   511,   512,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   513,   514,   515,   516,     0,   517,
       0,   518,     0,   519,   520,   521,     0,   522,     0,   667,
     523,     0,     0,     0,     0,   524,     0,     0,     0,     0,
       0,     0,   525,     0,     0,   526,     0,     0,     0,   527,
     528,   529,   530,     0,     0,     0,     0,     0,     0,   531,
       0,     0,     0,     0,   532,     0,     0,     0,   533,     0,
       0,   534,     0,     0,   535,     0,     0,     0,   536,   537,
     538,   539,   668,     0,   540,     0,     0,   541,     0,     0,
       0,     0,     0,     0,     0,     0,   945,     0,     0,   542,
     543,   544,   545,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   669,   546,     0,     0,     0,     0,     0,     0,
       0,     0,   547,     0,   548,     0,     0,     0,     0,     0,
     549,   550,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   551,   552,     0,     0,   553,
     554,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   555,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   556,
    3864,     0,     0,     0,   557,     0,   558,     0,     0,     0,
       0,     0,     0,     0,   559,     0,     0,   560,   561,   562,
     563,   564,   565,   566,     0,     0,     0,     0,     0,   567,
       0,   568,     0,     0,     0,     0,     0,     0,     0,   569,
       0,     0,     0,   570,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     571,   572,   573,   574,   575,   576,   577,   578,   579,   580,
     493,     0,     0,     0,     0,   494,   495,     0,     0,   496,
     497,     0,     0,     0,     0,     0,   498,   499,     0,   500,
     501,     0,   502,     0,   503,     0,     0,     0,     0,     0,
     504,     0,     0,     0,     0,     0,     0,   505,     0,   506,
     507,     0,     0,     0,     0,     0,     0,     0,     0,   508,
       0,   509,     0,     0,     0,     0,     0,     0,   510,     0,
     511,   512,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   513,   514,   515,   516,     0,   517,     0,   518,
       0,   519,   520,   521,     0,   522,     0,   667,   523,     0,
       0,     0,     0,   524,     0,     0,     0,     0,     0,     0,
     525,     0,     0,   526,     0,     0,     0,   527,   528,   529,
     530,     0,     0,     0,     0,     0,     0,   531,     0,     0,
       0,     0,   532,     0,     0,     0,   533,     0,     0,   534,
       0,     0,   535,     0,     0,     0,   536,   537,   538,   539,
     668,     0,   540,     0,     0,   541,     0,     0,     0,     0,
       0,     0,     0,     0,   945,     0,     0,   542,   543,   544,
     545,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     669,   546,     0,     0,     0,     0,     0,     0,     0,     0,
     547,     0,   548,     0,     0,     0,     0,     0,   549,   550,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   551,   552,     0,     0,   553,   554,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   555,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   556,     0,     0,
       0,     0,   557,     0,   558,     0,     0,     0,     0,     0,
       0,     0,   559,     0,     0,   560,   561,   562,   563,   564,
     565,   566,     0,     0,     0,     0,     0,   567,     0,   568,
       0,     0,     0,     0,     0,     0,     0,   569,     0,     0,
       0,   570,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   571,   572,
     573,   574,   575,   576,   577,   578,   579,   580,   493,     0,
       0,     0,     0,   494,   495,     0,     0,   496,   497,     0,
       0,     0,     0,     0,   498,   499,     0,   500,   501,     0,
     502,     0,   503,     0,     0,     0,     0,     0,   504,     0,
       0,     0,     0,     0,     0,   505,     0,   506,   507,     0,
       0,     0,     0,     0,     0,     0,     0,   508,     0,   509,
       0,     0,     0,     0,     0,     0,   510,     0,   511,   512,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     513,   514,   515,   516,     0,   517,     0,   518,     0,   519,
     520,   521,     0,   522,     0,   667,   523,     0,     0,     0,
       0,   524,     0,     0,     0,     0,     0,     0,   525,     0,
       0,   526,     0,     0,     0,   527,   528,   529,   530,     0,
       0,     0,     0,     0,     0,   531,     0,     0,     0,     0,
     532,     0,     0,     0,   533,     0,     0,   534,     0,     0,
     535,     0,     0,     0,   536,   537,   538,   539,   668,     0,
     540,     0,     0,   541,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   542,   543,   544,   545,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   669,   546,
       0,     0,     0,     0,     0,     0,     0,     0,   547,     0,
     548,     0,     0,     0,     0,     0,   549,   550,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   551,   552,     0,     0,   553,   554,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   555,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   556,  1516,     0,     0,     0,
     557,     0,   558,     0,     0,     0,     0,     0,     0,     0,
     559,     0,     0,   560,   561,   562,   563,   564,   565,   566,
       0,     0,     0,     0,     0,   567,     0,   568,     0,     0,
       0,     0,     0,     0,     0,   569,     0,     0,     0,   570,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   571,   572,   573,   574,
     575,   576,   577,   578,   579,   580,   493,     0,     0,     0,
       0,   494,   495,     0,     0,   496,   497,     0,     0,     0,
       0,     0,   498,   499,     0,   500,   501,     0,   502,     0,
     503,     0,     0,     0,     0,     0,   504,     0,     0,     0,
       0,     0,     0,   505,     0,   506,   507,     0,     0,     0,
       0,     0,     0,     0,     0,   508,     0,   509,     0,     0,
       0,     0,     0,     0,   510,     0,   511,   512,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   513,   514,
     515,   516,     0,   517,     0,   518,     0,   519,   520,   521,
       0,   522,     0,   667,   523,     0,     0,     0,     0,   524,
       0,     0,     0,     0,     0,     0,   525,     0,     0,   526,
       0,     0,     0,   527,   528,   529,   530,     0,     0,     0,
       0,     0,     0,   531,     0,     0,     0,     0,   532,     0,
       0,     0,   533,     0,     0,   534,     0,     0,   535,     0,
       0,     0,   536,   537,   538,   539,   668,     0,   540,     0,
       0,   541,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   542,   543,   544,   545,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   669,   546,     0,     0,
       0,     0,     0,     0,     0,     0,   547,     0,   548,     0,
       0,     0,     0,     0,   549,   550,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   551,
     552,     0,     0,   553,   554,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   555,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   556,     0,     0,     0,     0,   557,     0,
     558,     0,     0,     0,  1599,     0,     0,     0,   559,     0,
       0,   560,   561,   562,   563,   564,   565,   566,     0,     0,
       0,     0,     0,   567,     0,   568,     0,     0,     0,     0,
       0,     0,     0,   569,     0,     0,     0,   570,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   571,   572,   573,   574,   575,   576,
     577,   578,   579,   580,   493,     0,     0,     0,     0,   494,
     495,     0,     0,   496,   497,     0,     0,     0,     0,     0,
     498,   499,     0,   500,   501,     0,   502,     0,   503,     0,
       0,     0,     0,     0,   504,     0,     0,     0,     0,     0,
       0,   505,     0,   506,   507,     0,     0,     0,     0,     0,
       0,     0,     0,   508,     0,   509,     0,     0,     0,     0,
       0,     0,   510,     0,   511,   512,  1829,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   513,   514,   515,   516,
       0,   517,     0,   518,     0,   519,   520,   521,     0,   522,
       0,   667,   523,     0,     0,     0,     0,   524,     0,     0,
       0,     0,     0,     0,   525,     0,     0,   526,     0,     0,
       0,   527,   528,   529,   530,     0,     0,     0,     0,     0,
       0,   531,     0,     0,     0,     0,   532,     0,     0,     0,
     533,     0,     0,   534,     0,     0,   535,     0,     0,     0,
     536,   537,   538,   539,   668,     0,   540,     0,     0,   541,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   542,   543,   544,   545,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   669,   546,     0,     0,     0,     0,
       0,     0,     0,     0,   547,     0,   548,     0,     0,     0,
       0,     0,   549,   550,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   551,   552,     0,
       0,   553,   554,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   555,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   556,     0,     0,     0,     0,   557,     0,   558,     0,
       0,     0,     0,     0,     0,     0,   559,     0,     0,   560,
     561,   562,   563,   564,   565,   566,     0,     0,     0,     0,
       0,   567,     0,   568,     0,     0,     0,     0,     0,     0,
       0,   569,     0,     0,     0,   570,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   571,   572,   573,   574,   575,   576,   577,   578,
     579,   580,   493,     0,     0,     0,     0,   494,   495,     0,
       0,   496,   497,     0,     0,     0,     0,     0,   498,   499,
       0,   500,   501,     0,   502,     0,   503,     0,     0,     0,
       0,     0,   504,     0,     0,     0,     0,     0,     0,   505,
       0,   506,   507,     0,     0,     0,     0,     0,     0,     0,
       0,   508,     0,   509,     0,     0,     0,     0,     0,     0,
     510,     0,   511,   512,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   513,   514,   515,   516,     0,   517,
       0,   518,     0,   519,   520,   521,     0,   522,     0,   667,
     523,     0,     0,     0,     0,   524,     0,     0,     0,     0,
       0,     0,   525,     0,     0,   526,     0,     0,     0,   527,
     528,   529,   530,     0,     0,     0,     0,     0,     0,   531,
       0,     0,     0,     0,   532,     0,     0,     0,   533,     0,
       0,   534,     0,     0,   535,     0,     0,     0,   536,   537,
     538,   539,   668,     0,   540,     0,     0,   541,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   542,
     543,   544,   545,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   669,   546,     0,     0,     0,     0,     0,     0,
       0,     0,   547,     0,   548,     0,     0,     0,     0,     0,
     549,   550,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   551,   552,     0,     0,   553,
     554,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   555,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   556,
       0,     0,     0,  2089,   557,     0,   558,     0,     0,     0,
       0,     0,     0,     0,   559,     0,     0,   560,   561,   562,
     563,   564,   565,   566,     0,     0,     0,     0,     0,   567,
       0,   568,     0,     0,     0,     0,     0,     0,     0,   569,
       0,     0,     0,   570,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     571,   572,   573,   574,   575,   576,   577,   578,   579,   580,
     493,     0,     0,     0,     0,   494,   495,     0,     0,   496,
     497,     0,     0,     0,     0,     0,   498,   499,     0,   500,
     501,     0,   502,     0,   503,     0,     0,     0,     0,     0,
     504,     0,     0,     0,     0,     0,     0,   505,     0,   506,
     507,     0,     0,     0,     0,     0,     0,     0,     0,   508,
       0,   509,     0,     0,     0,     0,     0,     0,   510,     0,
     511,   512,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   513,   514,   515,   516,     0,   517,     0,   518,
       0,   519,   520,   521,     0,   522,     0,   667,   523,     0,
       0,     0,     0,   524,     0,     0,     0,     0,     0,     0,
     525,     0,     0,   526,     0,     0,     0,   527,   528,   529,
     530,     0,     0,     0,     0,     0,     0,   531,     0,     0,
       0,     0,   532,     0,     0,     0,   533,     0,     0,   534,
       0,     0,   535,     0,     0,     0,   536,   537,   538,   539,
     668,     0,   540,     0,     0,   541,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   542,   543,   544,
     545,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     669,   546,     0,     0,     0,     0,     0,     0,     0,     0,
     547,     0,   548,     0,     0,     0,     0,     0,   549,   550,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   551,   552,     0,     0,   553,   554,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   555,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   556,  2182,     0,
       0,     0,   557,     0,   558,     0,     0,     0,     0,     0,
       0,     0,   559,     0,     0,   560,   561,   562,   563,   564,
     565,   566,     0,     0,     0,     0,     0,   567,     0,   568,
       0,     0,     0,     0,     0,     0,     0,   569,     0,     0,
       0,   570,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   571,   572,
     573,   574,   575,   576,   577,   578,   579,   580,   493,     0,
       0,     0,     0,   494,   495,     0,     0,   496,   497,     0,
       0,     0,     0,     0,   498,   499,     0,   500,   501,     0,
     502,     0,   503,     0,     0,     0,     0,     0,   504,     0,
       0,     0,     0,     0,     0,   505,     0,   506,   507,     0,
       0,     0,     0,     0,     0,     0,     0,   508,     0,   509,
       0,     0,     0,     0,     0,     0,   510,     0,   511,   512,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     513,   514,   515,   516,     0,   517,     0,   518,     0,   519,
     520,   521,     0,   522,     0,   667,   523,     0,     0,     0,
       0,   524,     0,     0,     0,     0,     0,     0,   525,     0,
       0,   526,     0,     0,     0,   527,   528,   529,   530,     0,
       0,     0,     0,     0,     0,   531,     0,     0,     0,     0,
     532,     0,     0,     0,   533,     0,     0,   534,     0,     0,
     535,     0,     0,     0,   536,   537,   538,   539,   668,     0,
     540,     0,     0,   541,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   542,   543,   544,   545,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   669,   546,
       0,     0,     0,     0,     0,     0,     0,     0,   547,     0,
     548,     0,     0,     0,     0,     0,   549,   550,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   551,   552,     0,     0,   553,   554,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   555,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   556,  2527,     0,     0,     0,
     557,     0,   558,     0,     0,     0,     0,     0,     0,     0,
     559,     0,     0,   560,   561,   562,   563,   564,   565,   566,
       0,     0,     0,     0,     0,   567,     0,   568,     0,     0,
       0,     0,     0,     0,     0,   569,     0,     0,     0,   570,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   571,   572,   573,   574,
     575,   576,   577,   578,   579,   580,   493,     0,     0,     0,
       0,   494,   495,     0,     0,   496,   497,     0,     0,     0,
       0,     0,   498,   499,     0,   500,   501,     0,   502,     0,
     503,     0,     0,     0,     0,     0,   504,     0,     0,     0,
       0,     0,     0,   505,     0,   506,   507,     0,     0,     0,
       0,     0,     0,     0,     0,   508,     0,   509,     0,     0,
       0,     0,     0,     0,   510,     0,   511,   512,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   513,   514,
     515,   516,     0,   517,     0,   518,     0,   519,   520,   521,
       0,   522,     0,   667,   523,     0,     0,     0,     0,   524,
       0,     0,     0,     0,     0,     0,   525,     0,     0,   526,
       0,     0,     0,   527,   528,   529,   530,     0,     0,     0,
       0,     0,     0,   531,     0,     0,     0,     0,   532,     0,
       0,     0,   533,     0,     0,   534,     0,     0,   535,     0,
       0,     0,   536,   537,   538,   539,   668,     0,   540,     0,
       0,   541,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   542,   543,   544,   545,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   669,   546,     0,     0,
       0,     0,     0,     0,     0,     0,   547,     0,   548,     0,
       0,     0,     0,     0,   549,   550,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   551,
     552,     0,     0,   553,   554,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   555,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   556,  3122,     0,     0,     0,   557,     0,
     558,     0,     0,     0,     0,     0,     0,     0,   559,     0,
       0,   560,   561,   562,   563,   564,   565,   566,     0,     0,
       0,     0,     0,   567,     0,   568,     0,     0,     0,     0,
       0,     0,     0,   569,     0,     0,     0,   570,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   571,   572,   573,   574,   575,   576,
     577,   578,   579,   580,   493,     0,     0,     0,     0,   494,
     495,     0,     0,   496,   497,     0,     0,     0,     0,     0,
     498,   499,     0,   500,   501,     0,   502,     0,   503,     0,
       0,     0,     0,     0,   504,     0,     0,     0,     0,     0,
       0,   505,     0,   506,   507,     0,     0,     0,     0,     0,
       0,     0,     0,   508,     0,   509,     0,     0,     0,     0,
       0,     0,   510,     0,   511,   512,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   513,   514,   515,   516,
       0,   517,     0,   518,     0,   519,   520,   521,     0,   522,
       0,   667,   523,     0,     0,     0,     0,   524,     0,     0,
       0,     0,     0,     0,   525,     0,     0,   526,     0,     0,
       0,   527,   528,   529,   530,     0,     0,     0,     0,     0,
       0,   531,     0,     0,     0,     0,   532,     0,     0,     0,
     533,     0,     0,   534,     0,     0,   535,     0,     0,     0,
     536,   537,   538,   539,   668,     0,   540,     0,     0,   541,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   542,   543,   544,   545,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   669,   546,     0,     0,     0,     0,
       0,     0,     0,     0,   547,     0,   548,     0,     0,     0,
       0,     0,   549,   550,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   551,   552,     0,
       0,   553,   554,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   555,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   556,     0,     0,     0,     0,   557,     0,   558,     0,
       0,     0,     0,     0,     0,     0,   559,     0,     0,   560,
     561,   562,   563,   564,   565,   566,     0,     0,     0,     0,
       0,   567,     0,   568,     0,     0,     0,     0,     0,     0,
       0,   569,     0,     0,     0,   570,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   571,   572,   573,   574,   575,   576,   577,   578,
     579,   580,   493,     0,     0,     0,     0,   494,   495,     0,
       0,   496,   497,     0,     0,     0,     0,     0,   498,   499,
       0,   500,   501,     0,   502,     0,   503,     0,     0,     0,
       0,     0,   504,     0,     0,     0,     0,     0,     0,   505,
       0,   506,   507,     0,     0,     0,     0,     0,     0,     0,
       0,   508,     0,   509,     0,     0,     0,     0,     0,     0,
     510,     0,   511,   512,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   513,   514,   515,   516,     0,   517,
       0,   518,     0,   519,   520,   521,     0,   522,     0,     0,
     523,     0,     0,     0,     0,   524,     0,     0,     0,     0,
       0,     0,   525,     0,     0,   526,     0,     0,     0,   527,
     528,   529,   530,     0,     0,     0,     0,     0,     0,   531,
       0,     0,     0,     0,   532,     0,     0,     0,   533,     0,
       0,   534,     0,     0,   535,     0,     0,     0,   536,   537,
     538,   539,     0,     0,   540,     0,     0,   541,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   542,
     543,   544,   545,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   546,     0,     0,     0,     0,     0,     0,
       0,     0,   547,     0,   548,     0,     0,     0,     0,     0,
     549,   550,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   551,   552,     0,     0,   553,
     554,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   555,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   556,
       0,     0,     0,     0,   557,     0,   558,     0,     0,     0,
       0,     0,     0,     0,   559,     0,     0,   560,   561,   562,
     563,   564,   565,   566,     0,     0,     0,     0,     0,   567,
       0,   568,  3681,     0,     0,     0,     0,     0,     0,   569,
       0,     0,     0,   570,     0,     0,     0,     0,     0,     0,
       0,  3682,     0,     0,     0,     0,     0,     0,  3683,     0,
     571,   572,   573,   574,   575,   576,   577,   578,   579,   580,
     493,     0,     0,     0,     0,   494,   495,     0,     0,   496,
     497,     0,     0,     0,     0,     0,   498,   499,     0,   500,
     501,     0,   502,     0,   503,     0,     0,     0,     0,     0,
     504,     0,     0,     0,     0,     0,     0,   505,     0,   506,
     507,     0,     0,     0,     0,     0,     0,     0,     0,   508,
       0,   509,     0,     0,     0,     0,     0,     0,   510,     0,
     511,   512,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   513,   514,   515,   516,     0,   517,     0,   518,
       0,   519,   520,   521,     0,   522,     0,     0,   523,     0,
       0,     0,     0,   524,     0,     0,     0,     0,     0,     0,
     525,     0,     0,   526,     0,     0,     0,   527,   528,   529,
     530,     0,     0,     0,     0,     0,     0,   531,     0,     0,
       0,     0,   532,     0,     0,     0,   533,     0,     0,   534,
       0,     0,   535,     0,     0,     0,   536,   537,   538,   539,
     668,     0,   540,     0,     0,   541,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   542,   543,   544,
     545,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     669,   546,     0,     0,     0,     0,     0,     0,     0,     0,
     547,     0,   548,     0,     0,     0,     0,     0,   549,   550,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   551,   552,     0,     0,   553,   554,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   555,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   556,     0,     0,
       0,     0,   557,     0,   558,     0,     0,     0,     0,     0,
       0,     0,   559,     0,     0,   560,   561,   562,   563,   564,
     565,   566,     0,     0,     0,     0,     0,   567,     0,   568,
       0,     0,     0,     0,     0,     0,     0,   569,     0,     0,
       0,   570,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   571,   572,
     573,   574,   575,   576,   577,   578,   579,   580,   493,     0,
       0,     0,     0,   494,   495,     0,     0,   496,   497,     0,
       0,     0,     0,     0,   498,   499,     0,   500,   501,     0,
     502,     0,   503,     0,     0,     0,     0,     0,   504,     0,
       0,     0,     0,     0,     0,   505,     0,   506,   507,     0,
       0,     0,     0,     0,     0,     0,     0,   508,     0,   509,
       0,     0,     0,     0,     0,     0,   510,     0,   511,   512,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     513,   514,   515,   516,     0,   517,     0,   518,     0,   519,
     520,   521,     0,   522,     0,     0,   523,     0,     0,     0,
       0,   524,     0,     0,     0,     0,     0,     0,   525,     0,
       0,   526,     0,     0,     0,   527,   528,   529,   530,     0,
       0,     0,     0,     0,     0,   531,     0,     0,     0,     0,
     532,     0,     0,     0,   533,     0,     0,   534,     0,     0,
     535,     0,     0,     0,   536,   537,   538,   539,     0,     0,
     540,     0,     0,   541,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   542,   543,   544,   545,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   546,
       0,     0,     0,     0,     0,     0,     0,     0,   547,     0,
     548,     0,     0,     0,     0,     0,   549,   550,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   551,   552,     0,     0,   553,   554,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   555,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,  3824,     0,     0,     0,     0,
     557,     0,   558,     0,     0,     0,     0,     0,     0,     0,
     559,     0,     0,   560,   561,   562,   563,   564,   565,   566,
       0,     0,     0,     0,     0,   567,     0,   568,  3681,     0,
       0,     0,     0,     0,     0,   569,     0,     0,     0,   570,
       0,     0,     0,     0,     0,     0,     0,  3682,     0,     0,
       0,     0,     0,     0,     0,     0,   571,   572,   573,   574,
     575,   576,   577,   578,   579,   580,   493,     0,     0,     0,
       0,   494,   495,     0,     0,   496,   497,     0,     0,     0,
       0,     0,   498,   499,     0,   500,   501,     0,   502,     0,
     503,     0,     0,     0,     0,     0,   504,     0,     0,     0,
       0,     0,     0,   505,     0,   506,   507,     0,     0,     0,
       0,     0,     0,     0,     0,   508,     0,   509,     0,     0,
       0,     0,     0,     0,   510,     0,   511,   512,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   513,   514,
     515,   516,     0,   517,     0,   518,     0,   519,   520,   521,
       0,   522,     0,     0,   523,     0,     0,     0,     0,   524,
       0,     0,     0,     0,     0,     0,   525,     0,     0,   526,
       0,     0,     0,   527,   528,   529,   530,     0,     0,     0,
       0,     0,     0,   531,     0,     0,     0,     0,   532,     0,
       0,     0,   533,     0,     0,   534,     0,     0,   535,     0,
       0,     0,   536,   537,   538,   539,     0,     0,   540,     0,
       0,   541,     0,     0,     0,     0,     0,   934,     0,     0,
       0,     0,     0,   542,   543,   544,   545,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   546,     0,     0,
       0,     0,     0,     0,     0,     0,   547,     0,   548,     0,
       0,     0,     0,     0,   549,   550,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   551,
     552,     0,     0,   553,   554,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   555,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   556,     0,     0,     0,     0,   557,     0,
     558,     0,     0,     0,     0,     0,     0,     0,   559,     0,
       0,   560,   561,   562,   563,   564,   565,   566,     0,     0,
       0,     0,     0,   567,     0,   568,     0,     0,     0,     0,
       0,     0,     0,   569,     0,     0,     0,   570,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   571,   572,   573,   574,   575,   576,
     577,   578,   579,   580,   493,     0,     0,     0,     0,   494,
     495,     0,     0,   496,   497,     0,     0,     0,     0,     0,
     498,   499,     0,   500,   501,     0,   502,     0,   503,     0,
       0,     0,     0,     0,   504,     0,     0,     0,     0,     0,
       0,   505,     0,   506,   507,     0,     0,     0,     0,     0,
       0,     0,     0,   508,     0,   509,     0,     0,     0,     0,
       0,     0,   510,     0,   511,   512,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   513,   514,   515,   516,
       0,   517,     0,   518,     0,   519,   520,   521,     0,   522,
       0,     0,   523,     0,     0,     0,     0,   524,     0,     0,
       0,     0,     0,     0,   525,     0,     0,   526,     0,     0,
       0,   527,   528,   529,   530,     0,     0,     0,     0,     0,
       0,   531,     0,     0,     0,     0,   532,     0,     0,     0,
     533,     0,     0,   534,     0,     0,   535,     0,     0,     0,
     536,   537,   538,   539,     0,     0,   540,     0,     0,   541,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   542,   543,   544,   545,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   546,     0,     0,     0,     0,
       0,     0,     0,     0,   547,     0,   548,     0,     0,     0,
       0,     0,   549,   550,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   551,   552,     0,
       0,   553,   554,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   555,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   556,     0,     0,     0,   977,   557,     0,   558,     0,
       0,     0,     0,     0,     0,     0,   559,     0,     0,   560,
     561,   562,   563,   564,   565,   566,     0,     0,     0,     0,
       0,   567,     0,   568,     0,     0,     0,     0,     0,     0,
       0,   569,     0,     0,     0,   570,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   571,   572,   573,   574,   575,   576,   577,   578,
     579,   580,   493,     0,     0,     0,     0,   494,   495,     0,
       0,   496,   497,     0,     0,     0,     0,     0,   498,   499,
       0,   500,   501,     0,   502,     0,   503,     0,     0,     0,
       0,     0,   504,     0,     0,     0,     0,     0,     0,   505,
       0,   506,   507,     0,     0,     0,     0,     0,     0,     0,
       0,   508,     0,   509,     0,     0,     0,     0,     0,     0,
     510,     0,   511,   512,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   513,   514,   515,   516,     0,   517,
       0,   518,     0,   519,   520,   521,     0,   522,     0,     0,
     523,     0,     0,     0,     0,   524,     0,     0,     0,     0,
       0,     0,   525,     0,     0,   526,     0,     0,     0,   527,
     528,   529,   530,     0,     0,     0,     0,     0,     0,   531,
       0,     0,     0,     0,   532,     0,     0,     0,   533,     0,
       0,   534,     0,     0,   535,     0,     0,     0,   536,   537,
     538,   539,     0,     0,   540,     0,     0,   541,     0,     0,
       0,     0,     0,  1578,     0,     0,     0,     0,     0,   542,
     543,   544,   545,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   546,     0,     0,     0,     0,     0,     0,
       0,     0,   547,     0,   548,     0,     0,     0,     0,     0,
     549,   550,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   551,   552,     0,     0,   553,
     554,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   555,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   556,
       0,     0,     0,     0,   557,     0,   558,     0,     0,     0,
       0,     0,     0,     0,   559,     0,     0,   560,   561,   562,
     563,   564,   565,   566,     0,     0,     0,     0,     0,   567,
       0,   568,     0,     0,     0,     0,     0,     0,     0,   569,
       0,     0,     0,   570,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     571,   572,   573,   574,   575,   576,   577,   578,   579,   580,
     493,     0,     0,     0,     0,   494,   495,     0,     0,   496,
     497,     0,     0,     0,     0,     0,   498,   499,     0,   500,
     501,     0,   502,     0,   503,     0,     0,     0,     0,     0,
     504,     0,     0,     0,     0,  2973,     0,   505,     0,   506,
     507,     0,     0,     0,     0,     0,     0,     0,     0,   508,
       0,   509,     0,     0,     0,     0,     0,     0,   510,     0,
     511,   512,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   513,   514,   515,   516,     0,   517,     0,   518,
       0,   519,   520,   521,     0,   522,     0,     0,   523,     0,
       0,     0,     0,   524,     0,     0,     0,     0,     0,     0,
     525,     0,     0,   526,     0,     0,     0,   527,   528,   529,
     530,     0,     0,     0,     0,     0,     0,   531,     0,     0,
       0,     0,   532,     0,     0,     0,   533,     0,     0,   534,
       0,     0,   535,     0,     0,     0,   536,   537,   538,   539,
       0,     0,   540,     0,     0,   541,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   542,   543,   544,
     545,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   546,     0,     0,     0,     0,     0,     0,     0,     0,
     547,     0,   548,     0,     0,     0,     0,     0,   549,   550,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   551,   552,     0,     0,   553,   554,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   555,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   556,     0,     0,
       0,     0,   557,     0,   558,     0,     0,     0,     0,     0,
       0,     0,   559,     0,     0,   560,   561,   562,   563,   564,
     565,   566,     0,     0,     0,     0,     0,   567,     0,   568,
       0,     0,     0,     0,     0,     0,     0,   569,     0,     0,
       0,   570,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   571,   572,
     573,   574,   575,   576,   577,   578,   579,   580,   493,     0,
       0,     0,     0,   494,   495,     0,     0,   496,   497,     0,
       0,     0,     0,     0,   498,   499,     0,   500,   501,     0,
     502,     0,   503,     0,     0,     0,     0,     0,   504,     0,
       0,     0,     0,  3133,     0,   505,     0,   506,   507,     0,
       0,     0,     0,     0,     0,     0,     0,   508,     0,   509,
       0,     0,     0,     0,     0,     0,   510,     0,   511,   512,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     513,   514,   515,   516,     0,   517,     0,   518,     0,   519,
     520,   521,     0,   522,     0,     0,   523,     0,     0,     0,
       0,   524,     0,     0,     0,     0,     0,     0,   525,     0,
       0,   526,     0,     0,     0,   527,   528,   529,   530,     0,
       0,     0,     0,     0,     0,   531,     0,     0,     0,     0,
     532,     0,     0,     0,   533,     0,     0,   534,     0,     0,
     535,     0,     0,     0,   536,   537,   538,   539,     0,     0,
     540,     0,     0,   541,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   542,   543,   544,   545,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   546,
       0,     0,     0,     0,     0,     0,     0,     0,   547,     0,
     548,     0,     0,     0,     0,     0,   549,   550,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   551,   552,     0,     0,   553,   554,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   555,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   556,     0,     0,     0,     0,
     557,     0,   558,     0,     0,     0,     0,     0,     0,     0,
     559,     0,     0,   560,   561,   562,   563,   564,   565,   566,
       0,     0,     0,     0,     0,   567,     0,   568,     0,     0,
       0,     0,     0,     0,     0,   569,     0,     0,     0,   570,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   571,   572,   573,   574,
     575,   576,   577,   578,   579,   580,   493,     0,     0,     0,
       0,   494,   495,     0,     0,   496,   497,     0,     0,     0,
       0,     0,   498,   499,     0,   500,   501,     0,   502,     0,
     503,     0,     0,     0,     0,     0,   504,     0,     0,     0,
       0,  3534,     0,   505,     0,   506,   507,     0,     0,     0,
       0,     0,     0,     0,     0,   508,     0,   509,     0,     0,
       0,     0,     0,     0,   510,     0,   511,   512,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   513,   514,
     515,   516,     0,   517,     0,   518,     0,   519,   520,   521,
       0,   522,     0,     0,   523,     0,     0,     0,     0,   524,
       0,     0,     0,     0,     0,     0,   525,     0,     0,   526,
       0,     0,     0,   527,   528,   529,   530,     0,     0,     0,
       0,     0,     0,   531,     0,     0,     0,     0,   532,     0,
       0,     0,   533,     0,     0,   534,     0,     0,   535,     0,
       0,     0,   536,   537,   538,   539,     0,     0,   540,     0,
       0,   541,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   542,   543,   544,   545,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   546,     0,     0,
       0,     0,     0,     0,     0,     0,   547,     0,   548,     0,
       0,     0,     0,     0,   549,   550,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   551,
     552,     0,     0,   553,   554,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   555,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   556,     0,     0,     0,     0,   557,     0,
     558,     0,     0,     0,     0,     0,     0,     0,   559,     0,
       0,   560,   561,   562,   563,   564,   565,   566,     0,     0,
       0,     0,     0,   567,     0,   568,     0,     0,     0,     0,
       0,     0,     0,   569,     0,     0,     0,   570,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   571,   572,   573,   574,   575,   576,
     577,   578,   579,   580,   493,     0,     0,     0,     0,   494,
     495,     0,     0,   496,   497,     0,     0,     0,     0,     0,
     498,   499,     0,   500,   501,     0,   502,     0,   503,     0,
       0,     0,     0,     0,   504,     0,     0,     0,     0,     0,
       0,   505,     0,   506,   507,     0,     0,     0,     0,     0,
       0,     0,     0,   508,     0,   509,     0,     0,     0,     0,
       0,     0,   510,     0,   511,   512,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   513,   514,   515,   516,
       0,   517,     0,   518,     0,   519,   520,   521,     0,   522,
       0,     0,   523,     0,     0,     0,     0,   524,     0,     0,
       0,     0,     0,     0,   525,     0,     0,   526,     0,     0,
       0,   527,   528,   529,   530,     0,     0,     0,     0,     0,
       0,   531,     0,     0,     0,     0,   532,     0,     0,     0,
     533,     0,     0,   534,     0,     0,   535,     0,     0,     0,
     536,   537,   538,   539,     0,     0,   540,     0,     0,   541,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   542,   543,   544,   545,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   546,     0,     0,     0,     0,
       0,     0,     0,     0,   547,     0,   548,     0,     0,     0,
       0,     0,   549,   550,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   551,   552,     0,
       0,   553,   554,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   555,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   556,     0,     0,     0,  3810,   557,     0,   558,     0,
       0,     0,     0,     0,     0,     0,   559,     0,     0,   560,
     561,   562,   563,   564,   565,   566,     0,     0,     0,     0,
       0,   567,     0,   568,     0,     0,     0,     0,     0,     0,
       0,   569,     0,     0,     0,   570,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   571,   572,   573,   574,   575,   576,   577,   578,
     579,   580,   493,     0,     0,     0,     0,   494,   495,     0,
       0,   496,   497,     0,     0,     0,     0,     0,   498,   499,
       0,   500,   501,     0,   502,     0,   503,     0,     0,     0,
       0,     0,   504,     0,     0,     0,     0,     0,     0,   505,
       0,   506,   507,     0,     0,     0,     0,     0,     0,     0,
       0,   508,     0,   509,     0,     0,     0,     0,     0,     0,
     510,     0,   511,   512,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   513,   514,   515,   516,     0,   517,
       0,   518,     0,   519,   520,   521,     0,   522,     0,     0,
     523,     0,     0,     0,     0,   524,     0,     0,     0,     0,
       0,     0,   525,     0,     0,   526,     0,     0,     0,   527,
     528,   529,   530,     0,     0,     0,     0,     0,     0,   531,
       0,     0,     0,     0,   532,     0,     0,     0,   533,     0,
       0,   534,     0,     0,   535,     0,     0,     0,   536,   537,
     538,   539,     0,     0,   540,     0,     0,   541,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   542,
     543,   544,   545,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   546,     0,     0,     0,     0,     0,     0,
       0,     0,   547,     0,   548,     0,     0,     0,     0,     0,
     549,   550,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   551,   552,     0,     0,   553,
     554,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   555,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   556,
       0,     0,     0,     0,   557,     0,   558,     0,     0,     0,
       0,     0,     0,     0,   559,     0,     0,   560,   561,   562,
     563,   564,   565,   566,     0,     0,     0,     0,     0,   567,
       0,   568,     0,     0,     0,     0,     0,     0,     0,   569,
       0,     0,     0,   570,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     571,   572,   573,   574,   575,   576,   577,   578,   579,   580,
     493,     0,     0,     0,     0,   494,   495,     0,     0,   496,
     497,     0,     0,     0,     0,     0,   498,   499,     0,   500,
     501,     0,   502,     0,   503,     0,     0,     0,     0,     0,
     504,     0,     0,     0,     0,     0,     0,   505,     0,   506,
     507,     0,     0,     0,     0,     0,     0,     0,     0,   508,
       0,   509,     0,     0,     0,     0,     0,     0,   510,     0,
     511,   512,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   513,   514,   515,   516,     0,   517,     0,   518,
       0,   519,   520,   521,     0,   522,     0,     0,   523,     0,
       0,     0,     0,   524,     0,     0,     0,     0,     0,     0,
     525,     0,     0,     0,     0,     0,     0,   527,   528,   529,
     530,     0,     0,     0,     0,     0,     0,   531,     0,     0,
       0,     0,   532,     0,     0,     0,   533,     0,     0,   534,
       0,     0,   535,     0,     0,     0,   536,   537,   538,   539,
       0,     0,   540,     0,     0,   541,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   542,   543,   544,
     545,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   546,     0,     0,     0,     0,     0,     0,     0,     0,
     547,     0,   548,     0,     0,     0,     0,     0,   549,   550,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   551,   552,     0,     0,   553,   554,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   555,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   556,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   559,     0,     0,   560,   561,   562,   563,   564,
     565,   566,     0,     0,     0,     0,     0,   567,     0,   568,
       0,     0,     0,     0,     0,     0,     0,   569,     0,     0,
       0,   570,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   571,   572,
     573,   574,   575,   576,   577,   578,   579,   580,   493,     0,
       0,     0,     0,   494,   495,     0,     0,   496,   497,     0,
       0,     0,     0,     0,     0,   940,     0,   500,   501,     0,
     502,     0,   503,     0,     0,     0,     0,     0,   504,     0,
       0,     0,     0,     0,     0,   505,     0,   506,   507,     0,
       0,     0,     0,     0,     0,     0,     0,   508,     0,   509,
       0,     0,     0,     0,     0,     0,     0,     0,   511,   512,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     513,   514,     0,   516,     0,   517,     0,   518,     0,   519,
     520,   521,     0,   522,     0,     0,  1748,     0,     0,     0,
       0,   524,     0,     0,     0,     0,     0,     0,   525,     0,
       0,     0,     0,     0,     0,   527,   528,   529,   530,     0,
       0,     0,     0,     0,     0,   531,     0,     0,     0,     0,
     532,     0,     0,     0,   533,     0,     0,   534,     0,     0,
     535,     0,     0,     0,     0,   537,   538,     0,     0,     0,
     540,     0,     0,   541,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   542,   543,   544,   545,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   546,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     548,     0,     0,     0,     0,     0,     0,   550,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   551,   552,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,  1749,  1750,     0,     0,
       0,     0,     0,     0,     0,  1751,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,  1752,  1753,   562,  1754,  1755,   565,   566,
       0,     0,     0,     0,     0,   567,     0,   568,     0,     0,
       0,     0,     0,     0,     0,   569,     0,     0,     0,   570,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   571,   572,   573,   574,
     575,   576,   577,   578,   579,   580,   493,     0,     0,     0,
       0,   494,   495,     0,     0,   496,   497,     0,     0,     0,
       0,     0,     0,   940,     0,   500,   501,     0,   502,     0,
     503,     0,     0,     0,     0,     0,   504,     0,     0,     0,
       0,     0,  1531,   505,     0,   506,   507,     0,     0,     0,
       0,     0,     0,     0,     0,   508,     0,   509,     0,     0,
       0,     0,     0,     0,     0,     0,   511,   512,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   513,   514,
       0,   516,     0,   517,     0,   518,     0,   519,   520,   521,
       0,   522,     0,     0,     0,     0,     0,     0,     0,   524,
       0,     0,     0,     0,     0,     0,   525,     0,     0,  1532,
       0,     0,     0,   527,   528,   529,   530,     0,     0,     0,
       0,     0,     0,   531,     0,     0,     0,     0,   532,     0,
       0,     0,   533,     0,     0,   534,     0,     0,   535,     0,
       0,     0,     0,   537,   538,     0,     0,     0,   540,     0,
       0,   541,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   542,   543,   544,   545,     0,     0,     0,
       0,     0,     0,     0,     0,     0,  1533,   546,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   548,     0,
       0,     0,     0,     0,     0,   550,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   551,
     552,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   493,     0,     0,     0,     0,   494,   495,
       0,     0,   496,   497,     0,     0,     0,  2122,     0,     0,
     940,     0,   500,   501,     0,   502,     0,   503,     0,     0,
       0,     0,     0,   504,     0,     0,   565,   566,     0,  1531,
     505,     0,   506,   507,     0,     0,     0,     0,     0,     0,
    1534,     0,   508,   569,   509,     0,     0,  1535,  1536,     0,
       0,     0,     0,   511,   512,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   571,   572,   573,   574,   575,   576,
     577,   578,   579,   580,     0,   513,   514,     0,   516,     0,
     517,     0,   518,     0,   519,   520,   521,     0,   522,     0,
       0,     0,     0,     0,     0,     0,   524,     0,     0,     0,
       0,     0,     0,   525,     0,     0,  1532,     0,     0,     0,
     527,   528,   529,   530,     0,     0,     0,     0,     0,     0,
     531,     0,     0,     0,     0,   532,     0,     0,     0,   533,
       0,     0,   534,     0,     0,   535,     0,     0,     0,     0,
     537,   538,     0,     0,     0,   540,     0,     0,   541,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     542,   543,   544,   545,     0,     0,     0,     0,     0,     0,
       0,     0,     0,  1533,   546,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   548,     0,     0,     0,     0,
       0,     0,   550,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   551,   552,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     493,     0,     0,     0,     0,   494,   495,     0,     0,   496,
     497,     0,     0,     0,  2879,     0,     0,   940,     0,   500,
     501,     0,   502,     0,   503,     0,     0,     0,     0,     0,
     504,     0,     0,   565,   566,     0,  1531,   505,     0,   506,
     507,     0,     0,     0,     0,     0,     0,  1534,     0,   508,
     569,   509,     0,     0,  1535,  1536,     0,     0,     0,     0,
     511,   512,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   571,   572,   573,   574,   575,   576,   577,   578,   579,
     580,     0,   513,   514,     0,   516,     0,   517,     0,   518,
       0,   519,   520,   521,     0,   522,     0,     0,     0,     0,
       0,     0,     0,   524,     0,     0,     0,     0,     0,     0,
     525,     0,     0,  1532,     0,     0,     0,   527,   528,   529,
     530,     0,     0,     0,     0,     0,     0,   531,     0,     0,
       0,     0,   532,     0,     0,     0,   533,     0,     0,   534,
       0,     0,   535,     0,     0,     0,     0,   537,   538,     0,
       0,     0,   540,     0,     0,   541,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   542,   543,   544,
     545,     0,     0,     0,     0,     0,     0,     0,     0,     0,
    1533,   546,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   548,     0,     0,     0,     0,     0,     0,   550,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   551,   552,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   493,     0,     0,
       0,     0,   494,   495,     0,     0,   496,   497,     0,     0,
       0,     0,     0,     0,   940,     0,   500,   501,     0,   502,
       0,   503,     0,     0,     0,     0,     0,   504,     0,     0,
     565,   566,     0,     0,   505,     0,   506,   507,     0,     0,
       0,     0,     0,     0,  1534,     0,   508,   569,   509,     0,
       0,  1535,  1536,     0,     0,     0,     0,   511,   512,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   571,   572,
     573,   574,   575,   576,   577,   578,   579,   580,     0,   513,
     514,     0,   516,     0,   517,     0,   518,     0,   519,   520,
     521,     0,   522,     0,     0,     0,     0,     0,     0,     0,
     524,     0,     0,     0,     0,     0,     0,   525,     0,     0,
       0,     0,     0,     0,   527,   528,   529,   530,     0,     0,
       0,     0,     0,     0,   531,     0,     0,     0,  1502,   532,
       0,     0,     0,   533,     0,     0,   534,     0,     0,   535,
       0,     0,     0,     0,   537,   538,     0,     0,     0,   540,
       0,     0,   541,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   542,   543,   544,   545,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   546,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   548,
       0,     0,     0,     0,     0,     0,   550,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     551,   552,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   493,     0,     0,     0,
       0,   494,   495,     0,  1542,   496,   497,     0,     0,     0,
       0,     0,     0,   940,     0,   500,   501,     0,   502,     0,
     503,     0,     0,     0,     0,     0,   504,   565,   566,     0,
       0,     0,     0,   505,     0,   506,   507,     0,     0,     0,
       0,     0,     0,     0,   569,   508,     0,   509,   570,     0,
       0,     0,     0,     0,     0,     0,   511,   512,     0,     0,
       0,     0,     0,     0,     0,   571,   572,   573,   574,   575,
     576,   577,   578,   579,   580,     0,     0,     0,   513,   514,
       0,   516,     0,   517,     0,   518,     0,   519,   520,   521,
       0,   522,     0,     0,     0,     0,     0,     0,     0,   524,
       0,     0,     0,     0,     0,     0,   525,     0,     0,     0,
       0,     0,     0,   527,   528,   529,   530,     0,     0,     0,
       0,     0,     0,   531,     0,     0,     0,     0,   532,     0,
       0,     0,   533,     0,     0,   534,     0,     0,   535,     0,
       0,     0,     0,   537,   538,     0,     0,     0,   540,     0,
       0,   541,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   542,   543,   544,   545,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   546,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   548,     0,
       0,     0,     0,     0,     0,   550,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   551,
     552,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   493,     0,     0,     0,     0,   494,   495,
       0,     0,   496,   497,     0,     0,     0,     0,     0,     0,
     940,     0,   500,   501,     0,   502,     0,   503,     0,  1921,
       0,     0,  2027,   504,     0,     0,   565,   566,     0,     0,
     505,     0,   506,   507,     0,     0,     0,     0,     0,     0,
       0,     0,   508,   569,   509,     0,     0,   570,     0,     0,
       0,     0,     0,   511,   512,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   571,   572,   573,   574,   575,   576,
     577,   578,   579,   580,     0,   513,   514,     0,   516,     0,
     517,     0,   518,     0,   519,   520,   521,     0,   522,     0,
       0,     0,     0,     0,     0,     0,   524,     0,     0,     0,
       0,     0,     0,   525,     0,     0,     0,     0,     0,     0,
     527,   528,   529,   530,     0,     0,     0,     0,     0,     0,
     531,     0,     0,     0,     0,   532,     0,     0,     0,   533,
       0,     0,   534,     0,     0,   535,     0,     0,     0,     0,
     537,   538,     0,     0,     0,   540,     0,     0,   541,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     542,   543,   544,   545,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   546,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   548,     0,     0,     0,     0,
       0,     0,   550,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   551,   552,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     493,     0,     0,     0,     0,   494,   495,     0,     0,   496,
     497,     0,     0,     0,     0,     0,     0,   940,     0,   500,
     501,     0,   502,     0,   503,     0,     0,     0,     0,     0,
     504,     0,     0,   565,   566,     0,     0,   505,     0,   506,
     507,     0,     0,     0,     0,     0,     0,     0,     0,   508,
     569,   509,     0,     0,   570,     0,     0,     0,     0,     0,
     511,   512,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   571,   572,   573,   574,   575,   576,   577,   578,   579,
     580,     0,   513,   514,     0,   516,     0,   517,     0,   518,
       0,   519,   520,   521,     0,   522,     0,     0,     0,     0,
       0,     0,     0,   524,     0,     0,     0,     0,     0,     0,
     525,     0,     0,     0,     0,     0,     0,   527,   528,   529,
     530,     0,     0,     0,     0,     0,     0,   531,     0,     0,
       0,     0,   532,     0,     0,     0,   533,     0,     0,   534,
       0,     0,   535,     0,     0,     0,     0,   537,   538,     0,
       0,     0,   540,     0,     0,   541,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   542,   543,   544,
     545,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   546,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   548,     0,     0,     0,     0,     0,     0,   550,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   551,   552,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   493,     0,     0,
       0,     0,   494,   495,     0,     0,   496,   497,     0,     0,
       0,     0,     0,     0,   940,     0,   500,   501,     0,   502,
       0,   503,     0,     0,     0,     0,     0,   504,  2110,     0,
     565,   566,     0,     0,   505,     0,   506,   507,     0,     0,
       0,     0,     0,     0,     0,     0,   508,   569,   509,     0,
       0,   570,     0,     0,     0,     0,     0,   511,   512,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   571,   572,
     573,   574,   575,   576,   577,   578,   579,   580,     0,   513,
     514,     0,   516,     0,   517,     0,   518,     0,   519,   520,
     521,     0,   522,     0,     0,     0,     0,     0,     0,     0,
     524,     0,     0,     0,     0,     0,     0,   525,     0,     0,
       0,     0,     0,     0,   527,   528,   529,   530,     0,     0,
       0,     0,     0,     0,   531,     0,     0,     0,     0,   532,
       0,     0,     0,   533,     0,     0,   534,     0,     0,   535,
       0,     0,     0,     0,   537,   538,     0,     0,     0,   540,
       0,     0,   541,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   542,   543,   544,   545,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   546,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   548,
       0,     0,     0,     0,     0,     0,   550,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     551,   552,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   493,     0,     0,     0,
       0,   494,   495,     0,  2243,   496,   497,     0,     0,     0,
       0,     0,     0,   940,     0,   500,   501,     0,   502,     0,
     503,     0,     0,     0,     0,     0,   504,   565,   566,     0,
       0,     0,     0,   505,     0,   506,   507,     0,     0,     0,
       0,     0,     0,     0,   569,   508,     0,   509,   570,     0,
       0,     0,     0,     0,     0,     0,   511,   512,     0,     0,
       0,     0,     0,     0,     0,   571,   572,   573,   574,   575,
     576,   577,   578,   579,   580,     0,     0,     0,   513,   514,
       0,   516,     0,   517,     0,   518,     0,   519,   520,   521,
       0,   522,     0,     0,     0,     0,     0,     0,     0,   524,
       0,     0,     0,     0,     0,     0,   525,     0,     0,     0,
       0,     0,     0,   527,   528,   529,   530,     0,     0,     0,
       0,     0,     0,   531,     0,     0,     0,     0,   532,     0,
       0,     0,   533,     0,     0,   534,     0,     0,   535,     0,
       0,     0,     0,   537,   538,     0,     0,     0,   540,     0,
       0,   541,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   542,   543,   544,   545,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   546,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   548,     0,
       0,     0,     0,     0,     0,   550,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   551,
     552,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   493,     0,     0,     0,     0,
     494,   495,     0,  3763,   496,   497,     0,     0,     0,     0,
       0,     0,   940,     0,   500,   501,     0,   502,     0,   503,
       0,     0,     0,     0,     0,   504,   565,   566,     0,     0,
       0,     0,   505,     0,   506,   507,     0,     0,     0,     0,
       0,     0,     0,   569,   508,     0,   509,   570,     0,     0,
       0,     0,     0,     0,     0,   511,   512,     0,     0,     0,
       0,     0,     0,     0,   571,   572,   573,   574,   575,   576,
     577,   578,   579,   580,     0,     0,     0,   513,   514,     0,
     516,     0,   517,     0,   518,     0,   519,   520,   521,     0,
     522,     0,     0,     0,     0,     0,     0,     0,   524,     0,
       0,     0,     0,     0,     0,   525,     0,     0,     0,     0,
       0,     0,   527,   528,   529,   530,     0,     0,     0,     0,
       0,     0,   531,     0,     0,     0,     0,   532,     0,     0,
       0,   533,     0,     0,   534,     0,     0,   535,     0,     0,
       0,     0,   537,   538,     0,     0,     0,   540,     0,     0,
     541,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   542,   543,   544,   545,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   546,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   548,     0,     0,
       0,     0,     0,     0,   550,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   551,   552,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   493,     0,     0,     0,     0,   494,
     495,     0,  3812,   496,   497,     0,     0,     0,     0,     0,
       0,   940,     0,   500,   501,     0,   502,     0,   503,     0,
       0,     0,     0,     0,   504,   565,   566,     0,     0,     0,
       0,   505,     0,   506,   507,     0,     0,     0,     0,     0,
       0,     0,   569,   508,     0,   509,   570,     0,     0,     0,
       0,     0,     0,     0,   511,   512,     0,     0,     0,     0,
       0,     0,     0,   571,   572,   573,   574,   575,   576,   577,
     578,   579,   580,     0,     0,     0,   513,   514,     0,   516,
       0,   517,     0,   518,     0,   519,   520,   521,     0,   522,
       0,     0,     0,     0,     0,     0,     0,   524,     0,     0,
       0,     0,     0,     0,   525,     0,     0,     0,     0,     0,
       0,   527,   528,   529,   530,     0,     0,     0,     0,     0,
       0,   531,     0,     0,     0,     0,   532,     0,     0,     0,
     533,     0,     0,   534,     0,     0,   535,     0,     0,     0,
       0,   537,   538,     0,     0,     0,   540,     0,     0,   541,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   542,   543,   544,   545,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   546,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   548,     0,     0,     0,
       0,     0,     0,   550,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   551,   552,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   493,     0,     0,     0,     0,   494,   495,
       0,  3931,   496,   497,     0,     0,     0,     0,     0,     0,
     940,     0,   500,   501,     0,   502,     0,   503,     0,     0,
       0,     0,     0,   504,   565,   566,     0,     0,     0,     0,
     505,     0,   506,   507,     0,     0,     0,     0,     0,     0,
       0,   569,   508,     0,   509,   570,     0,     0,     0,     0,
       0,     0,     0,   511,   512,     0,     0,     0,     0,     0,
       0,     0,   571,   572,   573,   574,   575,   576,   577,   578,
     579,   580,     0,     0,     0,   513,   514,     0,   516,     0,
     517,     0,   518,     0,   519,   520,   521,     0,   522,     0,
       0,     0,     0,     0,     0,     0,   524,     0,     0,     0,
       0,     0,     0,   525,     0,     0,     0,     0,     0,     0,
     527,   528,   529,   530,     0,     0,     0,     0,     0,     0,
     531,     0,     0,     0,     0,   532,     0,     0,     0,   533,
       0,     0,   534,     0,     0,   535,     0,     0,     0,     0,
     537,   538,     0,     0,     0,   540,     0,     0,   541,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     542,   543,   544,   545,     0,  1007,     0,     0,     0,     0,
       0,     0,     0,     0,   546,  1008,     0,     0,     0,  1009,
       0,     0,  1010,     0,     0,   548,  1011,     0,     0,     0,
       0,  1012,   550,     0,     0,     0,     0,  1013,     0,   156,
       0,     0,     0,     0,     0,     0,   551,   552,     0,     0,
       0,     0,     0,     0,   161,     0,     0,     0,     0,  1014,
       0,     0,  1015,     0,     0,  1016,     0,     0,     0,     0,
    1017,     0,     0,     0,  1018,     0,     0,     0,   166,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,  1019,   167,   168,     0,     0,     0,     0,
       0,  1020,     0,   565,   566,     0,  1021,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     569,     0,     0,     0,   570,     0,     0,     0,  1022,     0,
       0,  1023,     0,   174,  1024,     0,     0,  1025,     0,     0,
       0,   571,   572,   573,   574,   575,   576,   577,   578,   579,
     580,     0,     0,     0,     0,     0,   178,     0,     0,     0,
       0,     0,     0,     0,     0,     0,  1026,  1027,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,  1028,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,  1007,     0,     0,     0,
       0,     0,     0,     0,     0,     0,  1008,     0,     0,     0,
    1009,     0,     0,  1010,   185,   186,   187,  1011,     0,     0,
       0,     0,  1012,     0,     0,     0,     0,     0,  1013,     0,
     156,     0,     0,     0,     0,     0,     0,     0,     0,     0,
    1029,     0,     0,     0,     0,   161,     0,  1632,     0,     0,
    1014,     0,  1030,  1015,     0,     0,  1016,     0,     0,     0,
    1031,  1017,     0,   565,   566,  1018,     0,     0,     0,   166,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,  1019,   167,   168,     0,     0,     0,
       0,     0,  1020,     0,     0,     0,     0,  1021,     0,     0,
       0,   571,   572,   573,   574,   575,   576,   577,   578,   579,
     580,     0,     0,     0,     0,     0,     0,     0,     0,  1022,
       0,     0,  1023,     0,   174,  1024,     0,     0,  1025,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   178,     0,     0,
       0,     0,     0,     0,     0,     0,     0,  1026,  1027,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,  1028,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,  1007,     0,     0,
       0,     0,     0,     0,     0,     0,     0,  1008,     0,     0,
       0,  1009,     0,     0,  1010,   185,   186,   187,  1011,     0,
       0,     0,     0,  1012,     0,     0,     0,     0,     0,  1013,
       0,   156,     0,     0,     0,     0,     0,     0,     0,     0,
       0,  1029,     0,     0,     0,  2153,   161,     0,     0,     0,
       0,  1014,     0,  1030,  1015,     0,     0,  1016,     0,     0,
       0,     0,  1017,     0,   565,   566,  1018,     0,     0,     0,
     166,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,  1019,   167,   168,     0,     0,
       0,     0,     0,  1020,     0,     0,     0,     0,  1021,     0,
       0,     0,   571,   572,   573,   574,   575,   576,   577,   578,
     579,   580,     0,     0,     0,     0,     0,     0,     0,     0,
    1022,     0,     0,  1023,     0,   174,  1024,     0,     0,  1025,
       0,     0,     0,     0,     0,  1007,     0,     0,     0,     0,
       0,     0,     0,     0,     0,  1008,     0,     0,   178,  1009,
       0,     0,  1010,     0,     0,     0,  1011,     0,  1026,  1027,
       0,  1012,     0,     0,     0,     0,     0,  1013,     0,   156,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,  1028,   161,     0,     0,     0,     0,  1014,
       0,     0,  1015,     0,     0,  1016,     0,     0,     0,     0,
    1017,     0,     0,     0,  1018,     0,     0,     0,   166,     0,
       0,     0,     0,     0,     0,     0,   185,   186,   187,     0,
       0,     0,     0,  1019,   167,   168,     0,     0,     0,     0,
       0,  1020,     0,     0,     0,     0,  1021,     0,     0,     0,
       0,     0,  1029,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,  1030,     0,     0,     0,  1022,     0,
       0,  1023,     0,   174,  1024,   565,   566,  1025,     3,     4,
       5,     0,     0,     0,     6,     7,     0,     9,     0,     0,
      10,    11,     0,     0,    13,    14,   178,     0,    16,     0,
       0,    18,    19,    20,    21,     0,  1026,  1027,     0,     0,
       0,    23,     0,   571,   572,   573,   574,   575,   576,   577,
     578,   579,   580,     0,     0,     0,     0,     0,     0,     0,
       0,  1028,     0,     0,    24,    25,    26,    27,    28,    29,
      30,    31,    32,    33,    34,    35,    36,    37,    38,    39,
      40,    41,    42,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   185,   186,   187,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   148,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
    1029,     0,     0,   149,     0,     0,   150,   151,     0,   152,
       0,     0,  1030,   153,     0,     0,     0,     0,   154,   155,
     156,   157,     0,   565,   566,   158,     0,   159,   160,     0,
       0,     0,     0,     0,     0,   161,     0,     0,     0,   162,
       0,     0,     0,   163,     0,     0,   164,     0,     0,     0,
       0,     0,   165,     0,     0,     0,     0,     0,     0,   166,
       0,   571,   572,   573,   574,   575,   576,   577,   578,   579,
     580,     0,     0,     0,     0,   167,   168,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   169,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   170,     0,   171,
     172,     0,   173,     0,   174,   175,     0,     0,   176,     0,
       0,     0,   716,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   177,     0,   178,     0,     0,
       0,     0,   150,   717,     0,     0,     0,     0,   179,   718,
       0,     0,     0,     0,     0,   155,   156,   157,     0,     0,
       0,     0,     0,   159,     0,     0,     0,     0,     0,     0,
       0,   161,     0,   180,     0,     0,     0,     0,     0,     0,
       0,     0,   719,     0,   181,   182,     0,     0,   165,     0,
       0,   183,     0,     0,     0,   166,     0,     0,     0,     0,
     184,     0,     0,     0,     0,   185,   186,   187,   188,   189,
       0,   167,   168,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   190,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   171,   172,     0,   720,     0,
     174,   175,     0,     0,   176,   191,     0,   192,     0,   193,
     194,     0,     0,     0,     0,   195,   716,     0,     0,     0,
     196,     0,     0,   178,     0,     0,     0,   197,     0,     0,
     198,     0,     0,     0,   179,     0,   150,   717,     0,     0,
       0,     0,     0,   718,     0,     0,     0,     0,     0,   155,
     156,   157,     0,     0,     0,     0,     0,   159,     0,   180,
       0,     0,     0,     0,     0,   161,     0,     0,     0,     0,
     181,   182,     0,     0,     0,     0,   719,   183,     0,     0,
       0,     0,   165,     0,     0,     0,   184,     0,     0,   166,
       0,   185,   186,   187,   188,   189,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   167,   168,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   190,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   721,     0,   171,
     172,   191,   720,     0,   174,   175,     0,     0,   176,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   197,     0,     0,     0,   178,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   179,     0,
       0,     0,     2,     0,     0,     3,     4,     5,     0,     0,
       0,     6,     7,     8,     9,     0,     0,    10,    11,    12,
       0,    13,    14,   180,    15,    16,     0,    17,    18,    19,
      20,    21,     0,     0,   181,   182,     0,     0,    23,     0,
       0,   183,     0,     0,     0,     0,     0,     0,     0,     0,
     184,     0,     0,     0,     0,   185,   186,   187,   188,   189,
       0,    24,    25,    26,    27,    28,    29,    30,    31,    32,
      33,    34,    35,    36,    37,    38,    39,    40,    41,    42,
       0,   190,     0,     3,     4,     5,     0,     0,     0,     6,
       7,     0,     9,     0,     0,    10,    11,     0,     0,    13,
      14,   725,     0,    16,     0,   191,    18,    19,    20,    21,
       0,  1330,     0,     0,     0,     0,    23,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   197,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    24,
      25,    26,    27,    28,    29,    30,    31,    32,    33,    34,
      35,    36,    37,    38,    39,    40,    41,    42
};

static const yytype_int16 yycheck[] =
{
     155,   362,   624,  1239,   961,    99,  1079,  1201,  1513,  1021,
    1515,   968,   981,   760,  1064,  1828,  1254,   972,  1054,  1605,
     951,   952,  1025,   643,  1257,   130,  2048,  1022,  1972,  1710,
     623,   130,  2100,   365,  2212,  1537,  1192,  1008,  2113,  1387,
    2214,  1178,   374,  1661,   171,  1026,   958,  2101,  1019,   171,
     753,  2075,  2533,  1577,   157,    53,  1251,  1184,   161,  2403,
    2114,    59,   346,   166,   167,   168,  2120,    54,   131,     0,
     946,  1973,   175,   770,   598,   178,     7,  1469,  1980,  2685,
    1542,  2134,   359,   125,   180,  1547,  1472,  1378,  1478,  2378,
    2409,   386,   173,  2412,  1484,   765,  2149,   913,    67,  3052,
    1031,  2420,  1045,  1996,  2174,  3177,  2307,  3179,  2470,  2491,
    1260,  1501,  2451,  2133,    99,  1474,    99,  1557,    99,  2695,
     214,   108,    53,   217,    55,   123,  2628,  1482,    59,  2631,
     895,  2737,    63,   227,  2649,  2650,    67,  1787,  3297,    54,
    2807,  2486,  2852,  2749,  1602,  3257,  3460,   853,  2487,  1841,
     850,  2947,    83,    39,  2905,   432,    39,   354,   374,    39,
    3245,  3246,  3247,  2057,    65,  3603,  3030,  3031,  3032,   132,
     117,  2646,   127,   269,   108,   113,   116,   112,   166,    39,
       0,  2662,  3600,    39,    60,   109,   130,   139,    60,   201,
      39,  2245,   123,   201,   125,   168,   105,   128,   138,   119,
     132,  2906,  2907,   118,   119,   136,   109,   142,   171,   229,
     306,   307,   308,   114,    61,    62,  3393,   143,   129,   137,
     206,  1190,   222,    39,   125,   126,    39,   119,   324,   214,
    3289,   214,   217,   214,   217,    55,   217,  3353,  3354,   132,
      60,    61,   227,    25,   227,    65,   227,   129,   126,    52,
      53,    54,   125,   132,   359,   151,    38,    39,   123,    41,
     359,  3763,   668,   669,  3702,   102,   144,   366,   130,   127,
      52,    53,  2895,   372,   373,   153,   154,  2478,   166,   168,
     158,   159,  2905,   105,    25,   238,   108,   386,   108,   139,
    3728,   139,   171,  3452,   114,  1042,  2520,    38,    39,  2523,
      41,   315,   160,   237,   218,   125,   126,     0,   228,  1504,
     147,    52,    53,   213,   238,   181,   301,   206,   301,   261,
     301,   151,   152,   249,   258,   222,  3959,   432,   165,   206,
     268,  2676,   296,   432,   179,   238,   351,   187,   351,   187,
     213,   354,  1045,   498,   359,   309,   264,   345,   248,  3982,
     446,   447,   448,   449,   352,   451,   452,  3859,   454,   455,
     257,   375,    55,   241,   406,   125,   280,   375,   310,   265,
     130,   391,   139,   238,   232,   375,  3553,   259,   474,   475,
     476,   477,  3498,    25,  2729,   319,   359,   375,   297,   375,
     301,  1013,   157,  2775,   406,   162,    38,    39,   159,    41,
    3196,   238,   163,   149,   241,  2880,  3844,  3112,   352,   361,
      52,    53,   375,   357,   345,  1008,   351,   239,  3477,  3837,
     375,   352,   375,   363,   387,   786,  1019,   425,   375,   556,
    1625,   362,  1025,   364,   349,   375,   105,   375,   425,   108,
     355,   375,  2957,   375,   599,   144,   319,   971,  2787,   375,
     382,   375,   392,   213,   153,   375,   788,   375,  2533,   374,
     792,  1054,   794,   795,   796,   357,   798,  2812,   357,   357,
    3784,  3785,   375,  3911,   319,   406,   808,  2952,   308,   149,
    2534,  3434,   375,   250,  2552,   846,   770,   206,  3111,   382,
     352,  2830,   206,   359,   425,   357,   375,   375,   375,   430,
     777,   351,  1095,   351,  3942,   406,   602,   603,   387,   605,
     606,   607,   608,   609,   610,  3627,   612,   375,   614,  1713,
    1714,   286,  1142,   807,  2577,   118,   137,   237,   823,   824,
     825,  2212,   352,   129,   392,   192,   422,   821,   375,   210,
     667,  1577,   364,   429,  2745,  2565,  1166,  1167,  1168,  1169,
    1170,  1171,  1172,  1173,  1174,  1175,  1176,  1177,   842,  1595,
    3424,  1504,   445,   423,   440,   445,   422,  1524,  1525,   441,
     239,  1540,  3731,   422,   851,   424,   792,  1508,   775,  2599,
    2600,  2475,   193,  1289,   129,   782,   406,  2662,  1288,   436,
     437,   351,  3302,  3907,  3908,   691,   692,  1578,   694,  3395,
    2654,  3352,  3714,  1546,  2254,   425,   422,  2299,   375,   422,
    2811,   168,   309,   429,   271,  1139,   429,  3223,    99,   422,
    3287,   424,  2080,   426,  3783,   371,   213,   443,   441,   390,
    2854,  2855,  1644,    82,   113,   381,   717,   383,  2862,   720,
     422,  1406,   235,   176,   117,   241,   428,   429,   430,   431,
     432,   433,  2007,   207,   233,  3231,   375,   314,  2276,  3265,
     357,   375,  2102,   445,   204,   375,   206,  1610,  2054,   204,
     228,   206,   777,   372,  3176,  3190,  3191,   210,   777,   241,
     371,   422,  3027,  1626,   375,   203,  1643,   428,   429,   430,
     431,   432,   433,  2052,  1844,   364,   365,   352,   237,   241,
     371,   371,   259,   444,   445,   301,  3791,   235,   319,  3091,
    3092,   381,   105,   383,   259,   108,  3078,   372,   375,   204,
     136,   206,   393,  1654,   823,   824,   825,   826,   827,   828,
     829,   830,   831,   832,   833,   834,   835,   836,   837,   838,
     839,   840,   841,  1712,   786,  3089,   851,   228,   213,   734,
     168,   734,   851,   734,  3093,  1710,  3841,  3842,   183,  3048,
    1166,  1167,  1168,  1169,  1170,  1171,  1172,  1173,  1174,  1175,
    1176,  1177,   359,   106,   107,   181,   192,   108,   139,   297,
     422,   260,   244,   116,   200,  2986,   428,   429,   430,   431,
     432,   433,    25,  1843,  1751,   268,   375,   222,  2479,   217,
     357,   443,   898,   357,   846,    38,    39,   934,    41,   111,
     906,   938,   934,   375,   147,   802,   938,   375,   914,    52,
      53,   154,  2503,   351,   244,   241,   143,   923,  3029,   235,
     370,   927,   244,   375,   961,   370,   375,   104,   371,   180,
     375,   968,   969,  1546,  2897,  2915,   239,   969,  2916,  3518,
    3519,   277,   850,   170,  1009,   786,  3928,   375,   398,   183,
     393,  3725,   268,   850,    42,    43,    44,    45,    46,    47,
      48,    49,    50,    51,   235,   806,   248,   792,   809,   123,
     182,  2901,  2902,  1630,   853,   358,  2779,   206,   180,   220,
     375,  1018,   988,  3846,   359,  1022,  1018,   370,   222,  1026,
    1022,   266,   219,   999,  1026,   125,   180,   268,   213,   325,
    1006,  2106,   253,   375,  1275,   846,   201,   848,  2266,   850,
      82,  1024,   853,  1626,    25,  3789,  3790,   258,  2609,   248,
    1011,   238,   249,   242,   270,   252,   140,    38,    39,   364,
      41,   204,  1023,   206,   370,   351,   105,  1279,   289,   108,
     375,    52,    53,  2634,  1885,   375,  3300,  3910,   376,   375,
     293,   253,   202,   375,   325,   326,   327,  3636,  3307,   204,
     388,   206,   238,  2168,  3263,   179,  1646,   395,   396,   253,
     313,   314,   802,   401,  1577,   297,  3939,   276,   319,   270,
     351,   409,   357,  2579,   238,  1290,  1273,   289,  1153,  3318,
    1155,  1817,  1595,   375,    25,  1160,  1161,  1162,  1163,  1164,
    1165,   352,  3618,   151,  3967,   289,   301,    38,    39,   285,
      41,  1148,  1149,   204,   244,   206,   194,   248,  2530,  1184,
    2157,    52,    53,  1653,  3716,  3717,  2384,   168,   357,  1135,
     364,   346,  1992,   445,  1140,  1141,   351,  2571,   179,   371,
     357,   375,  1329,   257,   359,   375,    99,  3538,   375,   325,
     352,    25,   398,   375,  1049,  2259,  1049,  2300,  1049,   378,
    2207,  2461,  2363,   154,    38,    39,   396,    41,   352,  2471,
     239,   370,   213,  1179,  2476,   394,   266,  1242,    52,    53,
    3143,   222,   414,  2249,   375,   140,   405,  2483,    99,  2236,
    3154,  3155,   342,   343,  2099,  2055,   125,   374,   375,   375,
    2160,   130,   375,   281,  2619,  2620,  3722,   398,  3724,  1246,
    1247,   325,   326,   327,  1246,  1247,   140,   265,  2178,   369,
     370,   244,   372,   373,   179,   370,   357,  2149,  3162,  1235,
     190,  2098,  2145,  2155,  3488,   412,   413,   414,   415,   416,
     417,   418,   419,   420,   421,  1852,  3856,   194,   271,    25,
    2103,    99,  2143,  2158,  2107,   179,  2137,   223,  1273,  2631,
     251,  2142,    38,    39,  1273,    41,  3769,   357,  2173,   549,
    2151,   312,   238,   364,  2115,  1709,    52,    53,   319,   422,
     235,  1290,  3874,  3875,   213,   428,   429,   430,   431,   432,
     433,   285,     5,     6,  3106,  2081,   110,    10,    11,    12,
     443,   278,   257,   214,    17,    18,   217,  3917,   105,    22,
     351,   108,    25,   268,  1329,    28,   181,   125,   274,   248,
    1329,    34,   130,  1275,    37,    38,    39,   293,    41,    99,
      25,   325,   113,   257,  2213,   116,   179,   139,   298,    52,
      53,  2301,   144,    38,    39,   105,    41,  2212,   108,   140,
     105,   153,   375,   108,   442,  1328,   176,    52,    53,  3323,
     274,   313,   314,  3866,  1272,   375,   235,  3977,  3978,  2960,
     213,   260,   136,   154,  3628,   398,  2198,  2199,    99,   222,
     144,   375,  3345,  1389,   192,   199,    25,   100,   179,   153,
     210,  1288,   402,   370,   259,   351,   351,   268,   375,    38,
      39,    39,    41,   350,   351,   213,   353,   235,   216,   375,
     128,   422,   359,    52,    53,    99,  3380,   428,   429,   430,
     431,   432,   433,   375,  1489,    25,   144,  1492,   192,   235,
     319,  1272,   443,  2538,  1275,   153,   200,   351,    38,    39,
     268,    41,   239,   245,   214,    99,   254,   217,     7,     8,
       9,   140,    52,    53,    13,    14,  1462,    16,  2873,  2874,
      19,    20,   106,  3317,    23,    24,   257,    99,    27,   312,
    1507,    30,    31,    32,    33,  1507,   319,   241,  2515,   239,
      39,    40,   351,   285,   239,  3076,   357,  1524,  1525,  1502,
     179,   422,   423,   235,  2107,  1560,   140,   428,   429,   430,
     431,   432,   433,   140,    63,    64,    65,    66,    67,    68,
      69,    70,    71,    72,    73,    74,    75,    76,    77,    78,
      79,    80,    81,   351,   177,   165,   268,   122,   315,   105,
     214,   183,   172,   217,   174,   179,   176,   190,  3929,   179,
     180,  1578,   179,  1370,   278,   351,  1578,   375,   422,   283,
     273,  3649,  3650,  3538,   428,   429,   430,   431,   432,   433,
     214,   325,  2996,   217,   336,   271,   251,   180,   257,   443,
     222,   213,   167,   260,  3931,   162,  2498,   138,  2625,  3970,
     222,   362,   214,  3111,   290,   217,  3520,  3178,   375,   147,
     162,   235,  1863,  1864,  1865,   179,   209,   192,  2645,  2665,
    3957,   251,  2562,   375,  3327,   181,  1643,  3198,   314,   351,
     295,   375,  1927,   257,  2479,   315,  1858,  1859,   399,  1861,
     257,   216,  2568,  3707,   268,  2571,   407,   408,   351,   269,
     283,   412,   319,  1460,  2137,  2581,   370,  3735,  2503,  2142,
     253,   375,  2145,   201,   202,   295,   422,   207,  2151,   425,
     235,   278,   428,   429,   430,   431,   432,   433,  1852,   254,
     427,  1726,  1727,   250,   944,  1980,   306,   307,   308,   375,
     219,    91,  2202,   257,  1868,   375,   289,   444,   250,   379,
     238,   105,   233,   268,   324,  2215,  2608,   236,   328,   317,
     970,   296,  2583,  2584,     0,   244,  2844,   177,    91,   259,
     249,  2582,   182,  3294,   309,   351,   257,   351,  2613,   422,
     190,    65,  3124,   359,  1751,   428,   429,   430,   431,   432,
     433,   201,   146,  2545,  2546,   164,   351,   422,   423,   319,
     443,   319,   445,   428,   429,   430,   431,   432,   433,   288,
     325,   326,   327,   370,  2609,   319,   138,   375,   375,    55,
     189,   300,   357,   139,    60,    61,  3171,  3172,   144,   357,
     114,   389,  3850,  1828,   352,  3023,   351,   153,   180,  2634,
     352,   362,   126,  3188,  3189,   357,  3754,   375,   352,   191,
    2621,  3635,   194,   422,  3375,   375,   425,   375,   319,   428,
     429,   430,   431,   432,   433,   112,   261,   262,   371,   271,
     272,   375,    88,   283,    90,   445,   446,   447,   448,   449,
    2651,   451,   452,   453,   454,   455,  2697,   457,   458,   319,
     393,   352,   422,   423,   112,   142,   375,   467,   428,   429,
     430,   431,   432,   433,   474,   475,   476,   477,  2984,   194,
    1846,   253,   482,   483,   375,   310,   486,   319,  3780,   105,
    3346,  1029,   352,   386,   142,  1870,   169,   212,   427,   245,
    3003,  1870,  2966,   422,   358,   178,   111,   400,  1046,   349,
     429,   184,   236,   186,   443,   375,   370,   289,   372,   526,
     244,   375,   441,   363,   364,   365,  1166,  1167,  1168,  1169,
    1170,  1171,  1172,  1173,  1174,  1175,  1176,  1177,   144,   285,
     427,   287,   427,   359,   362,   427,   362,   153,   122,   549,
     557,   558,    87,   427,    25,  1195,   427,   444,   231,   444,
    3643,   443,  3337,  3338,    99,   430,   431,    38,    39,   443,
      41,   202,   443,   371,   427,   427,   427,   375,  2898,  2004,
    2005,    52,    53,   381,  3929,   383,   350,   351,   351,  3913,
     443,   443,   443,   364,  2900,   359,   359,  1963,   598,   599,
     600,   601,   602,   603,   375,   605,   606,   607,   608,   609,
     610,   611,   612,   613,   614,   409,   616,   617,   192,   356,
    2893,   358,  2895,   623,   624,  3970,   103,   201,   202,  3953,
    3954,   362,  2905,   370,   356,   356,   358,   358,   115,   351,
    2867,    25,   216,  2068,   107,  2518,  2519,   423,   370,   370,
     362,   225,  2525,   358,    38,    39,   119,    41,  3514,  2025,
    2026,  2926,  1863,  1864,  1865,   370,  2556,  2557,    52,    53,
     144,   358,  2899,   233,  2875,  2995,   358,   371,   362,   153,
     254,   375,  2313,   370,   244,   685,   169,   381,   370,   383,
    2996,   691,   692,   693,   694,   178,  2739,   144,   698,   699,
     445,  2098,  2099,   186,  2747,  2568,   153,  2099,  2571,  2134,
     442,   342,   343,  2315,  2316,  2112,   371,    25,  2581,  2582,
    2112,   351,   425,   206,  2149,   356,   381,   358,   383,   359,
      38,    39,  2157,    41,   144,  2960,   371,   371,   369,   370,
     375,   372,   373,   153,    52,    53,  2143,   381,   231,   383,
     371,  2143,   352,  2935,  2936,   351,    89,   357,   374,   375,
     381,  2158,   383,   359,  3539,  3540,  2158,   372,   371,   374,
     375,   423,  2954,  2955,   371,   427,  2173,  3285,   381,   350,
     383,  2173,   351,   351,   381,   353,   383,   353,   359,    90,
     359,   359,  2217,   359,   362,  2220,   412,   413,   414,   415,
     416,   417,   418,   419,   420,   421,   215,   412,   413,   414,
     415,   416,   417,   418,   419,   420,   421,  3099,   388,  1459,
    3140,   233,   169,   350,   351,   395,   353,   143,   351,  1469,
     242,   178,   359,   144,  2697,   351,   359,   388,  3111,   186,
       5,     6,   153,   359,   395,    10,    11,    12,  2212,   355,
    3193,  3076,    17,    18,   360,   198,  1496,    22,   353,   206,
      25,  2286,   205,    28,   359,   429,   422,   215,   424,    34,
     434,   435,    37,    38,    39,   371,    41,   370,   371,   375,
    2277,   351,   375,   353,   231,  2277,   423,    52,    53,   359,
     427,   361,   352,   407,  2291,  2292,  2293,   357,   898,  2291,
    2292,  2293,   370,  2920,   372,   905,   906,   375,   389,   909,
     910,  3417,   349,  1553,   914,    25,   352,   269,   355,   919,
     352,   357,  1562,   923,   413,   357,   926,   927,    38,    39,
      35,    41,   350,   351,    39,   353,  3400,  3401,   204,   169,
     206,   359,    52,    53,   944,   945,  3128,  3129,   178,   352,
     352,   422,   423,  3178,   357,   357,   186,   428,   429,   430,
     431,   432,   433,   352,   352,   371,    25,  2364,   357,   357,
     970,   971,  2364,  3198,   423,   975,   206,   206,   427,    38,
      39,   981,    41,   983,   204,   375,   206,   143,   988,   350,
     351,  3192,   353,    52,    53,   352,   368,   352,   359,   999,
     357,   231,   357,   161,  1644,   371,  1006,   352,  1008,   375,
    1010,  1651,   357,  1013,  1014,  1015,  1016,  1017,   352,  1019,
     356,  1021,   358,   357,   375,  1025,   352,  1027,  1028,   110,
    2893,   357,  2895,   114,   161,   116,   357,  2900,   422,  2931,
     191,   362,  2905,   194,   428,   429,   430,   431,   432,   433,
     352,   370,   133,   244,  1054,   357,   375,   352,   374,   375,
      56,    57,   357,   219,  1064,   350,   351,   423,   353,  3294,
     244,   427,  2497,   154,   359,  2706,  2707,   244,  2709,  1079,
     236,  2712,  1082,  1083,   352,  1085,    58,    59,   244,   357,
    2515,   202,  1092,   249,   244,  1095,   412,   413,   414,   415,
     416,   417,   418,   419,   420,   421,  2472,   359,   264,  2711,
     362,   352,  2313,   423,   422,   423,   357,   427,   199,   244,
     428,   429,   430,   431,   432,   433,   330,   331,   332,   333,
     334,   335,   288,  2996,   423,  1135,  1136,  1137,   427,  1139,
    1140,  1141,   352,   224,   300,   166,  1786,   357,   370,   352,
    3375,   352,  2577,   375,   357,   352,   357,   352,  3397,   352,
     357,   422,   357,   424,   357,  3404,  1166,  1167,  1168,  1169,
    1170,  1171,  1172,  1173,  1174,  1175,  1176,  1177,  1178,  1179,
     352,   370,   371,   352,  1184,   357,  2583,  2584,   357,  1189,
    1190,  2583,  2584,   244,  1194,  1195,  3516,  1197,  1198,   352,
     352,  1841,  3384,  3385,   357,   357,   352,   422,   357,   424,
    3674,   357,   293,   362,  2639,  2640,  2613,   352,   352,   375,
     352,  2613,   357,   357,   352,   357,   386,   372,   352,   357,
     375,   342,   343,   357,   352,  1235,   352,   273,  2663,   357,
     351,   357,   323,   362,   352,   356,   352,   358,  3111,   357,
     121,   357,   423,   370,   371,  1255,   427,   422,   369,   370,
     375,   372,   373,   428,   429,   430,   431,   432,   433,   352,
     352,   370,   352,   371,   357,   357,   375,   357,   443,   352,
     445,   352,   352,   352,   357,   370,   357,   357,   357,  2653,
     375,   352,   352,   374,   375,   352,   357,   357,  1938,   362,
     357,   423,   352,  3542,  1944,   427,  3545,   357,  3547,   423,
    3549,   352,   422,   427,  3541,   204,   357,   206,   428,   429,
     430,   431,   432,   433,   352,   352,   204,  3647,   206,   357,
     357,   412,   413,   414,   415,   416,   417,   418,   419,   420,
     421,   352,   830,   831,   832,   833,   357,  1347,  1167,  1168,
    1169,  1170,  1171,  1172,  1173,  1174,  1175,  1176,  1177,   352,
     352,   352,   352,   422,   357,   357,   357,   357,   275,   428,
     429,   430,   431,   432,   433,   214,   362,  3008,   217,  3010,
    3011,     4,   253,   121,     7,     8,     9,   370,   371,  1389,
      13,    14,    15,    16,   352,   362,    19,    20,    21,   357,
      23,    24,  1402,    26,    27,   402,    29,    30,    31,    32,
      33,   352,   166,   352,   135,   355,   357,    40,   357,   352,
     360,   352,   402,   294,   357,   423,   357,   214,   362,   427,
     217,   302,   158,   159,  1380,   306,  3675,  1383,   201,   202,
      63,    64,    65,    66,    67,    68,    69,    70,    71,    72,
      73,    74,    75,    76,    77,    78,    79,    80,    81,  1459,
    2100,  2886,  1462,   417,   418,   419,   420,   421,  1468,  1469,
    2867,   204,  2897,   206,   425,   143,   427,   204,   362,   206,
     362,   356,  1482,   358,    83,  1485,   140,   415,   416,  2129,
     176,   177,   298,   299,   276,   277,  1496,   276,   277,   826,
     827,   443,  2899,   374,   375,  2706,  2707,   442,  2709,   834,
     835,  2712,   441,   828,   829,   253,  3755,  2737,  2738,  2159,
    3871,  3872,  3975,  3976,  1790,  1791,   964,   965,   443,  2926,
     251,   966,   967,    41,  2926,  3148,  3149,   422,   428,   422,
    1540,   412,   413,   414,   415,   416,   417,   418,   419,   420,
     421,   219,   422,  1553,   422,    55,   294,  1557,   438,   423,
    1560,   427,  1562,   217,   302,   439,   425,   423,   236,   441,
     427,   113,  1572,   362,   364,   243,   244,  1577,   375,  2976,
     143,   249,  2948,  2947,  2976,   264,   287,   375,   319,   375,
     254,   294,   243,  2990,  2234,  1595,   264,   319,  2990,   288,
     315,   264,   264,  2243,   137,  3236,   351,   359,   367,   371,
    3856,  1611,   280,   371,  3853,  3854,  2256,   351,   350,   347,
     288,   243,   264,   137,   375,   263,  3051,  3052,   375,   181,
     284,   259,   300,   363,   116,   375,   374,   375,   362,   168,
     218,  3887,  3888,   108,  1644,   108,   108,   108,   108,   108,
     108,  1651,  3891,   374,   375,   181,   181,   259,   371,  2299,
     351,   208,   107,   350,   364,   364,   351,  2307,   248,   357,
     375,  3917,  3097,   371,   412,   413,   414,   415,   416,   417,
     418,   419,   420,   421,   359,   357,   166,   168,   362,   273,
    3115,   412,   413,   414,   415,   416,   417,   418,   419,   420,
     421,   206,  1702,  3949,   375,   105,   257,   375,   268,  1709,
     351,    34,  1712,  3959,   111,   423,   441,   375,  3143,   375,
     374,   375,   441,  2363,   351,   443,   357,   364,  3974,  3975,
    3976,  3977,  3978,  2373,   388,   375,  3982,   128,   137,  2379,
     375,   395,   396,   375,   128,   375,   375,   137,   128,   375,
     375,   137,  3177,   117,  3179,   375,   288,   351,   412,   413,
     414,   415,   416,   417,   418,   419,   420,   421,   166,   287,
     375,   404,   279,   265,  1774,   371,  1776,   359,   235,   317,
     399,   213,   357,   168,  3209,   181,  1786,  1787,   119,   119,
     375,   279,   137,   166,   237,   166,   168,   362,   362,   370,
     362,  1801,   363,   362,   362,   362,   362,  3008,  3205,  3010,
    3011,   374,   375,  3205,  3178,   370,   362,   362,   362,   442,
     243,   362,   445,   362,   361,   170,   362,   362,   264,   244,
     361,  2471,  3196,   243,   181,   244,  2476,  3262,  2478,   244,
     219,  1841,  1842,  1843,   268,   364,  1846,   234,   137,   412,
     413,   414,   415,   416,   417,   418,   419,   420,   421,   188,
     111,   244,   351,  3288,   354,   213,   248,   268,   371,   294,
     357,   375,  2512,   375,   219,   370,   221,   117,   259,   222,
     238,   251,   206,   229,   362,   102,  2526,   372,   105,   106,
     256,   236,   237,   423,   422,   351,   423,   352,   352,   244,
     423,   425,  3327,   371,   249,   371,   248,   124,   248,   123,
     279,   375,  2552,   258,  1914,  3340,   268,   364,   364,   264,
    3345,  1921,  1922,   372,   372,   375,   288,   144,  1928,  1929,
     147,   375,   213,  1933,   233,  1935,   153,   233,  1938,   214,
    1940,  1941,  1942,   288,  1944,  1945,  1946,  1947,   213,   370,
    1950,  1951,   213,  1953,  3379,   300,  1956,   213,  1958,   319,
    1960,   375,   375,  1963,   319,   117,   285,   213,  2608,   375,
     315,   248,   375,   375,   319,   114,   114,  2617,   195,   351,
     375,  3378,   375,   375,   182,  1985,  3378,   364,  2628,   182,
     264,  2631,   359,   279,   166,   244,   371,   375,   117,   375,
     273,   375,   374,   375,   117,   364,   166,  2007,   135,  3434,
    3407,  3408,   375,   247,  2014,  3407,  3408,   370,   375,   375,
    2020,  2661,   372,   240,  2664,  2025,  2026,   111,  2028,   154,
     375,  3395,   357,   371,   212,  3236,  2036,  3462,   352,   393,
     412,   413,   414,   415,   416,   417,   418,   419,   420,   421,
     354,   351,   259,   351,   375,   108,  2056,   352,   351,   168,
     266,   131,   375,   375,   375,   238,   351,   116,   285,   265,
     151,   265,   362,   265,  3499,  2075,   293,   294,  2078,   247,
     362,   248,   362,   194,  3509,   364,   362,   359,   265,   154,
     154,   362,   362,   362,   362,   312,   362,   364,   362,  2739,
    2100,   364,  2102,   320,   182,  2745,   365,  2747,   149,  2109,
     357,   371,   248,   371,   264,  2755,   264,   264,   357,  2119,
     357,   156,   202,   107,   359,   168,   279,   181,   211,  2129,
     351,   176,   184,   115,   354,  3560,   372,  2137,   213,   117,
    2140,   371,  2142,   375,  3541,  2145,  2146,   181,   398,  2149,
     351,  2151,   357,   398,   325,  2155,   203,  2157,   375,  2159,
    2160,   423,  2162,   372,  3561,   111,   219,   220,   208,  3561,
     425,  2811,   357,   372,  2174,   248,   248,   259,  2178,   370,
     375,  2181,   370,   236,   237,   244,   370,   370,   364,   288,
     243,   244,   351,   197,   254,   185,   249,   351,   197,   297,
     375,   370,   372,   134,   370,   258,   137,  2207,  2208,   314,
     375,   264,   143,  2213,   370,   271,  3613,   244,  3643,  3585,
     375,  3613,   248,   372,   375,   370,   372,   280,  2868,   372,
     372,   372,   191,   370,  2234,  3660,  2236,   259,   370,   170,
     375,   111,   233,  2243,   233,  2885,   375,   300,   241,   111,
     134,   111,   268,   137,  2254,   154,  2256,   359,   111,   143,
     352,   192,   352,  2263,   220,  2265,   319,   281,   307,  2909,
    2270,   359,   351,  3670,   191,   371,   371,   208,  3670,   181,
     357,   181,  2922,   260,   375,   154,   154,   316,   219,   220,
     221,   362,   182,   194,   248,   362,  2296,   365,   156,  2299,
     362,  2301,   350,   107,  3701,   236,   375,  2307,   192,  3701,
     365,  2951,   243,   244,   245,   351,   168,  3742,   249,   168,
     364,   252,   359,   254,   208,   222,   147,   258,   352,  2969,
     222,   303,   370,   264,  3759,   210,   220,   221,   370,   116,
     226,    36,  3767,  3768,  3769,  3770,  2986,   364,   114,   280,
     370,   372,   236,   372,   372,   370,   351,   288,   370,   243,
     244,   245,   372,  2363,   351,   249,   185,   254,   185,   300,
     254,   319,   288,  2373,   258,   351,   357,   248,   259,  2379,
     264,   248,   248,   206,   357,   370,   268,   351,   319,  3029,
     357,   359,   357,   192,   357,   137,   280,   359,   176,   370,
     370,   375,   371,   312,   288,  2405,   351,   351,   111,  3049,
     371,   197,   197,   370,   259,   375,   300,   375,   374,   375,
     375,  3846,   375,   375,   375,   282,   375,  3824,   170,   370,
     166,   352,   375,   351,   213,   319,   351,  3862,   173,   351,
     354,  3866,   352,   375,   375,   131,   359,   351,   154,   120,
     181,   362,  2452,   348,  2454,   154,   412,   413,   414,   415,
     416,   417,   418,   419,   420,   421,   362,   181,   351,   362,
     208,  2471,  2472,   362,   365,   203,  2476,   219,  2478,   221,
     138,  3121,   362,   362,   260,  3910,   351,   364,   260,   260,
     259,   375,   370,   168,   236,   237,   359,   371,  2498,   371,
     359,   357,   244,  3928,  2504,   352,   293,   249,   176,   290,
     351,   375,  2512,   357,  3939,  2515,   258,   166,   248,   357,
     443,   193,   264,   187,   352,   357,  2526,   352,   238,  3169,
     273,     7,     8,     9,   293,   351,  3176,    13,    14,   351,
      16,   111,  3967,    19,    20,   351,   288,    23,    24,   351,
     359,    27,  2552,   375,    30,    31,    32,    33,   300,   370,
     370,   287,  2562,   111,    40,   372,   233,   375,  2568,   238,
     352,  2571,   238,   315,   259,   370,   351,   319,   319,   375,
     154,  2581,  2582,   375,   375,   375,   268,    63,    64,    65,
      66,    67,    68,    69,    70,    71,    72,    73,    74,    75,
      76,    77,    78,    79,    80,    81,   268,   117,  2608,   181,
     209,   375,   351,   359,   371,   371,  3256,  2617,   119,   154,
     120,   348,   362,   175,   375,  2625,   350,   279,  2628,   362,
     172,  2631,   351,   375,   351,   351,   351,   357,   351,   279,
     248,   364,     7,     8,     9,  2645,   375,   244,    13,    14,
     357,    16,   192,  2653,    19,    20,   150,   246,    23,    24,
      25,  2661,    27,   213,  2664,    30,    31,    32,    33,   259,
     375,   351,   351,    38,    39,    40,    41,   139,   202,   352,
     139,   375,   181,   375,   259,   213,   117,    52,    53,   352,
     371,   139,   357,   371,   370,  2695,   111,  2697,    63,    64,
      65,    66,    67,    68,    69,    70,    71,    72,    73,    74,
      75,    76,    77,    78,    79,    80,    81,   372,   274,   111,
     370,  3361,   374,   375,   375,   370,   229,   352,  2728,   117,
     209,  2731,   375,   362,   374,   375,   318,   357,   362,  2739,
     362,  2741,   357,   362,   288,  2745,   399,  2747,   357,   351,
     184,  2751,   352,   117,   268,  2755,  2756,  2757,   375,   173,
     412,   413,   414,   415,   416,   417,   418,   419,   420,   421,
    2770,  2771,   412,   413,   414,   415,   416,   417,   418,   419,
     420,   421,   290,   256,  2784,   362,   351,   375,   154,   111,
     351,   351,  2792,     7,     8,     9,   364,   374,   375,    13,
      14,   288,    16,   319,   274,    19,    20,   352,   213,    23,
      24,  2811,   167,    27,   352,   372,    30,    31,    32,    33,
     375,   207,   351,   111,   375,    39,    40,   357,   209,   352,
     117,   362,   371,   117,   370,   412,   413,   414,   415,   416,
     417,   418,   419,   420,   421,   362,  2846,   362,   370,    63,
      64,    65,    66,    67,    68,    69,    70,    71,    72,    73,
      74,    75,    76,    77,    78,    79,    80,    81,  2868,   352,
     350,   352,   350,   357,   351,   117,   174,   352,  2878,   375,
     352,   194,   351,   241,   396,  2885,   357,   351,   213,   182,
     265,   137,   319,  2893,   213,  2895,   225,   143,  2898,   319,
    2900,   111,   352,  3543,   181,  2905,   181,   185,   371,  2909,
     370,   352,   370,   370,   213,  2915,  2916,   126,   351,   357,
     117,  2921,  2922,   359,   170,   251,   352,   352,   265,   372,
     375,  2931,   303,   351,   244,   375,   375,   248,   351,   351,
     370,   351,   351,   375,   202,   254,   185,  2947,  2948,   352,
     352,  2951,   359,   429,   375,   291,   107,   352,   351,  3599,
     351,   362,   351,   357,   351,   411,   351,   375,   206,  2969,
    2970,   375,   352,   219,   220,   221,   247,   177,   362,   166,
     248,   374,   375,   352,   206,   359,  2986,   213,  2988,   351,
     236,   352,   352,   352,   351,  2995,  2996,   243,   244,   319,
     352,   357,   246,   249,   357,   238,   252,   177,   213,   351,
     260,   351,   258,   352,   352,   244,   351,   260,   264,   412,
     413,   414,   415,   416,   417,   418,   419,   420,   421,  3029,
     375,   352,   111,   246,   280,   140,   351,  3037,   374,   375,
    3680,   347,   288,    55,   837,   839,   119,   836,   345,  3049,
     838,   136,   430,  1272,   300,   406,  3056,   422,   841,  3059,
     840,   114,  2326,   428,   429,   430,   431,   432,   433,   315,
     352,   371,   714,   319,   637,  3075,   412,   413,   414,   415,
     416,   417,   418,   419,   420,   421,   351,   857,    99,   737,
    1213,  1142,   374,   375,   301,   324,   325,   326,   327,  3099,
    3100,  1344,   301,    99,   682,  2821,   172,  1137,  2208,   374,
     375,  3111,  3102,  2937,  3754,  3185,  1978,  2770,  3104,  3775,
    3760,  3121,   351,  3763,  1989,  3569,  2809,   895,   895,   375,
     412,   413,   414,   415,   416,   417,   418,   419,   420,   421,
    3140,   895,   916,  1948,  3252,   374,   375,   412,   413,   414,
     415,   416,   417,   418,   419,   420,   421,  3599,  3036,  3447,
    1413,  3241,  3162,  3698,  1461,  3575,  3593,  3730,  3462,  3169,
    3704,  2476,  3812,  2363,  1808,  2467,  3176,  2801,  3477,  3819,
    3475,  3078,  2256,   412,   413,   414,   415,   416,   417,   418,
     419,   420,   421,  3193,  3194,   159,  3196,  2066,   971,  3199,
    3200,  3201,  3340,   371,  1710,  3377,   374,   375,   422,   423,
     424,  2661,  3883,  3213,  2969,   429,  3657,  3217,   371,  3859,
    2631,   374,   375,  3660,  1027,  1049,  2078,  2257,   381,  2105,
     383,  3231,   751,  1560,  3761,  1624,  1805,  3917,  3819,  3948,
    3974,  3241,  3242,  3883,   412,   413,   414,   415,   416,   417,
     418,   419,   420,   421,  1194,  3768,  3256,  3550,  3862,   412,
     413,   414,   415,   416,   417,   418,   419,   420,   421,  3866,
    3866,  3770,  3414,  3412,  2286,  3567,  3207,   667,  2980,  3919,
    3280,  3742,  1490,  3283,  3499,  2070,   963,  1148,  1490,   936,
    3502,  3931,  2296,  3736,  1149,  3295,  2694,  2081,  2921,  3361,
    2136,  1085,  2135,  2176,  1582,  3344,   629,  1437,  3948,     4,
     374,   375,     7,     8,     9,  1449,  2460,  3957,    13,    14,
      15,    16,  2459,  2430,    19,    20,    21,  3491,    23,    24,
    2492,    26,    27,  2494,    29,    30,    31,    32,    33,  2778,
    2703,  2484,  3226,  3310,  3098,    40,  3494,  1818,   412,   413,
     414,   415,   416,   417,   418,   419,   420,   421,  3358,  3706,
      -1,  3361,    -1,    -1,    -1,    -1,    -1,    -1,    63,    64,
      65,    66,    67,    68,    69,    70,    71,    72,    73,    74,
      75,    76,    77,    78,    79,    80,    81,    -1,  1234,    -1,
      -1,    -1,    -1,    -1,    -1,  3395,    -1,  3397,    -1,    -1,
      -1,    -1,    -1,    -1,  3404,    -1,    -1,    -1,    -1,    -1,
      -1,     7,     8,     9,    -1,    -1,    -1,    13,    14,    -1,
      16,    -1,    -1,    19,    20,    -1,    -1,    23,    24,  3429,
      -1,    27,    -1,    -1,    30,    31,    32,    33,    -1,    -1,
      -1,    -1,    -1,    -1,    40,    -1,    -1,  3447,  3448,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,  3459,
      -1,    -1,    -1,    -1,    -1,  3465,  3466,    63,    64,    65,
      66,    67,    68,    69,    70,    71,    72,    73,    74,    75,
      76,    77,    78,    79,    80,    81,   374,   375,    84,    85,
      86,    -1,    -1,    -1,    -1,    -1,    92,    93,    94,    95,
      96,    97,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,  3516,    -1,    -1,    -1,
    3520,    -1,    -1,    -1,   412,   413,   414,   415,   416,   417,
     418,   419,   420,   421,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,  3542,  3543,    -1,  3545,    -1,  3547,    -1,  3549,
    3550,  3551,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,  3571,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,  3581,  3582,    -1,    -1,  3585,   105,   106,    -1,    -1,
      -1,  3591,    -1,    -1,    -1,    -1,    -1,  3597,    -1,  3599,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   133,    -1,    -1,    -1,    -1,    -1,
      -1,   140,    -1,    -1,  3624,    -1,    -1,    -1,   147,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     159,    -1,    -1,    -1,    -1,    -1,    -1,  3647,    -1,    -1,
      -1,    -1,  3652,   172,    -1,    -1,    -1,    -1,    -1,    -1,
     179,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,  3675,    -1,    -1,    -1,    -1,
    3680,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,  3693,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   222,    -1,    -1,   225,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   235,    -1,    -1,  3719,
    3720,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   257,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     445,   270,    -1,    -1,  3754,  3755,    -1,    -1,    -1,    -1,
    3760,    -1,    -1,  3763,    -1,     7,     8,     9,    -1,   288,
      -1,    13,    14,    -1,    16,    -1,  3776,    19,    20,    -1,
      -1,    23,    24,    -1,    -1,    27,    -1,    -1,    30,    31,
      32,    33,   311,    -1,    -1,    -1,    -1,    39,    40,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,  3812,    -1,    -1,    -1,    -1,    -1,    -1,  3819,
      -1,    63,    64,    65,    66,    67,    68,    69,    70,    71,
      72,    73,    74,    75,    76,    77,    78,    79,    80,    81,
    3840,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   444,    -1,
      -1,    -1,    -1,  3853,  3854,   374,   375,    -1,    -1,  3859,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,  3867,  3868,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,  3883,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,  3891,    -1,   412,   413,   414,   415,   416,   417,   418,
     419,   420,   421,    -1,    -1,    -1,    -1,    -1,    -1,  3909,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,  3919,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,  3931,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,  3948,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,  3957,     4,     5,
       6,     7,     8,     9,    10,    11,    12,    13,    14,    15,
      16,    17,    18,    19,    20,    21,    22,    23,    24,    25,
      26,    27,    28,    29,    30,    31,    32,    33,    34,    -1,
      -1,    37,    38,    39,    40,    41,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    52,    53,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    63,    64,    65,
      66,    67,    68,    69,    70,    71,    72,    73,    74,    75,
      76,    77,    78,    79,    80,    81,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   100,     4,     5,     6,     7,     8,
       9,    10,    11,    12,    13,    14,    15,    16,    17,    18,
      19,    20,    21,    22,    23,    24,    25,    26,    27,    28,
      29,    30,    31,    32,    33,    34,    -1,    -1,    37,    38,
      39,    40,    41,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    52,    53,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    63,    64,    65,    66,    67,    68,
      69,    70,    71,    72,    73,    74,    75,    76,    77,    78,
      79,    80,    81,     7,     8,     9,    -1,    -1,    -1,    13,
      14,    -1,    16,    -1,    -1,    19,    20,    -1,    -1,    23,
      24,   100,    -1,    27,    -1,    -1,    30,    31,    32,    33,
      -1,    -1,    -1,    -1,    -1,    -1,    40,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     422,    -1,   424,    -1,    -1,    -1,    -1,   429,    -1,    63,
      64,    65,    66,    67,    68,    69,    70,    71,    72,    73,
      74,    75,    76,    77,    78,    79,    80,    81,    -1,    -1,
      84,    85,    86,    -1,    -1,    -1,    -1,    -1,    92,    93,
      94,    95,    96,    97,     7,     8,     9,    -1,    -1,    -1,
      13,    14,    -1,    16,    -1,    -1,    19,    20,    -1,    -1,
      23,    24,    -1,    -1,    27,    -1,    -1,    30,    31,    32,
      33,    -1,    -1,    -1,    -1,    -1,    -1,    40,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      63,    64,    65,    66,    67,    68,    69,    70,    71,    72,
      73,    74,    75,    76,    77,    78,    79,    80,    81,    -1,
      -1,    84,    85,    86,    -1,    -1,    -1,    -1,    -1,    92,
      93,    94,    95,    96,    97,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,     7,     8,     9,    -1,    -1,    -1,
      13,    14,    -1,    16,    -1,    -1,    19,    20,    -1,    -1,
      23,    24,    -1,    -1,    27,    -1,    -1,    30,    31,    32,
      33,    -1,    -1,    -1,    -1,    -1,   422,    40,    -1,    -1,
      -1,    -1,   428,   429,   430,   431,   432,   433,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   443,   444,   445,
      63,    64,    65,    66,    67,    68,    69,    70,    71,    72,
      73,    74,    75,    76,    77,    78,    79,    80,    81,    -1,
      -1,    84,    85,    86,    -1,    -1,    -1,    -1,    -1,    92,
      93,    94,    95,    96,    97,     0,    -1,    -1,     3,     4,
      -1,    -1,     7,     8,     9,    -1,    -1,    -1,    13,    14,
      15,    16,    -1,    -1,    19,    20,    21,    -1,    23,    24,
      -1,    26,    27,    -1,    29,    30,    31,    32,    33,    -1,
      -1,    -1,    -1,   422,    39,    40,    -1,    -1,    -1,   428,
     429,   430,   431,   432,   433,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   443,   444,   445,    -1,    63,    64,
      65,    66,    67,    68,    69,    70,    71,    72,    73,    74,
      75,    76,    77,    78,    79,    80,    81,    -1,    -1,    84,
      85,    86,    -1,    -1,    -1,    -1,    -1,    92,    93,    94,
      95,    96,    97,    -1,    -1,   100,     3,     4,    -1,    -1,
       7,     8,     9,    -1,    -1,    -1,    13,    14,    15,    16,
      -1,    -1,    19,    20,    21,    -1,    23,    24,    -1,    26,
      27,    -1,    29,    30,    31,    32,    33,    -1,    -1,    -1,
     444,    -1,    39,    40,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    63,    64,    65,    66,
      67,    68,    69,    70,    71,    72,    73,    74,    75,    76,
      77,    78,    79,    80,    81,    -1,    -1,    84,    85,    86,
      -1,    -1,    -1,    -1,    -1,    92,    93,    94,    95,    96,
      97,    -1,    -1,   100,     7,     8,     9,    -1,    -1,    -1,
      13,    14,    -1,    16,    -1,    -1,    19,    20,    -1,    -1,
      23,    24,    -1,    -1,    27,    -1,    -1,    30,    31,    32,
      33,   444,    -1,    -1,    -1,    -1,    -1,    40,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      63,    64,    65,    66,    67,    68,    69,    70,    71,    72,
      73,    74,    75,    76,    77,    78,    79,    80,    81,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,     7,     8,     9,    -1,
      -1,    -1,    13,    14,    -1,    16,    -1,    -1,    19,    20,
      -1,    -1,    23,    24,    -1,    -1,    27,    -1,    -1,    30,
      31,    32,    33,    -1,    -1,    -1,    -1,    -1,    -1,    40,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   444,    63,    64,    65,    66,    67,    68,    69,    70,
      71,    72,    73,    74,    75,    76,    77,    78,    79,    80,
      81,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,     7,
       8,     9,    -1,    -1,    -1,    13,    14,    -1,    16,    -1,
      -1,    19,    20,    -1,    -1,    23,    24,    -1,    -1,    27,
      -1,    -1,    30,    31,    32,    33,    -1,    -1,    -1,    -1,
      -1,    -1,    40,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   422,    -1,    -1,
      -1,    -1,    -1,    -1,   429,    63,    64,    65,    66,    67,
      68,    69,    70,    71,    72,    73,    74,    75,    76,    77,
      78,    79,    80,    81,    -1,   104,   105,    -1,    -1,    -1,
     109,   110,    -1,    -1,   113,   114,    -1,    -1,    -1,    -1,
      -1,   120,   121,    -1,   123,   124,    -1,   126,    -1,   128,
      -1,    -1,    -1,    -1,    -1,   134,    -1,    -1,    -1,    -1,
      -1,    -1,   141,    -1,   143,   144,    -1,   146,    -1,    -1,
      -1,    -1,    -1,    -1,   153,    -1,   155,    -1,    -1,    -1,
      -1,    -1,    -1,   162,    -1,   164,   165,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   422,    -1,    -1,    -1,    -1,
      -1,    -1,   429,    -1,    -1,    -1,    -1,   186,   187,   188,
     189,    -1,   191,    -1,   193,    -1,   195,   196,   197,    -1,
     199,    -1,   201,   202,    -1,    -1,    -1,    -1,   207,    -1,
      -1,    -1,    -1,    -1,    -1,   214,    -1,    -1,   217,    -1,
      -1,    -1,   221,   222,   223,   224,    -1,    -1,    -1,    -1,
      -1,    -1,   231,    -1,    -1,    -1,    -1,   236,    -1,    -1,
      -1,   240,    -1,    -1,   243,    -1,    -1,   246,    -1,    -1,
      -1,   250,   251,   252,   253,   254,    -1,   256,    -1,    -1,
     259,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   422,
     423,   424,   271,   272,   273,   274,   429,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   284,   285,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   294,    -1,   296,    -1,    -1,
      -1,    -1,    -1,   302,   303,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   317,   318,
      -1,    -1,   321,   322,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     339,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   351,   352,    -1,    -1,   355,   356,    -1,   358,
      -1,   422,    -1,   424,    -1,    -1,    -1,   366,   429,    -1,
     369,   370,   371,   372,   373,   374,   375,    -1,    -1,    -1,
      -1,    -1,   381,    -1,   383,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   391,    -1,    -1,    -1,   395,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   412,   413,   414,   415,   416,   417,   418,
     419,   420,   421,    -1,   104,   105,    -1,    -1,    -1,   109,
     110,    -1,    -1,   113,   114,    -1,    -1,    -1,    -1,    -1,
     120,   121,    -1,   123,   124,   423,   126,    -1,   128,    -1,
      -1,    -1,    -1,    -1,   134,    -1,    -1,    -1,    -1,    -1,
      -1,   141,    -1,   143,   144,    -1,   146,    -1,    -1,    -1,
      -1,    -1,    -1,   153,    -1,   155,    -1,    -1,    -1,    -1,
      -1,    -1,   162,    -1,   164,   165,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   186,   187,   188,   189,
      -1,   191,    -1,   193,    -1,   195,   196,   197,    -1,   199,
      -1,   201,   202,    -1,    -1,    -1,    -1,   207,    -1,    -1,
      -1,    -1,    -1,    -1,   214,    -1,    -1,   217,    -1,    -1,
      -1,   221,   222,   223,   224,    -1,    -1,    -1,    -1,    -1,
      -1,   231,    -1,    -1,    -1,    -1,   236,    -1,    -1,    -1,
     240,    -1,    -1,   243,    -1,    -1,   246,    -1,    -1,    -1,
     250,   251,   252,   253,   254,    -1,   256,    -1,    -1,   259,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   271,   272,   273,   274,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   284,   285,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   294,    -1,   296,    -1,    -1,    -1,
      -1,    -1,   302,   303,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   317,   318,    -1,
      -1,   321,   322,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   339,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   351,   352,    -1,    -1,   355,   356,    -1,   358,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   366,    -1,    -1,   369,
     370,   371,   372,   373,   374,   375,    -1,    -1,    -1,    -1,
      -1,   381,    -1,   383,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   391,    -1,    -1,    -1,   395,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   412,   413,   414,   415,   416,   417,   418,   419,
     420,   421,   104,    -1,    -1,    -1,    -1,   109,   110,    -1,
      -1,   113,   114,    -1,    -1,    -1,    -1,    -1,   120,   121,
      -1,   123,   124,    -1,   126,    -1,   128,    -1,    -1,    -1,
      -1,    -1,   134,    -1,    -1,    -1,    -1,    -1,    -1,   141,
      -1,   143,   144,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   153,    -1,   155,    -1,    -1,    -1,    -1,    -1,    -1,
     162,    -1,   164,   165,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   186,   187,   188,   189,    -1,   191,
      -1,   193,    -1,   195,   196,   197,    -1,   199,    -1,   201,
     202,    -1,    -1,    -1,    -1,   207,    -1,    -1,    -1,    -1,
      -1,    -1,   214,    -1,    -1,   217,    -1,    -1,    -1,   221,
     222,   223,   224,    -1,    -1,    -1,    -1,    -1,    -1,   231,
      -1,    -1,    -1,    -1,   236,    -1,    -1,    -1,   240,    -1,
      -1,   243,    -1,    -1,   246,    -1,    -1,    -1,   250,   251,
     252,   253,   254,    -1,   256,    -1,    -1,   259,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   268,    -1,    -1,   271,
     272,   273,   274,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   284,   285,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   294,    -1,   296,    -1,    -1,    -1,    -1,    -1,
     302,   303,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   317,   318,    -1,    -1,   321,
     322,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   339,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   351,
     352,    -1,    -1,    -1,   356,    -1,   358,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   366,    -1,    -1,   369,   370,   371,
     372,   373,   374,   375,    -1,    -1,    -1,    -1,    -1,   381,
      -1,   383,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   391,
      -1,    -1,    -1,   395,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     412,   413,   414,   415,   416,   417,   418,   419,   420,   421,
     104,    -1,    -1,    -1,    -1,   109,   110,    -1,    -1,   113,
     114,    -1,    -1,    -1,    -1,    -1,   120,   121,    -1,   123,
     124,    -1,   126,    -1,   128,    -1,    -1,    -1,    -1,    -1,
     134,    -1,    -1,    -1,    -1,    -1,    -1,   141,    -1,   143,
     144,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   153,
      -1,   155,    -1,    -1,    -1,    -1,    -1,    -1,   162,    -1,
     164,   165,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   186,   187,   188,   189,    -1,   191,    -1,   193,
      -1,   195,   196,   197,    -1,   199,    -1,   201,   202,    -1,
      -1,    -1,    -1,   207,    -1,    -1,    -1,    -1,    -1,    -1,
     214,    -1,    -1,   217,    -1,    -1,    -1,   221,   222,   223,
     224,    -1,    -1,    -1,    -1,    -1,    -1,   231,    -1,    -1,
      -1,    -1,   236,    -1,    -1,    -1,   240,    -1,    -1,   243,
      -1,    -1,   246,    -1,    -1,    -1,   250,   251,   252,   253,
     254,    -1,   256,    -1,    -1,   259,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   268,    -1,    -1,   271,   272,   273,
     274,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     284,   285,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     294,    -1,   296,    -1,    -1,    -1,    -1,    -1,   302,   303,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   317,   318,    -1,    -1,   321,   322,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   339,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   351,    -1,    -1,
      -1,    -1,   356,    -1,   358,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   366,    -1,    -1,   369,   370,   371,   372,   373,
     374,   375,    -1,    -1,    -1,    -1,    -1,   381,    -1,   383,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   391,    -1,    -1,
      -1,   395,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   412,   413,
     414,   415,   416,   417,   418,   419,   420,   421,   104,    -1,
      -1,    -1,    -1,   109,   110,    -1,    -1,   113,   114,    -1,
      -1,    -1,    -1,    -1,   120,   121,    -1,   123,   124,    -1,
     126,    -1,   128,    -1,    -1,    -1,    -1,    -1,   134,    -1,
      -1,    -1,    -1,    -1,    -1,   141,    -1,   143,   144,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   153,    -1,   155,
      -1,    -1,    -1,    -1,    -1,    -1,   162,    -1,   164,   165,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     186,   187,   188,   189,    -1,   191,    -1,   193,    -1,   195,
     196,   197,    -1,   199,    -1,   201,   202,    -1,    -1,    -1,
      -1,   207,    -1,    -1,    -1,    -1,    -1,    -1,   214,    -1,
      -1,   217,    -1,    -1,    -1,   221,   222,   223,   224,    -1,
      -1,    -1,    -1,    -1,    -1,   231,    -1,    -1,    -1,    -1,
     236,    -1,    -1,    -1,   240,    -1,    -1,   243,    -1,    -1,
     246,    -1,    -1,    -1,   250,   251,   252,   253,   254,    -1,
     256,    -1,    -1,   259,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   271,   272,   273,   274,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   284,   285,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   294,    -1,
     296,    -1,    -1,    -1,    -1,    -1,   302,   303,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   317,   318,    -1,    -1,   321,   322,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   339,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   351,   352,    -1,    -1,    -1,
     356,    -1,   358,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     366,    -1,    -1,   369,   370,   371,   372,   373,   374,   375,
      -1,    -1,    -1,    -1,    -1,   381,    -1,   383,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   391,    -1,    -1,    -1,   395,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   412,   413,   414,   415,
     416,   417,   418,   419,   420,   421,   104,    -1,    -1,    -1,
      -1,   109,   110,    -1,    -1,   113,   114,    -1,    -1,    -1,
      -1,    -1,   120,   121,    -1,   123,   124,    -1,   126,    -1,
     128,    -1,    -1,    -1,    -1,    -1,   134,    -1,    -1,    -1,
      -1,    -1,    -1,   141,    -1,   143,   144,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   153,    -1,   155,    -1,    -1,
      -1,    -1,    -1,    -1,   162,    -1,   164,   165,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   186,   187,
     188,   189,    -1,   191,    -1,   193,    -1,   195,   196,   197,
      -1,   199,    -1,   201,   202,    -1,    -1,    -1,    -1,   207,
      -1,    -1,    -1,    -1,    -1,    -1,   214,    -1,    -1,   217,
      -1,    -1,    -1,   221,   222,   223,   224,    -1,    -1,    -1,
      -1,    -1,    -1,   231,    -1,    -1,    -1,    -1,   236,    -1,
      -1,    -1,   240,    -1,    -1,   243,    -1,    -1,   246,    -1,
      -1,    -1,   250,   251,   252,   253,   254,    -1,   256,    -1,
      -1,   259,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   271,   272,   273,   274,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   284,   285,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   294,    -1,   296,    -1,
      -1,    -1,    -1,    -1,   302,   303,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   317,
     318,    -1,    -1,   321,   322,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   339,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   351,    -1,    -1,    -1,    -1,   356,    -1,
     358,    -1,    -1,    -1,   362,    -1,    -1,    -1,   366,    -1,
      -1,   369,   370,   371,   372,   373,   374,   375,    -1,    -1,
      -1,    -1,    -1,   381,    -1,   383,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   391,    -1,    -1,    -1,   395,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   412,   413,   414,   415,   416,   417,
     418,   419,   420,   421,   104,    -1,    -1,    -1,    -1,   109,
     110,    -1,    -1,   113,   114,    -1,    -1,    -1,    -1,    -1,
     120,   121,    -1,   123,   124,    -1,   126,    -1,   128,    -1,
      -1,    -1,    -1,    -1,   134,    -1,    -1,    -1,    -1,    -1,
      -1,   141,    -1,   143,   144,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   153,    -1,   155,    -1,    -1,    -1,    -1,
      -1,    -1,   162,    -1,   164,   165,   166,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   186,   187,   188,   189,
      -1,   191,    -1,   193,    -1,   195,   196,   197,    -1,   199,
      -1,   201,   202,    -1,    -1,    -1,    -1,   207,    -1,    -1,
      -1,    -1,    -1,    -1,   214,    -1,    -1,   217,    -1,    -1,
      -1,   221,   222,   223,   224,    -1,    -1,    -1,    -1,    -1,
      -1,   231,    -1,    -1,    -1,    -1,   236,    -1,    -1,    -1,
     240,    -1,    -1,   243,    -1,    -1,   246,    -1,    -1,    -1,
     250,   251,   252,   253,   254,    -1,   256,    -1,    -1,   259,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   271,   272,   273,   274,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   284,   285,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   294,    -1,   296,    -1,    -1,    -1,
      -1,    -1,   302,   303,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   317,   318,    -1,
      -1,   321,   322,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   339,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   351,    -1,    -1,    -1,    -1,   356,    -1,   358,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   366,    -1,    -1,   369,
     370,   371,   372,   373,   374,   375,    -1,    -1,    -1,    -1,
      -1,   381,    -1,   383,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   391,    -1,    -1,    -1,   395,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   412,   413,   414,   415,   416,   417,   418,   419,
     420,   421,   104,    -1,    -1,    -1,    -1,   109,   110,    -1,
      -1,   113,   114,    -1,    -1,    -1,    -1,    -1,   120,   121,
      -1,   123,   124,    -1,   126,    -1,   128,    -1,    -1,    -1,
      -1,    -1,   134,    -1,    -1,    -1,    -1,    -1,    -1,   141,
      -1,   143,   144,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   153,    -1,   155,    -1,    -1,    -1,    -1,    -1,    -1,
     162,    -1,   164,   165,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   186,   187,   188,   189,    -1,   191,
      -1,   193,    -1,   195,   196,   197,    -1,   199,    -1,   201,
     202,    -1,    -1,    -1,    -1,   207,    -1,    -1,    -1,    -1,
      -1,    -1,   214,    -1,    -1,   217,    -1,    -1,    -1,   221,
     222,   223,   224,    -1,    -1,    -1,    -1,    -1,    -1,   231,
      -1,    -1,    -1,    -1,   236,    -1,    -1,    -1,   240,    -1,
      -1,   243,    -1,    -1,   246,    -1,    -1,    -1,   250,   251,
     252,   253,   254,    -1,   256,    -1,    -1,   259,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   271,
     272,   273,   274,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   284,   285,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   294,    -1,   296,    -1,    -1,    -1,    -1,    -1,
     302,   303,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   317,   318,    -1,    -1,   321,
     322,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   339,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   351,
      -1,    -1,    -1,   355,   356,    -1,   358,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   366,    -1,    -1,   369,   370,   371,
     372,   373,   374,   375,    -1,    -1,    -1,    -1,    -1,   381,
      -1,   383,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   391,
      -1,    -1,    -1,   395,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     412,   413,   414,   415,   416,   417,   418,   419,   420,   421,
     104,    -1,    -1,    -1,    -1,   109,   110,    -1,    -1,   113,
     114,    -1,    -1,    -1,    -1,    -1,   120,   121,    -1,   123,
     124,    -1,   126,    -1,   128,    -1,    -1,    -1,    -1,    -1,
     134,    -1,    -1,    -1,    -1,    -1,    -1,   141,    -1,   143,
     144,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   153,
      -1,   155,    -1,    -1,    -1,    -1,    -1,    -1,   162,    -1,
     164,   165,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   186,   187,   188,   189,    -1,   191,    -1,   193,
      -1,   195,   196,   197,    -1,   199,    -1,   201,   202,    -1,
      -1,    -1,    -1,   207,    -1,    -1,    -1,    -1,    -1,    -1,
     214,    -1,    -1,   217,    -1,    -1,    -1,   221,   222,   223,
     224,    -1,    -1,    -1,    -1,    -1,    -1,   231,    -1,    -1,
      -1,    -1,   236,    -1,    -1,    -1,   240,    -1,    -1,   243,
      -1,    -1,   246,    -1,    -1,    -1,   250,   251,   252,   253,
     254,    -1,   256,    -1,    -1,   259,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   271,   272,   273,
     274,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     284,   285,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     294,    -1,   296,    -1,    -1,    -1,    -1,    -1,   302,   303,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   317,   318,    -1,    -1,   321,   322,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   339,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   351,   352,    -1,
      -1,    -1,   356,    -1,   358,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   366,    -1,    -1,   369,   370,   371,   372,   373,
     374,   375,    -1,    -1,    -1,    -1,    -1,   381,    -1,   383,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   391,    -1,    -1,
      -1,   395,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   412,   413,
     414,   415,   416,   417,   418,   419,   420,   421,   104,    -1,
      -1,    -1,    -1,   109,   110,    -1,    -1,   113,   114,    -1,
      -1,    -1,    -1,    -1,   120,   121,    -1,   123,   124,    -1,
     126,    -1,   128,    -1,    -1,    -1,    -1,    -1,   134,    -1,
      -1,    -1,    -1,    -1,    -1,   141,    -1,   143,   144,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   153,    -1,   155,
      -1,    -1,    -1,    -1,    -1,    -1,   162,    -1,   164,   165,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     186,   187,   188,   189,    -1,   191,    -1,   193,    -1,   195,
     196,   197,    -1,   199,    -1,   201,   202,    -1,    -1,    -1,
      -1,   207,    -1,    -1,    -1,    -1,    -1,    -1,   214,    -1,
      -1,   217,    -1,    -1,    -1,   221,   222,   223,   224,    -1,
      -1,    -1,    -1,    -1,    -1,   231,    -1,    -1,    -1,    -1,
     236,    -1,    -1,    -1,   240,    -1,    -1,   243,    -1,    -1,
     246,    -1,    -1,    -1,   250,   251,   252,   253,   254,    -1,
     256,    -1,    -1,   259,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   271,   272,   273,   274,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   284,   285,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   294,    -1,
     296,    -1,    -1,    -1,    -1,    -1,   302,   303,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   317,   318,    -1,    -1,   321,   322,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   339,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   351,   352,    -1,    -1,    -1,
     356,    -1,   358,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     366,    -1,    -1,   369,   370,   371,   372,   373,   374,   375,
      -1,    -1,    -1,    -1,    -1,   381,    -1,   383,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   391,    -1,    -1,    -1,   395,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   412,   413,   414,   415,
     416,   417,   418,   419,   420,   421,   104,    -1,    -1,    -1,
      -1,   109,   110,    -1,    -1,   113,   114,    -1,    -1,    -1,
      -1,    -1,   120,   121,    -1,   123,   124,    -1,   126,    -1,
     128,    -1,    -1,    -1,    -1,    -1,   134,    -1,    -1,    -1,
      -1,    -1,    -1,   141,    -1,   143,   144,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   153,    -1,   155,    -1,    -1,
      -1,    -1,    -1,    -1,   162,    -1,   164,   165,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   186,   187,
     188,   189,    -1,   191,    -1,   193,    -1,   195,   196,   197,
      -1,   199,    -1,   201,   202,    -1,    -1,    -1,    -1,   207,
      -1,    -1,    -1,    -1,    -1,    -1,   214,    -1,    -1,   217,
      -1,    -1,    -1,   221,   222,   223,   224,    -1,    -1,    -1,
      -1,    -1,    -1,   231,    -1,    -1,    -1,    -1,   236,    -1,
      -1,    -1,   240,    -1,    -1,   243,    -1,    -1,   246,    -1,
      -1,    -1,   250,   251,   252,   253,   254,    -1,   256,    -1,
      -1,   259,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   271,   272,   273,   274,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   284,   285,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   294,    -1,   296,    -1,
      -1,    -1,    -1,    -1,   302,   303,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   317,
     318,    -1,    -1,   321,   322,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   339,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   351,   352,    -1,    -1,    -1,   356,    -1,
     358,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   366,    -1,
      -1,   369,   370,   371,   372,   373,   374,   375,    -1,    -1,
      -1,    -1,    -1,   381,    -1,   383,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   391,    -1,    -1,    -1,   395,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   412,   413,   414,   415,   416,   417,
     418,   419,   420,   421,   104,    -1,    -1,    -1,    -1,   109,
     110,    -1,    -1,   113,   114,    -1,    -1,    -1,    -1,    -1,
     120,   121,    -1,   123,   124,    -1,   126,    -1,   128,    -1,
      -1,    -1,    -1,    -1,   134,    -1,    -1,    -1,    -1,    -1,
      -1,   141,    -1,   143,   144,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   153,    -1,   155,    -1,    -1,    -1,    -1,
      -1,    -1,   162,    -1,   164,   165,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   186,   187,   188,   189,
      -1,   191,    -1,   193,    -1,   195,   196,   197,    -1,   199,
      -1,   201,   202,    -1,    -1,    -1,    -1,   207,    -1,    -1,
      -1,    -1,    -1,    -1,   214,    -1,    -1,   217,    -1,    -1,
      -1,   221,   222,   223,   224,    -1,    -1,    -1,    -1,    -1,
      -1,   231,    -1,    -1,    -1,    -1,   236,    -1,    -1,    -1,
     240,    -1,    -1,   243,    -1,    -1,   246,    -1,    -1,    -1,
     250,   251,   252,   253,   254,    -1,   256,    -1,    -1,   259,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   271,   272,   273,   274,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   284,   285,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   294,    -1,   296,    -1,    -1,    -1,
      -1,    -1,   302,   303,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   317,   318,    -1,
      -1,   321,   322,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   339,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   351,    -1,    -1,    -1,    -1,   356,    -1,   358,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   366,    -1,    -1,   369,
     370,   371,   372,   373,   374,   375,    -1,    -1,    -1,    -1,
      -1,   381,    -1,   383,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   391,    -1,    -1,    -1,   395,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   412,   413,   414,   415,   416,   417,   418,   419,
     420,   421,   104,    -1,    -1,    -1,    -1,   109,   110,    -1,
      -1,   113,   114,    -1,    -1,    -1,    -1,    -1,   120,   121,
      -1,   123,   124,    -1,   126,    -1,   128,    -1,    -1,    -1,
      -1,    -1,   134,    -1,    -1,    -1,    -1,    -1,    -1,   141,
      -1,   143,   144,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   153,    -1,   155,    -1,    -1,    -1,    -1,    -1,    -1,
     162,    -1,   164,   165,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   186,   187,   188,   189,    -1,   191,
      -1,   193,    -1,   195,   196,   197,    -1,   199,    -1,    -1,
     202,    -1,    -1,    -1,    -1,   207,    -1,    -1,    -1,    -1,
      -1,    -1,   214,    -1,    -1,   217,    -1,    -1,    -1,   221,
     222,   223,   224,    -1,    -1,    -1,    -1,    -1,    -1,   231,
      -1,    -1,    -1,    -1,   236,    -1,    -1,    -1,   240,    -1,
      -1,   243,    -1,    -1,   246,    -1,    -1,    -1,   250,   251,
     252,   253,    -1,    -1,   256,    -1,    -1,   259,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   271,
     272,   273,   274,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   285,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   294,    -1,   296,    -1,    -1,    -1,    -1,    -1,
     302,   303,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   317,   318,    -1,    -1,   321,
     322,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   339,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   351,
      -1,    -1,    -1,    -1,   356,    -1,   358,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   366,    -1,    -1,   369,   370,   371,
     372,   373,   374,   375,    -1,    -1,    -1,    -1,    -1,   381,
      -1,   383,   384,    -1,    -1,    -1,    -1,    -1,    -1,   391,
      -1,    -1,    -1,   395,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   403,    -1,    -1,    -1,    -1,    -1,    -1,   410,    -1,
     412,   413,   414,   415,   416,   417,   418,   419,   420,   421,
     104,    -1,    -1,    -1,    -1,   109,   110,    -1,    -1,   113,
     114,    -1,    -1,    -1,    -1,    -1,   120,   121,    -1,   123,
     124,    -1,   126,    -1,   128,    -1,    -1,    -1,    -1,    -1,
     134,    -1,    -1,    -1,    -1,    -1,    -1,   141,    -1,   143,
     144,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   153,
      -1,   155,    -1,    -1,    -1,    -1,    -1,    -1,   162,    -1,
     164,   165,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   186,   187,   188,   189,    -1,   191,    -1,   193,
      -1,   195,   196,   197,    -1,   199,    -1,    -1,   202,    -1,
      -1,    -1,    -1,   207,    -1,    -1,    -1,    -1,    -1,    -1,
     214,    -1,    -1,   217,    -1,    -1,    -1,   221,   222,   223,
     224,    -1,    -1,    -1,    -1,    -1,    -1,   231,    -1,    -1,
      -1,    -1,   236,    -1,    -1,    -1,   240,    -1,    -1,   243,
      -1,    -1,   246,    -1,    -1,    -1,   250,   251,   252,   253,
     254,    -1,   256,    -1,    -1,   259,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   271,   272,   273,
     274,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     284,   285,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     294,    -1,   296,    -1,    -1,    -1,    -1,    -1,   302,   303,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   317,   318,    -1,    -1,   321,   322,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   339,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   351,    -1,    -1,
      -1,    -1,   356,    -1,   358,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   366,    -1,    -1,   369,   370,   371,   372,   373,
     374,   375,    -1,    -1,    -1,    -1,    -1,   381,    -1,   383,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   391,    -1,    -1,
      -1,   395,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   412,   413,
     414,   415,   416,   417,   418,   419,   420,   421,   104,    -1,
      -1,    -1,    -1,   109,   110,    -1,    -1,   113,   114,    -1,
      -1,    -1,    -1,    -1,   120,   121,    -1,   123,   124,    -1,
     126,    -1,   128,    -1,    -1,    -1,    -1,    -1,   134,    -1,
      -1,    -1,    -1,    -1,    -1,   141,    -1,   143,   144,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   153,    -1,   155,
      -1,    -1,    -1,    -1,    -1,    -1,   162,    -1,   164,   165,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     186,   187,   188,   189,    -1,   191,    -1,   193,    -1,   195,
     196,   197,    -1,   199,    -1,    -1,   202,    -1,    -1,    -1,
      -1,   207,    -1,    -1,    -1,    -1,    -1,    -1,   214,    -1,
      -1,   217,    -1,    -1,    -1,   221,   222,   223,   224,    -1,
      -1,    -1,    -1,    -1,    -1,   231,    -1,    -1,    -1,    -1,
     236,    -1,    -1,    -1,   240,    -1,    -1,   243,    -1,    -1,
     246,    -1,    -1,    -1,   250,   251,   252,   253,    -1,    -1,
     256,    -1,    -1,   259,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   271,   272,   273,   274,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   285,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   294,    -1,
     296,    -1,    -1,    -1,    -1,    -1,   302,   303,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   317,   318,    -1,    -1,   321,   322,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   339,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   351,    -1,    -1,    -1,    -1,
     356,    -1,   358,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     366,    -1,    -1,   369,   370,   371,   372,   373,   374,   375,
      -1,    -1,    -1,    -1,    -1,   381,    -1,   383,   384,    -1,
      -1,    -1,    -1,    -1,    -1,   391,    -1,    -1,    -1,   395,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   403,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   412,   413,   414,   415,
     416,   417,   418,   419,   420,   421,   104,    -1,    -1,    -1,
      -1,   109,   110,    -1,    -1,   113,   114,    -1,    -1,    -1,
      -1,    -1,   120,   121,    -1,   123,   124,    -1,   126,    -1,
     128,    -1,    -1,    -1,    -1,    -1,   134,    -1,    -1,    -1,
      -1,    -1,    -1,   141,    -1,   143,   144,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   153,    -1,   155,    -1,    -1,
      -1,    -1,    -1,    -1,   162,    -1,   164,   165,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   186,   187,
     188,   189,    -1,   191,    -1,   193,    -1,   195,   196,   197,
      -1,   199,    -1,    -1,   202,    -1,    -1,    -1,    -1,   207,
      -1,    -1,    -1,    -1,    -1,    -1,   214,    -1,    -1,   217,
      -1,    -1,    -1,   221,   222,   223,   224,    -1,    -1,    -1,
      -1,    -1,    -1,   231,    -1,    -1,    -1,    -1,   236,    -1,
      -1,    -1,   240,    -1,    -1,   243,    -1,    -1,   246,    -1,
      -1,    -1,   250,   251,   252,   253,    -1,    -1,   256,    -1,
      -1,   259,    -1,    -1,    -1,    -1,    -1,   265,    -1,    -1,
      -1,    -1,    -1,   271,   272,   273,   274,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   285,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   294,    -1,   296,    -1,
      -1,    -1,    -1,    -1,   302,   303,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   317,
     318,    -1,    -1,   321,   322,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   339,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   351,    -1,    -1,    -1,    -1,   356,    -1,
     358,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   366,    -1,
      -1,   369,   370,   371,   372,   373,   374,   375,    -1,    -1,
      -1,    -1,    -1,   381,    -1,   383,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   391,    -1,    -1,    -1,   395,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   412,   413,   414,   415,   416,   417,
     418,   419,   420,   421,   104,    -1,    -1,    -1,    -1,   109,
     110,    -1,    -1,   113,   114,    -1,    -1,    -1,    -1,    -1,
     120,   121,    -1,   123,   124,    -1,   126,    -1,   128,    -1,
      -1,    -1,    -1,    -1,   134,    -1,    -1,    -1,    -1,    -1,
      -1,   141,    -1,   143,   144,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   153,    -1,   155,    -1,    -1,    -1,    -1,
      -1,    -1,   162,    -1,   164,   165,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   186,   187,   188,   189,
      -1,   191,    -1,   193,    -1,   195,   196,   197,    -1,   199,
      -1,    -1,   202,    -1,    -1,    -1,    -1,   207,    -1,    -1,
      -1,    -1,    -1,    -1,   214,    -1,    -1,   217,    -1,    -1,
      -1,   221,   222,   223,   224,    -1,    -1,    -1,    -1,    -1,
      -1,   231,    -1,    -1,    -1,    -1,   236,    -1,    -1,    -1,
     240,    -1,    -1,   243,    -1,    -1,   246,    -1,    -1,    -1,
     250,   251,   252,   253,    -1,    -1,   256,    -1,    -1,   259,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   271,   272,   273,   274,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   285,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   294,    -1,   296,    -1,    -1,    -1,
      -1,    -1,   302,   303,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   317,   318,    -1,
      -1,   321,   322,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   339,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   351,    -1,    -1,    -1,   355,   356,    -1,   358,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   366,    -1,    -1,   369,
     370,   371,   372,   373,   374,   375,    -1,    -1,    -1,    -1,
      -1,   381,    -1,   383,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   391,    -1,    -1,    -1,   395,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   412,   413,   414,   415,   416,   417,   418,   419,
     420,   421,   104,    -1,    -1,    -1,    -1,   109,   110,    -1,
      -1,   113,   114,    -1,    -1,    -1,    -1,    -1,   120,   121,
      -1,   123,   124,    -1,   126,    -1,   128,    -1,    -1,    -1,
      -1,    -1,   134,    -1,    -1,    -1,    -1,    -1,    -1,   141,
      -1,   143,   144,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   153,    -1,   155,    -1,    -1,    -1,    -1,    -1,    -1,
     162,    -1,   164,   165,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   186,   187,   188,   189,    -1,   191,
      -1,   193,    -1,   195,   196,   197,    -1,   199,    -1,    -1,
     202,    -1,    -1,    -1,    -1,   207,    -1,    -1,    -1,    -1,
      -1,    -1,   214,    -1,    -1,   217,    -1,    -1,    -1,   221,
     222,   223,   224,    -1,    -1,    -1,    -1,    -1,    -1,   231,
      -1,    -1,    -1,    -1,   236,    -1,    -1,    -1,   240,    -1,
      -1,   243,    -1,    -1,   246,    -1,    -1,    -1,   250,   251,
     252,   253,    -1,    -1,   256,    -1,    -1,   259,    -1,    -1,
      -1,    -1,    -1,   265,    -1,    -1,    -1,    -1,    -1,   271,
     272,   273,   274,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   285,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   294,    -1,   296,    -1,    -1,    -1,    -1,    -1,
     302,   303,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   317,   318,    -1,    -1,   321,
     322,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   339,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   351,
      -1,    -1,    -1,    -1,   356,    -1,   358,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   366,    -1,    -1,   369,   370,   371,
     372,   373,   374,   375,    -1,    -1,    -1,    -1,    -1,   381,
      -1,   383,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   391,
      -1,    -1,    -1,   395,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     412,   413,   414,   415,   416,   417,   418,   419,   420,   421,
     104,    -1,    -1,    -1,    -1,   109,   110,    -1,    -1,   113,
     114,    -1,    -1,    -1,    -1,    -1,   120,   121,    -1,   123,
     124,    -1,   126,    -1,   128,    -1,    -1,    -1,    -1,    -1,
     134,    -1,    -1,    -1,    -1,   139,    -1,   141,    -1,   143,
     144,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   153,
      -1,   155,    -1,    -1,    -1,    -1,    -1,    -1,   162,    -1,
     164,   165,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   186,   187,   188,   189,    -1,   191,    -1,   193,
      -1,   195,   196,   197,    -1,   199,    -1,    -1,   202,    -1,
      -1,    -1,    -1,   207,    -1,    -1,    -1,    -1,    -1,    -1,
     214,    -1,    -1,   217,    -1,    -1,    -1,   221,   222,   223,
     224,    -1,    -1,    -1,    -1,    -1,    -1,   231,    -1,    -1,
      -1,    -1,   236,    -1,    -1,    -1,   240,    -1,    -1,   243,
      -1,    -1,   246,    -1,    -1,    -1,   250,   251,   252,   253,
      -1,    -1,   256,    -1,    -1,   259,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   271,   272,   273,
     274,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   285,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     294,    -1,   296,    -1,    -1,    -1,    -1,    -1,   302,   303,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   317,   318,    -1,    -1,   321,   322,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   339,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   351,    -1,    -1,
      -1,    -1,   356,    -1,   358,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   366,    -1,    -1,   369,   370,   371,   372,   373,
     374,   375,    -1,    -1,    -1,    -1,    -1,   381,    -1,   383,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   391,    -1,    -1,
      -1,   395,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   412,   413,
     414,   415,   416,   417,   418,   419,   420,   421,   104,    -1,
      -1,    -1,    -1,   109,   110,    -1,    -1,   113,   114,    -1,
      -1,    -1,    -1,    -1,   120,   121,    -1,   123,   124,    -1,
     126,    -1,   128,    -1,    -1,    -1,    -1,    -1,   134,    -1,
      -1,    -1,    -1,   139,    -1,   141,    -1,   143,   144,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   153,    -1,   155,
      -1,    -1,    -1,    -1,    -1,    -1,   162,    -1,   164,   165,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     186,   187,   188,   189,    -1,   191,    -1,   193,    -1,   195,
     196,   197,    -1,   199,    -1,    -1,   202,    -1,    -1,    -1,
      -1,   207,    -1,    -1,    -1,    -1,    -1,    -1,   214,    -1,
      -1,   217,    -1,    -1,    -1,   221,   222,   223,   224,    -1,
      -1,    -1,    -1,    -1,    -1,   231,    -1,    -1,    -1,    -1,
     236,    -1,    -1,    -1,   240,    -1,    -1,   243,    -1,    -1,
     246,    -1,    -1,    -1,   250,   251,   252,   253,    -1,    -1,
     256,    -1,    -1,   259,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   271,   272,   273,   274,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   285,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   294,    -1,
     296,    -1,    -1,    -1,    -1,    -1,   302,   303,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   317,   318,    -1,    -1,   321,   322,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   339,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   351,    -1,    -1,    -1,    -1,
     356,    -1,   358,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     366,    -1,    -1,   369,   370,   371,   372,   373,   374,   375,
      -1,    -1,    -1,    -1,    -1,   381,    -1,   383,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   391,    -1,    -1,    -1,   395,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   412,   413,   414,   415,
     416,   417,   418,   419,   420,   421,   104,    -1,    -1,    -1,
      -1,   109,   110,    -1,    -1,   113,   114,    -1,    -1,    -1,
      -1,    -1,   120,   121,    -1,   123,   124,    -1,   126,    -1,
     128,    -1,    -1,    -1,    -1,    -1,   134,    -1,    -1,    -1,
      -1,   139,    -1,   141,    -1,   143,   144,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   153,    -1,   155,    -1,    -1,
      -1,    -1,    -1,    -1,   162,    -1,   164,   165,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   186,   187,
     188,   189,    -1,   191,    -1,   193,    -1,   195,   196,   197,
      -1,   199,    -1,    -1,   202,    -1,    -1,    -1,    -1,   207,
      -1,    -1,    -1,    -1,    -1,    -1,   214,    -1,    -1,   217,
      -1,    -1,    -1,   221,   222,   223,   224,    -1,    -1,    -1,
      -1,    -1,    -1,   231,    -1,    -1,    -1,    -1,   236,    -1,
      -1,    -1,   240,    -1,    -1,   243,    -1,    -1,   246,    -1,
      -1,    -1,   250,   251,   252,   253,    -1,    -1,   256,    -1,
      -1,   259,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   271,   272,   273,   274,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   285,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   294,    -1,   296,    -1,
      -1,    -1,    -1,    -1,   302,   303,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   317,
     318,    -1,    -1,   321,   322,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   339,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   351,    -1,    -1,    -1,    -1,   356,    -1,
     358,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   366,    -1,
      -1,   369,   370,   371,   372,   373,   374,   375,    -1,    -1,
      -1,    -1,    -1,   381,    -1,   383,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   391,    -1,    -1,    -1,   395,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   412,   413,   414,   415,   416,   417,
     418,   419,   420,   421,   104,    -1,    -1,    -1,    -1,   109,
     110,    -1,    -1,   113,   114,    -1,    -1,    -1,    -1,    -1,
     120,   121,    -1,   123,   124,    -1,   126,    -1,   128,    -1,
      -1,    -1,    -1,    -1,   134,    -1,    -1,    -1,    -1,    -1,
      -1,   141,    -1,   143,   144,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   153,    -1,   155,    -1,    -1,    -1,    -1,
      -1,    -1,   162,    -1,   164,   165,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   186,   187,   188,   189,
      -1,   191,    -1,   193,    -1,   195,   196,   197,    -1,   199,
      -1,    -1,   202,    -1,    -1,    -1,    -1,   207,    -1,    -1,
      -1,    -1,    -1,    -1,   214,    -1,    -1,   217,    -1,    -1,
      -1,   221,   222,   223,   224,    -1,    -1,    -1,    -1,    -1,
      -1,   231,    -1,    -1,    -1,    -1,   236,    -1,    -1,    -1,
     240,    -1,    -1,   243,    -1,    -1,   246,    -1,    -1,    -1,
     250,   251,   252,   253,    -1,    -1,   256,    -1,    -1,   259,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   271,   272,   273,   274,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   285,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   294,    -1,   296,    -1,    -1,    -1,
      -1,    -1,   302,   303,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   317,   318,    -1,
      -1,   321,   322,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   339,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   351,    -1,    -1,    -1,   355,   356,    -1,   358,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   366,    -1,    -1,   369,
     370,   371,   372,   373,   374,   375,    -1,    -1,    -1,    -1,
      -1,   381,    -1,   383,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   391,    -1,    -1,    -1,   395,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   412,   413,   414,   415,   416,   417,   418,   419,
     420,   421,   104,    -1,    -1,    -1,    -1,   109,   110,    -1,
      -1,   113,   114,    -1,    -1,    -1,    -1,    -1,   120,   121,
      -1,   123,   124,    -1,   126,    -1,   128,    -1,    -1,    -1,
      -1,    -1,   134,    -1,    -1,    -1,    -1,    -1,    -1,   141,
      -1,   143,   144,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   153,    -1,   155,    -1,    -1,    -1,    -1,    -1,    -1,
     162,    -1,   164,   165,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   186,   187,   188,   189,    -1,   191,
      -1,   193,    -1,   195,   196,   197,    -1,   199,    -1,    -1,
     202,    -1,    -1,    -1,    -1,   207,    -1,    -1,    -1,    -1,
      -1,    -1,   214,    -1,    -1,   217,    -1,    -1,    -1,   221,
     222,   223,   224,    -1,    -1,    -1,    -1,    -1,    -1,   231,
      -1,    -1,    -1,    -1,   236,    -1,    -1,    -1,   240,    -1,
      -1,   243,    -1,    -1,   246,    -1,    -1,    -1,   250,   251,
     252,   253,    -1,    -1,   256,    -1,    -1,   259,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   271,
     272,   273,   274,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   285,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   294,    -1,   296,    -1,    -1,    -1,    -1,    -1,
     302,   303,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   317,   318,    -1,    -1,   321,
     322,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   339,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   351,
      -1,    -1,    -1,    -1,   356,    -1,   358,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   366,    -1,    -1,   369,   370,   371,
     372,   373,   374,   375,    -1,    -1,    -1,    -1,    -1,   381,
      -1,   383,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   391,
      -1,    -1,    -1,   395,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     412,   413,   414,   415,   416,   417,   418,   419,   420,   421,
     104,    -1,    -1,    -1,    -1,   109,   110,    -1,    -1,   113,
     114,    -1,    -1,    -1,    -1,    -1,   120,   121,    -1,   123,
     124,    -1,   126,    -1,   128,    -1,    -1,    -1,    -1,    -1,
     134,    -1,    -1,    -1,    -1,    -1,    -1,   141,    -1,   143,
     144,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   153,
      -1,   155,    -1,    -1,    -1,    -1,    -1,    -1,   162,    -1,
     164,   165,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   186,   187,   188,   189,    -1,   191,    -1,   193,
      -1,   195,   196,   197,    -1,   199,    -1,    -1,   202,    -1,
      -1,    -1,    -1,   207,    -1,    -1,    -1,    -1,    -1,    -1,
     214,    -1,    -1,    -1,    -1,    -1,    -1,   221,   222,   223,
     224,    -1,    -1,    -1,    -1,    -1,    -1,   231,    -1,    -1,
      -1,    -1,   236,    -1,    -1,    -1,   240,    -1,    -1,   243,
      -1,    -1,   246,    -1,    -1,    -1,   250,   251,   252,   253,
      -1,    -1,   256,    -1,    -1,   259,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   271,   272,   273,
     274,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   285,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     294,    -1,   296,    -1,    -1,    -1,    -1,    -1,   302,   303,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   317,   318,    -1,    -1,   321,   322,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   339,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   351,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   366,    -1,    -1,   369,   370,   371,   372,   373,
     374,   375,    -1,    -1,    -1,    -1,    -1,   381,    -1,   383,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   391,    -1,    -1,
      -1,   395,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   412,   413,
     414,   415,   416,   417,   418,   419,   420,   421,   104,    -1,
      -1,    -1,    -1,   109,   110,    -1,    -1,   113,   114,    -1,
      -1,    -1,    -1,    -1,    -1,   121,    -1,   123,   124,    -1,
     126,    -1,   128,    -1,    -1,    -1,    -1,    -1,   134,    -1,
      -1,    -1,    -1,    -1,    -1,   141,    -1,   143,   144,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   153,    -1,   155,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   164,   165,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     186,   187,    -1,   189,    -1,   191,    -1,   193,    -1,   195,
     196,   197,    -1,   199,    -1,    -1,   202,    -1,    -1,    -1,
      -1,   207,    -1,    -1,    -1,    -1,    -1,    -1,   214,    -1,
      -1,    -1,    -1,    -1,    -1,   221,   222,   223,   224,    -1,
      -1,    -1,    -1,    -1,    -1,   231,    -1,    -1,    -1,    -1,
     236,    -1,    -1,    -1,   240,    -1,    -1,   243,    -1,    -1,
     246,    -1,    -1,    -1,    -1,   251,   252,    -1,    -1,    -1,
     256,    -1,    -1,   259,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   271,   272,   273,   274,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   285,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     296,    -1,    -1,    -1,    -1,    -1,    -1,   303,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   317,   318,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   342,   343,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   351,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   369,   370,   371,   372,   373,   374,   375,
      -1,    -1,    -1,    -1,    -1,   381,    -1,   383,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   391,    -1,    -1,    -1,   395,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   412,   413,   414,   415,
     416,   417,   418,   419,   420,   421,   104,    -1,    -1,    -1,
      -1,   109,   110,    -1,    -1,   113,   114,    -1,    -1,    -1,
      -1,    -1,    -1,   121,    -1,   123,   124,    -1,   126,    -1,
     128,    -1,    -1,    -1,    -1,    -1,   134,    -1,    -1,    -1,
      -1,    -1,   140,   141,    -1,   143,   144,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   153,    -1,   155,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   164,   165,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   186,   187,
      -1,   189,    -1,   191,    -1,   193,    -1,   195,   196,   197,
      -1,   199,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   207,
      -1,    -1,    -1,    -1,    -1,    -1,   214,    -1,    -1,   217,
      -1,    -1,    -1,   221,   222,   223,   224,    -1,    -1,    -1,
      -1,    -1,    -1,   231,    -1,    -1,    -1,    -1,   236,    -1,
      -1,    -1,   240,    -1,    -1,   243,    -1,    -1,   246,    -1,
      -1,    -1,    -1,   251,   252,    -1,    -1,    -1,   256,    -1,
      -1,   259,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   271,   272,   273,   274,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   284,   285,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   296,    -1,
      -1,    -1,    -1,    -1,    -1,   303,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   317,
     318,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   104,    -1,    -1,    -1,    -1,   109,   110,
      -1,    -1,   113,   114,    -1,    -1,    -1,   355,    -1,    -1,
     121,    -1,   123,   124,    -1,   126,    -1,   128,    -1,    -1,
      -1,    -1,    -1,   134,    -1,    -1,   374,   375,    -1,   140,
     141,    -1,   143,   144,    -1,    -1,    -1,    -1,    -1,    -1,
     388,    -1,   153,   391,   155,    -1,    -1,   395,   396,    -1,
      -1,    -1,    -1,   164,   165,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   412,   413,   414,   415,   416,   417,
     418,   419,   420,   421,    -1,   186,   187,    -1,   189,    -1,
     191,    -1,   193,    -1,   195,   196,   197,    -1,   199,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   207,    -1,    -1,    -1,
      -1,    -1,    -1,   214,    -1,    -1,   217,    -1,    -1,    -1,
     221,   222,   223,   224,    -1,    -1,    -1,    -1,    -1,    -1,
     231,    -1,    -1,    -1,    -1,   236,    -1,    -1,    -1,   240,
      -1,    -1,   243,    -1,    -1,   246,    -1,    -1,    -1,    -1,
     251,   252,    -1,    -1,    -1,   256,    -1,    -1,   259,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     271,   272,   273,   274,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   284,   285,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   296,    -1,    -1,    -1,    -1,
      -1,    -1,   303,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   317,   318,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     104,    -1,    -1,    -1,    -1,   109,   110,    -1,    -1,   113,
     114,    -1,    -1,    -1,   355,    -1,    -1,   121,    -1,   123,
     124,    -1,   126,    -1,   128,    -1,    -1,    -1,    -1,    -1,
     134,    -1,    -1,   374,   375,    -1,   140,   141,    -1,   143,
     144,    -1,    -1,    -1,    -1,    -1,    -1,   388,    -1,   153,
     391,   155,    -1,    -1,   395,   396,    -1,    -1,    -1,    -1,
     164,   165,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   412,   413,   414,   415,   416,   417,   418,   419,   420,
     421,    -1,   186,   187,    -1,   189,    -1,   191,    -1,   193,
      -1,   195,   196,   197,    -1,   199,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   207,    -1,    -1,    -1,    -1,    -1,    -1,
     214,    -1,    -1,   217,    -1,    -1,    -1,   221,   222,   223,
     224,    -1,    -1,    -1,    -1,    -1,    -1,   231,    -1,    -1,
      -1,    -1,   236,    -1,    -1,    -1,   240,    -1,    -1,   243,
      -1,    -1,   246,    -1,    -1,    -1,    -1,   251,   252,    -1,
      -1,    -1,   256,    -1,    -1,   259,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   271,   272,   273,
     274,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     284,   285,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   296,    -1,    -1,    -1,    -1,    -1,    -1,   303,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   317,   318,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   104,    -1,    -1,
      -1,    -1,   109,   110,    -1,    -1,   113,   114,    -1,    -1,
      -1,    -1,    -1,    -1,   121,    -1,   123,   124,    -1,   126,
      -1,   128,    -1,    -1,    -1,    -1,    -1,   134,    -1,    -1,
     374,   375,    -1,    -1,   141,    -1,   143,   144,    -1,    -1,
      -1,    -1,    -1,    -1,   388,    -1,   153,   391,   155,    -1,
      -1,   395,   396,    -1,    -1,    -1,    -1,   164,   165,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   412,   413,
     414,   415,   416,   417,   418,   419,   420,   421,    -1,   186,
     187,    -1,   189,    -1,   191,    -1,   193,    -1,   195,   196,
     197,    -1,   199,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     207,    -1,    -1,    -1,    -1,    -1,    -1,   214,    -1,    -1,
      -1,    -1,    -1,    -1,   221,   222,   223,   224,    -1,    -1,
      -1,    -1,    -1,    -1,   231,    -1,    -1,    -1,   235,   236,
      -1,    -1,    -1,   240,    -1,    -1,   243,    -1,    -1,   246,
      -1,    -1,    -1,    -1,   251,   252,    -1,    -1,    -1,   256,
      -1,    -1,   259,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   271,   272,   273,   274,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   285,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   296,
      -1,    -1,    -1,    -1,    -1,    -1,   303,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     317,   318,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   104,    -1,    -1,    -1,
      -1,   109,   110,    -1,   351,   113,   114,    -1,    -1,    -1,
      -1,    -1,    -1,   121,    -1,   123,   124,    -1,   126,    -1,
     128,    -1,    -1,    -1,    -1,    -1,   134,   374,   375,    -1,
      -1,    -1,    -1,   141,    -1,   143,   144,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   391,   153,    -1,   155,   395,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   164,   165,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   412,   413,   414,   415,   416,
     417,   418,   419,   420,   421,    -1,    -1,    -1,   186,   187,
      -1,   189,    -1,   191,    -1,   193,    -1,   195,   196,   197,
      -1,   199,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   207,
      -1,    -1,    -1,    -1,    -1,    -1,   214,    -1,    -1,    -1,
      -1,    -1,    -1,   221,   222,   223,   224,    -1,    -1,    -1,
      -1,    -1,    -1,   231,    -1,    -1,    -1,    -1,   236,    -1,
      -1,    -1,   240,    -1,    -1,   243,    -1,    -1,   246,    -1,
      -1,    -1,    -1,   251,   252,    -1,    -1,    -1,   256,    -1,
      -1,   259,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   271,   272,   273,   274,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   285,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   296,    -1,
      -1,    -1,    -1,    -1,    -1,   303,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   317,
     318,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   104,    -1,    -1,    -1,    -1,   109,   110,
      -1,    -1,   113,   114,    -1,    -1,    -1,    -1,    -1,    -1,
     121,    -1,   123,   124,    -1,   126,    -1,   128,    -1,   130,
      -1,    -1,   370,   134,    -1,    -1,   374,   375,    -1,    -1,
     141,    -1,   143,   144,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   153,   391,   155,    -1,    -1,   395,    -1,    -1,
      -1,    -1,    -1,   164,   165,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   412,   413,   414,   415,   416,   417,
     418,   419,   420,   421,    -1,   186,   187,    -1,   189,    -1,
     191,    -1,   193,    -1,   195,   196,   197,    -1,   199,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   207,    -1,    -1,    -1,
      -1,    -1,    -1,   214,    -1,    -1,    -1,    -1,    -1,    -1,
     221,   222,   223,   224,    -1,    -1,    -1,    -1,    -1,    -1,
     231,    -1,    -1,    -1,    -1,   236,    -1,    -1,    -1,   240,
      -1,    -1,   243,    -1,    -1,   246,    -1,    -1,    -1,    -1,
     251,   252,    -1,    -1,    -1,   256,    -1,    -1,   259,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     271,   272,   273,   274,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   285,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   296,    -1,    -1,    -1,    -1,
      -1,    -1,   303,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   317,   318,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     104,    -1,    -1,    -1,    -1,   109,   110,    -1,    -1,   113,
     114,    -1,    -1,    -1,    -1,    -1,    -1,   121,    -1,   123,
     124,    -1,   126,    -1,   128,    -1,    -1,    -1,    -1,    -1,
     134,    -1,    -1,   374,   375,    -1,    -1,   141,    -1,   143,
     144,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   153,
     391,   155,    -1,    -1,   395,    -1,    -1,    -1,    -1,    -1,
     164,   165,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   412,   413,   414,   415,   416,   417,   418,   419,   420,
     421,    -1,   186,   187,    -1,   189,    -1,   191,    -1,   193,
      -1,   195,   196,   197,    -1,   199,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   207,    -1,    -1,    -1,    -1,    -1,    -1,
     214,    -1,    -1,    -1,    -1,    -1,    -1,   221,   222,   223,
     224,    -1,    -1,    -1,    -1,    -1,    -1,   231,    -1,    -1,
      -1,    -1,   236,    -1,    -1,    -1,   240,    -1,    -1,   243,
      -1,    -1,   246,    -1,    -1,    -1,    -1,   251,   252,    -1,
      -1,    -1,   256,    -1,    -1,   259,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   271,   272,   273,
     274,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   285,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   296,    -1,    -1,    -1,    -1,    -1,    -1,   303,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   317,   318,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   104,    -1,    -1,
      -1,    -1,   109,   110,    -1,    -1,   113,   114,    -1,    -1,
      -1,    -1,    -1,    -1,   121,    -1,   123,   124,    -1,   126,
      -1,   128,    -1,    -1,    -1,    -1,    -1,   134,   372,    -1,
     374,   375,    -1,    -1,   141,    -1,   143,   144,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   153,   391,   155,    -1,
      -1,   395,    -1,    -1,    -1,    -1,    -1,   164,   165,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   412,   413,
     414,   415,   416,   417,   418,   419,   420,   421,    -1,   186,
     187,    -1,   189,    -1,   191,    -1,   193,    -1,   195,   196,
     197,    -1,   199,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     207,    -1,    -1,    -1,    -1,    -1,    -1,   214,    -1,    -1,
      -1,    -1,    -1,    -1,   221,   222,   223,   224,    -1,    -1,
      -1,    -1,    -1,    -1,   231,    -1,    -1,    -1,    -1,   236,
      -1,    -1,    -1,   240,    -1,    -1,   243,    -1,    -1,   246,
      -1,    -1,    -1,    -1,   251,   252,    -1,    -1,    -1,   256,
      -1,    -1,   259,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   271,   272,   273,   274,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   285,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   296,
      -1,    -1,    -1,    -1,    -1,    -1,   303,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     317,   318,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   104,    -1,    -1,    -1,
      -1,   109,   110,    -1,   351,   113,   114,    -1,    -1,    -1,
      -1,    -1,    -1,   121,    -1,   123,   124,    -1,   126,    -1,
     128,    -1,    -1,    -1,    -1,    -1,   134,   374,   375,    -1,
      -1,    -1,    -1,   141,    -1,   143,   144,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   391,   153,    -1,   155,   395,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   164,   165,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   412,   413,   414,   415,   416,
     417,   418,   419,   420,   421,    -1,    -1,    -1,   186,   187,
      -1,   189,    -1,   191,    -1,   193,    -1,   195,   196,   197,
      -1,   199,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   207,
      -1,    -1,    -1,    -1,    -1,    -1,   214,    -1,    -1,    -1,
      -1,    -1,    -1,   221,   222,   223,   224,    -1,    -1,    -1,
      -1,    -1,    -1,   231,    -1,    -1,    -1,    -1,   236,    -1,
      -1,    -1,   240,    -1,    -1,   243,    -1,    -1,   246,    -1,
      -1,    -1,    -1,   251,   252,    -1,    -1,    -1,   256,    -1,
      -1,   259,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   271,   272,   273,   274,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   285,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   296,    -1,
      -1,    -1,    -1,    -1,    -1,   303,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   317,
     318,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   104,    -1,    -1,    -1,    -1,
     109,   110,    -1,   351,   113,   114,    -1,    -1,    -1,    -1,
      -1,    -1,   121,    -1,   123,   124,    -1,   126,    -1,   128,
      -1,    -1,    -1,    -1,    -1,   134,   374,   375,    -1,    -1,
      -1,    -1,   141,    -1,   143,   144,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   391,   153,    -1,   155,   395,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   164,   165,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   412,   413,   414,   415,   416,   417,
     418,   419,   420,   421,    -1,    -1,    -1,   186,   187,    -1,
     189,    -1,   191,    -1,   193,    -1,   195,   196,   197,    -1,
     199,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   207,    -1,
      -1,    -1,    -1,    -1,    -1,   214,    -1,    -1,    -1,    -1,
      -1,    -1,   221,   222,   223,   224,    -1,    -1,    -1,    -1,
      -1,    -1,   231,    -1,    -1,    -1,    -1,   236,    -1,    -1,
      -1,   240,    -1,    -1,   243,    -1,    -1,   246,    -1,    -1,
      -1,    -1,   251,   252,    -1,    -1,    -1,   256,    -1,    -1,
     259,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   271,   272,   273,   274,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   285,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   296,    -1,    -1,
      -1,    -1,    -1,    -1,   303,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   317,   318,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   104,    -1,    -1,    -1,    -1,   109,
     110,    -1,   351,   113,   114,    -1,    -1,    -1,    -1,    -1,
      -1,   121,    -1,   123,   124,    -1,   126,    -1,   128,    -1,
      -1,    -1,    -1,    -1,   134,   374,   375,    -1,    -1,    -1,
      -1,   141,    -1,   143,   144,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   391,   153,    -1,   155,   395,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   164,   165,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   412,   413,   414,   415,   416,   417,   418,
     419,   420,   421,    -1,    -1,    -1,   186,   187,    -1,   189,
      -1,   191,    -1,   193,    -1,   195,   196,   197,    -1,   199,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   207,    -1,    -1,
      -1,    -1,    -1,    -1,   214,    -1,    -1,    -1,    -1,    -1,
      -1,   221,   222,   223,   224,    -1,    -1,    -1,    -1,    -1,
      -1,   231,    -1,    -1,    -1,    -1,   236,    -1,    -1,    -1,
     240,    -1,    -1,   243,    -1,    -1,   246,    -1,    -1,    -1,
      -1,   251,   252,    -1,    -1,    -1,   256,    -1,    -1,   259,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   271,   272,   273,   274,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   285,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   296,    -1,    -1,    -1,
      -1,    -1,    -1,   303,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   317,   318,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   104,    -1,    -1,    -1,    -1,   109,   110,
      -1,   351,   113,   114,    -1,    -1,    -1,    -1,    -1,    -1,
     121,    -1,   123,   124,    -1,   126,    -1,   128,    -1,    -1,
      -1,    -1,    -1,   134,   374,   375,    -1,    -1,    -1,    -1,
     141,    -1,   143,   144,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   391,   153,    -1,   155,   395,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   164,   165,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   412,   413,   414,   415,   416,   417,   418,   419,
     420,   421,    -1,    -1,    -1,   186,   187,    -1,   189,    -1,
     191,    -1,   193,    -1,   195,   196,   197,    -1,   199,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   207,    -1,    -1,    -1,
      -1,    -1,    -1,   214,    -1,    -1,    -1,    -1,    -1,    -1,
     221,   222,   223,   224,    -1,    -1,    -1,    -1,    -1,    -1,
     231,    -1,    -1,    -1,    -1,   236,    -1,    -1,    -1,   240,
      -1,    -1,   243,    -1,    -1,   246,    -1,    -1,    -1,    -1,
     251,   252,    -1,    -1,    -1,   256,    -1,    -1,   259,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     271,   272,   273,   274,    -1,   106,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   285,   116,    -1,    -1,    -1,   120,
      -1,    -1,   123,    -1,    -1,   296,   127,    -1,    -1,    -1,
      -1,   132,   303,    -1,    -1,    -1,    -1,   138,    -1,   140,
      -1,    -1,    -1,    -1,    -1,    -1,   317,   318,    -1,    -1,
      -1,    -1,    -1,    -1,   155,    -1,    -1,    -1,    -1,   160,
      -1,    -1,   163,    -1,    -1,   166,    -1,    -1,    -1,    -1,
     171,    -1,    -1,    -1,   175,    -1,    -1,    -1,   179,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   194,   195,   196,    -1,    -1,    -1,    -1,
      -1,   202,    -1,   374,   375,    -1,   207,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     391,    -1,    -1,    -1,   395,    -1,    -1,    -1,   229,    -1,
      -1,   232,    -1,   234,   235,    -1,    -1,   238,    -1,    -1,
      -1,   412,   413,   414,   415,   416,   417,   418,   419,   420,
     421,    -1,    -1,    -1,    -1,    -1,   257,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   267,   268,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   292,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   106,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   116,    -1,    -1,    -1,
     120,    -1,    -1,   123,   325,   326,   327,   127,    -1,    -1,
      -1,    -1,   132,    -1,    -1,    -1,    -1,    -1,   138,    -1,
     140,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     351,    -1,    -1,    -1,    -1,   155,    -1,   157,    -1,    -1,
     160,    -1,   363,   163,    -1,    -1,   166,    -1,    -1,    -1,
     371,   171,    -1,   374,   375,   175,    -1,    -1,    -1,   179,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   194,   195,   196,    -1,    -1,    -1,
      -1,    -1,   202,    -1,    -1,    -1,    -1,   207,    -1,    -1,
      -1,   412,   413,   414,   415,   416,   417,   418,   419,   420,
     421,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   229,
      -1,    -1,   232,    -1,   234,   235,    -1,    -1,   238,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   257,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   267,   268,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   292,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   106,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   116,    -1,    -1,
      -1,   120,    -1,    -1,   123,   325,   326,   327,   127,    -1,
      -1,    -1,    -1,   132,    -1,    -1,    -1,    -1,    -1,   138,
      -1,   140,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   351,    -1,    -1,    -1,   154,   155,    -1,    -1,    -1,
      -1,   160,    -1,   363,   163,    -1,    -1,   166,    -1,    -1,
      -1,    -1,   171,    -1,   374,   375,   175,    -1,    -1,    -1,
     179,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   194,   195,   196,    -1,    -1,
      -1,    -1,    -1,   202,    -1,    -1,    -1,    -1,   207,    -1,
      -1,    -1,   412,   413,   414,   415,   416,   417,   418,   419,
     420,   421,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     229,    -1,    -1,   232,    -1,   234,   235,    -1,    -1,   238,
      -1,    -1,    -1,    -1,    -1,   106,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   116,    -1,    -1,   257,   120,
      -1,    -1,   123,    -1,    -1,    -1,   127,    -1,   267,   268,
      -1,   132,    -1,    -1,    -1,    -1,    -1,   138,    -1,   140,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   292,   155,    -1,    -1,    -1,    -1,   160,
      -1,    -1,   163,    -1,    -1,   166,    -1,    -1,    -1,    -1,
     171,    -1,    -1,    -1,   175,    -1,    -1,    -1,   179,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   325,   326,   327,    -1,
      -1,    -1,    -1,   194,   195,   196,    -1,    -1,    -1,    -1,
      -1,   202,    -1,    -1,    -1,    -1,   207,    -1,    -1,    -1,
      -1,    -1,   351,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   363,    -1,    -1,    -1,   229,    -1,
      -1,   232,    -1,   234,   235,   374,   375,   238,     7,     8,
       9,    -1,    -1,    -1,    13,    14,    -1,    16,    -1,    -1,
      19,    20,    -1,    -1,    23,    24,   257,    -1,    27,    -1,
      -1,    30,    31,    32,    33,    -1,   267,   268,    -1,    -1,
      -1,    40,    -1,   412,   413,   414,   415,   416,   417,   418,
     419,   420,   421,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   292,    -1,    -1,    63,    64,    65,    66,    67,    68,
      69,    70,    71,    72,    73,    74,    75,    76,    77,    78,
      79,    80,    81,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   325,   326,   327,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   106,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     351,    -1,    -1,   123,    -1,    -1,   126,   127,    -1,   129,
      -1,    -1,   363,   133,    -1,    -1,    -1,    -1,   138,   139,
     140,   141,    -1,   374,   375,   145,    -1,   147,   148,    -1,
      -1,    -1,    -1,    -1,    -1,   155,    -1,    -1,    -1,   159,
      -1,    -1,    -1,   163,    -1,    -1,   166,    -1,    -1,    -1,
      -1,    -1,   172,    -1,    -1,    -1,    -1,    -1,    -1,   179,
      -1,   412,   413,   414,   415,   416,   417,   418,   419,   420,
     421,    -1,    -1,    -1,    -1,   195,   196,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   207,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   227,    -1,   229,
     230,    -1,   232,    -1,   234,   235,    -1,    -1,   238,    -1,
      -1,    -1,   106,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   255,    -1,   257,    -1,    -1,
      -1,    -1,   126,   127,    -1,    -1,    -1,    -1,   268,   133,
      -1,    -1,    -1,    -1,    -1,   139,   140,   141,    -1,    -1,
      -1,    -1,    -1,   147,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   155,    -1,   293,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   166,    -1,   304,   305,    -1,    -1,   172,    -1,
      -1,   311,    -1,    -1,    -1,   179,    -1,    -1,    -1,    -1,
     320,    -1,    -1,    -1,    -1,   325,   326,   327,   328,   329,
      -1,   195,   196,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   351,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   229,   230,    -1,   232,    -1,
     234,   235,    -1,    -1,   238,   375,    -1,   377,    -1,   379,
     380,    -1,    -1,    -1,    -1,   385,   106,    -1,    -1,    -1,
     390,    -1,    -1,   257,    -1,    -1,    -1,   397,    -1,    -1,
     400,    -1,    -1,    -1,   268,    -1,   126,   127,    -1,    -1,
      -1,    -1,    -1,   133,    -1,    -1,    -1,    -1,    -1,   139,
     140,   141,    -1,    -1,    -1,    -1,    -1,   147,    -1,   293,
      -1,    -1,    -1,    -1,    -1,   155,    -1,    -1,    -1,    -1,
     304,   305,    -1,    -1,    -1,    -1,   166,   311,    -1,    -1,
      -1,    -1,   172,    -1,    -1,    -1,   320,    -1,    -1,   179,
      -1,   325,   326,   327,   328,   329,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   195,   196,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   351,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   371,    -1,   229,
     230,   375,   232,    -1,   234,   235,    -1,    -1,   238,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   397,    -1,    -1,    -1,   257,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   268,    -1,
      -1,    -1,     4,    -1,    -1,     7,     8,     9,    -1,    -1,
      -1,    13,    14,    15,    16,    -1,    -1,    19,    20,    21,
      -1,    23,    24,   293,    26,    27,    -1,    29,    30,    31,
      32,    33,    -1,    -1,   304,   305,    -1,    -1,    40,    -1,
      -1,   311,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     320,    -1,    -1,    -1,    -1,   325,   326,   327,   328,   329,
      -1,    63,    64,    65,    66,    67,    68,    69,    70,    71,
      72,    73,    74,    75,    76,    77,    78,    79,    80,    81,
      -1,   351,    -1,     7,     8,     9,    -1,    -1,    -1,    13,
      14,    -1,    16,    -1,    -1,    19,    20,    -1,    -1,    23,
      24,   371,    -1,    27,    -1,   375,    30,    31,    32,    33,
      -1,    35,    -1,    -1,    -1,    -1,    40,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   397,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    63,
      64,    65,    66,    67,    68,    69,    70,    71,    72,    73,
      74,    75,    76,    77,    78,    79,    80,    81
};

/* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
   symbol of state STATE-NUM.  */
static const yytype_uint16 yystos[] =
{
       0,     3,     4,     7,     8,     9,    13,    14,    15,    16,
      19,    20,    21,    23,    24,    26,    27,    29,    30,    31,
      32,    33,    39,    40,    63,    64,    65,    66,    67,    68,
      69,    70,    71,    72,    73,    74,    75,    76,    77,    78,
      79,    80,    81,    84,    85,    86,    92,    93,    94,    95,
      96,    97,   100,   422,   429,   447,   448,   449,   471,   472,
     476,   477,   479,   482,   489,   492,   493,   496,   519,   521,
     523,   524,   525,   526,   527,   528,   529,   530,   531,   532,
     533,    82,    82,   445,   521,    88,    90,    87,    99,    99,
      99,    99,   113,   116,   154,   399,   407,   408,   412,   534,
     571,   572,   573,   574,   575,   492,   477,   496,   497,     0,
     448,   443,   473,   475,   492,   472,   472,   445,   480,   481,
     521,   445,   471,   472,   510,   513,   514,   520,   422,   424,
     494,   495,   493,   490,   491,   521,   445,    91,    91,   371,
     375,   138,   138,   351,   201,   406,   409,   576,   106,   123,
     126,   127,   129,   133,   138,   139,   140,   141,   145,   147,
     148,   155,   159,   163,   166,   172,   179,   195,   196,   207,
     227,   229,   230,   232,   234,   235,   238,   255,   257,   268,
     293,   304,   305,   311,   320,   325,   326,   327,   328,   329,
     351,   375,   377,   379,   380,   385,   390,   397,   400,   535,
     536,   537,   544,   545,   546,   550,   551,   552,   553,   554,
     555,   556,   558,   559,   560,   561,   563,   564,   565,   566,
     567,   568,   569,   570,   584,   585,   586,   587,   588,   589,
     590,   591,   595,   596,   599,   600,   601,   603,   604,   605,
     609,   610,   621,   622,   623,   624,   625,   626,   632,   633,
     634,   637,   638,   639,   660,   661,   662,   664,   665,   666,
     667,   668,   670,   676,   680,   699,   702,   706,   710,   711,
     723,   765,   766,   767,   773,   774,   775,   776,   779,   780,
     781,   782,   783,   784,   785,   788,   789,   799,   800,   801,
     802,   803,   813,   821,   842,   843,   844,   845,   846,   849,
     850,   863,   931,   994,   995,   996,   997,   998,   999,  1075,
    1076,  1077,  1078,  1079,  1087,  1089,  1094,  1100,  1101,  1102,
    1103,  1105,  1106,  1124,  1125,  1126,  1127,  1143,  1144,  1145,
    1146,  1149,  1151,  1152,  1160,  1161,   362,   362,   362,   362,
     423,   477,   496,   427,   443,   474,   442,   520,   444,   483,
     484,   485,   497,   523,   444,   483,   445,   492,     5,     6,
      10,    11,    12,    17,    18,    22,    25,    28,    34,    37,
      38,    41,    52,    53,   422,   428,   429,   430,   431,   432,
     433,   443,   444,   450,   451,   453,   454,   455,   456,   457,
     458,   459,   460,   461,   462,   463,   464,   465,   466,   467,
     469,   471,   508,   509,   510,   511,   512,   515,   516,   517,
     518,   521,   522,   533,   471,   510,   423,   498,   499,   521,
     425,   453,   466,   470,   521,   497,   500,   501,   502,   503,
     427,   444,   442,   490,    89,    90,   215,   215,   407,   389,
     132,   171,   375,   387,   413,   137,   170,   219,   221,   236,
     237,   244,   249,   258,   264,   288,   300,   315,   319,   375,
     375,   402,   206,   269,   602,   371,   134,   137,   143,   170,
     192,   208,   219,   220,   221,   236,   243,   244,   245,   249,
     252,   254,   258,   280,   288,   300,   319,   375,   644,   645,
     768,   777,   375,   104,   109,   110,   113,   114,   120,   121,
     123,   124,   126,   128,   134,   141,   143,   144,   153,   155,
     162,   164,   165,   186,   187,   188,   189,   191,   193,   195,
     196,   197,   199,   202,   207,   214,   217,   221,   222,   223,
     224,   231,   236,   240,   243,   246,   250,   251,   252,   253,
     256,   259,   271,   272,   273,   274,   285,   294,   296,   302,
     303,   317,   318,   321,   322,   339,   351,   356,   358,   366,
     369,   370,   371,   372,   373,   374,   375,   381,   383,   391,
     395,   412,   413,   414,   415,   416,   417,   418,   419,   420,
     421,   592,   594,   958,   960,   961,   962,   963,   964,   965,
     968,   975,   989,  1013,  1162,  1163,  1166,   368,   851,   851,
     137,   143,   170,   219,   220,   221,   236,   243,   244,   249,
     252,   258,   264,   280,   288,   300,   315,   319,   375,   777,
     118,   235,   851,   116,   138,   363,   375,   392,  1081,  1082,
    1083,   168,   217,   376,   388,   395,   396,   401,   409,   547,
     548,   370,   371,   375,   105,   106,   133,   140,   147,   159,
     172,   179,   222,   225,   235,   257,   270,   288,   311,   627,
     628,  1162,   851,   851,   851,   375,   161,   201,   254,   284,
     935,   936,   937,   938,   939,   958,   960,   627,   602,  1162,
     851,   104,   375,  1162,   161,   851,   847,   848,  1162,   607,
    1162,   244,   244,   244,   244,   351,   354,   864,   244,   244,
     845,   166,   386,   204,   206,   204,   206,   386,   400,   273,
     371,   375,   362,   375,   842,   362,   106,   127,   133,   166,
     232,   371,   585,   586,   589,   371,   585,   586,   275,   362,
     362,   788,   789,   799,   863,   589,   159,   163,   390,   402,
     402,   607,   164,   189,   807,   166,   924,   180,   209,   253,
     289,   831,   918,   845,   106,   595,   596,   788,   789,   799,
     842,   362,   362,   362,   607,   607,   607,   607,  1162,   475,
     445,   467,   506,    83,   478,   444,   484,   441,   486,   488,
     492,   478,   444,   443,   470,   443,   441,   508,   422,   521,
     443,   469,   422,   453,   422,   422,   422,   522,   422,   453,
     453,   469,   497,   503,    52,    53,    54,   422,   424,   426,
      42,    43,    44,    45,    46,    47,    48,    49,    50,    51,
     442,   468,   455,   429,   434,   435,   430,   431,    58,    59,
      61,    62,   436,   437,    56,    57,   428,   438,   439,    55,
      60,   440,   427,   443,   444,   511,   441,   423,   427,   425,
     422,   424,   492,   496,   504,   505,   423,   427,   491,   470,
     444,   113,   362,   364,   132,   171,   375,   387,   375,   132,
     375,   382,   371,   414,   577,   578,   110,   114,   116,   133,
     154,   199,   224,   293,   323,  1090,  1162,   607,   607,   607,
     607,   123,   238,   607,   607,  1162,   607,   607,   264,  1162,
    1162,   109,   238,   375,   375,   125,   244,   375,   287,   319,
     279,  1162,   254,   294,   243,   607,   607,   607,   607,   319,
     288,  1162,  1162,   264,  1162,   375,   315,   264,   137,   242,
     378,   394,   405,   538,   265,   960,   973,   974,   351,   965,
     121,   592,   966,  1162,   359,   268,   834,   935,   988,   965,
     965,   176,   210,   393,  1178,  1179,  1180,   371,   371,   367,
     967,   351,   350,   347,   356,   358,   355,   360,   351,   353,
     359,   168,   351,   375,   607,   786,   787,   355,   854,   857,
     858,   960,  1162,   279,  1162,  1162,   607,   607,   243,   607,
     607,   607,   607,   607,   607,  1162,   607,  1162,   607,   264,
    1162,  1162,   375,   137,   263,   375,   181,   106,   116,   120,
     123,   127,   132,   138,   160,   163,   166,   171,   175,   194,
     202,   207,   229,   232,   235,   238,   267,   268,   292,   351,
     363,   371,   599,   600,   601,   603,   788,   789,   799,   800,
     813,   821,   825,   826,   827,   832,   835,   838,   840,   863,
    1011,  1013,  1014,  1018,  1041,  1042,  1043,  1044,  1045,  1046,
    1047,  1048,  1052,  1059,  1060,  1061,  1062,  1063,  1065,  1066,
    1067,  1069,  1070,  1071,  1072,  1073,  1074,  1080,  1085,  1086,
    1162,  1175,   135,   251,  1021,  1022,  1023,  1025,  1026,  1027,
    1029,  1162,   363,   259,   362,   116,  1083,   356,   358,   370,
     549,   549,   375,   168,  1166,   218,   108,   237,   258,   319,
     375,   108,   219,   220,   236,   237,   243,   244,   249,   258,
     264,   280,   300,   319,   108,   108,   220,   258,   319,   108,
     108,   108,   108,   108,   108,   206,   248,   357,   105,   181,
     181,   181,   259,   371,   939,   351,   989,   989,   208,   107,
     350,   177,   182,   190,   201,   283,   349,   363,   364,   365,
     940,   941,   942,   943,   944,   945,   946,   947,   948,   949,
     950,   951,   952,   953,   954,   955,   956,   957,   168,   206,
     248,   375,   105,   146,   853,   364,   183,   222,   606,   364,
     786,   796,   797,   798,   357,   351,   769,   248,   359,   607,
     607,  1162,   607,   352,   372,   865,   866,  1162,  1162,   352,
     370,   371,   371,   375,   371,   375,   375,   371,  1167,   357,
     168,   166,   541,   541,   237,   602,   208,   602,   362,   788,
     789,   799,   375,   547,   273,   206,   297,   375,   925,   257,
     268,   772,   117,   375,   105,   849,   191,   194,   920,   922,
     923,   918,   237,   375,   924,   351,  1001,  1001,   111,   182,
    1000,   103,   115,  1128,  1130,   111,   506,   507,   478,   470,
     427,   443,   487,   441,   478,   441,   508,   533,    34,   443,
     469,   443,   443,   503,   469,   469,   469,   423,   422,   496,
     423,   521,   423,   452,   467,   469,   521,   467,   455,   455,
     455,   456,   456,   457,   457,   458,   458,   458,   458,   459,
     459,   460,   461,   462,   463,   464,   465,   467,   508,   533,
      35,   521,   423,   500,   504,   425,   470,   505,   422,   424,
      35,   502,   375,   162,   250,   375,   132,   375,   382,   351,
     364,   370,   371,   580,   357,   137,   193,   319,   375,   375,
     375,   137,   375,   241,   375,   237,   375,  1098,  1099,   128,
     128,   375,   136,   144,   153,   192,   200,   241,   325,   375,
     677,   678,   698,   709,   137,   315,   375,   379,   102,   105,
     106,   124,   144,   147,   153,   195,   240,   285,   293,   294,
     312,   320,   375,   686,   128,   144,   153,  1142,   139,   144,
     153,   245,   285,   287,   613,   614,   615,   616,   617,   618,
     619,   128,   223,   238,   293,   375,   703,   704,   607,   102,
     147,   165,   238,   241,   375,   106,   107,   116,   147,   154,
     293,   313,   314,  1104,  1119,   375,   375,   238,   285,   325,
     375,   608,  1162,   607,   137,   117,  1162,  1162,   375,  1088,
     288,   143,   170,   219,   249,   252,   375,   768,   607,   351,
     677,   707,   166,   168,   179,   213,   222,   312,   319,   351,
     724,   726,   731,   732,   763,  1184,  1162,   287,   607,   375,
     158,   159,  1084,  1162,   607,   279,   404,   539,   935,   265,
     969,   970,   151,   972,   974,   935,   359,   592,  1162,   836,
     837,  1162,   235,   841,   990,   991,   352,   357,   210,  1178,
    1178,   371,   399,  1176,  1177,  1176,   352,   988,   317,   962,
     963,   963,   964,   964,   105,   146,   352,   355,   988,   935,
    1009,   140,   217,   284,   388,   395,   396,   592,   593,  1162,
     786,   787,   351,   828,   829,   830,   833,   835,   839,   841,
     357,   213,  1188,   111,   372,   859,  1162,   168,   181,  1169,
     357,   859,   359,  1162,   607,   119,   675,   119,   701,   607,
     375,  1123,   279,   166,   166,   607,   237,  1041,   265,   960,
    1007,  1053,  1054,  1162,   602,   362,  1021,  1005,  1162,  1162,
    1015,  1162,  1162,   935,  1008,  1041,   362,  1011,  1162,   362,
    1009,   602,   851,  1014,  1162,  1008,   836,   362,  1040,  1162,
     832,   363,  1178,   362,   362,   362,   362,   362,   362,   362,
     362,   362,   362,   924,   831,   918,   832,   788,   789,   799,
     825,   361,   157,  1034,  1035,  1042,   362,  1005,   362,   121,
     975,  1010,  1162,   351,   359,   361,  1162,  1162,  1023,   157,
     286,  1028,  1162,   149,   371,   381,   383,   562,  1164,   370,
     370,   181,   259,   219,   236,   244,   249,   288,   300,   375,
     143,   219,   236,   244,   249,   264,   288,   300,   375,   243,
     264,   244,   143,   219,   236,   243,   244,   249,   264,   280,
     288,   300,   375,   243,   219,   218,   280,   244,   236,   244,
     244,   244,   143,   607,   220,   629,   630,  1162,   628,   181,
     790,   791,   786,   607,   607,   562,  1165,  1166,   937,   938,
     375,   389,   201,   202,   960,   177,   190,   283,   960,   364,
     105,   108,   239,   364,   365,   105,   108,   239,   105,   108,
     239,   364,   960,   960,   960,   960,   960,   960,   202,   342,
     343,   351,   369,   370,   372,   373,   592,   895,   959,   989,
    1162,  1166,   959,   959,   959,   959,   959,   959,   959,   959,
     959,   959,   959,   629,   143,   607,   234,   137,   854,   144,
     153,   188,   270,   398,  1162,   859,   238,   357,   169,   178,
     186,   231,   900,   848,   592,   770,   771,   111,  1162,  1162,
     248,   359,  1188,   354,   352,   357,   351,   213,   671,   268,
     557,   557,   357,   371,   222,   257,   238,   294,   259,   375,
    1167,   607,   370,   925,   222,   927,   928,   960,   117,   166,
     921,   935,   935,   922,   238,   375,   772,   352,  1002,  1003,
    1162,   229,  1000,  1020,  1024,  1029,   206,   140,   179,   257,
    1129,   372,   427,   444,   488,   470,   508,   533,   422,   443,
     469,   443,   423,   423,   423,   423,   455,   423,   427,   425,
     441,   423,   423,   425,   423,   500,   425,   470,   362,   352,
     352,   351,   415,   416,   579,   371,   581,   582,   578,   248,
     248,  1162,   372,   256,  1095,   372,   375,   123,   127,   160,
     232,   375,   392,   375,   268,   261,   310,   679,   117,   268,
     358,   370,   678,   375,   279,   364,   364,   375,   606,   364,
     125,   130,   213,   696,   700,   740,   741,   288,   213,   319,
     700,   213,   233,   130,   192,   213,   216,   254,   700,   233,
     214,   213,   213,  1184,   125,   130,   213,   248,   607,   370,
     213,   213,   685,   319,   375,   375,   319,  1162,   117,   619,
     213,   375,   704,   248,   204,   206,   364,   705,   375,  1150,
     244,   375,   244,   375,   105,   297,   654,   271,   290,   314,
     375,   113,   268,   375,   659,   244,   271,   375,   398,   655,
     656,   375,   375,   114,   375,   114,   375,   241,   375,   598,
     364,   126,   144,   153,   154,   241,   375,  1084,  1155,  1157,
    1159,   375,   375,   182,   359,   182,   279,   371,   375,   129,
     259,   778,   364,  1090,   375,   264,   166,   370,   592,   743,
     744,   708,   709,   607,   244,   375,   117,   270,   375,   398,
     370,  1162,   738,   739,   740,   744,   273,   733,   734,   735,
     144,   153,   725,   732,   731,   375,   117,   769,   372,  1107,
    1108,  1085,  1162,   166,   650,   769,  1162,   135,   247,   971,
     960,   970,   972,   960,   154,   111,   592,  1162,   357,   769,
     851,   831,   922,   935,  1178,  1179,   371,  1179,   352,   355,
     988,   988,   212,   979,   352,   352,   354,   967,   351,   353,
     359,   859,   259,   833,   352,   831,   918,   833,   375,   351,
     372,   592,   266,   934,   796,   371,   381,   383,  1170,   168,
     804,   858,   355,  1162,   131,   375,  1162,   375,   375,   351,
     755,   238,  1034,  1008,   265,  1055,  1056,   151,  1058,  1053,
     359,   362,   116,   265,  1068,   181,   359,   176,   177,  1189,
     362,   247,  1051,   154,   362,   359,   362,   853,   364,   359,
    1060,   362,   359,   352,  1162,   362,   838,   922,   918,   362,
     362,   362,   924,   364,   265,  1036,  1037,  1038,   154,   362,
     154,   359,   352,   988,   592,  1011,  1162,   364,  1001,   182,
     362,   592,  1016,  1017,  1162,   365,  1166,  1178,   371,   371,
     357,  1170,   149,   264,   264,   264,  1162,   248,   357,   786,
     791,   828,   859,  1188,  1188,   357,   202,   156,   960,   960,
     107,   105,   108,   239,   105,   108,   239,   105,   108,   239,
     105,   108,   239,   988,   359,  1162,   168,  1162,   279,   181,
    1171,   222,   375,   351,   592,   808,   809,   810,  1162,   797,
     211,   901,   901,   901,   184,   352,   357,   351,   115,  1162,
     176,   354,   866,   213,   669,   117,   725,   375,   371,   398,
     203,   297,   375,   542,   375,  1164,   181,   351,   330,   331,
     332,   333,   334,   335,   926,   398,   357,   112,   142,   929,
     927,   325,   357,   375,   375,   352,   357,   176,   210,  1004,
    1016,  1019,  1021,  1005,   154,  1029,   607,   203,  1131,   208,
     444,   506,   469,   423,   469,   443,   443,   469,   508,   508,
     508,   467,   466,   423,   425,  1178,   357,   372,   372,   248,
     111,  1097,   375,   259,  1096,   248,   370,   370,   370,   358,
     370,   358,   370,   358,   370,   370,   244,   105,  1162,   139,
     162,   250,   375,   370,   372,   375,   364,   162,   250,  1162,
    1162,   288,   693,   351,   122,   167,   192,   216,   254,   698,
    1162,  1162,   273,   351,   197,  1162,   254,  1162,   185,   351,
     592,   197,  1162,  1162,  1162,   592,  1162,  1162,  1162,   685,
    1162,  1162,  1162,  1162,   204,   206,   620,  1162,  1162,   607,
     204,   206,   206,   375,  1148,   168,   359,   653,   372,   647,
     648,   653,   647,   297,   370,   370,   375,   644,   144,   153,
     647,   698,   375,   314,   657,   658,  1162,   370,   271,   656,
     372,  1107,   372,  1121,  1122,  1121,   358,   370,   372,   375,
     372,   960,   960,   370,  1085,   375,   372,  1162,   372,   207,
     259,   370,   248,  1162,   259,   370,  1117,   375,  1091,  1092,
    1093,   607,   607,   261,   262,   310,   745,   751,   752,  1162,
     352,   357,   375,   191,  1162,   352,   357,   274,   351,   111,
     735,   233,   233,   731,  1107,  1162,   724,   375,  1109,  1110,
     296,   309,   357,  1112,  1113,  1115,  1116,   241,   359,   336,
     375,   111,   651,   111,   259,   778,   268,   540,   960,   971,
     154,   752,   359,   837,   111,   853,   991,   352,   352,   352,
     352,   351,   307,   992,   281,   976,   359,   352,   988,  1009,
     592,   593,  1162,   934,   796,   352,   839,   922,   918,  1162,
     935,   919,   920,   934,  1178,   371,   371,   357,   805,   806,
    1162,   934,   359,   107,   119,   635,   181,   181,   592,   759,
     260,   375,   154,  1051,  1007,  1057,  1058,  1055,  1041,   154,
    1162,  1041,  1008,   362,  1012,  1014,  1162,   316,  1007,  1011,
    1162,  1041,   151,   152,   308,  1049,  1050,   194,  1011,  1162,
     854,  1009,  1162,  1005,  1162,   365,   922,   362,  1009,   291,
    1039,  1040,  1038,  1005,   362,   395,  1162,   352,   359,   182,
     244,   375,   350,   139,   361,  1006,   351,   359,   365,  1176,
    1176,   371,   381,   383,  1166,   248,   629,   630,   351,   820,
     139,   351,   820,   828,   863,   859,   755,  1166,   960,   156,
     107,   960,   352,   592,  1162,   168,   629,   371,   375,   381,
     383,  1172,  1173,   168,   860,   375,   222,   592,   811,   812,
    1162,   357,   934,   364,   359,   797,   771,   843,   147,  1188,
     233,   375,   932,  1162,   352,  1162,   674,  1184,   370,   543,
    1162,   370,   364,  1170,   757,   935,   928,   303,   930,   935,
     935,   935,  1003,   210,  1016,  1000,  1005,   116,   226,  1132,
     759,   140,   179,   257,   423,   508,   423,   423,   469,   423,
     469,   443,    36,   364,   582,   372,   372,   372,   114,   372,
     370,   370,   370,   204,   206,   370,   398,   260,   319,  1181,
    1182,   351,   740,   742,   743,   935,   185,   254,   185,   351,
     756,   319,   288,   690,   274,   351,   592,   351,   701,   592,
     758,   357,   113,   260,  1184,   248,   248,   248,   259,   681,
     682,   127,   375,  1148,  1162,   357,   648,   370,   268,   351,
     357,   359,   370,   313,   314,   375,  1110,  1120,   357,   248,
     370,   375,   192,   597,   359,   371,   371,   375,  1109,  1162,
    1162,   375,   238,   375,   375,  1093,   375,  1092,   769,   176,
     746,   745,   375,   753,   754,   351,   763,   764,   744,   607,
     370,   351,   724,   739,   111,   759,   828,   197,   197,  1114,
    1115,   611,   612,   614,   617,   618,   619,   733,   204,   206,
     370,   375,  1109,   375,   370,  1118,  1108,  1113,   960,  1162,
     282,   375,   375,   640,   828,  1162,   259,   229,   391,   166,
     352,   592,   351,   854,   992,   992,   979,   213,   980,   351,
     979,   173,   992,   592,   352,   354,   967,   351,   359,   919,
     934,   922,   352,  1176,  1176,   371,   381,   383,   359,   355,
     375,   131,  1166,  1166,   352,   357,   351,  1005,  1051,   154,
     120,   362,  1034,   357,   362,   181,  1007,   348,  1060,   351,
    1041,  1008,  1008,   154,   362,   181,   855,  1168,   362,   359,
     362,   365,   362,  1051,  1051,   208,   359,   592,  1011,  1162,
     828,   203,   351,   251,   295,  1009,   364,   362,   370,   592,
    1162,   138,  1179,  1179,  1178,   371,   371,   629,   268,   631,
     810,   819,   260,   260,   819,   260,   828,   259,   168,   960,
     960,   359,   629,   635,  1172,  1172,  1178,   357,   244,   324,
     351,   861,   862,   863,   896,   899,  1162,   934,   352,   357,
     359,   809,   919,   139,   960,   592,   206,   352,   293,   663,
     176,   375,   233,   257,   290,   248,   351,   733,   357,   352,
     357,   929,   388,   395,  1006,  1019,  1041,   198,   205,  1133,
    1134,  1135,   166,  1139,  1131,   443,   508,   508,   423,   508,
     423,   423,   469,   508,   417,   418,   419,   420,   421,   583,
     193,   187,  1162,   733,   694,   695,  1162,   352,   352,   351,
     756,   929,   756,   758,   238,   713,  1162,   351,   273,   687,
     111,   759,   147,   201,   202,   238,   758,   929,   352,   357,
    1162,   351,   351,   592,  1162,  1162,   375,   293,   683,   359,
     370,   372,   643,   370,   658,  1162,  1118,  1122,  1121,  1162,
     207,   357,  1109,   287,  1162,   238,   111,   233,   352,   746,
     259,   370,  1181,   682,   759,   733,   351,   352,   612,   375,
    1147,   375,   396,   375,  1109,   375,   375,   154,  1153,   359,
     192,   271,   314,   375,   641,   642,   268,   772,  1162,   268,
     829,   181,   856,   979,   979,   117,   209,   983,   375,   351,
     979,   359,   352,   988,   592,  1179,  1179,  1178,   371,   371,
    1162,   635,   592,   139,   792,   793,   960,   362,   120,   362,
     154,  1014,  1012,   348,  1007,  1005,   352,   988,  1051,  1051,
     175,  1012,  1174,  1175,   860,   860,  1040,  1162,   924,  1017,
    1030,  1162,   592,  1032,  1033,  1009,   352,   357,   350,   359,
    1021,  1176,  1176,   631,   172,   352,   357,   351,   352,   351,
     794,   795,   862,   607,   592,   635,   375,   636,  1176,  1176,
     381,   383,  1172,   351,   351,   828,   357,   900,   351,   346,
     351,   359,  1188,   129,   241,   301,   913,   914,   916,   364,
     812,  1162,   935,   248,   932,   375,   925,   244,   759,  1162,
     192,   714,   715,   935,  1005,  1034,   357,   233,   244,  1136,
     150,  1000,  1141,   508,   508,   508,   423,   246,  1183,  1184,
     352,   357,   213,   733,   759,   713,   713,   713,   352,   375,
     259,   719,   690,   691,   692,  1162,   351,   351,   352,   139,
     202,   352,   139,   352,   701,   592,   181,   729,   730,   960,
     729,  1162,   375,   375,   684,  1162,   259,   649,   357,   352,
     213,   371,   371,   117,  1162,   828,   370,   764,   139,   750,
     372,   352,   357,   352,   111,   319,   375,   736,   737,   274,
     375,  1148,   111,   375,  1118,   370,  1118,   375,  1109,   960,
    1154,  1155,  1162,   370,   643,   642,   375,   646,   647,   229,
     352,  1012,  1170,   860,   960,   981,   982,   117,   375,   984,
     388,   395,   993,   209,   592,   352,   967,  1176,  1176,   352,
     357,   362,  1005,   362,  1007,   318,  1064,   362,   352,  1049,
    1049,   362,   357,   934,   934,   362,   288,  1031,   359,   752,
     352,   357,   356,   358,   370,   251,   295,   592,  1162,  1179,
    1179,   399,   810,   792,   828,   863,   792,   357,   206,   351,
     822,   636,  1179,  1179,  1172,  1172,  1178,   306,   375,   867,
     975,  1162,  1162,   352,   862,   184,   828,  1162,   897,   898,
    1162,  1162,   298,   299,   868,   869,   876,   117,   268,   935,
     173,   852,   916,   375,   917,   960,  1162,   290,   256,   933,
    1162,   352,   351,   375,   712,   929,   154,  1134,   111,  1137,
     233,   242,  1019,   508,   351,   695,  1162,   351,  1185,   352,
     719,   719,   719,   364,   288,   687,   352,   357,   319,   592,
     688,   689,   736,   274,   352,   352,   960,   701,   929,   213,
     697,   352,   357,   352,   167,   213,   248,   375,   372,  1162,
     207,  1162,   772,  1181,   960,   741,   747,   748,   749,   356,
     358,   370,   351,   828,  1162,   352,   737,   111,   375,  1148,
     829,   375,  1111,  1109,  1156,  1157,   652,   653,   934,   357,
     927,   278,   283,   370,   375,   985,   987,   352,   209,   117,
    1179,  1179,   793,   362,  1064,  1007,  1060,  1175,   913,   913,
     117,   362,  1162,   362,  1033,   370,   370,   352,   350,   352,
     828,   352,   795,   935,   139,   823,   824,   960,   934,  1176,
    1176,   351,   352,   359,   357,   868,   862,   352,   859,   352,
     357,   359,  1188,  1188,   351,   375,   877,   859,   935,   935,
     117,   174,   912,   917,   194,   914,   915,   925,   396,   357,
     351,   213,   716,   717,   182,   713,  1138,  1162,   265,  1140,
     729,   319,   213,  1186,  1187,   225,   760,   204,   206,   271,
     272,   319,   720,   721,   722,   692,  1162,   319,   352,   357,
     352,   111,   352,  1162,   693,   181,   730,   181,   185,  1162,
    1162,   370,   371,   122,   192,   201,   202,   216,   254,   760,
     749,   370,   370,   352,   213,   727,   728,   351,   375,  1148,
     375,  1111,  1118,  1158,  1159,   357,   913,   982,   985,   276,
     370,   276,   277,   117,   960,   977,   978,  1060,  1005,   852,
     852,   752,   359,   352,   352,   251,   265,   814,   815,   352,
     357,   919,  1179,  1179,   988,   859,   592,  1162,   372,   859,
     206,   859,   859,   898,  1162,   868,   375,   870,   871,   303,
     351,   384,   403,   410,   902,   905,   909,   910,   960,   935,
     933,   375,   669,   244,   672,   673,  1162,   352,   357,   375,
     720,   351,   352,  1162,  1162,   352,   357,   607,   370,  1162,
     722,  1162,   689,  1185,   351,  1182,   351,   351,   375,   248,
     359,   935,   254,   202,   185,   929,   352,   352,  1162,   352,
     357,   736,  1148,   375,   653,   852,   107,   375,   927,   112,
     142,   352,   357,  1005,   362,   912,   912,  1162,   201,   375,
     816,   815,   824,   352,   359,   352,   935,  1188,   859,   351,
     166,   357,   872,   351,   592,   878,   880,   351,   351,   351,
     357,   411,   911,  1162,   352,   357,   206,   717,   935,  1182,
     734,  1187,   755,   736,   697,   697,  1162,  1162,   352,   929,
     929,   713,   260,  1182,   728,   352,   912,   278,   375,   986,
     987,   352,   978,   362,   375,   375,   247,   592,  1162,   859,
     355,   960,   351,   592,   871,   177,   873,   879,   880,   166,
     903,   904,   960,   903,   351,   902,   906,   907,   908,   960,
     910,   248,   673,  1162,   352,   206,   761,   352,   693,   693,
     359,   713,   713,   719,   139,   187,   351,   277,   276,   277,
     179,   257,   817,   352,   352,   592,   351,   352,   352,   357,
     878,   352,   357,   352,   352,   352,   357,   213,   319,   718,
     140,   179,   257,  1185,   357,   357,  1162,   719,   719,  1182,
     246,   729,   820,   238,   859,   859,   352,   356,   358,   874,
     875,   894,   895,   880,   177,   881,   904,   907,   908,  1162,
    1162,   119,   228,   375,   762,   762,   762,   697,   697,   213,
     351,   352,   260,   808,   895,   895,   352,   357,   859,   351,
     352,   260,   375,   693,   693,  1162,   729,  1182,   351,   934,
     875,   351,   592,   882,   883,   884,   886,   139,   187,   351,
     352,   352,   352,   792,   919,   885,   886,   352,   357,   111,
     246,   729,  1182,   352,   140,   818,   352,   357,   883,   351,
     887,   889,   890,   891,   892,   893,   894,   351,   352,   934,
     934,   886,   888,   889,   347,   356,   358,   355,   360,   729,
     919,   352,   357,   892,   893,   893,   894,   894,   352,   889
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
#line 29 "ulpCompy.y"
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
#line 8699 "ulpCompy.cpp"

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
#line 593 "ulpCompy.y"
    {
        YYACCEPT;
    ;}
    break;

  case 84:

/* Line 1455 of yacc.c  */
#line 749 "ulpCompy.y"
    {
        /* BUG-28118 : system 헤더파일들도 파싱돼야함.            *
         * 2th. problem : 빈구조체 선언이 허용안됨. ex) struct A; */
        // <type> ; 이 올수 있다. ex> int; char; struct A; ...
        gUlpParseInfo.ulpInitHostInfo();
    ;}
    break;

  case 85:

/* Line 1455 of yacc.c  */
#line 756 "ulpCompy.y"
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
#line 829 "ulpCompy.y"
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
#line 974 "ulpCompy.y"
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
#line 1096 "ulpCompy.y"
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
#line 1146 "ulpCompy.y"
    {
        gUlpParseInfo.mHostValInfo[gUlpParseInfo.mStructDeclDepth]->mIsTypedef = ID_TRUE;
    ;}
    break;

  case 96:

/* Line 1455 of yacc.c  */
#line 1150 "ulpCompy.y"
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
#line 1164 "ulpCompy.y"
    {
        gUlpParseInfo.mSaveId = ID_TRUE;
        gUlpParseInfo.mHostValInfo[gUlpParseInfo.mStructDeclDepth]->mType = H_CHAR;
    ;}
    break;

  case 101:

/* Line 1455 of yacc.c  */
#line 1169 "ulpCompy.y"
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
#line 1184 "ulpCompy.y"
    {
        gUlpParseInfo.mSaveId = ID_TRUE;
        gUlpParseInfo.mHostValInfo[gUlpParseInfo.mStructDeclDepth]->mType = H_NCHAR;
    ;}
    break;

  case 103:

/* Line 1455 of yacc.c  */
#line 1189 "ulpCompy.y"
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
#line 1204 "ulpCompy.y"
    {
        gUlpParseInfo.mSaveId = ID_TRUE;
        gUlpParseInfo.mHostValInfo[gUlpParseInfo.mStructDeclDepth]->mType = H_SHORT;
    ;}
    break;

  case 105:

/* Line 1455 of yacc.c  */
#line 1209 "ulpCompy.y"
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
#line 1223 "ulpCompy.y"
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
#line 1236 "ulpCompy.y"
    {
        gUlpParseInfo.mHostValInfo[gUlpParseInfo.mStructDeclDepth]->mIssign = ID_TRUE;
    ;}
    break;

  case 108:

/* Line 1455 of yacc.c  */
#line 1240 "ulpCompy.y"
    {
        gUlpParseInfo.mHostValInfo[gUlpParseInfo.mStructDeclDepth]->mIssign = ID_FALSE;
    ;}
    break;

  case 109:

/* Line 1455 of yacc.c  */
#line 1244 "ulpCompy.y"
    {
        gUlpParseInfo.mSaveId = ID_TRUE;

        gUlpParseInfo.mHostValInfo[gUlpParseInfo.mStructDeclDepth]->mType = H_FLOAT;
    ;}
    break;

  case 110:

/* Line 1455 of yacc.c  */
#line 1250 "ulpCompy.y"
    {
        gUlpParseInfo.mSaveId = ID_TRUE;

        gUlpParseInfo.mHostValInfo[gUlpParseInfo.mStructDeclDepth]->mType = H_DOUBLE;
    ;}
    break;

  case 113:

/* Line 1455 of yacc.c  */
#line 1258 "ulpCompy.y"
    {
        gUlpParseInfo.mSaveId = ID_TRUE;

        gUlpParseInfo.mHostValInfo[gUlpParseInfo.mStructDeclDepth]->mType = H_UNKNOWN;
    ;}
    break;

  case 114:

/* Line 1455 of yacc.c  */
#line 1264 "ulpCompy.y"
    {
        gUlpParseInfo.mSaveId = ID_TRUE;

        gUlpParseInfo.mHostValInfo[gUlpParseInfo.mStructDeclDepth]->mType = H_USER_DEFINED;
        gUlpParseInfo.mHostValInfo[gUlpParseInfo.mStructDeclDepth]->mIsstruct = ID_TRUE;
    ;}
    break;

  case 115:

/* Line 1455 of yacc.c  */
#line 1271 "ulpCompy.y"
    {
        gUlpParseInfo.mSaveId = ID_TRUE;

        gUlpParseInfo.mHostValInfo[gUlpParseInfo.mStructDeclDepth]->mType = H_UNKNOWN;
    ;}
    break;

  case 116:

/* Line 1455 of yacc.c  */
#line 1277 "ulpCompy.y"
    {
        // In case of struct type or typedef type
        gUlpParseInfo.mSaveId = ID_TRUE;

        gUlpParseInfo.ulpCopyHostInfo4Typedef( gUlpParseInfo.mHostValInfo[gUlpParseInfo.mStructDeclDepth],
                                               gUlpParseInfo.mHostValInfo4Typedef );
    ;}
    break;

  case 117:

/* Line 1455 of yacc.c  */
#line 1285 "ulpCompy.y"
    {
        gUlpParseInfo.mSaveId = ID_TRUE;

        gUlpParseInfo.mHostValInfo[gUlpParseInfo.mStructDeclDepth]->mType = H_BINARY;
    ;}
    break;

  case 118:

/* Line 1455 of yacc.c  */
#line 1291 "ulpCompy.y"
    {
        gUlpParseInfo.mSaveId = ID_TRUE;

        gUlpParseInfo.mHostValInfo[gUlpParseInfo.mStructDeclDepth]->mType = H_BINARY2;
    ;}
    break;

  case 119:

/* Line 1455 of yacc.c  */
#line 1297 "ulpCompy.y"
    {
        gUlpParseInfo.mSaveId = ID_TRUE;

        gUlpParseInfo.mHostValInfo[gUlpParseInfo.mStructDeclDepth]->mType = H_BIT;
    ;}
    break;

  case 120:

/* Line 1455 of yacc.c  */
#line 1303 "ulpCompy.y"
    {
        gUlpParseInfo.mSaveId = ID_TRUE;

        gUlpParseInfo.mHostValInfo[gUlpParseInfo.mStructDeclDepth]->mType = H_BYTES;
    ;}
    break;

  case 121:

/* Line 1455 of yacc.c  */
#line 1309 "ulpCompy.y"
    {
        gUlpParseInfo.mSaveId = ID_TRUE;
        
        gUlpParseInfo.mHostValInfo[gUlpParseInfo.mStructDeclDepth]->mType = H_VARBYTE;
    ;}
    break;

  case 122:

/* Line 1455 of yacc.c  */
#line 1315 "ulpCompy.y"
    {
        gUlpParseInfo.mSaveId = ID_TRUE;

        gUlpParseInfo.mHostValInfo[gUlpParseInfo.mStructDeclDepth]->mType = H_NIBBLE;
    ;}
    break;

  case 123:

/* Line 1455 of yacc.c  */
#line 1321 "ulpCompy.y"
    {
        gUlpParseInfo.mSaveId = ID_TRUE;

        gUlpParseInfo.mHostValInfo[gUlpParseInfo.mStructDeclDepth]->mType = H_INTEGER;
    ;}
    break;

  case 124:

/* Line 1455 of yacc.c  */
#line 1327 "ulpCompy.y"
    {
        gUlpParseInfo.mSaveId = ID_TRUE;

        gUlpParseInfo.mHostValInfo[gUlpParseInfo.mStructDeclDepth]->mType = H_NUMERIC;
    ;}
    break;

  case 125:

/* Line 1455 of yacc.c  */
#line 1333 "ulpCompy.y"
    {
        gUlpParseInfo.mSaveId = ID_TRUE;

        gUlpParseInfo.mHostValInfo[gUlpParseInfo.mStructDeclDepth]->mType = H_BLOBLOCATOR;
    ;}
    break;

  case 126:

/* Line 1455 of yacc.c  */
#line 1339 "ulpCompy.y"
    {
        gUlpParseInfo.mSaveId = ID_TRUE;

        gUlpParseInfo.mHostValInfo[gUlpParseInfo.mStructDeclDepth]->mType = H_CLOBLOCATOR;
    ;}
    break;

  case 127:

/* Line 1455 of yacc.c  */
#line 1345 "ulpCompy.y"
    {
        gUlpParseInfo.mSaveId = ID_TRUE;

        gUlpParseInfo.mHostValInfo[gUlpParseInfo.mStructDeclDepth]->mType = H_BLOB;
    ;}
    break;

  case 128:

/* Line 1455 of yacc.c  */
#line 1351 "ulpCompy.y"
    {
        gUlpParseInfo.mSaveId = ID_TRUE;

        gUlpParseInfo.mHostValInfo[gUlpParseInfo.mStructDeclDepth]->mType = H_CLOB;
    ;}
    break;

  case 129:

/* Line 1455 of yacc.c  */
#line 1357 "ulpCompy.y"
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
#line 1372 "ulpCompy.y"
    {
        gUlpParseInfo.mSaveId = ID_TRUE;

        gUlpParseInfo.mHostValInfo[gUlpParseInfo.mStructDeclDepth]->mType = H_TIMESTAMP;
    ;}
    break;

  case 131:

/* Line 1455 of yacc.c  */
#line 1378 "ulpCompy.y"
    {
        gUlpParseInfo.mSaveId = ID_TRUE;

        gUlpParseInfo.mHostValInfo[gUlpParseInfo.mStructDeclDepth]->mType = H_TIME;
    ;}
    break;

  case 132:

/* Line 1455 of yacc.c  */
#line 1384 "ulpCompy.y"
    {
        gUlpParseInfo.mSaveId = ID_TRUE;

        gUlpParseInfo.mHostValInfo[gUlpParseInfo.mStructDeclDepth]->mType = H_DATE;
    ;}
    break;

  case 133:

/* Line 1455 of yacc.c  */
#line 1391 "ulpCompy.y"
    {
        gUlpParseInfo.mSaveId = ID_TRUE;

        gUlpParseInfo.mHostValInfo[gUlpParseInfo.mStructDeclDepth]->mType = H_NUMERIC_STRUCT;
    ;}
    break;

  case 134:

/* Line 1455 of yacc.c  */
#line 1398 "ulpCompy.y"
    {
        gUlpParseInfo.mSaveId = ID_TRUE;
        gUlpParseInfo.mHostValInfo[gUlpParseInfo.mStructDeclDepth]->mType = H_SQLDA;
    ;}
    break;

  case 135:

/* Line 1455 of yacc.c  */
#line 1403 "ulpCompy.y"
    {
        gUlpParseInfo.mSaveId = ID_TRUE;

        gUlpParseInfo.mHostValInfo[gUlpParseInfo.mStructDeclDepth]->mType = H_FAILOVERCB;
    ;}
    break;

  case 138:

/* Line 1455 of yacc.c  */
#line 1420 "ulpCompy.y"
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

  case 139:

/* Line 1455 of yacc.c  */
#line 1434 "ulpCompy.y"
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

  case 140:

/* Line 1455 of yacc.c  */
#line 1457 "ulpCompy.y"
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

  case 141:

/* Line 1455 of yacc.c  */
#line 1471 "ulpCompy.y"
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

  case 142:

/* Line 1455 of yacc.c  */
#line 1487 "ulpCompy.y"
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

  case 143:

/* Line 1455 of yacc.c  */
#line 1512 "ulpCompy.y"
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

  case 144:

/* Line 1455 of yacc.c  */
#line 1553 "ulpCompy.y"
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

  case 145:

/* Line 1455 of yacc.c  */
#line 1568 "ulpCompy.y"
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

  case 146:

/* Line 1455 of yacc.c  */
#line 1610 "ulpCompy.y"
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

  case 151:

/* Line 1455 of yacc.c  */
#line 1659 "ulpCompy.y"
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

  case 153:

/* Line 1455 of yacc.c  */
#line 1699 "ulpCompy.y"
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

  case 154:

/* Line 1455 of yacc.c  */
#line 1810 "ulpCompy.y"
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

  case 155:

/* Line 1455 of yacc.c  */
#line 1924 "ulpCompy.y"
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

  case 166:

/* Line 1455 of yacc.c  */
#line 1990 "ulpCompy.y"
    {
        gUlpParseInfo.mConstantExprStr[0] = '\0';
        gUlpParseInfo.mArrDepth = 0;
    ;}
    break;

  case 167:

/* Line 1455 of yacc.c  */
#line 1995 "ulpCompy.y"
    {
        gUlpParseInfo.mConstantExprStr[0] = '\0';
        gUlpParseInfo.mArrDepth = 0;
    ;}
    break;

  case 170:

/* Line 1455 of yacc.c  */
#line 2005 "ulpCompy.y"
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

  case 171:

/* Line 1455 of yacc.c  */
#line 2035 "ulpCompy.y"
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

  case 173:

/* Line 1455 of yacc.c  */
#line 2079 "ulpCompy.y"
    {
        gUlpCurDepth--;
    ;}
    break;

  case 175:

/* Line 1455 of yacc.c  */
#line 2087 "ulpCompy.y"
    {
        // array [ expr ] => expr 의 시작이라는 것을 알림. expr을 저장하기 위함.
        // 물론 expr 문법 체크도 함.
        gUlpParseInfo.mConstantExprStr[0] = '\0';
        gUlpParseInfo.mArrExpr = ID_TRUE;
    ;}
    break;

  case 176:

/* Line 1455 of yacc.c  */
#line 2097 "ulpCompy.y"
    {
        gUlpCurDepth++;
        gUlpParseInfo.mFuncDecl = ID_TRUE;
    ;}
    break;

  case 177:

/* Line 1455 of yacc.c  */
#line 2105 "ulpCompy.y"
    {
        gUlpParseInfo.mHostValInfo[gUlpParseInfo.mStructDeclDepth]->mPointer ++;
        gUlpParseInfo.mHostValInfo[gUlpParseInfo.mStructDeclDepth]->mAlloc = ID_TRUE;
    ;}
    break;

  case 178:

/* Line 1455 of yacc.c  */
#line 2110 "ulpCompy.y"
    {
        gUlpParseInfo.mHostValInfo[gUlpParseInfo.mStructDeclDepth]->mPointer ++;
        gUlpParseInfo.mHostValInfo[gUlpParseInfo.mStructDeclDepth]->mAlloc = ID_TRUE;
    ;}
    break;

  case 179:

/* Line 1455 of yacc.c  */
#line 2115 "ulpCompy.y"
    {
        gUlpParseInfo.mHostValInfo[gUlpParseInfo.mStructDeclDepth]->mPointer ++;
        gUlpParseInfo.mHostValInfo[gUlpParseInfo.mStructDeclDepth]->mAlloc = ID_TRUE;
    ;}
    break;

  case 180:

/* Line 1455 of yacc.c  */
#line 2120 "ulpCompy.y"
    {
        gUlpParseInfo.mHostValInfo[gUlpParseInfo.mStructDeclDepth]->mPointer ++;
        gUlpParseInfo.mHostValInfo[gUlpParseInfo.mStructDeclDepth]->mAlloc = ID_TRUE;
    ;}
    break;

  case 191:

/* Line 1455 of yacc.c  */
#line 2154 "ulpCompy.y"
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

  case 227:

/* Line 1455 of yacc.c  */
#line 2287 "ulpCompy.y"
    {
        /* BUG-29081 : 변수 선언부가 statement 중간에 들어오면 파싱 에러발생. */
        // statement 를 파싱한뒤 변수 type정보를 저장해두고 있는 자료구조를 초기화해줘야한다.
        // 저장 자체를 안하는게 이상적이나 type처리 문법을 선언부와 함께 공유하므로 어쩔수 없다.
        gUlpParseInfo.ulpInitHostInfo();
    ;}
    break;

  case 231:

/* Line 1455 of yacc.c  */
#line 2303 "ulpCompy.y"
    {
        if( gUlpParseInfo.mFuncDecl == ID_TRUE )
        {
            gUlpParseInfo.mFuncDecl = ID_FALSE;
        }
    ;}
    break;

  case 258:

/* Line 1455 of yacc.c  */
#line 2360 "ulpCompy.y"
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

  case 261:

/* Line 1455 of yacc.c  */
#line 2396 "ulpCompy.y"
    {
            /* BUG-28061 : preprocessing을마치면 marco table을 초기화하고, *
             *             ulpComp 에서 재구축한다.                       */
            gUlpCOMPStartCond = gUlpCOMPPrevCond;
        ;}
    break;

  case 262:

/* Line 1455 of yacc.c  */
#line 2402 "ulpCompy.y"
    {
            gUlpCOMPStartCond = gUlpCOMPPrevCond;
        ;}
    break;

  case 263:

/* Line 1455 of yacc.c  */
#line 2406 "ulpCompy.y"
    {
            gUlpCOMPStartCond = gUlpCOMPPrevCond;
        ;}
    break;

  case 264:

/* Line 1455 of yacc.c  */
#line 2410 "ulpCompy.y"
    {
            /* macro 조건문의 경우 참이면 C상태, 거짓이면 MACRO_IFSKIP 상태로
             * 전이 된다. */
        ;}
    break;

  case 265:

/* Line 1455 of yacc.c  */
#line 2415 "ulpCompy.y"
    {
            /* macro 조건문의 경우 참이면 C상태, 거짓이면 MACRO_IFSKIP 상태로
             * 전이 된다. */
        ;}
    break;

  case 266:

/* Line 1455 of yacc.c  */
#line 2420 "ulpCompy.y"
    {
            /* macro 조건문의 경우 참이면 C상태, 거짓이면 MACRO_IFSKIP 상태로
             * 전이 된다. */
        ;}
    break;

  case 267:

/* Line 1455 of yacc.c  */
#line 2425 "ulpCompy.y"
    {
            /* macro 조건문의 경우 참이면 C상태, 거짓이면 MACRO_IFSKIP 상태로
             * 전이 된다. */
        ;}
    break;

  case 268:

/* Line 1455 of yacc.c  */
#line 2430 "ulpCompy.y"
    {
            /* macro 조건문의 경우 참이면 C상태, 거짓이면 MACRO_IFSKIP 상태로
             * 전이 된다. */
        ;}
    break;

  case 270:

/* Line 1455 of yacc.c  */
#line 2439 "ulpCompy.y"
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

  case 271:

/* Line 1455 of yacc.c  */
#line 2467 "ulpCompy.y"
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

  case 272:

/* Line 1455 of yacc.c  */
#line 2499 "ulpCompy.y"
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

  case 273:

/* Line 1455 of yacc.c  */
#line 2532 "ulpCompy.y"
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

  case 274:

/* Line 1455 of yacc.c  */
#line 2570 "ulpCompy.y"
    {
            // $<strval>2 를 macro symbol table에서 삭제 한다.
            gUlpMacroT.ulpMUndef( (yyvsp[(2) - (2)].strval) );
        ;}
    break;

  case 275:

/* Line 1455 of yacc.c  */
#line 2578 "ulpCompy.y"
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

  case 276:

/* Line 1455 of yacc.c  */
#line 2644 "ulpCompy.y"
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

  case 277:

/* Line 1455 of yacc.c  */
#line 2720 "ulpCompy.y"
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

  case 278:

/* Line 1455 of yacc.c  */
#line 2761 "ulpCompy.y"
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

  case 279:

/* Line 1455 of yacc.c  */
#line 2802 "ulpCompy.y"
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

  case 280:

/* Line 1455 of yacc.c  */
#line 2838 "ulpCompy.y"
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

  case 281:

/* Line 1455 of yacc.c  */
#line 2876 "ulpCompy.y"
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

  case 282:

/* Line 1455 of yacc.c  */
#line 2895 "ulpCompy.y"
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
#line 2910 "ulpCompy.y"
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
#line 2925 "ulpCompy.y"
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
        /* BUG-46824 추가된 변수 초기화 */
        gUlpProcObjCount = 0;
        gUlpPSMObjName = NULL;
    ;}
    break;

  case 285:

/* Line 1455 of yacc.c  */
#line 2943 "ulpCompy.y"
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

  case 286:

/* Line 1455 of yacc.c  */
#line 2957 "ulpCompy.y"
    {
        gUlpCodeGen.ulpGenInitQBuff();
        gUlpCodeGen.ulpClearEmSQLInfo();
        gUlpCOMPStartCond = gUlpCOMPPrevCond;
    ;}
    break;

  case 287:

/* Line 1455 of yacc.c  */
#line 2963 "ulpCompy.y"
    {
        // whenever 구문을 comment로 쓴다.
        gUlpCodeGen.ulpGenComment( gUlpCodeGen.ulpGetQueryBuf() );

        gUlpCodeGen.ulpGenInitQBuff();
        gUlpCodeGen.ulpClearEmSQLInfo();
        gUlpCOMPStartCond = gUlpCOMPPrevCond;
    ;}
    break;

  case 288:

/* Line 1455 of yacc.c  */
#line 2972 "ulpCompy.y"
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

  case 289:

/* Line 1455 of yacc.c  */
#line 2986 "ulpCompy.y"
    {
        gUlpCodeGen.ulpGenComment( gUlpCodeGen.ulpGetQueryBuf() );
        gUlpCodeGen.ulpGenInitQBuff();
        gUlpCodeGen.ulpClearEmSQLInfo();
        gUlpIsPrintStmt = ID_TRUE;
        gUlpStmttype    = S_UNKNOWN;
    ;}
    break;

  case 290:

/* Line 1455 of yacc.c  */
#line 2995 "ulpCompy.y"
    {
        /* TASK-7218 Handling Multiple Errors */
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

  case 291:

/* Line 1455 of yacc.c  */
#line 3012 "ulpCompy.y"
    {
        // do nothing
    ;}
    break;

  case 292:

/* Line 1455 of yacc.c  */
#line 3016 "ulpCompy.y"
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

  case 293:

/* Line 1455 of yacc.c  */
#line 3036 "ulpCompy.y"
    {
        gUlpCodeGen.ulpGenEmSQL( GEN_CONNNAME, (void *) (yyvsp[(2) - (2)].strval) );
        gUlpCodeGen.ulpGenRemoveQueryToken( (yyvsp[(2) - (2)].strval) );
        gUlpCodeGen.ulpGenRemoveQueryToken( (yyvsp[(1) - (2)].strval) );
    ;}
    break;

  case 294:

/* Line 1455 of yacc.c  */
#line 3054 "ulpCompy.y"
    {
        gUlpStmttype = S_DeclareCur;
    ;}
    break;

  case 295:

/* Line 1455 of yacc.c  */
#line 3058 "ulpCompy.y"
    {
        gUlpStmttype = S_DeclareStmt;
        gUlpIsPrintStmt = ID_FALSE;
    ;}
    break;

  case 296:

/* Line 1455 of yacc.c  */
#line 3063 "ulpCompy.y"
    {
        gUlpStmttype = S_Open;
        gUlpIsPrintStmt = ID_FALSE;
    ;}
    break;

  case 297:

/* Line 1455 of yacc.c  */
#line 3068 "ulpCompy.y"
    {
        gUlpStmttype = S_Fetch;
        gUlpIsPrintStmt = ID_FALSE;
    ;}
    break;

  case 298:

/* Line 1455 of yacc.c  */
#line 3073 "ulpCompy.y"
    {
        gUlpIsPrintStmt = ID_FALSE;
    ;}
    break;

  case 299:

/* Line 1455 of yacc.c  */
#line 3077 "ulpCompy.y"
    {
        gUlpStmttype = S_AutoCommit;
        gUlpIsPrintStmt = ID_FALSE;
    ;}
    break;

  case 300:

/* Line 1455 of yacc.c  */
#line 3082 "ulpCompy.y"
    {
        gUlpStmttype = S_Connect;
        gUlpIsPrintStmt = ID_FALSE;
    ;}
    break;

  case 301:

/* Line 1455 of yacc.c  */
#line 3087 "ulpCompy.y"
    {
        gUlpStmttype = S_Disconnect;
        gUlpIsPrintStmt = ID_FALSE;
    ;}
    break;

  case 302:

/* Line 1455 of yacc.c  */
#line 3092 "ulpCompy.y"
    {
        gUlpStmttype = S_FreeLob;
        gUlpIsPrintStmt = ID_FALSE;
    ;}
    break;

  case 303:

/* Line 1455 of yacc.c  */
#line 3110 "ulpCompy.y"
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

  case 304:

/* Line 1455 of yacc.c  */
#line 3123 "ulpCompy.y"
    {
        gUlpCodeGen.ulpGenEmSQL( GEN_STMTNAME, (void *) (yyvsp[(2) - (3)].strval) );
        gUlpIsPrintStmt = ID_FALSE;
    ;}
    break;

  case 305:

/* Line 1455 of yacc.c  */
#line 3132 "ulpCompy.y"
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

  case 306:

/* Line 1455 of yacc.c  */
#line 3150 "ulpCompy.y"
    {
        UInt sValue = SQL_INSENSITIVE;
        gUlpCodeGen.ulpGenEmSQL( GEN_CURSORSENSITIVITY, (void *)&sValue );
  ;}
    break;

  case 307:

/* Line 1455 of yacc.c  */
#line 3155 "ulpCompy.y"
    {
        UInt sValue = SQL_SENSITIVE;
        gUlpCodeGen.ulpGenEmSQL( GEN_CURSORSENSITIVITY, (void *)&sValue );
  ;}
    break;

  case 308:

/* Line 1455 of yacc.c  */
#line 3160 "ulpCompy.y"
    {
        UInt sValue = SQL_INSENSITIVE;
        gUlpCodeGen.ulpGenEmSQL( GEN_CURSORSENSITIVITY, (void *)&sValue );
  ;}
    break;

  case 309:

/* Line 1455 of yacc.c  */
#line 3165 "ulpCompy.y"
    {
        UInt sValue = SQL_INSENSITIVE;
        gUlpCodeGen.ulpGenEmSQL( GEN_CURSORSENSITIVITY, (void *)&sValue );
  ;}
    break;

  case 310:

/* Line 1455 of yacc.c  */
#line 3174 "ulpCompy.y"
    {
        UInt sValue = SQL_NONSCROLLABLE;
        gUlpCodeGen.ulpGenEmSQL( GEN_CURSORSCROLLABLE, (void *) &sValue );
;}
    break;

  case 311:

/* Line 1455 of yacc.c  */
#line 3179 "ulpCompy.y"
    {
        UInt sValue = SQL_SCROLLABLE;
        gUlpCodeGen.ulpGenEmSQL( GEN_CURSORSCROLLABLE, (void *) &sValue );
        gUlpCodeGen.ulpGenEmSQL( GEN_SCROLLCUR, (void *) "1" );
    ;}
    break;

  case 312:

/* Line 1455 of yacc.c  */
#line 3189 "ulpCompy.y"
    {
      UInt sValue = SQL_CURSOR_HOLD_OFF;
      gUlpCodeGen.ulpGenEmSQL( GEN_CURSORWITHHOLD, (void *) &sValue );
  ;}
    break;

  case 313:

/* Line 1455 of yacc.c  */
#line 3194 "ulpCompy.y"
    {
      UInt sValue = SQL_CURSOR_HOLD_ON;
      gUlpCodeGen.ulpGenEmSQL( GEN_CURSORWITHHOLD, (void *) &sValue );
  ;}
    break;

  case 314:

/* Line 1455 of yacc.c  */
#line 3199 "ulpCompy.y"
    {
      ulpSetErrorCode( &gUlpParseInfo.mErrorMgr,
                       ulpERR_ABORT_COMP_Not_Supported_WITH_RETURN_Error );
      gUlpCOMPErrCode = ulpGetErrorSTATE( &gUlpParseInfo.mErrorMgr );
      COMPerror( ulpGetErrorMSG(&gUlpParseInfo.mErrorMgr) );
  ;}
    break;

  case 315:

/* Line 1455 of yacc.c  */
#line 3206 "ulpCompy.y"
    {
      ulpSetErrorCode( &gUlpParseInfo.mErrorMgr,
                       ulpERR_ABORT_COMP_Not_Supported_WITH_RETURN_Error );
      gUlpCOMPErrCode = ulpGetErrorSTATE( &gUlpParseInfo.mErrorMgr );
      COMPerror( ulpGetErrorMSG(&gUlpParseInfo.mErrorMgr) );
  ;}
    break;

  case 317:

/* Line 1455 of yacc.c  */
#line 3217 "ulpCompy.y"
    {

      ulpSetErrorCode( &gUlpParseInfo.mErrorMgr,
                       ulpERR_ABORT_COMP_Not_Supported_READ_ONLY_Error );
      gUlpCOMPErrCode = ulpGetErrorSTATE( &gUlpParseInfo.mErrorMgr );
      COMPerror( ulpGetErrorMSG(&gUlpParseInfo.mErrorMgr) );
  ;}
    break;

  case 325:

/* Line 1455 of yacc.c  */
#line 3245 "ulpCompy.y"
    {
        gUlpCodeGen.ulpGenEmSQL( GEN_STMTNAME, (void *) (yyvsp[(2) - (3)].strval) );
    ;}
    break;

  case 326:

/* Line 1455 of yacc.c  */
#line 3256 "ulpCompy.y"
    {
        gUlpCodeGen.ulpGenEmSQL( GEN_CURNAME, (void *) (yyvsp[(2) - (2)].strval) );
    ;}
    break;

  case 327:

/* Line 1455 of yacc.c  */
#line 3260 "ulpCompy.y"
    {
        gUlpCodeGen.ulpGenEmSQL( GEN_CURNAME, (void *) (yyvsp[(2) - (4)].strval) );
    ;}
    break;

  case 328:

/* Line 1455 of yacc.c  */
#line 3265 "ulpCompy.y"
    {
        gUlpCodeGen.ulpGenEmSQL( GEN_CURNAME, (void *) (yyvsp[(2) - (4)].strval) );
    ;}
    break;

  case 329:

/* Line 1455 of yacc.c  */
#line 3276 "ulpCompy.y"
    {
        gUlpCodeGen.ulpGenEmSQL( GEN_CURNAME, (void *) (yyvsp[(3) - (5)].strval) );
    ;}
    break;

  case 330:

/* Line 1455 of yacc.c  */
#line 3281 "ulpCompy.y"
    {
        gUlpCodeGen.ulpGenEmSQL( GEN_CURNAME, (void *) (yyvsp[(4) - (6)].strval) );
    ;}
    break;

  case 331:

/* Line 1455 of yacc.c  */
#line 3286 "ulpCompy.y"
    {
        gUlpCodeGen.ulpGenEmSQL( GEN_CURNAME, (void *) (yyvsp[(3) - (6)].strval) );
    ;}
    break;

  case 336:

/* Line 1455 of yacc.c  */
#line 3300 "ulpCompy.y"
    {
        gUlpCodeGen.ulpGenEmSQL( GEN_SCROLLCUR, (void *) "1" /*F_FIRST*/ );
    ;}
    break;

  case 337:

/* Line 1455 of yacc.c  */
#line 3304 "ulpCompy.y"
    {
        gUlpCodeGen.ulpGenEmSQL( GEN_SCROLLCUR, (void *) "2" /*F_PRIOR*/ );
    ;}
    break;

  case 338:

/* Line 1455 of yacc.c  */
#line 3308 "ulpCompy.y"
    {
        gUlpCodeGen.ulpGenEmSQL( GEN_SCROLLCUR, (void *) "3" /*F_NEXT*/ );
    ;}
    break;

  case 339:

/* Line 1455 of yacc.c  */
#line 3312 "ulpCompy.y"
    {
        gUlpCodeGen.ulpGenEmSQL( GEN_SCROLLCUR, (void *) "4" /*F_LAST*/ );
    ;}
    break;

  case 340:

/* Line 1455 of yacc.c  */
#line 3316 "ulpCompy.y"
    {
        gUlpCodeGen.ulpGenEmSQL( GEN_SCROLLCUR, (void *) "5" /*F_CURRENT*/ );
    ;}
    break;

  case 341:

/* Line 1455 of yacc.c  */
#line 3320 "ulpCompy.y"
    {
        gUlpCodeGen.ulpGenEmSQL( GEN_SCROLLCUR, (void *) "6" /*F_RELATIVE*/ );
    ;}
    break;

  case 342:

/* Line 1455 of yacc.c  */
#line 3324 "ulpCompy.y"
    {
        gUlpCodeGen.ulpGenEmSQL( GEN_SCROLLCUR, (void *) "7" /*F_ABSOLUTE*/ );
    ;}
    break;

  case 343:

/* Line 1455 of yacc.c  */
#line 3331 "ulpCompy.y"
    {
        gUlpCodeGen.ulpGenEmSQL( GEN_SQLINFO, (void *) (yyvsp[(1) - (1)].strval) );
    ;}
    break;

  case 344:

/* Line 1455 of yacc.c  */
#line 3335 "ulpCompy.y"
    {
        gUlpCodeGen.ulpGenEmSQL( GEN_SQLINFO, (void *) (yyvsp[(2) - (2)].strval) );
    ;}
    break;

  case 345:

/* Line 1455 of yacc.c  */
#line 3339 "ulpCompy.y"
    {
        SChar sTmpStr[MAX_NUMBER_LEN];
        idlOS::snprintf( sTmpStr, MAX_NUMBER_LEN ,"-%s", (yyvsp[(2) - (2)].strval) );
        gUlpCodeGen.ulpGenEmSQL( GEN_SQLINFO, (void *) sTmpStr );
    ;}
    break;

  case 346:

/* Line 1455 of yacc.c  */
#line 3353 "ulpCompy.y"
    {
        gUlpStmttype = S_CloseRel;
        gUlpCodeGen.ulpGenEmSQL( GEN_CURNAME, (void *) (yyvsp[(3) - (3)].strval) );
    ;}
    break;

  case 347:

/* Line 1455 of yacc.c  */
#line 3358 "ulpCompy.y"
    {
        gUlpStmttype = S_Close;
        gUlpCodeGen.ulpGenEmSQL( GEN_CURNAME, (void *) (yyvsp[(2) - (2)].strval) );
    ;}
    break;

  case 348:

/* Line 1455 of yacc.c  */
#line 3370 "ulpCompy.y"
    {
        gUlpCodeGen.ulpGenEmSQL( GEN_SQLINFO, (void *) "1" /*ON*/);
    ;}
    break;

  case 349:

/* Line 1455 of yacc.c  */
#line 3374 "ulpCompy.y"
    {
        gUlpCodeGen.ulpGenEmSQL( GEN_SQLINFO, (void *) "0" /*OFF*/);
    ;}
    break;

  case 350:

/* Line 1455 of yacc.c  */
#line 3385 "ulpCompy.y"
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

  case 351:

/* Line 1455 of yacc.c  */
#line 3409 "ulpCompy.y"
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

  case 352:

/* Line 1455 of yacc.c  */
#line 3443 "ulpCompy.y"
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

  case 353:

/* Line 1455 of yacc.c  */
#line 3478 "ulpCompy.y"
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

  case 354:

/* Line 1455 of yacc.c  */
#line 3520 "ulpCompy.y"
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

  case 355:

/* Line 1455 of yacc.c  */
#line 3570 "ulpCompy.y"
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

  case 357:

/* Line 1455 of yacc.c  */
#line 3627 "ulpCompy.y"
    {

    ;}
    break;

  case 358:

/* Line 1455 of yacc.c  */
#line 3631 "ulpCompy.y"
    {

    ;}
    break;

  case 359:

/* Line 1455 of yacc.c  */
#line 3645 "ulpCompy.y"
    {
        gUlpStmttype    = S_IGNORE;
        gUlpIsPrintStmt = ID_FALSE;
    ;}
    break;

  case 360:

/* Line 1455 of yacc.c  */
#line 3650 "ulpCompy.y"
    {
        gUlpStmttype    = S_IGNORE;
        gUlpIsPrintStmt = ID_FALSE;
    ;}
    break;

  case 361:

/* Line 1455 of yacc.c  */
#line 3655 "ulpCompy.y"
    {
        gUlpStmttype = S_Prepare;
        gUlpIsPrintStmt = ID_TRUE;
    ;}
    break;

  case 362:

/* Line 1455 of yacc.c  */
#line 3660 "ulpCompy.y"
    {
        gUlpStmttype    = S_IGNORE;
        gUlpIsPrintStmt = ID_FALSE;
    ;}
    break;

  case 363:

/* Line 1455 of yacc.c  */
#line 3665 "ulpCompy.y"
    {
        gUlpStmttype    = S_ExecIm;
        gUlpIsPrintStmt = ID_TRUE;
    ;}
    break;

  case 364:

/* Line 1455 of yacc.c  */
#line 3670 "ulpCompy.y"
    {
        gUlpStmttype    = S_ExecDML;
        gUlpIsPrintStmt = ID_FALSE;
    ;}
    break;

  case 365:

/* Line 1455 of yacc.c  */
#line 3676 "ulpCompy.y"
    {
        gUlpStmttype    = S_BindVariables;
        gUlpIsPrintStmt = ID_FALSE;
    ;}
    break;

  case 366:

/* Line 1455 of yacc.c  */
#line 3681 "ulpCompy.y"
    {
        gUlpStmttype    = S_SetArraySize;
        gUlpIsPrintStmt = ID_FALSE;
    ;}
    break;

  case 367:

/* Line 1455 of yacc.c  */
#line 3686 "ulpCompy.y"
    {
        gUlpStmttype    = S_SelectList;
        gUlpIsPrintStmt = ID_FALSE;
    ;}
    break;

  case 371:

/* Line 1455 of yacc.c  */
#line 3700 "ulpCompy.y"
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

  case 374:

/* Line 1455 of yacc.c  */
#line 3720 "ulpCompy.y"
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

  case 375:

/* Line 1455 of yacc.c  */
#line 3750 "ulpCompy.y"
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

  case 376:

/* Line 1455 of yacc.c  */
#line 3762 "ulpCompy.y"
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

  case 377:

/* Line 1455 of yacc.c  */
#line 3776 "ulpCompy.y"
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

  case 380:

/* Line 1455 of yacc.c  */
#line 3815 "ulpCompy.y"
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

  case 381:

/* Line 1455 of yacc.c  */
#line 3845 "ulpCompy.y"
    {
        gUlpCodeGen.ulpGenEmSQL( GEN_QUERYSTR,
                                 idlOS::strstr( gUlpCodeGen.ulpGetQueryBuf(),
                                                (yyvsp[(2) - (2)].strval))
                               );
    ;}
    break;

  case 382:

/* Line 1455 of yacc.c  */
#line 3852 "ulpCompy.y"
    {
        gUlpCodeGen.ulpGenEmSQL( GEN_QUERYSTR,
                                 idlOS::strstr( gUlpCodeGen.ulpGetQueryBuf(),
                                                (yyvsp[(2) - (2)].strval))
                               );
    ;}
    break;

  case 384:

/* Line 1455 of yacc.c  */
#line 3866 "ulpCompy.y"
    {
      gUlpCodeGen.ulpGenEmSQL( GEN_STMTNAME, (void *) (yyvsp[(2) - (2)].strval) );
  ;}
    break;

  case 385:

/* Line 1455 of yacc.c  */
#line 3870 "ulpCompy.y"
    {
      gUlpCodeGen.ulpGenEmSQL( GEN_STMTNAME, (void *) (yyvsp[(3) - (5)].strval) );
  ;}
    break;

  case 386:

/* Line 1455 of yacc.c  */
#line 3874 "ulpCompy.y"
    {
      gUlpCodeGen.ulpGenEmSQL( GEN_STMTNAME, (void *) (yyvsp[(2) - (4)].strval) );
  ;}
    break;

  case 387:

/* Line 1455 of yacc.c  */
#line 3878 "ulpCompy.y"
    {
      gUlpCodeGen.ulpGenEmSQL( GEN_STMTNAME, (void *) (yyvsp[(2) - (4)].strval) );
  ;}
    break;

  case 388:

/* Line 1455 of yacc.c  */
#line 3886 "ulpCompy.y"
    {
      gUlpCodeGen.ulpGenEmSQL( GEN_STMTNAME, (void *) (yyvsp[(5) - (7)].strval) );
  ;}
    break;

  case 389:

/* Line 1455 of yacc.c  */
#line 3893 "ulpCompy.y"
    {
      gUlpCodeGen.ulpGenEmSQL( GEN_STMTNAME, (void *) (yyvsp[(2) - (3)].strval) );
  ;}
    break;

  case 390:

/* Line 1455 of yacc.c  */
#line 3901 "ulpCompy.y"
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

  case 391:

/* Line 1455 of yacc.c  */
#line 3929 "ulpCompy.y"
    {
        gUlpStmttype    = S_DirectOTH;
        gUlpIsPrintStmt = ID_TRUE;
        gUlpCodeGen.ulpGenCutQueryTail4PSM( ';' );

    ;}
    break;

  case 392:

/* Line 1455 of yacc.c  */
#line 3936 "ulpCompy.y"
    {
        gUlpStmttype    = S_DirectOTH;
        gUlpIsPrintStmt = ID_TRUE;
        gUlpCodeGen.ulpGenCutQueryTail4PSM( ';' );
    ;}
    break;

  case 393:

/* Line 1455 of yacc.c  */
#line 3942 "ulpCompy.y"
    {
        gUlpStmttype    = S_DirectOTH;
        gUlpIsPrintStmt = ID_TRUE;
        gUlpCodeGen.ulpGenCutQueryTail4PSM( ';' );
    ;}
    break;

  case 394:

/* Line 1455 of yacc.c  */
#line 3948 "ulpCompy.y"
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
#line 3962 "ulpCompy.y"
    {
        idBool sTrue;
        gUlpIsPrintStmt = ID_TRUE;
        
        if ((gUlpProcObjCount == 1) && (gUlpPSMObjName != NULL))
        {
            /* BUG-46824 procedure 
             * BEGIN END안에 object_name이 한개이므로 procedure로 간주한다. 
             * 예)
             * BEGIN
             *     PROC1;
             * END; 
             */
            sTrue = ID_TRUE;
            gUlpStmttype = S_DirectPSM;
            /* BUG-47868 object_name 이 한개일때만 ulpGenCutStringTail4PSM 호출 */
            gUlpCodeGen.ulpGenCutStringTail4PSM( gUlpPSMObjName, ';' );
            gUlpCodeGen.ulpGenEmSQL( GEN_QUERYSTR, gUlpPSMObjName);
        }
        else
        {
            /* BUG-46824 anonymous block */
            sTrue = ID_FALSE;
            gUlpStmttype = S_DirectANONPSM;
            gUlpCodeGen.ulpGenEmSQL( GEN_QUERYSTR,
                                     idlOS::strstr( gUlpCodeGen.ulpGetQueryBuf(),
                                                    (yyvsp[(2) - (3)].strval))
                                   );
        }
        gUlpCodeGen.ulpGenCutQueryTail4PSM( ';' );
        
        gUlpCodeGen.ulpGenEmSQL( GEN_PSMEXEC, (void *) &sTrue );
    ;}
    break;

  case 396:

/* Line 1455 of yacc.c  */
#line 4005 "ulpCompy.y"
    {
        gUlpStmttype    = S_Free;
        gUlpIsPrintStmt = ID_FALSE;
    ;}
    break;

  case 397:

/* Line 1455 of yacc.c  */
#line 4010 "ulpCompy.y"
    {
        gUlpStmttype    = S_Batch;
        gUlpIsPrintStmt = ID_FALSE;
    ;}
    break;

  case 398:

/* Line 1455 of yacc.c  */
#line 4015 "ulpCompy.y"
    {
        gUlpStmttype    = S_Batch;
        gUlpIsPrintStmt = ID_FALSE;
    ;}
    break;

  case 399:

/* Line 1455 of yacc.c  */
#line 4021 "ulpCompy.y"
    {
        gUlpStmttype = S_AutoCommit;
        gUlpIsPrintStmt = ID_FALSE;
        gUlpCodeGen.ulpGenEmSQL( GEN_SQLINFO, (void *) "1" /*ON*/);
    ;}
    break;

  case 400:

/* Line 1455 of yacc.c  */
#line 4028 "ulpCompy.y"
    {
        gUlpStmttype = S_AutoCommit;
        gUlpIsPrintStmt = ID_FALSE;
        gUlpCodeGen.ulpGenEmSQL( GEN_SQLINFO, (void *) "0" /*OFF*/);
    ;}
    break;

  case 401:

/* Line 1455 of yacc.c  */
#line 4035 "ulpCompy.y"
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

  case 402:

/* Line 1455 of yacc.c  */
#line 4052 "ulpCompy.y"
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

  case 403:

/* Line 1455 of yacc.c  */
#line 4073 "ulpCompy.y"
    {
        gUlpCodeGen.ulpGenEmSQL( GEN_SQLINFO, (void *) "0" /*UNSET*/);
        gUlpStmttype = S_FailOver;
        gUlpIsPrintStmt = ID_FALSE;
    ;}
    break;

  case 404:

/* Line 1455 of yacc.c  */
#line 4089 "ulpCompy.y"
    {
        idBool sTrue = ID_TRUE;
        gUlpCodeGen.ulpGenComment( gUlpCodeGen.ulpGetQueryBuf() );
        gUlpCodeGen.ulpGenEmSQL( GEN_MT, (void *)&sTrue );
    ;}
    break;

  case 405:

/* Line 1455 of yacc.c  */
#line 4095 "ulpCompy.y"
    {
        idBool sFalse = ID_FALSE;
        gUlpCodeGen.ulpGenComment( gUlpCodeGen.ulpGetQueryBuf() );
        gUlpCodeGen.ulpGenEmSQL( GEN_MT, (void *)&sFalse );
  ;}
    break;

  case 406:

/* Line 1455 of yacc.c  */
#line 4111 "ulpCompy.y"
    {
        gUlpCodeGen.ulpGenSetWhenever( gUlpCurDepth,
                                       GEN_WHEN_SQLERROR,
                                       GEN_WHEN_CONT,
                                       NULL );
    ;}
    break;

  case 407:

/* Line 1455 of yacc.c  */
#line 4118 "ulpCompy.y"
    {
        gUlpCodeGen.ulpGenSetWhenever( gUlpCurDepth,
                                       GEN_WHEN_SQLERROR,
                                       GEN_WHEN_GOTO,
                                       (yyvsp[(4) - (4)].strval) );
    ;}
    break;

  case 408:

/* Line 1455 of yacc.c  */
#line 4125 "ulpCompy.y"
    {
        gUlpCodeGen.ulpGenSetWhenever( gUlpCurDepth,
                                       GEN_WHEN_SQLERROR,
                                       GEN_WHEN_DO_BREAK,
                                       NULL );
    ;}
    break;

  case 409:

/* Line 1455 of yacc.c  */
#line 4132 "ulpCompy.y"
    {
        gUlpCodeGen.ulpGenSetWhenever( gUlpCurDepth,
                                       GEN_WHEN_SQLERROR,
                                       GEN_WHEN_DO_CONT,
                                       NULL );
    ;}
    break;

  case 410:

/* Line 1455 of yacc.c  */
#line 4139 "ulpCompy.y"
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

  case 411:

/* Line 1455 of yacc.c  */
#line 4150 "ulpCompy.y"
    {
        gUlpCodeGen.ulpGenSetWhenever( gUlpCurDepth,
                                       GEN_WHEN_SQLERROR,
                                       GEN_WHEN_STOP,
                                       NULL );
    ;}
    break;

  case 412:

/* Line 1455 of yacc.c  */
#line 4157 "ulpCompy.y"
    {
        gUlpCodeGen.ulpGenSetWhenever( gUlpCurDepth,
                                       GEN_WHEN_NOTFOUND,
                                       GEN_WHEN_CONT,
                                       NULL );
    ;}
    break;

  case 413:

/* Line 1455 of yacc.c  */
#line 4164 "ulpCompy.y"
    {
        gUlpCodeGen.ulpGenSetWhenever( gUlpCurDepth,
                                       GEN_WHEN_NOTFOUND,
                                       GEN_WHEN_GOTO,
                                       (yyvsp[(5) - (5)].strval) );
    ;}
    break;

  case 414:

/* Line 1455 of yacc.c  */
#line 4171 "ulpCompy.y"
    {
        gUlpCodeGen.ulpGenSetWhenever( gUlpCurDepth,
                                       GEN_WHEN_NOTFOUND,
                                       GEN_WHEN_DO_BREAK,
                                       NULL );
    ;}
    break;

  case 415:

/* Line 1455 of yacc.c  */
#line 4178 "ulpCompy.y"
    {
        gUlpCodeGen.ulpGenSetWhenever( gUlpCurDepth, 
                                       GEN_WHEN_NOTFOUND,
                                       GEN_WHEN_DO_CONT,
                                       NULL );
    ;}
    break;

  case 416:

/* Line 1455 of yacc.c  */
#line 4185 "ulpCompy.y"
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

  case 417:

/* Line 1455 of yacc.c  */
#line 4196 "ulpCompy.y"
    {
        gUlpCodeGen.ulpGenSetWhenever( gUlpCurDepth,
                                       GEN_WHEN_NOTFOUND,
                                       GEN_WHEN_STOP,
                                       NULL );
    ;}
    break;

  case 419:

/* Line 1455 of yacc.c  */
#line 4208 "ulpCompy.y"
    {
        gUlpStmttype    = S_IGNORE;
        gUlpIsPrintStmt = ID_FALSE;
    ;}
    break;

  case 420:

/* Line 1455 of yacc.c  */
#line 4215 "ulpCompy.y"
    {
        gUlpSharedPtrPrevCond  = gUlpCOMPPrevCond;
        gUlpCOMPStartCond = CP_ST_C;
        idlOS::strcpy ( gUlpCodeGen.ulpGetSharedPtrName(), (yyvsp[(5) - (6)].strval) );
        gUlpParseInfo.mIsSharedPtr = ID_TRUE;

    ;}
    break;

  case 421:

/* Line 1455 of yacc.c  */
#line 4223 "ulpCompy.y"
    {
        gUlpCOMPStartCond = gUlpSharedPtrPrevCond;
        gUlpParseInfo.mIsSharedPtr = ID_FALSE;
        gUlpCodeGen.ulpClearSharedPtrInfo();
    ;}
    break;

  case 422:

/* Line 1455 of yacc.c  */
#line 4232 "ulpCompy.y"
    {
        gUlpStmttype = S_GetStmtDiag;
        gUlpIsPrintStmt = ID_FALSE;
    ;}
    break;

  case 423:

/* Line 1455 of yacc.c  */
#line 4237 "ulpCompy.y"
    {
        gUlpStmttype = S_GetConditionDiag;
        gUlpIsPrintStmt = ID_FALSE;
    ;}
    break;

  case 428:

/* Line 1455 of yacc.c  */
#line 4255 "ulpCompy.y"
    {
        ulpValidateHostValueWithDiagType(
                              yyvsp,
                              HV_OUT_TYPE,
                              HV_FILE_NONE,
                              ID_FALSE,
                              (SInt)3,
                              (SInt)1,
                              (SInt)0,
                              (ulpHostDiagType)(yyvsp[(3) - (3)].intval)
                            );
    ;}
    break;

  case 429:

/* Line 1455 of yacc.c  */
#line 4271 "ulpCompy.y"
    {
        (yyval.intval) = H_STMT_DIAG_NUMBER;
    ;}
    break;

  case 430:

/* Line 1455 of yacc.c  */
#line 4275 "ulpCompy.y"
    {
        (yyval.intval) = H_STMT_DIAG_ROW_COUNT;
    ;}
    break;

  case 431:

/* Line 1455 of yacc.c  */
#line 4282 "ulpCompy.y"
    {
        SInt sNum;

        sNum = idlOS::atoi((yyvsp[(1) - (1)].strval));

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
            gUlpCodeGen.ulpGenEmSQL( GEN_CONDITIONNUM, (void *) (yyvsp[(1) - (1)].strval) );
        }
    ;}
    break;

  case 432:

/* Line 1455 of yacc.c  */
#line 4303 "ulpCompy.y"
    {
        ulpSymTElement *sSymNode;

        if ( ( sSymNode = gUlpScopeT.ulpSLookupAll( (yyvsp[(1) - (1)].strval)+1, gUlpCurDepth ) ) == NULL )
        {
            //host 변수 못찾는 error
            //declare section에 선언하지 않은 변수도 condition 절에 호스트 변수로
            //사용할 수 있으므로 에러 처리하지 않음
        }

        gUlpCodeGen.ulpGenEmSQL( GEN_CONDITIONNUM, (yyvsp[(1) - (1)].strval)+1 );
    ;}
    break;

  case 435:

/* Line 1455 of yacc.c  */
#line 4324 "ulpCompy.y"
    {
        ulpValidateHostValueWithDiagType(
                              yyvsp,
                              HV_OUT_TYPE,
                              HV_FILE_NONE,
                              ID_FALSE,
                              (SInt)4,
                              (SInt)1,
                              (SInt)0,
                              (ulpHostDiagType)(yyvsp[(4) - (4)].intval)
                            );
    ;}
    break;

  case 436:

/* Line 1455 of yacc.c  */
#line 4340 "ulpCompy.y"
    {
        (yyval.intval) = H_COND_DIAG_RETURNED_SQLCODE;
    ;}
    break;

  case 437:

/* Line 1455 of yacc.c  */
#line 4344 "ulpCompy.y"
    {
        (yyval.intval) = H_COND_DIAG_RETURNED_SQLSTATE;
    ;}
    break;

  case 438:

/* Line 1455 of yacc.c  */
#line 4348 "ulpCompy.y"
    {
        (yyval.intval) = H_COND_DIAG_MESSAGE_TEXT;
    ;}
    break;

  case 439:

/* Line 1455 of yacc.c  */
#line 4352 "ulpCompy.y"
    {
        (yyval.intval) = H_COND_DIAG_ROW_NUMBER;
    ;}
    break;

  case 440:

/* Line 1455 of yacc.c  */
#line 4356 "ulpCompy.y"
    {
        (yyval.intval) = H_COND_DIAG_COLUMN_NUMBER;
    ;}
    break;

  case 441:

/* Line 1455 of yacc.c  */
#line 4371 "ulpCompy.y"
    {
        gUlpStmttype = S_DirectOTH;
        gUlpIsPrintStmt = ID_TRUE;
    ;}
    break;

  case 442:

/* Line 1455 of yacc.c  */
#line 4377 "ulpCompy.y"
    {
        gUlpIsPrintStmt = ID_TRUE;
    ;}
    break;

  case 443:

/* Line 1455 of yacc.c  */
#line 4381 "ulpCompy.y"
    {
        gUlpStmttype = S_Commit;
        gUlpIsPrintStmt = ID_FALSE;
    ;}
    break;

  case 444:

/* Line 1455 of yacc.c  */
#line 4386 "ulpCompy.y"
    {
    ;}
    break;

  case 519:

/* Line 1455 of yacc.c  */
#line 4490 "ulpCompy.y"
    {
        gUlpStmttype = S_DirectDML;

        gUlpCodeGen.ulpGenEmSQL( GEN_QUERYSTR,
                                 idlOS::strstr( gUlpCodeGen.ulpGetQueryBuf(),
                                                (yyvsp[(2) - (2)].strval))
                               );

    ;}
    break;

  case 520:

/* Line 1455 of yacc.c  */
#line 4501 "ulpCompy.y"
    {
        gUlpStmttype = S_DirectDML;

        gUlpCodeGen.ulpGenEmSQL( GEN_QUERYSTR,
                                 idlOS::strstr( gUlpCodeGen.ulpGetQueryBuf(),
                                                (yyvsp[(1) - (1)].strval))
                               );
    ;}
    break;

  case 521:

/* Line 1455 of yacc.c  */
#line 4511 "ulpCompy.y"
    {
        gUlpStmttype = S_DirectDML;

        gUlpCodeGen.ulpGenEmSQL( GEN_QUERYSTR,
                                 idlOS::strstr( gUlpCodeGen.ulpGetQueryBuf(),
                                                (yyvsp[(2) - (3)].strval))
                               );
    ;}
    break;

  case 522:

/* Line 1455 of yacc.c  */
#line 4521 "ulpCompy.y"
    {
        gUlpStmttype = S_DirectDML;

        gUlpCodeGen.ulpGenEmSQL( GEN_QUERYSTR,
                                 idlOS::strstr( gUlpCodeGen.ulpGetQueryBuf(),
                                                (yyvsp[(1) - (2)].strval))
                               );
    ;}
    break;

  case 523:

/* Line 1455 of yacc.c  */
#line 4531 "ulpCompy.y"
    {
        gUlpStmttype = S_DirectDML;

        gUlpCodeGen.ulpGenEmSQL( GEN_QUERYSTR,
                                 idlOS::strstr( gUlpCodeGen.ulpGetQueryBuf(),
                                                (yyvsp[(2) - (2)].strval))
                               );

    ;}
    break;

  case 524:

/* Line 1455 of yacc.c  */
#line 4542 "ulpCompy.y"
    {
        gUlpStmttype = S_DirectDML;

        gUlpCodeGen.ulpGenEmSQL( GEN_QUERYSTR,
                                 idlOS::strstr( gUlpCodeGen.ulpGetQueryBuf(),
                                                (yyvsp[(1) - (1)].strval))
                               );

    ;}
    break;

  case 525:

/* Line 1455 of yacc.c  */
#line 4553 "ulpCompy.y"
    {
        gUlpStmttype = S_DirectDML;

        gUlpCodeGen.ulpGenEmSQL( GEN_QUERYSTR,
                                 idlOS::strstr( gUlpCodeGen.ulpGetQueryBuf(),
                                                (yyvsp[(2) - (3)].strval))
                               );

    ;}
    break;

  case 526:

/* Line 1455 of yacc.c  */
#line 4564 "ulpCompy.y"
    {
        gUlpStmttype = S_DirectDML;

        gUlpCodeGen.ulpGenEmSQL( GEN_QUERYSTR,
                                 idlOS::strstr( gUlpCodeGen.ulpGetQueryBuf(),
                                                (yyvsp[(1) - (2)].strval))
                               );

    ;}
    break;

  case 527:

/* Line 1455 of yacc.c  */
#line 4575 "ulpCompy.y"
    {
        gUlpStmttype = S_DirectDML;

        gUlpCodeGen.ulpGenEmSQL( GEN_QUERYSTR,
                                 idlOS::strstr( gUlpCodeGen.ulpGetQueryBuf(),
                                                (yyvsp[(2) - (2)].strval))
                               );

    ;}
    break;

  case 528:

/* Line 1455 of yacc.c  */
#line 4586 "ulpCompy.y"
    {
        gUlpStmttype = S_DirectDML;

        gUlpCodeGen.ulpGenEmSQL( GEN_QUERYSTR,
                                 idlOS::strstr( gUlpCodeGen.ulpGetQueryBuf(),
                                                (yyvsp[(1) - (1)].strval))
                               );

    ;}
    break;

  case 529:

/* Line 1455 of yacc.c  */
#line 4597 "ulpCompy.y"
    {
        gUlpStmttype = S_DirectDML;

        gUlpCodeGen.ulpGenEmSQL( GEN_QUERYSTR,
                                 idlOS::strstr( gUlpCodeGen.ulpGetQueryBuf(),
                                                (yyvsp[(2) - (3)].strval))
                               );

    ;}
    break;

  case 530:

/* Line 1455 of yacc.c  */
#line 4608 "ulpCompy.y"
    {
        gUlpStmttype = S_DirectDML;

        gUlpCodeGen.ulpGenEmSQL( GEN_QUERYSTR,
                                 idlOS::strstr( gUlpCodeGen.ulpGetQueryBuf(),
                                                (yyvsp[(1) - (2)].strval))
                               );

    ;}
    break;

  case 531:

/* Line 1455 of yacc.c  */
#line 4619 "ulpCompy.y"
    {
        gUlpStmttype = S_DirectDML;

        gUlpCodeGen.ulpGenEmSQL( GEN_QUERYSTR,
                                 idlOS::strstr( gUlpCodeGen.ulpGetQueryBuf(),
                                                (yyvsp[(1) - (1)].strval))
                               );

    ;}
    break;

  case 532:

/* Line 1455 of yacc.c  */
#line 4630 "ulpCompy.y"
    {
        gUlpStmttype = S_DirectDML;

        gUlpCodeGen.ulpGenEmSQL( GEN_QUERYSTR,
                                 idlOS::strstr( gUlpCodeGen.ulpGetQueryBuf(),
                                                (yyvsp[(1) - (1)].strval))
                               );

    ;}
    break;

  case 533:

/* Line 1455 of yacc.c  */
#line 4641 "ulpCompy.y"
    {
        gUlpStmttype = S_DirectSEL;

        gUlpCodeGen.ulpGenEmSQL( GEN_QUERYSTR,
                                 idlOS::strstr( gUlpCodeGen.ulpGetQueryBuf(),
                                                (yyvsp[(1) - (3)].strval))
                               );

    ;}
    break;

  case 534:

/* Line 1455 of yacc.c  */
#line 4652 "ulpCompy.y"
    {
        gUlpStmttype = S_DirectSEL;

        gUlpCodeGen.ulpGenEmSQL( GEN_QUERYSTR,
                                 idlOS::strstr( gUlpCodeGen.ulpGetQueryBuf(),
                                                (yyvsp[(1) - (4)].strval))
                               );

    ;}
    break;

  case 535:

/* Line 1455 of yacc.c  */
#line 4663 "ulpCompy.y"
    {
        //is_select = sesTRUE;
        gUlpStmttype = S_DirectSEL;

        gUlpCodeGen.ulpGenEmSQL( GEN_QUERYSTR,
                                 idlOS::strstr( gUlpCodeGen.ulpGetQueryBuf(),
                                                (yyvsp[(1) - (3)].strval))
                               );
    ;}
    break;

  case 536:

/* Line 1455 of yacc.c  */
#line 4674 "ulpCompy.y"
    {
        gUlpStmttype = S_DirectDML;

        gUlpCodeGen.ulpGenEmSQL( GEN_QUERYSTR,
                                 idlOS::strstr( gUlpCodeGen.ulpGetQueryBuf(),
                                                (yyvsp[(1) - (1)].strval))
                               );
    ;}
    break;

  case 540:

/* Line 1455 of yacc.c  */
#line 4692 "ulpCompy.y"
    {
        gUlpCodeGen.ulpGenEmSQL( GEN_STATUSPTR, (yyvsp[(2) - (4)].strval)+1 );
        gUlpCodeGen.ulpGenEmSQL( GEN_ERRCODEPTR, (yyvsp[(4) - (4)].strval)+1 );
    ;}
    break;

  case 541:

/* Line 1455 of yacc.c  */
#line 4700 "ulpCompy.y"
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

  case 542:

/* Line 1455 of yacc.c  */
#line 4721 "ulpCompy.y"
    {
        ulpSymTElement *sSymNode;

        if ( ( sSymNode = gUlpScopeT.ulpSLookupAll( (yyvsp[(2) - (2)].strval)+1, gUlpCurDepth ) ) == NULL )
        {
            //host 변수 못찾는 error
        }

        gUlpCodeGen.ulpGenEmSQL( GEN_ITERS, (yyvsp[(2) - (2)].strval)+1 );
    ;}
    break;

  case 543:

/* Line 1455 of yacc.c  */
#line 4732 "ulpCompy.y"
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

  case 544:

/* Line 1455 of yacc.c  */
#line 4763 "ulpCompy.y"
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

  case 545:

/* Line 1455 of yacc.c  */
#line 4791 "ulpCompy.y"
    {
        gUlpCodeGen.ulpGenEmSQL( GEN_SQLINFO, (void *) "0" );
    ;}
    break;

  case 546:

/* Line 1455 of yacc.c  */
#line 4795 "ulpCompy.y"
    {
        // release 구문이 오면 disconnect 해야한다.
        gUlpCodeGen.ulpGenEmSQL( GEN_SQLINFO, (void *) "1" );
    ;}
    break;

  case 547:

/* Line 1455 of yacc.c  */
#line 4803 "ulpCompy.y"
    {
        gUlpCodeGen.ulpGenEmSQL( GEN_SQLINFO, (void *) "2" );
    ;}
    break;

  case 548:

/* Line 1455 of yacc.c  */
#line 4807 "ulpCompy.y"
    {
        // release 구문이 오면 disconnect 해야한다.
        gUlpCodeGen.ulpGenEmSQL( GEN_SQLINFO, (void *) "3" );
        // 나중에 rollback 구문을 comment로 출력할때 release 토큰은 제거됨을 주의하자.
        gUlpCodeGen.ulpGenCutQueryTail( (yyvsp[(2) - (2)].strval) );
    ;}
    break;

  case 615:

/* Line 1455 of yacc.c  */
#line 4902 "ulpCompy.y"
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

  case 616:

/* Line 1455 of yacc.c  */
#line 4918 "ulpCompy.y"
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

  case 617:

/* Line 1455 of yacc.c  */
#line 4932 "ulpCompy.y"
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

  case 618:

/* Line 1455 of yacc.c  */
#line 4946 "ulpCompy.y"
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

  case 620:

/* Line 1455 of yacc.c  */
#line 4962 "ulpCompy.y"
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

  case 622:

/* Line 1455 of yacc.c  */
#line 4982 "ulpCompy.y"
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

  case 623:

/* Line 1455 of yacc.c  */
#line 5008 "ulpCompy.y"
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

  case 624:

/* Line 1455 of yacc.c  */
#line 5042 "ulpCompy.y"
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

  case 625:

/* Line 1455 of yacc.c  */
#line 5056 "ulpCompy.y"
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

  case 626:

/* Line 1455 of yacc.c  */
#line 5069 "ulpCompy.y"
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

  case 627:

/* Line 1455 of yacc.c  */
#line 5082 "ulpCompy.y"
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

  case 628:

/* Line 1455 of yacc.c  */
#line 5095 "ulpCompy.y"
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

  case 629:

/* Line 1455 of yacc.c  */
#line 5108 "ulpCompy.y"
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

  case 630:

/* Line 1455 of yacc.c  */
#line 5122 "ulpCompy.y"
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

  case 634:

/* Line 1455 of yacc.c  */
#line 5145 "ulpCompy.y"
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

  case 637:

/* Line 1455 of yacc.c  */
#line 5168 "ulpCompy.y"
    {
        gUlpStmttype    = S_Commit;
        gUlpIsPrintStmt = ID_FALSE;
    ;}
    break;

  case 638:

/* Line 1455 of yacc.c  */
#line 5173 "ulpCompy.y"
    {
        gUlpStmttype    = S_DirectRB;
        gUlpIsPrintStmt = ID_TRUE;
        gUlpCodeGen.ulpGenEmSQL( GEN_QUERYSTR,
                                 idlOS::strstr( gUlpCodeGen.ulpGetQueryBuf(),
                                                (yyvsp[(1) - (5)].strval))
                               );
    ;}
    break;

  case 641:

/* Line 1455 of yacc.c  */
#line 5190 "ulpCompy.y"
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

  case 642:

/* Line 1455 of yacc.c  */
#line 5202 "ulpCompy.y"
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

  case 643:

/* Line 1455 of yacc.c  */
#line 5218 "ulpCompy.y"
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

  case 644:

/* Line 1455 of yacc.c  */
#line 5234 "ulpCompy.y"
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

  case 647:

/* Line 1455 of yacc.c  */
#line 5254 "ulpCompy.y"
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

  case 648:

/* Line 1455 of yacc.c  */
#line 5268 "ulpCompy.y"
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

  case 649:

/* Line 1455 of yacc.c  */
#line 5281 "ulpCompy.y"
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

  case 688:

/* Line 1455 of yacc.c  */
#line 5409 "ulpCompy.y"
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

  case 740:

/* Line 1455 of yacc.c  */
#line 5477 "ulpCompy.y"
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
#line 5489 "ulpCompy.y"
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

  case 742:

/* Line 1455 of yacc.c  */
#line 5501 "ulpCompy.y"
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

  case 770:

/* Line 1455 of yacc.c  */
#line 5576 "ulpCompy.y"
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

  case 778:

/* Line 1455 of yacc.c  */
#line 5627 "ulpCompy.y"
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

  case 779:

/* Line 1455 of yacc.c  */
#line 5640 "ulpCompy.y"
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

  case 780:

/* Line 1455 of yacc.c  */
#line 5653 "ulpCompy.y"
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

  case 782:

/* Line 1455 of yacc.c  */
#line 5672 "ulpCompy.y"
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

  case 789:

/* Line 1455 of yacc.c  */
#line 5705 "ulpCompy.y"
    {
        if(idlOS::strncasecmp("SYNC", (yyvsp[(4) - (5)].strval), 4) != 0 &&
           idlOS::strncasecmp("QUICKSTART", (yyvsp[(4) - (5)].strval), 10) != 0 &&
           idlOS::strncasecmp("STOP", (yyvsp[(4) - (5)].strval), 4) != 0 &&
           idlOS::strncasecmp("RESET", (yyvsp[(4) - (5)].strval), 5) != 0 &&
           idlOS::strncasecmp("FAILOVER", (yyvsp[(4) - (5)].strval), 8) != 0)
        {
            // error 처리

            ulpSetErrorCode( &gUlpParseInfo.mErrorMgr,
                             ulpERR_ABORT_COMP_Unterminated_String_Error );
            gUlpCOMPErrCode = ulpGetErrorSTATE( &gUlpParseInfo.mErrorMgr );
            COMPerror( ulpGetErrorMSG(&gUlpParseInfo.mErrorMgr) );
        }
    ;}
    break;

  case 790:

/* Line 1455 of yacc.c  */
#line 5723 "ulpCompy.y"
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

  case 793:

/* Line 1455 of yacc.c  */
#line 5742 "ulpCompy.y"
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

  case 796:

/* Line 1455 of yacc.c  */
#line 5762 "ulpCompy.y"
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

  case 802:

/* Line 1455 of yacc.c  */
#line 5791 "ulpCompy.y"
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

  case 806:

/* Line 1455 of yacc.c  */
#line 5813 "ulpCompy.y"
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

  case 811:

/* Line 1455 of yacc.c  */
#line 5839 "ulpCompy.y"
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

  case 812:

/* Line 1455 of yacc.c  */
#line 5852 "ulpCompy.y"
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

  case 815:

/* Line 1455 of yacc.c  */
#line 5870 "ulpCompy.y"
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

  case 816:

/* Line 1455 of yacc.c  */
#line 5883 "ulpCompy.y"
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

  case 818:

/* Line 1455 of yacc.c  */
#line 5899 "ulpCompy.y"
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

  case 830:

/* Line 1455 of yacc.c  */
#line 5937 "ulpCompy.y"
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

  case 839:

/* Line 1455 of yacc.c  */
#line 5969 "ulpCompy.y"
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

  case 840:

/* Line 1455 of yacc.c  */
#line 5982 "ulpCompy.y"
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

  case 871:

/* Line 1455 of yacc.c  */
#line 6085 "ulpCompy.y"
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

  case 872:

/* Line 1455 of yacc.c  */
#line 6098 "ulpCompy.y"
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

  case 873:

/* Line 1455 of yacc.c  */
#line 6111 "ulpCompy.y"
    {
        /* TASK-7217 Sharded sequence */
        if(idlOS::strncasecmp("RESTART", (yyvsp[(1) - (3)].strval), 7) != 0)
        {
            // error 처리

            ulpSetErrorCode( &gUlpParseInfo.mErrorMgr,
                             ulpERR_ABORT_COMP_Unterminated_String_Error );
            gUlpCOMPErrCode = ulpGetErrorSTATE( &gUlpParseInfo.mErrorMgr );
            COMPerror( ulpGetErrorMSG(&gUlpParseInfo.mErrorMgr) );
        }
    ;}
    break;

  case 874:

/* Line 1455 of yacc.c  */
#line 6125 "ulpCompy.y"
    {
        /* TASK-7217 Sharded sequence */
        if(idlOS::strncasecmp("RESTART", (yyvsp[(1) - (4)].strval), 7) != 0)
        {
            // error 처리

            ulpSetErrorCode( &gUlpParseInfo.mErrorMgr,
                             ulpERR_ABORT_COMP_Unterminated_String_Error );
            gUlpCOMPErrCode = ulpGetErrorSTATE( &gUlpParseInfo.mErrorMgr );
            COMPerror( ulpGetErrorMSG(&gUlpParseInfo.mErrorMgr) );
        }
    ;}
    break;

  case 875:

/* Line 1455 of yacc.c  */
#line 6141 "ulpCompy.y"
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

  case 876:

/* Line 1455 of yacc.c  */
#line 6155 "ulpCompy.y"
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

  case 877:

/* Line 1455 of yacc.c  */
#line 6172 "ulpCompy.y"
    {
        if(idlOS::strncasecmp("NOCACHE", (yyvsp[(1) - (1)].strval), 7) != 0 &&
           idlOS::strncasecmp("NOMAXVALUE", (yyvsp[(1) - (1)].strval), 10) != 0 &&
           idlOS::strncasecmp("NOMINVALUE", (yyvsp[(1) - (1)].strval), 10) != 0 &&
           idlOS::strncasecmp("RESTART", (yyvsp[(1) - (1)].strval), 7) != 0)  /* TASK-7217 Sharded sequence */
        {
            // error 처리

            ulpSetErrorCode( &gUlpParseInfo.mErrorMgr,
                             ulpERR_ABORT_COMP_Unterminated_String_Error );
            gUlpCOMPErrCode = ulpGetErrorSTATE( &gUlpParseInfo.mErrorMgr );
            COMPerror( ulpGetErrorMSG(&gUlpParseInfo.mErrorMgr) );
        }
    ;}
    break;

  case 895:

/* Line 1455 of yacc.c  */
#line 6230 "ulpCompy.y"
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

  case 977:

/* Line 1455 of yacc.c  */
#line 6476 "ulpCompy.y"
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

  case 979:

/* Line 1455 of yacc.c  */
#line 6496 "ulpCompy.y"
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

  case 980:

/* Line 1455 of yacc.c  */
#line 6508 "ulpCompy.y"
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

  case 1026:

/* Line 1455 of yacc.c  */
#line 6646 "ulpCompy.y"
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

  case 1036:

/* Line 1455 of yacc.c  */
#line 6688 "ulpCompy.y"
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

  case 1098:

/* Line 1455 of yacc.c  */
#line 6869 "ulpCompy.y"
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

  case 1108:

/* Line 1455 of yacc.c  */
#line 6932 "ulpCompy.y"
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

  case 1109:

/* Line 1455 of yacc.c  */
#line 6945 "ulpCompy.y"
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

  case 1126:

/* Line 1455 of yacc.c  */
#line 7005 "ulpCompy.y"
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

  case 1129:

/* Line 1455 of yacc.c  */
#line 7027 "ulpCompy.y"
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

  case 1130:

/* Line 1455 of yacc.c  */
#line 7039 "ulpCompy.y"
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

  case 1135:

/* Line 1455 of yacc.c  */
#line 7080 "ulpCompy.y"
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

  case 1136:

/* Line 1455 of yacc.c  */
#line 7091 "ulpCompy.y"
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

  case 1270:

/* Line 1455 of yacc.c  */
#line 7531 "ulpCompy.y"
    {
    gUlpCodeGen.ulpGenRemoveQueryToken( (yyvsp[(1) - (2)].strval) );
  ;}
    break;

  case 1407:

/* Line 1455 of yacc.c  */
#line 7864 "ulpCompy.y"
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

  case 1410:

/* Line 1455 of yacc.c  */
#line 7881 "ulpCompy.y"
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

  case 1415:

/* Line 1455 of yacc.c  */
#line 7908 "ulpCompy.y"
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

  case 1425:

/* Line 1455 of yacc.c  */
#line 7947 "ulpCompy.y"
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

  case 1445:

/* Line 1455 of yacc.c  */
#line 8006 "ulpCompy.y"
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

  case 1446:

/* Line 1455 of yacc.c  */
#line 8020 "ulpCompy.y"
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

  case 1447:

/* Line 1455 of yacc.c  */
#line 8033 "ulpCompy.y"
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

  case 1448:

/* Line 1455 of yacc.c  */
#line 8048 "ulpCompy.y"
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

  case 1450:

/* Line 1455 of yacc.c  */
#line 8066 "ulpCompy.y"
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

  case 1488:

/* Line 1455 of yacc.c  */
#line 8151 "ulpCompy.y"
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

  case 1713:

/* Line 1455 of yacc.c  */
#line 8892 "ulpCompy.y"
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

  case 1724:

/* Line 1455 of yacc.c  */
#line 8937 "ulpCompy.y"
    {
        /* BUG-46824 psm execute 여부를 확인하기 위하여 
         * begin ~~ end 사이의 모든 객체를 counting해야한다. */
        gUlpProcObjCount++;
    ;}
    break;

  case 1736:

/* Line 1455 of yacc.c  */
#line 8982 "ulpCompy.y"
    {
        /* BUG-46824 psm execute 여부를 확인하기 위하여 
         * begin ~~ end 사이의 모든 객체를 counting해야한다. */
        gUlpProcObjCount++;
    ;}
    break;

  case 1737:

/* Line 1455 of yacc.c  */
#line 8988 "ulpCompy.y"
    {
        /* BUG-46824 psm execute 여부를 확인하기 위하여 
         * begin ~~ end 사이의 모든 객체를 counting해야한다. */
        gUlpProcObjCount++;
    ;}
    break;

  case 1770:

/* Line 1455 of yacc.c  */
#line 9039 "ulpCompy.y"
    {
        if (gUlpPSMObjName == NULL)
        {
            /* BUG-46824 begin ~~ end 사이의 첫번째 object_name을 저장. */ 
            /* BUG-47868 gUlpPSMObjName는 ulpGetQueryBuf동일한 memory를 참조해야 한다. */
            gUlpPSMObjName = idlOS::strstr(gUlpCodeGen.ulpGetQueryBuf(), (yyvsp[(1) - (2)].strval));
        }
    ;}
    break;

  case 1841:

/* Line 1455 of yacc.c  */
#line 9385 "ulpCompy.y"
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

  case 1847:

/* Line 1455 of yacc.c  */
#line 9423 "ulpCompy.y"
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

  case 1848:

/* Line 1455 of yacc.c  */
#line 9441 "ulpCompy.y"
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

  case 1855:

/* Line 1455 of yacc.c  */
#line 9463 "ulpCompy.y"
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

  case 1856:

/* Line 1455 of yacc.c  */
#line 9476 "ulpCompy.y"
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

  case 1858:

/* Line 1455 of yacc.c  */
#line 9491 "ulpCompy.y"
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

  case 1859:

/* Line 1455 of yacc.c  */
#line 9504 "ulpCompy.y"
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

  case 1861:

/* Line 1455 of yacc.c  */
#line 9519 "ulpCompy.y"
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

  case 1862:

/* Line 1455 of yacc.c  */
#line 9532 "ulpCompy.y"
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

  case 1867:

/* Line 1455 of yacc.c  */
#line 9558 "ulpCompy.y"
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

  case 1868:

/* Line 1455 of yacc.c  */
#line 9577 "ulpCompy.y"
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

  case 1869:

/* Line 1455 of yacc.c  */
#line 9609 "ulpCompy.y"
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

  case 1870:

/* Line 1455 of yacc.c  */
#line 9622 "ulpCompy.y"
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

  case 1872:

/* Line 1455 of yacc.c  */
#line 9639 "ulpCompy.y"
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

  case 1873:

/* Line 1455 of yacc.c  */
#line 9652 "ulpCompy.y"
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

  case 1875:

/* Line 1455 of yacc.c  */
#line 9674 "ulpCompy.y"
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

  case 1876:

/* Line 1455 of yacc.c  */
#line 9693 "ulpCompy.y"
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

  case 1877:

/* Line 1455 of yacc.c  */
#line 9713 "ulpCompy.y"
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

  case 1878:

/* Line 1455 of yacc.c  */
#line 9742 "ulpCompy.y"
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

  case 1880:

/* Line 1455 of yacc.c  */
#line 9762 "ulpCompy.y"
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

  case 1881:

/* Line 1455 of yacc.c  */
#line 9778 "ulpCompy.y"
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

  case 1885:

/* Line 1455 of yacc.c  */
#line 9803 "ulpCompy.y"
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

  case 1886:

/* Line 1455 of yacc.c  */
#line 9819 "ulpCompy.y"
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

  case 1887:

/* Line 1455 of yacc.c  */
#line 9835 "ulpCompy.y"
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

  case 1888:

/* Line 1455 of yacc.c  */
#line 9852 "ulpCompy.y"
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

  case 1893:

/* Line 1455 of yacc.c  */
#line 9890 "ulpCompy.y"
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

  case 1894:

/* Line 1455 of yacc.c  */
#line 9905 "ulpCompy.y"
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

  case 1895:

/* Line 1455 of yacc.c  */
#line 9929 "ulpCompy.y"
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

  case 1896:

/* Line 1455 of yacc.c  */
#line 9943 "ulpCompy.y"
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

  case 1899:

/* Line 1455 of yacc.c  */
#line 9974 "ulpCompy.y"
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

  case 1900:

/* Line 1455 of yacc.c  */
#line 9988 "ulpCompy.y"
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

  case 1901:

/* Line 1455 of yacc.c  */
#line 10003 "ulpCompy.y"
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

  case 1902:

/* Line 1455 of yacc.c  */
#line 10018 "ulpCompy.y"
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

  case 1903:

/* Line 1455 of yacc.c  */
#line 10034 "ulpCompy.y"
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

  case 1904:

/* Line 1455 of yacc.c  */
#line 10048 "ulpCompy.y"
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

  case 1905:

/* Line 1455 of yacc.c  */
#line 10066 "ulpCompy.y"
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

  case 1906:

/* Line 1455 of yacc.c  */
#line 10087 "ulpCompy.y"
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

  case 1907:

/* Line 1455 of yacc.c  */
#line 10103 "ulpCompy.y"
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

  case 1918:

/* Line 1455 of yacc.c  */
#line 10144 "ulpCompy.y"
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

  case 1920:

/* Line 1455 of yacc.c  */
#line 10165 "ulpCompy.y"
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

  case 1929:

/* Line 1455 of yacc.c  */
#line 10206 "ulpCompy.y"
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

  case 1930:

/* Line 1455 of yacc.c  */
#line 10221 "ulpCompy.y"
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

  case 1975:

/* Line 1455 of yacc.c  */
#line 10374 "ulpCompy.y"
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

  case 1976:

/* Line 1455 of yacc.c  */
#line 10434 "ulpCompy.y"
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

  case 1977:

/* Line 1455 of yacc.c  */
#line 10449 "ulpCompy.y"
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

  case 1978:

/* Line 1455 of yacc.c  */
#line 10484 "ulpCompy.y"
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

  case 1979:

/* Line 1455 of yacc.c  */
#line 10546 "ulpCompy.y"
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

  case 1980:

/* Line 1455 of yacc.c  */
#line 10587 "ulpCompy.y"
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

  case 1981:

/* Line 1455 of yacc.c  */
#line 10640 "ulpCompy.y"
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

  case 1984:

/* Line 1455 of yacc.c  */
#line 10663 "ulpCompy.y"
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

  case 1985:

/* Line 1455 of yacc.c  */
#line 10698 "ulpCompy.y"
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

  case 1986:

/* Line 1455 of yacc.c  */
#line 10713 "ulpCompy.y"
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

  case 1988:

/* Line 1455 of yacc.c  */
#line 10764 "ulpCompy.y"
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

  case 2001:

/* Line 1455 of yacc.c  */
#line 10816 "ulpCompy.y"
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

  case 2002:

/* Line 1455 of yacc.c  */
#line 10829 "ulpCompy.y"
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

  case 2003:

/* Line 1455 of yacc.c  */
#line 10842 "ulpCompy.y"
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

  case 2004:

/* Line 1455 of yacc.c  */
#line 10855 "ulpCompy.y"
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

  case 2005:

/* Line 1455 of yacc.c  */
#line 10867 "ulpCompy.y"
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

  case 2006:

/* Line 1455 of yacc.c  */
#line 10880 "ulpCompy.y"
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

  case 2007:

/* Line 1455 of yacc.c  */
#line 10896 "ulpCompy.y"
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

  case 2021:

/* Line 1455 of yacc.c  */
#line 10934 "ulpCompy.y"
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

  case 2022:

/* Line 1455 of yacc.c  */
#line 10952 "ulpCompy.y"
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

  case 2023:

/* Line 1455 of yacc.c  */
#line 10963 "ulpCompy.y"
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

  case 2024:

/* Line 1455 of yacc.c  */
#line 10974 "ulpCompy.y"
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

  case 2025:

/* Line 1455 of yacc.c  */
#line 10985 "ulpCompy.y"
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

  case 2026:

/* Line 1455 of yacc.c  */
#line 10996 "ulpCompy.y"
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

  case 2029:

/* Line 1455 of yacc.c  */
#line 11015 "ulpCompy.y"
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

  case 2030:

/* Line 1455 of yacc.c  */
#line 11026 "ulpCompy.y"
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

  case 2031:

/* Line 1455 of yacc.c  */
#line 11037 "ulpCompy.y"
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

  case 2032:

/* Line 1455 of yacc.c  */
#line 11048 "ulpCompy.y"
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

  case 2033:

/* Line 1455 of yacc.c  */
#line 11059 "ulpCompy.y"
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

  case 2034:

/* Line 1455 of yacc.c  */
#line 11070 "ulpCompy.y"
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

  case 2035:

/* Line 1455 of yacc.c  */
#line 11084 "ulpCompy.y"
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

  case 2036:

/* Line 1455 of yacc.c  */
#line 11122 "ulpCompy.y"
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

  case 2039:

/* Line 1455 of yacc.c  */
#line 11156 "ulpCompy.y"
    {
        gUlpCodeGen.ulpGenRemoveQueryToken( (yyvsp[(1) - (2)].strval) );
    ;}
    break;

  case 2040:

/* Line 1455 of yacc.c  */
#line 11163 "ulpCompy.y"
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

  case 2041:

/* Line 1455 of yacc.c  */
#line 11174 "ulpCompy.y"
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

  case 2042:

/* Line 1455 of yacc.c  */
#line 11185 "ulpCompy.y"
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

  case 2043:

/* Line 1455 of yacc.c  */
#line 11196 "ulpCompy.y"
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

  case 2044:

/* Line 1455 of yacc.c  */
#line 11207 "ulpCompy.y"
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

  case 2045:

/* Line 1455 of yacc.c  */
#line 11218 "ulpCompy.y"
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

  case 2047:

/* Line 1455 of yacc.c  */
#line 11233 "ulpCompy.y"
    {
        gUlpCodeGen.ulpGenRemoveQueryToken( (yyvsp[(1) - (2)].strval) );
    ;}
    break;

  case 2050:

/* Line 1455 of yacc.c  */
#line 11245 "ulpCompy.y"
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

  case 2051:

/* Line 1455 of yacc.c  */
#line 11256 "ulpCompy.y"
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

  case 2052:

/* Line 1455 of yacc.c  */
#line 11267 "ulpCompy.y"
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

  case 2053:

/* Line 1455 of yacc.c  */
#line 11278 "ulpCompy.y"
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

  case 2054:

/* Line 1455 of yacc.c  */
#line 11289 "ulpCompy.y"
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

  case 2055:

/* Line 1455 of yacc.c  */
#line 11300 "ulpCompy.y"
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

  case 2058:

/* Line 1455 of yacc.c  */
#line 11320 "ulpCompy.y"
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

  case 2059:

/* Line 1455 of yacc.c  */
#line 11335 "ulpCompy.y"
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

  case 2061:

/* Line 1455 of yacc.c  */
#line 11363 "ulpCompy.y"
    {
        gUlpCodeGen.ulpGenRemoveQueryToken( (yyvsp[(1) - (1)].strval) );
    ;}
    break;

  case 2064:

/* Line 1455 of yacc.c  */
#line 11375 "ulpCompy.y"
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

  case 2066:

/* Line 1455 of yacc.c  */
#line 11475 "ulpCompy.y"
    {
        gUlpCodeGen.ulpGenRemoveQueryToken( (yyvsp[(1) - (1)].strval) );
    ;}
    break;

  case 2074:

/* Line 1455 of yacc.c  */
#line 11499 "ulpCompy.y"
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
#line 17002 "ulpCompy.cpp"
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
#line 11541 "ulpCompy.y"



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
    (void) ulpValidateHostValueWithDiagType(
                           yyvsp,
                           aInOutType,
                           aFileType,
                           aTransformQuery,
                           aNumofTokens,
                           aHostValIndex,
                           aRemoveTokIndexs, 
                           H_DIAG_UNKNOWN );
}

void ulpValidateHostValueWithDiagType(
                           void           *yyvsp,
                           ulpHVarType     aInOutType,
                           ulpHVFileType   aFileType,
                           idBool          aTransformQuery,
                           SInt            aNumofTokens,
                           SInt            aHostValIndex,
                           SInt            aRemoveTokIndexs, 
                           ulpHostDiagType aDiagType )
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

        /* TASK-7218 Handling Multiple Errors */
        sSymNode->mDiagType = aDiagType;
        if ( ulpValidateFORGetDiagnostics(sSymNode) != IDE_SUCCESS )
        {
            ulpSetErrorCode( &gUlpParseInfo.mErrorMgr,
                             ulpERR_ABORT_Incompatible_Type_With_Diag_Item,
                             (*(((YYSTYPE *)yyvsp)[aHostValIndex - aNumofTokens].strval)==':')?
                             (((YYSTYPE *)yyvsp)[aHostValIndex - aNumofTokens].strval)+1:
                             (((YYSTYPE *)yyvsp)[aHostValIndex - aNumofTokens].strval)
                           );
            gUlpCOMPErrCode = ulpGetErrorSTATE( &gUlpParseInfo.mErrorMgr );
            COMPerror( ulpGetErrorMSG(&gUlpParseInfo.mErrorMgr) );
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
 *  ulpValidateFORGetDiagnostics
 *
 *  Description :
 *     ulpValidateHostValue에서 호출되는 함수로써, 
 *     GET DIAGNOSTICS 문에 사용되는 host 변수를 체크한다.
 *     run-time시에 타입 호환 검사를 하면 에러가 발생하여도
 *     반환할 방법이 없으므로 precompile 시점에 검사를 함.
 *
 *  Parameters :  
 *     ulpSymTElement *aElement : 체크해야될 host 변수 정보
 * ========================================================*/
IDE_RC ulpValidateFORGetDiagnostics(ulpSymTElement *aElement)
{
    ulpHostType sType = aElement->mType;

    switch(aElement->mDiagType)
    {
    case H_STMT_DIAG_NUMBER:
    case H_STMT_DIAG_ROW_COUNT:
    case H_COND_DIAG_RETURNED_SQLCODE:
    case H_COND_DIAG_ROW_NUMBER:
    case H_COND_DIAG_COLUMN_NUMBER:
        IDE_TEST( sType != H_INTEGER && sType != H_INT );
        break;
    case H_COND_DIAG_RETURNED_SQLSTATE:
    case H_COND_DIAG_MESSAGE_TEXT:
        IDE_TEST( sType != H_CHAR && sType != H_VARCHAR );
        break;
    default:
        break;
    }
    return IDE_SUCCESS;
        
    IDE_EXCEPTION_END;
    
    return IDE_FAILURE;
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

