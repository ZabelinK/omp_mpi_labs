#include <iostream>
#include <algorithm>
#include <cstdlib>

extern "C" {

    typedef int aint __attribute__ ((__aligned__(32)));
    int* matrix_mul(aint* mat1, aint* mat2, unsigned int l, unsigned int m, unsigned int n) 
    {
        aint *new_mat = (aint*) calloc(l * n, sizeof(aint));

        for (unsigned int i = 0; i < l; ++i) {
            for (unsigned int r = 0; r < m; ++r) {
                int row_i = mat1[i * m + r];
                int* line1 = &new_mat[i * n];
                int* line2 = &mat2[r * n];
                for (unsigned int j = 0; j < n; ++j) {
                    line1[j] += row_i * line2[j];
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