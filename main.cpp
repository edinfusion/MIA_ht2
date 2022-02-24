#include <QCoreApplication>
#include <iostream>
#include <fstream> //Libreria para abrir archivo
#include <QFileInfo>
#include <QDir>
#include "scanner.h"
#include "parser.h"
#include "comando.h"
#include "estructuras.h"


using namespace std;
extern int yyparse();
extern Comando *ArbolComandos; // se obtiene raiz del arbol que se genero en parser




/* -- -- -- --  --  -- -- -- -- -- -- --
   -- -- -- -- LISTA METODOS -- -- -- --
   -- -- -- --  --  -- -- -- -- -- -- -- */
void leerComando(char[400]);

//enrutador
void identificarComando(Comando*);

//crear archivo disco
void crearArchivo(QString dir);

//quitarArchivo
QString getRuta(QString ruta);

//escribir en disco
void escribirDisco(QString directorio, int tamanodisco,MBR &infodisco);
/*---- METODOS ACCIONES COMANDOS ----*/
//Creacion de disco
void ejecutarMKDISK(Comando*);
//leer scripts
void ejecutarEXEC(Comando*);
//particionar disco duro
void ejecutarFDISK(Comando*);
//cracion de reporte
void ejecutarREP(Comando*);
//
void crearParticion(QString ruta,QString nombre,int tamano, char unidad);
//
bool particionRepetida(QString nombre, QString ruta);
//
void reporteDisco(QString rutadisco,QString destinoreporte);

/* -- -- -- --  --  -- -- -- -- -- -- --
   -- -- --  METODOS AUXILIARES -- -- --
   -- -- -- --  --  -- -- -- -- -- -- -- */

/* -- -- -- --  --  -- -- -- -- -- -- --
   -- -- -- -- CREAR ARCHIVO -- -- -- --
   -- -- -- --  --  -- -- -- -- -- -- -- */
void crearArchivo(QString dir)
{
    //acciones
    FILE *file;
    string archivo = dir.toStdString();
    QString ruta = getRuta(dir);//SE HACE EL SPLIT PARA QUITAR EL ARCHIVO A CREAR DE LA RUTA DE DIRECTORIO
    string ejecutarMAKEDIR = "sudo mkdir -p \'"+ruta.toStdString()+"\'";//crea directorio
    string ejecutarCHMOD   = "sudo chmod -R 777 \'"+ruta.toStdString()+"\'"; // da permisos y es legible por todas las carpetas
    //ejecucion
    system(ejecutarMAKEDIR.c_str());
    system(ejecutarCHMOD.c_str());
    //creacion de archivo
    file = fopen(archivo.c_str(),"wb");
    if((file = fopen(archivo.c_str(),"wb"))) fclose(file);
    else cout << "ERROR, NO ES POSIBLE CREAR EL ARCHIVO. VERIFIQUE" << endl;
}

/* -- -- -- --  --  -- -- -- -- -- -- --
   -- -- -- --    Get Ruta   -- -- -- --
   -- -- -- --  --  -- -- -- -- -- -- -- */
QString getRuta(QString ruta)
{
    string copyruta = ruta.toStdString();
    string separador = "/";
    size_t pos = 0;
    string res = "";
    while((pos = copyruta.find(separador))!=string::npos)
    {
        res += copyruta.substr(0,pos)+"/";
        copyruta.erase(0,pos + separador.length());
    }
    return QString::fromStdString(res);
}

/* -- -- -- --  --  -- -- -- -- -- -- --
   -- -- -- -- ESCRIBIR DISCO-- -- -- --
   -- -- -- --  --  -- -- -- -- -- -- -- */
void escribirDisco(QString directorio, int tamanodisco,MBR &infodiscoo)
{
    string instruccion = "dd if=/dev/zero of=\""+directorio.toStdString()+"\" bs=1024 count="+to_string(tamanodisco);//llena de ceros el archivo los mb o kb especificados
    system(instruccion.c_str());
    FILE *archivo = fopen(directorio.toStdString().c_str(),"rb+");
    fseek(archivo,0,SEEK_SET);
    fwrite(&infodiscoo,sizeof (infodiscoo),1,archivo);
    fclose(archivo);
    cout << "INFORMACION, DISCO CREADO CON EXITO" << endl;
}


