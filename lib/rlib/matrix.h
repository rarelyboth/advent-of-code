#pragma once

#include "graph.h"

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

// region rMatrix_rGraphNode

#define T rMatrix_rGraphNode
#define Ti rGraphNode

typedef struct T T;

extern T * r_matrix_new_rGraphNode(size_t m, size_t n);
extern Ti ** r_matrix_at_rGraphNode(T * matrix, size_t m, size_t n);
extern Ti * r_matrix_get_rGraphNode(T * matrix, size_t m, size_t n);
extern void r_matrix_set_rGraphNode(T * matrix, size_t m, size_t n, Ti * value);
extern void r_matrix_free_rGraphNode(T ** matrix);
extern size_t r_matrix_m_rGraphNode(T * matrix);
extern size_t r_matrix_n_rGraphNode(T * matrix);

#undef Ti
#undef T

// endregion rMatrix_rGraphNode