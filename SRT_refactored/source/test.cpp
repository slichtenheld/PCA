#include <stdio.h>

int main()
{
	int x[10000];
	for(int i = 0; i < 10000; i++)
		x[i%1000]++;
	return 0;
}
