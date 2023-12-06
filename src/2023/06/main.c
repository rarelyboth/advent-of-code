#include "rlib/string.h"
#include "rlib/file.h"

#include <stdlib.h>
#include <inttypes.h>
#include <assert.h>
#include <math.h>
#include <string.h>

int64_t partOne();
int64_t partTwo();

int main() {
    printf("Part One: %" PRIi64 "\n", partOne());
    printf("Part Two: %" PRIi64 "\n", partTwo());

    return 0;
}

int64_t numWinningOptions(int64_t time, int64_t distance) {
    double rootA = (-time + sqrt(pow(time, 2) - (4 * -1 * -distance))) / (2 * -1);
    double rootB = (-time - sqrt(pow(time, 2) - (4 * -1 * -distance))) / (2 * -1);

    return (ceil(rootB) - 1) - (floor(rootA) + 1) + 1;
}

int64_t partOne() {
    FILE * file = rFileOpen("input.txt");

    size_t const bufferSize = 64;
    char buffer[bufferSize];

    size_t const races = 4;
    int64_t raceTime[races];
    int64_t raceDistance[races];

    // Read the race times
    {
        fgets(buffer, bufferSize, file);
        assert(buffer != NULL);

        char * line = rStringCopyUntil(buffer, '\n');
        rStringParseNumbers(line, raceTime);
        free(line);
    }

    // Read the race distances
    {
        fgets(buffer, bufferSize, file);
        assert(buffer != NULL);

        char * line = rStringCopyUntil(buffer, '\n');
        rStringParseNumbers(line, raceDistance);
        free(line);
    }

    // Determine the range of winning options
    int64_t product = 1;
    {
        for (int i = 0; i < races; ++i) {
            uint64_t n = numWinningOptions(raceTime[i], raceDistance[i]);
            product *= n;
        }
    }

    fclose(file);

    return product;
}

int64_t parsePoorlyKernedNumber(char const * line) {
    int8_t maxDigits = strlen(line);
    char * digits = calloc(maxDigits, sizeof(char));

    char * it = strpbrk(line, "0123456789");
    for (int i = 0; it != NULL; ++i) {
        digits[i] = *it;
        ++it;
        it = strpbrk(it, "0123456789");
    }

    return strtoll(digits, NULL, 10);
}

int64_t partTwo() {
    FILE * file = rFileOpen("input.txt");

    size_t const bufferSize = 64;
    char buffer[bufferSize];

    int64_t raceTime;
    int64_t raceDistance;

    // Read the race times
    {
        fgets(buffer, bufferSize, file);
        assert(buffer != NULL);

        char * line = rStringCopyUntil(buffer, '\n');
        raceTime = parsePoorlyKernedNumber(line);
        free(line);
    }

    // Read the race distances
    {
        fgets(buffer, bufferSize, file);
        assert(buffer != NULL);

        char * line = rStringCopyUntil(buffer, '\n');
        raceDistance = parsePoorlyKernedNumber(line);
        free(line);
    }

    fclose(file);

    return numWinningOptions(raceTime, raceDistance);
}