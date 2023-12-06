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
