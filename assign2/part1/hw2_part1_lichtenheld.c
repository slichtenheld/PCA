// Samuel Lichtenheld
// eel 6763

#include "mpi.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

void initialize_data(void); //make sure each processor has its own seed
double estimate_g(double, double, long long);
void collect_results(double *result);

double function(double);
double randinrange(float,float);


int main (int argc, char * argv[]) {
	//printf("whats going on here...");
	if (argc != 4) {
        perror("ERROR: ./program <lower_bound> <upper_bound> <# of data points>");
        return(-1);
    }

    /* MPI BEGIN */
    MPI_Init(&argc,&argv);
    //printf("whats going on here Part 2...");
	float lower_bound	=	atof(argv[1]);
	float upper_bound	=	atof(argv[2]);
	long long	int	N	=	atof(argv[3]);

	double result = 0;

	initialize_data();
	result =  estimate_g(lower_bound,upper_bound,N);
	collect_results(&result);

	MPI_Finalize();
	return 0;

}

double function(double var) {
	return ( ( 8 * sqrt( 2 * M_PI ) ) / ( pow( M_E, pow(2 * var, 2) ) ) ) ;
}

double randinrange(float HI, float LO) {
	//return ( ( rand()%((int)upper_bound - (int)lower_bound) ) + (int)lower_bound )
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
	printf("RANK %d - # calculations: %lld\n", my_rank, N_split);

	for (int i = 0; i < N_split; i++) {
		result += function((double)randinrange(upper_bound, lower_bound) );
	}

	return ( ( (upper_bound - lower_bound) / N ) * result ); // still have to be divided by N
}	

void collect_results(double *result) {
	int my_rank, total_ranks;
	MPI_Comm_rank(MPI_COMM_WORLD,&my_rank); //gets my_rank for each processor
	MPI_Comm_size(MPI_COMM_WORLD,&total_ranks); //gets total amount of ranks
	int tag = 0;
	int dest=0;
	MPI_Status status;

	if(my_rank==0) { /* gather all messages from everyone else */
		int temp;
		for(int source=1; source < total_ranks; source){
			MPI_Recv(&temp, 1, MPI_INT, source, tag, MPI_COMM_WORLD, &status);
			*result += temp;
			printf("Received %f from RANK %d\n",*result, source );
		}
		printf("FINAL RESULT: %f\n",*result);
	}
	else {
		MPI_Send(result, 1, MPI_INT, dest, tag, MPI_COMM_WORLD);
	}



}