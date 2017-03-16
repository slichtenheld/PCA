CC = gcc

OBJECTS = randNumGen.o execTimeOPENMP.o
DEPS = randNumGen.h execTime.h

matrixOPENMP: $(OBJECTS) matrixOPENMP.o 
	$(CC) -o matrixOPENMP $(OBJECTS) matrixOPENMP.o -fopenmp

matrixOPENMP.o: matrixOPENMP.c
	$(CC) -c matrixOPENMP.c -fopenmp

randNumGen.o: randNumGen.c randNumGen.h
	$(CC) -c randNumGen.c -fopenmp

execTimeOPENMP.o: execTimeOPENMP.c execTime.h
	$(CC) -c execTimeOPENMP.c -fopenmp

clean:
	rm matrixOPENMP *.o