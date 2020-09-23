#include <iostream> 
#include <cmath> 
#include <vector>

#include <omp.h>

#define MAX_THREADS 4

bool is_prime_number(unsigned int number) 
{
    for (unsigned int i = 2; i < number; ++i) {
        if (number % i == 0) {
            return false;
        }
    }

    return true;
}

int main(int argc, char** argv) 
{
    if (argc != 2) {
        std::cerr << "Invalid count of parameters!\n";
        return 1;
    }

    size_t N = atol(argv[1]);

    omp_set_num_threads(std::min(omp_get_max_threads(), MAX_THREADS));

    std::vector<unsigned int> prime_values;

    for (size_t i = 0; i < N; ++i) {
        unsigned int number;
        std::cin >> number;
  
        if (is_prime_number(number)) {
            prime_values.push_back(number);
        }
    }
    
    unsigned int sum = 0;
    for (size_t i = 0; i < prime_values.size(); ++i) {
        sum += prime_values[i];
        std::cout << sum << '\n';
    }

    return 0;     
} 