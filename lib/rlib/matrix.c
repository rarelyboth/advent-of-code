#include "matrix.h"

#include <stdlib.h>
#include <assert.h>

// region rMatrix_Bit

#define T rMatrix_Bit
#define Ti uint64_t
#define Ti_SIZE 64

struct T {
    size_t m;
    size_t n;
    size_t n_Ti;
    Ti * data;
};

T * r_matrix_new_bit(size_t m, size_t n) {
    assert(m > 0 && n > 0);

    // Compute the number of Ti n_Ti necessary to store n bits
    size_t const n_Ti = n / Ti_SIZE + (n % Ti_SIZE != 0);

    T * matrix = (T *)malloc(sizeof(T));
    assert(matrix);

    matrix->m = m;
    matrix->n = n;
    matrix->n_Ti = n_Ti;
    matrix->data = (Ti *)calloc(m * n_Ti, sizeof(Ti));
    assert(matrix->data);

    return matrix;
}

Ti r_matrix_get_bit(T * matrix, size_t m, size_t n) {
    assert(matrix && matrix->data);
    assert(m > 0 && n > 0);
    assert(m < matrix->m && n < matrix->n);

    // Compute the Ti column being accessed
    size_t const n_ = n / Ti_SIZE;
    assert(n_ < matrix->n_Ti);

    // Compute the column within the Ti column being accessed
    size_t const n_n = n % Ti_SIZE;

    return matrix->data[(m * matrix->n_Ti) + n_] & 1 << n_n;
}

void r_matrix_push_row_bit(T * matrix, size_t m, Ti bit) {
    assert(matrix && matrix->data);
    assert(m > 0 && m < matrix->m);
    assert(bit == 1 || bit == 0);


}

// endregion rMatrix_Bit