/* -- -- -- --  --  -- -- -- -- -- -- --
   -- -- -- --      MAIN     -- -- -- --
   -- -- -- --  --  -- -- -- -- -- -- -- */
int main(int argc, char *argv[])
{
    cout << " **************** HOJA DE TRABAJO 2 **************** " << endl;
    cout << "    EDIN MONTENEGRO - 201709311      " << endl;
    cout << "    HOJA DE TRABAJO - 2   " << endl;
    cout << "    INGRESE UN COMANDO: " << endl;

    while (true)
    {
        char input[500];
        printf(">> ");
        fgets(input, sizeof (input), stdin);
        leerComando(input);

        memset(input,0,500);
    }
    return 0;
}
/* -- -- -- --  --  -- -- -- -- -- -- --
   -- -- -- -- LEER COMANDO  -- -- -- --
   -- -- -- --  --  -- -- -- -- -- -- -- */
void leerComando(char comando[400])
{
    if(comando[0] != '#')//sintaxis de comentario
    {
        yy_scan_string(comando); //recibe char y lo verifica
        if(yyparse()==0)        //si no se obtuvo erro en sintaxis se va a identificar el comando
        {
            identificarComando(ArbolComandos);
        }
        else
        {
            cout << "ERROR SINTACTICO: " << endl; //de lo contario ocurre un error
        }
    }
}


/* -- -- -- --  --  -- -- -- -- -- -- --
   -- -- -- --  ID COMANDOS  -- -- -- --
   -- -- -- --  --  -- -- -- -- -- -- -- */
void identificarComando(Comando *comandos)
{
    //enrutador para diversos metodos y se obtiene los parametros de los comandos
    switch (comandos->tipo)
    {
    /*** MKDISK **/
    case 1: { Comando params = comandos->parametros.at(0); ejecutarMKDISK(&params);}
        break;
    /*** REP ***/
    case 2: { ejecutarREP(ArbolComandos);}
        break;
    /*** EXEC ***/
    case 3: {ejecutarEXEC(ArbolComandos);}
        break;
    /***FDISK***/
    case 5: { Comando params = comandos->parametros.at(0); ejecutarFDISK(&params);}
        break;
    default: printf("COMANDO INVALIDO, VERIFIQUE\n");
    }
}

/* -- -- -- --  --  -- -- -- -- -- -- --
   -- -- -- --     MKDISK    -- -- -- --
   -- -- -- --  --  -- -- -- -- -- -- -- */
void ejecutarMKDISK(Comando *params)
{
    //indicadores de parameto MKDISK
    bool bsize = false;
    bool bunit = false;
    bool bpath = false;

    //valores
    int tamañoDisco = 0;    //parametro size valor que tendra(obligatorio)
    char unidad     = 'm';  //parametro unit valor que tenra (opcional)
    QString dir     = "";   //parametro paht directorio (obligatorio)

    //bandera si existen errores
    bool existeError = false;

    for(int i = 0; i < params->parametros.count(); i++)
    {
        Comando parametro = params->parametros.at(i);
        switch (parametro.tipo)
        {

        case 30:
        {   //SIZE
            bsize = true;
            tamañoDisco = parametro.valor.toInt();// se obtiene tamaño de disco
            /*validacion de que el tamaño sea mayor que cero*/
            if (tamañoDisco < 0)
            {
                cout << "ERROR, EL TAMAÑO DEL DISCO DEBE DE SER UN NUMERO POSITIVO Y MAYOR A CERO" << endl;
                existeError = true;
            }
        }
            break;

        case 31:
        {
            //UNIT
            bunit = true;
            unidad = parametro.valor.toStdString()[0]; //ya que es char es necesaria la conversion
            if(unidad == 'k' || unidad == 'K') unidad = 'K';
            else if(unidad == 'm' || unidad == 'M') unidad = 'M';
            else {cout << "ERROR, UNIDAD INCORRECTA" << endl; existeError = true;}
        }
            break;
        case 32:
        {
            //path
            bpath = true;
            dir = parametro.valor; // se obtiene directorio
            //LIMPIEZA DIR
            dir = dir.replace("\"","");
        }
            break;
        }
    }

    if(existeError==false) // si todo fue bien
    {
        //validacion de EXISTENCIA de parametros obligatorios
        if (bpath && bsize)
        {
            int tamañoDisk = 1024; //TAMAÑO Y FACTOR DE CONVERSION DISCO
            MBR infodisco;         //MBR INICIAL AL CREAR DISK
            int randomid =100000 + rand() % (1000000 +1 - 100000);

            //TAMAÑO MBR
            if(bunit)
            {
                if(unidad == 'M')
                {
                    infodisco.mbr_tamano = tamañoDisco*tamañoDisk*tamañoDisk; //tamaño en bytes para Mb
                    tamañoDisk *= tamañoDisco;
                }
                else //K
                {
                    infodisco.mbr_tamano = tamañoDisco*tamañoDisk;
                    tamañoDisk = tamañoDisco;
                }
            }
            else //default M
            {
                infodisco.mbr_tamano = tamañoDisco*tamañoDisk*tamañoDisk; //tamaño en bytes para Mb
                tamañoDisk *= tamañoDisco;
            }

            //FECHA MBR
            infodisco.mbr_fecha_creacion = time(nullptr);

            //ID MBR
            infodisco.mbr_dsk_signature  = randomid;

            //PARTICIONES MBR
            for(int i=0; i < 4; i++)
            {
                infodisco.mbrPartition[i].part_start = -1; //-1 valor default sin particion
                infodisco.mbrPartition[i].part_size = 0;
                strcpy(infodisco.mbrPartition[i].part_name,"");
            }

            //CREACIÓN DE ARCHIVO:
            crearArchivo(dir);

            //ESCRIBIR DISCO
            escribirDisco(dir,tamañoDisk,infodisco);
        }
        else
        {
            cout << "ERROR, VERIFIQUE PARAMETROS OBLIGATORIOS EN MKDISK (-path, -size)" << endl;
        }
    }
}


