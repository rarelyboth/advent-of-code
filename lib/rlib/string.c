#include "string.h"
#include "array.h"

#include <stdlib.h>
#include <string.h>
#include <assert.h>

char *rStringCopyUntil(const char *from, char until) {
    const char *itEnd = strchr(from, until);
    size_t lineLength = strnlen(from, itEnd - from);

    char *line = calloc(lineLength + 1, sizeof(char));
    strncpy(line, from, lineLength + 1);

    return line;
}

char * r_string_copy_until(char * from, char until) {
    const char *it_end = strchr(from, until);
    size_t length = strnlen(from, it_end - from);

    char * line = calloc(length + 1, sizeof(char));
    strncpy(line, from, length + 1);

    return line;
}

void r_string_append(char ** string, char * append) {
    size_t const string_len = strlen(*string);
    size_t const append_len = strlen(append);

    *string = realloc(string, (string_len + append_len + 1) * sizeof(char));
    assert(string != NULL);

    strncat(*string, append, 5);
}

void rStringParseNumbers(char const * const line, int64_t * into) {
    char * it = strpbrk(line, "0123456789");
    for (int i = 0; it != NULL; ++i) {
        *(into + i) = strtol(it, &it, 10);

        it = strpbrk(it, "0123456789");
    }
}

rArray_int64_t * r_string_parse_numbers(char * string) {
    rArray_int64_t * numbers = r_array_new_int64_t(strlen(string) / 2);

    char * it = strpbrk(string, "0123456789");
    for (int i = 0; it != NULL; ++i) {
        r_array_append_int64_t(numbers, strtol(it, &it, 10));

        it = strpbrk(it, "0123456789");
    }

    return numbers;
}

char * r_string_from_char(char character) {
    char * string = calloc(2, sizeof(char));
    string[0] = character;
    string[1] = '\0';
    return string;
}