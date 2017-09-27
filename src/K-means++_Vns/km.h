#ifndef __KM_H
#define __KM_H

#include "data.h"
#include "solution.h"
#include "common.h"

// Esegue il k-means
void km(ClusterArray *px, PointArray *pI, int clusters, int maxit, double *f);

// Crea la soluzione iniziale x(0) usando il k-means++
void kmeansPlus(ClusterArray *px, PointArray *pI);

#endif /* __KM_H */
