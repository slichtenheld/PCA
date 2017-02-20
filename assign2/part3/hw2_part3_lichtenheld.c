// Samuel Lichtenheld
// eel 6763

#include "mpi.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

void initialize_data(int *A, int *Ap, int* B, int N); //make sure each processor has its own seed

void	scatter_data(int	*A,	int *B, int	N);
void	mask_operation(int	*A,	int	N,	int	*Ap);	
void	gather_results(int	*Ap,	int	N);	

void print2DArray(int *A, int rows, int cols);

int randinrange(int,int);


int main (int argc, char * argv[]) {
	double start, end; // for time keeping purposes
	int my_rank; // this process' rank
	int num_tasks; // total tasks allocated
	int *A; // initial array
	int *Ap; // final array
	int *B; // temporary storage (receive buffer)

	int *sendcounts; //send amount for each process
	int *displs; //displacement for each process

	if (argc != 2) {
        perror("ERROR: ./program <N: for NxN matrix>");
        return(-1);
    }

    /* MPI BEGIN */
    MPI_Init(&argc,&argv);
	MPI_Comm_rank(MPI_COMM_WORLD,&my_rank);

	// Matrix of N*N size
	int N	=	atof(argv[1]);
	if(!my_rank){
		A = malloc(N*N*sizeof(int));
		Ap = malloc(N*N*sizeof(int));
	}
	B = malloc( (N*3) * sizeof(int) ); // has 3 rows
	
	MPI_Barrier(MPI_COMM_WORLD); /* IMPORTANT */
	start = MPI_Wtime();

	

	
	initialize_data(A, Ap, B, N); // instantiates start and finish array, populates start array
	//print2DArray(B,N);
	//scatter_data(A, B, N);
	//print2DArray(B,N);
	MPI_Barrier(MPI_COMM_WORLD); /* IMPORTANT */
	end = MPI_Wtime();
	

	if (my_rank == 0) { /* use time on master node */
    	printf("Runtime = %f\n", end-start);
	}

	MPI_Finalize();

	return 0;

}



void print2DArray(int *A, int rows, int cols){
    for(int j = 0; j < rows; j++){
        for (int i = 0; i < cols; i++){
            printf("%d ", A[j*cols+i]);
        }
        printf("\n"); // new line
    }
}


void initialize_data(int *A, int *Ap, int *B, int N){ // each processor's seed is its rank
	int my_rank,num_tasks;
	MPI_Comm_rank(MPI_COMM_WORLD,&my_rank);
	MPI_Comm_size(MPI_COMM_WORLD,&num_tasks);
	
	if (!my_rank){ // master node

		printf("leftovers: %d\n", (N*N)%num_tasks);

		for(int i = 0; i < N*N; i++){
			A[i] = randinrange(0,10000);
			//printf("%d\n",A[i]);
		}

		print2DArray(A,N,N);
		//print2DArray(B,N);
		
	}
	else{
		//B = malloc( (N*N)/num_tasks * sizeof(int) );
	}

}

void	scatter_data(int *A, int *B, int N){
	int my_rank,num_tasks;
	MPI_Comm_rank(MPI_COMM_WORLD,&my_rank);
	MPI_Comm_size(MPI_COMM_WORLD,&num_tasks);

	int root = 0;
	int send_count = 1;
	int recv_count = 1;
	
	MPI_Scatter(
    	A,
    	send_count,
    	MPI_INT,
    	B,
    	recv_count,
    	MPI_INT,
    	root,
    	MPI_COMM_WORLD);

	// if (!my_rank){
	// 	print2DArray(B,N);
	// }
	//printf("0_1: %d\n", B[1]);
	


}

int randinrange(int LO, int HI) {
	return LO + (rand()) / ( (RAND_MAX/(HI-LO)));
}




/* 
int MPI_Reduce(const void *sendbuf, void *recvbuf, int count, MPI_Datatype datatype,
               MPI_Op op, int root, MPI_Comm comm)

MPI_Scatter(
    void* send_data,
    int send_count,
    MPI_Datatype send_datatype,
    void* recv_data,
    int recv_count,
    MPI_Datatype recv_datatype,
    int root,
    MPI_Comm communicator)


MPI_Gather (&sendbuf,sendcnt,sendtype,&recvbuf, 
...... recvcount,recvtype,root,comm) 




*/