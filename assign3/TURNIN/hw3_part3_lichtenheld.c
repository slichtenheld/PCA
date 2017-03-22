#include <stdio.h>
#include <stdint.h>
#include <omp.h>
#include <math.h>

#include <stdlib.h>
#include "randNumGen.h"
#include "execTime.h"
 
#define IMAGEWIDTH 3840 //3840 
#define IMAGEHEIGHT 2160//2160

void mtrx_populate(int8_t* mtrx, int rows, int cols);
void mtrx_print(int8_t* mtrx, int rows, int cols);
void sobel_conv(int8_t* in_mtrx, int8_t* out_mtrx, int rows, int cols);

int xy2int(int x, int y, int cols);

int main(int argc, char * argv[]) {

	if(argc != 2) {
		perror("ERROR: ./program < numThreads > ");
		exit(EXIT_FAILURE);
	}
	// take in number of threads
	int numThreads = strtol(argv[1],NULL,10);


	// make 2k image 3840 x 2160 with 8 bits each, one for input, one for output
	int8_t* in_mtrx = malloc(IMAGEWIDTH*IMAGEHEIGHT*sizeof(int8_t));
	int8_t* sbl_mtrx = malloc(IMAGEWIDTH*IMAGEHEIGHT*sizeof(int8_t));

	// randomize input (fine for measuring performance since not real program)
	mtrx_populate(in_mtrx, IMAGEHEIGHT, IMAGEWIDTH);
	//mtrx_print(in_mtrx, IMAGEHEIGHT, IMAGEWIDTH);
	omp_set_num_threads(numThreads);

	startTimer();
	sobel_conv(in_mtrx, sbl_mtrx, IMAGEHEIGHT, IMAGEWIDTH);
	stopTimer();
	//mtrx_print(sbl_mtrx, IMAGEHEIGHT, IMAGEWIDTH);

	printf("EXECTIME: %f\n", getTimeSpent());
	// sobel filter 
}

void sobel_conv(int8_t* in_mtrx, int8_t* out_mtrx, int rows, int cols){
	
	// zero out output matrix edges
	for (int i = 0; i < cols; i++ ) { 
		out_mtrx[xy2int(i,0,cols)] = 0;
		out_mtrx[xy2int(i,rows-1,cols)] = 0;
	}
	for (int i = 1; i < rows-1; i++) { 
		out_mtrx[xy2int(0,i,cols)] = 0;
		out_mtrx[xy2int(cols-1,i,cols)] = 0;
	}

	int i, j;
	#pragma omp parallel shared(in_mtrx,out_mtrx) private(i,j)
	{
		#pragma omp for schedule(static)
		for (j = 1; j < rows-1; j++ ) {
			for (i = 1; i < cols-1; i++) {

				int gX = 0;
				gX += in_mtrx[ 		xy2int(	i-1,	j-1	,	cols)];
				gX += 2*in_mtrx[	xy2int(	i-1,	j 	,	cols)]; 
				gX += in_mtrx[		xy2int(	i-1,	j+1	,	cols)];

				gX -= in_mtrx[		xy2int(	i+1,	j-1	,	cols)];
				gX -= 2*in_mtrx[	xy2int(	i+1,	j 	,	cols)];
				gX -= in_mtrx[		xy2int(	i+1,	j+1	,	cols)];

				int gY = 0;
				gY = in_mtrx[ 		xy2int(	i-1,	j-1	,	cols)];
				gY += 2*in_mtrx[	xy2int(	i,		j-1	,	cols)]; 
				gY += in_mtrx[		xy2int(	i+1,	j-1	,	cols)];

				gY -= in_mtrx[		xy2int(	i-1,	j+1	,	cols)];
				gY -= 2*in_mtrx[	xy2int(	i,		j+1	,	cols)];
				gY -= in_mtrx[		xy2int(	i+1,	j+1	,	cols)];

				//printf("%d,%d\n",gX,gY );

				int temp = sqrt((double)(gX*gX+gY*gY));

				if (temp > 127) out_mtrx[xy2int(i,j,cols)] = 127;
				else if (temp < -128) out_mtrx[xy2int(i,j,cols)] = -128;
				else out_mtrx[xy2int(i,j,cols)] = temp;
				
				//out_mtrx[xy2int(i,j,cols)] = (int8_t)sqrt((double)(gX*gX+gY*gY));
			}
		}
	}
}	

void mtrx_populate(int8_t* mtrx, int rows, int cols){ // populates whole row, then moves to next row
	for(int i = 0; i < rows ; i++) {
		for (int j = 0; j < cols; j++ ){
			mtrx[i*cols + j] = randinrangeINT8(-128,127);
		}
	}
}

void mtrx_print(int8_t* mtrx, int rows, int cols){
	for(int i = 0; i < rows ; i++) {
		for (int j=0; j < cols ; j++) {
			printf("%d ", mtrx[i*cols + j]);
		}
		printf("\n");
	}
	printf("\n");
}

int xy2int(int x, int y, int cols){
	return y*cols+x;
}