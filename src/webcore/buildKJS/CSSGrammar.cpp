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


#line 194 "CSSGrammar.tab.c"

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
    UNICODERANGE = 307
  };
#endif

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
union YYSTYPE
{
#line 124 "CSSGrammar.y"

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

#line 323 "CSSGrammar.tab.c"

};
typedef union YYSTYPE YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif



int cssyyparse (void* parser);

#endif /* !YY_CSSYY_CSSGRAMMAR_TAB_H_INCLUDED  */

/* Second part of user prologue.  */
#line 148 "CSSGrammar.y"


static inline int cssyyerror(void*, const char*) { return 1; }
static int cssyylex(YYSTYPE* yylval, void*) { return CSSParser::current()->lex(yylval); }


#line 345 "CSSGrammar.tab.c"


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
#define YYLAST   947

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  74
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  65
/* YYNRULES -- Number of rules.  */
#define YYNRULES  196
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  386

#define YYUNDEFTOK  2
#define YYMAXUTOK   307


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
       2,     2,     2,     2,     2,    71,     2,    72,     2,     2,
      61,    62,    18,    64,    63,    67,    16,    70,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,    15,    60,
       2,    69,    66,     2,    73,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,    17,     2,    68,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,    58,    19,    59,    65,     2,     2,     2,
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
      50,    51,    52,    53,    54,    55,    56,    57
};

#if YYDEBUG
  /* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
static const yytype_int16 yyrline[] =
{
       0,   290,   290,   291,   292,   293,   294,   298,   299,   303,
     309,   315,   329,   336,   337,   340,   342,   343,   346,   348,
     353,   359,   361,   365,   367,   374,   376,   379,   381,   389,
     390,   391,   392,   393,   394,   395,   399,   402,   405,   411,
     416,   417,   421,   422,   426,   427,   431,   437,   440,   446,
     453,   458,   465,   468,   472,   475,   478,   484,   492,   495,
     499,   504,   509,   515,   518,   524,   525,   536,   553,   556,
     562,   566,   569,   575,   576,   577,   578,   582,   583,   587,
     593,   596,   604,   610,   613,   633,   639,   640,   641,   645,
     650,   657,   663,   674,   687,   701,   709,   717,   720,   733,
     739,   748,   761,   762,   763,   767,   779,   790,   795,   801,
     810,   823,   826,   829,   832,   835,   838,   844,   845,   849,
     874,   889,   899,   909,   920,   929,   939,   954,   957,   962,
     965,   968,   971,   977,   980,   983,   986,   989,   994,   997,
    1003,  1017,  1021,  1028,  1033,  1040,  1049,  1050,  1054,  1059,
    1073,  1079,  1082,  1085,  1091,  1092,  1093,  1094,  1102,  1103,
    1104,  1105,  1106,  1107,  1109,  1112,  1116,  1117,  1118,  1119,
    1120,  1121,  1122,  1123,  1124,  1125,  1126,  1127,  1128,  1129,
    1130,  1131,  1132,  1133,  1134,  1139,  1148,  1164,  1165,  1172,
    1175,  1181,  1187,  1204,  1205,  1209,  1210
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
  "URI", "FUNCTION", "NOTFUNCTION", "UNICODERANGE", "'{'", "'}'", "';'",
  "'('", "')'", "','", "'+'", "'~'", "'>'", "'-'", "']'", "'='", "'/'",
  "'#'", "'%'", "'@'", "$accept", "stylesheet", "ruleset_or_import",
  "webkit_rule", "webkit_decls", "webkit_value", "webkit_mediaquery",
  "maybe_space", "maybe_sgml", "maybe_charset", "charset", "import_list",
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
     300,   301,   302,   303,   304,   305,   306,   307,   123,   125,
      59,    40,    41,    44,    43,   126,    62,    45,    93,    61,
      47,    35,    37,    64
};
# endif

#define YYPACT_NINF (-208)

#define yypact_value_is_default(Yyn) \
  ((Yyn) == YYPACT_NINF)

#define YYTABLE_NINF (-154)

#define yytable_value_is_error(Yyn) \
  0

  /* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
     STATE-NUM.  */
