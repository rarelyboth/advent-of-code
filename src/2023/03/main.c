#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

typedef struct {
    int line;
    int start;
    int end;
    int value;
} Slice;

Slice *sliceNew(int line, int start, int end, int value) {
    assert(start <= end);

    Slice *slice = malloc(sizeof(Slice));

    slice->line = line;
    slice->start = start;
    slice->end = end;
    slice->value = value;

    return slice;
}

typedef struct {
    int nLines;
    int nParts;
    int nSymbols;
    Slice **parts;
    Slice **symbols;
} Schematic;

Schematic *schematicNew(int nLines) {
    assert(nLines > 0);

    Schematic *schematic = malloc(sizeof(Schematic));

    schematic->nLines = nLines;
    schematic->nParts = 0;
    schematic->nSymbols = 0;
    schematic->parts = calloc(0, sizeof(Slice *));
    schematic->symbols = calloc(0, sizeof(Slice *));

    return schematic;
}

void schematicFree(Schematic *schematic) {
    for (int i = 0; i < schematic->nParts; ++i) {
        free(*(schematic->parts + i));
    }
    free(schematic->parts);

    for (int i = 0; i < schematic->nSymbols; ++i) {
        free(*(schematic->symbols + i));
    }
    free(schematic->symbols);

    free(schematic);
}

void schematicAppendPart(Schematic *schematic, Slice *slice) {
    schematic->parts = realloc(schematic->parts, (schematic->nParts + 1) * sizeof(Slice *));
    if (schematic->parts == NULL) {
        fprintf(stderr, "Failed to append part to schematic!");
        exit(EXIT_FAILURE);
    }

    *(schematic->parts + schematic->nParts) = slice;
    ++schematic->nParts;
}

void schematicAppendSymbol(Schematic *schematic, Slice *slice) {
    schematic->symbols = realloc(schematic->symbols, (schematic->nSymbols + 1) * sizeof(Slice *));
    if (schematic->symbols == NULL) {
        fprintf(stderr, "Failed to append symbol to schematic!");
        exit(EXIT_FAILURE);
    }

    *(schematic->symbols + schematic->nSymbols) = slice;
    ++schematic->nSymbols;
}

int partOne();
int partTwo();

int main() {
    printf("Part one: %i\n", partOne());
    printf("Part two: %i\n", partTwo());

    return 0;
}

char *rStringCopyUntil(const char *from, char until) {
    const char *itEnd = strchr(from, until);
    unsigned long lineLength = strnlen(from, itEnd - from);

    char *line = calloc(lineLength + 1, sizeof(char));
    strncpy(line, from, lineLength + 1);

    return line;
}

FILE *rOpenFile(const char *filename) {
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        fprintf(stderr, "Failed to open file!");
        exit(EXIT_FAILURE);
    }

    return file;
}

void parseSchematicLine(Schematic *schematic, int lineNumber, const char *line) {
    // Find the first character of interest. Either a part number or a symbol
    char *it = strpbrk(line, "0123456789!@#$%^&*,/+-="); // Note: no '.'

    while (it != NULL) {
        if (strchr("0123456789", *it)) { // Current character's a number
            char *itEnd = NULL;
            int value = strtol(it, &itEnd, 10);

            Slice *slice = sliceNew(lineNumber, it - line, itEnd - line, value);
            schematicAppendPart(schematic, slice);

            it = itEnd;
        } else if (strchr("!@#$%^&*,/+-=", *it)) { // Current character's a symbol
            Slice *slice = sliceNew(lineNumber, it - line, it - line + 1, *it);
            schematicAppendSymbol(schematic, slice);

            ++it;
        }

        it = strpbrk(it, "0123456789!@#$%^&*,/+-="); // Note: no '.'
    }
}

int sumSchematicPartNumbers(Schematic *schematic) {
    int sum = 0;

    for (int i = 0; i < schematic->nSymbols; ++i) {
        Slice symbol = **(schematic->symbols + i);

        for (int j = 0; j < schematic->nParts; ++j) {
            Slice part = **(schematic->parts + j);

            if (part.line < symbol.line - 1) {
                continue;
            } else if (part.line > symbol.line + 1) {
                break;
            }

            if (part.start - 1 <= symbol.start && part.end >= symbol.start) {
                sum += part.value;
            }
        }
    }

    return sum;
}

int partOne() {
    FILE *file = rOpenFile("input.txt");

    Schematic *schematic = schematicNew(140);

    const int bufferSize = 256;
    char buffer[bufferSize];
    for (int i = 0; fgets(buffer, bufferSize, file) != NULL; ++i) {
        // Extract the line
        const char *line = rStringCopyUntil(buffer, '\n');

        // Parse the schematic
        parseSchematicLine(schematic, i, line);
    }

    int sum = sumSchematicPartNumbers(schematic);

    // Dealloc
    schematicFree(schematic);
    fclose(file);

    return sum;
}

int sumSchematicGearRatios(Schematic *schematic) {
    int sum = 0;

    for (int i = 0; i < schematic->nSymbols; ++i) {
        Slice symbol = **(schematic->symbols + i);

        if (symbol.value != '*') {
            continue;
        }

        int parts = 0;
        int gearRatio = 1;
        for (int j = 0; j < schematic->nParts; ++j) {
            Slice part = **(schematic->parts + j);

            if (part.line < symbol.line - 1) {
                continue;
            } else if (part.line > symbol.line + 1) {
                break;
            }

            if (part.start - 1 <= symbol.start && part.end >= symbol.start) {
                ++parts;
                gearRatio *= part.value;
            }
        }

        if (parts == 2) {
            sum += gearRatio;
        }
    }

    return sum;
}

int partTwo() {
    FILE *file = rOpenFile("input.txt");

    Schematic *schematic = schematicNew(140);

    const int bufferSize = 256;
    char buffer[bufferSize];
    for (int i = 0; fgets(buffer, bufferSize, file) != NULL; ++i) {
        // Extract the line
        const char *line = rStringCopyUntil(buffer, '\n');

        // Parse the schematic
        parseSchematicLine(schematic, i, line);
    }

    int sum = sumSchematicGearRatios(schematic);

    // Dealloc
    schematicFree(schematic);
    fclose(file);

    return sum;
}