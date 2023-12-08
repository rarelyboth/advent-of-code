#include "array.h"

#include <assert.h>
#include <stdlib.h>
#include <inttypes.h>

#define T rArray_int64_t
#define Ti int64_t

struct T {
    size_t n;
    size_t size;
    Ti * array;
};

T rArrayNew_int64_t(size_t size) {
    assert(size >= 0);

    T array = calloc(1, sizeof(T));

    array->n = 0;
    array->size = size;
    array->array = calloc(size, sizeof(Ti));

    return array;
}

size_t rArrayLength_int64_t(T array) {
    assert(array);
    return array->n;
}

Ti rArrayGet_int64_t(T array, size_t i) {
    assert(array && array->array);
    assert(i >= 0 && i < array->n);

    return *(array->array + i);
}

Ti rArraySet_int64_t(T array, size_t i, Ti element) {
    assert(array && array->array);
    assert(i >= 0 && i < array->n);

    *(array->array + i) = element;
    return element;
}

Ti rArrayInsert_int64_t(T array, size_t i, Ti element) {
    assert(array && array->array);
    assert(i >=0 && i < array->n);

    rArrayShift_int64_t(array, i, 1);
    rArraySet_int64_t(array, i, element);

    return element;
}

void rArrayShift_int64_t(T array, size_t i, size_t n) {
    assert(array && array->array);
    assert(i >= 0 && i < array->n);
    assert(n > 0);

    if (array->n + n > array->size) {
        rArrayResize_int64_t(array, array->size * 2);
    }

    Ti * it = array->array + array->n;
    Ti * itPrev = it - 1;
    for (; itPrev >= array->array + i; --itPrev, --it) {
        *it = *itPrev;
        *itPrev = 0;
    }

    ++array->n;
}

void rArrayResize_int64_t(T array, size_t size) {
    assert(array && array->array);
    assert(size >= 0);

    if (size == 0) {
        rArrayFree_int64_t(array);
    } else {
        array->size = size;
        array->array = realloc(array->array, array->size * sizeof(Ti));
        assert(array->array);

        if (array->size <= array->n) {
            array->n = array->size;
        }
    }
}

Ti rArrayAppend_int64_t(T array, Ti element) {
    assert(array && array->array);

    if (array->n + 1 > array->size) {
        rArrayResize_int64_t(array, array->size * 2);
    }

    *(array->array + array->n) = element;
    ++(array->n);

    return element;
}

Ti rArrayPrepend_int64_t(T array, Ti element) {
    assert(array && array->array);

    rArrayShift_int64_t(array, 0, 1);
    rArraySet_int64_t(array, 0, element);

    return element;
}

void rArrayFree_int64_t(T array) {
    assert(array && array->array);

    free(array->array);
    free(array);
}

#undef Ti
#undef T

