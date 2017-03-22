#include <stdlib.h>
#include <mpi.h>

int main(int argc, char **argv)
{   
    float **array;
    int rank,size,i,j;

    MPI_Init(&argc,&argv);
    MPI_Comm_rank(MPI_COMM_WORLD,&rank);
    MPI_Comm_size(MPI_COMM_WORLD,&size);

    array = (float **)malloc(100*sizeof(float));

    if(rank==0)
    {
        for(i=0;i<10;i++)
            for(j=0;j<10;j++)
                array[i][j]=i+j;
    }
    MPI_Bcast(array,10*10,MPI_FLOAT,0,MPI_COMM_WORLD);
    MPI_Finalize();
}