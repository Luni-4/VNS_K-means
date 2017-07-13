#include <stdio.h>
#include <math.h>

#include "point.h"

// Divide le coordinate di un punto per un valore
void divide(Point *p, int d)
{
        p->x = p->x / ((double)d);
        p->y = p->y / ((double)d);
}

// Moltiplica le coordinate di un punto per un valore
void mul(Point *p, int d)
{
        p->x = p->x * ((double)d);
        p->y = p->y * ((double)d);
}

// Somma tra punti
void add(Point *p1, Point *p2)
{       
        p1->x += p2->x;
        p1->y += p2->y;      
}

// Differenza tra punti
Point sub(Point *p1, Point *p2)
{
        Point p;
        p.x = p1->x - p2->x;
        p.y = p1->y - p2->y;
        return p;
}

// Modulo della distanza euclidea
double modulus(Point p)
{
        return (p.x * p.x) + (p.y * p.y);
}

// Distanza euclidea tra due punti
double euclidean(Point *p1, Point *p2)
{
        return modulus(sub(p1, p2));
}

// Stampa di un punto
void print_point(Point *p)
{
        printf("[%f %f]\n", p->x, p->y);
}
