%{
#include <iostream>
#include "scanner.h"
#include "comando.h"

using namespace std;

extern int yylineno; /*linea del parser*/
extern int columna; /*columna del parser*/
extern char *yytext; /*lexema donde esta el parser*/
extern Comando *ArbolComandos; // Raiz del arbol

int yyerror(const char* mens){
    std::cout<<mens<<std::endl;
    return 0;
}
%}

%union
{
    char text[100];
    class Comando *Comando_Terminal;
}
/* -- -- -- -- -- -- -- -- -- --
   -- -- -- Terminales  -- -- --
   -- -- -- -- -- -- -- -- -- -- */
%token <text> numero
%token <text> caracter
%token <text> cadena
%token <text> identificador
%token <text> ruta
%token <text> extension
%token <text> idextension
%token <text> buscar

// -- -- -- COMANDOS -- -- --
%token <text> mkdisk
%token <text> rep
%token <text> exec
%token <text> rmdisk
%token <text> fdisk

// -- -- -- PARAMETROS -- -- --
%token <text> path_p
%token <text> size_p
%token <text> unit_p
%token <text> fit_p
%token <text> type_p
%token <text> delete_p
%token <text> name_p
%token <text> add_p
%token <text> id_p
%token <text> fs_p
%token <text> user_p
%token <text> pass_p
%token <text> pwd_p
%token <text> grp_p
%token <text> ugo_p
%token <text> cont_p
%token <text> r_p
%token <text> dest_p
%token <text> p_p
%token <text> file_p
%token <text> ruta_p
%token <text> conte_p

// -- -- -- simbolos -- -- --
igual

/* -- -- -- -- -- -- -- -- -- -- --
   -- -- -- No Terminales  -- -- --
   -- -- -- -- -- -- -- -- -- -- -- */

%type <Comando_Terminal> INICIO
%type <Comando_Terminal> LISTA_COMANDO
%type <Comando_Terminal> MKDISK
%type <Comando_Terminal> P_MKDISK
%type <Comando_Terminal> REP
%type <Comando_Terminal> SCRIPT
%type <Comando_Terminal> FDISK
%type <Comando_Terminal> P_FDISK


/* -- -- -- -- -- -- -- -- -- --
   -- -- --  GRAMATICA  -- -- --
   -- -- -- -- -- -- -- -- -- --  */

%start INICIO
%%

INICIO: LISTA_COMANDO { ArbolComandos = $$; };

LISTA_COMANDO:      mkdisk MKDISK  {$$ = new Comando("MKDISK",""); $$->agregarParametros(*$2);}
|   REP    { $$ = $1; }
|   SCRIPT { $$ = $1; }
|   fdisk FDISK   {$$ = new Comando("FDISK", ""); $$->agregarParametros(*$2);}
;

SCRIPT:             exec path_p igual ruta {$$ = new Comando("EXEC",""); Comando *n = new Comando("PATH",$4); $$->agregarParametros(*n);}
;

// -- -- -- CREACION DE DISCO -- -- --
MKDISK:             MKDISK P_MKDISK { $$ = $1; $$->agregarParametros(*$2); }
|   P_MKDISK        { $$ = new Comando("PARAMETROS",""); $$->agregarParametros(*$1); }
;

P_MKDISK:           size_p igual numero { $$ = new Comando("SIZE",$3); }
|   unit_p igual caracter { $$ = new Comando("UNIT",$3);}
|   path_p igual ruta  { $$ = new Comando("PATH",$3); }
;


// -- -- -- PARTICIONES -- -- --
FDISK:              FDISK P_FDISK { $$ = $1; $$->agregarParametros(*$2); }
|   P_FDISK         { $$ = new Comando("PARAMETROS",""); $$->agregarParametros(*$1);}
;

P_FDISK:            size_p igual numero { $$ = new Comando("SIZE",$3); } //-size = numero
|   unit_p igual caracter { $$ = new Comando("UNIT",$3);} // –Unit=K
|   path_p igual ruta  { $$ = new Comando("PATH",$3); } //-path=/home/Disco1.dk
|   name_p igual identificador { $$ = new Comando("NAME",$3);}//-name=Particion1
|   name_p igual cadena { $$ = new Comando("NAME",$3); }

REP:                rep path_p igual ruta { $$ = new Comando("REP",""); Comando *n = new Comando("PATH",$4); $$->agregarParametros(*n); }

