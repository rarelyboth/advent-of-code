#include "array.h"

#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <inttypes.h>

struct rArray_int64_t {
    size_t length;
    size_t capacity;
    int64_t * data;
};

rArray_int64_t * r_array_new_int64_t(size_t capacity) {
    rArray_int64_t * array = (rArray_int64_t *)malloc(sizeof(rArray_int64_t));
    assert(array);

    array->length = 0;
    array->capacity = capacity;
    array->data = (int64_t *) calloc(capacity, sizeof(int64_t));
    assert(array->data);

    return array;
}

size_t r_array_length_int64_t(rArray_int64_t * const array) {
    assert(array);

    return array->length;
}

int64_t r_array_get_int64_t(rArray_int64_t * const array, size_t index) {
    assert(array && array->data);

    return array->data[index];
}

void r_array_append_int64_t(rArray_int64_t * const array, int64_t value) {
    assert(array && array->data);

    if (array->length >= array->capacity) {
        r_array_resize_int64_t(array, array->capacity * 2);
    }

    array->data[array->length++] = value;
}

void r_array_resize_int64_t(rArray_int64_t * const array, size_t capacity) {
    assert(array && array->data);
    assert(capacity > 0);

    array->capacity = capacity;
    array->data = (int64_t *)realloc(array->data, array->capacity * sizeof(int64_t));
    assert(array->data);
}

void r_array_free_int64_t(rArray_int64_t ** const array) {
    assert(array && (*array)->data);

    free((*array)->data);
    free(*array);
}

int64_t * r_array_begin_int64_t(rArray_int64_t * const array) {
    assert(array && array->data);

    return array->data;
}

int64_t * r_array_end_int64_t(rArray_int64_t * const array) {
    assert(array && array->data);

    return array->data + array->length;
}

int64_t * r_array_rbegin_int64_t(rArray_int64_t * const array) {
    assert(array && array->data);

    return r_array_end_int64_t(array) - 1;
}

int64_t * r_array_rend_int64_t(rArray_int64_t * const array) {
    assert(array && array->data);

    return r_array_begin_int64_t(array) - 1;
}

void r_array_print_int64_t(FILE * buffer, rArray_int64_t * const array) {
    for (int64_t * it = r_array_begin_int64_t(array); it != r_array_end_int64_t(array); ++it) {
        fprintf(buffer, "%" PRIi64, *it);
        if (it != r_array_end_int64_t(array) - 1) {
            fprintf(buffer, ", ");
        }
    }

    fprintf(buffer, "\n");
}

rArray_int64_t * r_array_reverse_int64_t(rArray_int64_t * const array) {
    rArray_int64_t * reversed = r_array_new_int64_t(array->length);

    for (int64_t * it = r_array_rbegin_int64_t(array); it != r_array_rend_int64_t(array); --it) {
        r_array_append_int64_t(reversed, *it);
    }

    return reversed;
}

