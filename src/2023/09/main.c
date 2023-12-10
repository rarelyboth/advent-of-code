#include "rlib/file.h"
#include "rlib/array.h"
#include "rlib/string.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <inttypes.h>

int64_t part_one();
int64_t part_two();

int main () {
    printf("Part One: %" PRIi64 "\n", part_one());
    printf("Part Two: %" PRIi64 "\n", part_two());

    return 0;
}

void parse_numbers(char const * string, rArray_int64_t ** into) {
    char * it = strpbrk(string, "-0123456789");
    int i = 0;
    for (; it != NULL; ++i) {
        r_array_append_int64_t(*into, strtol(it, &it, 10));

        it = strpbrk(it, "-0123456789");
    }
}

rArray_int64_t * calculate_adjacent_differences(rArray_int64_t * array) {
    rArray_int64_t * differences = r_array_new_int64_t(r_array_length_int64_t(array) - 1);

    int64_t * it = r_array_begin_int64_t(array);
    int64_t * itNext = it + 1;
    for (; itNext != r_array_end_int64_t(array); ++it, ++itNext) {
        int64_t difference = *itNext - *it;
        r_array_append_int64_t(differences, difference);
    }

    return differences;
}

int64_t sum_values(rArray_int64_t * array) {
    int64_t sum = 0;

    for (int64_t * it = r_array_begin_int64_t(array); it != r_array_end_int64_t(array); ++it) {
        sum += *it;
    }

    return sum;
}

int64_t extrapolate_next_value(rArray_int64_t * array, int64_t previous_last_value) {
    rArray_int64_t * last_differences = calculate_adjacent_differences(array);

    int64_t last_value = *(r_array_end_int64_t(last_differences) - 1);
    if (sum_values(array) == 0) {
        r_array_free_int64_t(&last_differences);
        return last_value + previous_last_value;
    } else {
        int64_t next = previous_last_value + extrapolate_next_value(last_differences, last_value);
        r_array_free_int64_t(&last_differences);
        return next;
    }
}

int64_t part_one() {
    FILE * file = rFileOpen("input.txt");

    size_t const buffer_size = 128;
    char buffer[buffer_size];
    char * line;

    int64_t sum = 0;

    while (fgets(buffer, buffer_size, file) != NULL) {
        line = rStringCopyUntil(buffer, '\n');

        rArray_int64_t * values = r_array_new_int64_t(1);
        parse_numbers(line, &values);

        int64_t next_value = extrapolate_next_value(values, *(r_array_end_int64_t(values) - 1));

        sum += next_value;

        r_array_free_int64_t(&values);
    }

    // Dealloc
    free(line);

    return sum;
}

int64_t part_two() {
    FILE * file = rFileOpen("input.txt");

    size_t const buffer_size = 128;
    char buffer[buffer_size];
    char * line;

    int64_t sum = 0;

    while (fgets(buffer, buffer_size, file) != NULL) {
        line = rStringCopyUntil(buffer, '\n');

        rArray_int64_t * values = r_array_new_int64_t(1);
        parse_numbers(line, &values);
        rArray_int64_t * r_values = r_array_reverse_int64_t(values);

        int64_t next_value = extrapolate_next_value(r_values, *(r_array_end_int64_t(r_values) - 1));

        sum += next_value;

        r_array_free_int64_t(&values);
    }

    // Dealloc
    free(line);

    return sum;
}