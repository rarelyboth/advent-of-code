#include "matrix.h"

#include <stdlib.h>
#include <assert.h>

// region rMatrix_int64_t

#define T rMatrix_int64_t
#define Ti int64_t

struct T {
    size_t m;
    size_t n;
    Ti * data;
};

rMatrix_int64_t * r_matrix_new_int64_t(size_t m, size_t n) {
    assert(m > 0 && n > 0);

    T * matrix = malloc(sizeof(T));
    assert(matrix);

    matrix->m = m;
    matrix->n = n;
    matrix->data = calloc(m * n, sizeof(Ti));
    assert(matrix->data);

    return matrix;
}

Ti * r_matrix_at_int64_t(T * matrix, size_t m, size_t n) {
    assert(m >= 0 && m < matrix->m);
    assert(n >= 0 && n < matrix->n);
    assert(matrix && matrix->data);

    return matrix->data + m * matrix->n + n;
}

Ti r_matrix_get_int64_t(rMatrix_int64_t * matrix, size_t m, size_t n) {
    return *r_matrix_at_int64_t(matrix, m, n);
}

void r_matrix_set_int64_t(rMatrix_int64_t * matrix, size_t m, size_t n, int64_t value) {
    *r_matrix_at_int64_t(matrix, m, n) = value;
}

void r_matrix_free_int64_t(rMatrix_int64_t ** matrix) {
    free((*matrix)->data);
    free(*matrix);
}

size_t r_matrix_m_int64_t(T * matrix) {
    return matrix->m;
}

size_t r_matrix_n_int64_t(T * matrix) {
    return matrix->n;
}

#undef Ti
#undef T

// endregion rMatrix_int64_t