/* -- -- -- --  --  -- -- -- -- -- -- --
   -- -- -- --      EXEC     -- -- -- --
   -- -- -- --  --  -- -- -- -- -- -- -- */
void ejecutarEXEC(Comando *params)
{
    string directorio = params->parametros.at(0).valor.toStdString();
    FILE *archivo;
    if ((archivo = fopen(directorio.c_str(),"r")))
    {
        char lineas[400]="";
        memset(lineas,0,sizeof(lineas)); //funcion copia determinado caracter la cantidad de veces indicada
        while(fgets(lineas,sizeof lineas,archivo))
        {
            if (lineas[0] != '\n')
            {
                cout << lineas;
                leerComando(lineas);
            }
            memset(lineas,0,sizeof(lineas));
        }
        fclose(archivo);
    }
    else
    {
        cout << "LA RUTA ES INCORRECTA O EL ARCHIVO NO EXISTE"<<endl;
    }
}


/* -- -- -- --  --  -- -- -- -- -- -- --
   -- -- -- --     FDISK     -- -- -- --
   -- -- -- --  --  -- -- -- -- -- -- -- */
void ejecutarFDISK(Comando *params)
{
    //indicadores de parameto FDISK
    bool bsize   = false; //obligatorio //30
    bool bunit   = false;               //31
    bool bpath   = false; //obligatorio //32
    bool bname   = false; //obligatorio //36

    //valores
    int tamañoparticion = 0;      //parametro size valor que tendra(obligatorio)
    char unidad         = 'K';    //parametro unit valor que tenra (opcional)
    QString dir         = "";     //parametro paht directorio (obligatorio)
    QString nombre      = "";     //parametro name, valor que tendra(obligatorio)

    //bandera si existen errores
    bool existeError = false;

    for(int i = 0; i < params->parametros.count(); i++)
    {
        Comando parametro = params->parametros.at(i);
        switch (parametro.tipo)
        {

        case 30:
        {   //SIZE
            //valido que si ya existe add, entonces size no existe
            bsize = true;
            tamañoparticion = parametro.valor.toInt();// se obtiene tamaño de disco
            /*validacion de que el tamaño sea mayor que cero*/
            if (tamañoparticion < 0)
            {
                cout << "ERROR, EL TAMAÑO DE LA PARTICION DEBE DE SER UN NUMERO POSITIVO Y MAYOR A CERO" << endl;
                existeError = true;
            }
        }
            break;
        case 31:
        {
            //UNIT
            bunit = true;
            unidad = parametro.valor.toStdString()[0]; //ya que es char es necesaria la conversion
            if(unidad == 'm' || unidad == 'M') unidad = 'M';
            else if(unidad == 'b' || unidad == 'B') unidad = 'B';
            else if(unidad == 'k' || unidad == 'K') unidad = 'K';
            else {cout << "ERROR, UNIDAD INCORRECTA (CORRECTAS M, K Y B)" << endl; existeError = true;}
        }
            break;

        case 32:
        {
            //path
            bpath = true;
            dir = parametro.valor; // se obtiene directorio
            //LIMPIEZA DIR
            dir = dir.replace("\"","");
        }
            break;
        case 36:
        {
            //NAME
            bname = true;
            nombre = parametro.valor; // se obtiene nombre particion
            //LIMPIEZA DIR  DE COMILLAS
            nombre = nombre.replace("\"","");
        }
            break;
        }//parentesis fin switch
    }//parent for

    //VALIDACIONES PARA CREAR, AGREGAR O ELIMINAR PARTICION
    if(existeError==false) // si todo fue bien
    {
        //validacion de EXISTENCIA de parametros obligatorios

        //CREAR PARTICION
        if (bsize) // si existe size NO DEBE DE VENIR DELETE
        {

            if(bpath && bname)//bsize debe de contener nombre de partcion y ruta sino F
            {
                crearParticion(dir,nombre,tamañoparticion,unidad);
                //particionp *np = new particionp(dir,nombre,tamañoparticion,ajuste,unidad);
                //np->ejecutarParticionPrimaria();
            }
            else
            {
                cout << "ERROR, PARAMETRO -SIZE DEBE INCLUIR -NAME Y -PATH. VERIFIQUE " << endl;
            }

        }
    }

    else
    {
        cout << "ERROR, UNO DE LOS PARAMETROS EN FDISK TIENE VALORES INCORRECTOS, VERIFIQUE" << endl;
    }
}


