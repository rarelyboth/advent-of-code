#include "rlib/file.h"
#include "rlib/string.h"

#include <stdlib.h>
#include <inttypes.h>
#include <string.h>
#include <assert.h>
#include <stdbool.h>
#include <limits.h>

#define DIGITS "0123456789"

typedef struct {
    int64_t from;
    int64_t range;
    int64_t offset;
} rRangeOffset;

bool inRange(rRangeOffset *rangeOffset, int64_t input) {
    if (input >= rangeOffset->from && input < (rangeOffset->from + rangeOffset->range)) {
        return true;
    }

    return false;
}

int64_t offsetNumber(rRangeOffset *rangeOffset, int64_t input) {
    if (inRange(rangeOffset, input)) {
        return input + rangeOffset->offset;
    }

    return input;
}

rRangeOffset *rRangeOffsetNew(int64_t dest, int64_t source, int64_t range) {
    rRangeOffset * rangeOffset = calloc(1, sizeof(rRangeOffset));

    rangeOffset->from = source;
    rangeOffset->range = range;
    rangeOffset->offset = dest - source;

    return rangeOffset;
}

typedef struct {
    rRangeOffset ** data;
    size_t size;
    int8_t n;
} rArray;

rArray *rArrayNew(size_t sizeHint) {
    rArray * array = calloc(1, sizeof(rArray));

    array->n = 0;
    array->size = sizeHint;
    array->data = calloc(sizeHint, sizeof(rRangeOffset));

    return array;
}

void rArrayAdd(rArray * array, rRangeOffset * element) {
    *(array->data + array->n) = element;
    ++(array->n);
}

int64_t partOne();
int64_t partTwo();

int main() {
    printf("Part One %" PRIu64 "\n", partOne());
    printf("Part Two %" PRIu64 "\n", partTwo());

    return 0;
}

// DESTINATION, SOURCE, RANGE

void rStringParseNumbers(char * const line, int64_t * const into) {
    char * it = strpbrk(line, DIGITS);
    for (int i = 0; it != NULL; ++i) {
        *(into + i) = strtol(it, &it, 10);

        it = strpbrk(it, DIGITS);
    }
}

int64_t getLocationFromSeed(rArray ** const maps, uint8_t nMaps, int64_t seed) {
    int64_t input = seed;
    for (int i = 0; i < nMaps; ++i) {
        rArray * map = *(maps + i);

        for (int j = 0; j < map->n; ++j) {
            rRangeOffset * rangeOffset = *(map->data + j);

            if (inRange(rangeOffset, input)) {
                input += rangeOffset->offset;
                break;
            }
        }
    }

    return input;
}

int64_t partOne() {
    FILE *file = rFileOpen("input.txt");

    int8_t const numSeeds = 20;
    uint8_t const bufferSize = 255;
    int8_t const numMaps = 7;

    char buffer[bufferSize];
    int64_t * const seeds = calloc(numSeeds, sizeof(int64_t));
    rArray ** const maps = calloc(numMaps, sizeof(rArray));

    // Extract the seeds first
    {
        char * ret = fgets(buffer, bufferSize, file);
        assert(ret != NULL);

        char * const line = rStringCopyUntil(buffer, '\n');
        rStringParseNumbers(line, seeds);
    }

    for (int i = -1; fgets(buffer, bufferSize, file) != NULL; ) {
        char * const line = rStringCopyUntil(buffer, '\n');
        if (strlen(line) == 0) {
            continue;
        } else if (strchr(line, ':') != NULL) {
            ++i;
            *(maps + i) = rArrayNew(64);
            continue;
        }

        int64_t * const nums = calloc(3, sizeof(int64_t));
        rStringParseNumbers(line, nums);

        rRangeOffset * rangeOffset = rRangeOffsetNew(nums[0], nums[1], nums[2]);

        rArrayAdd(*(maps + i), rangeOffset);
    }

    fclose(file);

    int64_t min = INT_MAX;
    for (int i = 0; i < numSeeds; ++i) {
        int64_t location = getLocationFromSeed(maps, numMaps, *(seeds + i));
//        fprintf(stderr, "Seed: %" PRIi64 ", Location %" PRIi64 "\n", *(seeds + i), location);

        if (location < min) {
            min = location;
        }
    }

    return min;
}

int64_t partTwo() {
    FILE *file = rFileOpen("input.txt");

    int8_t const numSeeds = 20;
    uint8_t const bufferSize = 255;
    int8_t const numMaps = 7;

    char buffer[bufferSize];
    int64_t * const seeds = calloc(numSeeds, sizeof(int64_t));
    rArray ** const maps = calloc(numMaps, sizeof(rArray));

    // Extract the seeds first
    {
        char * ret = fgets(buffer, bufferSize, file);
        assert(ret != NULL);

        char * const line = rStringCopyUntil(buffer, '\n');
        rStringParseNumbers(line, seeds);
    }

    for (int i = -1; fgets(buffer, bufferSize, file) != NULL; ) {
        char * const line = rStringCopyUntil(buffer, '\n');
        if (strlen(line) == 0) {
            continue;
        } else if (strchr(line, ':') != NULL) {
            ++i;
            *(maps + i) = rArrayNew(64);
            continue;
        }

        int64_t * const nums = calloc(3, sizeof(int64_t));
        rStringParseNumbers(line, nums);

        rRangeOffset * rangeOffset = rRangeOffsetNew(nums[0], nums[1], nums[2]);

        rArrayAdd(*(maps + i), rangeOffset);
    }

    fclose(file);

    int64_t min = INT_MAX;
    for (int i = 0, j = 1; j < numSeeds; i += 2, j += 2) {
        int64_t aSeed = *(seeds + i);
        int64_t seedRange = *(seeds + j);

        for (int64_t x = aSeed; x < aSeed + seedRange; ++x) {
            int64_t location = getLocationFromSeed(maps, numMaps, x);
            //fprintf(stderr, "Seed: %" PRIi64 ", Location %" PRIi64 "\n", *(seeds + i), location);

            if (location < min) {
                min = location;
            }
        }
    }

    return min;
}