static const yytype_int16 yypact[] =
{
     271,    31,    -9,    53,    76,    20,    27,  -208,  -208,  -208,
    -208,  -208,  -208,   -20,    18,  -208,  -208,  -208,  -208,  -208,
     144,   144,   144,   144,    40,    59,  -208,  -208,  -208,  -208,
     487,    29,   738,   219,  -208,  -208,   131,   264,   124,   202,
    -208,  -208,   217,   152,  -208,   218,  -208,   143,  -208,  -208,
    -208,    56,   799,   338,  -208,   274,   197,  -208,  -208,  -208,
    -208,    79,  -208,  -208,   102,    36,   145,    11,  -208,  -208,
    -208,  -208,  -208,  -208,  -208,  -208,  -208,  -208,  -208,  -208,
    -208,  -208,  -208,  -208,  -208,  -208,  -208,  -208,  -208,  -208,
    -208,  -208,  -208,  -208,  -208,  -208,  -208,  -208,  -208,   894,
     548,  -208,  -208,  -208,  -208,  -208,  -208,  -208,   180,   228,
    -208,  -208,  -208,   255,  -208,  -208,  -208,   262,   281,  -208,
    -208,   267,  -208,   194,   179,   120,  -208,  -208,  -208,  -208,
    -208,  -208,  -208,   496,  -208,  -208,   274,   197,   144,   197,
    -208,   144,  -208,  -208,   300,   144,   144,  -208,   220,   248,
    -208,    63,  -208,  -208,   144,   144,   144,   144,   144,   144,
     144,   144,   144,   144,   144,   144,   144,   144,   144,   144,
     144,   144,   144,   144,   144,   144,   144,   144,   144,   676,
     144,   144,   144,  -208,  -208,  -208,  -208,  -208,  -208,   855,
     214,  -208,   195,   155,  -208,    40,   273,  -208,   266,   181,
     553,   202,   218,   307,   237,  -208,  -208,  -208,  -208,  -208,
    -208,    29,   553,   144,   144,   144,  -208,   144,   197,   144,
     144,   144,   223,  -208,   323,  -208,   144,   275,  -208,   422,
    -208,   612,   144,   144,   144,  -208,  -208,   301,   244,   236,
     275,   335,  -208,   142,   336,  -208,  -208,  -208,  -208,  -208,
    -208,  -208,  -208,  -208,    40,  -208,  -208,  -208,  -208,  -208,
     277,   144,  -208,   258,  -208,  -208,  -208,  -208,  -208,  -208,
    -208,  -208,   128,   282,   818,   144,  -208,   144,   249,   144,
     144,   855,   358,  -208,   144,  -208,  -208,   301,  -208,  -208,
    -208,   339,     2,  -208,   253,   161,   256,   137,   257,    40,
      46,    47,  -208,  -208,  -208,   146,  -208,   280,    52,  -208,
     144,  -208,   358,  -208,  -208,   144,    61,  -208,  -208,  -208,
    -208,  -208,  -208,    62,  -208,  -208,  -208,  -208,  -208,  -208,
    -208,  -208,  -208,   146,  -208,  -208,  -208,  -208,  -208,  -208,
     144,  -208,   144,  -208,   129,   144,  -208,    29,   149,   177,
    -208,    32,   219,   265,  -208,   425,   144,   285,  -208,  -208,
      35,  -208,  -208,  -208,  -208,  -208,  -208,   433,  -208,    54,
     101,  -208,   144,    19,   144,  -208,   144,  -208,  -208,  -208,
     284,   738,  -208,   484,   144,   144
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
     104,   131,    13,    13,     0,     0,   127,     0,    13,    13,
      13,    13,    13,    13,    13,    13,    13,    13,    13,    13,
      13,    13,    13,    13,    13,    13,    13,    13,    13,    13,
      13,    13,    13,    13,    13,    78,    77,    13,    13,     0,
       0,   148,   154,   164,   162,    55,    56,    13,     0,    27,
      15,   194,   195,     0,    20,    88,   119,     0,    13,    13,
     105,     0,    87,     0,     0,     0,    82,    13,    13,    85,
      13,    13,    13,     0,    95,    96,    13,     0,    89,     0,
      99,    91,    98,    13,     0,   145,   143,    10,   132,   128,
      13,     0,   141,    13,   156,   157,   187,   188,   183,   182,
     184,   169,   170,   171,   172,   173,   174,   175,   176,   177,
     178,   179,   180,   181,   158,   168,   167,   166,   160,     0,
     161,   163,   165,    13,   155,   150,    11,    13,    13,     0,
       0,    12,     0,     0,    15,    24,     0,   120,     0,     0,
       0,    13,     0,     0,     0,    38,    37,    44,    45,    13,
       9,     0,     0,    73,    74,    75,    84,    92,     0,    94,
      90,   135,   130,    13,     0,    13,   133,     0,    13,     0,
     186,     0,   159,   152,   151,   149,    13,    52,     0,    42,
       0,     0,    13,     0,     0,    15,   191,    30,    31,    32,
      29,    34,    35,    33,    26,   193,   196,   121,    13,    13,
       0,   106,    13,     0,   112,   113,   114,   115,   116,   107,
     111,    13,    54,     0,     0,    93,    13,   138,     0,   137,
     134,     0,     0,    13,    67,    13,    50,    53,    57,    41,
      40,     0,     0,   192,     0,    54,     0,     0,     0,    28,
       0,     0,   126,   109,    13,     0,    60,     0,     0,    79,
     136,    13,     0,    13,   140,   185,     0,    51,    43,    13,
      69,    68,    13,     0,    72,    71,    13,   190,   189,   123,
      13,    13,   122,     0,   118,   117,    13,    36,    62,    13,
     139,   142,   146,    13,     0,    65,    13,     0,     0,     0,
      13,     0,    54,     0,    39,     0,    65,     0,    13,    13,
       0,   108,    61,    13,    13,    64,    13,     0,    13,     0,
       0,   110,    46,    47,    66,    63,    70,   124,   125,    13,
       0,     0,    13,     0,    49,    48
};

  /* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -208,  -208,  -208,  -208,  -208,  -208,  -208,    -1,  -100,  -208,
    -208,  -208,  -208,  -208,  -208,   -31,  -208,  -208,    55,  -208,
    -208,    73,  -208,  -208,  -208,   -32,  -208,    66,  -208,    -8,
    -208,  -208,  -208,  -208,  -208,   -27,  -208,   154,   243,  -102,
     315,   -34,   -38,  -208,   170,  -208,   111,    42,  -208,  -207,
    -208,   311,  -208,    65,  -177,  -208,   190,   286,  -208,  -208,
    -208,  -208,  -208,    -2,   -41
};

  /* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
      -1,     6,    48,     7,     8,     9,    10,   261,    24,    11,
      12,    36,   109,   193,   245,    49,   194,   292,   209,   364,
     380,   286,   287,   288,   107,   306,   307,   308,   247,   355,
     237,   248,   249,   133,    99,   366,    51,    52,    53,    54,
      55,    56,    57,    58,   204,    59,   271,   336,    60,    64,
      65,    66,    67,   314,   100,   189,   101,   102,   103,   104,
     251,   252,   253,   112,   113
};

  /* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
     positive, shift that token.  If negative, reduce the rule whose
     number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int16 yytable[] =
{
      14,   108,   231,    50,   273,   110,    20,    21,    22,    23,
     195,    27,   152,   207,    30,    31,    32,    33,   142,   137,
     144,   139,    28,    28,    18,   151,   153,    19,    38,    29,
      61,   216,    13,    28,   379,   -13,    28,   148,    25,    28,
      26,    62,   -13,   121,    34,    35,   124,   125,    62,    15,
      28,    28,   282,   338,   138,   141,   208,   126,    28,    63,
      37,   145,   146,   338,   227,    28,    63,   154,   155,   156,
     157,   158,   159,   160,   161,   162,   163,   164,   165,   166,
     167,   168,   169,   170,   171,   172,   173,   174,   175,   176,
     177,   178,   179,   180,   254,  -129,   181,   182,   260,   142,
     361,   142,   218,   371,   312,    28,   190,   224,   329,   332,
     330,    16,   -59,   331,   127,   339,   377,   199,   200,   128,
     346,   206,   343,   228,    28,   339,   211,   212,    28,   213,
     214,   215,    28,    28,    17,   217,   219,    25,   220,   143,
     357,    28,   221,   296,   123,   299,   -13,   -13,    28,   226,
      28,    47,   229,    28,   -13,    -2,   240,   334,   335,   105,
     106,   147,   246,   378,   120,    28,   250,    39,    40,    41,
      42,    43,    44,    45,    46,    47,   241,   242,   243,   210,
     142,    28,   232,    28,   114,    28,   233,   234,   -58,   354,
     207,   239,   105,   106,   256,   326,   238,   -13,   140,   -13,
     -13,   -13,   358,    25,   383,   150,   -13,   -13,   272,   -13,
      40,    41,    42,    43,    44,   -13,   -13,   275,    28,   322,
     256,   115,   277,    28,   279,   256,   236,   280,   244,   116,
     359,   258,   117,   208,   259,   284,   290,   122,   293,   191,
      28,   295,   297,   264,   265,   266,   267,   268,   291,   -13,
     105,   106,    25,   192,   205,   -13,   196,   300,   301,   -13,
     -13,   -13,   -13,   -13,   264,   265,   266,   267,   268,    28,
     305,    28,   118,   119,   197,   310,   256,   363,    25,   201,
     223,    25,   315,   276,   316,   202,    46,    40,    41,    42,
      43,    44,   321,   198,   325,     1,   328,     2,     3,     4,
       5,   222,    25,   333,   289,   269,   270,    25,   225,   311,
     340,    25,   342,   320,    25,    25,   324,   327,   344,   262,
     362,   345,    25,   111,   278,   347,   303,   270,   257,   348,
     349,    25,   255,    25,   285,   351,   294,   298,   352,   302,
     337,   309,   353,   318,   368,   356,   382,   319,   367,   360,
     134,    40,    41,    42,    43,    44,   135,   369,   370,   185,
     317,   323,   372,   373,   203,   374,   274,   376,   136,  -153,
    -153,  -153,  -153,   263,   304,   350,   149,   341,   381,   235,
       0,   384,   385,     0,     0,   184,     0,     0,   313,     0,
       0,     0,  -153,  -153,  -153,  -153,  -153,  -153,  -153,  -153,
    -153,  -153,  -153,  -153,  -153,  -153,  -153,  -153,  -153,  -153,
    -153,  -153,  -153,  -153,     0,  -153,  -147,  -147,  -147,     0,
       0,   187,  -153,   281,     0,  -153,    28,     0,   188,  -153,
    -153,     0,     0,    68,    69,    70,    71,    39,    40,    41,
      42,    43,    44,    45,    46,    39,    40,    41,    42,    43,
      44,    45,    46,     0,     0,     0,    72,    73,    74,    75,
      76,    77,    78,    79,    80,    81,    82,    83,    84,    85,
      86,    87,    88,    89,    90,    91,    92,    93,     0,    94,
    -144,  -144,  -144,     0,   365,   185,    95,     0,   -13,    96,
       0,    28,   375,    97,    98,  -153,  -153,  -153,  -153,    39,
      40,    41,    42,    43,    44,    45,    46,    47,    39,    40,
      41,    42,    43,    44,    45,    46,     0,     0,  -153,  -153,
    -153,  -153,  -153,  -153,  -153,  -153,  -153,  -153,  -153,  -153,
    -153,  -153,  -153,  -153,  -153,  -153,  -153,  -153,  -153,  -153,
       0,  -153,     0,     0,     0,     0,   -13,   187,  -153,   185,
       0,  -153,     0,     0,   188,  -153,  -153,    28,     0,  -153,
    -153,  -153,  -153,     0,     0,    39,    40,    41,    42,    43,
      44,    45,    46,     0,     0,     0,     0,     0,     0,     0,
       0,     0,  -153,  -153,  -153,  -153,  -153,  -153,  -153,  -153,
    -153,  -153,  -153,  -153,  -153,  -153,  -153,  -153,  -153,  -153,
    -153,  -153,  -153,  -153,     0,  -153,     0,   186,     0,     0,
       0,   187,  -153,   185,     0,  -153,     0,     0,   188,  -153,
    -153,     0,     0,  -153,  -153,  -153,  -153,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,  -153,  -153,  -153,  -153,
    -153,  -153,  -153,  -153,  -153,  -153,  -153,  -153,  -153,  -153,
    -153,  -153,  -153,  -153,  -153,  -153,  -153,  -153,     0,  -153,
       0,     0,     0,     0,   283,   187,  -153,   230,     0,  -153,
      28,     0,   188,  -153,  -153,     0,     0,    68,    69,    70,
      71,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      72,    73,    74,    75,    76,    77,    78,    79,    80,    81,
      82,    83,    84,    85,    86,    87,    88,    89,    90,    91,
      92,    93,     0,    94,     0,     0,     0,     0,     0,     0,
      95,     0,    28,    96,     0,     0,     0,    97,    98,    68,
      69,    70,    71,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    72,    73,    74,    75,    76,    77,    78,    79,
      80,    81,    82,    83,    84,    85,    86,    87,    88,    89,
      90,    91,    92,    93,     0,    94,     0,     0,     0,     0,
     129,     0,    95,     0,     0,    96,     0,     0,     0,    97,
      98,   -76,   -76,   -76,   -76,   -76,   -76,   -76,   -76,   129,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     -76,   -76,   -76,   -76,   -76,   -76,   -76,   -76,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   -80,     0,     0,
       0,     0,   -80,   130,   131,   132,    68,    69,    70,    71,
       0,     0,     0,     0,     0,     0,   -81,     0,     0,     0,
       0,   -81,   130,   131,   132,     0,     0,     0,     0,    72,
      73,    74,    75,    76,    77,    78,    79,    80,    81,    82,
      83,    84,    85,    86,    87,    88,    89,    90,    91,    92,
      93,     0,    94,     0,     0,     0,     0,     0,     0,    95,
       0,     0,    96,     0,     0,     0,    97,    98,    72,    73,
      74,    75,    76,    77,    78,    79,    80,    81,    82,    83,
      84,    85,    86,    87,   183,    89,    90,    91
};

static const yytype_int16 yycheck[] =
{
       1,    33,   179,    30,   211,    36,     7,     8,     9,    10,
     110,    13,     1,    11,    15,    16,    17,    18,    56,    53,
      61,    55,     4,     4,     4,    66,    15,     0,    29,    11,
       1,   133,     1,     4,    15,     4,     4,     1,    58,     4,
      60,    12,    11,    44,     4,     5,    47,    48,    12,    58,
       4,     4,   229,     1,    55,    56,    54,     1,     4,    30,
       1,    62,    63,     1,     1,     4,    30,    68,    69,    70,
      71,    72,    73,    74,    75,    76,    77,    78,    79,    80,
      81,    82,    83,    84,    85,    86,    87,    88,    89,    90,
      91,    92,    93,    94,   194,    59,    97,    98,   200,   137,
      68,   139,   136,    68,   281,     4,   107,   148,    62,    62,
      64,    58,    60,    67,    58,    63,    62,   118,   119,    63,
      58,   123,    61,    60,     4,    63,   127,   128,     4,   130,
     131,   132,     4,     4,    58,   136,   137,    58,   139,    60,
     347,     4,   143,     1,     1,   245,     4,     4,     4,   150,
       4,    20,   153,     4,    11,     0,     1,    11,    12,    31,
      32,    59,   193,    62,    12,     4,   193,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    21,    22,    23,    59,
     218,     4,   183,     4,    60,     4,   187,   188,    60,    60,
      11,   192,    31,    32,   196,    58,     1,    54,     1,     4,
      58,     4,    53,    58,   381,    60,    11,    12,   209,    12,
      13,    14,    15,    16,    17,    18,    19,   218,     4,    58,
     222,    19,   223,     4,   225,   227,    12,   228,    73,    12,
      53,    50,    15,    54,    53,   236,   238,    19,   240,    59,
       4,   242,   243,     6,     7,     8,     9,    10,    12,    54,
      31,    32,    58,    25,    60,    58,     1,   258,   259,    62,
      63,    64,    65,    66,     6,     7,     8,     9,    10,     4,
     271,     4,    55,    56,    12,   276,   278,    12,    58,    12,
      60,    58,   283,    60,   285,    18,    19,    13,    14,    15,
      16,    17,   294,    12,   296,    24,   298,    26,    27,    28,
      29,     1,    58,   304,    60,    68,    69,    58,    60,    60,
     311,    58,   313,    60,    58,    58,    60,    60,   319,    12,
     352,   322,    58,    59,     1,   326,    68,    69,    62,   330,
     331,    58,    59,    58,    33,   336,     1,     1,   339,    62,
      60,    59,   343,     4,    59,   346,    62,   292,   356,   350,
      12,    13,    14,    15,    16,    17,    18,   358,   359,     1,
     287,   295,   363,   364,   121,   366,   212,   368,    53,    11,
      12,    13,    14,   203,   263,   333,    65,   312,   379,   189,
      -1,   382,   383,    -1,    -1,    99,    -1,    -1,    30,    -1,
      -1,    -1,    34,    35,    36,    37,    38,    39,    40,    41,
      42,    43,    44,    45,    46,    47,    48,    49,    50,    51,
      52,    53,    54,    55,    -1,    57,    58,    59,    60,    -1,
      -1,    63,    64,     1,    -1,    67,     4,    -1,    70,    71,
      72,    -1,    -1,    11,    12,    13,    14,    12,    13,    14,
      15,    16,    17,    18,    19,    12,    13,    14,    15,    16,
      17,    18,    19,    -1,    -1,    -1,    34,    35,    36,    37,
      38,    39,    40,    41,    42,    43,    44,    45,    46,    47,
      48,    49,    50,    51,    52,    53,    54,    55,    -1,    57,
      58,    59,    60,    -1,    59,     1,    64,    -1,     4,    67,
      -1,     4,    59,    71,    72,    11,    12,    13,    14,    12,
      13,    14,    15,    16,    17,    18,    19,    20,    12,    13,
      14,    15,    16,    17,    18,    19,    -1,    -1,    34,    35,
      36,    37,    38,    39,    40,    41,    42,    43,    44,    45,
      46,    47,    48,    49,    50,    51,    52,    53,    54,    55,
      -1,    57,    -1,    -1,    -1,    -1,    62,    63,    64,     1,
      -1,    67,    -1,    -1,    70,    71,    72,     4,    -1,    11,
      12,    13,    14,    -1,    -1,    12,    13,    14,    15,    16,
      17,    18,    19,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    34,    35,    36,    37,    38,    39,    40,    41,
      42,    43,    44,    45,    46,    47,    48,    49,    50,    51,
      52,    53,    54,    55,    -1,    57,    -1,    59,    -1,    -1,
      -1,    63,    64,     1,    -1,    67,    -1,    -1,    70,    71,
      72,    -1,    -1,    11,    12,    13,    14,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    34,    35,    36,    37,
      38,    39,    40,    41,    42,    43,    44,    45,    46,    47,
      48,    49,    50,    51,    52,    53,    54,    55,    -1,    57,
      -1,    -1,    -1,    -1,    62,    63,    64,     1,    -1,    67,
       4,    -1,    70,    71,    72,    -1,    -1,    11,    12,    13,
      14,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      34,    35,    36,    37,    38,    39,    40,    41,    42,    43,
      44,    45,    46,    47,    48,    49,    50,    51,    52,    53,
      54,    55,    -1,    57,    -1,    -1,    -1,    -1,    -1,    -1,
      64,    -1,     4,    67,    -1,    -1,    -1,    71,    72,    11,
      12,    13,    14,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    34,    35,    36,    37,    38,    39,    40,    41,
      42,    43,    44,    45,    46,    47,    48,    49,    50,    51,
      52,    53,    54,    55,    -1,    57,    -1,    -1,    -1,    -1,
       1,    -1,    64,    -1,    -1,    67,    -1,    -1,    -1,    71,
      72,    12,    13,    14,    15,    16,    17,    18,    19,     1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      12,    13,    14,    15,    16,    17,    18,    19,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    58,    -1,    -1,
      -1,    -1,    63,    64,    65,    66,    11,    12,    13,    14,
      -1,    -1,    -1,    -1,    -1,    -1,    58,    -1,    -1,    -1,
      -1,    63,    64,    65,    66,    -1,    -1,    -1,    -1,    34,
      35,    36,    37,    38,    39,    40,    41,    42,    43,    44,
      45,    46,    47,    48,    49,    50,    51,    52,    53,    54,
      55,    -1,    57,    -1,    -1,    -1,    -1,    -1,    -1,    64,
      -1,    -1,    67,    -1,    -1,    -1,    71,    72,    34,    35,
      36,    37,    38,    39,    40,    41,    42,    43,    44,    45,
      46,    47,    48,    49,    50,    51,    52,    53
};

  /* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
     symbol of state STATE-NUM.  */
