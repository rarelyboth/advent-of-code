#include "rlib/array.h"

#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>

int main() {
    rArray_int64_t array = rArrayNew_int64_t(4);
    rArrayAppend_int64_t(array, 1);
    rArrayAppend_int64_t(array, 2);
    rArrayAppend_int64_t(array, 3);
    rArrayAppend_int64_t(array, 4);
    rArrayAppend_int64_t(array, 5);
    rArrayInsert_int64_t(array, 2, 7);
    rArrayPrepend_int64_t(array, 0);

    for (size_t i = 0; i < rArrayLength_int64_t(array); ++i) {
        fprintf(stderr, "%zu: %" PRIi64 "\n", i, rArrayGet_int64_t(array, i));
    }

    rArrayFree_int64_t(array);
}