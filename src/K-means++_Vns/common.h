#ifndef __COMMON_H
#define __COMMON_H

#define MAX_STRING 256

// Verifica che l'allocazione di un array o l'apertura di un file sia avvenuta correttamente
void check_pointer(void *p, char *err);

// Debug
void debug(double px, double py, int m, double *mp, double min, int n1); 

// Somma un array di double
double sum_vectord(double *v, int l);

// Stampa un array di double
void print_vectord(double *v, int l);

// Stampa un array di interi
void print_vectori(int *v, int l);

// Verifica che il contenuto di due array, di pari lunghezza, sia lo stesso
int change(int *v, int *v1, int n);

#endif /* __COMMON_H */