static const yytype_uint8 yystos[] =
{
       0,    24,    26,    27,    28,    29,    75,    77,    78,    79,
      80,    83,    84,     1,    81,    58,    58,    58,     4,     0,
      81,    81,    81,    81,    82,    58,    60,   137,     4,    11,
      81,    81,    81,    81,     4,     5,    85,     1,    81,    12,
      13,    14,    15,    16,    17,    18,    19,    20,    76,    89,
     109,   110,   111,   112,   113,   114,   115,   116,   117,   119,
     122,     1,    12,    30,   123,   124,   125,   126,    11,    12,
      13,    14,    34,    35,    36,    37,    38,    39,    40,    41,
      42,    43,    44,    45,    46,    47,    48,    49,    50,    51,
      52,    53,    54,    55,    57,    64,    67,    71,    72,   108,
     128,   130,   131,   132,   133,    31,    32,    98,    99,    86,
      89,    59,   137,   138,    60,    19,    12,    15,    55,    56,
      12,    81,    19,     1,    81,    81,     1,    58,    63,     1,
      64,    65,    66,   107,    12,    18,   114,   115,    81,   115,
       1,    81,   116,    60,   138,    81,    81,    59,     1,   125,
      60,   138,     1,    15,    81,    81,    81,    81,    81,    81,
      81,    81,    81,    81,    81,    81,    81,    81,    81,    81,
      81,    81,    81,    81,    81,    81,    81,    81,    81,    81,
      81,    81,    81,    50,   131,     1,    59,    63,    70,   129,
      81,    59,    25,    87,    90,    82,     1,    12,    12,    81,
      81,    12,    18,   112,   118,    60,   137,    11,    54,    92,
      59,    81,    81,    81,    81,    81,   113,    81,   115,    81,
      81,    81,     1,    60,   138,    60,    81,     1,    60,    81,
       1,   128,    81,    81,    81,   130,    12,   104,     1,    81,
       1,    21,    22,    23,    73,    88,    89,   102,   105,   106,
     109,   134,   135,   136,    82,    59,   137,    62,    50,    53,
     113,    81,    12,   118,     6,     7,     8,     9,    10,    68,
      69,   120,    81,   123,   111,    81,    60,    81,     1,    81,
      81,     1,   128,    62,    81,    33,    95,    96,    97,    60,
     137,    12,    91,   137,     1,    81,     1,    81,     1,    82,
      81,    81,    62,    68,   120,    81,    99,   100,   101,    59,
      81,    60,   128,    30,   127,    81,    81,    95,     4,    92,
      60,   137,    58,   101,    60,   137,    58,    60,   137,    62,
      64,    67,    62,    81,    11,    12,   121,    60,     1,    63,
      81,   127,    81,    61,    81,    81,    58,    81,    81,    81,
     121,    81,    81,    81,    60,   103,    81,   123,    53,    53,
      81,    68,    99,    12,    93,    59,   109,   103,    59,    81,
      81,    68,    81,    81,    81,    59,    81,    62,    62,    15,
      94,    81,    62,   128,    81,    81
};

  /* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_uint8 yyr1[] =
{
       0,    74,    75,    75,    75,    75,    75,    76,    76,    77,
      78,    79,    80,    81,    81,    82,    82,    82,    83,    83,
      84,    84,    84,    85,    85,    86,    86,    87,    87,    88,
      88,    88,    88,    88,    88,    88,    89,    89,    89,    90,
      90,    90,    91,    91,    92,    92,    93,    94,    94,    95,
      96,    96,    97,    97,    98,    98,    98,    99,   100,   100,
     101,   101,   101,   102,   102,   103,   103,   104,   105,   105,
     106,   106,   106,   107,   107,   107,   107,   108,   108,   109,
     110,   110,   110,   111,   111,   111,   112,   112,   112,   113,
     113,   113,   113,   113,   113,   114,   114,   115,   115,   115,
     116,   116,   116,   116,   116,   117,   118,   119,   119,   119,
     119,   120,   120,   120,   120,   120,   120,   121,   121,   122,
     122,   122,   122,   122,   122,   122,   122,   123,   123,   123,
     123,   123,   123,   124,   124,   124,   124,   124,   124,   124,
     125,   125,   125,   125,   125,   126,   127,   127,   128,   128,
     128,   129,   129,   129,   130,   130,   130,   130,   130,   130,
     130,   130,   130,   130,   130,   130,   131,   131,   131,   131,
     131,   131,   131,   131,   131,   131,   131,   131,   131,   131,
     131,   131,   131,   131,   131,   132,   132,   133,   133,   134,
     134,   135,   136,   137,   137,   138,   138
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
       3,     4,     6,     6,    10,    10,     5,     1,     2,     1,
       3,     1,     2,     3,     4,     3,     5,     4,     4,     6,
       5,     2,     6,     2,     3,     2,     2,     0,     1,     3,
       2,     2,     2,     0,     1,     2,     2,     2,     2,     3,
       2,     2,     1,     2,     1,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     5,     3,     2,     2,     3,
       3,     1,     2,     5,     3,     1,     3
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
#line 303 "CSSGrammar.y"
                                                                      {
        static_cast<CSSParser*>(parser)->rule = (yyvsp[-2].rule);
    }
#line 1908 "CSSGrammar.tab.c"
    break;

  case 10:
#line 309 "CSSGrammar.y"
                                                          {
        /* can be empty */
    }
