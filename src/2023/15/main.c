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
}

int64_t hash(int64_t previous_value, char c) {
    int64_t value = (previous_value + c);
    value *= 17;
    return value % 256;
}

int64_t hash_string(char * string) {
    int64_t sum = 0;
    for (char * it = string; *it != '\0'; ++it) {
        sum = hash(sum, *it);
    }

    return sum;
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
                value = 0;
                continue;
            }

            value = hash(value, *it);
        }

        // Include the final value
        sum += value;

        free(line);
    }

    fclose(file);

    return sum;
}

int64_t part_two() {
    FILE * file = r_file_open("input.txt");

    size_t const n_boxes = 256;
    rArray_Lens * boxes[n_boxes];

    for (size_t i = 0; i < n_boxes; ++i) {
        boxes[i] = r_array_new_Lens(10);
    }

    size_t const buffer_size = 32000;
    char buffer[buffer_size];
    while (fgets(buffer, buffer_size, file) != NULL) {
        char * line = r_string_copy_until(buffer, '\n');

        char * it = line;
        char * op = strpbrk(it, "-=");
        while (op != NULL) {
            char * label = r_string_copy_until(it, *op);
            size_t box_i = hash_string(label);
            rArray_Lens * box = boxes[box_i];

            if (*op == '=') {
                int64_t focal_length = strtol(strpbrk(op, "123456789"), NULL, 10);

                Lens * lens = r_array_find_Lens(box, (Lens){.label=label, .focal_length=focal_length});
                if (lens == r_array_end_Lens(box)) {
                    r_array_append_Lens(box, (Lens){.label=label, .focal_length=focal_length});
                } else {
                    lens->focal_length = focal_length;
                }
            } else if (*op == '-') {
                Lens * lens = r_array_find_Lens(box, (Lens){.label=label, .focal_length=0});
                if (lens != r_array_end_Lens(box)) {
                    r_array_remove_Lens(box, lens - r_array_begin_Lens(box));
                }
            }

            it = strchr(it, ',') + 1;
            op = strpbrk(op + 1, "-=");
        }

        free(line);
    }

    int64_t sum = 0;

    for (size_t i = 0; i < n_boxes; ++i) {
        rArray_Lens * box = boxes[i];
        Lens * it = r_array_begin_Lens(box);
        for (size_t j = 0; it != r_array_end_Lens(box); ++j, ++it) {
            sum += (i + 1) * (j + 1) * it->focal_length;
        }

        free(box);
    }

    fclose(file);

    return sum;
}