#ifndef __POINT_H
#define __POINT_H


// Struttura usata per definire i punti bidimensionali
typedef struct{
        double x;
        double y;      
}Point;

// Divide le coordinate di un punto per un valore
void divide(Point *p, int d);

// Moltiplica le coordinate di un punto per un valore
void mul(Point *p, int d);

// Somma tra punti
void add(Point *p1, Point *p2);

// Differenza tra punti
Point sub(Point *p1, Point *p2);

// Modulo della distanza euclidea
double modulus(Point p);

// Distanza euclidea tra due punti
double euclidean(Point *p1, Point *p2);

// Stampa di un punto
void print_point(Point *p);

#endif /* __POINT_H */