#line 1916 "CSSGrammar.tab.c"
    break;

  case 11:
#line 315 "CSSGrammar.y"
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
#line 1932 "CSSGrammar.tab.c"
    break;

  case 12:
#line 329 "CSSGrammar.y"
                                                                  {
         CSSParser* p = static_cast<CSSParser*>(parser);
         p->mediaQuery = p->sinkFloatingMediaQuery((yyvsp[-1].mediaQuery));
     }
#line 1941 "CSSGrammar.tab.c"
    break;

  case 19:
#line 348 "CSSGrammar.y"
            {
  }
#line 1948 "CSSGrammar.tab.c"
    break;

  case 20:
#line 353 "CSSGrammar.y"
                                                 {
     CSSParser* p = static_cast<CSSParser*>(parser);
     (yyval.rule) = static_cast<CSSParser*>(parser)->createCharsetRule((yyvsp[-2].string));
     if ((yyval.rule) && p->styleElement && p->styleElement->isCSSStyleSheet())
         p->styleElement->append((yyval.rule));
  }
#line 1959 "CSSGrammar.tab.c"
    break;

  case 21:
#line 359 "CSSGrammar.y"
                                    {
  }
#line 1966 "CSSGrammar.tab.c"
    break;

  case 22:
#line 361 "CSSGrammar.y"
                          {
  }
#line 1973 "CSSGrammar.tab.c"
    break;

  case 24:
#line 367 "CSSGrammar.y"
                                 {
     CSSParser* p = static_cast<CSSParser*>(parser);
     if ((yyvsp[-1].rule) && p->styleElement && p->styleElement->isCSSStyleSheet())
         p->styleElement->append((yyvsp[-1].rule));
 }
#line 1983 "CSSGrammar.tab.c"
    break;

  case 28:
#line 381 "CSSGrammar.y"
                             {
     CSSParser* p = static_cast<CSSParser*>(parser);
     if ((yyvsp[-1].rule) && p->styleElement && p->styleElement->isCSSStyleSheet())
         p->styleElement->append((yyvsp[-1].rule));
 }
#line 1993 "CSSGrammar.tab.c"
    break;

  case 36:
#line 399 "CSSGrammar.y"
                                                                          {
        (yyval.rule) = static_cast<CSSParser*>(parser)->createImportRule((yyvsp[-3].string), (yyvsp[-1].mediaList));
    }
#line 2001 "CSSGrammar.tab.c"
    break;

  case 37:
#line 402 "CSSGrammar.y"
                                   {
        (yyval.rule) = 0;
    }
#line 2009 "CSSGrammar.tab.c"
    break;

  case 38:
#line 405 "CSSGrammar.y"
                         {
        (yyval.rule) = 0;
    }
#line 2017 "CSSGrammar.tab.c"
    break;

  case 39:
#line 411 "CSSGrammar.y"
                                                                        {
    CSSParser* p = static_cast<CSSParser*>(parser);
    if (p->styleElement && p->styleElement->isCSSStyleSheet())
        static_cast<CSSStyleSheet*>(p->styleElement)->addNamespace(p, atomicString((yyvsp[-3].string)), atomicString((yyvsp[-2].string)));
}
#line 2027 "CSSGrammar.tab.c"
    break;

  case 42:
#line 421 "CSSGrammar.y"
            { (yyval.string).characters = 0; }
#line 2033 "CSSGrammar.tab.c"
    break;

  case 43:
#line 422 "CSSGrammar.y"
                   { (yyval.string) = (yyvsp[-1].string); }
#line 2039 "CSSGrammar.tab.c"
    break;

  case 46:
#line 431 "CSSGrammar.y"
                      {
        (yyval.string) = (yyvsp[-1].string);
    }
#line 2047 "CSSGrammar.tab.c"
    break;

  case 47:
#line 437 "CSSGrammar.y"
              {
        (yyval.valueList) = 0;
    }
#line 2055 "CSSGrammar.tab.c"
    break;

  case 48:
#line 440 "CSSGrammar.y"
                                       {
        (yyval.valueList) = (yyvsp[-1].valueList);
    }
#line 2063 "CSSGrammar.tab.c"
    break;

  case 49:
#line 446 "CSSGrammar.y"
                                                                                                      {
        (yyvsp[-4].string).lower();
        (yyval.mediaQueryExp) = static_cast<CSSParser*>(parser)->createFloatingMediaQueryExp(atomicString((yyvsp[-4].string)), (yyvsp[-2].valueList));
    }
#line 2072 "CSSGrammar.tab.c"
    break;

  case 50:
#line 453 "CSSGrammar.y"
                    {
      CSSParser* p = static_cast<CSSParser*>(parser);
      (yyval.mediaQueryExpList) = p->createFloatingMediaQueryExpList();
      (yyval.mediaQueryExpList)->append(p->sinkFloatingMediaQueryExp((yyvsp[0].mediaQueryExp)));
    }
#line 2082 "CSSGrammar.tab.c"
    break;

  case 51:
