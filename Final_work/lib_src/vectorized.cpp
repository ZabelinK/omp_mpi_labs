#include <iostream>
#include <algorithm>

extern "C" {

    int32_t* matrix_mul(int32_t* mat1, int32_t* mat2, unsigned int l, unsigned int m, unsigned int n) 
    {
        int32_t *new_mat = (int32_t*) calloc(l * n, sizeof(int32_t));

        for (unsigned int i = 0; i < l; ++i) {
            for (unsigned int r = 0; r < m; ++r) {
                int row_i = mat1[i * m + r];
                int32_t* line1 = &new_mat[i * n];
                int32_t* line2 = &mat2[r * n];
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