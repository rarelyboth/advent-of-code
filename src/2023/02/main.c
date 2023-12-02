#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define MAX_RED 12
#define MAX_GREEN 13
#define MAX_BLUE 14

typedef struct {
    int red;
    int green;
    int blue;
} Game;

Game initGame(int red, int green, int blue) {
    Game game;
    game.red = red;
    game.green = green;
    game.blue = blue;

    return game;
}

bool gamePossible(const Game game, int maxRed, int maxGreen, int maxBlue) {
    if (game.red > maxRed || game.green > maxGreen || game.blue > maxBlue) {
        return false;
    }

    return true;
}

int partOne();

int main() {
    printf("Part one: %i\n", partOne());

    return 0;
}

char *copyUntil(const char *from, char until) {
    const char *itEnd = strchr(from, until);
    unsigned long lineLength = strnlen(from, itEnd - from);

    char *line = calloc(lineLength + 1, sizeof(char));
    strlcpy(line, from, lineLength + 1);

    return line;
}

void copyToUntil(const char *from, char *to, char until) {
    const char *itEnd = strchr(from, until);
    unsigned long lineLength = strnlen(from, itEnd - from);

    strlcpy(to, from, lineLength + 1);
}

int parseGameID(const char *line) {
    // Expect all game IDs to be of the form "Game ###:"
    char *itID = strpbrk(line, "0123456789");

    return strtol(itID, NULL, 10);
}

bool parseDrawPossible(const char *drawLine) {
    // Find the first cube count
    char *itNum = strpbrk(drawLine, "0123456789");
    char *itColour = strpbrk(drawLine, "rgb");

    Game game = initGame(0, 0, 0);
    do {
        // Extract & increment the cube count
        int numCubes = strtol(itNum, &itNum, 10);
        switch (*itColour) {
        case 'r':
            game.red += numCubes;
            break;
        case 'g':
            game.green += numCubes;
            break;
        case 'b':
            game.blue += numCubes;
            break;
        default:
            fprintf(stderr, "Invalid cube colour: %c", *itColour);
            exit(EXIT_FAILURE);
        }

        if (!gamePossible(game, MAX_RED, MAX_GREEN, MAX_BLUE)) {
            return false;
        }

        // Find the next cube count
        itNum = strpbrk(itNum, "0123456789");
        itColour = strpbrk(itColour + 3, "rgb"); // Increment by 3 because of "red"
    } while (itNum != NULL && itColour != NULL);

    return true;
}

bool parseGamePossible(const char *line) {
    // Assumes games start after the first occurrence of ':'
    char *itStart = strchr(line, ':');

    char *draw = calloc(strlen(line) + 1, sizeof(char));

    do {
        ++itStart;
        copyToUntil(itStart, draw, ';');

        if (!parseDrawPossible(draw)) {
            return false;
        }

        // Find the start of the next game
        itStart = strchr(itStart, ';');
    } while (itStart != NULL);

    free(draw);

    return true;
}

int partOne() {
    FILE *file = fopen("input.txt", "r");
    if (file == NULL) {
        fprintf(stderr, "Failed to open file!");
        exit(EXIT_FAILURE);
    }

    int sum = 0;
    const int bufferSize = 256;
    char buffer[bufferSize];
    while(fgets(buffer, bufferSize, file) != NULL) {
        // Extract the line
        char *line = copyUntil(buffer, '\n');

        // Identify the game associated with this line
        int id = parseGameID(line);

        // If the game was possible, increment the sum by the game's ID
        if (parseGamePossible(line)) {
            sum += id;
        }

        // Dealloc
        free(line);
    }

    fclose(file);

    return sum;
}