#line 458 "CSSGrammar.y"
                                           {
      (yyval.mediaQueryExpList) = (yyvsp[-1].mediaQueryExpList);
      (yyval.mediaQueryExpList)->append(static_cast<CSSParser*>(parser)->sinkFloatingMediaQueryExp((yyvsp[0].mediaQueryExp)));
    }
#line 2091 "CSSGrammar.tab.c"
    break;

  case 52:
#line 465 "CSSGrammar.y"
              {
        (yyval.mediaQueryExpList) = static_cast<CSSParser*>(parser)->createFloatingMediaQueryExpList();
    }
#line 2099 "CSSGrammar.tab.c"
    break;

  case 54:
#line 472 "CSSGrammar.y"
              {
        (yyval.mediaQueryRestrictor) = MediaQuery::None;
    }
#line 2107 "CSSGrammar.tab.c"
    break;

  case 55:
#line 475 "CSSGrammar.y"
                 {
        (yyval.mediaQueryRestrictor) = MediaQuery::Only;
    }
#line 2115 "CSSGrammar.tab.c"
    break;

  case 56:
#line 478 "CSSGrammar.y"
                {
        (yyval.mediaQueryRestrictor) = MediaQuery::Not;
    }
#line 2123 "CSSGrammar.tab.c"
    break;

  case 57:
#line 484 "CSSGrammar.y"
                                                                         {
        CSSParser* p = static_cast<CSSParser*>(parser);
        (yyvsp[-1].string).lower();
        (yyval.mediaQuery) = p->createFloatingMediaQuery((yyvsp[-3].mediaQueryRestrictor), domString((yyvsp[-1].string)), p->sinkFloatingMediaQueryExpList((yyvsp[0].mediaQueryExpList)));
    }
#line 2133 "CSSGrammar.tab.c"
    break;

  case 58:
#line 492 "CSSGrammar.y"
                 {
        (yyval.mediaList) = static_cast<CSSParser*>(parser)->createMediaList();
     }
#line 2141 "CSSGrammar.tab.c"
    break;

  case 60:
#line 499 "CSSGrammar.y"
                {
        CSSParser* p = static_cast<CSSParser*>(parser);
        (yyval.mediaList) = p->createMediaList();
        (yyval.mediaList)->appendMediaQuery(p->sinkFloatingMediaQuery((yyvsp[0].mediaQuery)));
    }
#line 2151 "CSSGrammar.tab.c"
    break;

  case 61:
#line 504 "CSSGrammar.y"
                                             {
        (yyval.mediaList) = (yyvsp[-3].mediaList);
        if ((yyval.mediaList))
            (yyval.mediaList)->appendMediaQuery(static_cast<CSSParser*>(parser)->sinkFloatingMediaQuery((yyvsp[0].mediaQuery)));
    }
#line 2161 "CSSGrammar.tab.c"
    break;

  case 62:
#line 509 "CSSGrammar.y"
                       {
        (yyval.mediaList) = 0;
    }
#line 2169 "CSSGrammar.tab.c"
    break;

  case 63:
#line 515 "CSSGrammar.y"
                                                                      {
        (yyval.rule) = static_cast<CSSParser*>(parser)->createMediaRule((yyvsp[-4].mediaList), (yyvsp[-1].ruleList));
    }
#line 2177 "CSSGrammar.tab.c"
    break;

  case 64:
#line 518 "CSSGrammar.y"
                                                             {
        (yyval.rule) = static_cast<CSSParser*>(parser)->createMediaRule(0, (yyvsp[-1].ruleList));
    }
#line 2185 "CSSGrammar.tab.c"
    break;

  case 65:
#line 524 "CSSGrammar.y"
                { (yyval.ruleList) = 0; }
#line 2191 "CSSGrammar.tab.c"
    break;

  case 66:
#line 525 "CSSGrammar.y"
                                       {
        (yyval.ruleList) = (yyvsp[-2].ruleList);
        if ((yyvsp[-1].rule)) {
            if (!(yyval.ruleList))
                (yyval.ruleList) = static_cast<CSSParser*>(parser)->createRuleList();
            (yyval.ruleList)->append((yyvsp[-1].rule));
        }
    }
#line 2204 "CSSGrammar.tab.c"
    break;

  case 67:
#line 536 "CSSGrammar.y"
                    {
      (yyval.string) = (yyvsp[-1].string);
  }
#line 2212 "CSSGrammar.tab.c"
    break;

  case 68:
#line 553 "CSSGrammar.y"
                                 {
      (yyval.rule) = 0;
    }
#line 2220 "CSSGrammar.tab.c"
    break;

  case 69:
#line 556 "CSSGrammar.y"
                       {
      (yyval.rule) = 0;
    }
#line 2228 "CSSGrammar.tab.c"
    break;

  case 70:
#line 563 "CSSGrammar.y"
                                                      {
        (yyval.rule) = static_cast<CSSParser*>(parser)->createFontFaceRule();
    }
#line 2236 "CSSGrammar.tab.c"
    break;

  case 71:
#line 566 "CSSGrammar.y"
                                        {
      (yyval.rule) = 0;
    }
#line 2244 "CSSGrammar.tab.c"
    break;

  case 72:
#line 569 "CSSGrammar.y"
                              {
      (yyval.rule) = 0;
    }
#line 2252 "CSSGrammar.tab.c"
    break;

  case 73:
#line 575 "CSSGrammar.y"
                    { (yyval.relation) = CSSSelector::DirectAdjacent; }
#line 2258 "CSSGrammar.tab.c"
    break;

  case 74:
#line 576 "CSSGrammar.y"
                    { (yyval.relation) = CSSSelector::IndirectAdjacent; }
#line 2264 "CSSGrammar.tab.c"
    break;

  case 75:
#line 577 "CSSGrammar.y"
                    { (yyval.relation) = CSSSelector::Child; }
#line 2270 "CSSGrammar.tab.c"
    break;

  case 76:
#line 578 "CSSGrammar.y"
                { (yyval.relation) = CSSSelector::Descendant; }
#line 2276 "CSSGrammar.tab.c"
    break;

  case 77:
#line 582 "CSSGrammar.y"
        { (yyval.i) = -1; }
#line 2282 "CSSGrammar.tab.c"
    break;

  case 78:
#line 583 "CSSGrammar.y"
        { (yyval.i) = 1; }
#line 2288 "CSSGrammar.tab.c"
    break;

  case 79:
#line 587 "CSSGrammar.y"
                                                       {
        (yyval.rule) = static_cast<CSSParser*>(parser)->createStyleRule((yyvsp[-4].selector));
    }
#line 2296 "CSSGrammar.tab.c"
    break;

  case 80:
#line 593 "CSSGrammar.y"
                                   {
        (yyval.selector) = (yyvsp[0].selector);
    }
#line 2304 "CSSGrammar.tab.c"
    break;

  case 81:
#line 596 "CSSGrammar.y"
                                                                   {
        if ((yyvsp[-3].selector) && (yyvsp[0].selector)) {
            CSSParser* p = static_cast<CSSParser*>(parser);
            (yyval.selector) = (yyvsp[-3].selector);
            (yyval.selector)->append(p->sinkFloatingSelector((yyvsp[0].selector)));
        } else
            (yyval.selector) = 0;
    }
#line 2317 "CSSGrammar.tab.c"
    break;

  case 82:
#line 604 "CSSGrammar.y"
                        {
        (yyval.selector) = 0;
    }
#line 2325 "CSSGrammar.tab.c"
    break;

  case 83:
#line 610 "CSSGrammar.y"
                    {
        (yyval.selector) = (yyvsp[0].selector);
    }
#line 2333 "CSSGrammar.tab.c"
    break;

  case 84:
#line 613 "CSSGrammar.y"
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
#line 2358 "CSSGrammar.tab.c"
    break;

  case 85:
#line 633 "CSSGrammar.y"
                     {
        (yyval.selector) = 0;
    }
#line 2366 "CSSGrammar.tab.c"
    break;

  case 86:
#line 639 "CSSGrammar.y"
                    { (yyval.string).characters = 0; (yyval.string).length = 0; }
#line 2372 "CSSGrammar.tab.c"
    break;

  case 87:
#line 640 "CSSGrammar.y"
              { static UChar star = '*'; (yyval.string).characters = &star; (yyval.string).length = 1; }
#line 2378 "CSSGrammar.tab.c"
    break;

  case 88:
#line 641 "CSSGrammar.y"
                { (yyval.string) = (yyvsp[-1].string); }
#line 2384 "CSSGrammar.tab.c"
    break;

  case 89:
#line 645 "CSSGrammar.y"
                             {
        CSSParser* p = static_cast<CSSParser*>(parser);
        (yyval.selector) = p->createFloatingSelector();
        (yyval.selector)->m_tag = QualifiedName(nullAtom, atomicString((yyvsp[-1].string)), p->defaultNamespace);
    }
#line 2394 "CSSGrammar.tab.c"
    break;

  case 90:
