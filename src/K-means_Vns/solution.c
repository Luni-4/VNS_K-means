#include <stdio.h>
#include <stdlib.h>

#include <string.h>

#include "solution.h"
#include "common.h"

// Crea una soluzione
void create_solution(int clusters, ClusterArray *px)
{
        px->nc = clusters;
        px->carray = calloc(clusters, sizeof(*px->carray));
        check_pointer(px->carray, "Calloc has failed to allocate clusters array");          
}

// Dealloca i punti associati ad una soluzione
void deallocate_points(ClusterArray *px)
{
        // Dealloca i punti della soluzione
        if (px->carray[0].cpoints != NULL)
        {                
                for(int i = 0; i < px->nc; i++)
                {
                        free(px->carray[i].cpoints);
                        px->carray[i].cpoints = NULL;
                }
        }
}


// Dealloca una soluzione
void destroy_solution(ClusterArray *px)
{
        // Se ci sono dei punti assegnati alla soluzione, deallocarli
        deallocate_points(px);
        
        // Dealloca l'array di cluster
        free(px->carray);
}

// Alloca lo spazio per i punti di ciascun cluster
void allocate_points(ClusterArray *px)
{
        for(int i = 0; i < px->nc; i++)
        {         
                px->carray[i].cpoints = calloc(px->carray[i].ncl, sizeof(*px->carray[i].cpoints));
                check_pointer(px->carray[i].cpoints, "Calloc has failed to allocate clusters-point array");
        }                 
}

// Copia i punti di una soluzione in un'altra soluzione
void copy_cluster_point(ClusterArray *px, ClusterArray *c)
{
        for(int i = 0; i < px->nc; i++)                 
               memmove(px->carray[i].cpoints, c->carray[i].cpoints, (c->carray[i].ncl * sizeof(*c->carray[i].cpoints)));       
}


// Sostituisce il contenuto della prima soluzione con quello della seconda
void copy_solution(ClusterArray *px, ClusterArray *c)
{
        // Dealloca i punti assegnati alla prima soluzione
        deallocate_points(px);

        // Dalla seconda soluzione vengono copiati i centroidi ed il numero di elementi del cluster
        for(int i = 0; i < px->nc; i++)
        {         
                px->carray[i].centroid = c->carray[i].centroid;
                px->carray[i].ncl = c->carray[i].ncl;
        }       
        
        // Alloca alla prima soluzione lo spazio per i punti     
        allocate_points(px);

        // I punti associati ai cluster della seconda soluzione vengono copiati nella prima 
        copy_cluster_point(px, c);      
                            
}

// Sostituisce i centroidi della prima soluzione con quelli della seconda
void copy_centroid(ClusterArray *px, ClusterArray *c)
{
        // Copia i centroidi ed il numero di elementi del cluster
        for(int i = 0; i < px->nc; i++)
        {         
                px->carray[i].centroid = c->carray[i].centroid;
                px->carray[i].ncl = 0;                
        }
}

// Assegna i punti ai cluster di una soluzione
void assign_points(ClusterArray *px, PointArray *pI, int *m)
{
        int *pc = calloc(px->nc, sizeof(*pc));   
  
        // A ciascun cluster vengono associati i punti a seconda del valore contenuto nell'array di membership 
        for (int i = 0; i < pI->np; i++)
        {                
                px->carray[m[i]].cpoints[pc[m[i]]] = pI->points[i];
                pc[m[i]]++;
        }

        free(pc);
}

// Stampa i punti associati a ciascun cluster
void print_cluster_point(ClusterArray *px, int c)
{     
        printf("Punti\n");
        for(int i = 0; i < px->carray[c].ncl; i++)
                printf("%d [%f, %f]\n",i, px->carray[c].cpoints[i].x, px->carray[c].cpoints[i].y);
        printf("\n\n");
        
}       

// Stampa una soluzione
void print_solution(ClusterArray *px)
{
        for(int i = 0; i < px->nc; i++)
        {
                printf("\nStampa del cluster numero: %d\n\n",i);
                printf("Centroide: [%f, %f]\n",px->carray[i].centroid.x,px->carray[i].centroid.y);
                printf("Numero di punti nel centroide: %d\n", px->carray[i].ncl);
                if (px->carray[i].cpoints != NULL)
                {      
                        print_cluster_point(px, i); 
                }        
        }       
}

