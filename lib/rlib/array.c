#include "array.h"
#include "graph.h"

#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include <inttypes.h>

// region rArray_int64_t
#define T rArray_int64_t
#define Ti int64_t

struct T {
    size_t length;
    size_t capacity;
    Ti * data;
};

T * r_array_new_int64_t(size_t capacity) {
    T * array = (T *) malloc(sizeof(T));
    assert(array);

    array->length = 0;
    array->capacity = capacity;
    array->data = (Ti *) calloc(capacity, sizeof(Ti));
    assert(array->data);

    return array;
}

size_t r_array_length_int64_t(T * const array) {
    assert(array);

    return array->length;
}

Ti r_array_get_int64_t(T * const array, size_t index) {
    assert(array && array->data);
    assert(index < array->length);

    return array->data[index];
}

void r_array_append_int64_t(T * const array, Ti value) {
    assert(array && array->data);

    if (array->length >= array->capacity) {
        r_array_resize_int64_t(array, array->capacity * 2);
    }

    array->data[array->length++] = value;
}

void r_array_resize_int64_t(T * const array, size_t capacity) {
    assert(array && array->data);
    assert(capacity > 0);

    array->capacity = capacity;
    array->data = (Ti *) realloc(array->data, array->capacity * sizeof(Ti));
    assert(array->data);
}

void r_array_free_int64_t(T ** const array) {
    assert(array && (*array)->data);

    free((*array)->data);
    free(*array);
}

Ti * r_array_begin_int64_t(T * const array) {
    assert(array && array->data);

    return array->data;
}

Ti * r_array_end_int64_t(T * const array) {
    assert(array && array->data);

    return array->data + array->length;
}

Ti * r_array_rbegin_int64_t(T * const array) {
    assert(array && array->data);

    return r_array_end_int64_t(array) - 1;
}

Ti * r_array_rend_int64_t(T * const array) {
    assert(array && array->data);

    return r_array_begin_int64_t(array) - 1;
}

void r_array_print_int64_t(FILE * buffer, T * const array) {
    for (Ti * it = r_array_begin_int64_t(array); it != r_array_end_int64_t(array); ++it) {
        fprintf(buffer, "%" PRIi64, *it);
        if (it != r_array_end_int64_t(array) - 1) {
            fprintf(buffer, ", ");
        }
    }

    fprintf(buffer, "\n");
}

T * r_array_reverse_int64_t(T * const array) {
    T * reversed = r_array_new_int64_t(array->length);

    for (Ti * it = r_array_rbegin_int64_t(array); it != r_array_rend_int64_t(array); --it) {
        r_array_append_int64_t(reversed, *it);
    }

    return reversed;
}

void r_array_set_int64_t(T * const array, size_t index, Ti value) {
    assert(array && array->data);
    assert(index < array->length);

    array->data[index] = value;
}

Ti r_array_remove_int64_t(T * const array, size_t index) {
    assert(array && array->data);
    assert(index < array->length);

    Ti value = r_array_get_int64_t(array, index);

    Ti * it = array->data + index;
    Ti * it_next = it + 1;
    for (; it_next != r_array_end_int64_t(array); ++it, ++it_next) {
        *it = *it_next;
    }

    --array->length;

    return value;
}

Ti * r_array_find_int64_t(T * const array, Ti value) {
    Ti * end = r_array_end_int64_t(array);
    for (Ti * it = r_array_begin_int64_t(array); it != end; ++it) {
        if (*it == value) {
            return it;
        }
    }

    return end;
}

#undef Ti
#undef T
// endregion rArray_int64_t

// region rArray_string
#define T rArray_string
#define Ti char

struct T {
    size_t length;
    size_t capacity;
    Ti ** data;
};

T * r_array_new_string(size_t capacity) {
    T * array = (T *) malloc(sizeof(T));
    assert(array);

    array->length = 0;
    array->capacity = capacity;
    array->data = (Ti **) calloc(capacity, sizeof(Ti *));
    assert(array->data);

    return array;
}

size_t r_array_length_string(T * const array) {
    assert(array);

    return array->length;
}

Ti * r_array_get_string(T * const array, size_t index) {
    assert(array && array->data);

    return array->data[index];
}

void r_array_append_string(T * const array, Ti * value) {
    assert(array && array->data);

    if (array->length >= array->capacity) {
        r_array_resize_string(array, array->capacity * 2);
    }

    array->data[array->length++] = value;
}

void r_array_resize_string(T * const array, size_t capacity) {
    assert(array && array->data);
    assert(capacity > 0);

    array->capacity = capacity;
    array->data = (Ti **) realloc(array->data, array->capacity * sizeof(Ti *));
    assert(array->data);
}

void r_array_free_string(T ** const array) {
    assert(array && (*array)->data);

    for (Ti ** it = (*array)->data; it != r_array_end_string(*array); ++it) {
        free(*it);
    }

    free((*array)->data);
    free(*array);
}

