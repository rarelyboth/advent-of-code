#pragma once

#include <inttypes.h>

#define T rArray_int64_t
#define Ti int64_t

typedef struct T * T;

extern T rArrayNew_int64_t(size_t length);
extern size_t rArrayLength_int64_t(T array);
extern Ti rArrayGet_int64_t(T array, size_t i);
extern Ti rArraySet_int64_t(T array, size_t i, Ti element);
extern Ti rArrayInsert_int64_t(T array, size_t i, Ti element);
extern void rArrayShift_int64_t(T array, size_t i, size_t n);
extern void rArrayResize_int64_t(T array, size_t size);
extern Ti rArrayAppend_int64_t(T array, Ti element);
extern Ti rArrayPrepend_int64_t(T array, Ti element);
extern void rArrayFree_int64_t(T array);

#undef Ti
#undef T
