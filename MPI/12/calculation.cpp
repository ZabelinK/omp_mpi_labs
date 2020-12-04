#include <stdio.h>
#include <mpi.h>
#include <stdexcept>
#include <vector>
#include <fstream>
#include <iterator>
#include <algorithm>
#include <iostream>

#define MAGIC_VALUE 52467

int main (int argc, char *argv[])
{
    if (argc != 3) {
        std::cerr << "Invalid number of parameters\n";
        return 1;
    }

    MPI_Status status;
    
    //Matrix dimetions
    int m = atoi(argv[1]), n = atoi(argv[2]);
    int rank = 0, size = 0, lines_number = 0, start_line = 0;;

    //Read vector data
    std::ifstream vector_file("./data/vector.txt");
    std::vector<int> vec{std::istream_iterator<int>(vector_file), std::istream_iterator<int>()};
    vector_file.close();

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    // Reading meta-data and part of matrix 
    std::ifstream data_file("./data/mapped_data/" + std::to_string(rank) + ".txt");
    
    data_file >> lines_number >> start_line;
    
    std::vector<int> data{std::istream_iterator<int>(data_file), std::istream_iterator<int>()};
    data_file.close();

    // Calculate part of resulted vector
    std::vector<int> res_data(lines_number);

    for (int n = 0; n < lines_number; ++n) {
        for (int i = 0; i < vec.size(); ++i) {
            res_data[n] += data[n * vec.size() + i] * vec[i];
        }
    }
    
    if (rank == 0) {
        // Gather all information together at the 0th process
        std::vector<int> complete_data(m);
        int count_of_lines = 0, start_from = 0; 

        // Copy it is own data to the result vector
        std::copy(res_data.begin(), res_data.end(), complete_data.begin());

        for (int i = 1; i < size; ++i) {
            MPI_Recv(&count_of_lines, 1, MPI_INT, i, 0, MPI_COMM_WORLD, &status);
            MPI_Recv(&start_from, 1, MPI_INT, i, 0, MPI_COMM_WORLD, &status);
            MPI_Recv(&complete_data[start_from], count_of_lines, MPI_INT, i, 0, MPI_COMM_WORLD, &status);
        }

        std::ofstream result_file("./data/result.txt");
        std::copy(complete_data.begin(), complete_data.end(), std::ostream_iterator<int>(result_file, "\n"));

    } else {
        //Send part of information to the 0th process
        MPI_Send(&lines_number, 1, MPI_INT, 0, 0, MPI_COMM_WORLD);
        MPI_Send(&start_line, 1, MPI_INT, 0, 0, MPI_COMM_WORLD);
        MPI_Send(&res_data[0], lines_number, MPI_INT, 0, 0, MPI_COMM_WORLD);
    }

    MPI_Finalize();
}