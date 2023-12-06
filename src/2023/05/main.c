#include "rlib/file.h"
#include "rlib/string.h"

#include <string.h>
#include <stdlib.h>
#include <inttypes.h>
#include <stdbool.h>
#include <assert.h>

#define DIGITS "0123456789"

typedef struct {
    int64_t size;
    int64_t elements;
    int64_t *set;
} rSet;

rSet *rSetNew(int64_t sizeHint) {
    rSet *set = malloc(sizeof(rSet));

    set->size = sizeHint;
    set->elements = 0;
    set->set = calloc(sizeHint, sizeof(int64_t));

    return set;
}

void rSetAdd(rSet *set, int64_t element) {
    if (set->elements == set->size) {
        set->size *= 2;
        set->set = realloc(set->set, set->size * sizeof(int64_t));
        assert(set->set != NULL);
    }

    bool inSet = false;
    for (int i = 0; i < set->elements; ++i) {
        if (*(set->set + i) == element) {
            inSet = true;
            break;
        }
    }

    if (!inSet) {
        *(set->set + set->elements) = element;
        ++(set->elements);
    }
}

bool rSetIncludes(rSet *set, int64_t element) {
    for (int i = 0; i < set->elements; ++i) {
        if (*(set->set) == element) {
            return true;
        }
    }

    return false;
}

typedef struct {
    int64_t start;
    int64_t end;
} rRange;

rRange *rRangeNew(int64_t first, int64_t second) {
    rRange *range = calloc(1, sizeof(rRange));

    range->start = first;
    range->end = second;

    return range;
}

bool rRangeContains(rRange * range, int64_t element) {
    return range->start <= element && element < range->end;
}

void rRangeFree(rRange *range) {
    free(range);
}

typedef struct {
    int64_t size;
    int64_t elements;
    rRange **set;
} rRangeSet;

rRangeSet *rRangeSetNew(int64_t sizeHint) {
    rRangeSet *set = malloc(sizeof(rRangeSet));

    set->size = sizeHint;
    set->elements = 0;
    set->set = calloc(sizeHint, sizeof(rRange));

    return set;
}

bool rRangeSetIncludes(rRangeSet *set, rRange * element) {
    for (int i = 0; i < set->elements; ++i) {
        if (*(set->set) == element) {
            return true;
        }
    }

    return false;
}

void rRangeSetAdd(rRangeSet *set, rRange * element) {
    if (set->elements == set->size) {
        set->size *= 2;
        set->set = realloc(set->set, set->size * sizeof(rRange));
        assert(set->set != NULL);
    }

    if (!rRangeSetIncludes(set, element)) {
        *(set->set + set->elements) = element;
        ++(set->elements);
    }
}

typedef struct {
    rRange *domain;
    rRange *codomain;
} rFunction;

rFunction *rFunctionNew(rRange *domain, rRange *codomain) {
    rFunction *function = calloc(1, sizeof(rFunction));

    function->domain = domain;
    function->codomain = codomain;

    return function;
}

rFunction *rFunctionNewFromRange(int64_t destination, int64_t source, int64_t range) {
    rRange * domain = rRangeNew(source, source + range);
    rRange * codomain = rRangeNew(destination, destination + range);

    return rFunctionNew(domain, codomain);
}

int64_t rFunctionApply(rFunction *function, int64_t input) {
    return input + (function->codomain->start - function->domain->start);
}

int64_t rFunctionUnapply(rFunction *function, int64_t input) {
    return input - (function->codomain->start - function->domain->start);
}

