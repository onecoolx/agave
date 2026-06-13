#ifndef CSSGRAMMAR_H
#define CSSGRAMMAR_H
/* A Bison parser, made by GNU Bison 3.5.1.  */

/* Bison interface for Yacc-like parsers in C

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

/* Undocumented macros, especially those whose name start with YY_,
   are private implementation details.  Do not rely on them.  */

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

#line 138 "CSSGrammar.tab.h"

};
typedef union YYSTYPE YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif



int cssyyparse (void* parser);

#endif /* !YY_CSSYY_CSSGRAMMAR_TAB_H_INCLUDED  */
#endif
