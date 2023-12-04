#include "rlib/file.h"
#include "rlib/string.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

int partOne();
int partTwo();

int main() {
    printf("Part one: %i\n", partOne());
    printf("Part two: %i\n", partTwo());

    return 0;
}

char *readDigits(const char *string) {
    char *digits = "0123456789";

    int size = 0;
    char *digitString = calloc((strlen(string) + 1), sizeof(char));

    for (const char *it = string; *it != '\0'; ++it) {
        if (strchr(digits, *it) != NULL) {
            *(digitString + size) = *it;
            ++size;
        }
    }

    return digitString;
}

char *readNumbers(const char *string) {
    const int nDigits = 10;
    char numbers[nDigits][6] = {"zero", "one", "two", "three", "four", "five", "six", "seven", "eight", "nine"};
    char *digits = "0123456789";

    int size = 0;
    char *numString = calloc((strlen(string) + 1), sizeof(char));

    for (const char *it = string; *it != '\0'; ++it) {
        if (strchr(digits, *it) != NULL) {
            *(numString + size) = *it;
            ++size;
        } else {
            // Start at 1 to avoid "zero" as there are no ocurrences
            for (int i = 1; i < nDigits; ++i) {
                const char *number = *(numbers + i);
                const int numLen = strlen(number);

                if (strncmp(it, number, numLen) == 0) {
                    *(numString + size) = *(digits + i);
                    ++size;
                    it += numLen - 2; // -1 for ++it, -1 for overlaps "zerone"
                    break;
                }
            }
        }
    }

    return numString;
}

int partOne() {
    FILE *file = rFileOpen("input.txt");

    int sum = 0;
    const int bufferSize = 64;
    char buffer[bufferSize];
    while (fgets(buffer, bufferSize, file) != NULL) {
        // Extract the line
        char *line = rStringCopyUntil(buffer, '\n');

        // Extract numbers from the line
        char *numbersInLine = readDigits(line);
        if (strlen(numbersInLine) == 0) {
            continue;
        }

        // Extract the first and last digits
        char digitBuffer[3];
        digitBuffer[0] = *numbersInLine;
        digitBuffer[1] = *(numbersInLine + strlen(numbersInLine) - 1);

        // Convert the digits & sum
        sum += strtol(digitBuffer, NULL, 10);

        // Dealloc
        free(line);
        free(numbersInLine);
    }

    fclose(file);

    return sum;
}

int partTwo() {
    FILE *file = rFileOpen("input.txt");

    int sum = 0;
    const int bufferSize = 64;
    char buffer[bufferSize];
    while (fgets(buffer, bufferSize, file) != NULL) {
        // Extract the line
        char *line = rStringCopyUntil(buffer, '\n');

        // Extract numbers from the line
        char *numbersInLine = readNumbers(line);
        if (strlen(numbersInLine) == 0) {
            continue;
        }

        // Extract the first and last digits
        char digitBuffer[3];
        digitBuffer[0] = *numbersInLine;
        digitBuffer[1] = *(numbersInLine + strlen(numbersInLine) - 1);

        // Convert the digits & sum
        sum += strtol(digitBuffer, NULL, 10);

        // Dealloc
        free(line);
        free(numbersInLine);
    }

    fclose(file);

    return sum;
}