void rFunctionFree(rFunction *function) {
    rRangeFree(function->domain);
    rRangeFree(function->codomain);
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

int64_t rMapApplyFunction(rMap *map, int64_t input) {
    for (int i = 0; i < map->n; ++i) {
        rFunction * function = *(map->data + i);

        if (rRangeContains(function->domain, input)) {
            return rFunctionApply(function, input);
        }
    }

    return input;
}

int64_t rMapUnapplyFunction(rMap *map, int64_t input) {
    for (int i = 0; i < map->n; ++i) {
        rFunction * function = *(map->data + i);

        if (rRangeContains(function->codomain, input)) {
            return rFunctionUnapply(function, input);
        }
    }

    return input;
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

int64_t partOne() {
    FILE *file = rFileOpen("input.txt");

    uint8_t const numSeeds = 20;
    uint8_t const bufferSize = 255;
    uint8_t const numMaps = 7;

    char buffer[bufferSize];
    int64_t * const seeds = calloc(numSeeds, sizeof(int64_t));
    rMap ** const maps = calloc(numMaps, sizeof(rMap));

    // Extract seeds, which we assume is on the start line
    {
        char * seedLine = fgets(buffer, bufferSize, file);

        char * const line = rStringCopyUntil(buffer, '\n');
        rStringParseNumbers(line, seeds);
    }

    // Now parse the maps
    {
        for (int i = -1; fgets(buffer, bufferSize, file) != NULL; ) {
            char * const line = rStringCopyUntil(buffer, '\n');
            if (*line == '\n') {
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

    // Calculate the min location of all seeds to check
    int64_t min = INT64_MAX;
    {
        for (int i = 0; i < numSeeds; ++i) {
            int64_t seed = *(seeds + i);

            int64_t output = seed;
            for (int j = 0; j < numMaps; ++j) {
                rMap * map = *(maps + j);

                output = rMapApplyFunction(map, output);
            }

            if (output < min) {
                min = output;
            }
        }
    }

    return min;
}

void rStringParseSeeds(char const * const line, rRangeSet * into) {
    char * it = strpbrk(line, "0123456789");
    while (it != NULL) {

        int64_t first = strtol(it, &it, 10);
        it = strpbrk(it, "0123456789");
        int64_t second = strtol(it, &it, 10);

        rRange * range = rRangeNew(first, first + second);

        rRangeSetAdd(into, range);

        it = strpbrk(it, "0123456789");
    }
}

int64_t partTwo() {
    FILE *file = rFileOpen("input.txt");

    uint8_t const numSeeds = 20;
    uint8_t const bufferSize = 255;
    uint8_t const numMaps = 7;

    char buffer[bufferSize];
    rRangeSet * seeds = rRangeSetNew(numSeeds);
    rMap ** const maps = calloc(numMaps, sizeof(rMap));

    // Extract seed ranges, which we assume is on the start line
    {
        char * seedLine = fgets(buffer, bufferSize, file);

        char * const line = rStringCopyUntil(buffer, '\n');
        rStringParseSeeds(line, seeds);
    }

    // Now parse the maps
    {
        for (int i = -1; fgets(buffer, bufferSize, file) != NULL; ) {
            char * const line = rStringCopyUntil(buffer, '\n');
            if (*line == '\n') {
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

    // Calculate the optimal set of seeds to check
    rSet * potentialSeeds = rSetNew(64);
    {
        for (int i = numMaps - 1; i > 0; --i) {
            rMap * map = *(maps + i);

            for (int f = 0; f < map->n; ++f) {
                rFunction * function = *(map->data + f);

                int64_t domainStart = function->domain->start;
                int64_t domainEnd = function->domain->end;
                for (int j = i - 1; j >= 0; --j) {
                    rMap * prevMap = *(maps + j);

                    domainStart = rMapUnapplyFunction(prevMap, domainStart);
                    domainEnd = rMapUnapplyFunction(prevMap, domainEnd);
                }

                rSetAdd(potentialSeeds, domainStart);
                rSetAdd(potentialSeeds, domainEnd);
            }
        }
    }

    // Add the seed ranges to the list of potential seeds
    {
        for (int i = 0; i < seeds->elements; ++i) {
            rRange * seedRange = *(seeds->set + i);
            rSetAdd(potentialSeeds, seedRange->start);
            rSetAdd(potentialSeeds, seedRange->end);
        }
    }

    // Calculate the min location of all seeds to check
    int64_t min = INT64_MAX;
    {
        for (int i = 0; i < potentialSeeds->elements; ++i) {
            int64_t potentialSeed = *(potentialSeeds->set + i);

            for (int j = 0; j < seeds->elements; ++j) {
                rRange * seedRange = *(seeds->set + j);

                if (rRangeContains(seedRange, potentialSeed)) {
                    int64_t output = potentialSeed;
                    for (int k = 0; k < numMaps; ++k) {
                        rMap * map = *(maps + k);

                        output = rMapApplyFunction(map, output);
                    }

                    if (output < min) {
                        min = output;
                    }
                }
            }
        }
    }

    return min;
}