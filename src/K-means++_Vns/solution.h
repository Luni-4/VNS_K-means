#ifndef __SOLUTION_H
#define __SOLUTION_H

#include "point.h"
#include "data.h"

// Struttura usata per definire un cluster
typedef struct{
        Point centroid; 
        int ncl; 
        Point *cpoints; 
}Cluster;

// Struttura che definisce un array di cluster
typedef struct{
        int nc; 
        Cluster *carray;
}ClusterArray;


// Crea una soluzione
void create_solution(int clusters, ClusterArray *px);

// Dealloca i punti associati ad una soluzione
void deallocate_points(ClusterArray *px);

// Dealloca una soluzione
void destroy_solution(ClusterArray *px);

// Alloca lo spazio per i punti di ciascun cluster
void allocate_points(ClusterArray *px);

// Sostituisce il contenuto della prima soluzione con quello della seconda
void copy_solution(ClusterArray *px, ClusterArray *c);

// Sostituisce i centroidi della prima soluzione con quelli della seconda
void copy_centroid(ClusterArray *px, ClusterArray *c);

// Assegna i punti ai cluster di una soluzione
void assign_points(ClusterArray *px, PointArray *pI, int *m);

// Stampa una soluzione
void print_solution(ClusterArray *px);

// Stampa i centroidi di una soluzione
void print_centroid(ClusterArray *px);

// Stampa l'output come un file EPS
void print_eps(ClusterArray *px, PointArray *pI, char *data_file);

// Debug di una soluzione
void debug_solution(double x, double y, ClusterArray *px, double *mp, double min, int oldm, int newm);

#endif /* __SOLUTION_H */
