#include "rlib/file.h"
#include "rlib/array.h"
#include "rlib/string.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <inttypes.h>

int64_t part_one();
int64_t part_two();

int main() {
    printf("Part One: %" PRIi64 "\n", part_one());
    // printf("Part Two: %" PRIi64 "\n", part_two());
}

int64_t hash(int64_t previous_value, char c) {
    int64_t value = (previous_value + c);
    value *= 17;
    return value % 256;
}

int64_t part_one() {
    FILE * file = r_file_open("input.txt");

    int64_t sum = 0;

    size_t const buffer_size = 32000;
    char buffer[buffer_size];
    while (fgets(buffer, buffer_size, file) != NULL) {
        char * line = r_string_copy_until(buffer, '\n');

        int64_t value = 0;
        for (char * it = line; *it != '\0'; ++it) {
            if (*it == ',') {
                sum += value;
                printf("%" PRIi64 "\n", value);
                value = 0;
                continue;
            }

            value = hash(value, *it);
        }

        sum += value;
        printf("%" PRIi64 "\n", value);

        free(line);
    }

    fclose(file);

    return sum;
}