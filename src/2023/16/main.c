#include "rlib/file.h"
#include "rlib/array.h"
#include "rlib/string.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <inttypes.h>

typedef enum {
    UP=0,
    RIGHT=1,
    DOWN=2,
    LEFT=3
} Direction;

int64_t part_one(char const * filename, size_t map_size);
int64_t part_two(char const * filename, size_t map_size);

int main() {
    char const * filename = "input.txt";

    printf("Part One: %" PRIi64 "\n", part_one(filename, 110));
    printf("Part Two: %" PRIi64 "\n", part_two(filename, 110));

    return 0;
}

void energise_tile(int64_t * cache, int64_t * visited, size_t map_size, size_t x, size_t y, Direction dir) {
    cache[(x * map_size * 4) + (y * 4) + dir] = 1;
    visited[(x * map_size) + y] = 1;
}

bool is_cached(int64_t * cache, size_t map_size, size_t x, size_t y, Direction dir) {
    return cache[(x * map_size * 4) + (y * 4) + dir];
}

void emit_light_ray(int64_t * cache, int64_t * visited, rArray_string * map, size_t const map_size, size_t x, size_t y, Direction dir) {
    // The light ray has exited the map!
    if (x >= map_size || y >= map_size) {
        return;
    }

    // If we've visited this location headed in this direction before, break
    if (is_cached(cache, map_size, x, y, dir)) {
        return;
    }

    energise_tile(cache, visited, map_size, x, y, dir);

    char const c = r_array_get_string(map, y)[x];
    // The light ray has exited the map
    if (c == '\0') {
        return;

    // The light ray continues in its current direction
    } else if (c == '.') {
        size_t new_x = x;
        size_t new_y = y;

        switch (dir) {
        case UP:
            new_y = y - 1;
            break;
        case RIGHT:
            new_x = x + 1;
            break;
        case DOWN:
            new_y = y + 1;
            break;
        case LEFT:
            new_x = x - 1;
            break;
        }

        emit_light_ray(cache, visited, map, map_size, new_x, new_y, dir);

    // The light ray gets reflected
    } else if (c == '/') {
        size_t new_x = x;
        size_t new_y = y;
        Direction new_dir = dir;

        switch (dir) {
        case UP:
            new_x = x + 1;
            new_dir = RIGHT;
            break;
        case RIGHT:
            new_y = y - 1;
            new_dir = UP;
            break;
        case DOWN:
            new_x = x - 1;
            new_dir = LEFT;
            break;
        case LEFT:
            new_y = y + 1;
            new_dir = DOWN;
            break;
        }

        emit_light_ray(cache, visited, map, map_size, new_x, new_y, new_dir);
    // The light ray gets reflected
    } else if (c == '\\') {
        size_t new_x = x;
        size_t new_y = y;
        Direction new_dir = dir;

        switch (dir) {
        case UP:
            new_x = x - 1;
            new_dir = LEFT;
            break;
        case RIGHT:
            new_y = y + 1;
            new_dir = DOWN;
            break;
        case DOWN:
            new_x = x + 1;
            new_dir = RIGHT;
            break;
        case LEFT:
            new_y = y - 1;
            new_dir = UP;
            break;
        }

        emit_light_ray(cache, visited, map, map_size, new_x, new_y, new_dir);

    // The light gets split, potentially
    } else if (c == '-') {
        if (dir == RIGHT || dir == LEFT) {
            size_t new_x = x;
            size_t new_y = y;

            switch (dir) {
            case UP:
                new_y = y - 1;
                break;
            case RIGHT:
                new_x = x + 1;
                break;
            case DOWN:
                new_y = y + 1;
                break;
            case LEFT:
                new_x = x - 1;
                break;
            }

            emit_light_ray(cache, visited, map, map_size, new_x, new_y, dir);
        } else {
            emit_light_ray(cache, visited, map, map_size, x + 1, y, RIGHT);
            emit_light_ray(cache, visited, map, map_size, x - 1, y, LEFT);
        }

    // The light gets split, potentially
    } else if (c == '|') {
        if (dir == UP || dir == DOWN) {
            size_t new_x = x;
            size_t new_y = y;

            switch (dir) {
            case UP:
                new_y = y - 1;
                break;
            case RIGHT:
                new_x = x + 1;
                break;
            case DOWN:
                new_y = y + 1;
                break;
            case LEFT:
                new_x = x - 1;
                break;
            }

            emit_light_ray(cache, visited, map, map_size, new_x, new_y, dir);
        } else {
            emit_light_ray(cache, visited, map, map_size, x, y - 1, UP);
            emit_light_ray(cache, visited, map, map_size, x, y + 1, DOWN);
        }
    }
}

