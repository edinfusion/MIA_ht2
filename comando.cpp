#include "comando.h"
#include <iostream>
using namespace std;

//constructor
Comando::Comando(QString comandoX, QString valorX)
{
    comando = comandoX; //se obtiene comando o parametro del Comando
    valor = valorX; //si es parametro se obtiene el valor, caso contrario -> ""
    tipo = getTipo(); //se le asigna un valor numerico
    parametros = QList<Comando>(); //lista de parametros para Comando
}

int Comando::getTipo()
{
    if(this->comando=="MKDISK")      return  1; //COMANDO
    if(this->comando=="REP")         return  2; //COMANDO
    if(this->comando=="EXEC")        return  3; //COMANDO
    if(this->comando=="RMDISK")      return  4; //COMANDO
    if(this->comando=="FDISK")       return  5; //COMANDO
    if(this->comando=="MOUNT")       return  6; //COMANDO
    if(this->comando=="RUTA")        return 28; //PARAMETRO
    if(this->comando=="PARAMETROS")  return 29; //PARAMETRO
    if(this->comando=="SIZE")        return 30; //PARAMETRO
    if(this->comando=="UNIT")        return 31; //PARAMETRO
    if(this->comando=="PATH")        return 32; //PARAMETRO
    if(this->comando=="FIT")         return 33; //PARAMETRO
    if(this->comando=="TYPE")        return 34; //PARAMETRO
    if(this->comando=="DELETE")      return 35; //PARAMETRO
    if(this->comando=="NAME")        return 36; //PARAMETRO
    if(this->comando=="ADD")         return 37; //PARAMETRO
    if(this->comando=="ID")          return 38; //PARAMETRO

    return 51; //COMANDO INCORRECTO **DEFAULT
}

void Comando::agregarParametros(Comando Comando)
{
    parametros.append(Comando);
}
