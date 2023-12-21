#include "rlib/matrix.h"

#include <stdio.h>
#include <inttypes.h>

int main() {
    rMatrix_int64_t * matrix = r_matrix_new_int64_t(256, 32);

    for (size_t m = 0; m < r_matrix_m_int64_t(matrix); ++m) {
        for (size_t n = 0; n < r_matrix_n_int64_t(matrix); ++n) {
            r_matrix_set_int64_t(matrix, m, n, 1000 + (m * n));
        }
    }

    for (size_t m = 0; m < r_matrix_m_int64_t(matrix); ++m) {
        for (size_t n = 0; n < r_matrix_n_int64_t(matrix); ++n) {
            printf("%" PRIi64 "\t", r_matrix_get_int64_t(matrix, m, n));
        }
        printf("\n");
    }

    r_matrix_free_int64_t(&matrix);

    return 0;
}