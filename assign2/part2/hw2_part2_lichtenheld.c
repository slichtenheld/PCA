// Samuel Lichtenheld
// eel 6763

#include "mpi.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

void initialize_data(void); //make sure each processor has its own seed
double estimate_g(double, double, long long);
void collect_results(double *result, double *globalResult);

double function(double);
double randinrange(float,float);


int main (int argc, char * argv[]) {
	//printf("whats going on here...");
	double start, end;
	int my_rank;

	double result = 0;
	double globalResult = 0;

	if (argc != 4) {
        perror("ERROR: ./program <lower_bound> <upper_bound> <# of data points>");
        return(-1);
    }

    /* MPI BEGIN */
    MPI_Init(&argc,&argv);
	MPI_Comm_rank(MPI_COMM_WORLD,&my_rank);
	

	MPI_Barrier(MPI_COMM_WORLD); /* IMPORTANT */
	start = MPI_Wtime();

	float lower_bound	=	atof(argv[1]);
	float upper_bound	=	atof(argv[2]);
	long long	int	N	=	atof(argv[3]);

	

	initialize_data();
	result =  estimate_g(lower_bound,upper_bound,N);
	//printf("%d: %f\n", my_rank, result );
	collect_results(&result,&globalResult);


	MPI_Barrier(MPI_COMM_WORLD); /* IMPORTANT */
	end = MPI_Wtime();
	
	if (my_rank == 0) { /* use time on master node */
		globalResult = (upper_bound - lower_bound) / N * globalResult; //since collect results just adds all individual results 
    	printf("Result: %f\n", globalResult);
    	printf("Runtime = %f\n", end-start);
	}

	MPI_Finalize();

	return 0;

}

double function(double var) {
	return ( ( 8 * sqrt( 2 * M_PI ) ) / ( pow( M_E, pow(2 * var, 2) ) ) ) ;
}

double randinrange(float HI, float LO) {
	return LO + (double)(rand()) / ( (double)(RAND_MAX/(HI-LO)));
}

void initialize_data(){ // each processor's seed is its rank
	int my_rank;
	MPI_Comm_rank(MPI_COMM_WORLD,&my_rank);
	srand(my_rank);
}

double estimate_g(double lower_bound, double upper_bound, long long N) {
	int my_rank, total_ranks;
	MPI_Comm_rank(MPI_COMM_WORLD,&my_rank); //gets my_rank for each processor
	MPI_Comm_size(MPI_COMM_WORLD,&total_ranks); //gets total amount of ranks

	double result;

	long long N_split = 0;

	/* root 0 takes remainder of N, others compute equal amount */
	if (my_rank == 0){
		if (total_ranks == 1) {
			N_split = N;
		}
		else {
			N_split = N % (total_ranks - 1);
		}
	}
	else {
		N_split = N / (total_ranks - 1);
	}

	//printf("N_split: %ld\n",N_split);

	for (int i = 0; i < N_split; i++) {
		result += function((double)randinrange(upper_bound, lower_bound) );
	}

	return  result; // still have to be divided by N
}	

void collect_results(double *result, double * globalResult) {
	int dest=0;
	int count = 1;

	int rc = MPI_Reduce(result, globalResult, count, MPI_DOUBLE,
               MPI_SUM, dest, MPI_COMM_WORLD);
	if (rc != MPI_SUCCESS)
      printf("failure on mpc_reduce\n");
}

/* 
int MPI_Reduce(const void *sendbuf, void *recvbuf, int count, MPI_Datatype datatype,
               MPI_Op op, int root, MPI_Comm comm)
*/