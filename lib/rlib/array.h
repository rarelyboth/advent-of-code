#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>

typedef struct rArray_int64_t rArray_int64_t;

extern rArray_int64_t * r_array_new_int64_t(size_t capacity);
extern size_t r_array_length_int64_t(rArray_int64_t * const array);
extern int64_t r_array_get_int64_t(rArray_int64_t * const array, size_t index);
extern void r_array_append_int64_t(rArray_int64_t * const array, int64_t value);
extern void r_array_resize_int64_t(rArray_int64_t * const array, size_t capacity);
extern void r_array_free_int64_t(rArray_int64_t ** const array);
extern int64_t * r_array_begin_int64_t(rArray_int64_t * const array);
extern int64_t * r_array_rbegin_int64_t(rArray_int64_t * const array);
extern int64_t * r_array_rend_int64_t(rArray_int64_t * const array);
extern int64_t * r_array_end_int64_t(rArray_int64_t * const array);
extern void r_array_print_int64_t(FILE * buffer, rArray_int64_t * const array);
extern rArray_int64_t * r_array_reverse_int64_t(rArray_int64_t * const array);