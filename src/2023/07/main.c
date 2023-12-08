#include "rlib/file.h"
#include "rlib/string.h"

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <inttypes.h>

typedef struct {
    int64_t handType;
    int64_t handValue;
    int64_t bid;
    int64_t rank;
} Play;

Play playNew(int64_t handType, int64_t handValue, int64_t bid, int64_t rank) {
    Play play = {0};

    play.handType = handType;
    play.handValue = handValue;
    play.bid = bid;
    play.rank = rank;

    return play;
}

int64_t partOne();
int64_t partTwo();

int main() {
    printf("Part One: %" PRIi64 "\n", partOne());
    printf("Part Two: %" PRIi64 "\n", partTwo());
}

int64_t getCardValue(char const card) {
    int64_t value = 0;
    switch (card) {
    case 'A':
        value = 14;
        break;
    case 'K':
        value = 13;
        break;
    case 'Q':
        value = 12;
        break;
    case 'J':
        value = 11;
        break;
    case 'T':
        value = 10;
        break;
    default:
        value = card - '0';
        break;
    }

    return value;
}

int64_t getJokerCardValue(char const card) {
    int64_t value = 0;
    switch (card) {
    case 'A':
        value = 13;
        break;
    case 'K':
        value = 12;
        break;
    case 'Q':
        value = 11;
        break;
    case 'J':
        value = 1;
        break;
    case 'T':
        value = 10;
        break;
    default:
        value = card - '0';
        break;
    }

    return value;
}

int64_t parseHandType(char const * string) {
    size_t const numCards = 14;
    int64_t cards[numCards] = {0};

    char const * it = string;
    for (; *it != '\0'; ++it) {
        int64_t card = getCardValue(*it) - 2; // Card 2 -> 0

        cards[card] = cards[card] << 1 | 1;
    }

    int64_t handType = 0;
    for (size_t i = 0; i < numCards; ++i) {
        handType += cards[i];
    }

    return handType;
}

int64_t parseJokerHandType(char const * string) {
    size_t const numCards = 14;
    int64_t cards[numCards] = {0};

    int64_t nJokers = 0;
    char const * it = string;
    for (; *it != '\0'; ++it) {
        char card = *it;
        if (card == 'J') {
            ++nJokers;
        }

        int64_t cardIndex = getJokerCardValue(*it) - 1; // Card 1 -> 0
        cards[cardIndex] = cards[cardIndex] << 1 | 1;
    }

    int64_t mostCards = 0;
    size_t iMostCards = 0;
    if (nJokers < 5) {
        for (size_t i = 1; i < numCards; ++i) {
            if (cards[i] > mostCards) {
                mostCards = cards[i];
                iMostCards = i;
            }
        }

        for (int64_t i = 0; i < nJokers; ++i) {
            cards[iMostCards] = cards[iMostCards] << 1 | 1;
            cards[0] = cards[0] >> 1;
        }
    }

    int64_t handType = 0;
    for (size_t i = 0; i < numCards; ++i) {
        handType += cards[i];
    }

    return handType;
}

int64_t int2Pow(int64_t exponent) {
    return 1 << exponent;
}

int64_t parseHandValue(char const * string) {
    int64_t const handLength = strlen(string);

    int64_t handValue = 0;
    char const * it = string;
    for (size_t i = 0; *it != '\0'; ++it, ++i) {
        int64_t cardValue = getCardValue(*it);

        handValue = (100 * handValue) + (cardValue);
    }

    return handValue;
}

int64_t parseJokerHandValue(char const * string) {
    int64_t const handLength = strlen(string);

    int64_t handValue = 0;
    char const * it = string;
    for (size_t i = 0; *it != '\0'; ++it, ++i) {
        int64_t cardValue = getJokerCardValue(*it);

        handValue = (100 * handValue) + (cardValue);
    }

    return handValue;
}

int64_t parseBid(char const * string) {
    return strtol(string, NULL, 10);
}

int compareHands(void const * a, void const * b) {
    Play * aPlay = (Play *)a;
    Play * bPlay = (Play *)b;

    if (aPlay->handType == bPlay->handType) {
        if (aPlay->handValue == bPlay->handValue) {
            return 0;
        }
        return aPlay->handValue < bPlay->handValue ? -1 : 1;
    }
    return aPlay->handType < bPlay->handType ? -1 : 1;
}

int64_t partOne() {
    FILE * file = rFileOpen("input.txt");

    int16_t const numPlays = 1000;
    Play * plays = calloc(numPlays, sizeof(Play));

    uint8_t const bufferSize = 255;
    char buffer[bufferSize];
    for (size_t i = 0; fgets(buffer, bufferSize, file) != NULL; ++i) {
        char const * line = rStringCopyUntil(buffer, '\n');
        char const * handString = rStringCopyUntil(line, ' ');
        char const * bidString = strchr(line, ' ') + 1;

        // Parse the hand & bid
        int64_t handType = parseHandType(handString);
        int64_t handValue = parseHandValue(handString);
        int64_t bid = parseBid(bidString);

        Play play = playNew(handType, handValue, bid, 0);
        *(plays + i) = play;
    }

    qsort(plays, numPlays, sizeof(Play), &compareHands);

    int64_t total = 0;
    for (size_t i = 0; i < numPlays; ++i) {
        plays[i].rank = i + 1;
        total += plays[i].rank * plays[i].bid;
    }

    free(plays);

    return total;
}

int64_t partTwo() {
    FILE * file = rFileOpen("input.txt");

    int16_t const numPlays = 1000;
    Play * plays = calloc(numPlays, sizeof(Play));

    uint8_t const bufferSize = 255;
    char buffer[bufferSize];
    for (size_t i = 0; fgets(buffer, bufferSize, file) != NULL; ++i) {
        char const * line = rStringCopyUntil(buffer, '\n');
        char const * handString = rStringCopyUntil(line, ' ');
        char const * bidString = strchr(line, ' ') + 1;

        // Parse the hand & bid
        int64_t handType = parseJokerHandType(handString);
        int64_t handValue = parseJokerHandValue(handString);
        int64_t bid = parseBid(bidString);

        Play play = playNew(handType, handValue, bid, 0);
        *(plays + i) = play;

        fprintf(stderr, "Hand %s -> Type %" PRIi64 " -> Value %" PRIi64 "\n", handString, handType, handValue);
    }

    qsort(plays, numPlays, sizeof(Play), &compareHands);

    int64_t total = 0;
    for (size_t i = 0; i < numPlays; ++i) {
        plays[i].rank = i + 1;
        total += plays[i].rank * plays[i].bid;
    }

    free(plays);

    return total;
}