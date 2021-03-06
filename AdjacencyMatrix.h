#include <stdio.h>

#ifndef _ADJ_MTRX_
#define _ADJ_MTRX_

struct Vertex {
	unsigned label;
	unsigned maxHop;
} typedef Vertex_ts;

enum MatrixType {
	AdjacencyMatrix_ADJ,
	AdjacencyMatrix_SHP
} typedef Matrix_te;

extern int AdjacencyMatrix(FILE*);
extern void AdjacencyMatrix_display(Matrix_te);
extern void AdjacencyMatrix_destructor();
extern int AdjacencyMatrix_createShortestPath();
extern void AdjacencyMatrix_writeToFile(FILE*, float*, size_t);

#endif