Ti ** r_array_begin_string(T * const array) {
    assert(array && array->data);

    return array->data;
}

Ti ** r_array_end_string(T * const array) {
    assert(array && array->data);

    return array->data + array->length;
}

Ti ** r_array_rbegin_string(T * const array) {
    assert(array && array->data);

    return r_array_end_string(array) - 1;
}

Ti ** r_array_rend_string(T * const array) {
    assert(array && array->data);

    return r_array_begin_string(array) - 1;
}

void r_array_print_string(FILE * buffer, T * const array) {
    for (Ti ** it = r_array_begin_string(array); it != r_array_end_string(array); ++it) {
        fprintf(buffer, "%s\n", *it);
    }
}

T * r_array_reverse_string(T * const array) {
    T * reversed = r_array_new_string(array->length);

    for (Ti ** it = r_array_rbegin_string(array); it != r_array_rend_string(array); --it) {
        r_array_append_string(reversed, *it);
    }

    return reversed;
}

#undef Ti
#undef T
// endregion rArray_string

// region rArray_pair
#define T rArray_Pair
#define Ti Pair

struct T {
    size_t length;
    size_t capacity;
    Ti * data;
};

T * r_array_new_Pair(size_t capacity) {
    T * array = (T *) malloc(sizeof(T));
    assert(array);

    array->length = 0;
    array->capacity = capacity;
    array->data = (Ti *) calloc(capacity, sizeof(Ti));
    assert(array->data);

    return array;
}

size_t r_array_length_Pair(T * const array) {
    assert(array);

    return array->length;
}

Ti r_array_get_Pair(T * const array, size_t index) {
    assert(array && array->data);

    return array->data[index];
}

void r_array_append_Pair(T * const array, Ti value) {
    assert(array && array->data);

    if (array->length >= array->capacity) {
        r_array_resize_Pair(array, array->capacity * 2);
    }

    array->data[array->length++] = value;
}

void r_array_resize_Pair(T * const array, size_t capacity) {
    assert(array && array->data);
    assert(capacity > 0);

    array->capacity = capacity;
    array->data = (Ti *) realloc(array->data, array->capacity * sizeof(Ti));
    assert(array->data);
}

void r_array_free_Pair(T ** const array) {
    assert(array && (*array)->data);

    free((*array)->data);
    free(*array);
}

Ti * r_array_begin_Pair(T * const array) {
    assert(array && array->data);

    return array->data;
}

Ti * r_array_end_Pair(T * const array) {
    assert(array && array->data);

    return array->data + array->length;
}

Ti * r_array_rbegin_Pair(T * const array) {
    assert(array && array->data);

    return r_array_end_Pair(array) - 1;
}

Ti * r_array_rend_Pair(T * const array) {
    assert(array && array->data);

    return r_array_begin_Pair(array) - 1;
}

void r_array_print_Pair(FILE * buffer, T * const array) {
    for (Ti * it = r_array_begin_Pair(array); it != r_array_end_Pair(array); ++it) {
        fprintf(buffer, "%", *it);
        if (it != r_array_end_Pair(array) - 1) {
            fprintf(buffer, ", ");
        }
    }

    fprintf(buffer, "\n");
}

T * r_array_reverse_Pair(T * const array) {
    T * reversed = r_array_new_Pair(array->length);

    for (Ti * it = r_array_rbegin_Pair(array); it != r_array_rend_Pair(array); --it) {
        r_array_append_Pair(reversed, *it);
    }

    return reversed;
}
bool r_array_includes_Pair(rArray_Pair * const array, Pair value) {
    for (Ti * it = r_array_begin_Pair(array); it != r_array_end_Pair(array); ++it) {
        if (it->first == value.first && it->second == value.second) {
            return true;
        }
    }

    return false;
}

#undef Ti
#undef T
// endregion rArray_pair


#define T rArray_Lens
#define Ti Lens

struct T {
    size_t length;
    size_t capacity;
    Ti * data;
};

T * r_array_new_Lens(size_t capacity) {
    T * array = (T *) malloc(sizeof(T));
    assert(array);

    array->length = 0;
    array->capacity = capacity;
    array->data = (Ti *) calloc(capacity, sizeof(Ti));
    assert(array->data);

    return array;
}

size_t r_array_length_Lens(T * const array) {
    assert(array);

    return array->length;
}

Ti r_array_get_Lens(T * const array, size_t index) {
    assert(array && array->data);

    return array->data[index];
}

void r_array_append_Lens(T * const array, Ti value) {
    assert(array && array->data);

    if (array->length >= array->capacity) {
        r_array_resize_Lens(array, array->capacity * 2);
    }

    array->data[array->length++] = value;
}

