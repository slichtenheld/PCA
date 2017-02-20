
#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

void initialize_data(int *A, int *Ap, int* B, int N);
void print2DArray(int *A, int rows, int cols);
int randinrange(int,int);
void scatter_data(int *A, int *B, int N);
void    mask_operation(int  *A, int N,  int *Ap);   
void    gather_results(int  *Ap,    int N); 

int main(int argc, char *argv[])
{
    int rank, num_tasks;     // for storing this process' rank, and the number of processes
    int *sendcounts;    // array describing how many elements to send to each process
    int *displs;        // array describing the displacements where each segment begins

    int rem; // elements remaining after division among processes
    int sum = 0;                // Sum of counts. Used to calculate displacements
    int rec_buf[100];          // buffer where the received data should be stored

   if (argc != 2) {
        perror("ERROR: ./program <N: for NxN matrix>");
        return(-1);
    }
    

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &num_tasks);

    int N   =   atof(argv[1]);
    rem = (N*N)%num_tasks;

    int *data;

    if (rank==0){ // only initialize data in main task
        data = malloc(N*N*sizeof(int));
        initialize_data(data,NULL,NULL,N); 
    }
    

    sendcounts = malloc(sizeof(int)*num_tasks);
    displs = malloc(sizeof(int)*num_tasks);

    

    // divide the data among processes as described by sendcounts and displs
    scatter_data(data,NULL,&rec_buf,N);    

    // print what each process received
    printf("%d: ", rank);
    for (int i = 0; i < sendcounts[rank]; i++) {
        printf("%d\t", rec_buf[i]);
    }
    printf("\n");

    MPI_Finalize();

    free(sendcounts);
    free(displs);

    return 0;

}

void scatter_data(int *A, int *B, int N){
    int rank, num_tasks;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &num_tasks);
    // calculate send counts and displacements
    for (int i = 0; i < num_tasks; i++) {
        sendcounts[i] = (N*N)/num_tasks;
        if (rem > 0) {
            sendcounts[i]++;
            rem--;
        }

        displs[i] = sum;
        sum += sendcounts[i];
    }

    // print calculated send counts and displacements for each process
    if (rank == 0) {
        for (int i = 0; i < num_tasks; i++) {
            printf("sendcounts[%d] = %d\tdispls[%d] = %d\n", i, sendcounts[i], i, displs[i]);
        }
    }

    MPI_Scatterv(data, sendcounts, displs, MPI_INT, &rec_buf, 100, MPI_INT, 0, MPI_COMM_WORLD);
}

void initialize_data(int *A, int *Ap, int* B, int N){
    int my_rank,num_tasks;
    MPI_Comm_rank(MPI_COMM_WORLD,&my_rank);
    MPI_Comm_size(MPI_COMM_WORLD,&num_tasks);
    
    if (!my_rank){ // master node

        printf("leftovers: %d\n", (N*N)%num_tasks);

        for(int i = 0; i < N*N; i++){
            A[i] = randinrange(0,10000);
        //    printf("%d\n",A[i]);
        }


        print2DArray(A,N,N);
        //print2DArray(B,N);
        
    }
    else{
        //B = malloc( (N*N)/num_tasks * sizeof(int) );
    }
}

void print2DArray(int *A, int rows, int cols){
    for(int j = 0; j < rows; j++){
        for (int i = 0; i < cols; i++){
            printf("%d ", A[j*cols+i]);
        }
        printf("\n"); // new line
    }
}

int randinrange(int LO, int HI) {
    return LO + (rand()) / ( (RAND_MAX/(HI-LO)));
}