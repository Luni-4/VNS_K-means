#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "data.h"
#include "common.h"


// Carica dal file i dati associati all'istanza
void load_data(char *data_file, PointArray *pI)
{
        FILE *fp;
        char temp[MAX_STRING];

        // Copia il filename in una stringa temporanea
        strcpy(temp, data_file);        
        
        // Apertura del file
        fp = fopen(temp, "r");
        check_pointer(fp, strcat(temp, "can't be opened!"));       
        
        // Identifica il numero di dati associati all'istanza
        fscanf(fp,"%d",&pI->np);
        
        // Viene allocato l'array utilizzato per memorizzare i dati associati all'istanza
        pI->points = malloc(pI->np * sizeof(*pI->points));
        check_pointer(pI->points, "Points can't be allocated!"); 
        
        // Variabili contenenti i dati letti all'iterazione corrente
        double xx, yy;        

        for(int i = 0; i < pI->np; i++)
        {
                // Acquisizione dei dati
                fscanf(fp,"%*s %lf %lf",&xx,&yy);
                // Memorizzazione dei dati              
                pI->points[i].x = xx;
                pI->points[i].y = yy;
        }
        
        // Chiusura del file        
        fclose(fp);                                
  
}

// Dealloca i dati caricati
void destroy_data(PointArray *pI)
{
        free(pI->points);
}


// Stampa i dati associati all'istanza
void print_data(PointArray *pI)
{
        // Stampa il numero dei dati 
        printf("Numero punti istanza: %d\n\n",pI->np);
        
        // Stampa i dati
        for (int i = 0; i < pI->np; i++)     
                printf("[%f,%f]\n",pI->points[i].x, pI->points[i].y);
        
        printf("\n\n");
}
