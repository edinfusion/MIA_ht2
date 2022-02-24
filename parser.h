/* A Bison parser, made by GNU Bison 3.7.6.  */

/* Bison interface for Yacc-like parsers in C

   Copyright (C) 1984, 1989-1990, 2000-2015, 2018-2021 Free Software Foundation,
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
   along with this program.  If not, see <https://www.gnu.org/licenses/>.  */

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

/* DO NOT RELY ON FEATURES THAT ARE NOT DOCUMENTED in the manual,
   especially those whose name start with YY_ or yy_.  They are
   private implementation details that can be changed or removed.  */

#ifndef YY_YY_PARSER_H_INCLUDED
# define YY_YY_PARSER_H_INCLUDED
/* Debug traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
#endif
#if YYDEBUG
extern int yydebug;
#endif

/* Token kinds.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
  enum yytokentype
  {
    YYEMPTY = -2,
    YYEOF = 0,                     /* "end of file"  */
    YYerror = 256,                 /* error  */
    YYUNDEF = 257,                 /* "invalid token"  */
    numero = 258,                  /* numero  */
    caracter = 259,                /* caracter  */
    cadena = 260,                  /* cadena  */
    identificador = 261,           /* identificador  */
    ruta = 262,                    /* ruta  */
    extension = 263,               /* extension  */
    idextension = 264,             /* idextension  */
    buscar = 265,                  /* buscar  */
    mkdisk = 266,                  /* mkdisk  */
    rep = 267,                     /* rep  */
    exec = 268,                    /* exec  */
    rmdisk = 269,                  /* rmdisk  */
    fdisk = 270,                   /* fdisk  */
    path_p = 271,                  /* path_p  */
    size_p = 272,                  /* size_p  */
    unit_p = 273,                  /* unit_p  */
    fit_p = 274,                   /* fit_p  */
    type_p = 275,                  /* type_p  */
    delete_p = 276,                /* delete_p  */
    name_p = 277,                  /* name_p  */
    add_p = 278,                   /* add_p  */
    id_p = 279,                    /* id_p  */
    fs_p = 280,                    /* fs_p  */
    user_p = 281,                  /* user_p  */
    pass_p = 282,                  /* pass_p  */
    pwd_p = 283,                   /* pwd_p  */
    grp_p = 284,                   /* grp_p  */
    ugo_p = 285,                   /* ugo_p  */
    cont_p = 286,                  /* cont_p  */
    r_p = 287,                     /* r_p  */
    dest_p = 288,                  /* dest_p  */
    p_p = 289,                     /* p_p  */
    file_p = 290,                  /* file_p  */
    ruta_p = 291,                  /* ruta_p  */
    conte_p = 292,                 /* conte_p  */
    igual = 293                    /* igual  */
  };
  typedef enum yytokentype yytoken_kind_t;
#endif

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
union YYSTYPE
{
#line 20 "sintactico.y"

    char text[100];
    class Comando *Comando_Terminal;

#line 107 "parser.h"

};
typedef union YYSTYPE YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif


extern YYSTYPE yylval;

int yyparse (void);

#endif /* !YY_YY_PARSER_H_INCLUDED  */
