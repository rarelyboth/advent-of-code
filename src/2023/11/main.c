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

void parse_galaxy_locations(char const * string, size_t y, rArray_Pair * galaxies) {
    char const * it = string;
    for (size_t x = 0; *it != '\0'; ++it, ++x) {
        if (*it == '#') {
            r_array_append_Pair(galaxies, (Pair){.first=x, .second=y});
        }
    }
}

void detect_expanded_space(rArray_Pair * galaxies, size_t const universe_size, rArray_int64_t * expanded_x, rArray_int64_t * expanded_y) {
    for (size_t i = 0; i < universe_size; ++i) {
        bool found_x = false;
        bool found_y = false;
        for (Pair * it = r_array_begin_Pair(galaxies); it != r_array_end_Pair(galaxies); ++it) {
            if (it->first == i) {
                found_x = true;
            }

            if (it->second == i) {
                found_y = true;
            }

            if (found_x && found_y) {
                break;
            }
        }

        if (!found_x) {
            r_array_append_int64_t(expanded_x, i);
        }

        if (!found_y) {
            r_array_append_int64_t(expanded_y, i);
        }
    }
}

int64_t expanded_space_between(int64_t a, int64_t b, rArray_int64_t * expanded) {
    if (a == b) {
        return 0;
    }

    int64_t closest = a < b ? a : b;
    int64_t farthest = a < b ? b : a;

    int64_t n_expanded = 0;
    for (int64_t * it = r_array_begin_int64_t(expanded); it != r_array_end_int64_t(expanded); ++it) {
        if (closest <= *it && *it <= farthest) {
            ++n_expanded;
        }
    }

    return n_expanded;
}

int64_t distance_between(Pair const * a, Pair const * b, rArray_int64_t * expanded_x, rArray_int64_t * expanded_y, int64_t expansion_multiplier) {
    int64_t x_distance = abs(a->first - b->first) + expanded_space_between(a->first, b->first, expanded_x) * (expansion_multiplier - 1);
    int64_t y_distance = abs(a->second - b->second) + expanded_space_between(a->second, b->second, expanded_y) * (expansion_multiplier - 1);

    return x_distance + y_distance;
}

int64_t part_one() {
    FILE * file = rFileOpen("input.txt");

    int64_t const expansion_multiplier = 1;
    size_t const universe_size = 140;
    rArray_Pair * galaxies = r_array_new_Pair(universe_size);
    rArray_int64_t * expanded_x = r_array_new_int64_t(universe_size / 2);
    rArray_int64_t * expanded_y = r_array_new_int64_t(universe_size / 2);

    char * line;
    size_t const buffer_size = 256;
    char buffer[buffer_size];
    for (size_t y = 0; fgets(buffer, buffer_size, file) != NULL; ++y) {
        line = rStringCopyUntil(buffer, '\n');
        parse_galaxy_locations(line, y, galaxies);
    }

    // Identify expanded space
    detect_expanded_space(galaxies, universe_size, expanded_x, expanded_y);

    // For every pair, calculate their distances & sum
    int64_t sum = 0;
    for (Pair * it = r_array_begin_Pair(galaxies); it != r_array_end_Pair(galaxies); ++it) {
        for (Pair * it_next = it + 1; it_next != r_array_end_Pair(galaxies); ++it_next) {
            if (it == it_next) {
                continue;
            }

            sum += distance_between(it, it_next, expanded_x, expanded_y, expansion_multiplier);
        }
    }

    return sum;
}

int64_t part_two() {
    FILE * file = rFileOpen("input.txt");

    int64_t const expansion_multiplier = 1000000;
    size_t const universe_size = 140;
    rArray_Pair * galaxies = r_array_new_Pair(universe_size);
    rArray_int64_t * expanded_x = r_array_new_int64_t(universe_size / 2);
    rArray_int64_t * expanded_y = r_array_new_int64_t(universe_size / 2);

    char * line;
    size_t const buffer_size = 256;
    char buffer[buffer_size];
    for (size_t y = 0; fgets(buffer, buffer_size, file) != NULL; ++y) {
        line = rStringCopyUntil(buffer, '\n');
        parse_galaxy_locations(line, y, galaxies);
    }

    // Identify expanded space
    detect_expanded_space(galaxies, universe_size, expanded_x, expanded_y);

    // For every pair, calculate their distances & sum
    int64_t sum = 0;
    for (Pair * it = r_array_begin_Pair(galaxies); it != r_array_end_Pair(galaxies); ++it) {
        for (Pair * it_next = it + 1; it_next != r_array_end_Pair(galaxies); ++it_next) {
            if (it == it_next) {
                continue;
            }

            sum += distance_between(it, it_next, expanded_x, expanded_y, expansion_multiplier);
        }
    }

    return sum;
}