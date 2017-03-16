#include "randNumGen.h"
#include <stdlib.h>

int randinrangeINT(int LO, int HI) {
        return LO + ( (rand() ) / (RAND_MAX/(HI-LO) ) );
}

double randinrangeDOUBLE(float HI, float LO) {
	return LO + (double)(rand()) / ( (double)(RAND_MAX/(HI-LO)));
}