#pragma once

#include <stdlib.h>
#include <inttypes.h>

// region rMatrix_Bit

#define T rMatrix_Bit
#define Ti uint64_t

typedef struct T T;

extern T * r_matrix_new_bit(size_t m, size_t n);
extern Ti r_matrix_get_bit(T * matrix, size_t m, size_t n);
extern void r_matrix_push_row_bit(T * matrix, size_t m, Ti bit);

#undef Ti
#undef T

// endregion rMatrix_Bit