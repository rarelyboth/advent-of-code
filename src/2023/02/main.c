#include "rlib/file.h"
#include "rlib/string.h"

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
} Match;

Match *initMatch(int red, int green, int blue) {
    Match *match = malloc(sizeof(Match));

    match->red = red;
    match->green = green;
    match->blue = blue;

    return match;
}

bool matchPossible(const Match *match, int maxRed, int maxGreen, int maxBlue) {
    if (match->red > maxRed || match->green > maxGreen || match->blue > maxBlue) {
        return false;
    }

    return true;
}

int matchPower(Match match) {
    return match.red * match.green * match.blue;
}

typedef struct {
    int id;
    int nMatches;
    Match **matches;
} Game;

Game *initGame(int id) {
    Game *game = malloc(sizeof(Game));
    game->id = id;
    game->matches = malloc(0 * sizeof(Match));

    return game;
}

void freeGame(Game *game) {
    for (int i = 0; i < game->nMatches; ++i) {
        free(*(game->matches + i));
    }

    free(game->matches);
}

void addMatchToGame(Game *game, Match *match) {
    game->nMatches += 1;
    game->matches = realloc(game->matches, game->nMatches * sizeof(Match));
    if (game->matches == NULL) {
        fprintf(stderr, "Failed to add match to game!");
        exit(EXIT_FAILURE);
    }
    *(game->matches + game->nMatches - 1) = match;
}

int gamePower(Game *game) {
    Match max;
    max.red = 0;
    max.green = 0;
    max.blue = 0;

    for (int i = 0; i < game->nMatches; ++i) {
        Match match = **(game->matches + i);

        if (match.red > max.red) {
            max.red = match.red;
        }
        if (match.green > max.green) {
            max.green = match.green;
        }
        if (match.blue > max.blue) {
            max.blue = match.blue;
        }
    }

    return matchPower(max);
}

int partOne();
int partTwo();

int main() {
    printf("Part one: %i\n", partOne());
    printf("Part two: %i\n", partTwo());

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

Match *parseMatch(const char *matchString) {
    // Find the first cube count
    char *itNum = strpbrk(matchString, "0123456789");
    char *itColour = strpbrk(matchString, "rgb");

    Match *match = initMatch(0, 0, 0);
    do {
        // Extract & increment the cube count
        int numCubes = strtol(itNum, &itNum, 10);
        switch (*itColour) {
        case 'r':
            match->red += numCubes;
            break;
        case 'g':
            match->green += numCubes;
            break;
        case 'b':
            match->blue += numCubes;
            break;
        default:
            fprintf(stderr, "Invalid cube colour: %c", *itColour);
            exit(EXIT_FAILURE);
        }

        // Find the next cube count
        itNum = strpbrk(itNum, "0123456789");
        itColour = strpbrk(itColour + 3, "rgb"); // Increment by 3 because of "red"
    } while (itNum != NULL && itColour != NULL);

    return match;
}

Game *parseGame(const char *gameString) {
    int id = parseGameID(gameString);

    Game *game = initGame(id);

    // Assumes games start after the first occurrence of ':'
    char *itStart = strchr(gameString, ':');
    char *matchString = calloc(strlen(gameString) + 1, sizeof(char));
    do {
        ++itStart;
        copyToUntil(itStart, matchString, ';');

        Match *match = parseMatch(matchString);
        addMatchToGame(game, match);

        // Find the start of the next game
        itStart = strchr(itStart, ';');
    } while (itStart != NULL);

    free(matchString);

    return game;
}

int partOne() {
    FILE *file = rFileOpen("input.txt");

    int sum = 0;
    const int bufferSize = 256;
    char buffer[bufferSize];
    while(fgets(buffer, bufferSize, file) != NULL) {
        // Extract the line
        char *line = rStringCopyUntil(buffer, '\n');

        // Parse the game
        Game *game = parseGame(line);

        // Check if the game was possible
        bool possible = true;
        for (int i = 0; i < game->nMatches; ++i) {
            if (!matchPossible(*(game->matches + i), MAX_RED, MAX_GREEN, MAX_BLUE)) {
                possible = false;
                break;
            }
        }

        // If the game was possible, sum its ID
        if (possible) {
            sum += game->id;
        }

        // Dealloc
        free(line);
        freeGame(game);
    }

    fclose(file);

    return sum;
}

int partTwo() {
    FILE *file = rFileOpen("input.txt");

    int sum = 0;
    const int bufferSize = 256;
    char buffer[bufferSize];
    while(fgets(buffer, bufferSize, file) != NULL) {
        // Extract the line
        char *line = rStringCopyUntil(buffer, '\n');

        // Parse the game
        Game *game = parseGame(line);

        // Sum the power of the game
        sum += gamePower(game);

        // Dealloc
        free(line);
        freeGame(game);
    }

    fclose(file);

    return sum;
}