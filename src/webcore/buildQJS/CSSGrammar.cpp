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
    CHARSET_SYM = 274,
    NAMESPACE_SYM = 275,
    WEBKIT_RULE_SYM = 276,
    WEBKIT_DECLS_SYM = 277,
    WEBKIT_VALUE_SYM = 278,
    WEBKIT_MEDIAQUERY_SYM = 279,
    IMPORTANT_SYM = 280,
    MEDIA_ONLY = 281,
    MEDIA_NOT = 282,
    MEDIA_AND = 283,
    QEMS = 284,
    EMS = 285,
    EXS = 286,
    PXS = 287,
    CMS = 288,
    MMS = 289,
    INS = 290,
    PTS = 291,
    PCS = 292,
    DEGS = 293,
    RADS = 294,
    GRADS = 295,
    MSECS = 296,
    SECS = 297,
    HERZ = 298,
    KHERZ = 299,
    DIMEN = 300,
    PERCENTAGE = 301,
    FLOATTOKEN = 302,
    INTEGER = 303,
    URI = 304,
    FUNCTION = 305,
    NOTFUNCTION = 306,
    ISFUNCTION = 307,
    WHEREFUNCTION = 308,
    HASFUNCTION = 309,
    UNICODERANGE = 310
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

#line 327 "CSSGrammar.tab.c"

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


#line 349 "CSSGrammar.tab.c"


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
#define YYLAST   990

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  77
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  65
/* YYNRULES -- Number of rules.  */
#define YYNRULES  203
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  410

#define YYUNDEFTOK  2
#define YYMAXUTOK   310


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
       2,     2,     2,     2,     2,    74,     2,    75,     2,     2,
      64,    65,    18,    67,    66,    70,    16,    73,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,    15,    63,
       2,    72,    69,     2,    76,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,    17,     2,    71,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,    61,    19,    62,    68,     2,     2,     2,
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
      60
};

#if YYDEBUG
  /* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
static const yytype_int16 yyrline[] =
{
       0,   294,   294,   295,   296,   297,   298,   302,   303,   307,
     313,   319,   333,   340,   341,   344,   346,   347,   350,   352,
     357,   363,   365,   369,   371,   378,   380,   383,   385,   393,
     394,   395,   396,   397,   398,   399,   403,   406,   409,   415,
     420,   421,   425,   426,   430,   431,   435,   441,   444,   450,
     457,   462,   469,   472,   476,   479,   482,   488,   496,   499,
     503,   508,   513,   519,   522,   528,   529,   540,   557,   560,
     566,   570,   573,   579,   580,   581,   582,   586,   587,   591,
     597,   600,   608,   614,   617,   637,   643,   644,   645,   649,
     654,   661,   667,   678,   691,   705,   713,   721,   724,   737,
     743,   752,   765,   766,   767,   771,   783,   794,   799,   805,
     814,   827,   830,   833,   836,   839,   842,   848,   849,   853,
     878,   893,   903,   913,   924,   933,   943,   960,   971,   982,
     998,  1001,  1006,  1009,  1012,  1015,  1021,  1024,  1027,  1030,
    1033,  1038,  1041,  1047,  1061,  1070,  1074,  1081,  1086,  1093,
    1112,  1113,  1117,  1122,  1136,  1142,  1145,  1148,  1151,  1154,
    1157,  1163,  1164,  1165,  1166,  1174,  1175,  1176,  1177,  1178,
    1179,  1181,  1184,  1188,  1189,  1190,  1191,  1192,  1193,  1194,
    1195,  1196,  1197,  1198,  1199,  1200,  1201,  1202,  1203,  1204,
    1205,  1206,  1211,  1220,  1236,  1237,  1244,  1247,  1253,  1259,
    1276,  1277,  1281,  1282
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
  "IMPORT_SYM", "PAGE_SYM", "MEDIA_SYM", "FONT_FACE_SYM", "CHARSET_SYM",
  "NAMESPACE_SYM", "WEBKIT_RULE_SYM", "WEBKIT_DECLS_SYM",
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
  "combinator", "unary_operator", "ruleset", "selector_list", "selector",
  "namespace_selector", "simple_selector", "element_name",
  "specifier_list", "specifier", "class", "attr_name", "attrib", "match",
  "ident_or_string", "pseudo", "declaration_list", "decl_list",
  "declaration", "property", "prio", "expr", "operator", "term",
  "unary_term", "function", "hexcolor", "invalid_at", "invalid_import",
  "invalid_rule", "invalid_block", "invalid_block_list", YY_NULLPTR
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
     310,   123,   125,    59,    40,    41,    44,    43,   126,    62,
      45,    93,    61,    47,    35,    37,    64
};
# endif

#define YYPACT_NINF (-211)

#define yypact_value_is_default(Yyn) \
  ((Yyn) == YYPACT_NINF)

#define YYTABLE_NINF (-161)

#define yytable_value_is_error(Yyn) \
  0

  /* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
     STATE-NUM.  */
static const yytype_int16 yypact[] =
{
     290,    41,   -56,   -23,    -4,   128,   127,  -211,  -211,  -211,
    -211,  -211,  -211,   -29,    49,  -211,  -211,  -211,  -211,  -211,
     137,   137,   137,   137,    17,   144,  -211,  -211,  -211,  -211,
     588,    29,   767,    19,  -211,  -211,   136,   106,   133,   143,
    -211,  -211,   228,   161,  -211,   163,  -211,   270,  -211,  -211,
    -211,    47,   850,   292,  -211,   313,   831,  -211,  -211,  -211,
    -211,   164,  -211,  -211,   188,   145,   114,   181,    25,  -211,
    -211,  -211,  -211,  -211,  -211,  -211,  -211,  -211,  -211,  -211,
    -211,  -211,  -211,  -211,  -211,  -211,  -211,  -211,  -211,  -211,
    -211,  -211,  -211,  -211,  -211,  -211,  -211,  -211,  -211,  -211,
     937,   516,  -211,  -211,  -211,  -211,  -211,  -211,  -211,   168,
     193,  -211,  -211,  -211,   244,  -211,  -211,  -211,   240,   243,
    -211,  -211,  -211,  -211,  -211,   278,  -211,   231,   200,   148,
    -211,  -211,  -211,  -211,  -211,  -211,  -211,   597,  -211,  -211,
     313,   831,   137,   831,  -211,   137,  -211,  -211,   288,   137,
     137,   264,  -211,   260,   239,  -211,    43,  -211,  -211,   137,
     137,   137,   137,   137,   137,   137,   137,   137,   137,   137,
     137,   137,   137,   137,   137,   137,   137,   137,   137,   137,
     137,   137,   137,   137,   715,   137,   137,   137,  -211,  -211,
    -211,  -211,  -211,  -211,  -211,  -211,  -211,   895,   167,  -211,
     237,   246,  -211,    17,   214,  -211,   215,   173,   659,   659,
     659,   659,   143,   163,   300,    57,  -211,  -211,  -211,  -211,
    -211,  -211,    29,   659,   137,   137,   137,  -211,   137,   831,
     137,   137,   137,   271,  -211,  -211,   319,  -211,   137,   282,
    -211,   583,  -211,   648,   137,   137,   137,   137,   137,   137,
    -211,  -211,   298,   275,   217,   282,   345,  -211,    35,   349,
    -211,  -211,  -211,  -211,  -211,  -211,  -211,  -211,  -211,    17,
    -211,  -211,  -211,  -211,  -211,   294,    99,   115,   123,   137,
    -211,   141,  -211,  -211,  -211,  -211,  -211,  -211,  -211,  -211,
     154,   289,   859,   137,  -211,   767,   137,   276,   137,   137,
     895,   382,  -211,   137,  -211,  -211,   298,  -211,  -211,  -211,
     356,     2,  -211,   279,   174,   286,   159,   293,    17,   105,
      45,  -211,  -211,  -211,  -211,  -211,  -211,   266,  -211,   299,
     135,  -211,   137,   382,  -211,   382,  -211,  -211,   137,   119,
    -211,  -211,  -211,  -211,  -211,  -211,   142,  -211,  -211,  -211,
    -211,  -211,  -211,  -211,  -211,  -211,   266,  -211,  -211,  -211,
    -211,  -211,  -211,  -211,   137,  -211,   137,  -211,   134,   137,
    -211,    29,   156,   162,  -211,    31,    19,   220,  -211,   456,
     137,   301,  -211,  -211,    33,  -211,  -211,  -211,  -211,  -211,
    -211,   523,  -211,   149,   151,  -211,   137,    16,   137,  -211,
     137,  -211,  -211,  -211,   302,   767,  -211,   449,   137,   137
};

  /* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
     Performed when YYTABLE does not specify something else to do.  Zero
     means the default is an error.  */