// Stampa i centroidi di una soluzione
void print_centroid(ClusterArray *px)
{
        for(int i = 0; i < px->nc; i++)
        {
                printf("\nStampa del cluster numero: %d\n\n",i);
                printf("Centroide: [%f, %f]\n",px->carray[i].centroid.x, px->carray[i].centroid.y);
                printf("Numero di punti nel centroide: %d\n", px->carray[i].ncl);
        }
}

// Stampa l'output come un file EPS
void print_eps(ClusterArray *px, PointArray *pI, char *data_file)
{
        static int const W = 400;
        static int const H = 400;

        char temp[MAX_STRING];
        char c[MAX_STRING];
        char *t;

        double min_x, max_x, min_y, max_y, scale, cx, cy;
        double *colors = malloc(px->nc * 3 * sizeof(*colors));

        FILE *fp;        
 
	for(int i = 0; i < px->nc; i++)
        {
		colors[3*i + 0] = (3 * (i + 1) % 11)/11.;
		colors[3*i + 1] = (7 * i % 11)/11.;
		colors[3*i + 2] = (9 * i % 11)/11.;
	}

        max_x = min_x =  pI->points[0].x;
        max_y = min_y =  pI->points[0].y;
        
	for(int i = 1; i < pI->np; i++)
        {
		if (max_x < pI->points[i].x) max_x = pI->points[i].x;
                if (max_y < pI->points[i].y) max_y = pI->points[i].y;

		if (min_x > pI->points[i].x) min_x = pI->points[i].x;		
		if (min_y > pI->points[i].y) min_y = pI->points[i].y;
	}


        scale = W / (max_x - min_x);
	if (scale > H / (max_y - min_y)) scale = H / (max_y - min_y);
        cx = (max_x + min_x) / 2;
	cy = (max_y + min_y) / 2;

        memset(temp, '\0', sizeof(temp));                 
        
        strncpy(temp, data_file, strlen(data_file) - 3);        

        t = strrchr(temp, '/'); 

        memset(c, '\0', sizeof(c));

        strcpy(c, t);  

        memset(temp, '\0', sizeof(temp));      

        strcat(temp, "Risultati");

        strcat(temp, c);        
        
        strcat(temp, "eps");          

        fp = fopen(temp, "w");
        check_pointer(fp, strcat(temp, "can't be opened!"));

        fprintf(fp, "%%!PS-Adobe-3.0\n%%%%BoundingBox: -5 -5 %d %d\n", W + 10, H + 10);
	fprintf(fp, "/l {rlineto} def /m {rmoveto} def\n"
		    "/c { .25 sub exch .25 sub exch .5 0 360 arc fill } def\n"
		    "/s { moveto -2 0 m 2 2 l 2 -2 l -2 -2 l closepath "
		    "gsave 1 setgray fill grestore gsave 3 setlinewidth"
		    " 1 setgray stroke grestore 0 setgray stroke }def\n"
	);

	for(int i = 0; i < px->nc; i++)
        {
		fprintf(fp, "\n%g %g %g setrgbcolor\n",
			colors[3*i], colors[3*i + 1], colors[3*i + 2]);
		
                for(int j = 0; j < px->carray[i].ncl; j++)
                {
	                fprintf(fp, "%.3f %.3f c\n",
				(px->carray[i].cpoints[j].x - cx) * scale + W / 2,
				(px->carray[i].cpoints[j].y - cy) * scale + H / 2);
		}

		fprintf(fp, "\n0 setgray %g %g s\n",
			(px->carray[i].centroid.x - cx) * scale + W / 2,
			(px->carray[i].centroid.y - cy) * scale + H / 2);
	}
	
        fprintf(fp, "\n%%%%EOF");

        fclose(fp);
	
        free(colors);
}

// Debug di una soluzione
void debug_solution(double x, double y, ClusterArray *px, double *mp, double min, int oldm, int newm)
{
        printf("\n\n");

        // Stampa punto
        printf("Punto: %f %f\n", x, y);

        // Stampa centroidi
        printf("Centroidi:");
        for(int i = 0; i < px->nc; i++)
                printf(" (%f %f)",px->carray[i].centroid.x, px->carray[i].centroid.y);
        printf("\n");

        // Stampa mp
        printf("Mp:");
        for(int i =0; i < px->nc; i++)
                printf(" (%d, %f)", i, mp[i]);
        printf("\n\n");

        printf("Minima: %f\n", min);

        printf("Vecchio cluster: %d\n", oldm);  
        printf("Nuovo cluster: %d\n", newm);       

}
