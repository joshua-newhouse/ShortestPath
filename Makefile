CC = gcc
FLAGS = -Wall -g
PROG = ShortestPath
OBJ = main.o AdjacencyMatrix.o

$(PROG) : $(OBJ)
	$(CC) $(FLAGS) -o $(PROG).out $(OBJ)

generator : generator.c AdjacencyMatrix.o
	$(CC) -Wall -O3 -o generator.out generator.c AdjacencyMatrix.o

main.o : main.c AdjacencyMatrix.h
	$(CC) $(FLAGS) -c main.c

AdjacencyMatrix.o : AdjacencyMatrix.c AdjacencyMatrix.h
	$(CC) $(FLAGS) -c AdjacencyMatrix.c

clean :
	rm -f $(PROG) $(OBJ)

cleanGen :
	rm -f generator AdjacencyMatrix.o
