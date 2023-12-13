#include "rlib/file.h"
#include "rlib/array.h"
#include "rlib/string.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <inttypes.h>

int64_t part_one();
//int64_t part_two();

int main() {
    for (size_t i = 0; i < 1000; ++i) {
        printf("Part One: %" PRIi64 "\n", part_one());
    }

//    printf("Part One: %" PRIi64 "\n", part_two());

    return 0;
}

//int64_t enumerate_solutions(char const * c, int64_t * g, int64_t n_g, int64_t n) {
//    if (*c == '\0') { // ONly half sure this is right?
//        if (n_g == 0 && n == 0) {
//            return 1;
//        } else {
//            return 0;
//        }
//    }
//
//    int64_t remaining_group_size = 0;
//    for (size_t i = 0; i < n_g; ++i) {
//        remaining_group_size += *(g + i);
//    }
//    remaining_group_size -= n;
//
//    if (strlen(c) < (remaining_group_size)) {
//        return 0;
//    }
//
//    int64_t num_solutions = 0;
//
//    if (*c == '?') {
//        // Try a #
//        num_solutions += enumerate_solutions(c + 1, g, n_g, n + 1);
//
//        // Try a .
//        if (n != 0) {
//            if (n_g != 0 && n == *g) {
//                num_solutions += enumerate_solutions(c + 1, g + 1, n_g - 1, 0);
//            }
//        } else {
//            num_solutions += enumerate_solutions(c + 1, g, n_g, 0);
//        }
//    } else if (*c == '#') {
//        num_solutions += enumerate_solutions(c + 1, g, n_g, n + 1);
//    } else {
//        if (n != 0) {
//            if (n_g != 0 && n == *g) {
//                num_solutions += enumerate_solutions(c + 1, g + 1, n_g - 1, 0);
//            }
//        } else {
//            num_solutions += enumerate_solutions(c + 1, g, n_g, 0);
//        }
//    }
//
//    return num_solutions;
//}

//int64_t enumerate_solutions(char const * string, rArray_int64_t * groups, size_t s_i, size_t g_i, int64_t g_i_i) {
//    size_t n_g = r_array_length_int64_t(groups);
//    int64_t g = r_array_get_int64_t(groups, g_i);
//    char c = string[s_i];
//
//    if (c == '\0') {
//        if (g_i == n_g && g_i_i == 0) {
//            return 1;
//        } else {
//            return 0;
//        }
//    }
//
//    int64_t n_solutions = 0;
//
////    if (string[s_i] == '#' || string[s_i] == '?') {
////        n_solutions += enumerate_solutions(string, groups, s_i + 1, g_i, g_i_i + 1);
////    }
////
////    if (string[s_i] == '.' || string[s_i] == '?') {
////        if (g_i_i != 0) {
////            if (g_i != n_g && g_i_i == g) {
////                n_solutions += enumerate_solutions(string, groups, s_i + 1, g_i + 1, 0);
////            }
////        } else {
////            n_solutions += enumerate_solutions(string, groups, s_i + 1, g_i, 0);
////        }
////    }
//
//    if (string[s_i] == '?') {
//        n_solutions += enumerate_solutions(string, groups, s_i + 1, g_i, g_i_i + 1);
//
//        if (g_i_i != 0) {
//            if (g_i != n_g && g_i_i == g) {
//                n_solutions += enumerate_solutions(string, groups, s_i + 1, g_i + 1, 0);
//            }
//        } else {
//            n_solutions += enumerate_solutions(string, groups, s_i + 1, g_i, 0);
//        }
//    } else if (string[s_i] == '#') {
//        n_solutions += enumerate_solutions(string, groups, s_i + 1, g_i, g_i_i + 1);
//    } else {
//        if (g_i_i != 0) {
//            if (g_i != n_g && g_i_i == g) {
//                n_solutions += enumerate_solutions(string, groups, s_i + 1, g_i + 1, 0);
//            }
//        } else {
//            n_solutions += enumerate_solutions(string, groups, s_i + 1, g_i, 0);
//        }
//    }
////
////    if (c == '#' || c == '?') {
////        // Extend group
////        n_solutions += enumerate_solutions(string, groups, s_i + 1, g_i, g_i_i + 1);
////    }
////
////    if (c == '.' || c == '?') {
////        if (g_i_i != 0) {
////            if (g_i != n_g && g_i_i == g) {
////                // Close group
////                n_solutions += enumerate_solutions(string, groups, s_i + 1, g_i + 1, 0);
////            }
////        } else {
////            // No group
////            n_solutions += enumerate_solutions(string, groups, s_i + 1, g_i, 0);
////        }
////    }
//
//    return n_solutions;
//}

size_t n_i(size_t i, size_t j, size_t k) {
    return (i * 128 * 32) + (j * 32) + k;
}

int64_t * cache;

