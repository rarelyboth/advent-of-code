#include "rlib/file.h"
#include "rlib/string.h"

#include <string.h>
#include <stdlib.h>
#include <inttypes.h>

#define DIGITS "0123456789"

typedef struct {
    int64_t first;
    int64_t second;
} rPair;

rPair *rPairNew(int64_t first, int64_t second) {
    rPair *pair = calloc(1, sizeof(rPair));

    pair->first = first;
    pair->second = second;

    return pair;
}

void rPairFree(rPair *pair) {
    free(pair);
}

typedef struct {
    rPair *domain;
    rPair *codomain;
} rFunction;

rFunction *rFunctionNew(rPair *domain, rPair *codomain) {
    rFunction *function = calloc(1, sizeof(rFunction));

    function->domain = domain;
    function->codomain = codomain;

    return function;
}

rFunction *rFunctionNewFromRange(int64_t destination, int64_t source, int64_t range) {
    rPair * domain = rPairNew(source, source + range);
    rPair * codomain = rPairNew(destination, destination + range);

    return rFunctionNew(domain, codomain);
}

int64_t rFunctionApply(rFunction *function, int64_t input) {
    return input + (function->codomain - function->domain);
}

int64_t rFunctionUnapply(rFunction *function, int64_t input) {
    return input - (function->codomain - function->domain);
}

void rFunctionFree(rFunction *function) {
    rPairFree(function->domain);
    rPairFree(function->codomain);
    free(function);
}

typedef struct {
    rFunction **data;
    size_t n;
    size_t size;
} rMap;

rMap *rMapNew(size_t sizeHint) {
    rMap * array = calloc(1, sizeof(rMap));

    array->n = 0;
    array->size=sizeHint;
    array->data = calloc(sizeHint, sizeof(rFunction));

    return array;
}

void rMapAdd(rMap *map, rFunction *function) {
    if (map->n + 1 > map->size) {
        map->size *= 2;
        map->data = realloc(map->data, map->size * sizeof(rFunction));
        if (map->data == NULL) {
            fprintf(stderr, "Failed to add function to map!");
            exit(EXIT_FAILURE);
        }
    }

    *(map->data + map->n) = function;
    ++map->n;
}

void rMapFree(rMap *map) {
    for (int i = 0; i < map->n; ++i) {
        rFunctionFree(*(map->data + i));
    }
    free(map);
}

int64_t partOne();
int64_t partTwo();

int main() {
    printf("Part One: %" PRIi64 "\n", partOne());
    printf("Part Two: %" PRIi64 "\n", partTwo());
}

void rStringParseNumbers(char * const line, int64_t * const into) {
    char * it = strpbrk(line, DIGITS);
    for (int i = 0; it != NULL; ++i) {
        *(into + i) = strtol(it, &it, 10);

        it = strpbrk(it, DIGITS);
    }
}

int64_t partOne() {
    FILE *file = rFileOpen("input.txt");

    uint8_t const numSeeds = 20;
    uint8_t const bufferSize = 255;
    uint8_t const numMaps = 7;

    char buffer[bufferSize];
    int64_t * const seeds = calloc(numSeeds, sizeof(int64_t));
    rMap ** const maps = calloc(numMaps, sizeof(rMap));

    // Extract seeds, which we assume is on the first line
    {
        char * seedLine = fgets(buffer, bufferSize, file);

        char * const line = rStringCopyUntil(buffer, '\n');
        rStringParseNumbers(line, seeds);
    }

    // Now parse the maps
    {
        for (int i = -1; fgets(buffer, bufferSize, file) != NULL; ) {
            char * const line = rStringCopyUntil(buffer, '\n');
            if (strlen(line) == 0) {
                continue;
            } else if (strchr(line, ':') != NULL) {
                ++i;
                *(maps + i) = rMapNew(64);
                continue;
            }

            int64_t * const nums = calloc(3, sizeof(int64_t));
            rStringParseNumbers(line, nums);

            rFunction * function = rFunctionNewFromRange(nums[0], nums[1], nums[2]);

            rMapAdd(*(maps + i), function);
        }
    }

    fclose(file);

    //
    {

    }
}