void r_array_resize_Lens(T * const array, size_t capacity) {
    assert(array && array->data);
    assert(capacity > 0);

    array->capacity = capacity;
    array->data = (Ti *) realloc(array->data, array->capacity * sizeof(Ti));
    assert(array->data);
}

void r_array_free_Lens(T ** const array) {
    assert(array && (*array)->data);

    free((*array)->data);
    free(*array);
}

Ti * r_array_begin_Lens(T * const array) {
    assert(array && array->data);

    return array->data;
}

Ti * r_array_end_Lens(T * const array) {
    assert(array && array->data);

    return array->data + array->length;
}

Ti * r_array_rbegin_Lens(T * const array) {
    assert(array && array->data);

    return r_array_end_Lens(array) - 1;
}

Ti * r_array_rend_Lens(T * const array) {
    assert(array && array->data);

    return r_array_begin_Lens(array) - 1;
}

void r_array_print_Lens(FILE * buffer, T * const array) {
    for (Ti * it = r_array_begin_Lens(array); it != r_array_end_Lens(array); ++it) {
        fprintf(buffer, "[%s %" PRIi64 "]", it->label, it->focal_length);
        if (it != r_array_end_Lens(array) - 1) {
            fprintf(buffer, ", ");
        }
    }

    fprintf(buffer, "\n");
}

T * r_array_reverse_Lens(T * const array) {
    T * reversed = r_array_new_Lens(array->length);

    for (Ti * it = r_array_rbegin_Lens(array); it != r_array_rend_Lens(array); --it) {
        r_array_append_Lens(reversed, *it);
    }

    return reversed;
}

void r_array_set_Lens(T * const array, size_t index, Ti value) {
    assert(array && array->data);
    assert(index < array->length);

    array->data[index] = value;
}

Ti r_array_remove_Lens(T * const array, size_t index) {
    assert(array && array->data);
    assert(index < array->length);

    Ti value = r_array_get_Lens(array, index);

    Ti * it = array->data + index;
    Ti * it_next = it + 1;
    for (; it_next != r_array_end_Lens(array); ++it, ++it_next) {
        *it = *it_next;
    }

    --array->length;

    return value;
}

Ti * r_array_find_Lens(T * const array, Ti value) {
    Ti * end = r_array_end_Lens(array);
    for (Ti * it = r_array_begin_Lens(array); it != end; ++it) {
        if (strcmp(it->label, value.label) == 0) {
            return it;
        }
    }

    return end;
}

#undef Ti
#undef T

#define T rArray_rArc
#define Ti rGraphArc

struct T {
    size_t length;
    size_t capacity;
    Ti ** data;
};

T * r_array_new_rArc(size_t capacity) {
    T * array = (T *) malloc(sizeof(T));
    assert(array);

    array->length = 0;
    array->capacity = capacity;
    array->data = (Ti **) calloc(capacity, sizeof(Ti *));
    assert(array->data);

    return array;
}

size_t r_array_length_rArc(T * const array) {
    assert(array);

    return array->length;
}

Ti * r_array_get_rArc(T * const array, size_t index) {
    assert(array && array->data);

    return array->data[index];
}

void r_array_append_rArc(T * const array, Ti * value) {
    assert(array && array->data);

    if (array->length >= array->capacity) {
        r_array_resize_rArc(array, array->capacity * 2);
    }

    array->data[array->length++] = value;
}

void r_array_resize_rArc(T * const array, size_t capacity) {
    assert(array && array->data);
    assert(capacity > 0);

    array->capacity = capacity;
    array->data = (Ti **) realloc(array->data, array->capacity * sizeof(Ti *));
    assert(array->data);
}

void r_array_free_rArc(T ** const array) {
    assert(array && (*array)->data);

    free((*array)->data);
    free(*array);
}

Ti ** r_array_begin_rArc(T * const array) {
    assert(array && array->data);

    return array->data;
}

Ti ** r_array_end_rArc(T * const array) {
    assert(array && array->data);

    return array->data + array->length;
}

Ti ** r_array_rbegin_rArc(T * const array) {
    assert(array && array->data);

    return r_array_end_rArc(array) - 1;
}

Ti ** r_array_rend_rArc(T * const array) {
    assert(array && array->data);

    return r_array_begin_rArc(array) - 1;
}

void r_array_set_rArc(T * const array, size_t index, Ti * value) {
    assert(array && array->data);
    assert(index < array->length);

    array->data[index] = value;
}

Ti * r_array_remove_rArc(T * const array, size_t index) {
    assert(array && array->data);
    assert(index < array->length);

    Ti * value = r_array_get_rArc(array, index);

    Ti ** it = array->data + index;
    Ti ** it_next = it + 1;
    for (; it_next != r_array_end_rArc(array); ++it, ++it_next) {
        *it = *it_next;
    }

    --array->length;

    return value;
}

#undef Ti
#undef T
