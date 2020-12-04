#include <stdio.h>
#include <mpi.h>
#include <stdexcept>
    
#define DATA_SIZE 1000

int main (int argc, char *argv[])
{
    int rank;
    int value;
    MPI_Status status;
    MPI_Init (&argc, &argv);
    MPI_Comm_rank (MPI_COMM_WORLD, &rank);

    int arr[DATA_SIZE] = {};

    MPI_Bcast (&arr, DATA_SIZE, MPI_INT, 0, MPI_COMM_WORLD); 

    MPI_Finalize();
}