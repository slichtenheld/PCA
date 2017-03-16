CC = gcc

OBJECTS = randNumGen.o execTimeOPENMP.o
DEPS = randNumGen.h execTime.h

sobel: $(OBJECTS) sobel.o 
	$(CC) -o sobel $(OBJECTS) sobel.o -fopenmp -lm

sobel.o: sobel.c
	$(CC) -c sobel.c -fopenmp

randNumGen.o: randNumGen.c randNumGen.h
	$(CC) -c randNumGen.c -fopenmp

execTimeOPENMP.o: execTimeOPENMP.c execTime.h
	$(CC) -c execTimeOPENMP.c -fopenmp

clean:
	rm sobel *.o