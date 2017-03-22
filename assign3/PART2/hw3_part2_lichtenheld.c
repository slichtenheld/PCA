#include "mpi.h"

#include <stdio.h>
#include <omp.h>

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


	//double* B_mtrx = (double *)malloc(B_rows*B_cols*sizeof(double));
	double B_mtrx[B_rows*B_cols];

	int A_distributed_elements = A_rows/total_ranks*A_cols;

	// Populate and broadcast B
	if(my_rank==root) {
		srand(0);
		mtrx_populate(B_mtrx,B_rows,B_cols);
		mtrx_print(B_mtrx, B_rows, B_cols);
		printf("Each node works on %d rows from A\n", A_rows/total_ranks );
		printf("Node 0 works on an additional %d rows\n", A_rows%total_ranks );
	}

	int error = MPI_Bcast(&B_mtrx, B_rows*B_cols, MPI_DOUBLE,root , MPI_COMM_WORLD);

	startTimer();

	if (error!= 0)
		printf("RANK %d: %d\n", my_rank, error );

	// Populate A and Result Vectors
	if(my_rank==root) {
		srand(total_ranks+1);
		printf("A_mtrx size: %d, A_vector size: %d, A_distributed_elements %d\n", A_rows*A_cols, (A_rows/total_ranks+A_rows%total_ranks)*A_cols, A_distributed_elements);
		

		int A_vector_elements = (A_rows/total_ranks+A_rows%total_ranks)*A_cols ;
		double* A_vector = malloc(A_vector_elements*sizeof(double)); 
		mtrx_populate(A_vector,(A_rows/total_ranks+A_rows%total_ranks), A_cols);// rank 0 works on leftovers
		
		double* R_vector = malloc((A_rows/total_ranks+A_rows%total_ranks) * B_cols); // rank 0 works on leftovers
		
		// perform matrix calculations

		// collect results from all nodes
	}
	else{
		srand(my_rank);
		double* A_vector = malloc(A_rows/total_ranks*A_cols*sizeof(double));
		mtrx_populate(A_vector,A_rows/total_ranks, A_cols);
		double* R_vector = malloc(A_rows/total_ranks*B_cols); // storage for results

		// perform matrix calculations

		// send info back to root node
	}

	stopTimer();

	if (my_rank==root){
		printf("WALL TIME: %f\n", getTimeSpent());
	}
	MPI_Finalize();
	
	return 0;

}

void mtrx_populate(double* mtrx, int rows, int cols){ // populates whole row, then moves to next row
	for(int i = 0; i < rows ; i++) {
		for (int j = 0; j < cols; j++ ){
			mtrx[i*cols + j] = randinrangeDOUBLE(0,10000);
			//printf("%d ", mtrx[i*cols + j] );
		}
	}
	//printf("\n");
}

void mtrx_print(double* mtrx, int rows, int cols){
	for(int i = 0; i < rows ; i++) {
		for (int j=0; j < cols ; j++) {
			//printf("%d, %d\n",i*cols + j, xy2int(j,i, cols) );
			//printf("%s\n", );
			printf("%f ", mtrx[i*cols + j]);
		}
		printf("\n");
	}
	printf("\n");
}

int xy2int(int x, int y, int cols){
	return y*cols+x;
}

// void mtrx_multiply(double* A, double* B, double* AB, int A_rows, int A_cols, int B_cols){
// 	int i, j, k;
	
// 	#pragma omp parallel shared(A,B,AB) private(i,j,k)
// 	//printf("num of threads: %d\n", omp_get_num_threads());
// 	{
// 	#pragma omp for schedule(static)
// 		for (i = 0; i < A_rows ; i++ ) {
// 			for (j = 0; j < B_cols; j++ ) {
// 				AB[i*B_cols + j] = 0;
// 				for (k = 0; k < A_cols ; k++ ){
// 					AB[i*B_cols + j] += A[i*A_cols+k]*B[k*B_cols+j];
// 				}
// 			}
// 		}
// 	}
// }

