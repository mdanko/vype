/* A Bison parser, made by GNU Bison 3.0.2.  */

/* Bison interface for Yacc-like parsers in C

   Copyright (C) 1984, 1989-1990, 2000-2013 Free Software Foundation, Inc.

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

#ifndef YY_YY_PARSER_TAB_H_INCLUDED
# define YY_YY_PARSER_TAB_H_INCLUDED
/* Debug traces.  */
#ifndef YYDEBUG
# define YYDEBUG 1
#endif
#if YYDEBUG
extern int yydebug;
#endif

/* Token type.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
  enum yytokentype
  {
    TID = 258,
    TINT = 259,
    TCHAR = 260,
    TSTRING = 261,
    TKEY_CHAR = 262,
    TKEY_ELSE = 263,
    TKEY_IF = 264,
    TKEY_INT = 265,
    TKEY_RETURN = 266,
    TKEY_STRING = 267,
    TKEY_VOID = 268,
    TKEY_WHILE = 269,
    TRES_BREAK = 270,
    TRES_CONT = 271,
    TRES_FOR = 272,
    TRES_SHORT = 273,
    TRES_UNSIGNED = 274,
    TASS = 275,
    TLPAR = 276,
    TRPAR = 277,
    TLBRAC = 278,
    TRBRAC = 279,
    TDOT = 280,
    TCOM = 281,
    TSEMICOL = 282,
    TEOF = 283,
    TOP_EQ = 284,
    TOP_NEQ = 285,
    TOP_LT = 286,
    TOP_LTE = 287,
    TOP_GT = 288,
    TOP_GTE = 289,
    TOP_PLUS = 290,
    TOP_MINUS = 291,
    TOP_MUL = 292,
    TOP_DIV = 293,
    TOP_MOD = 294,
    TOP_NEG = 295,
    TOP_AND = 296,
    TOP_OR = 297
  };
#endif

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
typedef union YYSTYPE YYSTYPE;
union YYSTYPE
{
#line 20 "parser.y" /* yacc.c:1909  */

    string *id;
    int ival;
    char cval;
    string *sval;
    int token;

    int dtval;
    symbol *eval;

#line 108 "parser.tab.h" /* yacc.c:1909  */
};
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif


extern YYSTYPE yylval;

int yyparse (void);

#endif /* !YY_YY_PARSER_TAB_H_INCLUDED  */
