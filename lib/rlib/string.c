#include "string.h"

#include <stdlib.h>
#include <string.h>

char *rStringCopyUntil(const char *from, char until) {
    const char *itEnd = strchr(from, until);
    size_t lineLength = strnlen(from, itEnd - from);

    char *line = calloc(lineLength + 1, sizeof(char));
    strncpy(line, from, lineLength + 1);

    return line;
}

void rStringParseNumbers(char const * const line, int64_t * into) {
    char * it = strpbrk(line, "0123456789");
    for (int i = 0; it != NULL; ++i) {
        *(into + i) = strtol(it, &it, 10);

        it = strpbrk(it, "0123456789");
    }
}