static const yytype_uint8 yydefact[] =
{
      18,     0,     0,     0,     0,     0,     0,    13,    13,    13,
      13,    15,    19,     0,     0,    13,    13,    13,    13,     1,
       3,     4,     5,     6,    23,     0,    22,    21,    14,    13,
       0,     0,     0,    54,    17,    16,    25,     0,     0,    95,
     101,   100,     0,     0,    13,    96,    86,     0,    13,     8,
       7,     0,     0,     0,    83,    13,     0,    97,   102,   103,
     104,   134,    13,    13,     0,     0,     0,   130,     0,    13,
      13,    13,    13,    13,    13,    13,    13,    13,    13,    13,
      13,    13,    13,    13,    13,    13,    13,    13,    13,    13,
      13,    13,    13,    13,    13,    13,    78,    77,    13,    13,
       0,     0,   152,   161,   171,   169,    55,    56,    13,     0,
      27,    15,   201,   202,     0,    20,    88,   119,     0,    13,
      13,    13,    13,    13,   105,     0,    87,     0,     0,     0,
      82,    13,    13,    85,    13,    13,    13,     0,    95,    96,
      13,     0,    89,     0,    99,    91,    98,    13,     0,   149,
     147,     0,    10,   135,   131,    13,     0,   145,    13,   163,
     164,   194,   195,   190,   189,   191,   176,   177,   178,   179,
     180,   181,   182,   183,   184,   185,   186,   187,   188,   165,
     175,   174,   173,   167,     0,   168,   170,   172,    13,   162,
     154,    13,    11,    13,    13,    13,    13,     0,     0,    12,
       0,     0,    15,    24,     0,   120,     0,     0,     0,     0,
       0,     0,    13,     0,     0,     0,    38,    37,    44,    45,
      13,     9,     0,     0,    73,    74,    75,    84,    92,     0,
      94,    90,   138,   133,    13,    13,     0,    13,   136,     0,
      13,     0,   193,     0,   166,   159,   156,   157,   158,   155,
     153,    13,    52,     0,    42,     0,     0,    13,     0,     0,
      15,   198,    30,    31,    32,    29,    34,    35,    33,    26,
     200,   203,   121,    13,    13,     0,     0,     0,     0,   106,
      13,     0,   112,   113,   114,   115,   116,   107,   111,    13,
      54,     0,     0,    93,    13,     0,   141,     0,   140,   137,
       0,     0,    13,    67,    13,    50,    53,    57,    41,    40,
       0,     0,   199,     0,    54,     0,     0,     0,    28,     0,
       0,   126,   127,   128,   129,   109,    13,     0,    60,     0,
       0,    79,   139,     0,    13,     0,    13,   143,   192,     0,
      51,    43,    13,    69,    68,    13,     0,    72,    71,    13,
     197,   196,   123,    13,    13,   122,     0,   118,   117,    13,
      36,    62,    13,   144,   142,   146,   150,    13,     0,    65,
      13,     0,     0,     0,    13,     0,    54,     0,    39,     0,
      65,     0,    13,    13,     0,   108,    61,    13,    13,    64,
      13,     0,    13,     0,     0,   110,    46,    47,    66,    63,
      70,   124,   125,    13,     0,     0,    13,     0,    49,    48
};

  /* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -211,  -211,  -211,  -211,  -211,  -211,  -211,    -1,  -101,  -211,
    -211,  -211,  -211,  -211,  -211,   -32,  -211,  -211,    53,  -211,
    -211,    59,  -211,  -211,  -211,   -31,  -211,    54,  -211,   -10,
    -211,  -211,  -211,  -211,  -211,   -27,    89,   152,   247,  -113,
     318,   -26,   -38,  -211,   160,  -211,    95,    21,  -211,  -210,
    -211,   312,  -211,    22,  -183,  -211,   182,   280,  -211,  -211,
    -211,  -211,  -211,    -2,   -42
};

  /* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
      -1,     6,    48,     7,     8,     9,    10,   279,    24,    11,
      12,    36,   110,   201,   260,    49,   202,   311,   220,   388,
     404,   305,   306,   307,   108,   328,   329,   330,   262,   379,
     252,   263,   264,   137,   100,   390,    51,    52,    53,    54,
      55,    56,    57,    58,   215,    59,   289,   359,    60,    65,
      66,    67,    68,   337,   101,   197,   102,   103,   104,   105,
     266,   267,   268,   113,   114
};

  /* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
     positive, shift that token.  If negative, reduce the rule whose
     number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int16 yytable[] =
{
      14,   243,   109,    50,   111,    15,    20,    21,    22,    23,
     203,    27,   291,   218,    30,    31,    32,    33,   146,   148,
      28,    34,    35,    28,   227,   156,   157,   141,    38,   143,
      61,   403,    25,    28,    26,    28,   315,    28,    16,   -13,
     158,    62,    13,   125,   239,   -13,   128,   129,   130,    28,
     106,   107,   -13,    28,   142,   145,   219,    17,   301,    63,
      29,   149,   150,   282,   283,   284,   285,   286,   159,   160,
     161,   162,   163,   164,   165,   166,   167,   168,   169,   170,
     171,   172,   173,   174,   175,   176,   177,   178,   179,   180,
     181,   182,   183,   184,   185,   275,   -13,   186,   187,    64,
     130,   269,   385,   146,   395,   146,   240,   198,   131,    28,
     355,   236,   333,   132,   229,   153,   130,   335,   207,   208,
     209,   210,   211,    28,   130,   217,    62,    19,   287,   288,
     222,   223,    18,   224,   225,   226,   361,    28,    28,   228,
     230,    28,   231,   361,    63,    37,   232,   282,   283,   284,
     285,   286,    28,    28,   238,    28,    47,   241,    28,   318,
      28,   381,   116,    28,   322,   132,    28,    25,   112,   261,
     352,    28,   353,   124,   265,   354,  -132,    28,    28,   251,
     323,   132,   126,   367,    64,   106,   107,   244,   324,   132,
     245,   146,   246,   247,   248,   249,   115,   378,   -59,   254,
     151,   362,   271,   370,    28,   106,   107,   152,   362,   382,
     221,   218,   325,   288,   401,   383,   402,   -58,   200,   290,
     349,    28,   407,   273,    28,    25,   274,   147,   293,   310,
     199,   271,   387,   295,   296,   345,   298,   271,   253,   299,
     117,   -13,    25,   118,   155,   204,    -2,   255,   -13,   -13,
     303,   309,   205,   312,   219,   206,   314,   316,    39,    40,
      41,    42,    43,    44,    45,    46,    47,   256,   257,   258,
      28,   127,   319,   320,   -13,    25,   270,   357,   358,   234,
     272,   -13,    28,   119,   120,   121,   122,   123,   327,   233,
     212,   -13,    25,   332,   216,   271,   213,    46,   276,   277,
     278,   338,   237,   339,   138,    40,    41,    42,    43,    44,
     139,   344,   280,   348,     1,   351,     2,     3,     4,     5,
     297,    25,   259,   235,   -13,   356,    40,    41,    42,    43,
      44,   304,    25,   364,   294,   366,    25,    25,   308,   334,
      25,   368,   343,    25,   369,   386,   313,    25,   371,   347,
     317,   331,   372,   373,    25,   363,   350,   365,   375,   321,
     341,   376,   360,   392,   342,   340,   377,   406,   346,   380,
     391,   140,   214,   384,   281,   292,   326,   374,   154,   250,
     189,   393,   394,   190,     0,     0,   396,   397,     0,   398,
       0,   400,     0,  -160,  -160,  -160,  -160,     0,     0,     0,
     191,     0,   405,     0,     0,   408,   409,     0,     0,     0,
       0,     0,   336,     0,     0,     0,  -160,  -160,  -160,  -160,
    -160,  -160,  -160,  -160,  -160,  -160,  -160,  -160,  -160,  -160,
    -160,  -160,  -160,  -160,  -160,  -160,  -160,  -160,     0,     0,
       0,     0,  -160,  -151,  -151,  -151,     0,     0,   193,   194,
     190,     0,   195,   -13,     0,   196,  -160,  -160,     0,     0,
    -160,  -160,  -160,  -160,     0,     0,     0,   191,    39,    40,
      41,    42,    43,    44,    45,    46,     0,     0,     0,     0,
       0,     0,     0,  -160,  -160,  -160,  -160,  -160,  -160,  -160,
    -160,  -160,  -160,  -160,  -160,  -160,  -160,  -160,  -160,  -160,
    -160,  -160,  -160,  -160,  -160,     0,     0,     0,     0,  -160,
       0,     0,     0,     0,   -13,   193,   194,   190,   389,   195,
       0,     0,   196,  -160,  -160,     0,     0,  -160,  -160,  -160,
    -160,     0,     0,     0,   191,    39,    40,    41,    42,    43,
      44,    45,    46,     0,     0,     0,     0,     0,     0,     0,
    -160,  -160,  -160,  -160,  -160,  -160,  -160,  -160,  -160,  -160,
    -160,  -160,  -160,  -160,  -160,  -160,  -160,  -160,  -160,  -160,
    -160,  -160,     0,     0,     0,     0,  -160,     0,   192,     0,
       0,     0,   193,   194,   300,   399,   195,    28,     0,   196,
    -160,  -160,    28,     0,    69,    70,    71,    72,     0,     0,
      39,    40,    41,    42,    43,    44,    45,    46,    47,    39,
      40,    41,    42,    43,    44,    45,    46,    73,    74,    75,
      76,    77,    78,    79,    80,    81,    82,    83,    84,    85,
      86,    87,    88,    89,    90,    91,    92,    93,    94,     0,
       0,     0,     0,    95,  -148,  -148,  -148,     0,     0,   190,
      96,     0,     0,    97,     0,     0,     0,    98,    99,  -160,
    -160,  -160,  -160,    28,     0,     0,   191,     0,     0,     0,
       0,    39,    40,    41,    42,    43,    44,    45,    46,     0,
       0,     0,  -160,  -160,  -160,  -160,  -160,  -160,  -160,  -160,
    -160,  -160,  -160,  -160,  -160,  -160,  -160,  -160,  -160,  -160,
    -160,  -160,  -160,  -160,     0,     0,     0,     0,  -160,     0,
       0,     0,     0,   302,   193,   194,   242,     0,   195,    28,
       0,   196,  -160,  -160,     0,     0,    69,    70,    71,    72,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    73,
      74,    75,    76,    77,    78,    79,    80,    81,    82,    83,
      84,    85,    86,    87,    88,    89,    90,    91,    92,    93,
      94,    28,     0,     0,     0,    95,     0,     0,    69,    70,
      71,    72,    96,     0,     0,    97,     0,     0,     0,    98,
      99,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    73,    74,    75,    76,    77,    78,    79,    80,    81,
      82,    83,    84,    85,    86,    87,    88,    89,    90,    91,
      92,    93,    94,     0,     0,     0,     0,    95,     0,     0,
       0,     0,   144,     0,    96,   -13,     0,    97,     0,     0,
       0,    98,    99,   -13,    40,    41,    42,    43,    44,   -13,
     -13,   133,     0,     0,     0,     0,     0,     0,     0,     0,
     133,     0,   -76,   -76,   -76,   -76,   -76,   -76,   -76,   -76,
       0,   -76,   -76,   -76,   -76,   -76,   -76,   -76,   -76,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   -13,     0,     0,     0,   -13,   -13,   -13,   -13,
     -13,     0,     0,     0,     0,     0,    69,    70,    71,    72,
       0,   -80,     0,     0,     0,   -80,   -80,   134,   135,   136,
     -81,     0,     0,     0,   -81,   -81,   134,   135,   136,    73,
      74,    75,    76,    77,    78,    79,    80,    81,    82,    83,
      84,    85,    86,    87,    88,    89,    90,    91,    92,    93,
      94,     0,     0,     0,     0,    95,     0,     0,     0,     0,
       0,     0,    96,     0,     0,    97,     0,     0,     0,    98,
      99,    73,    74,    75,    76,    77,    78,    79,    80,    81,
      82,    83,    84,    85,    86,    87,    88,   188,    90,    91,
      92
};

static const yytype_int16 yycheck[] =
{
       1,   184,    33,    30,    36,    61,     7,     8,     9,    10,
     111,    13,   222,    11,    15,    16,    17,    18,    56,    61,
       4,     4,     5,     4,   137,    67,     1,    53,    29,    55,
       1,    15,    61,     4,    63,     4,     1,     4,    61,     4,
      15,    12,     1,    44,     1,     4,    47,    48,     1,     4,
      31,    32,    11,     4,    55,    56,    54,    61,   241,    30,
      11,    62,    63,     6,     7,     8,     9,    10,    69,    70,
      71,    72,    73,    74,    75,    76,    77,    78,    79,    80,
      81,    82,    83,    84,    85,    86,    87,    88,    89,    90,
      91,    92,    93,    94,    95,   208,    61,    98,    99,    70,
       1,   202,    71,   141,    71,   143,    63,   108,    61,     4,
      65,   153,   295,    66,   140,     1,     1,   300,   119,   120,
     121,   122,   123,     4,     1,   127,    12,     0,    71,    72,
     131,   132,     4,   134,   135,   136,     1,     4,     4,   140,
     141,     4,   143,     1,    30,     1,   147,     6,     7,     8,
       9,    10,     4,     4,   155,     4,    20,   158,     4,   260,
       4,   371,    19,     4,    65,    66,     4,    61,    62,   201,
      65,     4,    67,    12,   201,    70,    62,     4,     4,    12,
      65,    66,    19,    64,    70,    31,    32,   188,    65,    66,
     191,   229,   193,   194,   195,   196,    63,    63,    63,   200,
      12,    66,   204,    61,     4,    31,    32,    62,    66,    53,
      62,    11,    71,    72,    65,    53,    65,    63,    25,   220,
      61,     4,   405,    50,     4,    61,    53,    63,   229,    12,
      62,   233,    12,   234,   235,    61,   237,   239,     1,   240,
      12,     4,    61,    15,    63,     1,     0,     1,    11,    12,
     251,   253,    12,   255,    54,    12,   257,   258,    12,    13,
      14,    15,    16,    17,    18,    19,    20,    21,    22,    23,
       4,     1,   273,   274,     4,    61,    62,    11,    12,    15,
      65,    11,     4,    55,    56,    57,    58,    59,   289,     1,
      12,    54,    61,   294,    63,   297,    18,    19,   209,   210,
     211,   302,    63,   304,    12,    13,    14,    15,    16,    17,
      18,   313,    12,   315,    24,   317,    26,    27,    28,    29,
       1,    61,    76,    63,    54,   326,    13,    14,    15,    16,
      17,    33,    61,   334,    63,   336,    61,    61,    63,    63,
      61,   342,    63,    61,   345,   376,     1,    61,   349,    63,
       1,    62,   353,   354,    61,   333,    63,   335,   359,    65,
       4,   362,    63,    62,   311,   306,   367,    65,   314,   370,
     380,    53,   125,   374,   214,   223,   281,   356,    66,   197,
     100,   382,   383,     1,    -1,    -1,   387,   388,    -1,   390,
      -1,   392,    -1,    11,    12,    13,    14,    -1,    -1,    -1,
      18,    -1,   403,    -1,    -1,   406,   407,    -1,    -1,    -1,
      -1,    -1,    30,    -1,    -1,    -1,    34,    35,    36,    37,
      38,    39,    40,    41,    42,    43,    44,    45,    46,    47,
      48,    49,    50,    51,    52,    53,    54,    55,    -1,    -1,
      -1,    -1,    60,    61,    62,    63,    -1,    -1,    66,    67,
       1,    -1,    70,     4,    -1,    73,    74,    75,    -1,    -1,
      11,    12,    13,    14,    -1,    -1,    -1,    18,    12,    13,
      14,    15,    16,    17,    18,    19,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    34,    35,    36,    37,    38,    39,    40,
      41,    42,    43,    44,    45,    46,    47,    48,    49,    50,
      51,    52,    53,    54,    55,    -1,    -1,    -1,    -1,    60,
      -1,    -1,    -1,    -1,    65,    66,    67,     1,    62,    70,
      -1,    -1,    73,    74,    75,    -1,    -1,    11,    12,    13,
      14,    -1,    -1,    -1,    18,    12,    13,    14,    15,    16,
      17,    18,    19,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      34,    35,    36,    37,    38,    39,    40,    41,    42,    43,
      44,    45,    46,    47,    48,    49,    50,    51,    52,    53,
      54,    55,    -1,    -1,    -1,    -1,    60,    -1,    62,    -1,
      -1,    -1,    66,    67,     1,    62,    70,     4,    -1,    73,
      74,    75,     4,    -1,    11,    12,    13,    14,    -1,    -1,
      12,    13,    14,    15,    16,    17,    18,    19,    20,    12,
      13,    14,    15,    16,    17,    18,    19,    34,    35,    36,
      37,    38,    39,    40,    41,    42,    43,    44,    45,    46,
      47,    48,    49,    50,    51,    52,    53,    54,    55,    -1,
      -1,    -1,    -1,    60,    61,    62,    63,    -1,    -1,     1,
      67,    -1,    -1,    70,    -1,    -1,    -1,    74,    75,    11,
      12,    13,    14,     4,    -1,    -1,    18,    -1,    -1,    -1,
      -1,    12,    13,    14,    15,    16,    17,    18,    19,    -1,
      -1,    -1,    34,    35,    36,    37,    38,    39,    40,    41,
      42,    43,    44,    45,    46,    47,    48,    49,    50,    51,
      52,    53,    54,    55,    -1,    -1,    -1,    -1,    60,    -1,
      -1,    -1,    -1,    65,    66,    67,     1,    -1,    70,     4,
      -1,    73,    74,    75,    -1,    -1,    11,    12,    13,    14,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    34,
      35,    36,    37,    38,    39,    40,    41,    42,    43,    44,
      45,    46,    47,    48,    49,    50,    51,    52,    53,    54,
      55,     4,    -1,    -1,    -1,    60,    -1,    -1,    11,    12,
      13,    14,    67,    -1,    -1,    70,    -1,    -1,    -1,    74,
      75,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    34,    35,    36,    37,    38,    39,    40,    41,    42,
      43,    44,    45,    46,    47,    48,    49,    50,    51,    52,
      53,    54,    55,    -1,    -1,    -1,    -1,    60,    -1,    -1,
      -1,    -1,     1,    -1,    67,     4,    -1,    70,    -1,    -1,
      -1,    74,    75,    12,    13,    14,    15,    16,    17,    18,
      19,     1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
       1,    -1,    12,    13,    14,    15,    16,    17,    18,    19,
      -1,    12,    13,    14,    15,    16,    17,    18,    19,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    61,    -1,    -1,    -1,    65,    66,    67,    68,
      69,    -1,    -1,    -1,    -1,    -1,    11,    12,    13,    14,
      -1,    61,    -1,    -1,    -1,    65,    66,    67,    68,    69,
      61,    -1,    -1,    -1,    65,    66,    67,    68,    69,    34,
      35,    36,    37,    38,    39,    40,    41,    42,    43,    44,
      45,    46,    47,    48,    49,    50,    51,    52,    53,    54,
      55,    -1,    -1,    -1,    -1,    60,    -1,    -1,    -1,    -1,
      -1,    -1,    67,    -1,    -1,    70,    -1,    -1,    -1,    74,
      75,    34,    35,    36,    37,    38,    39,    40,    41,    42,
      43,    44,    45,    46,    47,    48,    49,    50,    51,    52,
      53
};

  /* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
     symbol of state STATE-NUM.  */
