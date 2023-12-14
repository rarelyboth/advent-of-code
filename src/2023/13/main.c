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
    printf("Part Two: %" PRIi64 "\n", part_two());

    return 0;
}

bool is_power_of_two(int64_t value) {
    return value && (!(value & (value - 1)));
}

bool differs_by_only_one_bit(int64_t a, int64_t b) {
    return is_power_of_two(a ^ b);
}

int64_t count_reflections(rArray_int64_t * array) {
    // Begin the iteration one in from the left
    // it represents the element one after the line of reflection
    int64_t * it = r_array_begin_int64_t(array) + 1;
    for (int64_t i = 1; it < r_array_end_int64_t(array); ++it, ++i) {
        bool reflects = true;

        // Bi-directional iterators
        int64_t * it_l = it - 1;
        int64_t * it_r = it;

        for (; it_l >= r_array_begin_int64_t(array) && it_r < r_array_end_int64_t(array); --it_l, ++it_r) {
            if (*it_l != *it_r) {
                reflects = false;
                break;
            }
        }

        // If there are no reflections about this line, keep looking
        if (!reflects) {
            continue;
        } else {
            return i;  // Assumes each pattern has only one unique reflection
        }
    }

    return 0;
}

int64_t count_smudgy_reflections(rArray_int64_t * array) {
    // Begin the iteration one in from the left
    // it represents the element one after the line of reflection
    int64_t * it = r_array_begin_int64_t(array) + 1;
    for (int64_t i = 1; it < r_array_end_int64_t(array); ++it, ++i) {
        bool reflects = true;
        bool cleaned = false;

        // Bi-directional iterators
        int64_t * it_l = it - 1;
        int64_t * it_r = it;

        for (; it_l >= r_array_begin_int64_t(array) && it_r < r_array_end_int64_t(array); --it_l, ++it_r) {
            if (*it_l != *it_r) {
                if (!cleaned && differs_by_only_one_bit(*it_l, *it_r)) {
                    cleaned = true;
                    continue;
                }

                reflects = false;
                break;
            }
        }

        // If there are no reflections about this line, keep looking
        if (!reflects || !cleaned) {
            continue;
        } else {
            return i;  // Assumes each pattern has only one unique reflection
        }
    }

    return 0;
}

int64_t part_one() {
    FILE * file = r_file_open("input.txt");

    size_t const buffer_size = 32;
    char buffer[buffer_size];

    int64_t sum = 0;

    bool file_empty = false;
    while (1) {
        rArray_int64_t * rows = r_array_new_int64_t(buffer_size);     // y
        rArray_int64_t * columns = r_array_new_int64_t(buffer_size);  // x

        for (size_t y = 0; ; ++y) {
            if (fgets(buffer, buffer_size, file) == NULL) {
                file_empty = true;
                break;
            }
            char * line = r_string_copy_until(buffer, '\n');
            if (line[0] == '\0') {
                break;
            }

            r_array_append_int64_t(rows, 0);
            for (size_t x = 0; line[x] != '\0'; ++x) {
                if (r_array_length_int64_t(columns) <= x) {
                    r_array_append_int64_t(columns, 0);
                }

                int64_t row = r_array_get_int64_t(rows, y);
                int64_t col = r_array_get_int64_t(columns, x);

                int8_t bit = line[x] == '#' ? 1 : 0;
                r_array_set_int64_t(rows, y, row << 1 | bit);
                r_array_set_int64_t(columns, x, col << 1 | bit);
            }
        }

        int64_t row_reflections = count_reflections(rows) * 100;
        int64_t column_reflections = count_reflections(columns);

        sum += row_reflections > column_reflections ? row_reflections : column_reflections;

        r_array_free_int64_t(&rows);
        r_array_free_int64_t(&columns);

        if (file_empty) {
            break;
        }
    }

    fclose(file);

    return sum;
}

int64_t part_two() {
    FILE * file = r_file_open("input.txt");

    size_t const buffer_size = 32;
    char buffer[buffer_size];

    int64_t sum = 0;

    bool file_empty = false;
    while (1) {
        rArray_int64_t * rows = r_array_new_int64_t(buffer_size);     // y
        rArray_int64_t * columns = r_array_new_int64_t(buffer_size);  // x

        for (size_t y = 0; ; ++y) {
            if (fgets(buffer, buffer_size, file) == NULL) {
                file_empty = true;
                break;
            }
            char * line = r_string_copy_until(buffer, '\n');
            if (line[0] == '\0') {
                break;
            }

            r_array_append_int64_t(rows, 0);
            for (size_t x = 0; line[x] != '\0'; ++x) {
                if (r_array_length_int64_t(columns) <= x) {
                    r_array_append_int64_t(columns, 0);
                }

                int64_t row = r_array_get_int64_t(rows, y);
                int64_t col = r_array_get_int64_t(columns, x);

                int8_t bit = line[x] == '#' ? 1 : 0;
                r_array_set_int64_t(rows, y, row << 1 | bit);
                r_array_set_int64_t(columns, x, col << 1 | bit);
            }
        }

        int64_t row_reflections = count_smudgy_reflections(rows) * 100;
        int64_t column_reflections = count_smudgy_reflections(columns);

        sum += row_reflections > column_reflections ? row_reflections : column_reflections;

        r_array_free_int64_t(&rows);
        r_array_free_int64_t(&columns);

        if (file_empty) {
            break;
        }
    }

    fclose(file);

    return sum;
}
