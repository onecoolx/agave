/* A Bison parser, made by GNU Bison 3.5.1.  */

/* Bison implementation for Yacc-like parsers in C

   Copyright (C) 1984, 1989-1990, 2000-2015, 2018-2020 Free Software Foundation,
   Inc.

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

/* Undocumented macros, especially those whose name start with YY_,
   are private implementation details.  Do not rely on them.  */

/* Identify Bison output.  */
#define YYBISON 1

/* Bison version.  */
#define YYBISON_VERSION "3.5.1"

/* Skeleton name.  */
#define YYSKELETON_NAME "yacc.c"

/* Pure parsers.  */
#define YYPURE 1

/* Push parsers.  */
#define YYPUSH 0

/* Pull parsers.  */
#define YYPULL 1


/* Substitute the variable and function names.  */
#define yyparse         cssyyparse
#define yylex           cssyylex
#define yyerror         cssyyerror
#define yydebug         cssyydebug
#define yynerrs         cssyynerrs

/* First part of user prologue.  */
#line 1 "CSSGrammar.y"


/*
 *  This file is part of the KDE libraries
 *  Copyright (C) 2002-2003 Lars Knoll (knoll@kde.org)
 *  Copyright (C) 2004, 2005, 2006, 2007 Apple Inc.
 *  Copyright (C) 2006 Alexey Proskuryakov (ap@nypop.com)
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU Lesser General Public
 *  License as published by the Free Software Foundation; either
 *  version 2 of the License, or (at your option) any later version.
 *
 *  This library is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *  Lesser General Public License for more details.
 *
 *  You should have received a copy of the GNU Lesser General Public
 *  License along with this library; if not, write to the Free Software
 *  Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
 *
 */

#include "config.h"

#include "CSSMediaRule.h"
#include "CSSParser.h"
#include "CSSPrimitiveValue.h"
#include "CSSRule.h"
#include "CSSRuleList.h"
#include "CSSSelector.h"
#include "CSSStyleSheet.h"
#include "CString.h"
#include "CSSCustomPropertyValue.h"
#include "Document.h"
#include "HTMLNames.h"
#include "MediaList.h"
#include "MediaQuery.h"
#include "MediaQueryExp.h"
#include "PlatformString.h"
#include <stdlib.h>
#include <string.h>

using namespace WebCore;
using namespace HTMLNames;

#define YYMALLOC fastMalloc
#define YYFREE fastFree
// The following file defines the function
//     const struct props *findProp(const char *word, int len)
//
// with 'props->id' a CSS property in the range from CSS_PROP_MIN to
// (and including) CSS_PROP_TOTAL-1

#include "CSSPropertyNames.c"
#include "CSSValueKeywords.c"

namespace WebCore {

int getPropertyID(const char* tagStr, int len)
{
    // Buffer that keeps the rewritten "-webkit-" string alive for the duration
    // of this call when a vendor-prefix alias is normalized below.
    CString buffer;

    if (len && tagStr[0] == '-') {
        String prop(tagStr, len);
        if (prop.startsWith("-apple-") || prop.startsWith("-khtml-")) {
            prop = "-webkit-" + prop.substring(7);
            buffer = prop.latin1();
            tagStr = buffer.data();
            len = buffer.length();
        }

        // Honor the use of old-style opacity (for Safari 1.1).
        if (prop == "-webkit-opacity") {
            const char* const opacity = "opacity";
            tagStr = opacity;
            len = strlen(opacity);
        }
    }

    const struct props* propsPtr = findProp(tagStr, (size_t)len);
    if (!propsPtr)
        return 0;

    return propsPtr->id;
}

} // namespace WebCore

static inline int getValueID(const char* tagStr, int len)
{
    // Buffer that keeps the rewritten "-webkit-" string alive (see above).
    CString buffer;
    if (len && tagStr[0] == '-') {
        String prop(tagStr, len);
        if (prop.startsWith("-apple-") || prop.startsWith("-khtml-")) {
            prop = "-webkit-" + prop.substring(7);
            buffer = prop.latin1();
            tagStr = buffer.data();
            len = buffer.length();
        }
    }

    const struct css_value* val = findValue(tagStr, (size_t)len);
    if (!val)
        return 0;

    return val->id;
}

#define YYENABLE_NLS 0
#define YYLTYPE_IS_TRIVIAL 1
#define YYMAXDEPTH 10000
#define YYDEBUG 0


#line 195 "CSSGrammar.tab.c"

# ifndef YY_CAST
#  ifdef __cplusplus
#   define YY_CAST(Type, Val) static_cast<Type> (Val)
#   define YY_REINTERPRET_CAST(Type, Val) reinterpret_cast<Type> (Val)
#  else
#   define YY_CAST(Type, Val) ((Type) (Val))
#   define YY_REINTERPRET_CAST(Type, Val) ((Type) (Val))
#  endif
# endif
# ifndef YY_NULLPTR
#  if defined __cplusplus
#   if 201103L <= __cplusplus
#    define YY_NULLPTR nullptr
#   else
#    define YY_NULLPTR 0
#   endif
#  else
#   define YY_NULLPTR ((void*)0)
#  endif
# endif

/* Enabling verbose error messages.  */
#ifdef YYERROR_VERBOSE
# undef YYERROR_VERBOSE
# define YYERROR_VERBOSE 1
#else
# define YYERROR_VERBOSE 0
#endif

/* Use api.header.include to #include this header
   instead of duplicating it here.  */
#ifndef YY_CSSYY_CSSGRAMMAR_TAB_H_INCLUDED
# define YY_CSSYY_CSSGRAMMAR_TAB_H_INCLUDED
/* Debug traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
#endif
#if YYDEBUG
extern int cssyydebug;
#endif

/* Token type.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
  enum yytokentype
  {
    UNIMPORTANT_TOK = 258,
    WHITESPACE = 259,
    SGML_CD = 260,
    INCLUDES = 261,
    DASHMATCH = 262,
    BEGINSWITH = 263,
    ENDSWITH = 264,
    CONTAINS = 265,
    STRING = 266,
    IDENT = 267,
    HEX = 268,
    IDSEL = 269,
    IMPORT_SYM = 270,
    PAGE_SYM = 271,
    MEDIA_SYM = 272,
    FONT_FACE_SYM = 273,
    KEYFRAMES_SYM = 274,
    CHARSET_SYM = 275,
    NAMESPACE_SYM = 276,
    WEBKIT_RULE_SYM = 277,
    WEBKIT_DECLS_SYM = 278,
    WEBKIT_VALUE_SYM = 279,
    WEBKIT_MEDIAQUERY_SYM = 280,
    IMPORTANT_SYM = 281,
    MEDIA_ONLY = 282,
    MEDIA_NOT = 283,
    MEDIA_AND = 284,
    QEMS = 285,
    EMS = 286,
    EXS = 287,
    PXS = 288,
    CMS = 289,
    MMS = 290,
    INS = 291,
    PTS = 292,
    PCS = 293,
    DEGS = 294,
    RADS = 295,
    GRADS = 296,
    MSECS = 297,
    SECS = 298,
    HERZ = 299,
    KHERZ = 300,
    DIMEN = 301,
    PERCENTAGE = 302,
    FLOATTOKEN = 303,
    INTEGER = 304,
    URI = 305,
    FUNCTION = 306,
    NOTFUNCTION = 307,
    ISFUNCTION = 308,
    WHEREFUNCTION = 309,
    HASFUNCTION = 310,
    UNICODERANGE = 311
  };
#endif

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
union YYSTYPE
{
#line 125 "CSSGrammar.y"

    CSSRule* rule;
    CSSSelector* selector;
    bool ok;
    MediaList *mediaList;
    CSSMediaRule* mediaRule;
    CSSRuleList* ruleList;
    ParseString string;
    float val;
    int prop_id;
    int attribute;
    CSSSelector::Relation relation;
    bool b;
    int i;
    char tok;
    Value value;
    ValueList* valueList;

    MediaQuery* mediaQuery;
    MediaQueryExp* mediaQueryExp;
    Vector<MediaQueryExp*>* mediaQueryExpList;
    MediaQuery::Restrictor mediaQueryRestrictor;

#line 328 "CSSGrammar.tab.c"

};
typedef union YYSTYPE YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif



int cssyyparse (void* parser);

#endif /* !YY_CSSYY_CSSGRAMMAR_TAB_H_INCLUDED  */

/* Second part of user prologue.  */
#line 149 "CSSGrammar.y"


static inline int cssyyerror(void*, const char*) { return 1; }
static int cssyylex(YYSTYPE* yylval, void*) { return CSSParser::current()->lex(yylval); }


#line 350 "CSSGrammar.tab.c"


#ifdef short
# undef short
#endif

/* On compilers that do not define __PTRDIFF_MAX__ etc., make sure
   <limits.h> and (if available) <stdint.h> are included
   so that the code can choose integer types of a good width.  */

#ifndef __PTRDIFF_MAX__
# include <limits.h> /* INFRINGES ON USER NAME SPACE */
# if defined __STDC_VERSION__ && 199901 <= __STDC_VERSION__
#  include <stdint.h> /* INFRINGES ON USER NAME SPACE */
#  define YY_STDINT_H
# endif
#endif

/* Narrow types that promote to a signed type and that can represent a
   signed or unsigned integer of at least N bits.  In tables they can
   save space and decrease cache pressure.  Promoting to a signed type
   helps avoid bugs in integer arithmetic.  */

#ifdef __INT_LEAST8_MAX__
typedef __INT_LEAST8_TYPE__ yytype_int8;
#elif defined YY_STDINT_H
typedef int_least8_t yytype_int8;
#else
typedef signed char yytype_int8;
#endif

#ifdef __INT_LEAST16_MAX__
typedef __INT_LEAST16_TYPE__ yytype_int16;
#elif defined YY_STDINT_H
typedef int_least16_t yytype_int16;
#else
typedef short yytype_int16;
#endif

#if defined __UINT_LEAST8_MAX__ && __UINT_LEAST8_MAX__ <= __INT_MAX__
typedef __UINT_LEAST8_TYPE__ yytype_uint8;
#elif (!defined __UINT_LEAST8_MAX__ && defined YY_STDINT_H \
       && UINT_LEAST8_MAX <= INT_MAX)
typedef uint_least8_t yytype_uint8;
#elif !defined __UINT_LEAST8_MAX__ && UCHAR_MAX <= INT_MAX
typedef unsigned char yytype_uint8;
#else
typedef short yytype_uint8;
#endif

#if defined __UINT_LEAST16_MAX__ && __UINT_LEAST16_MAX__ <= __INT_MAX__
typedef __UINT_LEAST16_TYPE__ yytype_uint16;
#elif (!defined __UINT_LEAST16_MAX__ && defined YY_STDINT_H \
       && UINT_LEAST16_MAX <= INT_MAX)
typedef uint_least16_t yytype_uint16;
#elif !defined __UINT_LEAST16_MAX__ && USHRT_MAX <= INT_MAX
typedef unsigned short yytype_uint16;
#else
typedef int yytype_uint16;
#endif

#ifndef YYPTRDIFF_T
# if defined __PTRDIFF_TYPE__ && defined __PTRDIFF_MAX__
#  define YYPTRDIFF_T __PTRDIFF_TYPE__
#  define YYPTRDIFF_MAXIMUM __PTRDIFF_MAX__
# elif defined PTRDIFF_MAX
#  ifndef ptrdiff_t
#   include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  endif
#  define YYPTRDIFF_T ptrdiff_t
#  define YYPTRDIFF_MAXIMUM PTRDIFF_MAX
# else
#  define YYPTRDIFF_T long
#  define YYPTRDIFF_MAXIMUM LONG_MAX
# endif
#endif

#ifndef YYSIZE_T
# ifdef __SIZE_TYPE__
#  define YYSIZE_T __SIZE_TYPE__
# elif defined size_t
#  define YYSIZE_T size_t
# elif defined __STDC_VERSION__ && 199901 <= __STDC_VERSION__
#  include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  define YYSIZE_T size_t
# else
#  define YYSIZE_T unsigned
# endif
#endif

#define YYSIZE_MAXIMUM                                  \
  YY_CAST (YYPTRDIFF_T,                                 \
           (YYPTRDIFF_MAXIMUM < YY_CAST (YYSIZE_T, -1)  \
            ? YYPTRDIFF_MAXIMUM                         \
            : YY_CAST (YYSIZE_T, -1)))

#define YYSIZEOF(X) YY_CAST (YYPTRDIFF_T, sizeof (X))

/* Stored state numbers (used for stacks). */
typedef yytype_int16 yy_state_t;

/* State numbers in computations.  */
typedef int yy_state_fast_t;

#ifndef YY_
# if defined YYENABLE_NLS && YYENABLE_NLS
#  if ENABLE_NLS
#   include <libintl.h> /* INFRINGES ON USER NAME SPACE */
#   define YY_(Msgid) dgettext ("bison-runtime", Msgid)
#  endif
# endif
# ifndef YY_
#  define YY_(Msgid) Msgid
# endif
#endif

#ifndef YY_ATTRIBUTE_PURE
# if defined __GNUC__ && 2 < __GNUC__ + (96 <= __GNUC_MINOR__)
#  define YY_ATTRIBUTE_PURE __attribute__ ((__pure__))
# else
#  define YY_ATTRIBUTE_PURE
# endif
#endif

#ifndef YY_ATTRIBUTE_UNUSED
# if defined __GNUC__ && 2 < __GNUC__ + (7 <= __GNUC_MINOR__)
#  define YY_ATTRIBUTE_UNUSED __attribute__ ((__unused__))
# else
#  define YY_ATTRIBUTE_UNUSED
# endif
#endif

/* Suppress unused-variable warnings by "using" E.  */
#if ! defined lint || defined __GNUC__
# define YYUSE(E) ((void) (E))
#else
# define YYUSE(E) /* empty */
#endif

#if defined __GNUC__ && ! defined __ICC && 407 <= __GNUC__ * 100 + __GNUC_MINOR__
/* Suppress an incorrect diagnostic about yylval being uninitialized.  */
# define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN                            \
    _Pragma ("GCC diagnostic push")                                     \
    _Pragma ("GCC diagnostic ignored \"-Wuninitialized\"")              \
    _Pragma ("GCC diagnostic ignored \"-Wmaybe-uninitialized\"")
# define YY_IGNORE_MAYBE_UNINITIALIZED_END      \
    _Pragma ("GCC diagnostic pop")
#else
# define YY_INITIAL_VALUE(Value) Value
#endif
#ifndef YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
# define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
# define YY_IGNORE_MAYBE_UNINITIALIZED_END
#endif
#ifndef YY_INITIAL_VALUE
# define YY_INITIAL_VALUE(Value) /* Nothing. */
#endif

#if defined __cplusplus && defined __GNUC__ && ! defined __ICC && 6 <= __GNUC__
# define YY_IGNORE_USELESS_CAST_BEGIN                          \
    _Pragma ("GCC diagnostic push")                            \
    _Pragma ("GCC diagnostic ignored \"-Wuseless-cast\"")
# define YY_IGNORE_USELESS_CAST_END            \
    _Pragma ("GCC diagnostic pop")
#endif
#ifndef YY_IGNORE_USELESS_CAST_BEGIN
# define YY_IGNORE_USELESS_CAST_BEGIN
# define YY_IGNORE_USELESS_CAST_END
#endif


#define YY_ASSERT(E) ((void) (0 && (E)))

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
#    if ! defined _ALLOCA_H && ! defined EXIT_SUCCESS
#     include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
      /* Use EXIT_SUCCESS as a witness for stdlib.h.  */
#     ifndef EXIT_SUCCESS
#      define EXIT_SUCCESS 0
#     endif
#    endif
#   endif
#  endif
# endif

# ifdef YYSTACK_ALLOC
   /* Pacify GCC's 'empty if-body' warning.  */
#  define YYSTACK_FREE(Ptr) do { /* empty */; } while (0)
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
#  if (defined __cplusplus && ! defined EXIT_SUCCESS \
       && ! ((defined YYMALLOC || defined malloc) \
             && (defined YYFREE || defined free)))
#   include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#   ifndef EXIT_SUCCESS
#    define EXIT_SUCCESS 0
#   endif
#  endif
#  ifndef YYMALLOC
#   define YYMALLOC malloc
#   if ! defined malloc && ! defined EXIT_SUCCESS
void *malloc (YYSIZE_T); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
#  ifndef YYFREE
#   define YYFREE free
#   if ! defined free && ! defined EXIT_SUCCESS
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
  yy_state_t yyss_alloc;
  YYSTYPE yyvs_alloc;
};

/* The size of the maximum gap between one aligned stack and the next.  */
# define YYSTACK_GAP_MAXIMUM (YYSIZEOF (union yyalloc) - 1)

/* The size of an array large to enough to hold all stacks, each with
   N elements.  */
