#ifndef ESTRUCTURAS_H
#define ESTRUCTURAS_H
#include "time.h"
// -- -- -- -- -- -- STRUCTS PARA SISTEMA DE ARCHIVOS -- -- -- -- --
typedef struct
{
    int part_start;      //Indica en qué byte del disco inicia la partición
    int part_size;       //Contiene el tamaño total de la partición en bytes
    char part_name[16];  //Nombre de la partición
}PARTITION;

typedef struct
{
    int mbr_tamano;             //Tamaño total del disco en bytes
    time_t mbr_fecha_creacion;  //Fecha y hora de creación del disco
    int mbr_dsk_signature;      //Número random, que identifica de forma única a cada disco
    PARTITION mbrPartition[4];  // estructura con informacion de particiones (4 PARTICIONES)
}MBR;


class Estructuras
{
public:
    Estructuras();
};

#endif // ESTRUCTURAS_H