/* -- -- -- --  --  -- -- -- -- -- -- --
   -- -- -- --       REP     -- -- -- --
   -- -- -- --  --  -- -- -- -- -- -- -- */
void ejecutarREP(Comando *params)
{
    QString dir;
    Comando path = params->parametros.at(0);
    dir =  path.valor;//ruta para almacenar reporte
    //crear dir
    string comando1="sudo mkdir -p \'"+dir.toStdString()+"\'";
    string comando2="sudo chmod -R 777 \'" +dir.toStdString()+"\'";
    system(comando1.c_str());
    system(comando2.c_str());
    QString disco1="/home/mia/Disco1.dk";
    QString disco2="/home/mia/primer semestre/Disco2.dk";
    QString disco3="/home/mia/primer semestre/entrada1/Disco3.dk";
    reporteDisco(disco1,"disco1");
    reporteDisco(disco2,"disco2");
    reporteDisco(disco3,"disco3");
}


void crearParticion(QString ruta,QString nombre,int tamano, char unidad)
{
    int tambyte    = 1024;
    int recuento   = 0;
    int disponible = 0;
    char copyunidad;
    string copyruta=ruta.toStdString();
    MBR mbr;
    char temporal = '1';

    if(unidad != 0)
    {
        copyunidad = unidad;
        if(copyunidad == 'B') tambyte = tamano;
        else if(copyunidad == 'K') tambyte = tamano * 1024;
        else tambyte = tamano*1024*1024;
    }
    else
    {
        tambyte = tamano*1024; // kb se establece default
    }

    bool existeParticion = false;
    int numParticion = 0;
    FILE *archivo;

    if((archivo = fopen(copyruta.c_str(),"rb+")))//se abre disco, si existe
    {

        fseek(archivo,0,SEEK_SET); //posicion cursor en estructura mbr
        fread(&mbr,sizeof(MBR),1,archivo);//obtengo struct mbr
        //deben de haber como máximo 4 particiones primarias, se verifica que
        //aun se pueda particionar:

        for(int i=0; i < 4; i++)
        {
            if(mbr.mbrPartition[i].part_start == -1 )
            {
                existeParticion = true;
                numParticion = i;
                break; //se sale del ciclo ya que se sabe que si se puede particionar aún
            }
        }


        recuento = 0;
        for(int i = 0; i < 4; i++)
        {
            if(mbr.mbrPartition[i].part_start != -1)
            {
                recuento += mbr.mbrPartition[i].part_size;//esto para saber cuanto espacio disponible hay
            }
        }

        disponible = mbr.mbr_tamano - recuento;
        //si existe espacio para particion:
        //se valida que el espacio sea el necesario
        if (disponible >= tambyte)
        {
            if(existeParticion)
            {
                if(!particionRepetida(nombre,ruta))
                {
                    //posicion de inicio
                    if (numParticion == 0) mbr.mbrPartition[numParticion].part_start=sizeof(mbr);
                    //de lo contrario tiene que ir a -1 para obtener la posicion de inicio de memoria y sumarle el tamaño del mismo para no escribir encima
                    else mbr.mbrPartition[numParticion].part_start = mbr.mbrPartition[numParticion-1].part_start + mbr.mbrPartition[numParticion-1].part_size;

                    mbr.mbrPartition[numParticion].part_size=tambyte;

                    //nombre
                    strcpy(mbr.mbrPartition[numParticion].part_name,nombre.toStdString().c_str());
                    //se actualiza el mbr
                    fseek(archivo,0,SEEK_SET);
                    //se escribe el mbr
                    fwrite(&mbr,sizeof(MBR),1,archivo);

                    fseek(archivo,mbr.mbrPartition[numParticion].part_start,SEEK_SET);
                    for(int i = 0; i < tambyte; i++)
                    {
                        fwrite(&temporal,1,1,archivo);
                    }
                    cout << "INFORMACION, PARTICION FUE CREADA EXITOSAMENTE" << endl;
                }else cout << "ERROR, NOMBRE PARA PARTICION YA EXISTE. VERIFIQUE" << endl;//se actualiza el mbr
            }else cout << "ERROR, DISCO YA LLEGO AL LIMITE DE PARTICIONES" << endl;
        }else cout << "INFORMACION, ESPACIO REQUERIDO PARA PARTICION ES MAYOR AL ESPACIO DISPONIBLE" << endl;
        fclose(archivo);
    }else cout << "INFORMACION, RUTA O DISCO NO EXISTE, VERIFIQUE" << endl;
}