#line 650 "CSSGrammar.y"
                                              {
        (yyval.selector) = (yyvsp[-1].selector);
        if ((yyval.selector)) {
            CSSParser* p = static_cast<CSSParser*>(parser);
            (yyval.selector)->m_tag = QualifiedName(nullAtom, atomicString((yyvsp[-2].string)), p->defaultNamespace);
        }
    }
#line 2406 "CSSGrammar.tab.c"
    break;

  case 91:
#line 657 "CSSGrammar.y"
                                 {
        (yyval.selector) = (yyvsp[-1].selector);
        CSSParser* p = static_cast<CSSParser*>(parser);
        if ((yyval.selector) && p->defaultNamespace != starAtom)
            (yyval.selector)->m_tag = QualifiedName(nullAtom, starAtom, p->defaultNamespace);
    }
#line 2417 "CSSGrammar.tab.c"
    break;

  case 92:
#line 663 "CSSGrammar.y"
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
#line 2433 "CSSGrammar.tab.c"
    break;

  case 93:
#line 674 "CSSGrammar.y"
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
#line 2451 "CSSGrammar.tab.c"
    break;

  case 94:
#line 687 "CSSGrammar.y"
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
#line 2467 "CSSGrammar.tab.c"
    break;

  case 95:
#line 701 "CSSGrammar.y"
          {
        ParseString& str = (yyvsp[0].string);
        CSSParser* p = static_cast<CSSParser*>(parser);
        Document* doc = p->document();
        if (doc && doc->isHTMLDocument())
            str.lower();
        (yyval.string) = str;
    }
#line 2480 "CSSGrammar.tab.c"
    break;

  case 96:
#line 709 "CSSGrammar.y"
          {
        static UChar star = '*';
        (yyval.string).characters = &star;
        (yyval.string).length = 1;
    }
#line 2490 "CSSGrammar.tab.c"
    break;

  case 97:
#line 717 "CSSGrammar.y"
              {
        (yyval.selector) = (yyvsp[0].selector);
    }
#line 2498 "CSSGrammar.tab.c"
    break;

  case 98:
#line 720 "CSSGrammar.y"
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
#line 2516 "CSSGrammar.tab.c"
    break;

  case 99:
#line 733 "CSSGrammar.y"
                           {
        (yyval.selector) = 0;
    }
#line 2524 "CSSGrammar.tab.c"
    break;

  case 100:
#line 739 "CSSGrammar.y"
          {
        CSSParser* p = static_cast<CSSParser*>(parser);
        (yyval.selector) = p->createFloatingSelector();
        (yyval.selector)->m_match = CSSSelector::Id;
        if (!p->strict)
            (yyvsp[0].string).lower();
        (yyval.selector)->m_attr = idAttr;
        (yyval.selector)->m_value = atomicString((yyvsp[0].string));
    }
#line 2538 "CSSGrammar.tab.c"
    break;

  case 101:
#line 748 "CSSGrammar.y"
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
#line 2556 "CSSGrammar.tab.c"
    break;

  case 105:
#line 767 "CSSGrammar.y"
              {
        CSSParser* p = static_cast<CSSParser*>(parser);
        (yyval.selector) = p->createFloatingSelector();
        (yyval.selector)->m_match = CSSSelector::Class;
        if (!p->strict)
            (yyvsp[0].string).lower();
        (yyval.selector)->m_attr = classAttr;
        (yyval.selector)->m_value = atomicString((yyvsp[0].string));
    }
#line 2570 "CSSGrammar.tab.c"
    break;

  case 106:
#line 779 "CSSGrammar.y"
                      {
        ParseString& str = (yyvsp[-1].string);
        CSSParser* p = static_cast<CSSParser*>(parser);
        Document* doc = p->document();
        if (doc && doc->isHTMLDocument())
            str.lower();
        (yyval.string) = str;
    }
#line 2583 "CSSGrammar.tab.c"
    break;

  case 107:
#line 790 "CSSGrammar.y"
                                  {
        (yyval.selector) = static_cast<CSSParser*>(parser)->createFloatingSelector();
        (yyval.selector)->m_attr = QualifiedName(nullAtom, atomicString((yyvsp[-1].string)), nullAtom);
        (yyval.selector)->m_match = CSSSelector::Set;
    }
#line 2593 "CSSGrammar.tab.c"
    break;

  case 108:
#line 795 "CSSGrammar.y"
                                                                                  {
        (yyval.selector) = static_cast<CSSParser*>(parser)->createFloatingSelector();
        (yyval.selector)->m_attr = QualifiedName(nullAtom, atomicString((yyvsp[-5].string)), nullAtom);
        (yyval.selector)->m_match = (CSSSelector::Match)(yyvsp[-4].i);
        (yyval.selector)->m_value = atomicString((yyvsp[-2].string));
    }
#line 2604 "CSSGrammar.tab.c"
    break;

  case 109:
#line 801 "CSSGrammar.y"
                                                       {
        AtomicString namespacePrefix = atomicString((yyvsp[-2].string));
        CSSParser* p = static_cast<CSSParser*>(parser);
        (yyval.selector) = p->createFloatingSelector();
        (yyval.selector)->m_attr = QualifiedName(namespacePrefix,
                                   atomicString((yyvsp[-1].string)),
                                   static_cast<CSSStyleSheet*>(p->styleElement)->determineNamespace(namespacePrefix));
        (yyval.selector)->m_match = CSSSelector::Set;
    }
#line 2618 "CSSGrammar.tab.c"
    break;

  case 110:
#line 810 "CSSGrammar.y"
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
#line 2633 "CSSGrammar.tab.c"
    break;

  case 111:
#line 823 "CSSGrammar.y"
        {
        (yyval.i) = CSSSelector::Exact;
    }
#line 2641 "CSSGrammar.tab.c"
    break;

  case 112:
#line 826 "CSSGrammar.y"
               {
        (yyval.i) = CSSSelector::List;
    }
#line 2649 "CSSGrammar.tab.c"
    break;

  case 113:
#line 829 "CSSGrammar.y"
                {
        (yyval.i) = CSSSelector::Hyphen;
    }
#line 2657 "CSSGrammar.tab.c"
    break;

  case 114:
#line 832 "CSSGrammar.y"
                 {
        (yyval.i) = CSSSelector::Begin;
    }
#line 2665 "CSSGrammar.tab.c"
    break;

  case 115:
#line 835 "CSSGrammar.y"
               {
        (yyval.i) = CSSSelector::End;
    }
#line 2673 "CSSGrammar.tab.c"
    break;

  case 116:
#line 838 "CSSGrammar.y"
               {
        (yyval.i) = CSSSelector::Contain;
    }
#line 2681 "CSSGrammar.tab.c"
    break;

  case 119:
#line 849 "CSSGrammar.y"
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
#line 2711 "CSSGrammar.tab.c"
    break;

  case 120:
#line 874 "CSSGrammar.y"
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
#line 2730 "CSSGrammar.tab.c"
    break;

  case 121:
#line 889 "CSSGrammar.y"
                             {
        (yyval.selector) = static_cast<CSSParser*>(parser)->createFloatingSelector();
        (yyval.selector)->m_match = CSSSelector::PseudoClass;
        (yyval.selector)->m_argument = atomicString((yyvsp[-1].string));
        (yyvsp[-2].string).lower();
        (yyval.selector)->m_value = atomicString((yyvsp[-2].string));
        if ((yyval.selector)->pseudoType() == CSSSelector::PseudoUnknown)
            (yyval.selector) = 0;
    }
#line 2744 "CSSGrammar.tab.c"
    break;

  case 122:
#line 899 "CSSGrammar.y"
                                                       {
        (yyval.selector) = static_cast<CSSParser*>(parser)->createFloatingSelector();
        (yyval.selector)->m_match = CSSSelector::PseudoClass;
        (yyval.selector)->m_argument = String::number((int)(yyvsp[-2].val));
        (yyvsp[-4].string).lower();
        (yyval.selector)->m_value = atomicString((yyvsp[-4].string));
        if ((yyval.selector)->pseudoType() == CSSSelector::PseudoUnknown)
            (yyval.selector) = 0;
    }
#line 2758 "CSSGrammar.tab.c"
    break;

  case 123:
#line 909 "CSSGrammar.y"
                                                     {
        (yyval.selector) = static_cast<CSSParser*>(parser)->createFloatingSelector();
        (yyval.selector)->m_match = CSSSelector::PseudoClass;
        (yyval.selector)->m_argument = atomicString((yyvsp[-2].string));
        (yyvsp[-4].string).lower();
        (yyval.selector)->m_value = atomicString((yyvsp[-4].string));
        if ((yyval.selector)->pseudoType() == CSSSelector::PseudoUnknown)
            (yyval.selector) = 0;
    }
#line 2772 "CSSGrammar.tab.c"
    break;

  case 124:
