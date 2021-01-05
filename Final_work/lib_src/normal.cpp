#include <iostream>
#include <algorithm>
#include <cstdlib>

extern "C" {

    int* matrix_mul(int32_t *mat1, int32_t *mat2, unsigned int l, unsigned int m, unsigned int n) 
    {
        int32_t *new_mat = (int32_t*) calloc(l * n, sizeof(int32_t));

        for (unsigned int i = 0; i < l; ++i) {
            for (unsigned int j = 0; j < n; ++j) {
                for (unsigned int r = 0; r < m; ++r) {
                    new_mat[i * n + j] += mat1[i * m + r] * mat2[r * n + j];
                }
            }
        }

        return new_mat;
    }


    void free_matrix(int* matrix)
    {
        free(matrix);
    }

}

void __attribute__ ((constructor)) initLibrary(void) 
{}

void __attribute__ ((destructor)) cleanUpLibrary(void) 
{}