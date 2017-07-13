#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>

#include "km.h"

// Crea la soluzione iniziale x(0)
void firstCentroid(ClusterArray *px, PointArray *pI)
{
        // Variabili locali       
        int i = 0;
        int cas, d;

        // Array usato per evitare la ripetizione degli indici estratti
        int *ind = malloc(pI->np * sizeof(*ind));
        check_pointer(ind, "Malloc has failed on ind array!");

        // Ogni cella dell'array contiene gli indici associati ai dati dell'istanza
        for(int j = 0; j < pI->np; j++)
                ind[j] = j;
        
        // Variabile usata per decrementare ogni volta il numero di indici estraibili
        // dalla funzione rand
        d = pI->np;
        
        // Ciclo che estrae la partizione casuale iniziale
        while(i < px->nc)
        {
                // Viene estratto un indice casuale dall'array di dati associato all'istanza
                cas = rand() % d;                               
                
                // Assegnamento di un dato dell'istanza ad un centroide
                px->carray[i].centroid = pI->points[ind[cas]];
                
                // Viene aggiornato l'array di indici
                ind[cas] = ind[d - 1];                   
                
                d--;
                               
                i++;                
        }
        
        // Viene deallocato l'array degli indici
        free(ind);
}

// Viene individuato il centroide che ha distanza minima dal punto considerato
int fmember(Point p, ClusterArray *px, double *m)
{
        // Calcolo della distanza euclidea tra il punto ed il primo centroide       
        m[0] = euclidean(&p, &px->carray[0].centroid);       

        // Variabili locali usate per individuare il minimo
        double min = m[0];
        int cmin = 0;       

        // Vengono calcolate le distanze euclidee tra il punto ed i restanti centroidi,
        // infine viene individuata la minima distanza
        for(int j = 1; j < px->nc; j++)
        {       
                m[j] = euclidean(&p, &px->carray[j].centroid);
                if (m[j] < min)
                {
                        min = m[j];
                        cmin = j;
                }             
        }        
        
        // Viene ritornato l'indice associato alla distanza minima
        return cmin;
}

// Calcola la prima partizione dei punti
void firstPartition(ClusterArray *px, PointArray *pI, int *membership, double *mp, double *dist, double *f)
{       
        // Assegna i punti ai centroidi
        for(int i = 0; i < pI->np; i++)
        {
                membership[i] = fmember(pI->points[i], px, mp);                     
                dist[i] = mp[membership[i]];                                  
        }      

        // Viene calcolata la funzione obiettivo
        *f = sum_vectord(dist, pI->np);         
}

// Calcola i nuovi valori dei centroidi facendo la media dei punti associati ad essi
void medium(ClusterArray *px, PointArray *pI, int *m)
{
        // Ogni cella dell'array contiene la somma dei punti di un dato cluster
        Point *s = calloc(px->nc, sizeof(*s));       
 
        for(int i = 0; i < pI->np; i++)
        {
                // Somma tra loro i punti di un dato cluster                             
                add(&(s[m[i]]), &(pI->points[i]));        
                
                // Rileva quanti punti sono associati ad un dato cluster
                px->carray[m[i]].ncl++;
        }
              
        // Calcola la media per ciascun cluster e l'assegna al centroide corrispondente
        for(int i = 0; i < px->nc; i++)
        {    
                // La somma dei punti di ciascun cluster viene divisa per la sua cardinalità
                divide(&s[i], px->carray[i].ncl);

                // Assegna il valore ottenuto al centroide
                px->carray[i].centroid = s[i];
        }              
 
        // Dealloca l'array contenente la somma dei punti di ciascun cluster 
        free(s);
}

// Aggiorna il valore dei centroidi
void mediumUpdate(Point p, ClusterArray *px, int m, int mnew)
{
        /* Vecchio centroide aggiornato */

        // Moltiplica il centroide per il numero di punti nel cluster
        mul(&px->carray[m].centroid, px->carray[m].ncl);

        // Sottrae al centroide il valore del punto
        Point pn = sub(&px->carray[m].centroid, &p);

        // Divide per numero di punti del cluster meno 1
        divide(&pn, (px->carray[m].ncl - 1));

        // Assegna il valore al centroide
        px->carray[m].centroid = pn;


        /* Nuovo centroide aggiornato */

        // Moltiplica centroide per numero di punti nel cluster
        mul(&px->carray[mnew].centroid, px->carray[mnew].ncl);

        // Aggiunge al centroide il valore del punto
        add(&px->carray[mnew].centroid, &p);

        // Divide per numero di punti del cluster meno 1
        divide(&px->carray[mnew].centroid, (px->carray[mnew].ncl + 1));
}

