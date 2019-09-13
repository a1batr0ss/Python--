/* A Bison parser, made by GNU Bison 3.0.4.  */

/* Bison interface for Yacc-like parsers in C

   Copyright (C) 1984, 1989-1990, 2000-2015 Free Software Foundation, Inc.

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

#ifndef YY_YY_Y_TAB_H_INCLUDED
# define YY_YY_Y_TAB_H_INCLUDED
/* Debug traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
#endif
#if YYDEBUG
extern int yydebug;
#endif

/* Token type.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
  enum yytokentype
  {
    IF = 258,
    FOR = 259,
    IN = 260,
    DEF = 261,
    ELSE = 262,
    BREAK = 263,
    CONTINUE = 264,
    RETURN = 265,
    NUMBERS = 266,
    ID = 267,
    CHARS = 268,
    STRING = 269,
    DOT = 270,
    LE_S_BRACKET = 271,
    RG_S_BRACKET = 272,
    LE_M_BRACKET = 273,
    RG_M_BRACKET = 274,
    LE_B_BRACKET = 275,
    RG_B_BRACKET = 276,
    COMMA = 277,
    COLON = 278,
    QUOTE_MARK = 279,
    ADD = 280,
    MINUS = 281,
    TIMES = 282,
    DIV = 283,
    MOD = 284,
    ASSIGN = 285,
    GREATER = 286,
    LESS = 287,
    EQ = 288,
    EOL = 289
  };
#endif
/* Tokens.  */
#define IF 258
#define FOR 259
#define IN 260
#define DEF 261
#define ELSE 262
#define BREAK 263
#define CONTINUE 264
#define RETURN 265
#define NUMBERS 266
#define ID 267
#define CHARS 268
#define STRING 269
#define DOT 270
#define LE_S_BRACKET 271
#define RG_S_BRACKET 272
#define LE_M_BRACKET 273
#define RG_M_BRACKET 274
#define LE_B_BRACKET 275
#define RG_B_BRACKET 276
#define COMMA 277
#define COLON 278
#define QUOTE_MARK 279
#define ADD 280
#define MINUS 281
#define TIMES 282
#define DIV 283
#define MOD 284
#define ASSIGN 285
#define GREATER 286
#define LESS 287
#define EQ 288
#define EOL 289

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
typedef int YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif


extern YYSTYPE yylval;

int yyparse (void);

#endif /* !YY_YY_Y_TAB_H_INCLUDED  */