# define YYSTACK_BYTES(N) \
     ((N) * (YYSIZEOF (yy_state_t) + YYSIZEOF (YYSTYPE)) \
      + YYSTACK_GAP_MAXIMUM)

# define YYCOPY_NEEDED 1

/* Relocate STACK from its old location to the new one.  The
   local variables YYSIZE and YYSTACKSIZE give the old and new number of
   elements in the stack, and YYPTR gives the new location of the
   stack.  Advance YYPTR to a properly aligned location for the next
   stack.  */
# define YYSTACK_RELOCATE(Stack_alloc, Stack)                           \
    do                                                                  \
      {                                                                 \
        YYPTRDIFF_T yynewbytes;                                         \
        YYCOPY (&yyptr->Stack_alloc, Stack, yysize);                    \
        Stack = &yyptr->Stack_alloc;                                    \
        yynewbytes = yystacksize * YYSIZEOF (*Stack) + YYSTACK_GAP_MAXIMUM; \
        yyptr += yynewbytes / YYSIZEOF (*yyptr);                        \
      }                                                                 \
    while (0)

#endif

#if defined YYCOPY_NEEDED && YYCOPY_NEEDED
/* Copy COUNT objects from SRC to DST.  The source and destination do
   not overlap.  */
# ifndef YYCOPY
#  if defined __GNUC__ && 1 < __GNUC__
#   define YYCOPY(Dst, Src, Count) \
      __builtin_memcpy (Dst, Src, YY_CAST (YYSIZE_T, (Count)) * sizeof (*(Src)))
#  else
#   define YYCOPY(Dst, Src, Count)              \
      do                                        \
        {                                       \
          YYPTRDIFF_T yyi;                      \
          for (yyi = 0; yyi < (Count); yyi++)   \
            (Dst)[yyi] = (Src)[yyi];            \
        }                                       \
      while (0)
#  endif
# endif
#endif /* !YYCOPY_NEEDED */

/* YYFINAL -- State number of the termination state.  */
#define YYFINAL  19
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   991

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  78
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  71
/* YYNRULES -- Number of rules.  */
#define YYNRULES  215
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  437

#define YYUNDEFTOK  2
#define YYMAXUTOK   311


/* YYTRANSLATE(TOKEN-NUM) -- Symbol number corresponding to TOKEN-NUM
   as returned by yylex, with out-of-bounds checking.  */
#define YYTRANSLATE(YYX)                                                \
  (0 <= (YYX) && (YYX) <= YYMAXUTOK ? yytranslate[YYX] : YYUNDEFTOK)

/* YYTRANSLATE[TOKEN-NUM] -- Symbol number corresponding to TOKEN-NUM
   as returned by yylex.  */
static const yytype_int8 yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,    75,     2,    76,     2,     2,
      65,    66,    18,    68,    67,    71,    16,    74,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,    15,    64,
       2,    73,    70,     2,    77,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,    17,     2,    72,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,    62,    19,    63,    69,     2,     2,     2,
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
      20,    21,    22,    23,    24,    25,    26,    27,    28,    29,
      30,    31,    32,    33,    34,    35,    36,    37,    38,    39,
      40,    41,    42,    43,    44,    45,    46,    47,    48,    49,
      50,    51,    52,    53,    54,    55,    56,    57,    58,    59,
      60,    61
};

#if YYDEBUG
  /* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
static const yytype_int16 yyrline[] =
{
       0,   297,   297,   298,   299,   300,   301,   305,   306,   310,
     316,   322,   336,   343,   344,   347,   349,   350,   353,   355,
     360,   366,   368,   372,   374,   381,   383,   386,   388,   396,
     397,   398,   399,   400,   401,   402,   403,   407,   410,   413,
     419,   424,   425,   429,   430,   434,   435,   439,   445,   448,
     454,   461,   466,   473,   476,   480,   483,   486,   492,   500,
     503,   507,   512,   517,   523,   526,   532,   533,   544,   561,
     564,   570,   574,   577,   583,   586,   592,   593,   596,   598,
     602,   608,   609,   615,   616,   626,   627,   628,   629,   633,
     634,   638,   644,   647,   655,   661,   664,   684,   690,   691,
     692,   696,   701,   708,   714,   725,   738,   752,   760,   768,
     771,   784,   790,   799,   812,   813,   814,   818,   830,   841,
     846,   852,   861,   874,   877,   880,   883,   886,   889,   895,
     896,   900,   925,   940,   950,   960,   971,   980,   990,  1007,
    1018,  1029,  1045,  1048,  1053,  1056,  1059,  1062,  1068,  1071,
    1074,  1077,  1080,  1085,  1088,  1094,  1108,  1117,  1121,  1128,
    1133,  1140,  1159,  1160,  1164,  1169,  1183,  1189,  1192,  1195,
    1198,  1201,  1204,  1210,  1211,  1212,  1213,  1221,  1222,  1223,
    1224,  1225,  1226,  1228,  1231,  1235,  1236,  1237,  1238,  1239,
    1240,  1241,  1242,  1243,  1244,  1245,  1246,  1247,  1248,  1249,
    1250,  1251,  1252,  1253,  1258,  1267,  1283,  1284,  1291,  1294,
    1300,  1306,  1323,  1324,  1328,  1329
};
#endif

#if YYDEBUG || YYERROR_VERBOSE || 0
/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "$end", "error", "$undefined", "UNIMPORTANT_TOK", "WHITESPACE",
  "SGML_CD", "INCLUDES", "DASHMATCH", "BEGINSWITH", "ENDSWITH", "CONTAINS",
  "STRING", "IDENT", "HEX", "IDSEL", "':'", "'.'", "'['", "'*'", "'|'",
  "IMPORT_SYM", "PAGE_SYM", "MEDIA_SYM", "FONT_FACE_SYM", "KEYFRAMES_SYM",
  "CHARSET_SYM", "NAMESPACE_SYM", "WEBKIT_RULE_SYM", "WEBKIT_DECLS_SYM",
  "WEBKIT_VALUE_SYM", "WEBKIT_MEDIAQUERY_SYM", "IMPORTANT_SYM",
  "MEDIA_ONLY", "MEDIA_NOT", "MEDIA_AND", "QEMS", "EMS", "EXS", "PXS",
  "CMS", "MMS", "INS", "PTS", "PCS", "DEGS", "RADS", "GRADS", "MSECS",
  "SECS", "HERZ", "KHERZ", "DIMEN", "PERCENTAGE", "FLOATTOKEN", "INTEGER",
  "URI", "FUNCTION", "NOTFUNCTION", "ISFUNCTION", "WHEREFUNCTION",
  "HASFUNCTION", "UNICODERANGE", "'{'", "'}'", "';'", "'('", "')'", "','",
  "'+'", "'~'", "'>'", "'-'", "']'", "'='", "'/'", "'#'", "'%'", "'@'",
  "$accept", "stylesheet", "ruleset_or_import", "webkit_rule",
  "webkit_decls", "webkit_value", "webkit_mediaquery", "maybe_space",
  "maybe_sgml", "maybe_charset", "charset", "import_list",
  "namespace_list", "rule_list", "rule", "import", "namespace",
  "maybe_ns_prefix", "string_or_uri", "media_feature", "maybe_media_value",
  "media_query_exp", "media_query_exp_list", "maybe_media_query_exp_list",
  "maybe_media_restrictor", "media_query", "maybe_media_list",
  "media_list", "media", "ruleset_list", "medium", "page", "font_face",
  "keyframes", "keyframe_name", "keyframe_rules", "keyframe_rule",
  "key_list", "key", "combinator", "unary_operator", "ruleset",
  "selector_list", "selector", "namespace_selector", "simple_selector",
  "element_name", "specifier_list", "specifier", "class", "attr_name",
  "attrib", "match", "ident_or_string", "pseudo", "declaration_list",
  "decl_list", "declaration", "property", "prio", "expr", "operator",
  "term", "unary_term", "function", "hexcolor", "invalid_at",
  "invalid_import", "invalid_rule", "invalid_block", "invalid_block_list", YY_NULLPTR
};
#endif

# ifdef YYPRINT
/* YYTOKNUM[NUM] -- (External) token number corresponding to the
   (internal) symbol number NUM (which must be that of a token).  */
static const yytype_int16 yytoknum[] =
{
       0,   256,   257,   258,   259,   260,   261,   262,   263,   264,
     265,   266,   267,   268,   269,    58,    46,    91,    42,   124,
     270,   271,   272,   273,   274,   275,   276,   277,   278,   279,
     280,   281,   282,   283,   284,   285,   286,   287,   288,   289,
     290,   291,   292,   293,   294,   295,   296,   297,   298,   299,
     300,   301,   302,   303,   304,   305,   306,   307,   308,   309,
     310,   311,   123,   125,    59,    40,    41,    44,    43,   126,
      62,    45,    93,    61,    47,    35,    37,    64
};
# endif

#define YYPACT_NINF (-221)

#define yypact_value_is_default(Yyn) \
  ((Yyn) == YYPACT_NINF)

#define YYTABLE_NINF (-173)

#define yytable_value_is_error(Yyn) \
  0

  /* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
     STATE-NUM.  */
static const yytype_int16 yypact[] =
{
     293,   206,   -36,   -20,   -14,   137,   153,  -221,  -221,  -221,
    -221,  -221,  -221,   -41,    33,  -221,  -221,  -221,  -221,  -221,
     164,   164,   164,   164,   140,   171,  -221,  -221,  -221,  -221,
     546,    29,   790,    31,  -221,  -221,   165,   178,   132,   169,
    -221,  -221,   284,   203,  -221,   201,  -221,   276,  -221,  -221,
    -221,    65,   855,   371,  -221,   319,   752,  -221,  -221,  -221,
    -221,   117,  -221,  -221,   260,   216,    98,   220,    21,  -221,
    -221,  -221,  -221,  -221,  -221,  -221,  -221,  -221,  -221,  -221,
    -221,  -221,  -221,  -221,  -221,  -221,  -221,  -221,  -221,  -221,
    -221,  -221,  -221,  -221,  -221,  -221,  -221,  -221,  -221,  -221,
     583,   535,  -221,  -221,  -221,  -221,  -221,  -221,  -221,   222,
     198,  -221,  -221,  -221,   287,  -221,  -221,  -221,   281,   290,
    -221,  -221,  -221,  -221,  -221,   288,  -221,   252,   197,   148,
    -221,  -221,  -221,  -221,  -221,  -221,  -221,   676,  -221,  -221,
     319,   752,   164,   752,  -221,   164,  -221,  -221,   329,   164,
     164,   323,  -221,   263,   246,  -221,    44,  -221,  -221,   164,
     164,   164,   164,   164,   164,   164,   164,   164,   164,   164,
     164,   164,   164,   164,   164,   164,   164,   164,   164,   164,
     164,   164,   164,   164,   737,   164,   164,   164,  -221,  -221,
    -221,  -221,  -221,  -221,  -221,  -221,  -221,   915,   217,  -221,
     231,   247,  -221,   140,   292,  -221,   280,   176,   390,   390,
     390,   390,   169,   201,   338,    43,  -221,  -221,  -221,  -221,
    -221,  -221,    29,   390,   164,   164,   164,  -221,   164,   752,
     164,   164,   164,   264,  -221,  -221,   350,  -221,   164,   296,
    -221,   603,  -221,   669,   164,   164,   164,   164,   164,   164,
    -221,  -221,   333,   285,   269,   296,   374,  -221,   161,   297,
     375,  -221,  -221,  -221,  -221,  -221,  -221,  -221,  -221,  -221,
    -221,   140,  -221,  -221,  -221,  -221,  -221,   312,   100,   111,
     116,   164,  -221,   141,  -221,  -221,  -221,  -221,  -221,  -221,
    -221,  -221,   212,   316,   874,   164,  -221,   790,   164,   295,
     164,   164,   915,   399,  -221,   164,  -221,  -221,   333,  -221,
    -221,  -221,   368,     1,  -221,   301,   193,   306,   156,   296,
      28,   309,   140,   105,    92,  -221,  -221,  -221,  -221,  -221,
    -221,   300,  -221,   317,   136,  -221,   164,   399,  -221,   399,
    -221,  -221,   164,     0,  -221,  -221,  -221,  -221,  -221,  -221,
     142,  -221,  -221,  -221,  -221,  -221,  -221,  -221,  -221,  -221,
    -221,  -221,  -221,  -221,   300,  -221,  -221,  -221,  -221,  -221,
    -221,  -221,   164,  -221,   164,  -221,   134,   164,  -221,    29,
     160,   151,   152,  -221,    30,    31,   279,  -221,   474,   164,
     330,  -221,  -221,  -221,    34,  -221,  -221,  -221,  -221,  -221,
    -221,   482,  -221,   164,   109,   120,  -221,   164,    16,   164,
    -221,   164,   226,  -221,  -221,  -221,   314,  -221,  -221,  -221,
    -221,  -221,  -221,   790,  -221,   164,   122,   467,   164,  -221,
    -221,   164,    29,   242,   332,  -221,  -221
};

  /* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
     Performed when YYTABLE does not specify something else to do.  Zero
     means the default is an error.  */
