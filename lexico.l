%{
    #include <iostream>
    #include "parser.h"
    Comando *ArbolComandos;
    char comentario[400];
%}

%option case-insensitive
%option noyywrap

/* EXPRESIONES REGULARES */
numero ("-")?[0-9]+
letra [a-zA-ZÑñáéíóúÁÉÍÓÚ]
caracter ({letra}|{numero})
id ({numero}|{letra})({numero}|{letra}|"_")*
iddir {id}(" "{id})*
cadena "\""{id}"\""
extension "."{id}
dir "\""?("/"{iddir})*("/"{iddir}{extension}?)"\""?
idextension {id}{extension}
buscar "\""?("*"|"?")"\""?

%x COMENTARIO

%%
INITIAL
{
    "#"         { BEGIN COMENTARIO; } // comentarios

    "MKDISK"    { return mkdisk; } // COMANDO
    "REP"       { return rep;    } // COMANDO
    "EXEC"      { return exec;   } // COMANDO
    "RMDISK"    { return rmdisk; } // COMANDO
    "FDISK"     { return fdisk;  } // COMANDO

    "-path"      { return path_p;   } // PARAMETRO
    "-size"      { return size_p;   } // PARAMETRO
    "-unit"      { return unit_p;   } // PARAMETRO
    "-fit"       { return fit_p;    } // PARAMETRO
    "-type"      { return type_p;   } // PARAMETRO
    "-delete"    { return delete_p; } // PARAMETRO
    "-name"      { return name_p;   } // PARAMETRO
    "-add"       { return add_p;    } // PARAMETRO
    "-id"        { return id_p;     } // PARAMETRO
    "-fs"        { return fs_p;     } // PARAMETRO
    "-destino"   { return dest_p;   } // PARAMETRO
    "-p"         { return p_p;      } // PARAMETRO
    "-file"      { return file_p;   } // PARAMETRO
    "-ruta"      { return ruta_p;   } // PARAMETRO
    "-contenido" { return conte_p;  } // PARAMETRO

    "="         { return igual;      }

    {extension}     {strcpy(yylval.text, yytext); return extension;}
    {idextension}   {strcpy(yylval.text, yytext); return idextension;}
    {buscar}        {strcpy(yylval.text, yytext); return buscar;}
    {numero}        {strcpy(yylval.text, yytext); return numero;}
    {caracter}      {strcpy(yylval.text, yytext); return caracter;}
    {cadena}        {strcpy(yylval.text, yytext); return cadena;}
    {id}            {strcpy(yylval.text, yytext); return identificador;}
    {dir}           {strcpy(yylval.text, yytext); return ruta;}

    [[:blank:]]     {}

    .               {std::cout << "ERROR AL ESCRIBIR COMANDO(Lexico), VERIFIQUE [: "<<yytext <<":]" << std::endl;}

}

<COMENTARIO>"\n"    {BEGIN INITIAL; /*strcpy(yylval.text, comentario); memset(comentario,0,400); return comentario;*/};
<COMENTARIO>.       {/*strcat(comentario, yytext);*/};

%%
