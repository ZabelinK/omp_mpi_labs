#include <stdio.h>
#include <mpi.h>
#include <stdexcept>
    
#define DATA_SIZE 1000

int main (int argc, char *argv[])
{
    int rank, size;
    int value;
    MPI_Status status;
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    int arr[DATA_SIZE] = {};

    if (rank == 0) {
        for (int i = 1; i < size; ++i) {
            MPI_Send(&arr, DATA_SIZE, MPI_INT, i, 0, MPI_COMM_WORLD);
        }
    } else {
        MPI_Recv(&arr, DATA_SIZE, MPI_INT, 0, 0, MPI_COMM_WORLD, &status);
    }

    MPI_Finalize();
}