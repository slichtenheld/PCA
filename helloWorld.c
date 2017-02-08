#include "mpi.h"
#include <stdio.h>
#include <string.h>

int main(int argc, char **argv) {
	
	int my_rank,total_ranks;
	int source,dest;
 	int tag=0;
 	char mesg[100];
 	MPI_Status status;

 	MPI_Init(NULL,NULL);
 	MPI_Comm_rank(MPI_COMM_WORLD,&my_rank);
 	MPI_Comm_size(MPI_COMM_WORLD,&total_ranks);

	if(my_rank!=0) {
		sprintf(mesg,"Greetings from %d!",my_rank);
		dest=0;
		MPI_Send(mesg,strlen(mesg)+1,MPI_CHAR,dest,tag,MPI_COMM_WORLD);
	}
	else {
		printf("Rank %d starting and total_ranks is %d\n",my_rank,total_ranks);
		for(source=1;source<total_ranks;source++){
			MPI_Recv(&mesg,100,MPI_CHAR,source,tag,MPI_COMM_WORLD,&status);
			printf("%s\n",mesg);
		}
	}
	MPI_Finalize();
	return 0;
}