static const yytype_uint8 yydefact[] =
{
      18,     0,     0,     0,     0,     0,     0,    13,    13,    13,
      13,    15,    19,     0,     0,    13,    13,    13,    13,     1,
       3,     4,     5,     6,    23,     0,    22,    21,    14,    13,
       0,     0,     0,    55,    17,    16,    25,     0,     0,   107,
     113,   112,     0,     0,    13,   108,    98,     0,    13,     8,
       7,     0,     0,     0,    95,    13,     0,   109,   114,   115,
     116,   146,    13,    13,     0,     0,     0,   142,     0,    13,
      13,    13,    13,    13,    13,    13,    13,    13,    13,    13,
      13,    13,    13,    13,    13,    13,    13,    13,    13,    13,
      13,    13,    13,    13,    13,    13,    90,    89,    13,    13,
       0,     0,   164,   173,   183,   181,    56,    57,    13,     0,
      27,    15,   213,   214,     0,    20,   100,   131,     0,    13,
      13,    13,    13,    13,   117,     0,    99,     0,     0,     0,
      94,    13,    13,    97,    13,    13,    13,     0,   107,   108,
      13,     0,   101,     0,   111,   103,   110,    13,     0,   161,
     159,     0,    10,   147,   143,    13,     0,   157,    13,   175,
     176,   206,   207,   202,   201,   203,   188,   189,   190,   191,
     192,   193,   194,   195,   196,   197,   198,   199,   200,   177,
     187,   186,   185,   179,     0,   180,   182,   184,    13,   174,
     166,    13,    11,    13,    13,    13,    13,     0,     0,    12,
       0,     0,    15,    24,     0,   132,     0,     0,     0,     0,
       0,     0,    13,     0,     0,     0,    39,    38,    45,    46,
      13,     9,     0,     0,    85,    86,    87,    96,   104,     0,
     106,   102,   150,   145,    13,    13,     0,    13,   148,     0,
      13,     0,   205,     0,   178,   171,   168,   169,   170,   167,
     165,    13,    53,     0,    43,     0,     0,    13,     0,     0,
       0,    15,   210,    30,    31,    32,    33,    29,    35,    36,
      34,    26,   212,   215,   133,    13,    13,     0,     0,     0,
       0,   118,    13,     0,   124,   125,   126,   127,   128,   119,
     123,    13,    55,     0,     0,   105,    13,     0,   153,     0,
     152,   149,     0,     0,    13,    68,    13,    51,    54,    58,
      42,    41,     0,     0,   211,     0,    55,     0,     0,     0,
       0,     0,    28,     0,     0,   138,   139,   140,   141,   121,
      13,     0,    61,     0,     0,    91,   151,     0,    13,     0,
      13,   155,   204,     0,    52,    44,    13,    70,    69,    13,
       0,    73,    72,    13,    75,    77,    76,    13,   209,   208,
     135,    13,    13,   134,     0,   130,   129,    13,    37,    63,
      13,   156,   154,   158,   162,    13,     0,    66,    13,     0,
       0,     0,     0,    13,     0,    55,     0,    40,     0,    66,
       0,    13,    13,    13,     0,   120,    62,    13,    13,    65,
      13,     0,    13,    78,     0,     0,   122,    47,    48,    67,
      64,    71,     0,   136,   137,    13,     0,    84,    83,    74,
      13,    13,    81,     0,    13,    79,     0,     0,    50,    13,
      13,    49,     0,     0,     0,    82,    80
};

  /* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -221,  -221,  -221,  -221,  -221,  -221,  -221,    -1,   -98,  -221,
    -221,  -221,  -221,  -221,  -221,   -31,  -221,  -221,    85,  -221,
    -221,   107,  -221,  -221,  -221,   -32,  -221,   102,  -221,    27,
    -221,  -221,  -221,  -221,  -221,  -221,  -221,  -221,   -12,  -221,
    -221,   -27,  -152,   199,   299,  -113,   372,   -26,   -38,  -221,
     213,  -221,   149,    67,  -221,  -220,  -221,   367,  -221,    25,
    -174,  -221,   259,   357,  -221,  -221,  -221,  -221,  -221,    -2,
     -42
};

  /* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
      -1,     6,    48,     7,     8,     9,    10,   281,    24,    11,
      12,    36,   110,   201,   261,    49,   202,   313,   220,   398,
     416,   307,   308,   309,   108,   332,   333,   334,   263,   388,
     252,   264,   265,   266,   357,   412,   420,   421,   422,   137,
     100,   400,    51,    52,    53,    54,    55,    56,    57,    58,
     215,    59,   291,   367,    60,    65,    66,    67,    68,   341,
     101,   197,   102,   103,   104,   105,   268,   269,   270,   113,
     114
};

  /* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
     positive, shift that token.  If negative, reduce the rule whose
     number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int16 yytable[] =
{
      14,   109,   293,    50,    28,   111,    20,    21,    22,    23,
     243,    27,   218,   203,    30,    31,    32,    33,   146,   148,
      28,    25,   157,    26,   227,   156,    15,   141,    38,   143,
      61,   415,    28,    28,    28,    28,   158,    28,    28,   355,
     356,    62,    16,   125,    29,   239,   128,   129,    17,   284,
     285,   286,   287,   288,   142,   145,   219,   278,   279,   280,
      63,   149,   150,   106,   107,   375,   130,   303,   159,   160,
     161,   162,   163,   164,   165,   166,   167,   168,   169,   170,
     171,   172,   173,   174,   175,   176,   177,   178,   179,   180,
     181,   182,   183,   184,   185,   277,    28,   186,   187,   153,
      64,   130,   395,   146,   271,   146,   406,   198,   240,    28,
      62,   236,   130,    28,   229,   289,   290,   130,   207,   208,
     209,   210,   211,   337,    28,   217,    28,   131,   339,    63,
     222,   223,   132,   224,   225,   226,    28,   369,    28,   228,
     230,    18,   231,   369,    34,    35,   232,   284,   285,   286,
     287,   288,    28,    19,   238,    28,    28,   241,   363,   390,
      28,  -144,   317,   322,    28,   -13,   326,   132,    28,    64,
     262,   360,    37,   361,   267,   413,   362,   327,   132,    25,
      28,   147,   328,   132,   429,    47,   414,   244,   116,   430,
     245,   146,   246,   247,   248,   249,   115,    28,   387,   254,
     -60,    28,   273,   370,   378,   392,   393,    13,   218,   370,
     -13,   221,   434,   329,   290,   124,    28,   -13,   353,   292,
     126,    28,   391,   -13,   200,   106,   107,   275,   295,   251,
     276,   273,   253,   297,   298,   -13,   300,   273,   417,   301,
      25,   112,   -13,   -13,   106,   107,    28,    -2,   255,   427,
     305,   311,   219,   314,   417,   349,   316,   318,   320,    39,
      40,    41,    42,    43,    44,    45,    46,    47,   256,   257,
     258,   259,   151,    28,   323,   324,   -59,   127,   418,   152,
     -13,   312,    25,    28,   155,   199,   -13,   -13,   204,   419,
     331,   397,    28,   205,   418,   336,   117,   273,   319,   118,
     212,   -13,   206,   342,    28,   343,   213,    46,   -13,   -13,
     237,   365,   366,   348,    25,   352,   216,   354,     1,   359,
       2,     3,     4,     5,   260,    25,    25,   235,   296,   364,
     233,   -13,    40,    41,    42,    43,    44,   372,   234,   374,
     119,   120,   121,   122,   123,   376,   274,    25,   377,   310,
     282,   299,   379,   396,    25,   272,   380,    25,    25,   338,
     381,   382,   371,    25,   373,   347,   384,   306,    25,   385,
     351,    25,   345,   358,   386,   315,   321,   389,   325,   335,
     424,   368,   394,   138,    40,    41,    42,    43,    44,   139,
     403,   404,   405,   402,    28,   436,   407,   408,   346,   409,
     190,   411,    39,    40,    41,    42,    43,    44,    45,    46,
    -172,  -172,  -172,  -172,   423,   344,   401,   191,   350,   425,
     426,   435,   294,   428,   214,   140,   431,   283,   432,   433,
     340,   383,   330,   154,  -172,  -172,  -172,  -172,  -172,  -172,
    -172,  -172,  -172,  -172,  -172,  -172,  -172,  -172,  -172,  -172,
    -172,  -172,  -172,  -172,  -172,  -172,   250,   189,     0,     0,
    -172,  -163,  -163,  -163,     0,     0,   193,   194,   190,     0,
     195,   -13,     0,   196,  -172,  -172,     0,     0,  -172,  -172,
    -172,  -172,     0,     0,     0,   191,    39,    40,    41,    42,
      43,    44,    45,    46,    39,    40,    41,    42,    43,    44,
      45,    46,  -172,  -172,  -172,  -172,  -172,  -172,  -172,  -172,
    -172,  -172,  -172,  -172,  -172,  -172,  -172,  -172,  -172,  -172,
    -172,  -172,  -172,  -172,     0,     0,     0,     0,  -172,     0,
       0,     0,     0,   -13,   193,   194,   190,   399,   195,     0,
       0,   196,  -172,  -172,     0,   410,  -172,  -172,  -172,  -172,
      28,     0,     0,   191,     0,     0,     0,     0,    39,    40,
      41,    42,    43,    44,    45,    46,    47,     0,     0,     0,
    -172,  -172,  -172,  -172,  -172,  -172,  -172,  -172,  -172,  -172,
    -172,  -172,  -172,  -172,  -172,  -172,  -172,  -172,  -172,  -172,
    -172,  -172,     0,     0,     0,     0,  -172,     0,   192,     0,
       0,     0,   193,   194,   302,     0,   195,    28,     0,   196,
    -172,  -172,     0,     0,    69,    70,    71,    72,    73,    74,
      75,    76,    77,    78,    79,    80,    81,    82,    83,    84,
      85,    86,    87,    88,   188,    90,    91,    92,    73,    74,
      75,    76,    77,    78,    79,    80,    81,    82,    83,    84,
      85,    86,    87,    88,    89,    90,    91,    92,    93,    94,
       0,     0,     0,     0,    95,  -160,  -160,  -160,     0,     0,
     190,    96,     0,     0,    97,     0,     0,     0,    98,    99,
    -172,  -172,  -172,  -172,     0,     0,     0,   191,    39,    40,
      41,    42,    43,    44,    45,    46,     0,     0,     0,     0,
       0,     0,     0,     0,  -172,  -172,  -172,  -172,  -172,  -172,
    -172,  -172,  -172,  -172,  -172,  -172,  -172,  -172,  -172,  -172,
    -172,  -172,  -172,  -172,  -172,  -172,     0,     0,     0,     0,
    -172,     0,     0,     0,     0,   304,   193,   194,   242,     0,
     195,    28,     0,   196,  -172,  -172,     0,     0,    69,    70,
      71,    72,     0,   144,     0,     0,   -13,     0,     0,     0,
       0,     0,     0,     0,   -13,    40,    41,    42,    43,    44,
     -13,   -13,    73,    74,    75,    76,    77,    78,    79,    80,
      81,    82,    83,    84,    85,    86,    87,    88,    89,    90,
      91,    92,    93,    94,    28,     0,     0,     0,    95,     0,
       0,    69,    70,    71,    72,    96,     0,     0,    97,     0,
       0,     0,    98,    99,   -13,     0,     0,     0,   -13,   -13,
     -13,   -13,   -13,     0,     0,    73,    74,    75,    76,    77,
      78,    79,    80,    81,    82,    83,    84,    85,    86,    87,
      88,    89,    90,    91,    92,    93,    94,     0,     0,     0,
       0,    95,     0,     0,     0,     0,   133,     0,    96,     0,
       0,    97,     0,     0,     0,    98,    99,   -88,   -88,   -88,
     -88,   -88,   -88,   -88,   -88,   133,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   -88,   -88,   -88,   -88,
     -88,   -88,   -88,   -88,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   -92,     0,     0,
       0,   -92,   -92,   134,   135,   136,    69,    70,    71,    72,
       0,     0,     0,     0,     0,     0,   -93,     0,     0,     0,
     -93,   -93,   134,   135,   136,     0,     0,     0,     0,     0,
      73,    74,    75,    76,    77,    78,    79,    80,    81,    82,
      83,    84,    85,    86,    87,    88,    89,    90,    91,    92,
      93,    94,     0,     0,     0,     0,    95,     0,     0,     0,
       0,     0,     0,    96,     0,     0,    97,     0,     0,     0,
      98,    99
};

static const yytype_int16 yycheck[] =
{
       1,    33,   222,    30,     4,    36,     7,     8,     9,    10,
     184,    13,    11,   111,    15,    16,    17,    18,    56,    61,
       4,    62,     1,    64,   137,    67,    62,    53,    29,    55,
       1,    15,     4,     4,     4,     4,    15,     4,     4,    11,
      12,    12,    62,    44,    11,     1,    47,    48,    62,     6,
       7,     8,     9,    10,    55,    56,    55,   209,   210,   211,
      31,    62,    63,    32,    33,    65,     1,   241,    69,    70,
      71,    72,    73,    74,    75,    76,    77,    78,    79,    80,
      81,    82,    83,    84,    85,    86,    87,    88,    89,    90,
      91,    92,    93,    94,    95,   208,     4,    98,    99,     1,
      71,     1,    72,   141,   202,   143,    72,   108,    64,     4,
      12,   153,     1,     4,   140,    72,    73,     1,   119,   120,
     121,   122,   123,   297,     4,   127,     4,    62,   302,    31,
     131,   132,    67,   134,   135,   136,     4,     1,     4,   140,
     141,     4,   143,     1,     4,     5,   147,     6,     7,     8,
       9,    10,     4,     0,   155,     4,     4,   158,    66,   379,
       4,    63,     1,   261,     4,     4,    66,    67,     4,    71,
     201,    66,     1,    68,   201,    66,    71,    66,    67,    62,
       4,    64,    66,    67,    62,    20,    66,   188,    19,    67,
     191,   229,   193,   194,   195,   196,    64,     4,    64,   200,
      64,     4,   204,    67,    62,    54,    54,     1,    11,    67,
       4,    63,   432,    72,    73,    12,     4,    11,    62,   220,
      19,     4,    62,    62,    26,    32,    33,    51,   229,    12,
      54,   233,     1,   234,   235,     4,   237,   239,    12,   240,
      62,    63,    11,    12,    32,    33,     4,     0,     1,   423,
     251,   253,    55,   255,    12,    62,   257,   258,   259,    12,
      13,    14,    15,    16,    17,    18,    19,    20,    21,    22,
      23,    24,    12,     4,   275,   276,    64,     1,    52,    63,
       4,    12,    62,     4,    64,    63,    55,    11,     1,    63,
     291,    12,     4,    12,    52,   296,    12,   299,     1,    15,
      12,     4,    12,   304,     4,   306,    18,    19,    11,    12,
      64,    11,    12,   315,    62,   317,    64,   319,    25,   321,
      27,    28,    29,    30,    77,    62,    62,    64,    64,   330,
       1,    55,    13,    14,    15,    16,    17,   338,    15,   340,
      56,    57,    58,    59,    60,   346,    66,    62,   349,    64,
      12,     1,   353,   385,    62,    63,   357,    62,    62,    64,
     361,   362,   337,    62,   339,    64,   367,    34,    62,   370,
      64,    62,     4,    64,   375,     1,     1,   378,    66,    63,
      66,    64,   383,    12,    13,    14,    15,    16,    17,    18,
     391,   392,   393,    63,     4,    63,   397,   398,   313,   400,
       1,   402,    12,    13,    14,    15,    16,    17,    18,    19,
      11,    12,    13,    14,   415,   308,   389,    18,   316,   420,
     421,   433,   223,   424,   125,    53,   427,   214,   429,   430,
      31,   364,   283,    66,    35,    36,    37,    38,    39,    40,
      41,    42,    43,    44,    45,    46,    47,    48,    49,    50,
      51,    52,    53,    54,    55,    56,   197,   100,    -1,    -1,
      61,    62,    63,    64,    -1,    -1,    67,    68,     1,    -1,
      71,     4,    -1,    74,    75,    76,    -1,    -1,    11,    12,
      13,    14,    -1,    -1,    -1,    18,    12,    13,    14,    15,
      16,    17,    18,    19,    12,    13,    14,    15,    16,    17,
      18,    19,    35,    36,    37,    38,    39,    40,    41,    42,
      43,    44,    45,    46,    47,    48,    49,    50,    51,    52,
      53,    54,    55,    56,    -1,    -1,    -1,    -1,    61,    -1,
      -1,    -1,    -1,    66,    67,    68,     1,    63,    71,    -1,
      -1,    74,    75,    76,    -1,    63,    11,    12,    13,    14,
       4,    -1,    -1,    18,    -1,    -1,    -1,    -1,    12,    13,
      14,    15,    16,    17,    18,    19,    20,    -1,    -1,    -1,
      35,    36,    37,    38,    39,    40,    41,    42,    43,    44,
      45,    46,    47,    48,    49,    50,    51,    52,    53,    54,
      55,    56,    -1,    -1,    -1,    -1,    61,    -1,    63,    -1,
      -1,    -1,    67,    68,     1,    -1,    71,     4,    -1,    74,
      75,    76,    -1,    -1,    11,    12,    13,    14,    35,    36,
      37,    38,    39,    40,    41,    42,    43,    44,    45,    46,
      47,    48,    49,    50,    51,    52,    53,    54,    35,    36,
      37,    38,    39,    40,    41,    42,    43,    44,    45,    46,
      47,    48,    49,    50,    51,    52,    53,    54,    55,    56,
      -1,    -1,    -1,    -1,    61,    62,    63,    64,    -1,    -1,
       1,    68,    -1,    -1,    71,    -1,    -1,    -1,    75,    76,
      11,    12,    13,    14,    -1,    -1,    -1,    18,    12,    13,
      14,    15,    16,    17,    18,    19,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    35,    36,    37,    38,    39,    40,
      41,    42,    43,    44,    45,    46,    47,    48,    49,    50,
      51,    52,    53,    54,    55,    56,    -1,    -1,    -1,    -1,
      61,    -1,    -1,    -1,    -1,    66,    67,    68,     1,    -1,
      71,     4,    -1,    74,    75,    76,    -1,    -1,    11,    12,
      13,    14,    -1,     1,    -1,    -1,     4,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    12,    13,    14,    15,    16,    17,
      18,    19,    35,    36,    37,    38,    39,    40,    41,    42,
      43,    44,    45,    46,    47,    48,    49,    50,    51,    52,
      53,    54,    55,    56,     4,    -1,    -1,    -1,    61,    -1,
      -1,    11,    12,    13,    14,    68,    -1,    -1,    71,    -1,
      -1,    -1,    75,    76,    62,    -1,    -1,    -1,    66,    67,
      68,    69,    70,    -1,    -1,    35,    36,    37,    38,    39,
      40,    41,    42,    43,    44,    45,    46,    47,    48,    49,
      50,    51,    52,    53,    54,    55,    56,    -1,    -1,    -1,
      -1,    61,    -1,    -1,    -1,    -1,     1,    -1,    68,    -1,
      -1,    71,    -1,    -1,    -1,    75,    76,    12,    13,    14,
      15,    16,    17,    18,    19,     1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    12,    13,    14,    15,
      16,    17,    18,    19,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    62,    -1,    -1,
      -1,    66,    67,    68,    69,    70,    11,    12,    13,    14,
      -1,    -1,    -1,    -1,    -1,    -1,    62,    -1,    -1,    -1,
      66,    67,    68,    69,    70,    -1,    -1,    -1,    -1,    -1,
      35,    36,    37,    38,    39,    40,    41,    42,    43,    44,
      45,    46,    47,    48,    49,    50,    51,    52,    53,    54,
      55,    56,    -1,    -1,    -1,    -1,    61,    -1,    -1,    -1,
      -1,    -1,    -1,    68,    -1,    -1,    71,    -1,    -1,    -1,
      75,    76
};

  /* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
     symbol of state STATE-NUM.  */
