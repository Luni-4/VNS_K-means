#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Verifica che l'allocazione di un array o l'apertura di un file sia avvenuta correttamente
void check_pointer(void *p, char *err)
{
        if (p == NULL)
        {
              fprintf(stderr,"%s\n", err);
              exit(EXIT_FAILURE);
        }       
}

void debug(double px, double py, int m, double *mp, double min, int n1)
{
        
        printf("\n\n");

        // Stampa punto
        printf("Punto: %f %f\n", px, py);

        printf("\n\n");

        // Stampa membership
        printf("Membership: %d", m);
        
        printf("\n\n");

        // Stampa mp
        printf("Mp:");
        for(int i =0; i < n1; i++)
                printf(" (%d, %f)", i, mp[i]);
        printf("\n\n"); 

        printf("Minima: %f\n", min);

        printf("Differenza: %f\n", (mp[m] - min));        

}

// Somma un array di double
double sum_vectord(double *v, int l)
{
        double sum = 0;

        for (int i = 0; i < l; i++)
                sum += v[i];

        return sum;
}

// Stampa un array di double
void print_vectord(double *v, int l)
{
        printf("\n\n");

        for (int i = 0; i < l; i++)
               printf("%f\n",v[i]);
        
        printf("\n");
}

// Stampa un array di interi
void print_vectori(int *v, int l)
{
        printf("\n\n");

        for (int i = 0; i < l; i++)
               printf("%d %d\n",i,v[i]);
        
        printf("\n");
}

// Verifica che il contenuto di due array, di pari lunghezza, sia lo stesso
int change(int *v, int *v1, int n)
{       
     return memcmp(v, v1, n * sizeof(*v));
}
