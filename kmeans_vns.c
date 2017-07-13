#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "data.h"
#include "solution.h"
#include "km.h"

void print_usage(char *command)
{
        fprintf(stderr,"Use: %s [datafile] [ncluster] [maxit] [kmax] [time] [seed]\n",command);
        fprintf(stderr,"datafile: name and path of data file\n");
        fprintf(stderr,"ncluster: number of cluster\n");
        fprintf(stderr,"maxit: max number of iterations\n");
        fprintf(stderr,"kmax: max neighborhood dimension\n");
        fprintf(stderr,"time: VNS time\n");
        fprintf(stderr,"seed: Random seed\n");
}


void parse_command_line(int argc, char **argv, char *data_file, int *clusters, int *maxit, int *kmax, int *t, int *seed)
{
        // Se il numero di parametri inseriti è minore di 5 mostra funzionamento
        if (argc < 7)
        {
                print_usage(argv[0]);
                exit(EXIT_FAILURE);
        }
        
        // Copia il path + filename in data_file
        strcpy(data_file, argv[1]);

        // Il numero di clusters deve essere un intero        
        if (!sscanf(argv[2],"%d", clusters))
        {
                fprintf(stderr,"\nNumber of clusters must be a number\n");
                print_usage(argv[0]);
                exit(EXIT_FAILURE);
        }
        
        // Se il numero di cluster è minore di 1 restituisce un errore
        if (*clusters < 2)
        {
                fprintf(stderr,"\nNumber of clusters must be greater than 1\n");
                exit(EXIT_FAILURE);
        }
        
        // Il numero massimo di iterazioni del k-means deve essere un intero        
        if (!sscanf(argv[3],"%d", maxit))
        {
                fprintf(stderr,"\nMax number of iterations must be a number\n");
                print_usage(argv[0]);
                exit(EXIT_FAILURE);
        }
        
        // Se il numero massimo di iterazioni è minore di 1 restituisce un errore
        if (*maxit < 1)
        {
                fprintf(stderr,"\nMax number of iterations must be greater than 0\n");
                exit(EXIT_FAILURE);
        }

        // La massima dimensione dell'intorno deve essere un intero       
        if (!sscanf(argv[4],"%d", kmax))
        {
                fprintf(stderr,"\nMax neighborhood dimension must be a number\n");
                print_usage(argv[0]);
                exit(EXIT_FAILURE);
        }
        
        // Se la massima dimensione dell'intorno è minore di 1 restituisce un errore
        if (*kmax < 1)
        {
                fprintf(stderr,"\nMax neighborhood dimension must be greater than 0\n");
                exit(EXIT_FAILURE);
        }

        // Se la massima dimensione dell'intorno è maggiore del numero di clusters restituisce un errore
        if (*kmax > *clusters)
        {
                fprintf(stderr,"\nMax neighborhood dimension can't be greater than number of clusters\n");
                exit(EXIT_FAILURE);
        }

        // Il tempo di esecuzione deve essere un intero        
        if (!sscanf(argv[5],"%d", t))
        {
                fprintf(stderr,"\nVNS time must be a number\n");
                print_usage(argv[0]);
                exit(EXIT_FAILURE);
        }

        // Se il tempo di esecuzione è minore di 1 restituisce un errore
        if (*t < 1)
        {
                fprintf(stderr,"\nVNS time must be greater than 0\n");
                exit(EXIT_FAILURE);
        }

        // Il seme casuale deve essere un intero      
        if (!sscanf(argv[6],"%d", seed))
        {
                fprintf(stderr,"\nRandom seed must be a number\n");
                print_usage(argv[0]);
                exit(EXIT_FAILURE);
        } 
}



// Dall'intorno di dimensione k vengono estratti casualmente dei punti che sostituiranno alcuni dei
// centroidi della soluzione di partenza
void shaking(ClusterArray *xo, PointArray *pI, ClusterArray *x, int k, int *rd, int *rc)
{       
        // Copia i centroidi di px in px1
        copy_centroid(x, xo);           

        int j = 0;
        int r1, r2;
        int c1, c2;

        // Viene inizializzato l'array casuale per i dati dell'istanza
        for(int i = 0; i < pI->np; i++)
                rd[i] = i;
        
        // Viene inizializzato l'array casuale per i cluster
        for(int i = 0; i < xo->nc; i++)
                rc[i] = i; 

        while (j < k)
        {
                c1 = pI->np - j;
                c2 = xo->nc - j; 
                
                r1 = rand() % c1;
                r2 = rand() % c2;
                
                // Assegna dato a centroide 
                x->carray[rc[r2]].centroid = pI->points[rd[r1]];                

                // Viene eseguito lo scambio per i dati
                rd[r1] = rd[c1 - 1];               

                // Viene eseguito lo scambio per i centroidi
                rc[r2] = rc[c2 - 1];                         

                j++;                        
                                
        }            
}