static const yytype_uint8 yystos[] =
{
       0,    25,    27,    28,    29,    30,    79,    81,    82,    83,
      84,    87,    88,     1,    85,    62,    62,    62,     4,     0,
      85,    85,    85,    85,    86,    62,    64,   147,     4,    11,
      85,    85,    85,    85,     4,     5,    89,     1,    85,    12,
      13,    14,    15,    16,    17,    18,    19,    20,    80,    93,
     119,   120,   121,   122,   123,   124,   125,   126,   127,   129,
     132,     1,    12,    31,    71,   133,   134,   135,   136,    11,
      12,    13,    14,    35,    36,    37,    38,    39,    40,    41,
      42,    43,    44,    45,    46,    47,    48,    49,    50,    51,
      52,    53,    54,    55,    56,    61,    68,    71,    75,    76,
     118,   138,   140,   141,   142,   143,    32,    33,   102,   103,
      90,    93,    63,   147,   148,    64,    19,    12,    15,    56,
      57,    58,    59,    60,    12,    85,    19,     1,    85,    85,
       1,    62,    67,     1,    68,    69,    70,   117,    12,    18,
     124,   125,    85,   125,     1,    85,   126,    64,   148,    85,
      85,    12,    63,     1,   135,    64,   148,     1,    15,    85,
      85,    85,    85,    85,    85,    85,    85,    85,    85,    85,
      85,    85,    85,    85,    85,    85,    85,    85,    85,    85,
      85,    85,    85,    85,    85,    85,    85,    85,    51,   141,
       1,    18,    63,    67,    68,    71,    74,   139,    85,    63,
      26,    91,    94,    86,     1,    12,    12,    85,    85,    85,
      85,    85,    12,    18,   122,   128,    64,   147,    11,    55,
      96,    63,    85,    85,    85,    85,    85,   123,    85,   125,
      85,    85,    85,     1,    15,    64,   148,    64,    85,     1,
      64,    85,     1,   138,    85,    85,    85,    85,    85,    85,
     140,    12,   108,     1,    85,     1,    21,    22,    23,    24,
      77,    92,    93,   106,   109,   110,   111,   119,   144,   145,
     146,    86,    63,   147,    66,    51,    54,   123,   120,   120,
     120,    85,    12,   128,     6,     7,     8,     9,    10,    72,
      73,   130,    85,   133,   121,    85,    64,    85,    85,     1,
      85,    85,     1,   138,    66,    85,    34,    99,   100,   101,
      64,   147,    12,    95,   147,     1,    85,     1,    85,     1,
      85,     1,    86,    85,    85,    66,    66,    66,    66,    72,
     130,    85,   103,   104,   105,    63,    85,   138,    64,   138,
      31,   137,    85,    85,    99,     4,    96,    64,   147,    62,
     105,    64,   147,    62,   147,    11,    12,   112,    64,   147,
      66,    68,    71,    66,    85,    11,    12,   131,    64,     1,
      67,   137,    85,   137,    85,    65,    85,    85,    62,    85,
      85,    85,    85,   131,    85,    85,    85,    64,   107,    85,
     133,    62,    54,    54,    85,    72,   103,    12,    97,    63,
     119,   107,    63,    85,    85,    85,    72,    85,    85,    85,
      63,    85,   113,    66,    66,    15,    98,    12,    52,    63,
     114,   115,   116,    85,    66,    85,    85,   138,    85,    62,
      67,    85,    85,    85,   133,   116,    63
};

  /* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_uint8 yyr1[] =
{
       0,    78,    79,    79,    79,    79,    79,    80,    80,    81,
      82,    83,    84,    85,    85,    86,    86,    86,    87,    87,
      88,    88,    88,    89,    89,    90,    90,    91,    91,    92,
      92,    92,    92,    92,    92,    92,    92,    93,    93,    93,
      94,    94,    94,    95,    95,    96,    96,    97,    98,    98,
      99,   100,   100,   101,   101,   102,   102,   102,   103,   104,
     104,   105,   105,   105,   106,   106,   107,   107,   108,   109,
     109,   110,   110,   110,   111,   111,   112,   112,   113,   113,
     114,   115,   115,   116,   116,   117,   117,   117,   117,   118,
     118,   119,   120,   120,   120,   121,   121,   121,   122,   122,
     122,   123,   123,   123,   123,   123,   123,   124,   124,   125,
     125,   125,   126,   126,   126,   126,   126,   127,   128,   129,
     129,   129,   129,   130,   130,   130,   130,   130,   130,   131,
     131,   132,   132,   132,   132,   132,   132,   132,   132,   132,
     132,   132,   133,   133,   133,   133,   133,   133,   134,   134,
     134,   134,   134,   134,   134,   135,   135,   135,   135,   135,
     135,   136,   137,   137,   138,   138,   138,   139,   139,   139,
     139,   139,   139,   140,   140,   140,   140,   140,   140,   140,
     140,   140,   140,   140,   140,   141,   141,   141,   141,   141,
     141,   141,   141,   141,   141,   141,   141,   141,   141,   141,
     141,   141,   141,   141,   142,   142,   143,   143,   144,   144,
     145,   146,   147,   147,   148,   148
};

  /* YYR2[YYN] -- Number of symbols on the right hand side of rule YYN.  */
static const yytype_int8 yyr2[] =
{
       0,     2,     5,     2,     2,     2,     2,     1,     1,     6,
       5,     5,     5,     0,     2,     0,     2,     2,     0,     1,
       5,     3,     3,     0,     3,     0,     3,     0,     3,     1,
       1,     1,     1,     1,     1,     1,     1,     6,     3,     3,
       6,     3,     3,     0,     2,     1,     1,     2,     0,     4,
       9,     1,     2,     0,     1,     0,     1,     1,     4,     0,
       1,     1,     4,     2,     7,     6,     0,     3,     2,     3,
       3,     7,     3,     3,     8,     3,     1,     1,     0,     3,
       6,     1,     5,     1,     1,     2,     2,     2,     0,     1,
       1,     5,     1,     4,     2,     1,     3,     2,     1,     2,
       2,     2,     3,     2,     3,     4,     3,     1,     1,     1,
       2,     2,     1,     1,     1,     1,     1,     2,     2,     4,
       8,     5,     9,     1,     1,     1,     1,     1,     1,     1,
       1,     2,     3,     4,     6,     6,    10,    10,     5,     5,
       5,     5,     1,     2,     1,     3,     1,     2,     3,     4,
       3,     5,     4,     4,     6,     5,     6,     2,     6,     2,
       3,     2,     2,     0,     1,     3,     2,     2,     2,     2,
       2,     2,     0,     1,     2,     2,     2,     2,     3,     2,
       2,     1,     2,     1,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     5,     3,     2,     2,     3,     3,
       1,     2,     5,     3,     1,     3
};


#define yyerrok         (yyerrstatus = 0)
#define yyclearin       (yychar = YYEMPTY)
#define YYEMPTY         (-2)
#define YYEOF           0

#define YYACCEPT        goto yyacceptlab
#define YYABORT         goto yyabortlab
#define YYERROR         goto yyerrorlab


#define YYRECOVERING()  (!!yyerrstatus)

#define YYBACKUP(Token, Value)                                    \
  do                                                              \
    if (yychar == YYEMPTY)                                        \
      {                                                           \
        yychar = (Token);                                         \
        yylval = (Value);                                         \
        YYPOPSTACK (yylen);                                       \
        yystate = *yyssp;                                         \
        goto yybackup;                                            \
      }                                                           \
    else                                                          \
      {                                                           \
        yyerror (parser, YY_("syntax error: cannot back up")); \
        YYERROR;                                                  \
      }                                                           \
  while (0)

/* Error token number */
#define YYTERROR        1
#define YYERRCODE       256



/* Enable debugging if requested.  */
#if YYDEBUG

# ifndef YYFPRINTF
#  include <stdio.h> /* INFRINGES ON USER NAME SPACE */
#  define YYFPRINTF fprintf
# endif

# define YYDPRINTF(Args)                        \
do {                                            \
  if (yydebug)                                  \
    YYFPRINTF Args;                             \
} while (0)

/* This macro is provided for backward compatibility. */
#ifndef YY_LOCATION_PRINT
# define YY_LOCATION_PRINT(File, Loc) ((void) 0)
#endif


# define YY_SYMBOL_PRINT(Title, Type, Value, Location)                    \
do {                                                                      \
  if (yydebug)                                                            \
    {                                                                     \
      YYFPRINTF (stderr, "%s ", Title);                                   \
      yy_symbol_print (stderr,                                            \
                  Type, Value, parser); \
      YYFPRINTF (stderr, "\n");                                           \
    }                                                                     \
} while (0)


/*-----------------------------------.
| Print this symbol's value on YYO.  |
`-----------------------------------*/

static void
yy_symbol_value_print (FILE *yyo, int yytype, YYSTYPE const * const yyvaluep, void* parser)
{
  FILE *yyoutput = yyo;
  YYUSE (yyoutput);
  YYUSE (parser);
  if (!yyvaluep)
    return;
# ifdef YYPRINT
  if (yytype < YYNTOKENS)
    YYPRINT (yyo, yytoknum[yytype], *yyvaluep);
# endif
  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  YYUSE (yytype);
  YY_IGNORE_MAYBE_UNINITIALIZED_END
}


/*---------------------------.
| Print this symbol on YYO.  |
`---------------------------*/

static void
yy_symbol_print (FILE *yyo, int yytype, YYSTYPE const * const yyvaluep, void* parser)
{
  YYFPRINTF (yyo, "%s %s (",
             yytype < YYNTOKENS ? "token" : "nterm", yytname[yytype]);

  yy_symbol_value_print (yyo, yytype, yyvaluep, parser);
  YYFPRINTF (yyo, ")");
}

/*------------------------------------------------------------------.
| yy_stack_print -- Print the state stack from its BOTTOM up to its |
| TOP (included).                                                   |
`------------------------------------------------------------------*/

static void
yy_stack_print (yy_state_t *yybottom, yy_state_t *yytop)
{
  YYFPRINTF (stderr, "Stack now");
  for (; yybottom <= yytop; yybottom++)
    {
      int yybot = *yybottom;
      YYFPRINTF (stderr, " %d", yybot);
    }
  YYFPRINTF (stderr, "\n");
}

# define YY_STACK_PRINT(Bottom, Top)                            \
do {                                                            \
  if (yydebug)                                                  \
    yy_stack_print ((Bottom), (Top));                           \
} while (0)


/*------------------------------------------------.
| Report that the YYRULE is going to be reduced.  |
`------------------------------------------------*/

static void
yy_reduce_print (yy_state_t *yyssp, YYSTYPE *yyvsp, int yyrule, void* parser)
{
  int yylno = yyrline[yyrule];
  int yynrhs = yyr2[yyrule];
  int yyi;
  YYFPRINTF (stderr, "Reducing stack by rule %d (line %d):\n",
             yyrule - 1, yylno);
  /* The symbols being reduced.  */
  for (yyi = 0; yyi < yynrhs; yyi++)
    {
      YYFPRINTF (stderr, "   $%d = ", yyi + 1);
      yy_symbol_print (stderr,
                       yystos[+yyssp[yyi + 1 - yynrhs]],
                       &yyvsp[(yyi + 1) - (yynrhs)]
                                              , parser);
      YYFPRINTF (stderr, "\n");
    }
}

