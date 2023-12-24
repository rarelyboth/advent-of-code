#pragma once

#include "graph.h"

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <inttypes.h>

// region rArray_int64_t
#define T rArray_int64_t
#define Ti int64_t

typedef struct T T;

extern T * r_array_new_int64_t(size_t capacity);
extern size_t r_array_length_int64_t(T * const array);
extern Ti r_array_get_int64_t(T * const array, size_t index);
extern void r_array_append_int64_t(T * const array, Ti value);
extern void r_array_resize_int64_t(T * const array, size_t capacity);
extern void r_array_free_int64_t(T ** const array);
extern Ti * r_array_begin_int64_t(T * const array);
extern Ti * r_array_rbegin_int64_t(T * const array);
extern Ti * r_array_rend_int64_t(T * const array);
extern Ti * r_array_end_int64_t(T * const array);
extern void r_array_print_int64_t(FILE * buffer, T * const array);
extern T * r_array_reverse_int64_t(T * const array);
extern void r_array_set_int64_t(T * const array, size_t index, Ti value);
extern Ti r_array_remove_int64_t(T * const array, size_t index);
extern Ti * r_array_find_int64_t(T * const array, Ti value);

#undef Ti
#undef T
// endregion rArray_int64_t

// region rArray_string
#define T rArray_string
#define Ti char

typedef struct T T;

extern T * r_array_new_string(size_t capacity);
extern size_t r_array_length_string(T * const array);
extern Ti * r_array_get_string(T * const array, size_t index);
extern void r_array_append_string(T * const array, Ti * value);
extern void r_array_resize_string(T * const array, size_t capacity);
extern void r_array_free_string(T ** const array);
extern Ti ** r_array_begin_string(T * const array);
extern Ti ** r_array_rbegin_string(T * const array);
extern Ti ** r_array_rend_string(T * const array);
extern Ti ** r_array_end_string(T * const array);
extern void r_array_print_string(FILE * buffer, T * const array);
extern T * r_array_reverse_string(T * const array);

#undef Ti
#undef T
// endregion rArray_string

// region rArray_Pair
#define T rArray_Pair
#define Ti Pair

typedef struct {
    int64_t first;
    int64_t second;
} Pair;

typedef struct T T;

extern T * r_array_new_Pair(size_t capacity);
extern size_t r_array_length_Pair(T * const array);
extern Ti r_array_get_Pair(T * const array, size_t index);
extern void r_array_append_Pair(T * const array, Ti value);
extern void r_array_resize_Pair(T * const array, size_t capacity);
extern void r_array_free_Pair(T ** const array);
extern Ti * r_array_begin_Pair(T * const array);
extern Ti * r_array_rbegin_Pair(T * const array);
extern Ti * r_array_rend_Pair(T * const array);
extern Ti * r_array_end_Pair(T * const array);
extern void r_array_print_Pair(FILE * buffer, T * const array);
extern T * r_array_reverse_Pair(T * const array);
extern bool r_array_includes_Pair(T * const array, Ti value);

#undef Ti
#undef T
// endregion rArray_Pair

// region rArray_Lens
#define T rArray_Lens
#define Ti Lens

typedef struct {
    char * label;
    int64_t focal_length;
} Lens;

typedef struct T T;

extern T * r_array_new_Lens(size_t capacity);
extern size_t r_array_length_Lens(T * const array);
extern Ti r_array_get_Lens(T * const array, size_t index);
extern void r_array_append_Lens(T * const array, Ti value);
extern void r_array_resize_Lens(T * const array, size_t capacity);
extern void r_array_free_Lens(T ** const array);
extern Ti * r_array_begin_Lens(T * const array);
extern Ti * r_array_rbegin_Lens(T * const array);
extern Ti * r_array_rend_Lens(T * const array);
extern Ti * r_array_end_Lens(T * const array);
extern void r_array_print_Lens(FILE * buffer, T * const array);
extern T * r_array_reverse_Lens(T * const array);
extern void r_array_set_Lens(T * const array, size_t index, Ti value);
extern Ti r_array_remove_Lens(T * const array, size_t index);
extern Ti * r_array_find_Lens(T * const array, Ti value);

#undef Ti
#undef T
// endregion rArray_Lens

// region rArray_rArc
#define T rArray_rArc
#define Ti rGraphArc

typedef struct T T;

extern T * r_array_new_rArc(size_t capacity);
extern size_t r_array_length_rArc(T * const array);
extern Ti * r_array_get_rArc(T * const array, size_t index);
extern void r_array_append_rArc(T * const array, Ti * value);
extern void r_array_resize_rArc(T * const array, size_t capacity);
extern void r_array_free_rArc(T ** const array);
extern Ti ** r_array_begin_rArc(T * const array);
extern Ti ** r_array_rbegin_rArc(T * const array);
extern Ti ** r_array_rend_rArc(T * const array);
extern Ti ** r_array_end_rArc(T * const array);
extern void r_array_set_rArc(T * const array, size_t index, Ti * value);

#undef Ti
#undef T
// endregion rArray_rArc
