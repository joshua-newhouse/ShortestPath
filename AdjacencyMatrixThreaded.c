#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <float.h>
#include <pthread.h>

#include "AdjacencyMatrix.h"

#define THREAD_COUNT 4

static size_t nVertices;
static float* adjMatrix;
static Vertex_ts* vertices;
static float* shortestPathMatrix;

static float vector_op(float*, float*, size_t);
static char nextHop(float*, size_t, size_t);
static void display(float*);
static void* calculateSPM(void*);

/* Constructor
	Takes a pointer to the file containing the graph as input parameter.
	Returns 1 if successfully builds the adjacency matrix, 0 otherwise.	*/
int AdjacencyMatrix(FILE* fp_input) {
	//Check for valid input file
	if(!fp_input) {
		fprintf(stderr, "AdjacencyMatrix(FILE* fp_input) : invalid file\n");
		return 0;
	}

	//Get number of vertices from input file
	if(fscanf(fp_input, " %zu ", &nVertices) == 0) {
		fprintf(stderr, "AdjacencyMatrix(FILE* fp_input) : invalid number "
			"of vertices.\n");
		return 0;
	}

	//Allocate space for adjacency matrix and vertice array
	adjMatrix = calloc(nVertices * nVertices, sizeof(float));
	vertices = calloc(nVertices, sizeof(Vertex_ts));

	//Check for allocation failure of adjMatrix and vertices
	if(!(adjMatrix && vertices)) {
		char adjMatrixErr[] = "Error allocating space for adjacency matrix.";
		char vertErr[] = "Error allocating space for vertices.";

		adjMatrix ? fprintf(stderr, "AdjacencyMatrix(FILE* fp_input) : %s\n",
					vertErr) :
				fprintf(stderr, "AdjacencyMatrix(FILE* fp_input) : %s\n",
					adjMatrixErr);
		AdjacencyMatrix_destructor();
		return 0;
	}

	//Read vertices and graph weights from file
	size_t lineCounter;
	for(lineCounter = 0; lineCounter < nVertices; lineCounter++) {
		int error;

		error = fscanf(fp_input, " %u ", &vertices[lineCounter].label);
		if(!error) {
			fprintf(stderr, "AdjacencyMatrix(FILE* fp_input) : "
				"Error reading vertex label.\n");
			AdjacencyMatrix_destructor();
			return 0;
		}

		size_t i;
		for(i = 0; i < nVertices; i++) {
			error = fscanf(fp_input, " %f ",
					adjMatrix + lineCounter * nVertices + i);
			if(!error) {
				fprintf(stderr, "AdjacencyMatrix(FILE* fp_input) : "
					"Error reading matrix entry %zu, %zu.\n",
					lineCounter, i);
				AdjacencyMatrix_destructor();
				return 0;
			}
		}
	}

	return 1;
}

/* Public interface for displaying the adjacency or shortest path matrices */
void AdjacencyMatrix_display(Matrix_te mType) {
	switch(mType) {
	case AdjacencyMatrix_ADJ:
		display(adjMatrix);
		break;
	case AdjacencyMatrix_SHP:
		display(shortestPathMatrix);
		break;
	}
}

/* Writes the matrix to the stdout */
static void display(float* matrix) {
	size_t i, j;

	if(!(vertices && matrix)) {
		fprintf(stderr, "AdjacencyMatrix_display() : "
			"Matrix not initialized.\n");
		return;
	}

	printf(" ");
	for(i = 0; i <nVertices; i++)
		printf("\t%u", vertices[i].label);
	printf("\n");

	for(i = 0; i < nVertices; i++) {
		printf("%u\t", vertices[i].label);
		for(j = 0; j < nVertices; j++)
			printf("%.1f\t", matrix[i * nVertices + j]);
		printf("\n");
	}
}

/* Frees allocated memory */
void AdjacencyMatrix_destructor() {
	if(adjMatrix)
		free(adjMatrix);
	if(vertices)
		free(vertices);
	if(shortestPathMatrix)
		free(shortestPathMatrix);
}

/* Creates the shortest path matrix */
int AdjacencyMatrix_createShortestPath() {
	if(!adjMatrix) {
		fprintf(stderr, "AdjacencyMatrix_createShortestPath() : "
			"Adjacency matrix not initialized.\n");
		return 0;
	}

	shortestPathMatrix = calloc(nVertices * nVertices, sizeof(float));

	if(!shortestPathMatrix) {
		fprintf(stderr, "AdjacencyMatrix_createShortestPath() : "
			"Error allocating space for shortest path matrix.\n");
		return 0;
	}

	memcpy(shortestPathMatrix, adjMatrix,
						sizeof(float) * nVertices * nVertices);

	pthread_t threads[THREAD_COUNT];
	long thread;
	for(thread = 0; thread < THREAD_COUNT; thread++)
		pthread_create(&threads[thread], NULL, calculateSPM, (void*)thread);

	for(thread = 0; thread < THREAD_COUNT; thread++)
		pthread_join(threads[thread], NULL);

	return 1;
}

static float vector_op(float* v1, float* v2, size_t n) {
	float ret_val = FLT_MAX;

	size_t i;
	for(i = 0; i < n; i++) {
		float sum = v1[i] && v2[i] ? v1[i] + v2[i] : 0.0f;
		ret_val = sum && (ret_val > sum) ? sum : ret_val;
	}

	return ret_val == FLT_MAX ? 0.0f : ret_val;
}

static char nextHop(float* spmRow, size_t row, size_t n) {
	char ret_val = 0;

	size_t i;
	for(i = 0; i < n; i++) {
		if(i == row)
			continue;

		float initVal = spmRow[i];
		float tmp = vector_op(spmRow, adjMatrix + i * n, n);
		if(tmp && initVal)
			spmRow[i] = initVal < tmp ? initVal : tmp;
		else
			spmRow[i] = initVal ? initVal : tmp;
		ret_val = ret_val || (initVal != spmRow[i]);
	}

	return ret_val;
}

static void* calculateSPM(void* rank) {
	long my_rank = (long)rank;
	size_t extraRows = nVertices % THREAD_COUNT;
	size_t my_rows = my_rank < extraRows ?
							nVertices / THREAD_COUNT + 1 :
							nVertices / THREAD_COUNT;

	size_t begin_row = my_rank >= extraRows ?
								my_rank * my_rows + extraRows :
								my_rank * my_rows;
	size_t end_row = begin_row + my_rows;

	size_t i;
	for(i = begin_row; i < end_row; i++) {
		char hasChanged = 0;
		do {
			hasChanged =
				nextHop(shortestPathMatrix + i * nVertices,
											i, nVertices);
		} while(hasChanged);
	}

	return NULL;
}

void AdjacencyMatrix_writeToFile(FILE* out, float* matrix, size_t n) {
	size_t i, j;

	if(!(n && matrix)) {
		fprintf(stderr, "AdjacencyMatrix_writeToFile() : "
			"Matrix not initialized.\n");
		return;
	}

	fprintf(out, "%zu\n", n);

	for(i = 0; i < n; i++) {
		fprintf(out, "%zu", i + 1);
		for(j = 0; j < n; j++)
			fprintf(out, "\t%.1f", matrix[i * n + j]);
		fprintf(out, "\n");
	}
}