static const yytype_uint8 yystos[] =
{
       0,    24,    26,    27,    28,    29,    78,    80,    81,    82,
      83,    86,    87,     1,    84,    61,    61,    61,     4,     0,
      84,    84,    84,    84,    85,    61,    63,   140,     4,    11,
      84,    84,    84,    84,     4,     5,    88,     1,    84,    12,
      13,    14,    15,    16,    17,    18,    19,    20,    79,    92,
     112,   113,   114,   115,   116,   117,   118,   119,   120,   122,
     125,     1,    12,    30,    70,   126,   127,   128,   129,    11,
      12,    13,    14,    34,    35,    36,    37,    38,    39,    40,
      41,    42,    43,    44,    45,    46,    47,    48,    49,    50,
      51,    52,    53,    54,    55,    60,    67,    70,    74,    75,
     111,   131,   133,   134,   135,   136,    31,    32,   101,   102,
      89,    92,    62,   140,   141,    63,    19,    12,    15,    55,
      56,    57,    58,    59,    12,    84,    19,     1,    84,    84,
       1,    61,    66,     1,    67,    68,    69,   110,    12,    18,
     117,   118,    84,   118,     1,    84,   119,    63,   141,    84,
      84,    12,    62,     1,   128,    63,   141,     1,    15,    84,
      84,    84,    84,    84,    84,    84,    84,    84,    84,    84,
      84,    84,    84,    84,    84,    84,    84,    84,    84,    84,
      84,    84,    84,    84,    84,    84,    84,    84,    50,   134,
       1,    18,    62,    66,    67,    70,    73,   132,    84,    62,
      25,    90,    93,    85,     1,    12,    12,    84,    84,    84,
      84,    84,    12,    18,   115,   121,    63,   140,    11,    54,
      95,    62,    84,    84,    84,    84,    84,   116,    84,   118,
      84,    84,    84,     1,    15,    63,   141,    63,    84,     1,
      63,    84,     1,   131,    84,    84,    84,    84,    84,    84,
     133,    12,   107,     1,    84,     1,    21,    22,    23,    76,
      91,    92,   105,   108,   109,   112,   137,   138,   139,    85,
      62,   140,    65,    50,    53,   116,   113,   113,   113,    84,
      12,   121,     6,     7,     8,     9,    10,    71,    72,   123,
      84,   126,   114,    84,    63,    84,    84,     1,    84,    84,
       1,   131,    65,    84,    33,    98,    99,   100,    63,   140,
      12,    94,   140,     1,    84,     1,    84,     1,    85,    84,
      84,    65,    65,    65,    65,    71,   123,    84,   102,   103,
     104,    62,    84,   131,    63,   131,    30,   130,    84,    84,
      98,     4,    95,    63,   140,    61,   104,    63,   140,    61,
      63,   140,    65,    67,    70,    65,    84,    11,    12,   124,
      63,     1,    66,   130,    84,   130,    84,    64,    84,    84,
      61,    84,    84,    84,   124,    84,    84,    84,    63,   106,
      84,   126,    53,    53,    84,    71,   102,    12,    96,    62,
     112,   106,    62,    84,    84,    71,    84,    84,    84,    62,
      84,    65,    65,    15,    97,    84,    65,   131,    84,    84
};

  /* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_uint8 yyr1[] =
{
       0,    77,    78,    78,    78,    78,    78,    79,    79,    80,
      81,    82,    83,    84,    84,    85,    85,    85,    86,    86,
      87,    87,    87,    88,    88,    89,    89,    90,    90,    91,
      91,    91,    91,    91,    91,    91,    92,    92,    92,    93,
      93,    93,    94,    94,    95,    95,    96,    97,    97,    98,
      99,    99,   100,   100,   101,   101,   101,   102,   103,   103,
     104,   104,   104,   105,   105,   106,   106,   107,   108,   108,
     109,   109,   109,   110,   110,   110,   110,   111,   111,   112,
     113,   113,   113,   114,   114,   114,   115,   115,   115,   116,
     116,   116,   116,   116,   116,   117,   117,   118,   118,   118,
     119,   119,   119,   119,   119,   120,   121,   122,   122,   122,
     122,   123,   123,   123,   123,   123,   123,   124,   124,   125,
     125,   125,   125,   125,   125,   125,   125,   125,   125,   125,
     126,   126,   126,   126,   126,   126,   127,   127,   127,   127,
     127,   127,   127,   128,   128,   128,   128,   128,   128,   129,
     130,   130,   131,   131,   131,   132,   132,   132,   132,   132,
     132,   133,   133,   133,   133,   133,   133,   133,   133,   133,
     133,   133,   133,   134,   134,   134,   134,   134,   134,   134,
     134,   134,   134,   134,   134,   134,   134,   134,   134,   134,
     134,   134,   135,   135,   136,   136,   137,   137,   138,   139,
     140,   140,   141,   141
};

  /* YYR2[YYN] -- Number of symbols on the right hand side of rule YYN.  */
