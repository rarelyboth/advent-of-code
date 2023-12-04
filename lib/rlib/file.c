#include "file.h"

#include <stdlib.h>

FILE *rFileOpen(const char *filename) {
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        fprintf(stderr, "Failed to open file!");
        exit(EXIT_FAILURE);
    }

    return file;
}