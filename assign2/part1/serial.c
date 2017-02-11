#include <stdio.h>
#include <stdlib.h>
#include <math.h>

void initialize_data(void);
double estimate_g(double, double, long long);

double function(double);

double randinrange(float,float);

// pow(x,y) returns x^y

int main (int argc, char *argv[]){

	if (argc != 4) {
        perror("ERROR: ./program <lowerbound> <upper_bound> <# of data points>");
        exit(-1);
    }
	float lower_bound	=	atof(argv[1]);
	float upper_bound	=	atof(argv[2]);
	long long	int	N	=	atof(argv[3]);

	printf("lower bound: %f, upper bound: %f, N: %lld\n", lower_bound, upper_bound, N );
	
	printf("RESULT: %f\n",estimate_g((double)lower_bound, (double)upper_bound, N) );
	//double result = ( ( 8 * sqrt( 2 * M_PI ) ) / ( pow( M_E, pow(2 * val, 2) ) ) )


	//srand(2);
	//printf("Hello: %d, %d, %d\n", rand(), rand(), rand());



}

double function(double var) {
	return ( ( 8 * sqrt( 2 * M_PI ) ) / ( pow( M_E, pow(2 * var, 2) ) ) ) ;
}

double randinrange(float HI, float LO) {
	//return ( ( rand()%((int)upper_bound - (int)lower_bound) ) + (int)lower_bound )
	return LO + (double)(rand()) / ( (double)(RAND_MAX/(HI-LO)));
}

void initialize_data(){

}

double estimate_g(double lower_bound, double upper_bound, long long N) {
	double result;

	for (long long i = 0; i < N; i++) {
		result += function((double)randinrange(upper_bound, lower_bound) );
	}

	return ( ( (upper_bound - lower_bound) / N ) * result );
}

