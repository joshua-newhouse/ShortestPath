#include <stdlib.h>
#include <stdio.h>
#include <time.h>

#include "AdjacencyMatrix.h"

#define RAND_INIT() {			\
	time_t t;				\
	srand((unsigned)time(&t));	\
}

float randomVal(void) {
	int selector = rand() % 10;
	switch(selector) {
	case 8: case 9:
		return (float)rand() / (float)RAND_MAX * (float)(rand() % 100);
	default:
		return 0.0f;
	}
}

int main(int argc, char* argv[]) {
	size_t nVert = 0;

	RAND_INIT();

	if(argc != 3) {
		printf("Useage: generator <n> <OutputFilename>\n"
			"where n is an integer denoting the number of vertices in "
			"the graph.\n");
		exit(EXIT_FAILURE);
	}
	else if((nVert = (size_t)atoi(argv[1])) == 0) {
		printf("Invalid number of vertices: must be an integer greater "
			" than 0.\n");
		exit(EXIT_FAILURE);
	}

	float* graph = calloc(nVert * nVert, sizeof(float));

	for(int i = 0; i < nVert; i++)
		for(int j = 0; j < nVert; j++) {
			float val = i == j ? 0.0f : randomVal();
			*(graph + i * nVert + j) = *(graph + j * nVert + i) = val;
		}

	FILE* fp = fopen(argv[2], "w");
	if(fp)
		AdjacencyMatrix_writeToFile(fp, graph, nVert);
	else
		printf("Error opening file %s\n", argv[2]);
	fclose(fp);
	
	return 0;
}
