#pragma once

#include <stdlib.h>
#include <inttypes.h>

// region rMatrix_int64_t

#define T rMatrix_int64_t
#define Ti int64_t

typedef struct T T;

extern T * r_matrix_new_int64_t(size_t m, size_t n);
extern Ti * r_matrix_at_int64_t(T * matrix, size_t m, size_t n);
extern Ti r_matrix_get_int64_t(T * matrix, size_t m, size_t n);
extern void r_matrix_set_int64_t(T * matrix, size_t m, size_t n, Ti value);
extern void r_matrix_free_int64_t(T ** matrix);
extern size_t r_matrix_m_int64_t(T * matrix);
extern size_t r_matrix_n_int64_t(T * matrix);

#undef Ti
#undef T

// endregion rMatrix_int64_t