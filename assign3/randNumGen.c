#include "randNumGen.h"
#include <stdlib.h>

int randinrange(int LO, int HI) {
        return LO + ( (rand() ) / (RAND_MAX/(HI-LO) ) );
}