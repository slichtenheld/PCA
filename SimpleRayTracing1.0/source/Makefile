# TO DO:1- Test the program with MPI and OPENMP
#		2- esubmit with OPENMP
#		3- Load measurement
#		4- Image printing
#		5- Data preparation for report

# For now this file is empty except the 
# definition of some macros 

#CC=mpic++
#CC=icpc
#CC=g++ -fopenmp 
#CC=g++
CC=mpic++ -fopenmp
 
#CFLAGS=-c -O2 -DSRAY_OPENMP -DSRAY_LB1 -Wno-deprecated -I../include/
#CFLAGS=-c -O3 -DSRAY_MPI -DSRAY_LB1 -DLOAD_MEASUREMENT -Wno-deprecated -I../include/
#CFLAGS=-c -O3 -Wno-deprecated -I../include/
CFLAGS=-c -O3 -DSRAY_OPENMP -DSRAY_MPI -DSRAY_LB1 -DLOAD_MEASUREMENT -Wno-deprecated -I../include/ 

# -DSRAY_LB1
# -DSRAY_OPENMP -fopenmp
# -DSRAY_MPI
# -DLOAD_MEASUREMENT

CONFIGIN = config.xml
#CONFIGOUT = output.xml
IMAGEOUT = img.tga
PROGRAM = ./sray_new_load

#RUN = ./$(PROGRAM) 
#RUN = mpirun
#RUNFLAGS = -np 1

all: $(PROGRAM)
$(PROGRAM): main.o sphere.o viewport.o scene.o color.o light.o materialproperty.o object.o xml.o
	$(CC) main.o sphere.o viewport.o scene.o color.o light.o materialproperty.o object.o xml.o -o $(PROGRAM)

main.o: main.cpp
	$(CC) $(CFLAGS) main.cpp

sphere.o: sphere.cpp
	$(CC) $(CFLAGS) sphere.cpp

viewport.o: viewport.cpp
	$(CC) $(CFLAGS) viewport.cpp

scene.o: scene.cpp
	$(CC) $(CFLAGS) scene.cpp

color.o: color.cpp
	$(CC) $(CFLAGS) color.cpp

light.o: light.cpp
	$(CC) $(CFLAGS) light.cpp

materialproperty.o: materialproperty.cpp
	$(CC) $(CFLAGS) materialproperty.cpp

object.o: object.cpp
	$(CC) $(CFLAGS) object.cpp

xml.o: xml.cpp
	$(CC) $(CFLAGS) xml.cpp

run:
	$(RUN) $(RUNFLAGS) $(PROGRAM) $(CONFIGIN) $(IMAGEOUT) $(CONFIGOUT)

clean:
	rm $(PROGRAM); rm *.o

# esubmit -t 5 -n 8 /opt/scali/bin/mpirun -np 16 ./simpleray
# esubmit -t 5 -n 4 /opt/scali/bin/mpirun -np 8 ./simpleray
# esubmit -t 5 -n 2 /opt/scali/bin/mpirun -np 4 ./simpleray
# esubmit -t 5 -n 1 /opt/scali/bin/mpirun -np 2 ./simpleray
# esubmit -t 5 -n 1 /opt/scali/bin/mpirun -np 1 ./simpleray
# esubmit -t 5 -n 2 /opt/scali/bin/mpirun -np 3 ./simpleray
# esubmit -t 5 -n 3 /opt/scali/bin/mpirun -np 5 ./simpleray
# esubmit -t 5 -n 3 /opt/scali/bin/mpirun -np 6 ./simpleray
# esubmit -t 5 -n 4 /opt/scali/bin/mpirun -np 7 ./simpleray
# esubmit -t 5 -n 5 /opt/scali/bin/mpirun -np 9 ./simpleray
# esubmit -t 5 -n 5 /opt/scali/bin/mpirun -np 10 ./simpleray
# esubmit -t 5 -n 6 /opt/scali/bin/mpirun -np 11 ./simpleray
# esubmit -t 5 -n 6 /opt/scali/bin/mpirun -np 12 ./simpleray
# esubmit -t 5 -n 7 /opt/scali/bin/mpirun -np 13 ./simpleray
# esubmit -t 5 -n 7 /opt/scali/bin/mpirun -np 14 ./simpleray
# esubmit -t 5 -n 8 /opt/scali/bin/mpirun -np 15 ./simpleray
