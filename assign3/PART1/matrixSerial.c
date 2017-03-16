/*
A00 A10 	B00 B10 B20		A00*B00+A10*B01 A00*B10+A10*B11 A00*B20+A10*B21 
A01 A11	 *	B01 B11 B21  =  A01*B00+A11*B01 A01*B10+A11*B11 A01*B20+A11*B21 
A02 A12 					A02*B00+A12*B01 A02*B10+A12*B11 A02*B20+A12*B21 

3x2 			2x3			 3x3
*/

#include <stdio.h>
#include <omp.h>

#include <stdlib.h>
#include "randNumGen.h"
#include "execTime.h"

void mtrx_populate(int* mtrx, int rows, int cols);
void mtrx_print(int* mtrx, int rows, int cols);
void mtrx_multiply(int* A, int* B, int* AB, int A_rows, int A_cols, int B_cols);

int main(int argc, char * argv[]){

	if (argc != 4) {
		perror("ERROR: ./program < A_rows > < A_cols > < B_cols >");
		exit(EXIT_FAILURE);
	}
	//printf("num of threads: %d\n", omp_get_num_threads());

	int A_rows = strtol(argv[1],NULL,10);
	int A_cols = strtol(argv[2],NULL,10);
	int B_rows = A_cols;
	int B_cols = strtol(argv[3],NULL,10);

	int* A_mtrx = malloc(A_rows*A_cols*sizeof(int));
	int* B_mtrx = malloc(B_rows*B_cols*sizeof(int));
	int* r_mtrx = malloc(A_rows*B_cols*sizeof(int));

	/* populate matrices */
	mtrx_populate(A_mtrx,A_rows,A_cols);
	mtrx_populate(B_mtrx,B_rows,B_cols);

	//printf("\n");

	//mtrx_print(A_mtrx,A_rows,A_cols);
	//mtrx_print(B_mtrx,B_rows,B_cols);
	//omp_set_num_threads(8);
	startTimer();
	mtrx_multiply(A_mtrx, B_mtrx, r_mtrx, A_rows, A_cols, B_cols);
	stopTimer();

	printf("EXECTIME: %f\n", getTimeSpent());

	//mtrx_print(r_mtrx,A_rows,B_cols);
	
	return 0;

}

void mtrx_populate(int* mtrx, int rows, int cols){ // populates whole row, then moves to next row
	for(int i = 0; i < rows ; i++) {
		for (int j = 0; j < cols; j++ ){
			mtrx[i*cols + j] = randinrange(0,65000);
			//printf("%d ", mtrx[i*cols + j] );
		}
	}
	//printf("\n");
}

void mtrx_print(int* mtrx, int rows, int cols){
	for(int i = 0; i < rows ; i++) {
		for (int j=0; j < cols ; j++) {
			printf("%d ", mtrx[i*cols + j]);
		}
		printf("\n");
	}
	printf("\n");
}

void mtrx_multiply(int* A, int* B, int* AB, int A_rows, int A_cols, int B_cols){
	int i, j, k;
	{
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

/*
A00 A10 	B00 B10 B20		A00*B00+A10*B01 A00*B10+A10*B11 A00*B20+A10*B21 
A01 A11	 *	B01 B11 B21  =  A01*B00+A11*B01 A01*B10+A11*B11 A01*B20+A11*B21 
A02 A12 					A02*B00+A12*B01 A02*B10+A12*B11 A02*B20+A12*B21 

3x2 			2x3			 3x3
*/