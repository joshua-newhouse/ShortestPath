CC = gcc
FLAGS = -Wall -g
PROG = ShortestPath
OBJ = main.o AdjacencyMatrix.o
OBJTHRD = main.o AdjacencyMatrixThreaded.o

$(PROG) : $(OBJ)
	$(CC) $(FLAGS) -o $(PROG).out $(OBJ)

threaded : $(OBJTHRD)
	$(CC) $(FLAGS) -o $(PROG)threaded.out $(OBJTHRD) -lpthread

generator : generator.c AdjacencyMatrix.o
	$(CC) -Wall -O3 -o generator.out generator.c AdjacencyMatrix.o

main.o : main.c AdjacencyMatrix.h
	$(CC) $(FLAGS) -c main.c

AdjacencyMatrix.o : AdjacencyMatrix.c AdjacencyMatrix.h
	$(CC) $(FLAGS) -c AdjacencyMatrix.c

AdjacencyMatrixThreaded.o : AdjacencyMatrixThreaded.c AdjacencyMatrix.h
	$(CC) $(FLAGS) -c AdjacencyMatrixThreaded.c -lpthread

clean :
	rm -f $(PROG) $(OBJ) $(OBJTHRD)

cleanGen :
	rm -f generator AdjacencyMatrix.o
