#ifndef COMANDO_H
#define COMANDO_H

#include <qstring.h>
#include <QList>

class Comando
{
public:
    Comando(QString comando, QString valor);
    QString comando; //comando leido
    QString valor; // valor en caso tenga hijos;
    int tipo; //tipo no
    int getTipo(); //obtener valor tipo

    void agregarParametros(Comando); //agrega hijos a raiz

    QList <Comando> parametros; //lista con parametros para Comando
};

#endif

