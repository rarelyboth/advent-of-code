#pragma once

#include <stdlib.h>
#include <inttypes.h>

#define T rHashTable
#define Ti int64_t
#define Tk char *

typedef struct T * T;

extern T rHashTableNew_int64_t(size_t size, int8_t comparator(const Tk x, const Tk y), uint64_t hash(Tk key));
extern size_t rHashTableLength_int64_t(T hashTable);
extern Ti rHashTableAdd_int64_t(T hashTable, const Tk key, const Ti value);
extern Ti rHashTableGet_int64_t(T hashTable, const Tk key);
extern void rHashTableFree_int64_t(T hashTable);

#undef Tk
#undef Ti
#undef T