#line 920 "CSSGrammar.y"
                                                                                         {
        (yyval.selector) = static_cast<CSSParser*>(parser)->createFloatingSelector();
        (yyval.selector)->m_match = CSSSelector::PseudoClass;
        (yyval.selector)->m_argument = domString((yyvsp[-6].string)) + "+" + String::number((int)(yyvsp[-2].val));
        (yyvsp[-8].string).lower();
        (yyval.selector)->m_value = atomicString((yyvsp[-8].string));
        if ((yyval.selector)->pseudoType() == CSSSelector::PseudoUnknown)
            (yyval.selector) = 0;
    }
#line 2786 "CSSGrammar.tab.c"
    break;

  case 125:
#line 929 "CSSGrammar.y"
                                                                                         {
        (yyval.selector) = static_cast<CSSParser*>(parser)->createFloatingSelector();
        (yyval.selector)->m_match = CSSSelector::PseudoClass;
        (yyval.selector)->m_argument = domString((yyvsp[-6].string)) + "-" + String::number((int)(yyvsp[-2].val));
        (yyvsp[-8].string).lower();
        (yyval.selector)->m_value = atomicString((yyvsp[-8].string));
        if ((yyval.selector)->pseudoType() == CSSSelector::PseudoUnknown)
            (yyval.selector) = 0;
    }
#line 2800 "CSSGrammar.tab.c"
    break;

  case 126:
#line 939 "CSSGrammar.y"
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
#line 2817 "CSSGrammar.tab.c"
    break;

  case 127:
#line 954 "CSSGrammar.y"
                {
        (yyval.ok) = (yyvsp[0].ok);
    }
#line 2825 "CSSGrammar.tab.c"
    break;

  case 128:
#line 957 "CSSGrammar.y"
                            {
        (yyval.ok) = (yyvsp[-1].ok);
        if ( (yyvsp[0].ok) )
            (yyval.ok) = (yyvsp[0].ok);
    }
#line 2835 "CSSGrammar.tab.c"
    break;

  case 129:
#line 962 "CSSGrammar.y"
                {
        (yyval.ok) = (yyvsp[0].ok);
    }
#line 2843 "CSSGrammar.tab.c"
    break;

  case 130:
#line 965 "CSSGrammar.y"
                                     {
        (yyval.ok) = false;
    }
#line 2851 "CSSGrammar.tab.c"
    break;

  case 131:
#line 968 "CSSGrammar.y"
            {
        (yyval.ok) = false;
    }
#line 2859 "CSSGrammar.tab.c"
    break;

  case 132:
#line 971 "CSSGrammar.y"
                      {
        (yyval.ok) = (yyvsp[-1].ok);
    }
#line 2867 "CSSGrammar.tab.c"
    break;

  case 133:
#line 977 "CSSGrammar.y"
                                {
        (yyval.ok) = (yyvsp[-2].ok);
    }
#line 2875 "CSSGrammar.tab.c"
    break;

  case 134:
#line 980 "CSSGrammar.y"
                                                     {
        (yyval.ok) = false;
    }
#line 2883 "CSSGrammar.tab.c"
    break;

  case 135:
#line 983 "CSSGrammar.y"
                            {
        (yyval.ok) = false;
    }
#line 2891 "CSSGrammar.tab.c"
    break;

  case 136:
#line 986 "CSSGrammar.y"
                                                     {
        (yyval.ok) = false;
    }
#line 2899 "CSSGrammar.tab.c"
    break;

  case 137:
#line 989 "CSSGrammar.y"
                                            {
        (yyval.ok) = (yyvsp[-3].ok);
        if ((yyvsp[-2].ok))
            (yyval.ok) = (yyvsp[-2].ok);
    }
#line 2909 "CSSGrammar.tab.c"
    break;

  case 138:
#line 994 "CSSGrammar.y"
                                      {
        (yyval.ok) = (yyvsp[-3].ok);
    }
#line 2917 "CSSGrammar.tab.c"
    break;

  case 139:
#line 997 "CSSGrammar.y"
                                                               {
        (yyval.ok) = (yyvsp[-5].ok);
    }
#line 2925 "CSSGrammar.tab.c"
    break;

  case 140:
#line 1003 "CSSGrammar.y"
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
#line 2943 "CSSGrammar.tab.c"
    break;

  case 141:
#line 1017 "CSSGrammar.y"
                   {
        (yyval.ok) = false;
    }
#line 2951 "CSSGrammar.tab.c"
    break;

  case 142:
#line 1021 "CSSGrammar.y"
                                             {
        /* The default movable type template has letter-spacing: .none;  Handle this by looking for
        error tokens at the start of an expr, recover the expr and then treat as an error, cleaning
        up and deleting the shifted expr.  */
        (yyval.ok) = false;
    }
#line 2962 "CSSGrammar.tab.c"
    break;

  case 143:
#line 1028 "CSSGrammar.y"
                              {
        /* Handle this case: div { text-align: center; !important } Just reduce away the stray !important. */
        (yyval.ok) = false;
    }
#line 2971 "CSSGrammar.tab.c"
    break;

  case 144:
#line 1033 "CSSGrammar.y"
                             {
        /* div { font-family: } Just reduce away this property with no value. */
        (yyval.ok) = false;
    }
#line 2980 "CSSGrammar.tab.c"
    break;

  case 145:
#line 1040 "CSSGrammar.y"
                      {
        (yyvsp[-1].string).lower();
        String str = domString((yyvsp[-1].string));
        CString bytes = str.latin1();
        (yyval.prop_id) = getPropertyID(bytes.data(), bytes.length());
    }
#line 2991 "CSSGrammar.tab.c"
    break;

  case 146:
#line 1049 "CSSGrammar.y"
                              { (yyval.b) = true; }
#line 2997 "CSSGrammar.tab.c"
    break;

  case 147:
#line 1050 "CSSGrammar.y"
                  { (yyval.b) = false; }
#line 3003 "CSSGrammar.tab.c"
    break;

  case 148:
#line 1054 "CSSGrammar.y"
         {
        CSSParser* p = static_cast<CSSParser*>(parser);
        (yyval.valueList) = p->createFloatingValueList();
        (yyval.valueList)->addValue(p->sinkFloatingValue((yyvsp[0].value)));
    }
#line 3013 "CSSGrammar.tab.c"
    break;

  case 149:
#line 1059 "CSSGrammar.y"
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
#line 3032 "CSSGrammar.tab.c"
    break;

  case 150:
#line 1073 "CSSGrammar.y"
                 {
        (yyval.valueList) = 0;
    }
#line 3040 "CSSGrammar.tab.c"
    break;

  case 151:
#line 1079 "CSSGrammar.y"
                    {
        (yyval.tok) = '/';
    }
#line 3048 "CSSGrammar.tab.c"
    break;

  case 152:
#line 1082 "CSSGrammar.y"
                    {
        (yyval.tok) = ',';
    }
#line 3056 "CSSGrammar.tab.c"
    break;

  case 153:
#line 1085 "CSSGrammar.y"
                {
        (yyval.tok) = 0;
  }
#line 3064 "CSSGrammar.tab.c"
    break;

  case 154:
#line 1091 "CSSGrammar.y"
             { (yyval.value) = (yyvsp[0].value); }
#line 3070 "CSSGrammar.tab.c"
    break;

  case 155:
#line 1092 "CSSGrammar.y"
                              { (yyval.value) = (yyvsp[0].value); (yyval.value).fValue *= (yyvsp[-1].i); }
#line 3076 "CSSGrammar.tab.c"
    break;

  case 156:
#line 1093 "CSSGrammar.y"
                       { (yyval.value).id = 0; (yyval.value).string = (yyvsp[-1].string); (yyval.value).unit = CSSPrimitiveValue::CSS_STRING; }
#line 3082 "CSSGrammar.tab.c"
    break;

  case 157:
#line 1094 "CSSGrammar.y"
                      {
      String str = domString((yyvsp[-1].string));
      CString bytes = str.lower().latin1();
      (yyval.value).id = getValueID(bytes.data(), bytes.length());
      (yyval.value).unit = CSSPrimitiveValue::CSS_IDENT;
      (yyval.value).string = (yyvsp[-1].string);
  }
#line 3094 "CSSGrammar.tab.c"
    break;

  case 158:
#line 1102 "CSSGrammar.y"
                      { (yyval.value).id = 0; (yyval.value).string = (yyvsp[-1].string); (yyval.value).unit = CSSPrimitiveValue::CSS_DIMENSION; }
#line 3100 "CSSGrammar.tab.c"
    break;

  case 159:
#line 1103 "CSSGrammar.y"
                                     { (yyval.value).id = 0; (yyval.value).string = (yyvsp[-1].string); (yyval.value).unit = CSSPrimitiveValue::CSS_DIMENSION; }
#line 3106 "CSSGrammar.tab.c"
    break;

  case 160:
#line 1104 "CSSGrammar.y"
                    { (yyval.value).id = 0; (yyval.value).string = (yyvsp[-1].string); (yyval.value).unit = CSSPrimitiveValue::CSS_URI; }
#line 3112 "CSSGrammar.tab.c"
    break;

  case 161:
