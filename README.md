ShortestPath

This class computes the distance of the shortest path between nodes of a simple, undirected, irreflexive graph.
The pupose of this project is to gain experience using multithreading via pthread API as well as OpenCL programming.

Current Status:
Program calculates shortest path using an adjacency matrix.  Uses pthreads for parallel computations.
Current speed-up is directly proportional to number of threads == number of available cores.

To Do:
Redesign with CUDA or OpenCL
Optimize by reducing matrix calculations to upper triangular only
