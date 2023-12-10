#include "rlib/file.h"
#include "rlib/array.h"
#include "rlib/string.h"

#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include <string.h>
#include <stdbool.h>

int64_t part_one();
int64_t part_two();

int main() {
    printf("Part One: %" PRIi64 "\n", part_one());
    printf("Part Two: %" PRIi64 "\n", part_two());

    return 0;
}

size_t get_direction(size_t prev_coordinate, size_t current_coordinate) {
    if (prev_coordinate == current_coordinate) {
        return 0;
    }

    return prev_coordinate < current_coordinate ? 1 : -1;
}

void follow_pipe(rArray_string ** map, char current_char, size_t * prev_x, size_t * x, size_t * prev_y, size_t * y) {
    size_t x_dir = get_direction(*prev_x, *x);
    size_t y_dir = get_direction(*prev_y, *y);

    switch (current_char) {
    case '|':
        *prev_y = *y;
        *y += y_dir;

        break;
    case '-':
        *prev_x = *x;
        *x += x_dir;

        break;
    case 'L':
        *prev_x = *x;
        *prev_y = *y;
        *y += y_dir == 0 ? -1 : 0;
        *x += x_dir == 0 ? 1 : 0;

        break;
    case 'F':
        *prev_x = *x;
        *prev_y = *y;
        *y += y_dir == 0 ? 1 : 0;
        *x += x_dir == 0 ? 1 : 0;

        break;
    case 'J':
        *prev_x = *x;
        *prev_y = *y;
        *y += y_dir == 0 ? -1 : 0;
        *x += x_dir == 0 ? -1 : 0;

        break;
    case '7':
        *prev_x = *x;
        *prev_y = *y;
        *y += y_dir == 0 ? 1 : 0;
        *x += x_dir == 0 ? -1 : 0;

        break;
    case 'S':
        *y += 1;  // Manually assessed starting direction
        break;
    default:
        fprintf(stderr, "Unknown pipe: %c!\n", current_char);
        break;
    }
}

int64_t part_one() {
    FILE * file = rFileOpen("input.txt");

    size_t const map_height = 140;
    rArray_string * map = r_array_new_string(map_height);
    size_t start_x = 0;
    size_t start_y = 0;

    size_t const buffer_size = 255;
    char buffer[buffer_size];
    for (size_t i = 0; fgets(buffer, buffer_size, file) != NULL; ++i) {
        char * line = rStringCopyUntil(buffer, '\n');
        r_array_append_string(map, line);

        char * start = strchr(line, 'S');
        if (start_x == 0 && start_y == 0 && start != NULL) {
            start_x = start - line;
            start_y = i;
        }
    }
    fclose(file);

    int64_t steps = 0;
    // Follow the pipe back to the start, counting the steps taken
    {
        size_t x = start_x;
        size_t y = start_y;
        size_t prev_x = x;
        size_t prev_y = y;

        char current_char = *(r_array_get_string(map, y) + x);
        do {
            follow_pipe(&map, current_char, &prev_x, &x, &prev_y, &y);
            ++steps;
            current_char = *(r_array_get_string(map, y) + x);
        } while (current_char != 'S');
    }

    r_array_free_string(&map);

    return steps / 2;
}

void evaluate_inside_or_outside(char * prev_char, char current_char, bool * inside) {
    bool invert = false;

    switch (current_char) {
    case '|':
        invert = true;
        break;
    case '-':
        break;
    case 'F':
        *prev_char = current_char;
        invert = true;
        break;
    case 'J':
        if (*prev_char != 'F') {
            invert = true;
        }
        break;
    case 'L':
        *prev_char = current_char;
        invert = true;
        break;
    case '7':
        if (*prev_char != 'L') {
            invert = true;
        }
        break;
    case 'S':
        invert = true; // Manually evaluated
        break;
    default:
        fprintf(stderr, "Unknown pipe section: %c\n", current_char);
        exit(EXIT_FAILURE);
    }

    *inside = invert ? !(*inside) : *inside;
}

int64_t part_two() {
    FILE * file = rFileOpen("input.txt");

    size_t const map_height = 140;
    rArray_string * map = r_array_new_string(map_height);
    size_t start_x = 0;
    size_t start_y = 0;

    size_t const buffer_size = 255;
    char buffer[buffer_size];
    for (size_t i = 0; fgets(buffer, buffer_size, file) != NULL; ++i) {
        char * line = rStringCopyUntil(buffer, '\n');
        r_array_append_string(map, line);

        char * start = strchr(line, 'S');
        if (start_x == 0 && start_y == 0 && start != NULL) {
            start_x = start - line;
            start_y = i;
        }
    }
    fclose(file);

    rArray_Pair * loop_sections = r_array_new_Pair(100);
    // Record all the pipe section connected to the loop
    {
        size_t x = start_x;
        size_t y = start_y;
        size_t prev_x = x;
        size_t prev_y = y;

        char current_char = *(r_array_get_string(map, y) + x);
        do {
            follow_pipe(&map, current_char, &prev_x, &x, &prev_y, &y);
            r_array_append_Pair(loop_sections, (Pair){.first=x, .second=y});
            current_char = *(r_array_get_string(map, y) + x);
        } while (current_char != 'S');
    }

    int64_t enclosed = 0;
    // Determine which tiles are enclosed
    {
        for (size_t y = 0; y < r_array_length_string(map); ++y) {
            bool inside = false;
            char prev_char = *r_array_get_string(map, y);

            for (size_t x = 0; *(r_array_get_string(map, y) + x) != '\0'; ++x) {
                char current_char = *(r_array_get_string(map, y) + x);

                if (r_array_includes_Pair(loop_sections, (Pair){.first=x, .second=y})) {
                    evaluate_inside_or_outside(&prev_char, current_char, &inside);
                    continue;
                }

                if (inside) {
                    ++enclosed;
                }
            }
        }
    }

    r_array_free_string(&map);

    return enclosed;
}