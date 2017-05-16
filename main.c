#include <stdlib.h>
#include <stdio.h>
#include <time.h>

#include "AdjacencyMatrix.h"

int main(int argc, char* argv[]) {
	/* Open input file */
	FILE* fp_input = fopen(argv[1], "r");

	/* Create AdjacencyMatrix from input file *
	 * If an error occurs durring creation:	*
	 * 	write error message to stderr		*
	 *	close input file				*
	 *	terminate program				*/
	if(!AdjacencyMatrix(fp_input)) {
		fprintf(stderr, "Could not create adjacency matrix.\n"
			"Program terminating.\n");
		fclose(fp_input);
		exit(1);
	}

	/* Close input file */
	fclose(fp_input);

	/* Display AdjacencyMatrix */
	AdjacencyMatrix_display(AdjacencyMatrix_ADJ);

	clock_t timeBegin = clock();

	/* Generate shortest path information */
	AdjacencyMatrix_createShortestPath();

	clock_t timeEnd = clock();

	/* Display AdjacencyMatrix */
	AdjacencyMatrix_display(AdjacencyMatrix_SHP);

	printf("Elapsed: %lf seconds\n",
				(double)(timeEnd - timeBegin) / (double)CLOCKS_PER_SEC);

	/* Free AdjacencyMatrix resources */
	AdjacencyMatrix_destructor();

	return 0;
}
