#pragma once

#include <inttypes.h>

char *rStringCopyUntil(const char *from, char until);

void rStringParseNumbers(char const * const line, int64_t * into);

