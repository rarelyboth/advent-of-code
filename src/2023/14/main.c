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

    return 0;
}

int64_t part_one() {
    FILE * file = r_file_open("input.txt");

    int64_t load = 0;

    size_t const platform_size = 100;
    rArray_int64_t * square_rocks = r_array_new_int64_t(platform_size);
    rArray_int64_t * round_rocks = r_array_new_int64_t(platform_size);

    size_t const buffer_size = 128;
    char buffer[buffer_size];
    for (size_t line_no = 0; fgets(buffer, buffer_size, file) != NULL; ++line_no) {
        char const * line = r_string_copy_until(buffer, '\n');

        char const * it = line;
        for (size_t i = 0; *it != '\0'; ++it, ++i) {
            if (r_array_length_int64_t(round_rocks) <= i && r_array_length_int64_t(square_rocks) <= i) {
                r_array_append_int64_t(round_rocks, 0);
                r_array_append_int64_t(square_rocks, -1);
            }

            if (*it == 'O') {
                *(r_array_begin_int64_t(round_rocks) + i) += 1;
            }

            if (*it == '#' || line_no == platform_size - 1) {
                int64_t last_square_rock = r_array_get_int64_t(square_rocks, i);
                int64_t round_rocks_seen = r_array_get_int64_t(round_rocks, i);

                for (size_t j = 0; j < round_rocks_seen; ++j) {
                    load += (int64_t)platform_size - 1 - last_square_rock - (int64_t)j;
                }

                *(r_array_begin_int64_t(square_rocks) + i) = (int64_t)line_no;
                r_array_set_int64_t(round_rocks, i, 0);
            }
        }
    }

    return load;
}

int64_t part_two() {
    FILE * file = r_file_open("example.txt");

    size_t const buffer_size = 16;
    char buffer[buffer_size];
    for (size_t y = 0; fgets(buffer, buffer_size, file) != NULL; ++y) {
        char * line = r_string_copy_until(buffer, '\n');

        char const * it = line;
        for (size_t x = 0; *it != '\0'; ++it, ++x) {

        }

        free(line);
    }

    fclose(file);

    return 0;
}