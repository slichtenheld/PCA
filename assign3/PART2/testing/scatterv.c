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

	int num_rows_per_proc = A_rows/total_ranks;
	int stride = num_rows_per_proc*A_cols; // A_cols = elements per row
	int left_over_rows = A_rows%total_ranks; 
	int left_over_elems = left_over_rows*A_cols;

	int A_recv_bufSIZE = stride+left_over_elems;

	double* A_recv_buf = (double *)malloc( (A_recv_bufSIZE)*(sizeof(double)) );
	int* sendcounts = (int *)malloc(total_ranks*sizeof(int));
	int* displs = (int *)malloc(total_ranks*sizeof(int));

	if (my_rank == root){
		printf("num_rows_per_proc: %d stride: %d left_over_rows: %d A_recv_bufSIZE: %d\n",num_rows_per_proc, stride, left_over_rows,A_recv_bufSIZE );
	}
	for (int i = 0; i < total_ranks; i++) {
		sendcounts[i] = stride;
		displs[i] = i*stride; 
	}
	sendcounts[total_ranks-1]+= left_over_elems; //last node calculates left over rows

	if (my_rank == root){
		for (int i = 0; i < total_ranks; i++) {
			printf("%d: sendcount: %d displs: %d \n", i, sendcounts[i], displs[i]);
		}
	}

	double* A_mtrx; 
	double* R_mtrx;
	// Populate and scatter A
	if(my_rank==root) {
		A_mtrx = (double *)malloc(A_rows*A_cols*sizeof(double));
		R_mtrx = (double *)malloc(A_rows*A_cols*sizeof(double));
		mtrx_populate(A_mtrx,A_rows,A_cols);
		mtrx_print(A_mtrx,A_rows,A_cols);

	}


	MPI_Scatterv(A_mtrx, sendcounts, displs,
                 MPI_DOUBLE, A_recv_buf, A_recv_bufSIZE,
                 MPI_DOUBLE,
                 root, MPI_COMM_WORLD);

	for(int i = 0; i < sendcounts[my_rank]; i++) {
		printf("%f ", A_recv_buf[i]);
		A_recv_buf[i] = my_rank;
	}
	printf("\n");
	//mtrx_print(B_mtrx, B_rows, B_cols);

	MPI_Gatherv(A_recv_buf, A_recv_bufSIZE, MPI_DOUBLE,
                R_mtrx, sendcounts, displs,
                MPI_DOUBLE, root, MPI_COMM_WORLD);

	if (my_rank==root){
		mtrx_print(R_mtrx, A_rows, A_cols);
	}
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