int64_t enumerate_solutions_memoised(char const * string, rArray_int64_t * groups, size_t s_i, size_t g_i, size_t g_i_i) {
    int64_t r_s = strlen(string) - s_i;
    int64_t r_g = r_array_length_int64_t(groups) - g_i;

    if (string[s_i] == '\0') {
        if (g_i == r_array_length_int64_t(groups) && g_i_i == 0) {
            return 1;
        } else {
            return 0;
        }
    }

    if (cache[n_i(r_s, r_g, g_i_i)] >= 0) {
        return cache[n_i(r_s, r_g, g_i_i)];
    }

    int64_t n_solutions = 0;

    if (string[s_i] == '?') {
        n_solutions += enumerate_solutions_memoised(string, groups, s_i + 1, g_i, g_i_i + 1);

        if (g_i_i != 0) {
            if (g_i != r_array_length_int64_t(groups) && g_i_i == r_array_get_int64_t(groups, g_i)) {
                n_solutions += enumerate_solutions_memoised(string, groups, s_i + 1, g_i + 1, 0);
            }
        } else {
            n_solutions += enumerate_solutions_memoised(string, groups, s_i + 1, g_i, 0);
        }
    } else if (string[s_i] == '#') {
        n_solutions += enumerate_solutions_memoised(string, groups, s_i + 1, g_i, g_i_i + 1);
    } else {
        if (g_i_i != 0) {
            if (g_i != r_array_length_int64_t(groups) && g_i_i == r_array_get_int64_t(groups, g_i)) {
                n_solutions += enumerate_solutions_memoised(string, groups, s_i + 1, g_i + 1, 0);
            }
        } else {
            n_solutions += enumerate_solutions_memoised(string, groups, s_i + 1, g_i, 0);
        }
    }

    cache[n_i(r_s, r_g, g_i_i)] = n_solutions;
    return n_solutions;
}

//int64_t enumerate_solutions(char const * string, rArray_int64_t * groups, size_t s_i, size_t g_i, size_t g_i_i) {
//    if (string[s_i] == '\0') {
//        if (g_i == r_array_length_int64_t(groups) && g_i_i == 0) {
//            return 1;
//        }
//        if (g_i == r_array_length_int64_t(groups) - 1 && r_array_get_int64_t(groups, g_i) == g_i_i) {
//            return 1;
//        }
//
//        return 0;
//    }
//
//    int64_t n_solutions = 0;
//
//    if (string[s_i] == '#' || string[s_i] == '?') {
//        n_solutions += enumerate_solutions(string, groups, s_i + 1, g_i, g_i_i + 1);
//    }
//
//    if (string[s_i] == '.' || string[s_i] == '?') {
//        if (g_i_i == 0) {
//            n_solutions += enumerate_solutions(string, groups, s_i + 1, g_i, 0);
//        }
//        if (g_i_i > 0 && g_i < r_array_length_int64_t(groups) && r_array_get_int64_t(groups, g_i) == g_i_i) {
//            n_solutions += enumerate_solutions(string, groups, s_i + 1, g_i + 1, 0);
//        }
//    }
//
//    return n_solutions;
//}

//int64_t enumerate_solutions_mem(char const * string, rArray_int64_t * groups, size_t string_i, size_t groups_i, size_t group_i_i) {
////    if (cache[n_i(string_i, groups_i, group_i_i)] >= 0) {
////        return cache[n_i(string_i, groups_i, group_i_i)];
////    }
//
//    char character = string[string_i];
//    int64_t group = r_array_get_int64_t(groups, groups_i);
//    size_t groups_n = r_array_length_int64_t(groups);
//
//    if (character == '\0') {
//        if (groups_i == groups_n && group_i_i == 0) {
//            return 1;
//        } else {
//            return 0;
//        }
//    }
//
//    int64_t n_solutions = 0;
//
//    if (character == '?') {
//        // Extend the current group by one
//        n_solutions += enumerate_solutions_mem(string, groups, string_i + 1, groups_i, group_i_i + 1);
//
//        // If we can close the current group, do it
//        if (group_i_i > 0) {
//            if (groups_i < groups_n && group_i_i == group) {
//                n_solutions += enumerate_solutions_mem(string, groups, string_i + 1, groups_i + 1, 0);
//            }
//        } else { // Otherwise we're not in a group, continue
//            n_solutions += enumerate_solutions_mem(string, groups, string_i + 1, groups_i, 0);
//        }
//    } else if (character == '#') {
//        n_solutions += enumerate_solutions_mem(string, groups, string_i + 1, groups_i, group_i_i + 1);
//    } else {
//        // If we can close the current group, do it
//        if (group_i_i > 0) {
//            if (groups_i < groups_n && group_i_i == group) {
//                n_solutions += enumerate_solutions_mem(string, groups, string_i + 1, groups_i + 1, 0);
//            }
//        } else { // Otherwise we're not in a group, continue
//            n_solutions += enumerate_solutions_mem(string, groups, string_i + 1, groups_i, 0);
//        }
//    }
//
////    cache[n_i(string_i, groups_i, group_i_i)] = n_solutions;
//    return n_solutions;
//}

int64_t part_one() {
    FILE * file = rFileOpen("input.txt");

    int64_t sum = 0;

    cache = malloc(256 * 128 * 32 * sizeof(int64_t));

    size_t const buffer_size = 256;
    char buffer[buffer_size];
    for (size_t m = 0; fgets(buffer, buffer_size, file) != NULL; ++m) {
        char * line = r_string_copy_until(buffer, '\n');
        char * pattern = r_string_copy_until(line, ' ');
//        r_string_append(&pattern, ".");
        rArray_int64_t * groups = r_string_parse_numbers(strchr(line, ' '));

//        fprintf(stderr, "%s: ", pattern);
        //r_array_print_int64_t(stderr, groups);

        for (size_t i = 0; i < 256 * 128 * 32; ++i) {
            cache[i] = -1;
        }

        size_t x = strlen(pattern);
        int64_t solutions = enumerate_solutions_memoised(pattern, groups, 0, 0, 0);
        sum += solutions;
//        printf("%zu: %" PRIi64"\n", m, solutions);

    }

    free(cache);

    fclose(file);

    return sum;
}
