#include "mpi.h"
#include <stdio.h>
#include <stdlib.h>

// SHOULD BELONG IN SEPARATE C FILES
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
double randinrangeDOUBLE(float HI, float LO) {
	return LO + (double)(rand()) / ( (double)(RAND_MAX/(HI-LO)));
}

//******************************************************

void mtrx_populate(double* mtrx, int rows, int cols);
void mtrx_populate_simple(double* mtrx, int rows, int cols);
void mtrx_print(double* mtrx, int rows, int cols);
void mtrx_multiply(double* A, double* B, double* AB, int A_rows, int A_cols, int B_cols);
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
	A_rows+= (total_ranks - (A_rows%total_ranks)); // BOOTLEGGED FIX FOR ISSUE
	int A_cols = strtol(argv[2],NULL,10);
	int B_rows = A_cols;
	int B_cols = strtol(argv[3],NULL,10);
	int numThreads = strtol(argv[4],NULL,10);

	//omp_set_num_threads(numThreads);
	startTimer();

	int num_rows_per_proc = A_rows/total_ranks;
	int stride = num_rows_per_proc*A_cols; // A_cols = elements per row
	int left_over_rows = A_rows%total_ranks; 
	int left_over_elems = left_over_rows*A_cols;

	int A_recv_bufSIZE = stride+A_cols;
	int R_bufSIZE = (num_rows_per_proc)*B_cols;
	if (left_over_rows) R_bufSIZE+=B_cols;
	
	double* A_recv_buf = (double *)malloc( (A_recv_bufSIZE)*(sizeof(double)) );
	double* R_buf = (double *)malloc( R_bufSIZE*(sizeof(double)));


	int* sendcounts = (int *)malloc(total_ranks*sizeof(int));
	int* recvcounts = (int *)malloc(total_ranks*sizeof(int));
	int* displs = (int *)malloc(total_ranks*sizeof(int));
	int* recv_displs = (int *)malloc(total_ranks*sizeof(int));

	int sum = 0;
	int recv_sum = 0;
	for (int i = 0; i < total_ranks; i++) {
		sendcounts[i] = stride;
		if (left_over_rows > 0) {
			sendcounts[i] += A_cols;
			left_over_rows--;
		}
		displs[i] = sum;
		sum+=sendcounts[i]; 

		recvcounts[i] = sendcounts[i]/A_cols*B_cols;
		recv_displs[i] = recv_sum;
		recv_sum+=recvcounts[i];
	} 
	
	// if (my_rank == root){
	// 	printf("TOTAL RANKS: %d\n", total_ranks );
	// 	printf("A_recv_bufSIZE: %d R_bufSIZE: %d \n", R_bufSIZE, A_recv_bufSIZE);
	// 	printf("R_MATRIX_SIZE: %d \n", A_rows*B_cols);
	// 	for (int i = 0; i < total_ranks; i++) {
	// 		printf("%d: sendcount: %d displs: %d recvcount: %d recv_displs: %d \n", i, sendcounts[i], displs[i], recvcounts[i], recv_displs[i]);
	// 	}
	// }

	double* B_mtrx = (double *)malloc(B_rows*B_cols*sizeof(double)); 
	
	// A_mtrx and R_mtrx only used by root
	double* A_mtrx;  // row major order 
	double* R_mtrx; 

	// Populate A and B, instantiate R_mtrx in root
	if(my_rank==root) {
		
		mtrx_populate_simple(B_mtrx,B_rows,B_cols); // TODO: change back to normal
		//mtrx_print(B_mtrx, B_rows, B_cols);

		A_mtrx = (double *)malloc(A_rows*A_cols*sizeof(double)); 
		mtrx_populate_simple(A_mtrx,A_rows,A_cols); // TODO: change back to normal

		//mtrx_print(A_mtrx, A_rows, A_cols);
		R_mtrx = (double *)malloc(A_rows*B_cols*sizeof(double)); 
	}

	// broadcast B_mtrx to all processes
	MPI_Bcast(B_mtrx, B_rows*B_cols, MPI_DOUBLE,root , MPI_COMM_WORLD);
	
	// scatter A to all processes
	MPI_Scatterv(A_mtrx, sendcounts, displs,
                 MPI_DOUBLE, A_recv_buf, A_recv_bufSIZE,
                 MPI_DOUBLE,
                 root, MPI_COMM_WORLD);

	// for(int i = 0; i < sendcounts[my_rank]; i++) {
	// 	printf("%d ", (int)A_recv_buf[i]);
	// }
	// printf("\n");

	// matrix multiplication
	mtrx_multiply(A_recv_buf, B_mtrx, R_buf, sendcounts[my_rank]/A_cols, A_cols, B_cols );

	MPI_Gatherv(R_buf, R_bufSIZE, MPI_DOUBLE,
                R_mtrx, recvcounts, recv_displs,
                MPI_DOUBLE, root, MPI_COMM_WORLD);

	stopTimer();
	if (my_rank == root) {
		printf("WALL TIME: %f\n",getTimeSpent() );
		// mtrx_print(R_mtrx, A_rows, B_cols);
	}

	
	MPI_Finalize();
	
	return 0;

}

void mtrx_multiply(double* A, double* B, double* AB, int A_rows, int A_cols, int B_cols){
	int i, j, k;
	
	#pragma omp parallel shared(A,B,AB) private(i,j,k)
	//printf("num of threads: %d\n", omp_get_num_threads());
	{
	#pragma omp for schedule(static)
		for (i = 0; i < A_rows ; i++ ) {
			for (j = 0; j < B_cols; j++ ) {
				AB[i*B_cols + j] = 0;
				for (k = 0; k < A_cols ; k++ ){
					AB[i*B_cols + j] += A[i*A_cols+k]*B[k*B_cols+j];
				}
			}
		}
	}
}

void mtrx_populate(double* mtrx, int rows, int cols){ // populates whole row, then moves to next row
	for(int i = 0; i < rows ; i++) {
		for (int j = 0; j < cols; j++ ){
			mtrx[i*cols + j] = randinrangeDOUBLE(0,10000);
		}
	}
}

void mtrx_populate_simple(double* mtrx, int rows, int cols){
	for(int i = 0; i < rows ; i++) {
		for (int j = 0; j < cols; j++ ){
			mtrx[i*cols + j] = i*cols + j;
		}
	}
}

void mtrx_print(double* mtrx, int rows, int cols){
	for(int i = 0; i < rows ; i++) {
		for (int j=0; j < cols ; j++) {
			printf("%d ", (int)mtrx[i*cols + j]);
		}
		printf("\n");
	}
	printf("\n");
}

int xy2int(int x, int y, int cols){
	return y*cols+x;
}