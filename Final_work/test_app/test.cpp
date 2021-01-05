#include <algorithm>
#include <iterator>
#include <iostream>
#include <memory>
#include <ctime>
#include <functional>
#include <random>
#include <chrono>

#include "my_math.hpp"

inline std::chrono::milliseconds getCurrentTime()
{
    return std::chrono::duration_cast< std::chrono::milliseconds >(std::chrono::system_clock::now().time_since_epoch());
}

template<class Iterator>
void fillRandom(Iterator arr, size_t n)
{
    static std::random_device rd;  //Will be used to obtain a seed for the random number engine
    static std::mt19937 gen(rd()); //Standard mersenne_twister_engine seeded with rd()
    static std::uniform_int_distribution<> distrib(-10, 10);
    
    std::generate(arr, arr + n, std::bind(distrib, gen));
}

template<class Iterator>
void print_matrix(Iterator mat, size_t l, size_t n)
{
    for (size_t i = 0; i < l; ++i) {
        std::copy((mat + i * n), (mat + (i + 1) * n), std::ostream_iterator<int>(std::cout, "\t"));
        std::cout << '\n';
    }
}

int main() 
{
    constexpr int l = 7000, m = 100, n = 5000;

    std::srand(std::time(0));

    std::unique_ptr<int> mat1(new int[l * m]);
    std::unique_ptr<int> mat2(new int[m * n]);

    fillRandom(mat1.get(), l * m);
    fillRandom(mat2.get(), m * n);

    //print_matrix(mat1.get(), l, m);
    std::cout << "\n\n";
    //print_matrix(mat2.get(), m, n);

    std::cout << "\n\n";

    std::chrono::milliseconds ms = getCurrentTime();

    int* new_mat = matrix_mul(mat1.get(), mat2.get(), l, m, n);

    ms = getCurrentTime() - ms;

    //print_matrix(new_mat, l, n);

    free_matrix(new_mat);

    std::cout << "\n\n Time Result : " << ms.count() << " ms\n\n";

    return 0;
}