int64_t sum_energised_tiles(int64_t * visited, size_t map_size) {
    int64_t sum = 0;
    for (size_t i = 0; i < map_size * map_size; ++i) {
        if (visited[i] != 0) {
            ++sum;
        }
    }

    return sum;
}

int64_t part_one(char const * filename, size_t const map_size) {
    FILE * file = r_file_open(filename);

    rArray_string * map = r_array_new_string(map_size);

    size_t const buffer_size = 128;
    char buffer[buffer_size];
    while (fgets(buffer, buffer_size, file) != NULL) {
        r_array_append_string(map, r_string_copy_until(buffer, '\n'));
    }

    fclose(file);

    size_t n_directions = 4;
    int64_t * cache = calloc(map_size * map_size * n_directions, sizeof(int64_t));
    int64_t * visited = calloc(map_size * map_size, sizeof(int64_t));
    emit_light_ray(cache, visited, map, map_size, 0, 0, RIGHT);

    free(cache);
    r_array_free_string(&map);

    int64_t sum = sum_energised_tiles(visited, map_size);

    free(visited);

    return sum;
}

int64_t part_two(char const * filename, size_t const map_size) {
    FILE * file = r_file_open(filename);

    rArray_string * map = r_array_new_string(map_size);

    size_t const buffer_size = 128;
    char buffer[buffer_size];
    while (fgets(buffer, buffer_size, file) != NULL) {
        r_array_append_string(map, r_string_copy_until(buffer, '\n'));
    }

    fclose(file);

    size_t n_directions = 4;
    int64_t * cache = calloc(map_size * map_size * n_directions, sizeof(int64_t));
    int64_t * visited = calloc(map_size * map_size, sizeof(int64_t));

    int64_t max_sum = 0;

    for (size_t x = 0; x < map_size; ++x) {
        emit_light_ray(cache, visited, map, map_size, x, 0, DOWN);

        int64_t sum = sum_energised_tiles(visited, map_size);
        if (sum > max_sum) {
            max_sum = sum;
        }

        for (size_t i = 0; i < map_size * map_size * n_directions; ++i) {
            cache[i] = 0;
        }
        for (size_t i = 0; i < map_size * map_size; ++i) {
            visited[i] = 0;
        }
    }

    for (size_t x = 0; x < map_size; ++x) {
        emit_light_ray(cache, visited, map, map_size, x, map_size - 1, UP);

        int64_t sum = sum_energised_tiles(visited, map_size);
        if (sum > max_sum) {
            max_sum = sum;
        }

        for (size_t i = 0; i < map_size * map_size * n_directions; ++i) {
            cache[i] = 0;
        }
        for (size_t i = 0; i < map_size * map_size; ++i) {
            visited[i] = 0;
        }
    }

    for (size_t y = 0; y < map_size; ++y) {
        emit_light_ray(cache, visited, map, map_size, 0, y, RIGHT);

        int64_t sum = sum_energised_tiles(visited, map_size);
        if (sum > max_sum) {
            max_sum = sum;
        }

        for (size_t i = 0; i < map_size * map_size * n_directions; ++i) {
            cache[i] = 0;
        }
        for (size_t i = 0; i < map_size * map_size; ++i) {
            visited[i] = 0;
        }
    }

    for (size_t y = 0; y < map_size; ++y) {
        emit_light_ray(cache, visited, map, map_size, map_size - 1, y, LEFT);

        int64_t sum = sum_energised_tiles(visited, map_size);
        if (sum > max_sum) {
            max_sum = sum;
        }

        for (size_t i = 0; i < map_size * map_size * n_directions; ++i) {
            cache[i] = 0;
        }
        for (size_t i = 0; i < map_size * map_size; ++i) {
            visited[i] = 0;
        }
    }

    free(cache);
    free(visited);
    r_array_free_string(&map);

    return max_sum;
}