# define YY_REDUCE_PRINT(Rule)          \
do {                                    \
  if (yydebug)                          \
    yy_reduce_print (yyssp, yyvsp, Rule, parser); \
} while (0)

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
#ifndef YYINITDEPTH
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
#   define yystrlen(S) (YY_CAST (YYPTRDIFF_T, strlen (S)))
#  else
/* Return the length of YYSTR.  */
static YYPTRDIFF_T
yystrlen (const char *yystr)
{
  YYPTRDIFF_T yylen;
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
static char *
yystpcpy (char *yydest, const char *yysrc)
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
static YYPTRDIFF_T
yytnamerr (char *yyres, const char *yystr)
{
  if (*yystr == '"')
    {
      YYPTRDIFF_T yyn = 0;
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
            else
              goto append;

          append:
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

  if (yyres)
    return yystpcpy (yyres, yystr) - yyres;
  else
    return yystrlen (yystr);
}
# endif

/* Copy into *YYMSG, which is of size *YYMSG_ALLOC, an error message
   about the unexpected token YYTOKEN for the state stack whose top is
   YYSSP.

   Return 0 if *YYMSG was successfully written.  Return 1 if *YYMSG is
   not large enough to hold the message.  In that case, also set
   *YYMSG_ALLOC to the required number of bytes.  Return 2 if the
   required number of bytes is too large to store.  */
static int
yysyntax_error (YYPTRDIFF_T *yymsg_alloc, char **yymsg,
                yy_state_t *yyssp, int yytoken)
{
  enum { YYERROR_VERBOSE_ARGS_MAXIMUM = 5 };
  /* Internationalized format string. */
  const char *yyformat = YY_NULLPTR;
  /* Arguments of yyformat: reported tokens (one for the "unexpected",
     one per "expected"). */
  char const *yyarg[YYERROR_VERBOSE_ARGS_MAXIMUM];
  /* Actual size of YYARG. */
  int yycount = 0;
  /* Cumulated lengths of YYARG.  */
  YYPTRDIFF_T yysize = 0;

  /* There are many possibilities here to consider:
     - If this state is a consistent state with a default action, then
       the only way this function was invoked is if the default action
       is an error action.  In that case, don't check for expected
       tokens because there are none.
     - The only way there can be no lookahead present (in yychar) is if
       this state is a consistent state with a default action.  Thus,
       detecting the absence of a lookahead is sufficient to determine
       that there is no unexpected or expected token to report.  In that
       case, just report a simple "syntax error".
     - Don't assume there isn't a lookahead just because this state is a
       consistent state with a default action.  There might have been a
       previous inconsistent state, consistent state with a non-default
       action, or user semantic action that manipulated yychar.
     - Of course, the expected token list depends on states to have
       correct lookahead information, and it depends on the parser not
       to perform extra reductions after fetching a lookahead from the
       scanner and before detecting a syntax error.  Thus, state merging
       (from LALR or IELR) and default reductions corrupt the expected
       token list.  However, the list is correct for canonical LR with
       one exception: it will still contain any token that will not be
       accepted due to an error action in a later state.
  */
  if (yytoken != YYEMPTY)
    {
      int yyn = yypact[+*yyssp];
      YYPTRDIFF_T yysize0 = yytnamerr (YY_NULLPTR, yytname[yytoken]);
      yysize = yysize0;
      yyarg[yycount++] = yytname[yytoken];
      if (!yypact_value_is_default (yyn))
        {
          /* Start YYX at -YYN if negative to avoid negative indexes in
             YYCHECK.  In other words, skip the first -YYN actions for
             this state because they are default actions.  */
          int yyxbegin = yyn < 0 ? -yyn : 0;
          /* Stay within bounds of both yycheck and yytname.  */
          int yychecklim = YYLAST - yyn + 1;
          int yyxend = yychecklim < YYNTOKENS ? yychecklim : YYNTOKENS;
          int yyx;

          for (yyx = yyxbegin; yyx < yyxend; ++yyx)
            if (yycheck[yyx + yyn] == yyx && yyx != YYTERROR
                && !yytable_value_is_error (yytable[yyx + yyn]))
              {
                if (yycount == YYERROR_VERBOSE_ARGS_MAXIMUM)
                  {
                    yycount = 1;
                    yysize = yysize0;
                    break;
                  }
                yyarg[yycount++] = yytname[yyx];
                {
                  YYPTRDIFF_T yysize1
                    = yysize + yytnamerr (YY_NULLPTR, yytname[yyx]);
                  if (yysize <= yysize1 && yysize1 <= YYSTACK_ALLOC_MAXIMUM)
                    yysize = yysize1;
                  else
                    return 2;
                }
              }
        }
    }

  switch (yycount)
    {
# define YYCASE_(N, S)                      \
      case N:                               \
        yyformat = S;                       \
      break
    default: /* Avoid compiler warnings. */
      YYCASE_(0, YY_("syntax error"));
      YYCASE_(1, YY_("syntax error, unexpected %s"));
      YYCASE_(2, YY_("syntax error, unexpected %s, expecting %s"));
      YYCASE_(3, YY_("syntax error, unexpected %s, expecting %s or %s"));
      YYCASE_(4, YY_("syntax error, unexpected %s, expecting %s or %s or %s"));
      YYCASE_(5, YY_("syntax error, unexpected %s, expecting %s or %s or %s or %s"));
# undef YYCASE_
    }

  {
    /* Don't count the "%s"s in the final size, but reserve room for
       the terminator.  */
    YYPTRDIFF_T yysize1 = yysize + (yystrlen (yyformat) - 2 * yycount) + 1;
    if (yysize <= yysize1 && yysize1 <= YYSTACK_ALLOC_MAXIMUM)
      yysize = yysize1;
    else
      return 2;
  }

  if (*yymsg_alloc < yysize)
    {
      *yymsg_alloc = 2 * yysize;
      if (! (yysize <= *yymsg_alloc
             && *yymsg_alloc <= YYSTACK_ALLOC_MAXIMUM))
        *yymsg_alloc = YYSTACK_ALLOC_MAXIMUM;
      return 1;
    }

  /* Avoid sprintf, as that infringes on the user's name space.
     Don't have undefined behavior even if the translation
     produced a string with the wrong number of "%s"s.  */
  {
    char *yyp = *yymsg;
    int yyi = 0;
    while ((*yyp = *yyformat) != '\0')
      if (*yyp == '%' && yyformat[1] == 's' && yyi < yycount)
        {
          yyp += yytnamerr (yyp, yyarg[yyi++]);
          yyformat += 2;
        }
      else
        {
          ++yyp;
          ++yyformat;
        }
  }
  return 0;
}
#endif /* YYERROR_VERBOSE */

/*-----------------------------------------------.
| Release the memory associated to this symbol.  |
`-----------------------------------------------*/

static void
yydestruct (const char *yymsg, int yytype, YYSTYPE *yyvaluep, void* parser)
{
  YYUSE (yyvaluep);
  YYUSE (parser);
  if (!yymsg)
    yymsg = "Deleting";
  YY_SYMBOL_PRINT (yymsg, yytype, yyvaluep, yylocationp);

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  YYUSE (yytype);
  YY_IGNORE_MAYBE_UNINITIALIZED_END
}




/*----------.
| yyparse.  |
`----------*/

int
yyparse (void* parser)
{
/* The lookahead symbol.  */
int yychar;


/* The semantic value of the lookahead symbol.  */
/* Default value used for initialization, for pacifying older GCCs
   or non-GCC compilers.  */
YY_INITIAL_VALUE (static YYSTYPE yyval_default;)
YYSTYPE yylval YY_INITIAL_VALUE (= yyval_default);

    /* Number of syntax errors so far.  */
    int yynerrs;

    yy_state_fast_t yystate;
    /* Number of tokens to shift before error messages enabled.  */
    int yyerrstatus;

    /* The stacks and their tools:
       'yyss': related to states.
       'yyvs': related to semantic values.

       Refer to the stacks through separate pointers, to allow yyoverflow
       to reallocate them elsewhere.  */

    /* The state stack.  */
    yy_state_t yyssa[YYINITDEPTH];
    yy_state_t *yyss;
    yy_state_t *yyssp;

    /* The semantic value stack.  */
    YYSTYPE yyvsa[YYINITDEPTH];
    YYSTYPE *yyvs;
    YYSTYPE *yyvsp;

    YYPTRDIFF_T yystacksize;

  int yyn;
  int yyresult;
  /* Lookahead token as an internal (translated) token number.  */
  int yytoken = 0;
  /* The variables used to return semantic value and location from the
     action routines.  */
  YYSTYPE yyval;

#if YYERROR_VERBOSE
  /* Buffer for error messages, and its allocated size.  */
  char yymsgbuf[128];
  char *yymsg = yymsgbuf;
  YYPTRDIFF_T yymsg_alloc = sizeof yymsgbuf;
#endif

#define YYPOPSTACK(N)   (yyvsp -= (N), yyssp -= (N))

  /* The number of symbols on the RHS of the reduced rule.
     Keep to zero when no symbol should be popped.  */
  int yylen = 0;

  yyssp = yyss = yyssa;
  yyvsp = yyvs = yyvsa;
  yystacksize = YYINITDEPTH;

  YYDPRINTF ((stderr, "Starting parse\n"));

  yystate = 0;
  yyerrstatus = 0;
  yynerrs = 0;
  yychar = YYEMPTY; /* Cause a token to be read.  */
  goto yysetstate;


/*------------------------------------------------------------.
| yynewstate -- push a new state, which is found in yystate.  |
`------------------------------------------------------------*/
yynewstate:
  /* In all cases, when you get here, the value and location stacks
     have just been pushed.  So pushing a state here evens the stacks.  */
  yyssp++;


/*--------------------------------------------------------------------.
| yysetstate -- set current state (the top of the stack) to yystate.  |
`--------------------------------------------------------------------*/
yysetstate:
  YYDPRINTF ((stderr, "Entering state %d\n", yystate));
  YY_ASSERT (0 <= yystate && yystate < YYNSTATES);
  YY_IGNORE_USELESS_CAST_BEGIN
  *yyssp = YY_CAST (yy_state_t, yystate);
  YY_IGNORE_USELESS_CAST_END

  if (yyss + yystacksize - 1 <= yyssp)
#if !defined yyoverflow && !defined YYSTACK_RELOCATE
    goto yyexhaustedlab;
#else
    {
      /* Get the current used size of the three stacks, in elements.  */
      YYPTRDIFF_T yysize = yyssp - yyss + 1;

# if defined yyoverflow
      {
        /* Give user a chance to reallocate the stack.  Use copies of
           these so that the &'s don't force the real ones into
           memory.  */
        yy_state_t *yyss1 = yyss;
        YYSTYPE *yyvs1 = yyvs;

        /* Each stack pointer address is followed by the size of the
           data in use in that stack, in bytes.  This used to be a
           conditional around just the two extra args, but that might
           be undefined if yyoverflow is a macro.  */
        yyoverflow (YY_("memory exhausted"),
                    &yyss1, yysize * YYSIZEOF (*yyssp),
                    &yyvs1, yysize * YYSIZEOF (*yyvsp),
                    &yystacksize);
        yyss = yyss1;
        yyvs = yyvs1;
      }
# else /* defined YYSTACK_RELOCATE */
      /* Extend the stack our own way.  */
      if (YYMAXDEPTH <= yystacksize)
        goto yyexhaustedlab;
      yystacksize *= 2;
      if (YYMAXDEPTH < yystacksize)
        yystacksize = YYMAXDEPTH;

      {
        yy_state_t *yyss1 = yyss;
        union yyalloc *yyptr =
          YY_CAST (union yyalloc *,
                   YYSTACK_ALLOC (YY_CAST (YYSIZE_T, YYSTACK_BYTES (yystacksize))));
        if (! yyptr)
          goto yyexhaustedlab;
        YYSTACK_RELOCATE (yyss_alloc, yyss);
        YYSTACK_RELOCATE (yyvs_alloc, yyvs);
# undef YYSTACK_RELOCATE
        if (yyss1 != yyssa)
          YYSTACK_FREE (yyss1);
      }
# endif

      yyssp = yyss + yysize - 1;
      yyvsp = yyvs + yysize - 1;

      YY_IGNORE_USELESS_CAST_BEGIN
      YYDPRINTF ((stderr, "Stack size increased to %ld\n",
                  YY_CAST (long, yystacksize)));
      YY_IGNORE_USELESS_CAST_END

      if (yyss + yystacksize - 1 <= yyssp)
        YYABORT;
    }
#endif /* !defined yyoverflow && !defined YYSTACK_RELOCATE */

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
  if (yypact_value_is_default (yyn))
    goto yydefault;

  /* Not known => get a lookahead token if don't already have one.  */

  /* YYCHAR is either YYEMPTY or YYEOF or a valid lookahead symbol.  */
  if (yychar == YYEMPTY)
    {
      YYDPRINTF ((stderr, "Reading a token: "));
      yychar = yylex (&yylval, parser);
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
      if (yytable_value_is_error (yyn))
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
  yystate = yyn;
  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END

  /* Discard the shifted token.  */
  yychar = YYEMPTY;
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
| yyreduce -- do a reduction.  |
`-----------------------------*/
yyreduce:
  /* yyn is the number of a rule to reduce with.  */
  yylen = yyr2[yyn];

  /* If YYLEN is nonzero, implement the default value of the action:
     '$$ = $1'.

     Otherwise, the following line sets YYVAL to garbage.
     This behavior is undocumented and Bison
     users should not rely upon it.  Assigning to YYVAL
     unconditionally makes the parser a bit smaller, and it avoids a
     GCC warning that YYVAL may be used uninitialized.  */
  yyval = yyvsp[1-yylen];


  YY_REDUCE_PRINT (yyn);
  switch (yyn)
    {
  case 9:
#line 310 "CSSGrammar.y"
                                                                      {
        static_cast<CSSParser*>(parser)->rule = (yyvsp[-2].rule);
    }
#line 1949 "CSSGrammar.tab.c"
    break;

  case 10:
#line 316 "CSSGrammar.y"
                                                          {
        /* can be empty */
    }
#line 1957 "CSSGrammar.tab.c"
    break;

  case 11:
#line 322 "CSSGrammar.y"
                                              {
        CSSParser* p = static_cast<CSSParser*>(parser);
        if ((yyvsp[-1].valueList)) {
            p->valueList = p->sinkFloatingValueList((yyvsp[-1].valueList));
            int oldParsedProperties = p->numParsedProperties;
            if (!p->parseValue(p->id, p->important))
                p->rollbackLastProperties(p->numParsedProperties - oldParsedProperties);
            delete p->valueList;
            p->valueList = 0;
        }
    }
#line 1973 "CSSGrammar.tab.c"
    break;

  case 12:
#line 336 "CSSGrammar.y"
                                                                  {
         CSSParser* p = static_cast<CSSParser*>(parser);
         p->mediaQuery = p->sinkFloatingMediaQuery((yyvsp[-1].mediaQuery));
     }
#line 1982 "CSSGrammar.tab.c"
    break;

  case 19:
#line 355 "CSSGrammar.y"
            {
  }
#line 1989 "CSSGrammar.tab.c"
    break;

  case 20:
#line 360 "CSSGrammar.y"
                                                 {
     CSSParser* p = static_cast<CSSParser*>(parser);
     (yyval.rule) = static_cast<CSSParser*>(parser)->createCharsetRule((yyvsp[-2].string));
     if ((yyval.rule) && p->styleElement && p->styleElement->isCSSStyleSheet())
         p->styleElement->append((yyval.rule));
  }
#line 2000 "CSSGrammar.tab.c"
    break;

  case 21:
#line 366 "CSSGrammar.y"
                                    {
  }
#line 2007 "CSSGrammar.tab.c"
    break;

  case 22:
#line 368 "CSSGrammar.y"
                          {
  }
#line 2014 "CSSGrammar.tab.c"
    break;

  case 24:
#line 374 "CSSGrammar.y"
                                 {
     CSSParser* p = static_cast<CSSParser*>(parser);
     if ((yyvsp[-1].rule) && p->styleElement && p->styleElement->isCSSStyleSheet())
         p->styleElement->append((yyvsp[-1].rule));
 }
#line 2024 "CSSGrammar.tab.c"
    break;

  case 28:
#line 388 "CSSGrammar.y"
                             {
     CSSParser* p = static_cast<CSSParser*>(parser);
     if ((yyvsp[-1].rule) && p->styleElement && p->styleElement->isCSSStyleSheet())
         p->styleElement->append((yyvsp[-1].rule));
 }
#line 2034 "CSSGrammar.tab.c"
    break;

  case 37:
#line 407 "CSSGrammar.y"
                                                                          {
        (yyval.rule) = static_cast<CSSParser*>(parser)->createImportRule((yyvsp[-3].string), (yyvsp[-1].mediaList));
    }
#line 2042 "CSSGrammar.tab.c"
    break;

  case 38:
#line 410 "CSSGrammar.y"
                                   {
        (yyval.rule) = 0;
    }
#line 2050 "CSSGrammar.tab.c"
    break;

  case 39:
#line 413 "CSSGrammar.y"
                         {
        (yyval.rule) = 0;
    }
#line 2058 "CSSGrammar.tab.c"
    break;

  case 40:
#line 419 "CSSGrammar.y"
                                                                        {
    CSSParser* p = static_cast<CSSParser*>(parser);
    if (p->styleElement && p->styleElement->isCSSStyleSheet())
        static_cast<CSSStyleSheet*>(p->styleElement)->addNamespace(p, atomicString((yyvsp[-3].string)), atomicString((yyvsp[-2].string)));
}
#line 2068 "CSSGrammar.tab.c"
    break;

  case 43:
#line 429 "CSSGrammar.y"
            { (yyval.string).characters = 0; }
#line 2074 "CSSGrammar.tab.c"
    break;

  case 44:
#line 430 "CSSGrammar.y"
                   { (yyval.string) = (yyvsp[-1].string); }
#line 2080 "CSSGrammar.tab.c"
    break;

  case 47:
#line 439 "CSSGrammar.y"
                      {
        (yyval.string) = (yyvsp[-1].string);
    }
#line 2088 "CSSGrammar.tab.c"
    break;

  case 48:
#line 445 "CSSGrammar.y"
              {
        (yyval.valueList) = 0;
    }
#line 2096 "CSSGrammar.tab.c"
    break;

  case 49:
#line 448 "CSSGrammar.y"
                                       {
        (yyval.valueList) = (yyvsp[-1].valueList);
    }
#line 2104 "CSSGrammar.tab.c"
    break;

  case 50:
#line 454 "CSSGrammar.y"
                                                                                                      {
        (yyvsp[-4].string).lower();
        (yyval.mediaQueryExp) = static_cast<CSSParser*>(parser)->createFloatingMediaQueryExp(atomicString((yyvsp[-4].string)), (yyvsp[-2].valueList));
    }
#line 2113 "CSSGrammar.tab.c"
    break;

  case 51:
#line 461 "CSSGrammar.y"
                    {
      CSSParser* p = static_cast<CSSParser*>(parser);
      (yyval.mediaQueryExpList) = p->createFloatingMediaQueryExpList();
      (yyval.mediaQueryExpList)->append(p->sinkFloatingMediaQueryExp((yyvsp[0].mediaQueryExp)));
    }
#line 2123 "CSSGrammar.tab.c"
    break;

  case 52:
#line 466 "CSSGrammar.y"
                                           {
      (yyval.mediaQueryExpList) = (yyvsp[-1].mediaQueryExpList);
      (yyval.mediaQueryExpList)->append(static_cast<CSSParser*>(parser)->sinkFloatingMediaQueryExp((yyvsp[0].mediaQueryExp)));
    }
#line 2132 "CSSGrammar.tab.c"
    break;

  case 53:
#line 473 "CSSGrammar.y"
              {
        (yyval.mediaQueryExpList) = static_cast<CSSParser*>(parser)->createFloatingMediaQueryExpList();
    }
#line 2140 "CSSGrammar.tab.c"
    break;

  case 55:
#line 480 "CSSGrammar.y"
              {
        (yyval.mediaQueryRestrictor) = MediaQuery::None;
    }
#line 2148 "CSSGrammar.tab.c"
    break;

  case 56:
#line 483 "CSSGrammar.y"
                 {
        (yyval.mediaQueryRestrictor) = MediaQuery::Only;
    }
#line 2156 "CSSGrammar.tab.c"
    break;

  case 57:
#line 486 "CSSGrammar.y"
                {
        (yyval.mediaQueryRestrictor) = MediaQuery::Not;
    }
#line 2164 "CSSGrammar.tab.c"
    break;

  case 58:
#line 492 "CSSGrammar.y"
                                                                         {
        CSSParser* p = static_cast<CSSParser*>(parser);
        (yyvsp[-1].string).lower();
        (yyval.mediaQuery) = p->createFloatingMediaQuery((yyvsp[-3].mediaQueryRestrictor), domString((yyvsp[-1].string)), p->sinkFloatingMediaQueryExpList((yyvsp[0].mediaQueryExpList)));
    }
#line 2174 "CSSGrammar.tab.c"
    break;

  case 59:
#line 500 "CSSGrammar.y"
                 {
        (yyval.mediaList) = static_cast<CSSParser*>(parser)->createMediaList();
     }
#line 2182 "CSSGrammar.tab.c"
    break;

  case 61:
#line 507 "CSSGrammar.y"
                {
        CSSParser* p = static_cast<CSSParser*>(parser);
        (yyval.mediaList) = p->createMediaList();
        (yyval.mediaList)->appendMediaQuery(p->sinkFloatingMediaQuery((yyvsp[0].mediaQuery)));
    }
#line 2192 "CSSGrammar.tab.c"
    break;

  case 62:
#line 512 "CSSGrammar.y"
                                             {
        (yyval.mediaList) = (yyvsp[-3].mediaList);
        if ((yyval.mediaList))
            (yyval.mediaList)->appendMediaQuery(static_cast<CSSParser*>(parser)->sinkFloatingMediaQuery((yyvsp[0].mediaQuery)));
    }
#line 2202 "CSSGrammar.tab.c"
    break;

  case 63:
#line 517 "CSSGrammar.y"
                       {
        (yyval.mediaList) = 0;
    }
#line 2210 "CSSGrammar.tab.c"
    break;

  case 64:
#line 523 "CSSGrammar.y"
                                                                      {
        (yyval.rule) = static_cast<CSSParser*>(parser)->createMediaRule((yyvsp[-4].mediaList), (yyvsp[-1].ruleList));
    }
#line 2218 "CSSGrammar.tab.c"
    break;

  case 65:
#line 526 "CSSGrammar.y"
                                                             {
        (yyval.rule) = static_cast<CSSParser*>(parser)->createMediaRule(0, (yyvsp[-1].ruleList));
    }
#line 2226 "CSSGrammar.tab.c"
    break;

  case 66:
#line 532 "CSSGrammar.y"
                { (yyval.ruleList) = 0; }
#line 2232 "CSSGrammar.tab.c"
    break;

  case 67:
#line 533 "CSSGrammar.y"
                                       {
        (yyval.ruleList) = (yyvsp[-2].ruleList);
        if ((yyvsp[-1].rule)) {
            if (!(yyval.ruleList))
                (yyval.ruleList) = static_cast<CSSParser*>(parser)->createRuleList();
            (yyval.ruleList)->append((yyvsp[-1].rule));
        }
    }
#line 2245 "CSSGrammar.tab.c"
    break;

  case 68:
#line 544 "CSSGrammar.y"
                    {
      (yyval.string) = (yyvsp[-1].string);
  }
#line 2253 "CSSGrammar.tab.c"
    break;

  case 69:
#line 561 "CSSGrammar.y"
                                 {
      (yyval.rule) = 0;
    }
#line 2261 "CSSGrammar.tab.c"
    break;

  case 70:
#line 564 "CSSGrammar.y"
                       {
      (yyval.rule) = 0;
    }
#line 2269 "CSSGrammar.tab.c"
    break;

  case 71:
#line 571 "CSSGrammar.y"
                                                      {
        (yyval.rule) = static_cast<CSSParser*>(parser)->createFontFaceRule();
    }
#line 2277 "CSSGrammar.tab.c"
    break;

  case 72:
#line 574 "CSSGrammar.y"
                                        {
      (yyval.rule) = 0;
    }
#line 2285 "CSSGrammar.tab.c"
    break;

  case 73:
#line 577 "CSSGrammar.y"
                              {
      (yyval.rule) = 0;
    }
#line 2293 "CSSGrammar.tab.c"
    break;

  case 74:
#line 583 "CSSGrammar.y"
                                                                                           {
        (yyval.rule) = static_cast<CSSParser*>(parser)->createKeyframesRule();
    }
#line 2301 "CSSGrammar.tab.c"
    break;

  case 75:
#line 586 "CSSGrammar.y"
                                      {
      (yyval.rule) = 0;
    }
#line 2309 "CSSGrammar.tab.c"
    break;

  case 76:
#line 592 "CSSGrammar.y"
          { static_cast<CSSParser*>(parser)->setKeyframesName(domString((yyvsp[0].string))); }
#line 2315 "CSSGrammar.tab.c"
    break;

  case 77:
#line 593 "CSSGrammar.y"
           { static_cast<CSSParser*>(parser)->setKeyframesName(domString((yyvsp[0].string))); }
#line 2321 "CSSGrammar.tab.c"
    break;

  case 80:
#line 602 "CSSGrammar.y"
                                                              {
        static_cast<CSSParser*>(parser)->createKeyframeRule();
    }
#line 2329 "CSSGrammar.tab.c"
    break;

  case 81:
#line 608 "CSSGrammar.y"
        { static_cast<CSSParser*>(parser)->addKeyframeKey((yyvsp[0].val)); }
#line 2335 "CSSGrammar.tab.c"
    break;

  case 82:
#line 609 "CSSGrammar.y"
                                             {
        static_cast<CSSParser*>(parser)->addKeyframeKey((yyvsp[0].val));
    }
#line 2343 "CSSGrammar.tab.c"
    break;

  case 83:
#line 615 "CSSGrammar.y"
               { (yyval.val) = (yyvsp[0].val) / 100.0f; }
#line 2349 "CSSGrammar.tab.c"
    break;

  case 84:
#line 616 "CSSGrammar.y"
          {
        // "from" => 0%, "to" => 100%; anything else is invalid (-1 sentinel).
        String s = domString((yyvsp[0].string)).lower();
        if (s == "from") (yyval.val) = 0.0f;
        else if (s == "to") (yyval.val) = 1.0f;
        else (yyval.val) = -1.0f;
    }
#line 2361 "CSSGrammar.tab.c"
    break;

  case 85:
#line 626 "CSSGrammar.y"
                    { (yyval.relation) = CSSSelector::DirectAdjacent; }
#line 2367 "CSSGrammar.tab.c"
    break;

  case 86:
#line 627 "CSSGrammar.y"
                    { (yyval.relation) = CSSSelector::IndirectAdjacent; }
#line 2373 "CSSGrammar.tab.c"
    break;

  case 87:
#line 628 "CSSGrammar.y"
                    { (yyval.relation) = CSSSelector::Child; }
#line 2379 "CSSGrammar.tab.c"
    break;

  case 88:
#line 629 "CSSGrammar.y"
                { (yyval.relation) = CSSSelector::Descendant; }
#line 2385 "CSSGrammar.tab.c"
    break;

  case 89:
#line 633 "CSSGrammar.y"
        { (yyval.i) = -1; }
#line 2391 "CSSGrammar.tab.c"
    break;

  case 90:
#line 634 "CSSGrammar.y"
        { (yyval.i) = 1; }
#line 2397 "CSSGrammar.tab.c"
    break;

  case 91:
#line 638 "CSSGrammar.y"
                                                       {
        (yyval.rule) = static_cast<CSSParser*>(parser)->createStyleRule((yyvsp[-4].selector));
    }
#line 2405 "CSSGrammar.tab.c"
    break;

  case 92:
#line 644 "CSSGrammar.y"
                                   {
        (yyval.selector) = (yyvsp[0].selector);
    }
#line 2413 "CSSGrammar.tab.c"
    break;

  case 93:
#line 647 "CSSGrammar.y"
                                                                   {
        if ((yyvsp[-3].selector) && (yyvsp[0].selector)) {
            CSSParser* p = static_cast<CSSParser*>(parser);
            (yyval.selector) = (yyvsp[-3].selector);
            (yyval.selector)->append(p->sinkFloatingSelector((yyvsp[0].selector)));
        } else
            (yyval.selector) = 0;
    }
#line 2426 "CSSGrammar.tab.c"
    break;

  case 94:
#line 655 "CSSGrammar.y"
                        {
        (yyval.selector) = 0;
    }
#line 2434 "CSSGrammar.tab.c"
    break;

  case 95:
#line 661 "CSSGrammar.y"
                    {
        (yyval.selector) = (yyvsp[0].selector);
    }
#line 2442 "CSSGrammar.tab.c"
    break;

  case 96:
#line 664 "CSSGrammar.y"
                                          {
        (yyval.selector) = (yyvsp[0].selector);
        if (!(yyvsp[-2].selector))
            (yyval.selector) = 0;
        else if ((yyval.selector)) {
            CSSParser* p = static_cast<CSSParser*>(parser);
            CSSSelector* end = (yyval.selector);
            while (end->m_tagHistory)
                end = end->m_tagHistory;
            end->m_relation = (yyvsp[-1].relation);
            end->m_tagHistory = p->sinkFloatingSelector((yyvsp[-2].selector));
            if ((yyvsp[-1].relation) == CSSSelector::Descendant || (yyvsp[-1].relation) == CSSSelector::Child) {
                if (Document* doc = p->document())
                    doc->setUsesDescendantRules(true);
            } else if ((yyvsp[-1].relation) == CSSSelector::DirectAdjacent || (yyvsp[-1].relation) == CSSSelector::IndirectAdjacent) {
                if (Document* doc = p->document())
                    doc->setUsesSiblingRules(true);
            }
        }
    }
#line 2467 "CSSGrammar.tab.c"
    break;

  case 97:
#line 684 "CSSGrammar.y"
                     {
        (yyval.selector) = 0;
    }
#line 2475 "CSSGrammar.tab.c"
    break;

  case 98:
#line 690 "CSSGrammar.y"
                    { (yyval.string).characters = 0; (yyval.string).length = 0; }
#line 2481 "CSSGrammar.tab.c"
    break;

  case 99:
#line 691 "CSSGrammar.y"
              { static UChar star = '*'; (yyval.string).characters = &star; (yyval.string).length = 1; }
#line 2487 "CSSGrammar.tab.c"
    break;

  case 100:
#line 692 "CSSGrammar.y"
                { (yyval.string) = (yyvsp[-1].string); }
#line 2493 "CSSGrammar.tab.c"
    break;

  case 101:
#line 696 "CSSGrammar.y"
                             {
        CSSParser* p = static_cast<CSSParser*>(parser);
        (yyval.selector) = p->createFloatingSelector();
        (yyval.selector)->m_tag = QualifiedName(nullAtom, atomicString((yyvsp[-1].string)), p->defaultNamespace);
    }
#line 2503 "CSSGrammar.tab.c"
    break;

  case 102:
#line 701 "CSSGrammar.y"
                                              {
        (yyval.selector) = (yyvsp[-1].selector);
        if ((yyval.selector)) {
            CSSParser* p = static_cast<CSSParser*>(parser);
            (yyval.selector)->m_tag = QualifiedName(nullAtom, atomicString((yyvsp[-2].string)), p->defaultNamespace);
        }
    }
#line 2515 "CSSGrammar.tab.c"
    break;

  case 103:
#line 708 "CSSGrammar.y"
                                 {
        (yyval.selector) = (yyvsp[-1].selector);
        CSSParser* p = static_cast<CSSParser*>(parser);
        if ((yyval.selector) && p->defaultNamespace != starAtom)
            (yyval.selector)->m_tag = QualifiedName(nullAtom, starAtom, p->defaultNamespace);
    }
#line 2526 "CSSGrammar.tab.c"
    break;

  case 104:
#line 714 "CSSGrammar.y"
                                                  {
        AtomicString namespacePrefix = atomicString((yyvsp[-2].string));
        CSSParser* p = static_cast<CSSParser*>(parser);
        (yyval.selector) = p->createFloatingSelector();
        if (p->styleElement && p->styleElement->isCSSStyleSheet())
            (yyval.selector)->m_tag = QualifiedName(namespacePrefix,
                                    atomicString((yyvsp[-1].string)),
                                    static_cast<CSSStyleSheet*>(p->styleElement)->determineNamespace(namespacePrefix));
        else // FIXME: Shouldn't this case be an error?
            (yyval.selector)->m_tag = QualifiedName(nullAtom, atomicString((yyvsp[-1].string)), p->defaultNamespace);
    }
#line 2542 "CSSGrammar.tab.c"
    break;

  case 105:
#line 725 "CSSGrammar.y"
                                                                 {
        (yyval.selector) = (yyvsp[-1].selector);
        if ((yyval.selector)) {
            AtomicString namespacePrefix = atomicString((yyvsp[-3].string));
            CSSParser* p = static_cast<CSSParser*>(parser);
            if (p->styleElement && p->styleElement->isCSSStyleSheet())
                (yyval.selector)->m_tag = QualifiedName(namespacePrefix,
                                          atomicString((yyvsp[-2].string)),
                                          static_cast<CSSStyleSheet*>(p->styleElement)->determineNamespace(namespacePrefix));
            else // FIXME: Shouldn't this case be an error?
                (yyval.selector)->m_tag = QualifiedName(nullAtom, atomicString((yyvsp[-2].string)), p->defaultNamespace);
        }
    }
#line 2560 "CSSGrammar.tab.c"
    break;

  case 106:
#line 738 "CSSGrammar.y"
                                                    {
        (yyval.selector) = (yyvsp[-1].selector);
        if ((yyval.selector)) {
            AtomicString namespacePrefix = atomicString((yyvsp[-2].string));
            CSSParser* p = static_cast<CSSParser*>(parser);
            if (p->styleElement && p->styleElement->isCSSStyleSheet())
                (yyval.selector)->m_tag = QualifiedName(namespacePrefix,
                                          starAtom,
                                          static_cast<CSSStyleSheet*>(p->styleElement)->determineNamespace(namespacePrefix));
        }
    }
#line 2576 "CSSGrammar.tab.c"
    break;

  case 107:
#line 752 "CSSGrammar.y"
          {
        ParseString& str = (yyvsp[0].string);
        CSSParser* p = static_cast<CSSParser*>(parser);
        Document* doc = p->document();
        if (doc && doc->isHTMLDocument())
            str.lower();
        (yyval.string) = str;
    }
#line 2589 "CSSGrammar.tab.c"
    break;

  case 108:
#line 760 "CSSGrammar.y"
          {
        static UChar star = '*';
        (yyval.string).characters = &star;
        (yyval.string).length = 1;
    }
#line 2599 "CSSGrammar.tab.c"
    break;

  case 109:
#line 768 "CSSGrammar.y"
              {
        (yyval.selector) = (yyvsp[0].selector);
    }
#line 2607 "CSSGrammar.tab.c"
    break;

  case 110:
#line 771 "CSSGrammar.y"
                               {
        if (!(yyvsp[0].selector))
            (yyval.selector) = 0;
        else if ((yyvsp[-1].selector)) {
            (yyval.selector) = (yyvsp[-1].selector);
            CSSParser* p = static_cast<CSSParser*>(parser);
            CSSSelector* end = (yyvsp[-1].selector);
            while (end->m_tagHistory)
                end = end->m_tagHistory;
            end->m_relation = CSSSelector::SubSelector;
            end->m_tagHistory = p->sinkFloatingSelector((yyvsp[0].selector));
        }
    }
#line 2625 "CSSGrammar.tab.c"
    break;

  case 111:
#line 784 "CSSGrammar.y"
                           {
        (yyval.selector) = 0;
    }
#line 2633 "CSSGrammar.tab.c"
    break;

  case 112:
#line 790 "CSSGrammar.y"
          {
        CSSParser* p = static_cast<CSSParser*>(parser);
        (yyval.selector) = p->createFloatingSelector();
        (yyval.selector)->m_match = CSSSelector::Id;
        if (!p->strict)
            (yyvsp[0].string).lower();
        (yyval.selector)->m_attr = idAttr;
        (yyval.selector)->m_value = atomicString((yyvsp[0].string));
    }
#line 2647 "CSSGrammar.tab.c"
    break;

  case 113:
#line 799 "CSSGrammar.y"
        {
        if ((yyvsp[0].string).characters[0] >= '0' && (yyvsp[0].string).characters[0] <= '9') {
            (yyval.selector) = 0;
        } else {
            CSSParser* p = static_cast<CSSParser*>(parser);
            (yyval.selector) = p->createFloatingSelector();
            (yyval.selector)->m_match = CSSSelector::Id;
            if (!p->strict)
                (yyvsp[0].string).lower();
            (yyval.selector)->m_attr = idAttr;
            (yyval.selector)->m_value = atomicString((yyvsp[0].string));
        }
    }
#line 2665 "CSSGrammar.tab.c"
    break;

  case 117:
#line 818 "CSSGrammar.y"
              {
        CSSParser* p = static_cast<CSSParser*>(parser);
        (yyval.selector) = p->createFloatingSelector();
        (yyval.selector)->m_match = CSSSelector::Class;
        if (!p->strict)
            (yyvsp[0].string).lower();
        (yyval.selector)->m_attr = classAttr;
        (yyval.selector)->m_value = atomicString((yyvsp[0].string));
    }
#line 2679 "CSSGrammar.tab.c"
    break;

  case 118:
#line 830 "CSSGrammar.y"
                      {
        ParseString& str = (yyvsp[-1].string);
        CSSParser* p = static_cast<CSSParser*>(parser);
        Document* doc = p->document();
        if (doc && doc->isHTMLDocument())
            str.lower();
        (yyval.string) = str;
    }
#line 2692 "CSSGrammar.tab.c"
    break;

  case 119:
#line 841 "CSSGrammar.y"
                                  {
        (yyval.selector) = static_cast<CSSParser*>(parser)->createFloatingSelector();
        (yyval.selector)->m_attr = QualifiedName(nullAtom, atomicString((yyvsp[-1].string)), nullAtom);
        (yyval.selector)->m_match = CSSSelector::Set;
    }
#line 2702 "CSSGrammar.tab.c"
    break;

  case 120:
#line 846 "CSSGrammar.y"
                                                                                  {
        (yyval.selector) = static_cast<CSSParser*>(parser)->createFloatingSelector();
        (yyval.selector)->m_attr = QualifiedName(nullAtom, atomicString((yyvsp[-5].string)), nullAtom);
        (yyval.selector)->m_match = (CSSSelector::Match)(yyvsp[-4].i);
        (yyval.selector)->m_value = atomicString((yyvsp[-2].string));
    }
#line 2713 "CSSGrammar.tab.c"
    break;

  case 121:
#line 852 "CSSGrammar.y"
                                                       {
        AtomicString namespacePrefix = atomicString((yyvsp[-2].string));
        CSSParser* p = static_cast<CSSParser*>(parser);
        (yyval.selector) = p->createFloatingSelector();
        (yyval.selector)->m_attr = QualifiedName(namespacePrefix,
                                   atomicString((yyvsp[-1].string)),
                                   static_cast<CSSStyleSheet*>(p->styleElement)->determineNamespace(namespacePrefix));
        (yyval.selector)->m_match = CSSSelector::Set;
    }
#line 2727 "CSSGrammar.tab.c"
    break;

  case 122:
#line 861 "CSSGrammar.y"
                                                                                                     {
        AtomicString namespacePrefix = atomicString((yyvsp[-6].string));
        CSSParser* p = static_cast<CSSParser*>(parser);
        (yyval.selector) = p->createFloatingSelector();
        (yyval.selector)->m_attr = QualifiedName(namespacePrefix,
                                   atomicString((yyvsp[-5].string)),
                                   static_cast<CSSStyleSheet*>(p->styleElement)->determineNamespace(namespacePrefix));
        (yyval.selector)->m_match = (CSSSelector::Match)(yyvsp[-4].i);
        (yyval.selector)->m_value = atomicString((yyvsp[-2].string));
    }
#line 2742 "CSSGrammar.tab.c"
    break;

  case 123:
#line 874 "CSSGrammar.y"
        {
        (yyval.i) = CSSSelector::Exact;
    }
#line 2750 "CSSGrammar.tab.c"
    break;

  case 124:
#line 877 "CSSGrammar.y"
               {
        (yyval.i) = CSSSelector::List;
    }
#line 2758 "CSSGrammar.tab.c"
    break;

  case 125:
#line 880 "CSSGrammar.y"
                {
        (yyval.i) = CSSSelector::Hyphen;
    }
#line 2766 "CSSGrammar.tab.c"
    break;

  case 126:
#line 883 "CSSGrammar.y"
                 {
        (yyval.i) = CSSSelector::Begin;
    }
#line 2774 "CSSGrammar.tab.c"
    break;

  case 127:
#line 886 "CSSGrammar.y"
               {
        (yyval.i) = CSSSelector::End;
    }
#line 2782 "CSSGrammar.tab.c"
    break;

  case 128:
#line 889 "CSSGrammar.y"
               {
        (yyval.i) = CSSSelector::Contain;
    }
#line 2790 "CSSGrammar.tab.c"
    break;

  case 131:
#line 900 "CSSGrammar.y"
              {
        (yyval.selector) = static_cast<CSSParser*>(parser)->createFloatingSelector();
        (yyval.selector)->m_match = CSSSelector::PseudoClass;
        (yyvsp[0].string).lower();
        (yyval.selector)->m_value = atomicString((yyvsp[0].string));
        CSSSelector::PseudoType type = (yyval.selector)->pseudoType();
        if (type == CSSSelector::PseudoUnknown)
            (yyval.selector) = 0;
        else if (type == CSSSelector::PseudoEmpty ||
                 type == CSSSelector::PseudoFirstChild ||
                 type == CSSSelector::PseudoFirstOfType ||
                 type == CSSSelector::PseudoLastChild ||
                 type == CSSSelector::PseudoLastOfType ||
                 type == CSSSelector::PseudoOnlyChild ||
                 type == CSSSelector::PseudoOnlyOfType) {
            CSSParser* p = static_cast<CSSParser*>(parser);
            Document* doc = p->document();
            if (doc)
                doc->setUsesSiblingRules(true);
        } else if (type == CSSSelector::PseudoFirstLine) {
            CSSParser* p = static_cast<CSSParser*>(parser);
            if (Document* doc = p->document())
                doc->setUsesFirstLineRules(true);
        }
    }
#line 2820 "CSSGrammar.tab.c"
    break;

  case 132:
#line 925 "CSSGrammar.y"
                    {
        (yyval.selector) = static_cast<CSSParser*>(parser)->createFloatingSelector();
        (yyval.selector)->m_match = CSSSelector::PseudoElement;
        (yyvsp[0].string).lower();
        (yyval.selector)->m_value = atomicString((yyvsp[0].string));
        CSSSelector::PseudoType type = (yyval.selector)->pseudoType();
        if (type == CSSSelector::PseudoUnknown)
            (yyval.selector) = 0;
        else if (type == CSSSelector::PseudoFirstLine) {
            CSSParser* p = static_cast<CSSParser*>(parser);
            if (Document* doc = p->document())
                doc->setUsesFirstLineRules(true);
        }
    }
#line 2839 "CSSGrammar.tab.c"
    break;

  case 133:
#line 940 "CSSGrammar.y"
                             {
        (yyval.selector) = static_cast<CSSParser*>(parser)->createFloatingSelector();
        (yyval.selector)->m_match = CSSSelector::PseudoClass;
        (yyval.selector)->m_argument = atomicString((yyvsp[-1].string));
        (yyvsp[-2].string).lower();
        (yyval.selector)->m_value = atomicString((yyvsp[-2].string));
        if ((yyval.selector)->pseudoType() == CSSSelector::PseudoUnknown)
            (yyval.selector) = 0;
    }
#line 2853 "CSSGrammar.tab.c"
    break;

  case 134:
#line 950 "CSSGrammar.y"
                                                       {
        (yyval.selector) = static_cast<CSSParser*>(parser)->createFloatingSelector();
        (yyval.selector)->m_match = CSSSelector::PseudoClass;
        (yyval.selector)->m_argument = String::number((int)(yyvsp[-2].val));
        (yyvsp[-4].string).lower();
        (yyval.selector)->m_value = atomicString((yyvsp[-4].string));
        if ((yyval.selector)->pseudoType() == CSSSelector::PseudoUnknown)
            (yyval.selector) = 0;
    }
#line 2867 "CSSGrammar.tab.c"
    break;

  case 135:
#line 960 "CSSGrammar.y"
                                                     {
        (yyval.selector) = static_cast<CSSParser*>(parser)->createFloatingSelector();
        (yyval.selector)->m_match = CSSSelector::PseudoClass;
        (yyval.selector)->m_argument = atomicString((yyvsp[-2].string));
        (yyvsp[-4].string).lower();
        (yyval.selector)->m_value = atomicString((yyvsp[-4].string));
        if ((yyval.selector)->pseudoType() == CSSSelector::PseudoUnknown)
            (yyval.selector) = 0;
    }
#line 2881 "CSSGrammar.tab.c"
    break;

  case 136:
#line 971 "CSSGrammar.y"
                                                                                         {
        (yyval.selector) = static_cast<CSSParser*>(parser)->createFloatingSelector();
        (yyval.selector)->m_match = CSSSelector::PseudoClass;
        (yyval.selector)->m_argument = domString((yyvsp[-6].string)) + "+" + String::number((int)(yyvsp[-2].val));
        (yyvsp[-8].string).lower();
        (yyval.selector)->m_value = atomicString((yyvsp[-8].string));
        if ((yyval.selector)->pseudoType() == CSSSelector::PseudoUnknown)
            (yyval.selector) = 0;
    }
#line 2895 "CSSGrammar.tab.c"
    break;

  case 137:
#line 980 "CSSGrammar.y"
                                                                                         {
        (yyval.selector) = static_cast<CSSParser*>(parser)->createFloatingSelector();
        (yyval.selector)->m_match = CSSSelector::PseudoClass;
        (yyval.selector)->m_argument = domString((yyvsp[-6].string)) + "-" + String::number((int)(yyvsp[-2].val));
        (yyvsp[-8].string).lower();
        (yyval.selector)->m_value = atomicString((yyvsp[-8].string));
        if ((yyval.selector)->pseudoType() == CSSSelector::PseudoUnknown)
            (yyval.selector) = 0;
    }
#line 2909 "CSSGrammar.tab.c"
    break;

  case 138:
#line 990 "CSSGrammar.y"
                                                      {
        if (!(yyvsp[-1].selector))
            (yyval.selector) = 0;
        else {
            CSSParser* p = static_cast<CSSParser*>(parser);
            (yyval.selector) = p->createFloatingSelector();
            (yyval.selector)->m_match = CSSSelector::PseudoClass;
            (yyval.selector)->m_simpleSelector = p->sinkFloatingSelector((yyvsp[-1].selector));
            (yyvsp[-3].string).lower();
            (yyval.selector)->m_value = atomicString((yyvsp[-3].string));
        }
    }
#line 2926 "CSSGrammar.tab.c"
    break;

  case 139:
#line 1007 "CSSGrammar.y"
                                                   {
        CSSParser* p = static_cast<CSSParser*>(parser);
        if (!(yyvsp[-1].selector))
            (yyval.selector) = 0;
        else {
            (yyval.selector) = p->createFloatingSelector();
            (yyval.selector)->m_match = CSSSelector::PseudoClass;
            (yyval.selector)->m_simpleSelector = p->sinkFloatingSelector((yyvsp[-1].selector));
            (yyval.selector)->m_value = AtomicString("is(");
        }
    }
#line 2942 "CSSGrammar.tab.c"
    break;

  case 140:
#line 1018 "CSSGrammar.y"
                                                      {
        CSSParser* p = static_cast<CSSParser*>(parser);
        if (!(yyvsp[-1].selector))
            (yyval.selector) = 0;
        else {
            (yyval.selector) = p->createFloatingSelector();
            (yyval.selector)->m_match = CSSSelector::PseudoClass;
            (yyval.selector)->m_simpleSelector = p->sinkFloatingSelector((yyvsp[-1].selector));
            (yyval.selector)->m_value = AtomicString("where(");
        }
    }
#line 2958 "CSSGrammar.tab.c"
    break;

  case 141:
#line 1029 "CSSGrammar.y"
                                                    {
        CSSParser* p = static_cast<CSSParser*>(parser);
        if (!(yyvsp[-1].selector))
            (yyval.selector) = 0;
        else {
            (yyval.selector) = p->createFloatingSelector();
            (yyval.selector)->m_match = CSSSelector::PseudoClass;
            (yyval.selector)->m_simpleSelector = p->sinkFloatingSelector((yyvsp[-1].selector));
            (yyval.selector)->m_value = AtomicString("has(");
            if (Document* doc = p->document())
                doc->setUsesSiblingRules(true); // :has re-eval on subtree changes
        }
    }
#line 2976 "CSSGrammar.tab.c"
    break;

  case 142:
#line 1045 "CSSGrammar.y"
                {
        (yyval.ok) = (yyvsp[0].ok);
    }
#line 2984 "CSSGrammar.tab.c"
    break;

  case 143:
#line 1048 "CSSGrammar.y"
                            {
        (yyval.ok) = (yyvsp[-1].ok);
        if ( (yyvsp[0].ok) )
            (yyval.ok) = (yyvsp[0].ok);
    }
#line 2994 "CSSGrammar.tab.c"
    break;

  case 144:
#line 1053 "CSSGrammar.y"
                {
        (yyval.ok) = (yyvsp[0].ok);
    }
#line 3002 "CSSGrammar.tab.c"
    break;

  case 145:
#line 1056 "CSSGrammar.y"
                                     {
        (yyval.ok) = false;
    }
#line 3010 "CSSGrammar.tab.c"
    break;

  case 146:
#line 1059 "CSSGrammar.y"
            {
        (yyval.ok) = false;
    }
#line 3018 "CSSGrammar.tab.c"
    break;

  case 147:
#line 1062 "CSSGrammar.y"
                      {
        (yyval.ok) = (yyvsp[-1].ok);
    }
#line 3026 "CSSGrammar.tab.c"
    break;

  case 148:
#line 1068 "CSSGrammar.y"
                                {
        (yyval.ok) = (yyvsp[-2].ok);
    }
#line 3034 "CSSGrammar.tab.c"
    break;

  case 149:
#line 1071 "CSSGrammar.y"
                                                     {
        (yyval.ok) = false;
    }
#line 3042 "CSSGrammar.tab.c"
    break;

  case 150:
#line 1074 "CSSGrammar.y"
                            {
        (yyval.ok) = false;
    }
#line 3050 "CSSGrammar.tab.c"
    break;

  case 151:
#line 1077 "CSSGrammar.y"
                                                     {
        (yyval.ok) = false;
    }
#line 3058 "CSSGrammar.tab.c"
    break;

  case 152:
#line 1080 "CSSGrammar.y"
                                            {
        (yyval.ok) = (yyvsp[-3].ok);
        if ((yyvsp[-2].ok))
            (yyval.ok) = (yyvsp[-2].ok);
    }
#line 3068 "CSSGrammar.tab.c"
    break;

  case 153:
#line 1085 "CSSGrammar.y"
                                      {
        (yyval.ok) = (yyvsp[-3].ok);
    }
#line 3076 "CSSGrammar.tab.c"
    break;

  case 154:
#line 1088 "CSSGrammar.y"
                                                               {
        (yyval.ok) = (yyvsp[-5].ok);
    }
#line 3084 "CSSGrammar.tab.c"
    break;

  case 155:
#line 1094 "CSSGrammar.y"
                                       {
        (yyval.ok) = false;
        CSSParser* p = static_cast<CSSParser*>(parser);
        if ((yyvsp[-4].prop_id) && (yyvsp[-1].valueList)) {
            p->valueList = p->sinkFloatingValueList((yyvsp[-1].valueList));
            int oldParsedProperties = p->numParsedProperties;
            (yyval.ok) = p->parseValue((yyvsp[-4].prop_id), (yyvsp[0].b));
            if (!(yyval.ok))
                p->rollbackLastProperties(p->numParsedProperties - oldParsedProperties);
            delete p->valueList;
            p->valueList = 0;
        }
    }
#line 3102 "CSSGrammar.tab.c"
    break;

  case 156:
#line 1108 "CSSGrammar.y"
                                        {
        /* Custom property "--name: value": "--name" now tokenizes as a single
           IDENT (tokenizer customprop rule) and is handled by the regular
           "property ':' expr" path, which routes it to CSS_PROP_CUSTOM_PROPERTY.
           This legacy rule is retained only to gracefully consume any stray
           single-'-' sequence without aborting the stylesheet. */
        (yyval.ok) = false;
    }
#line 3115 "CSSGrammar.tab.c"
    break;

  case 157:
#line 1117 "CSSGrammar.y"
                   {
        (yyval.ok) = false;
    }
#line 3123 "CSSGrammar.tab.c"
    break;

  case 158:
#line 1121 "CSSGrammar.y"
                                             {
        /* The default movable type template has letter-spacing: .none;  Handle this by looking for
        error tokens at the start of an expr, recover the expr and then treat as an error, cleaning
        up and deleting the shifted expr.  */
        (yyval.ok) = false;
    }
#line 3134 "CSSGrammar.tab.c"
    break;

  case 159:
#line 1128 "CSSGrammar.y"
                              {
        /* Handle this case: div { text-align: center; !important } Just reduce away the stray !important. */
        (yyval.ok) = false;
    }
#line 3143 "CSSGrammar.tab.c"
    break;

  case 160:
#line 1133 "CSSGrammar.y"
                             {
        /* div { font-family: } Just reduce away this property with no value. */
        (yyval.ok) = false;
    }
#line 3152 "CSSGrammar.tab.c"
    break;

  case 161:
#line 1140 "CSSGrammar.y"
                      {
        (yyvsp[-1].string).lower();
        String str = domString((yyvsp[-1].string));
        CSSParser* p = static_cast<CSSParser*>(parser);
        // A custom property name "--foo" arrives as a single IDENT (tokenizer
        // customprop rule). Route it to the custom-property id and remember the
        // name for parseValue.
        if (str.length() > 2 && str[0] == '-' && str[1] == '-') {
            p->m_currentCustomPropertyName = str;
            (yyval.prop_id) = CSS_PROP_CUSTOM_PROPERTY;
        } else {
            p->m_currentCustomPropertyName = String();
            CString bytes = str.latin1();
            (yyval.prop_id) = getPropertyID(bytes.data(), bytes.length());
        }
    }
#line 3173 "CSSGrammar.tab.c"
    break;

  case 162:
#line 1159 "CSSGrammar.y"
                              { (yyval.b) = true; }
#line 3179 "CSSGrammar.tab.c"
    break;

  case 163:
#line 1160 "CSSGrammar.y"
                  { (yyval.b) = false; }
#line 3185 "CSSGrammar.tab.c"
    break;

  case 164:
#line 1164 "CSSGrammar.y"
         {
        CSSParser* p = static_cast<CSSParser*>(parser);
        (yyval.valueList) = p->createFloatingValueList();
        (yyval.valueList)->addValue(p->sinkFloatingValue((yyvsp[0].value)));
    }
#line 3195 "CSSGrammar.tab.c"
    break;

  case 165:
#line 1169 "CSSGrammar.y"
                         {
        CSSParser* p = static_cast<CSSParser*>(parser);
        (yyval.valueList) = (yyvsp[-2].valueList);
        if ((yyval.valueList)) {
            if ((yyvsp[-1].tok)) {
                Value v;
                v.id = 0;
                v.unit = Value::Operator;
                v.iValue = (yyvsp[-1].tok);
                (yyval.valueList)->addValue(v);
            }
            (yyval.valueList)->addValue(p->sinkFloatingValue((yyvsp[0].value)));
        }
    }
#line 3214 "CSSGrammar.tab.c"
    break;

  case 166:
#line 1183 "CSSGrammar.y"
                 {
        (yyval.valueList) = 0;
    }
#line 3222 "CSSGrammar.tab.c"
    break;

  case 167:
#line 1189 "CSSGrammar.y"
                    {
        (yyval.tok) = '/';
    }
#line 3230 "CSSGrammar.tab.c"
    break;

  case 168:
#line 1192 "CSSGrammar.y"
                    {
        (yyval.tok) = ',';
    }
#line 3238 "CSSGrammar.tab.c"
    break;

  case 169:
#line 1195 "CSSGrammar.y"
                    {
        (yyval.tok) = '+';
    }
#line 3246 "CSSGrammar.tab.c"
    break;

  case 170:
#line 1198 "CSSGrammar.y"
                    {
        (yyval.tok) = '-';
    }
#line 3254 "CSSGrammar.tab.c"
    break;

  case 171:
#line 1201 "CSSGrammar.y"
                    {
        (yyval.tok) = '*';
    }
#line 3262 "CSSGrammar.tab.c"
    break;

  case 172:
#line 1204 "CSSGrammar.y"
                {
        (yyval.tok) = 0;
  }
#line 3270 "CSSGrammar.tab.c"
    break;

  case 173:
#line 1210 "CSSGrammar.y"
             { (yyval.value) = (yyvsp[0].value); }
#line 3276 "CSSGrammar.tab.c"
    break;

  case 174:
#line 1211 "CSSGrammar.y"
                              { (yyval.value) = (yyvsp[0].value); (yyval.value).fValue *= (yyvsp[-1].i); }
#line 3282 "CSSGrammar.tab.c"
    break;

  case 175:
#line 1212 "CSSGrammar.y"
                       { (yyval.value).id = 0; (yyval.value).string = (yyvsp[-1].string); (yyval.value).unit = CSSPrimitiveValue::CSS_STRING; }
#line 3288 "CSSGrammar.tab.c"
    break;

  case 176:
#line 1213 "CSSGrammar.y"
                      {
      String str = domString((yyvsp[-1].string));
      CString bytes = str.lower().latin1();
      (yyval.value).id = getValueID(bytes.data(), bytes.length());
      (yyval.value).unit = CSSPrimitiveValue::CSS_IDENT;
      (yyval.value).string = (yyvsp[-1].string);
  }
#line 3300 "CSSGrammar.tab.c"
    break;

  case 177:
#line 1221 "CSSGrammar.y"
                      { (yyval.value).id = 0; (yyval.value).string = (yyvsp[-1].string); (yyval.value).unit = CSSPrimitiveValue::CSS_DIMENSION; }
#line 3306 "CSSGrammar.tab.c"
    break;

  case 178:
#line 1222 "CSSGrammar.y"
                                     { (yyval.value).id = 0; (yyval.value).string = (yyvsp[-1].string); (yyval.value).unit = CSSPrimitiveValue::CSS_DIMENSION; }
#line 3312 "CSSGrammar.tab.c"
    break;

  case 179:
#line 1223 "CSSGrammar.y"
                    { (yyval.value).id = 0; (yyval.value).string = (yyvsp[-1].string); (yyval.value).unit = CSSPrimitiveValue::CSS_URI; }
#line 3318 "CSSGrammar.tab.c"
    break;

  case 180:
#line 1224 "CSSGrammar.y"
                             { (yyval.value).id = 0; (yyval.value).iValue = 0; (yyval.value).unit = CSSPrimitiveValue::CSS_UNKNOWN;/* ### */ }
#line 3324 "CSSGrammar.tab.c"
    break;

  case 181:
#line 1225 "CSSGrammar.y"
             { (yyval.value).id = 0; (yyval.value).string = (yyvsp[0].string); (yyval.value).unit = CSSPrimitiveValue::CSS_RGBCOLOR; }
#line 3330 "CSSGrammar.tab.c"
    break;

  case 182:
#line 1226 "CSSGrammar.y"
                    { (yyval.value).id = 0; (yyval.value).string = ParseString(); (yyval.value).unit = CSSPrimitiveValue::CSS_RGBCOLOR; }
#line 3336 "CSSGrammar.tab.c"
    break;

  case 183:
#line 1228 "CSSGrammar.y"
             {
      (yyval.value) = (yyvsp[0].value);
  }
#line 3344 "CSSGrammar.tab.c"
    break;

  case 184:
#line 1231 "CSSGrammar.y"
                    {}
#line 3350 "CSSGrammar.tab.c"
    break;

  case 185:
#line 1235 "CSSGrammar.y"
                      { (yyval.value).id = 0; (yyval.value).isInt = true; (yyval.value).fValue = (yyvsp[-1].val); (yyval.value).unit = CSSPrimitiveValue::CSS_NUMBER; }
#line 3356 "CSSGrammar.tab.c"
    break;

  case 186:
#line 1236 "CSSGrammar.y"
                           { (yyval.value).id = 0; (yyval.value).isInt = false; (yyval.value).fValue = (yyvsp[-1].val); (yyval.value).unit = CSSPrimitiveValue::CSS_NUMBER; }
#line 3362 "CSSGrammar.tab.c"
    break;

  case 187:
#line 1237 "CSSGrammar.y"
                           { (yyval.value).id = 0; (yyval.value).fValue = (yyvsp[-1].val); (yyval.value).unit = CSSPrimitiveValue::CSS_PERCENTAGE; }
#line 3368 "CSSGrammar.tab.c"
    break;

  case 188:
#line 1238 "CSSGrammar.y"
                    { (yyval.value).id = 0; (yyval.value).fValue = (yyvsp[-1].val); (yyval.value).unit = CSSPrimitiveValue::CSS_PX; }
#line 3374 "CSSGrammar.tab.c"
    break;

  case 189:
#line 1239 "CSSGrammar.y"
                    { (yyval.value).id = 0; (yyval.value).fValue = (yyvsp[-1].val); (yyval.value).unit = CSSPrimitiveValue::CSS_CM; }
#line 3380 "CSSGrammar.tab.c"
    break;

  case 190:
#line 1240 "CSSGrammar.y"
                    { (yyval.value).id = 0; (yyval.value).fValue = (yyvsp[-1].val); (yyval.value).unit = CSSPrimitiveValue::CSS_MM; }
#line 3386 "CSSGrammar.tab.c"
    break;

  case 191:
#line 1241 "CSSGrammar.y"
                    { (yyval.value).id = 0; (yyval.value).fValue = (yyvsp[-1].val); (yyval.value).unit = CSSPrimitiveValue::CSS_IN; }
#line 3392 "CSSGrammar.tab.c"
    break;

  case 192:
#line 1242 "CSSGrammar.y"
                    { (yyval.value).id = 0; (yyval.value).fValue = (yyvsp[-1].val); (yyval.value).unit = CSSPrimitiveValue::CSS_PT; }
#line 3398 "CSSGrammar.tab.c"
    break;

  case 193:
#line 1243 "CSSGrammar.y"
                    { (yyval.value).id = 0; (yyval.value).fValue = (yyvsp[-1].val); (yyval.value).unit = CSSPrimitiveValue::CSS_PC; }
#line 3404 "CSSGrammar.tab.c"
    break;

  case 194:
#line 1244 "CSSGrammar.y"
                     { (yyval.value).id = 0; (yyval.value).fValue = (yyvsp[-1].val); (yyval.value).unit = CSSPrimitiveValue::CSS_DEG; }
#line 3410 "CSSGrammar.tab.c"
    break;

  case 195:
#line 1245 "CSSGrammar.y"
                     { (yyval.value).id = 0; (yyval.value).fValue = (yyvsp[-1].val); (yyval.value).unit = CSSPrimitiveValue::CSS_RAD; }
#line 3416 "CSSGrammar.tab.c"
    break;

  case 196:
#line 1246 "CSSGrammar.y"
                      { (yyval.value).id = 0; (yyval.value).fValue = (yyvsp[-1].val); (yyval.value).unit = CSSPrimitiveValue::CSS_GRAD; }
#line 3422 "CSSGrammar.tab.c"
    break;

  case 197:
#line 1247 "CSSGrammar.y"
                      { (yyval.value).id = 0; (yyval.value).fValue = (yyvsp[-1].val); (yyval.value).unit = CSSPrimitiveValue::CSS_MS; }
#line 3428 "CSSGrammar.tab.c"
    break;

  case 198:
#line 1248 "CSSGrammar.y"
                     { (yyval.value).id = 0; (yyval.value).fValue = (yyvsp[-1].val); (yyval.value).unit = CSSPrimitiveValue::CSS_S; }
#line 3434 "CSSGrammar.tab.c"
    break;

  case 199:
#line 1249 "CSSGrammar.y"
                     { (yyval.value).id = 0; (yyval.value).fValue = (yyvsp[-1].val); (yyval.value).unit = CSSPrimitiveValue::CSS_HZ; }
#line 3440 "CSSGrammar.tab.c"
    break;

  case 200:
#line 1250 "CSSGrammar.y"
                      { (yyval.value).id = 0; (yyval.value).fValue = (yyvsp[-1].val); (yyval.value).unit = CSSPrimitiveValue::CSS_KHZ; }
#line 3446 "CSSGrammar.tab.c"
    break;

  case 201:
#line 1251 "CSSGrammar.y"
                    { (yyval.value).id = 0; (yyval.value).fValue = (yyvsp[-1].val); (yyval.value).unit = CSSPrimitiveValue::CSS_EMS; }
#line 3452 "CSSGrammar.tab.c"
    break;

  case 202:
#line 1252 "CSSGrammar.y"
                     { (yyval.value).id = 0; (yyval.value).fValue = (yyvsp[-1].val); (yyval.value).unit = Value::Q_EMS; }
#line 3458 "CSSGrammar.tab.c"
    break;

  case 203:
#line 1253 "CSSGrammar.y"
                    { (yyval.value).id = 0; (yyval.value).fValue = (yyvsp[-1].val); (yyval.value).unit = CSSPrimitiveValue::CSS_EXS; }
#line 3464 "CSSGrammar.tab.c"
    break;

  case 204:
#line 1258 "CSSGrammar.y"
                                              {
        CSSParser* p = static_cast<CSSParser*>(parser);
        Function* f = p->createFloatingFunction();
        f->name = (yyvsp[-4].string);
        f->args = p->sinkFloatingValueList((yyvsp[-2].valueList));
        (yyval.value).id = 0;
        (yyval.value).unit = Value::QFunction;
        (yyval.value).function = f;
    }
#line 3478 "CSSGrammar.tab.c"
    break;

  case 205:
#line 1267 "CSSGrammar.y"
                               {
        CSSParser* p = static_cast<CSSParser*>(parser);
        Function* f = p->createFloatingFunction();
        f->name = (yyvsp[-2].string);
        f->args = 0;
        (yyval.value).id = 0;
        (yyval.value).unit = Value::QFunction;
        (yyval.value).function = f;
  }
#line 3492 "CSSGrammar.tab.c"
    break;

  case 206:
#line 1283 "CSSGrammar.y"
                  { (yyval.string) = (yyvsp[-1].string); }
#line 3498 "CSSGrammar.tab.c"
    break;

  case 207:
#line 1284 "CSSGrammar.y"
                      { (yyval.string) = (yyvsp[-1].string); }
#line 3504 "CSSGrammar.tab.c"
    break;

  case 208:
#line 1291 "CSSGrammar.y"
                            {
        (yyval.rule) = 0;
    }
#line 3512 "CSSGrammar.tab.c"
    break;

  case 209:
#line 1294 "CSSGrammar.y"
                  {
        (yyval.rule) = 0;
    }
#line 3520 "CSSGrammar.tab.c"
    break;

  case 210:
#line 1300 "CSSGrammar.y"
           {
        (yyval.rule) = 0;
    }
#line 3528 "CSSGrammar.tab.c"
    break;

  case 211:
#line 1306 "CSSGrammar.y"
                        {
        (yyval.rule) = 0;
    }
#line 3536 "CSSGrammar.tab.c"
    break;


#line 3540 "CSSGrammar.tab.c"

      default: break;
    }
  /* User semantic actions sometimes alter yychar, and that requires
     that yytoken be updated with the new translation.  We take the
     approach of translating immediately before every use of yytoken.
     One alternative is translating here after every semantic action,
     but that translation would be missed if the semantic action invokes
     YYABORT, YYACCEPT, or YYERROR immediately after altering yychar or
     if it invokes YYBACKUP.  In the case of YYABORT or YYACCEPT, an
     incorrect destructor might then be invoked immediately.  In the
     case of YYERROR or YYBACKUP, subsequent parser actions might lead
     to an incorrect destructor call or verbose syntax error message
     before the lookahead is translated.  */
  YY_SYMBOL_PRINT ("-> $$ =", yyr1[yyn], &yyval, &yyloc);

  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);

  *++yyvsp = yyval;

  /* Now 'shift' the result of the reduction.  Determine what state
     that goes to, based on the state we popped back to and the rule
     number reduced by.  */
  {
    const int yylhs = yyr1[yyn] - YYNTOKENS;
    const int yyi = yypgoto[yylhs] + *yyssp;
    yystate = (0 <= yyi && yyi <= YYLAST && yycheck[yyi] == *yyssp
               ? yytable[yyi]
               : yydefgoto[yylhs]);
  }

  goto yynewstate;


/*--------------------------------------.
| yyerrlab -- here on detecting error.  |
`--------------------------------------*/
yyerrlab:
  /* Make sure we have latest lookahead translation.  See comments at
     user semantic actions for why this is necessary.  */
  yytoken = yychar == YYEMPTY ? YYEMPTY : YYTRANSLATE (yychar);

  /* If not already recovering from an error, report this error.  */
  if (!yyerrstatus)
    {
      ++yynerrs;
#if ! YYERROR_VERBOSE
      yyerror (parser, YY_("syntax error"));
#else
# define YYSYNTAX_ERROR yysyntax_error (&yymsg_alloc, &yymsg, \
                                        yyssp, yytoken)
      {
        char const *yymsgp = YY_("syntax error");
        int yysyntax_error_status;
        yysyntax_error_status = YYSYNTAX_ERROR;
        if (yysyntax_error_status == 0)
          yymsgp = yymsg;
        else if (yysyntax_error_status == 1)
          {
            if (yymsg != yymsgbuf)
              YYSTACK_FREE (yymsg);
            yymsg = YY_CAST (char *, YYSTACK_ALLOC (YY_CAST (YYSIZE_T, yymsg_alloc)));
            if (!yymsg)
              {
                yymsg = yymsgbuf;
                yymsg_alloc = sizeof yymsgbuf;
                yysyntax_error_status = 2;
              }
            else
              {
                yysyntax_error_status = YYSYNTAX_ERROR;
                yymsgp = yymsg;
              }
          }
        yyerror (parser, yymsgp);
        if (yysyntax_error_status == 2)
          goto yyexhaustedlab;
      }
# undef YYSYNTAX_ERROR
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
                      yytoken, &yylval, parser);
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
  /* Pacify compilers when the user code never invokes YYERROR and the
     label yyerrorlab therefore never appears in user code.  */
  if (0)
    YYERROR;

  /* Do not reclaim the symbols of the rule whose action triggered
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
  yyerrstatus = 3;      /* Each real token shifted decrements this.  */

  for (;;)
    {
      yyn = yypact[yystate];
      if (!yypact_value_is_default (yyn))
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
                  yystos[yystate], yyvsp, parser);
      YYPOPSTACK (1);
      yystate = *yyssp;
      YY_STACK_PRINT (yyss, yyssp);
    }

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END


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


#if !defined yyoverflow || YYERROR_VERBOSE
/*-------------------------------------------------.
| yyexhaustedlab -- memory exhaustion comes here.  |
`-------------------------------------------------*/
yyexhaustedlab:
  yyerror (parser, YY_("memory exhausted"));
  yyresult = 2;
  /* Fall through.  */
#endif


/*-----------------------------------------------------.
| yyreturn -- parsing is finished, return the result.  |
`-----------------------------------------------------*/
yyreturn:
  if (yychar != YYEMPTY)
    {
      /* Make sure we have latest lookahead translation.  See comments at
         user semantic actions for why this is necessary.  */
      yytoken = YYTRANSLATE (yychar);
      yydestruct ("Cleanup: discarding lookahead",
                  yytoken, &yylval, parser);
    }
  /* Do not reclaim the symbols of the rule whose action triggered
     this YYABORT or YYACCEPT.  */
  YYPOPSTACK (yylen);
  YY_STACK_PRINT (yyss, yyssp);
  while (yyssp != yyss)
    {
      yydestruct ("Cleanup: popping",
                  yystos[+*yyssp], yyvsp, parser);
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
  return yyresult;
}
#line 1332 "CSSGrammar.y"