static const yytype_int8 yyr2[] =
{
       0,     2,     5,     2,     2,     2,     2,     1,     1,     6,
       5,     5,     5,     0,     2,     0,     2,     2,     0,     1,
       5,     3,     3,     0,     3,     0,     3,     0,     3,     1,
       1,     1,     1,     1,     1,     1,     6,     3,     3,     6,
       3,     3,     0,     2,     1,     1,     2,     0,     4,     9,
       1,     2,     0,     1,     0,     1,     1,     4,     0,     1,
       1,     4,     2,     7,     6,     0,     3,     2,     3,     3,
       7,     3,     3,     2,     2,     2,     0,     1,     1,     5,
       1,     4,     2,     1,     3,     2,     1,     2,     2,     2,
       3,     2,     3,     4,     3,     1,     1,     1,     2,     2,
       1,     1,     1,     1,     1,     2,     2,     4,     8,     5,
       9,     1,     1,     1,     1,     1,     1,     1,     1,     2,
       3,     4,     6,     6,    10,    10,     5,     5,     5,     5,
       1,     2,     1,     3,     1,     2,     3,     4,     3,     5,
       4,     4,     6,     5,     6,     2,     6,     2,     3,     2,
       2,     0,     1,     3,     2,     2,     2,     2,     2,     2,
       0,     1,     2,     2,     2,     2,     3,     2,     2,     1,
       2,     1,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     5,     3,     2,     2,     3,     3,     1,     2,
       5,     3,     1,     3
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
#line 307 "CSSGrammar.y"
                                                                      {
        static_cast<CSSParser*>(parser)->rule = (yyvsp[-2].rule);
    }
#line 1933 "CSSGrammar.tab.c"
    break;

  case 10:
#line 313 "CSSGrammar.y"
                                                          {
        /* can be empty */
    }
#line 1941 "CSSGrammar.tab.c"
    break;

  case 11:
#line 319 "CSSGrammar.y"
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
#line 1957 "CSSGrammar.tab.c"
    break;

  case 12:
#line 333 "CSSGrammar.y"
                                                                  {
         CSSParser* p = static_cast<CSSParser*>(parser);
         p->mediaQuery = p->sinkFloatingMediaQuery((yyvsp[-1].mediaQuery));
     }
#line 1966 "CSSGrammar.tab.c"
    break;

  case 19:
#line 352 "CSSGrammar.y"
            {
  }
#line 1973 "CSSGrammar.tab.c"
    break;

  case 20:
#line 357 "CSSGrammar.y"
                                                 {
     CSSParser* p = static_cast<CSSParser*>(parser);
     (yyval.rule) = static_cast<CSSParser*>(parser)->createCharsetRule((yyvsp[-2].string));
     if ((yyval.rule) && p->styleElement && p->styleElement->isCSSStyleSheet())
         p->styleElement->append((yyval.rule));
  }
#line 1984 "CSSGrammar.tab.c"
    break;

  case 21:
#line 363 "CSSGrammar.y"
                                    {
  }
#line 1991 "CSSGrammar.tab.c"
    break;

  case 22:
#line 365 "CSSGrammar.y"
                          {
  }
#line 1998 "CSSGrammar.tab.c"
    break;

  case 24:
#line 371 "CSSGrammar.y"
                                 {
     CSSParser* p = static_cast<CSSParser*>(parser);
     if ((yyvsp[-1].rule) && p->styleElement && p->styleElement->isCSSStyleSheet())
         p->styleElement->append((yyvsp[-1].rule));
 }
#line 2008 "CSSGrammar.tab.c"
    break;

  case 28:
#line 385 "CSSGrammar.y"
                             {
     CSSParser* p = static_cast<CSSParser*>(parser);
     if ((yyvsp[-1].rule) && p->styleElement && p->styleElement->isCSSStyleSheet())
         p->styleElement->append((yyvsp[-1].rule));
 }
#line 2018 "CSSGrammar.tab.c"
    break;

  case 36:
#line 403 "CSSGrammar.y"
                                                                          {
        (yyval.rule) = static_cast<CSSParser*>(parser)->createImportRule((yyvsp[-3].string), (yyvsp[-1].mediaList));
    }
#line 2026 "CSSGrammar.tab.c"
    break;

  case 37:
#line 406 "CSSGrammar.y"
                                   {
        (yyval.rule) = 0;
    }
#line 2034 "CSSGrammar.tab.c"
    break;

  case 38:
#line 409 "CSSGrammar.y"
                         {
        (yyval.rule) = 0;
    }
#line 2042 "CSSGrammar.tab.c"
    break;

  case 39:
#line 415 "CSSGrammar.y"
                                                                        {
    CSSParser* p = static_cast<CSSParser*>(parser);
    if (p->styleElement && p->styleElement->isCSSStyleSheet())
        static_cast<CSSStyleSheet*>(p->styleElement)->addNamespace(p, atomicString((yyvsp[-3].string)), atomicString((yyvsp[-2].string)));
}
#line 2052 "CSSGrammar.tab.c"
    break;

  case 42:
#line 425 "CSSGrammar.y"
            { (yyval.string).characters = 0; }
#line 2058 "CSSGrammar.tab.c"
    break;

  case 43:
#line 426 "CSSGrammar.y"
                   { (yyval.string) = (yyvsp[-1].string); }
#line 2064 "CSSGrammar.tab.c"
    break;

  case 46:
#line 435 "CSSGrammar.y"
                      {
        (yyval.string) = (yyvsp[-1].string);
    }
#line 2072 "CSSGrammar.tab.c"
    break;

  case 47:
#line 441 "CSSGrammar.y"
              {
        (yyval.valueList) = 0;
    }
#line 2080 "CSSGrammar.tab.c"
    break;

  case 48:
#line 444 "CSSGrammar.y"
                                       {
        (yyval.valueList) = (yyvsp[-1].valueList);
    }
#line 2088 "CSSGrammar.tab.c"
    break;

  case 49:
#line 450 "CSSGrammar.y"
                                                                                                      {
        (yyvsp[-4].string).lower();
        (yyval.mediaQueryExp) = static_cast<CSSParser*>(parser)->createFloatingMediaQueryExp(atomicString((yyvsp[-4].string)), (yyvsp[-2].valueList));
    }
#line 2097 "CSSGrammar.tab.c"
    break;

  case 50:
#line 457 "CSSGrammar.y"
                    {
      CSSParser* p = static_cast<CSSParser*>(parser);
      (yyval.mediaQueryExpList) = p->createFloatingMediaQueryExpList();
      (yyval.mediaQueryExpList)->append(p->sinkFloatingMediaQueryExp((yyvsp[0].mediaQueryExp)));
    }
#line 2107 "CSSGrammar.tab.c"
    break;

  case 51:
#line 462 "CSSGrammar.y"
                                           {
      (yyval.mediaQueryExpList) = (yyvsp[-1].mediaQueryExpList);
      (yyval.mediaQueryExpList)->append(static_cast<CSSParser*>(parser)->sinkFloatingMediaQueryExp((yyvsp[0].mediaQueryExp)));
    }
#line 2116 "CSSGrammar.tab.c"
    break;

  case 52:
#line 469 "CSSGrammar.y"
              {
        (yyval.mediaQueryExpList) = static_cast<CSSParser*>(parser)->createFloatingMediaQueryExpList();
    }
#line 2124 "CSSGrammar.tab.c"
    break;

  case 54:
#line 476 "CSSGrammar.y"
              {
        (yyval.mediaQueryRestrictor) = MediaQuery::None;
    }
#line 2132 "CSSGrammar.tab.c"
    break;

  case 55:
#line 479 "CSSGrammar.y"
                 {
        (yyval.mediaQueryRestrictor) = MediaQuery::Only;
    }
#line 2140 "CSSGrammar.tab.c"
    break;

  case 56:
#line 482 "CSSGrammar.y"
                {
        (yyval.mediaQueryRestrictor) = MediaQuery::Not;
    }
#line 2148 "CSSGrammar.tab.c"
    break;

  case 57:
#line 488 "CSSGrammar.y"
                                                                         {
        CSSParser* p = static_cast<CSSParser*>(parser);
        (yyvsp[-1].string).lower();
        (yyval.mediaQuery) = p->createFloatingMediaQuery((yyvsp[-3].mediaQueryRestrictor), domString((yyvsp[-1].string)), p->sinkFloatingMediaQueryExpList((yyvsp[0].mediaQueryExpList)));
    }
#line 2158 "CSSGrammar.tab.c"
    break;

  case 58:
#line 496 "CSSGrammar.y"
                 {
        (yyval.mediaList) = static_cast<CSSParser*>(parser)->createMediaList();
     }
#line 2166 "CSSGrammar.tab.c"
    break;

  case 60:
#line 503 "CSSGrammar.y"
                {
        CSSParser* p = static_cast<CSSParser*>(parser);
        (yyval.mediaList) = p->createMediaList();
        (yyval.mediaList)->appendMediaQuery(p->sinkFloatingMediaQuery((yyvsp[0].mediaQuery)));
    }
#line 2176 "CSSGrammar.tab.c"
    break;

  case 61:
#line 508 "CSSGrammar.y"
                                             {
        (yyval.mediaList) = (yyvsp[-3].mediaList);
        if ((yyval.mediaList))
            (yyval.mediaList)->appendMediaQuery(static_cast<CSSParser*>(parser)->sinkFloatingMediaQuery((yyvsp[0].mediaQuery)));
    }
#line 2186 "CSSGrammar.tab.c"
    break;

  case 62:
#line 513 "CSSGrammar.y"
                       {
        (yyval.mediaList) = 0;
    }
#line 2194 "CSSGrammar.tab.c"
    break;

  case 63:
#line 519 "CSSGrammar.y"
                                                                      {
        (yyval.rule) = static_cast<CSSParser*>(parser)->createMediaRule((yyvsp[-4].mediaList), (yyvsp[-1].ruleList));
    }
#line 2202 "CSSGrammar.tab.c"
    break;

  case 64:
#line 522 "CSSGrammar.y"
                                                             {
        (yyval.rule) = static_cast<CSSParser*>(parser)->createMediaRule(0, (yyvsp[-1].ruleList));
    }
#line 2210 "CSSGrammar.tab.c"
    break;

  case 65:
#line 528 "CSSGrammar.y"
                { (yyval.ruleList) = 0; }
#line 2216 "CSSGrammar.tab.c"
    break;

  case 66:
#line 529 "CSSGrammar.y"
                                       {
        (yyval.ruleList) = (yyvsp[-2].ruleList);
        if ((yyvsp[-1].rule)) {
            if (!(yyval.ruleList))
                (yyval.ruleList) = static_cast<CSSParser*>(parser)->createRuleList();
            (yyval.ruleList)->append((yyvsp[-1].rule));
        }
    }
#line 2229 "CSSGrammar.tab.c"
    break;

  case 67:
#line 540 "CSSGrammar.y"
                    {
      (yyval.string) = (yyvsp[-1].string);
  }
#line 2237 "CSSGrammar.tab.c"
    break;

  case 68:
#line 557 "CSSGrammar.y"
                                 {
      (yyval.rule) = 0;
    }
#line 2245 "CSSGrammar.tab.c"
    break;

  case 69:
#line 560 "CSSGrammar.y"
                       {
      (yyval.rule) = 0;
    }
#line 2253 "CSSGrammar.tab.c"
    break;

  case 70:
#line 567 "CSSGrammar.y"
                                                      {
        (yyval.rule) = static_cast<CSSParser*>(parser)->createFontFaceRule();
    }
#line 2261 "CSSGrammar.tab.c"
    break;

  case 71:
#line 570 "CSSGrammar.y"
                                        {
      (yyval.rule) = 0;
    }
#line 2269 "CSSGrammar.tab.c"
    break;

  case 72:
#line 573 "CSSGrammar.y"
                              {
      (yyval.rule) = 0;
    }
#line 2277 "CSSGrammar.tab.c"
    break;

  case 73:
#line 579 "CSSGrammar.y"
                    { (yyval.relation) = CSSSelector::DirectAdjacent; }
#line 2283 "CSSGrammar.tab.c"
    break;

  case 74:
#line 580 "CSSGrammar.y"
                    { (yyval.relation) = CSSSelector::IndirectAdjacent; }
#line 2289 "CSSGrammar.tab.c"
    break;

  case 75:
#line 581 "CSSGrammar.y"
                    { (yyval.relation) = CSSSelector::Child; }
#line 2295 "CSSGrammar.tab.c"
    break;

  case 76:
#line 582 "CSSGrammar.y"
                { (yyval.relation) = CSSSelector::Descendant; }
#line 2301 "CSSGrammar.tab.c"
    break;

  case 77:
#line 586 "CSSGrammar.y"
        { (yyval.i) = -1; }
#line 2307 "CSSGrammar.tab.c"
    break;

  case 78:
#line 587 "CSSGrammar.y"
        { (yyval.i) = 1; }
#line 2313 "CSSGrammar.tab.c"
    break;

  case 79:
#line 591 "CSSGrammar.y"
                                                       {
        (yyval.rule) = static_cast<CSSParser*>(parser)->createStyleRule((yyvsp[-4].selector));
    }
#line 2321 "CSSGrammar.tab.c"
    break;

  case 80:
#line 597 "CSSGrammar.y"
                                   {
        (yyval.selector) = (yyvsp[0].selector);
    }
#line 2329 "CSSGrammar.tab.c"
    break;

  case 81:
#line 600 "CSSGrammar.y"
                                                                   {
        if ((yyvsp[-3].selector) && (yyvsp[0].selector)) {
            CSSParser* p = static_cast<CSSParser*>(parser);
            (yyval.selector) = (yyvsp[-3].selector);
            (yyval.selector)->append(p->sinkFloatingSelector((yyvsp[0].selector)));
        } else
            (yyval.selector) = 0;
    }
#line 2342 "CSSGrammar.tab.c"
    break;

  case 82:
#line 608 "CSSGrammar.y"
                        {
        (yyval.selector) = 0;
    }
#line 2350 "CSSGrammar.tab.c"
    break;

  case 83:
#line 614 "CSSGrammar.y"
                    {
        (yyval.selector) = (yyvsp[0].selector);
    }
#line 2358 "CSSGrammar.tab.c"
    break;

  case 84:
#line 617 "CSSGrammar.y"
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
#line 2383 "CSSGrammar.tab.c"
    break;

  case 85:
#line 637 "CSSGrammar.y"
                     {
        (yyval.selector) = 0;
    }
#line 2391 "CSSGrammar.tab.c"
    break;

  case 86:
#line 643 "CSSGrammar.y"
                    { (yyval.string).characters = 0; (yyval.string).length = 0; }
#line 2397 "CSSGrammar.tab.c"
    break;

  case 87:
#line 644 "CSSGrammar.y"
              { static UChar star = '*'; (yyval.string).characters = &star; (yyval.string).length = 1; }
#line 2403 "CSSGrammar.tab.c"
    break;

  case 88:
#line 645 "CSSGrammar.y"
                { (yyval.string) = (yyvsp[-1].string); }
#line 2409 "CSSGrammar.tab.c"
    break;

  case 89:
#line 649 "CSSGrammar.y"
                             {
        CSSParser* p = static_cast<CSSParser*>(parser);
        (yyval.selector) = p->createFloatingSelector();
        (yyval.selector)->m_tag = QualifiedName(nullAtom, atomicString((yyvsp[-1].string)), p->defaultNamespace);
    }
#line 2419 "CSSGrammar.tab.c"
    break;

  case 90:
#line 654 "CSSGrammar.y"
                                              {
        (yyval.selector) = (yyvsp[-1].selector);
        if ((yyval.selector)) {
            CSSParser* p = static_cast<CSSParser*>(parser);
            (yyval.selector)->m_tag = QualifiedName(nullAtom, atomicString((yyvsp[-2].string)), p->defaultNamespace);
        }
    }
#line 2431 "CSSGrammar.tab.c"
    break;

  case 91:
#line 661 "CSSGrammar.y"
                                 {
        (yyval.selector) = (yyvsp[-1].selector);
        CSSParser* p = static_cast<CSSParser*>(parser);
        if ((yyval.selector) && p->defaultNamespace != starAtom)
            (yyval.selector)->m_tag = QualifiedName(nullAtom, starAtom, p->defaultNamespace);
    }
#line 2442 "CSSGrammar.tab.c"
    break;

  case 92:
#line 667 "CSSGrammar.y"
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
#line 2458 "CSSGrammar.tab.c"
    break;

  case 93:
#line 678 "CSSGrammar.y"
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
#line 2476 "CSSGrammar.tab.c"
    break;

  case 94:
#line 691 "CSSGrammar.y"
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
#line 2492 "CSSGrammar.tab.c"
    break;

  case 95:
#line 705 "CSSGrammar.y"
          {
        ParseString& str = (yyvsp[0].string);
        CSSParser* p = static_cast<CSSParser*>(parser);
        Document* doc = p->document();
        if (doc && doc->isHTMLDocument())
            str.lower();
        (yyval.string) = str;
    }
#line 2505 "CSSGrammar.tab.c"
    break;

  case 96:
#line 713 "CSSGrammar.y"
          {
        static UChar star = '*';
        (yyval.string).characters = &star;
        (yyval.string).length = 1;
    }
#line 2515 "CSSGrammar.tab.c"
    break;

  case 97:
#line 721 "CSSGrammar.y"
              {
        (yyval.selector) = (yyvsp[0].selector);
    }
#line 2523 "CSSGrammar.tab.c"
    break;

  case 98:
#line 724 "CSSGrammar.y"
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
#line 2541 "CSSGrammar.tab.c"
    break;

  case 99:
#line 737 "CSSGrammar.y"
                           {
        (yyval.selector) = 0;
    }
#line 2549 "CSSGrammar.tab.c"
    break;

  case 100:
#line 743 "CSSGrammar.y"
          {
        CSSParser* p = static_cast<CSSParser*>(parser);
        (yyval.selector) = p->createFloatingSelector();
        (yyval.selector)->m_match = CSSSelector::Id;
        if (!p->strict)
            (yyvsp[0].string).lower();
        (yyval.selector)->m_attr = idAttr;
        (yyval.selector)->m_value = atomicString((yyvsp[0].string));
    }
#line 2563 "CSSGrammar.tab.c"
    break;

  case 101:
#line 752 "CSSGrammar.y"
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
#line 2581 "CSSGrammar.tab.c"
    break;

  case 105:
#line 771 "CSSGrammar.y"
              {
        CSSParser* p = static_cast<CSSParser*>(parser);
        (yyval.selector) = p->createFloatingSelector();
        (yyval.selector)->m_match = CSSSelector::Class;
        if (!p->strict)
            (yyvsp[0].string).lower();
        (yyval.selector)->m_attr = classAttr;
        (yyval.selector)->m_value = atomicString((yyvsp[0].string));
    }
#line 2595 "CSSGrammar.tab.c"
    break;

  case 106:
#line 783 "CSSGrammar.y"
                      {
        ParseString& str = (yyvsp[-1].string);
        CSSParser* p = static_cast<CSSParser*>(parser);
        Document* doc = p->document();
        if (doc && doc->isHTMLDocument())
            str.lower();
        (yyval.string) = str;
    }
#line 2608 "CSSGrammar.tab.c"
    break;

  case 107:
#line 794 "CSSGrammar.y"
                                  {
        (yyval.selector) = static_cast<CSSParser*>(parser)->createFloatingSelector();
        (yyval.selector)->m_attr = QualifiedName(nullAtom, atomicString((yyvsp[-1].string)), nullAtom);
        (yyval.selector)->m_match = CSSSelector::Set;
    }
#line 2618 "CSSGrammar.tab.c"
    break;

  case 108:
#line 799 "CSSGrammar.y"
                                                                                  {
        (yyval.selector) = static_cast<CSSParser*>(parser)->createFloatingSelector();
        (yyval.selector)->m_attr = QualifiedName(nullAtom, atomicString((yyvsp[-5].string)), nullAtom);
        (yyval.selector)->m_match = (CSSSelector::Match)(yyvsp[-4].i);
        (yyval.selector)->m_value = atomicString((yyvsp[-2].string));
    }
#line 2629 "CSSGrammar.tab.c"
    break;

  case 109:
#line 805 "CSSGrammar.y"
                                                       {
        AtomicString namespacePrefix = atomicString((yyvsp[-2].string));
        CSSParser* p = static_cast<CSSParser*>(parser);
        (yyval.selector) = p->createFloatingSelector();
        (yyval.selector)->m_attr = QualifiedName(namespacePrefix,
                                   atomicString((yyvsp[-1].string)),
                                   static_cast<CSSStyleSheet*>(p->styleElement)->determineNamespace(namespacePrefix));
        (yyval.selector)->m_match = CSSSelector::Set;
    }
#line 2643 "CSSGrammar.tab.c"
    break;

  case 110:
#line 814 "CSSGrammar.y"
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
#line 2658 "CSSGrammar.tab.c"
    break;

  case 111:
#line 827 "CSSGrammar.y"
        {
        (yyval.i) = CSSSelector::Exact;
    }
#line 2666 "CSSGrammar.tab.c"
    break;

  case 112:
#line 830 "CSSGrammar.y"
               {
        (yyval.i) = CSSSelector::List;
    }
#line 2674 "CSSGrammar.tab.c"
    break;

  case 113:
#line 833 "CSSGrammar.y"
                {
        (yyval.i) = CSSSelector::Hyphen;
    }
#line 2682 "CSSGrammar.tab.c"
    break;

  case 114:
#line 836 "CSSGrammar.y"
                 {
        (yyval.i) = CSSSelector::Begin;
    }
#line 2690 "CSSGrammar.tab.c"
    break;

  case 115:
#line 839 "CSSGrammar.y"
               {
        (yyval.i) = CSSSelector::End;
    }
#line 2698 "CSSGrammar.tab.c"
    break;

  case 116:
#line 842 "CSSGrammar.y"
               {
        (yyval.i) = CSSSelector::Contain;
    }
#line 2706 "CSSGrammar.tab.c"
    break;

  case 119:
#line 853 "CSSGrammar.y"
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
#line 2736 "CSSGrammar.tab.c"
    break;

  case 120:
#line 878 "CSSGrammar.y"
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
#line 2755 "CSSGrammar.tab.c"
    break;

  case 121:
#line 893 "CSSGrammar.y"
                             {
        (yyval.selector) = static_cast<CSSParser*>(parser)->createFloatingSelector();
        (yyval.selector)->m_match = CSSSelector::PseudoClass;
        (yyval.selector)->m_argument = atomicString((yyvsp[-1].string));
        (yyvsp[-2].string).lower();
        (yyval.selector)->m_value = atomicString((yyvsp[-2].string));
        if ((yyval.selector)->pseudoType() == CSSSelector::PseudoUnknown)
            (yyval.selector) = 0;
    }
#line 2769 "CSSGrammar.tab.c"
    break;

  case 122:
#line 903 "CSSGrammar.y"
                                                       {
        (yyval.selector) = static_cast<CSSParser*>(parser)->createFloatingSelector();
        (yyval.selector)->m_match = CSSSelector::PseudoClass;
        (yyval.selector)->m_argument = String::number((int)(yyvsp[-2].val));
        (yyvsp[-4].string).lower();
        (yyval.selector)->m_value = atomicString((yyvsp[-4].string));
        if ((yyval.selector)->pseudoType() == CSSSelector::PseudoUnknown)
            (yyval.selector) = 0;
    }
#line 2783 "CSSGrammar.tab.c"
    break;

  case 123:
#line 913 "CSSGrammar.y"
                                                     {
        (yyval.selector) = static_cast<CSSParser*>(parser)->createFloatingSelector();
        (yyval.selector)->m_match = CSSSelector::PseudoClass;
        (yyval.selector)->m_argument = atomicString((yyvsp[-2].string));
        (yyvsp[-4].string).lower();
        (yyval.selector)->m_value = atomicString((yyvsp[-4].string));
        if ((yyval.selector)->pseudoType() == CSSSelector::PseudoUnknown)
            (yyval.selector) = 0;
    }
#line 2797 "CSSGrammar.tab.c"
    break;

  case 124:
#line 924 "CSSGrammar.y"
                                                                                         {
        (yyval.selector) = static_cast<CSSParser*>(parser)->createFloatingSelector();
        (yyval.selector)->m_match = CSSSelector::PseudoClass;
        (yyval.selector)->m_argument = domString((yyvsp[-6].string)) + "+" + String::number((int)(yyvsp[-2].val));
        (yyvsp[-8].string).lower();
        (yyval.selector)->m_value = atomicString((yyvsp[-8].string));
        if ((yyval.selector)->pseudoType() == CSSSelector::PseudoUnknown)
            (yyval.selector) = 0;
    }
#line 2811 "CSSGrammar.tab.c"
    break;

  case 125:
#line 933 "CSSGrammar.y"
                                                                                         {
        (yyval.selector) = static_cast<CSSParser*>(parser)->createFloatingSelector();
        (yyval.selector)->m_match = CSSSelector::PseudoClass;
        (yyval.selector)->m_argument = domString((yyvsp[-6].string)) + "-" + String::number((int)(yyvsp[-2].val));
        (yyvsp[-8].string).lower();
        (yyval.selector)->m_value = atomicString((yyvsp[-8].string));
        if ((yyval.selector)->pseudoType() == CSSSelector::PseudoUnknown)
            (yyval.selector) = 0;
    }
#line 2825 "CSSGrammar.tab.c"
    break;

  case 126:
#line 943 "CSSGrammar.y"
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
#line 2842 "CSSGrammar.tab.c"
    break;

  case 127:
#line 960 "CSSGrammar.y"
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
#line 2858 "CSSGrammar.tab.c"
    break;

  case 128:
#line 971 "CSSGrammar.y"
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
#line 2874 "CSSGrammar.tab.c"
    break;

  case 129:
#line 982 "CSSGrammar.y"
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
#line 2892 "CSSGrammar.tab.c"
    break;

  case 130:
#line 998 "CSSGrammar.y"
                {
        (yyval.ok) = (yyvsp[0].ok);
    }
#line 2900 "CSSGrammar.tab.c"
    break;

  case 131:
#line 1001 "CSSGrammar.y"
                            {
        (yyval.ok) = (yyvsp[-1].ok);
        if ( (yyvsp[0].ok) )
            (yyval.ok) = (yyvsp[0].ok);
    }
#line 2910 "CSSGrammar.tab.c"
    break;

  case 132:
#line 1006 "CSSGrammar.y"
                {
        (yyval.ok) = (yyvsp[0].ok);
    }
#line 2918 "CSSGrammar.tab.c"
    break;

  case 133:
#line 1009 "CSSGrammar.y"
                                     {
        (yyval.ok) = false;
    }
#line 2926 "CSSGrammar.tab.c"
    break;

  case 134:
#line 1012 "CSSGrammar.y"
            {
        (yyval.ok) = false;
    }
#line 2934 "CSSGrammar.tab.c"
    break;

  case 135:
#line 1015 "CSSGrammar.y"
                      {
        (yyval.ok) = (yyvsp[-1].ok);
    }
#line 2942 "CSSGrammar.tab.c"
    break;

  case 136:
#line 1021 "CSSGrammar.y"
                                {
        (yyval.ok) = (yyvsp[-2].ok);
    }
#line 2950 "CSSGrammar.tab.c"
    break;

  case 137:
#line 1024 "CSSGrammar.y"
                                                     {
        (yyval.ok) = false;
    }
#line 2958 "CSSGrammar.tab.c"
    break;

  case 138:
#line 1027 "CSSGrammar.y"
                            {
        (yyval.ok) = false;
    }
#line 2966 "CSSGrammar.tab.c"
    break;

  case 139:
#line 1030 "CSSGrammar.y"
                                                     {
        (yyval.ok) = false;
    }
#line 2974 "CSSGrammar.tab.c"
    break;

  case 140:
#line 1033 "CSSGrammar.y"
                                            {
        (yyval.ok) = (yyvsp[-3].ok);
        if ((yyvsp[-2].ok))
            (yyval.ok) = (yyvsp[-2].ok);
    }
#line 2984 "CSSGrammar.tab.c"
    break;

  case 141:
#line 1038 "CSSGrammar.y"
                                      {
        (yyval.ok) = (yyvsp[-3].ok);
    }
#line 2992 "CSSGrammar.tab.c"
    break;

  case 142:
#line 1041 "CSSGrammar.y"
                                                               {
        (yyval.ok) = (yyvsp[-5].ok);
    }
#line 3000 "CSSGrammar.tab.c"
    break;

  case 143:
#line 1047 "CSSGrammar.y"
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
#line 3018 "CSSGrammar.tab.c"
    break;

  case 144:
#line 1061 "CSSGrammar.y"
                                        {
        /* Custom property "--name: value": "--name" now tokenizes as a single
           IDENT (tokenizer customprop rule) and is handled by the regular
           "property ':' expr" path, which routes it to CSS_PROP_CUSTOM_PROPERTY.
           This legacy rule is retained only to gracefully consume any stray
           single-'-' sequence without aborting the stylesheet. */
        (yyval.ok) = false;
    }
#line 3031 "CSSGrammar.tab.c"
    break;

  case 145:
#line 1070 "CSSGrammar.y"
                   {
        (yyval.ok) = false;
    }
#line 3039 "CSSGrammar.tab.c"
    break;

  case 146:
#line 1074 "CSSGrammar.y"
                                             {
        /* The default movable type template has letter-spacing: .none;  Handle this by looking for
        error tokens at the start of an expr, recover the expr and then treat as an error, cleaning
        up and deleting the shifted expr.  */
        (yyval.ok) = false;
    }
#line 3050 "CSSGrammar.tab.c"
    break;

  case 147:
#line 1081 "CSSGrammar.y"
                              {
        /* Handle this case: div { text-align: center; !important } Just reduce away the stray !important. */
        (yyval.ok) = false;
    }
#line 3059 "CSSGrammar.tab.c"
    break;

  case 148:
#line 1086 "CSSGrammar.y"
                             {
        /* div { font-family: } Just reduce away this property with no value. */
        (yyval.ok) = false;
    }
#line 3068 "CSSGrammar.tab.c"
    break;

  case 149:
#line 1093 "CSSGrammar.y"
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
#line 3089 "CSSGrammar.tab.c"
    break;

  case 150:
#line 1112 "CSSGrammar.y"
                              { (yyval.b) = true; }
#line 3095 "CSSGrammar.tab.c"
    break;

  case 151:
#line 1113 "CSSGrammar.y"
                  { (yyval.b) = false; }
#line 3101 "CSSGrammar.tab.c"
    break;

  case 152:
#line 1117 "CSSGrammar.y"
         {
        CSSParser* p = static_cast<CSSParser*>(parser);
        (yyval.valueList) = p->createFloatingValueList();
        (yyval.valueList)->addValue(p->sinkFloatingValue((yyvsp[0].value)));
    }
#line 3111 "CSSGrammar.tab.c"
    break;

  case 153:
#line 1122 "CSSGrammar.y"
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
#line 3130 "CSSGrammar.tab.c"
    break;

  case 154:
#line 1136 "CSSGrammar.y"
                 {
        (yyval.valueList) = 0;
    }
#line 3138 "CSSGrammar.tab.c"
    break;

  case 155:
#line 1142 "CSSGrammar.y"
                    {
        (yyval.tok) = '/';
    }
#line 3146 "CSSGrammar.tab.c"
    break;

  case 156:
#line 1145 "CSSGrammar.y"
                    {
        (yyval.tok) = ',';
    }
#line 3154 "CSSGrammar.tab.c"
    break;

  case 157:
#line 1148 "CSSGrammar.y"
                    {
        (yyval.tok) = '+';
    }
#line 3162 "CSSGrammar.tab.c"
    break;

  case 158:
#line 1151 "CSSGrammar.y"
                    {
        (yyval.tok) = '-';
    }
#line 3170 "CSSGrammar.tab.c"
    break;

  case 159:
#line 1154 "CSSGrammar.y"
                    {
        (yyval.tok) = '*';
    }
#line 3178 "CSSGrammar.tab.c"
    break;

  case 160:
#line 1157 "CSSGrammar.y"
                {
        (yyval.tok) = 0;
  }
#line 3186 "CSSGrammar.tab.c"
    break;

  case 161:
#line 1163 "CSSGrammar.y"
             { (yyval.value) = (yyvsp[0].value); }
#line 3192 "CSSGrammar.tab.c"
    break;

  case 162:
#line 1164 "CSSGrammar.y"
                              { (yyval.value) = (yyvsp[0].value); (yyval.value).fValue *= (yyvsp[-1].i); }
#line 3198 "CSSGrammar.tab.c"
    break;

  case 163:
#line 1165 "CSSGrammar.y"
                       { (yyval.value).id = 0; (yyval.value).string = (yyvsp[-1].string); (yyval.value).unit = CSSPrimitiveValue::CSS_STRING; }
#line 3204 "CSSGrammar.tab.c"
    break;

  case 164:
#line 1166 "CSSGrammar.y"
                      {
      String str = domString((yyvsp[-1].string));
      CString bytes = str.lower().latin1();
      (yyval.value).id = getValueID(bytes.data(), bytes.length());
      (yyval.value).unit = CSSPrimitiveValue::CSS_IDENT;
      (yyval.value).string = (yyvsp[-1].string);
  }
#line 3216 "CSSGrammar.tab.c"
    break;

  case 165:
#line 1174 "CSSGrammar.y"
                      { (yyval.value).id = 0; (yyval.value).string = (yyvsp[-1].string); (yyval.value).unit = CSSPrimitiveValue::CSS_DIMENSION; }
#line 3222 "CSSGrammar.tab.c"
    break;

  case 166:
#line 1175 "CSSGrammar.y"
                                     { (yyval.value).id = 0; (yyval.value).string = (yyvsp[-1].string); (yyval.value).unit = CSSPrimitiveValue::CSS_DIMENSION; }
#line 3228 "CSSGrammar.tab.c"
    break;

  case 167:
#line 1176 "CSSGrammar.y"
                    { (yyval.value).id = 0; (yyval.value).string = (yyvsp[-1].string); (yyval.value).unit = CSSPrimitiveValue::CSS_URI; }
#line 3234 "CSSGrammar.tab.c"
    break;

  case 168:
#line 1177 "CSSGrammar.y"
                             { (yyval.value).id = 0; (yyval.value).iValue = 0; (yyval.value).unit = CSSPrimitiveValue::CSS_UNKNOWN;/* ### */ }
#line 3240 "CSSGrammar.tab.c"
    break;

  case 169:
#line 1178 "CSSGrammar.y"
             { (yyval.value).id = 0; (yyval.value).string = (yyvsp[0].string); (yyval.value).unit = CSSPrimitiveValue::CSS_RGBCOLOR; }
#line 3246 "CSSGrammar.tab.c"
    break;

  case 170:
#line 1179 "CSSGrammar.y"
                    { (yyval.value).id = 0; (yyval.value).string = ParseString(); (yyval.value).unit = CSSPrimitiveValue::CSS_RGBCOLOR; }
#line 3252 "CSSGrammar.tab.c"
    break;

  case 171:
#line 1181 "CSSGrammar.y"
             {
      (yyval.value) = (yyvsp[0].value);
  }
#line 3260 "CSSGrammar.tab.c"
    break;

  case 172:
#line 1184 "CSSGrammar.y"
                    {}
#line 3266 "CSSGrammar.tab.c"
    break;

  case 173:
#line 1188 "CSSGrammar.y"
                      { (yyval.value).id = 0; (yyval.value).isInt = true; (yyval.value).fValue = (yyvsp[-1].val); (yyval.value).unit = CSSPrimitiveValue::CSS_NUMBER; }
#line 3272 "CSSGrammar.tab.c"
    break;

  case 174:
#line 1189 "CSSGrammar.y"
                           { (yyval.value).id = 0; (yyval.value).isInt = false; (yyval.value).fValue = (yyvsp[-1].val); (yyval.value).unit = CSSPrimitiveValue::CSS_NUMBER; }
#line 3278 "CSSGrammar.tab.c"
    break;

  case 175:
#line 1190 "CSSGrammar.y"
                           { (yyval.value).id = 0; (yyval.value).fValue = (yyvsp[-1].val); (yyval.value).unit = CSSPrimitiveValue::CSS_PERCENTAGE; }
#line 3284 "CSSGrammar.tab.c"
    break;

  case 176:
#line 1191 "CSSGrammar.y"
                    { (yyval.value).id = 0; (yyval.value).fValue = (yyvsp[-1].val); (yyval.value).unit = CSSPrimitiveValue::CSS_PX; }
#line 3290 "CSSGrammar.tab.c"
    break;

  case 177:
#line 1192 "CSSGrammar.y"
                    { (yyval.value).id = 0; (yyval.value).fValue = (yyvsp[-1].val); (yyval.value).unit = CSSPrimitiveValue::CSS_CM; }
#line 3296 "CSSGrammar.tab.c"
    break;

  case 178:
#line 1193 "CSSGrammar.y"
                    { (yyval.value).id = 0; (yyval.value).fValue = (yyvsp[-1].val); (yyval.value).unit = CSSPrimitiveValue::CSS_MM; }
#line 3302 "CSSGrammar.tab.c"
    break;

  case 179:
#line 1194 "CSSGrammar.y"
                    { (yyval.value).id = 0; (yyval.value).fValue = (yyvsp[-1].val); (yyval.value).unit = CSSPrimitiveValue::CSS_IN; }
#line 3308 "CSSGrammar.tab.c"
    break;

  case 180:
#line 1195 "CSSGrammar.y"
                    { (yyval.value).id = 0; (yyval.value).fValue = (yyvsp[-1].val); (yyval.value).unit = CSSPrimitiveValue::CSS_PT; }
#line 3314 "CSSGrammar.tab.c"
    break;

  case 181:
#line 1196 "CSSGrammar.y"
                    { (yyval.value).id = 0; (yyval.value).fValue = (yyvsp[-1].val); (yyval.value).unit = CSSPrimitiveValue::CSS_PC; }
#line 3320 "CSSGrammar.tab.c"
    break;

  case 182:
#line 1197 "CSSGrammar.y"
                     { (yyval.value).id = 0; (yyval.value).fValue = (yyvsp[-1].val); (yyval.value).unit = CSSPrimitiveValue::CSS_DEG; }
#line 3326 "CSSGrammar.tab.c"
    break;

  case 183:
#line 1198 "CSSGrammar.y"
                     { (yyval.value).id = 0; (yyval.value).fValue = (yyvsp[-1].val); (yyval.value).unit = CSSPrimitiveValue::CSS_RAD; }
#line 3332 "CSSGrammar.tab.c"
    break;

  case 184:
#line 1199 "CSSGrammar.y"
                      { (yyval.value).id = 0; (yyval.value).fValue = (yyvsp[-1].val); (yyval.value).unit = CSSPrimitiveValue::CSS_GRAD; }
#line 3338 "CSSGrammar.tab.c"
    break;

  case 185:
#line 1200 "CSSGrammar.y"
                      { (yyval.value).id = 0; (yyval.value).fValue = (yyvsp[-1].val); (yyval.value).unit = CSSPrimitiveValue::CSS_MS; }
#line 3344 "CSSGrammar.tab.c"
    break;

  case 186:
#line 1201 "CSSGrammar.y"
                     { (yyval.value).id = 0; (yyval.value).fValue = (yyvsp[-1].val); (yyval.value).unit = CSSPrimitiveValue::CSS_S; }
#line 3350 "CSSGrammar.tab.c"
    break;

  case 187:
#line 1202 "CSSGrammar.y"
                     { (yyval.value).id = 0; (yyval.value).fValue = (yyvsp[-1].val); (yyval.value).unit = CSSPrimitiveValue::CSS_HZ; }
#line 3356 "CSSGrammar.tab.c"
    break;

  case 188:
#line 1203 "CSSGrammar.y"
                      { (yyval.value).id = 0; (yyval.value).fValue = (yyvsp[-1].val); (yyval.value).unit = CSSPrimitiveValue::CSS_KHZ; }
#line 3362 "CSSGrammar.tab.c"
    break;

  case 189:
#line 1204 "CSSGrammar.y"
                    { (yyval.value).id = 0; (yyval.value).fValue = (yyvsp[-1].val); (yyval.value).unit = CSSPrimitiveValue::CSS_EMS; }
#line 3368 "CSSGrammar.tab.c"
    break;

  case 190:
#line 1205 "CSSGrammar.y"
                     { (yyval.value).id = 0; (yyval.value).fValue = (yyvsp[-1].val); (yyval.value).unit = Value::Q_EMS; }
#line 3374 "CSSGrammar.tab.c"
    break;

  case 191:
#line 1206 "CSSGrammar.y"
                    { (yyval.value).id = 0; (yyval.value).fValue = (yyvsp[-1].val); (yyval.value).unit = CSSPrimitiveValue::CSS_EXS; }
#line 3380 "CSSGrammar.tab.c"
    break;

  case 192:
#line 1211 "CSSGrammar.y"
                                              {
        CSSParser* p = static_cast<CSSParser*>(parser);
        Function* f = p->createFloatingFunction();
        f->name = (yyvsp[-4].string);
        f->args = p->sinkFloatingValueList((yyvsp[-2].valueList));
        (yyval.value).id = 0;
        (yyval.value).unit = Value::QFunction;
        (yyval.value).function = f;
    }
#line 3394 "CSSGrammar.tab.c"
    break;

  case 193:
#line 1220 "CSSGrammar.y"
                               {
        CSSParser* p = static_cast<CSSParser*>(parser);
        Function* f = p->createFloatingFunction();
        f->name = (yyvsp[-2].string);
        f->args = 0;
        (yyval.value).id = 0;
        (yyval.value).unit = Value::QFunction;
        (yyval.value).function = f;
  }
#line 3408 "CSSGrammar.tab.c"
    break;

  case 194:
#line 1236 "CSSGrammar.y"
                  { (yyval.string) = (yyvsp[-1].string); }
#line 3414 "CSSGrammar.tab.c"
    break;

  case 195:
#line 1237 "CSSGrammar.y"
                      { (yyval.string) = (yyvsp[-1].string); }
#line 3420 "CSSGrammar.tab.c"
    break;

  case 196:
#line 1244 "CSSGrammar.y"
                            {
        (yyval.rule) = 0;
    }
#line 3428 "CSSGrammar.tab.c"
    break;

  case 197:
#line 1247 "CSSGrammar.y"
                  {
        (yyval.rule) = 0;
    }
#line 3436 "CSSGrammar.tab.c"
    break;

  case 198:
#line 1253 "CSSGrammar.y"
           {
        (yyval.rule) = 0;
    }
#line 3444 "CSSGrammar.tab.c"
    break;

  case 199:
#line 1259 "CSSGrammar.y"
                        {
        (yyval.rule) = 0;
    }
#line 3452 "CSSGrammar.tab.c"
    break;


#line 3456 "CSSGrammar.tab.c"

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
#line 1285 "CSSGrammar.y"

