#include "execTime.h"
#include <time.h>
#include <omp.h>

double begin;
double end;


void startTimer(){
	begin = omp_get_wtime();
}

void stopTimer(){
	end = omp_get_wtime();
}

double getTimeSpent(){
	return (double)(end - begin);
}
