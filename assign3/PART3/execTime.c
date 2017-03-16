
#include "execTime.h"
#include <time.h>

clock_t begin;
clock_t end;


void startTimer(){
	begin = clock();
}

void stopTimer(){
	end = clock();
}

double getTimeSpent(){
	return (double)(end - begin) / CLOCKS_PER_SEC;
}
