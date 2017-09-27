#ifndef __DATA_H
#define __DATA_H

#include "point.h"

// Struttura che definisce un array di punti
typedef struct{
        int np;
        Point *points;
}PointArray;

// Carica dal file i dati associati all'istanza
void load_data(char *data_file, PointArray *pI);

// Dealloca i dati caricati
void destroy_data(PointArray *pI);

// Stampa i dati associati all'istanza
void print_data(PointArray *pI);


#endif /* __DATA_H */
