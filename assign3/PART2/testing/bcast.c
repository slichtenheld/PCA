#include "mpi.h"

#include <stdio.h>

#include <stdlib.h>

// SHOULD BELONG IN SEPARATE C FILES
double randinrangeDOUBLE(float HI, float LO) {
	return LO + (double)(rand()) / ( (double)(RAND_MAX/(HI-LO)));
}

double begin;
double end;
void startTimer(){
	begin = MPI_Wtime();
}
void stopTimer(){
	MPI_Barrier(MPI_COMM_WORLD); /* IMPORTANT */
	end = MPI_Wtime();
}
double getTimeSpent(){
	return (double)(end - begin);
}

//******************************************************

void mtrx_populate(double* mtrx, int rows, int cols);
void mtrx_print(double* mtrx, int rows, int cols);
//void mtrx_multiply(double* A, double* B, double* AB, int A_rows, int A_cols, int B_cols);
int xy2int(int x, int y, int cols);


int main(int argc, char * argv[]){

	MPI_Status status;
	int my_rank, total_ranks;
	int root = 0;
	int tag = 0;

	if (argc != 5) {
		perror("ERROR: ./program < A_rows > < A_cols > < B_cols > < numThreads> ");
		exit(EXIT_FAILURE);
	}

	// MPI BEGIN
	MPI_Init(&argc,&argv);
	MPI_Comm_rank(MPI_COMM_WORLD,&my_rank);
	MPI_Comm_size(MPI_COMM_WORLD,&total_ranks);

	// Every node needs access to rows and cols of each input matrix
	int A_rows = strtol(argv[1],NULL,10);
	int A_cols = strtol(argv[2],NULL,10);
	int B_rows = A_cols;
	int B_cols = strtol(argv[3],NULL,10);


	double* B_mtrx = (double *)malloc(B_rows*B_cols*sizeof(double));
	//double B_mtrx[B_rows*B_cols];


	// Populate and broadcast B
	if(my_rank==root) {
		mtrx_populate(B_mtrx,B_rows,B_cols);
	}

	MPI_Bcast(B_mtrx, B_rows*B_cols, MPI_DOUBLE,root , MPI_COMM_WORLD);
	mtrx_print(B_mtrx, B_rows, B_cols);

	
	MPI_Finalize();
	
	return 0;

}

void mtrx_populate(double* mtrx, int rows, int cols){ // populates whole row, then moves to next row
	for(int i = 0; i < rows ; i++) {
		for (int j = 0; j < cols; j++ ){
			mtrx[i*cols + j] = randinrangeDOUBLE(0,10000);
		}
	}
}

void mtrx_print(double* mtrx, int rows, int cols){
	for(int i = 0; i < rows ; i++) {
		for (int j=0; j < cols ; j++) {
			printf("%f ", mtrx[i*cols + j]);
		}
		printf("\n");
	}
	printf("\n");
}

int xy2int(int x, int y, int cols){
	return y*cols+x;
}