// Aggiorna il valore della funzione obiettivo
void objUpdate(double dold, double d, double *f)
{
        *f = (*f - dold) + d;        
}

// Calcola le successive partizioni dei punti
void partition(ClusterArray *px, ClusterArray *c, PointArray *pI, int *m, double *mp, double *dist, double *f)
{
        int mpos;
        double dold;                           

        // Assegna ciascun punto al cluster corrispondente
        for(int i = 0; i < pI->np; i++)
        {
                // Viene trovata la distanza del centroide più vicino al punto considerato
                mpos = fmember(pI->points[i], px, mp); 

                //debug_solution(pI->points[i].x, pI->points[i].y, px, mp, mp[mpos], m[i], mpos, dist[i]);
               
                // Distanza del punto dal vecchio centroide
                dold = dist[i];

                // Viene salvata la distanza minima del punto dal nuovo cluster
                dist[i] = mp[mpos];

                // Aggiorna la funzione obiettivo
                objUpdate(dold, dist[i], f);
                    
                // Aggiorna la media
                mediumUpdate(pI->points[i], c, m[i], mpos);                    
                    
                // Vengono aggiornati i numeri dei punti del vecchio e del nuovo cluster
                c->carray[m[i]].ncl--;
                c->carray[mpos].ncl++;

                px->carray[m[i]].ncl--;
                px->carray[mpos].ncl++;                    

                // Aggiornamento dell'array di membership
                m[i] = mpos;            
        }
}

// Esegue il k-means
void km(ClusterArray *px, PointArray *pI, int clusters, int maxit, double *f)
{
        /* Creazione e allocazione delle soluzioni*/       

        // Soluzione copia
        ClusterArray copy;       
        
        // Creazione della soluzione copia
        create_solution(clusters, &copy);        
        
        /* Variabili e array */     
        
        // Array usato per associare a ciascun punto il cluster corrispondente      
        int *membership = malloc(pI->np * sizeof(*membership));
        check_pointer(membership, "Malloc has failed to allocate membership array");         
        
        // Array copia della membership
        int *membership1 = malloc(pI->np * sizeof(*membership1));
        check_pointer(membership1, "Malloc has failed to allocate membership-like array");

        // Array usato per calcolare le distanze di ciascun punto dai centroidi
        double *mp = malloc(px->nc * sizeof(*mp));
        check_pointer(mp, "Malloc has failed to allocate point-to-centroid array"); 

        // Array usato per salvare le distanze di ciascun punto dal proprio centroide
        double *dist = malloc(pI->np * sizeof(*dist));
        check_pointer(dist, "Malloc has failed to allocate distance array!");    

        
        /* K-means */             

        // Calcola la prima partizione dei punti
        firstPartition(px, pI, membership, mp, dist, f);  

        // Calcola la media
        medium(px, pI, membership);

        // Copiare array di membership nell'array di copia
        memcpy(membership1, membership, pI->np * sizeof(*membership));

        // Copia la soluzione con i nuovi centroidi aggiornati in una soluzione temporanea
        memmove(copy.carray, px->carray, px->nc * sizeof(*px->carray));

        // Viene calcolata la nuova partizione
        partition(px, &copy, pI, membership, mp, dist, f);        

        // Iterazioni eseguite dal k-means
        int l = 1;        
        
        // Il ciclo termina o quando l'array di membership dell'iterazione precedente è uguale a
        // quello dell'iterazione corrente oppure quando si raggiunge il numero massimo di 
        // iterazioni consentite
        while(change(membership, membership1, pI->np) && l < maxit)
        {
                // Copiare array di membership nell'array di copia
                memmove(membership1, membership, pI->np * sizeof(*membership));
           
                // Copia la soluzione con i nuovi centroidi aggiornati nella soluzione corrente
                memmove(px->carray, copy.carray, px->nc * sizeof(*px->carray));

                // Viene calcolata la nuova partizione
                partition(px, &copy, pI, membership, mp, dist, f);                                          
        
                // Incrementa l'iterazione
                l++;                
        }         
        
        /* Deallocazione ed assegnamento dei punti a ciascun cluster */

        // Vengono deallocati gli array
        free(mp);
        free(dist);
        free(membership1);
        
        // Dealloca la soluzione copia
        destroy_solution(&copy);               
        
        // Alloca lo spazio per i punti di ciascun cluster
        allocate_points(px);
        
        // Assegna i punti ai cluster di una soluzione
        assign_points(px, pI, membership);        
       
        // Dealloca l'array di membership
        free(membership);            
        
}