//valida si particion a crear ya existe o es extendida y ya hay una
bool particionRepetida(QString nombre, QString ruta)
{
    FILE *archivo;
    MBR mbr2;
    if ((archivo = fopen(ruta.toStdString().c_str(),"rb+")))
    {
        fseek(archivo,0,SEEK_SET);
        fread(&mbr2,sizeof (MBR),1,archivo);
        for(int i = 0; i < 4; i++)
        {
            if(strcmp(mbr2.mbrPartition[i].part_name,nombre.toStdString().c_str()) == 0)
            {
                fclose(archivo);
                return true;
            }
        }
        fclose(archivo);

    }
    return false;
}

//graficar disco
void reporteDisco(QString rutadisco,QString destinoreporte)
{
    FILE *archivo;
    FILE *discoimagen;

    string rutastr = rutadisco.toStdString();

    if ((archivo = fopen(rutastr.c_str(),"r")))
    {
        discoimagen = fopen("disc.txt","w");
        fprintf(discoimagen,"digraph G{\n\n");
        fprintf(discoimagen, "  tbl [\n    shape=box\n    label=<\n");
        fprintf(discoimagen, "     <table border=\'0\' cellborder=\'2\' width=\'600\' height=\"200\" color=\'LIGHTSTEELBLUE\'>\n");
        fprintf(discoimagen, "     <tr>\n");
        fprintf(discoimagen, "     <td height=\'200\' width=\'100\'> MBR </td>\n");
        MBR mbr;
        fseek(archivo,0,SEEK_SET);
        fread(&mbr,sizeof(MBR),1,archivo);
        int tam_total= mbr.mbr_tamano;
        double tam_usado = 0;

        //recorrer particiones
        for(int i = 0; i < 4; i++)
        {
            int tampar=mbr.mbrPartition[i].part_size;
            if(mbr.mbrPartition[i].part_start != -1)
            {
                fprintf(discoimagen, "     <td height=\'200\' width=\'200'> %s <br/> TAMAÑO: %d </td>\n",mbr.mbrPartition[i].part_name,tampar);
            }

        }

        fprintf(discoimagen,"     </tr> \n     </table>        \n>];\n\n}");
        fclose(discoimagen);
        fclose(archivo);
        string comando = "dot -Tpng disc.txt -o "+destinoreporte.toStdString();
        system(comando.c_str());

        cout << "Reporte generado con exito " << endl;

    }
}