#line 1105 "CSSGrammar.y"
                             { (yyval.value).id = 0; (yyval.value).iValue = 0; (yyval.value).unit = CSSPrimitiveValue::CSS_UNKNOWN;/* ### */ }
#line 3118 "CSSGrammar.tab.c"
    break;

  case 162:
#line 1106 "CSSGrammar.y"
             { (yyval.value).id = 0; (yyval.value).string = (yyvsp[0].string); (yyval.value).unit = CSSPrimitiveValue::CSS_RGBCOLOR; }
#line 3124 "CSSGrammar.tab.c"
    break;

  case 163:
#line 1107 "CSSGrammar.y"
                    { (yyval.value).id = 0; (yyval.value).string = ParseString(); (yyval.value).unit = CSSPrimitiveValue::CSS_RGBCOLOR; }
#line 3130 "CSSGrammar.tab.c"
    break;

  case 164:
#line 1109 "CSSGrammar.y"
             {
      (yyval.value) = (yyvsp[0].value);
  }
#line 3138 "CSSGrammar.tab.c"
    break;

  case 165:
#line 1112 "CSSGrammar.y"
                    {}
#line 3144 "CSSGrammar.tab.c"
    break;

  case 166:
#line 1116 "CSSGrammar.y"
                      { (yyval.value).id = 0; (yyval.value).isInt = true; (yyval.value).fValue = (yyvsp[-1].val); (yyval.value).unit = CSSPrimitiveValue::CSS_NUMBER; }
#line 3150 "CSSGrammar.tab.c"
    break;

  case 167:
#line 1117 "CSSGrammar.y"
                           { (yyval.value).id = 0; (yyval.value).isInt = false; (yyval.value).fValue = (yyvsp[-1].val); (yyval.value).unit = CSSPrimitiveValue::CSS_NUMBER; }
#line 3156 "CSSGrammar.tab.c"
    break;

  case 168:
#line 1118 "CSSGrammar.y"
                           { (yyval.value).id = 0; (yyval.value).fValue = (yyvsp[-1].val); (yyval.value).unit = CSSPrimitiveValue::CSS_PERCENTAGE; }
#line 3162 "CSSGrammar.tab.c"
    break;

  case 169:
#line 1119 "CSSGrammar.y"
                    { (yyval.value).id = 0; (yyval.value).fValue = (yyvsp[-1].val); (yyval.value).unit = CSSPrimitiveValue::CSS_PX; }
#line 3168 "CSSGrammar.tab.c"
    break;

  case 170:
#line 1120 "CSSGrammar.y"
                    { (yyval.value).id = 0; (yyval.value).fValue = (yyvsp[-1].val); (yyval.value).unit = CSSPrimitiveValue::CSS_CM; }
#line 3174 "CSSGrammar.tab.c"
    break;

  case 171:
#line 1121 "CSSGrammar.y"
                    { (yyval.value).id = 0; (yyval.value).fValue = (yyvsp[-1].val); (yyval.value).unit = CSSPrimitiveValue::CSS_MM; }
#line 3180 "CSSGrammar.tab.c"
    break;

  case 172:
#line 1122 "CSSGrammar.y"
                    { (yyval.value).id = 0; (yyval.value).fValue = (yyvsp[-1].val); (yyval.value).unit = CSSPrimitiveValue::CSS_IN; }
#line 3186 "CSSGrammar.tab.c"
    break;

  case 173:
#line 1123 "CSSGrammar.y"
                    { (yyval.value).id = 0; (yyval.value).fValue = (yyvsp[-1].val); (yyval.value).unit = CSSPrimitiveValue::CSS_PT; }
#line 3192 "CSSGrammar.tab.c"
    break;

  case 174:
#line 1124 "CSSGrammar.y"
                    { (yyval.value).id = 0; (yyval.value).fValue = (yyvsp[-1].val); (yyval.value).unit = CSSPrimitiveValue::CSS_PC; }
#line 3198 "CSSGrammar.tab.c"
    break;

  case 175:
#line 1125 "CSSGrammar.y"
                     { (yyval.value).id = 0; (yyval.value).fValue = (yyvsp[-1].val); (yyval.value).unit = CSSPrimitiveValue::CSS_DEG; }
#line 3204 "CSSGrammar.tab.c"
    break;

  case 176:
#line 1126 "CSSGrammar.y"
                     { (yyval.value).id = 0; (yyval.value).fValue = (yyvsp[-1].val); (yyval.value).unit = CSSPrimitiveValue::CSS_RAD; }
#line 3210 "CSSGrammar.tab.c"
    break;

  case 177:
#line 1127 "CSSGrammar.y"
                      { (yyval.value).id = 0; (yyval.value).fValue = (yyvsp[-1].val); (yyval.value).unit = CSSPrimitiveValue::CSS_GRAD; }
#line 3216 "CSSGrammar.tab.c"
    break;

  case 178:
#line 1128 "CSSGrammar.y"
                      { (yyval.value).id = 0; (yyval.value).fValue = (yyvsp[-1].val); (yyval.value).unit = CSSPrimitiveValue::CSS_MS; }
#line 3222 "CSSGrammar.tab.c"
    break;

  case 179:
#line 1129 "CSSGrammar.y"
                     { (yyval.value).id = 0; (yyval.value).fValue = (yyvsp[-1].val); (yyval.value).unit = CSSPrimitiveValue::CSS_S; }
#line 3228 "CSSGrammar.tab.c"
    break;

  case 180:
#line 1130 "CSSGrammar.y"
                     { (yyval.value).id = 0; (yyval.value).fValue = (yyvsp[-1].val); (yyval.value).unit = CSSPrimitiveValue::CSS_HZ; }
#line 3234 "CSSGrammar.tab.c"
    break;

  case 181:
#line 1131 "CSSGrammar.y"
                      { (yyval.value).id = 0; (yyval.value).fValue = (yyvsp[-1].val); (yyval.value).unit = CSSPrimitiveValue::CSS_KHZ; }
#line 3240 "CSSGrammar.tab.c"
    break;

  case 182:
#line 1132 "CSSGrammar.y"
                    { (yyval.value).id = 0; (yyval.value).fValue = (yyvsp[-1].val); (yyval.value).unit = CSSPrimitiveValue::CSS_EMS; }
#line 3246 "CSSGrammar.tab.c"
    break;

  case 183:
#line 1133 "CSSGrammar.y"
                     { (yyval.value).id = 0; (yyval.value).fValue = (yyvsp[-1].val); (yyval.value).unit = Value::Q_EMS; }
#line 3252 "CSSGrammar.tab.c"
    break;

  case 184:
#line 1134 "CSSGrammar.y"
                    { (yyval.value).id = 0; (yyval.value).fValue = (yyvsp[-1].val); (yyval.value).unit = CSSPrimitiveValue::CSS_EXS; }
#line 3258 "CSSGrammar.tab.c"
    break;

  case 185:
#line 1139 "CSSGrammar.y"
                                              {
        CSSParser* p = static_cast<CSSParser*>(parser);
        Function* f = p->createFloatingFunction();
        f->name = (yyvsp[-4].string);
        f->args = p->sinkFloatingValueList((yyvsp[-2].valueList));
        (yyval.value).id = 0;
        (yyval.value).unit = Value::QFunction;
        (yyval.value).function = f;
    }
#line 3272 "CSSGrammar.tab.c"
    break;

  case 186:
#line 1148 "CSSGrammar.y"
                               {
        CSSParser* p = static_cast<CSSParser*>(parser);
        Function* f = p->createFloatingFunction();
        f->name = (yyvsp[-2].string);
        f->args = 0;
        (yyval.value).id = 0;
        (yyval.value).unit = Value::QFunction;
        (yyval.value).function = f;
  }
#line 3286 "CSSGrammar.tab.c"
    break;

  case 187:
#line 1164 "CSSGrammar.y"
                  { (yyval.string) = (yyvsp[-1].string); }
#line 3292 "CSSGrammar.tab.c"
    break;

  case 188:
#line 1165 "CSSGrammar.y"
                      { (yyval.string) = (yyvsp[-1].string); }
#line 3298 "CSSGrammar.tab.c"
    break;

  case 189:
#line 1172 "CSSGrammar.y"
                            {
        (yyval.rule) = 0;
    }
#line 3306 "CSSGrammar.tab.c"
    break;

  case 190:
#line 1175 "CSSGrammar.y"
                  {
        (yyval.rule) = 0;
    }
#line 3314 "CSSGrammar.tab.c"
    break;

  case 191:
#line 1181 "CSSGrammar.y"
           {
        (yyval.rule) = 0;
    }
#line 3322 "CSSGrammar.tab.c"
    break;

  case 192:
#line 1187 "CSSGrammar.y"
                        {
        (yyval.rule) = 0;
    }
#line 3330 "CSSGrammar.tab.c"
    break;


#line 3334 "CSSGrammar.tab.c"

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
#line 1213 "CSSGrammar.y"