// Se la soluzione trovata è migliore di quella ottima, sostituisce soluzione e riporta l'intorno a 1,
// altrimenti aumenta la grandezza dell'intorno
void neighborhoodChange(ClusterArray *xo, ClusterArray *x, int *k, double fx, double *fo)
{
        if (fx < *fo)
        {
                *fo = fx;
                copy_solution(xo, x);
                *k = 1;
        }
        else
        {
                (*k)++;
        }
        
        // Dealloca i punti associati alla soluzione intermedia
        deallocate_points(x);
}

int main(int argc, char **argv)
{
        // Stringa contenente il filename dell'istanza
        char data_file[MAX_STRING];
        
        // Array contenente i dati associati all'istanza
        PointArray I;

        // Soluzione ottima x*
        ClusterArray xo;

        // Soluzione parziale x'
        ClusterArray x;
        
        // Parametri usati dall'algoritmo        
        int clusters, maxit, kmax, t, seed;
        int k;
                
        // Variabili usate per l'analisi delle tempistiche
        time_t inizio, fine;
        double tempo;

        // f*
        double fo;

        // f'
        double fx;
       
        // Analizza i parametri dell'algoritmo inseriti dall'utente
        parse_command_line(argc, argv, data_file, &clusters, &maxit, &kmax, &t, &seed);
        
        // Generatore di numeri casuali con seme che varia nel tempo
        srand((unsigned int) time(NULL));

        if (seed > 0)        
                // Generatore di numeri casuali con seme fissato
                srand((unsigned int) seed);     
      

        // Legge i dati associati all'istanza
        load_data(data_file, &I);

        // Controlla che il numero di cluster scelti dall'utente siano inferiori o uguali al numero di dati
        if(clusters > I.np - 1)
        {
            fprintf(stderr,"\nThe number of clusters must be less than number of data\n");            
            exit(EXIT_FAILURE);
        }

        // Variabili ed array intermedi
        int *rd = malloc(I.np * sizeof(*rd));
        check_pointer(rd, "Malloc has failed on the first array of shaking!");

        int *rc = malloc(clusters * sizeof(*rc));
        check_pointer(rc, "Malloc has failed on the second array of shaking!");        
        
        // Stampa i dati acquisiti
        //print_data(&I);

        // Tempo di inizio dell'algoritmo
        inizio = clock();

        // Creazione della soluzione ottima
        create_solution(clusters, &xo);        

        // Viene creata la soluzione iniziale x(0)
        firstCentroid(&xo, &I);              

        // Viene eseguito il k-means, che trova la funzione parziale x'
        km(&xo, &I, clusters, maxit, &fx);      

        // Stampa della prima f*
        //printf("Funzione obiettivo iniziale: %f\n", fx);                      

        // Creazione della soluzione restituita dallo shaking
        create_solution(clusters, &x);
        
        // La dimensione massima dell'intorno viene incrementata siccome si parte da 1
        kmax = kmax + 1;

        // All'inizio la funzione obiettivo ottima è uguale alla funzione obiettivo parziale
        fo = fx;      
                
        // La VNS viene eseguita fino a quando si eguaglia o si supera il tempo di termine
        while (clock() / CLOCKS_PER_SEC < t)
        {
                // La dimensione iniziale dell'intorno viene impostata a 1
                k = 1;
                
                // Ciclare fino a quando k è maggiore o uguale a k-max
                // oppure non si eguaglia o si supera il tempo di termine
                while (clock() / CLOCKS_PER_SEC < t && k < kmax)
                {                                               

                         // Viene eseguita la funzione di shaking
                         shaking(&xo, &I, &x, k, rd, rc);                                               
                  
                         // Viene eseguito il k-means
                         km(&x, &I, clusters, maxit, &fx);                                         
                         
                         // Viene eseguita la funzione relativa alla variazione dell'intorno
                         neighborhoodChange(&xo, &x, &k, fx, &fo);
                }   
        }
        
        // Vengono deallocati gli array
        free(rd);
        free(rc);                                  
        
        // La soluzione parziale x' viene distrutta       
        destroy_solution(&x);

        // Stampa la soluzione in un file EPS
        //print_eps(&xo, &I, data_file); 

        // Stampa della soluzione ottimale
        //print_solution(&xo);

        // Stampa della f*
        printf("%f ", fo);  

        // La soluzione ottimale x* viene distrutta
        destroy_solution(&xo);           
        
        // Tempo di fine dell'algoritmo        
        fine = clock();
        
        // Calcolo della differenza tra il tempo di fine e quello di inizio
        tempo = (double) (fine - inizio) / CLOCKS_PER_SEC;

        // Stampa della durata temporale dell'algoritmo        
        //printf("%f\n", tempo);    

        // Vengono distrutti i dati associati all'istanza
        destroy_data(&I);

        return EXIT_SUCCESS;  
}
