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
//    printf("Part One: %" PRIi64 "\n", part_one());
    printf("Part One: %" PRIi64 "\n", part_two());

    return 0;
}

int64_t enumerate_solutions(char const * c, int64_t * g, int64_t n_g, int64_t n) {
    if (*c == '\0') { // ONly half sure this is right?
        if (n_g == 0 && n == 0) {
            return 1;
        } else {
            return 0;
        }
    }

    int64_t remaining_group_size = 0;
    for (size_t i = 0; i < n_g; ++i) {
        remaining_group_size += *(g + i);
    }
    remaining_group_size -= n;

    if (strlen(c) < (remaining_group_size)) {
        return 0;
    }

    int64_t num_solutions = 0;

    if (*c == '?') {
        // Try a #
        num_solutions += enumerate_solutions(c + 1, g, n_g, n + 1);

        // Try a .
        if (n != 0) {
            if (n_g != 0 && n == *g) {
                num_solutions += enumerate_solutions(c + 1, g + 1, n_g - 1, 0);
            }
        } else {
            num_solutions += enumerate_solutions(c + 1, g, n_g, 0);
        }
    } else if (*c == '#') {
        num_solutions += enumerate_solutions(c + 1, g, n_g, n + 1);
    } else {
        if (n != 0) {
            if (n_g != 0 && n == *g) {
                num_solutions += enumerate_solutions(c + 1, g + 1, n_g - 1, 0);
            }
        } else {
            num_solutions += enumerate_solutions(c + 1, g, n_g, 0);
        }
    }

    return num_solutions;
}

int64_t part_one() {
    FILE * file = rFileOpen("input.txt");

    int64_t sum = 0;

    size_t const buffer_size = 256;
    char buffer[buffer_size];
    for (size_t i = 0; fgets(buffer, buffer_size, file) != NULL; ++i) {
        char * line = r_string_copy_until(buffer, '\n');
        char * pattern = r_string_copy_until(line, ' ');
//        r_string_append(&pattern, ".");
        rArray_int64_t * groups = r_string_parse_numbers(strchr(line, ' '));

//        fprintf(stderr, "%s: ", pattern);
        //r_array_print_int64_t(stderr, groups);

        sum += enumerate_solutions(pattern, r_array_begin_int64_t(groups), r_array_length_int64_t(groups), 0);
        printf("%zu\n", i);

    }

    return sum;
}