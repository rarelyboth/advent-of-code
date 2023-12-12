#pragma once

#include "array.h"

#include <inttypes.h>

char *rStringCopyUntil(const char *from, char until);

void rStringParseNumbers(char const * const line, int64_t * into);

char * r_string_copy_until(char * from, char until);

void r_string_append(char ** string, char * append);

char * r_string_from_char(char character);

rArray_int64_t * r_string_parse_numbers(char * string);

