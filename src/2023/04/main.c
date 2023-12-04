#include "rlib/file.h"
#include "rlib/string.h"

#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include <string.h>
#include <assert.h>
#include <stdbool.h>

#define DIGITS "0123456789"

typedef struct {
    uint8_t size;
    uint8_t elements;
    uint8_t *set;
} rSet;

rSet *rSetNew(uint8_t elements) {
    rSet *set = malloc(sizeof(rSet));

    set->size = elements;
    set->elements = 0;
    set->set = calloc(elements, sizeof(uint8_t));

    return set;
}

void rSetAdd(rSet *set, uint8_t element) {
    if (set->elements == set->size) {
        set->size *= 2;
        set->set = realloc(set->set, set->size * sizeof(uint8_t));
        assert(set->set != NULL);
    }

    bool inSet = false;
    for (int i = 0; i < set->elements; ++i) {
        if (*(set->set + i) == element) {
            inSet = true;
            break;
        }
    }

    if (!inSet) {
        *(set->set + set->elements) = element;
        ++(set->elements);
    }
}

bool rSetIncludes(rSet *set, uint8_t element) {
    for (int i = 0; i < set->elements; ++i) {
        if (*(set->set) == element) {
            return true;
        }
    }

    return false;
}

void rSetPrint(rSet *set) {
    for (int i = 0; i < set->elements; ++i) {
        fprintf(stderr, "%i, ", *(set->set + i));
    }
}

rSet *rSetIntersection(rSet *a, rSet *b) {
    uint8_t maxIntersectionElements = a->elements < b->elements ? a->elements : b->elements;

    rSet *intersection = rSetNew(maxIntersectionElements);

    for (int i = 0; i < a->elements; ++i) {
        uint8_t aElement = *(a->set + i);

        bool inBoth = false;
        for (int j = 0; j < b->elements; ++j) {
            uint8_t bElement = *(b->set + j);

            if (aElement == bElement) {
                inBoth = true;
                break;
            }
        }

        if (inBoth) {
            rSetAdd(intersection, aElement);
        }
    }

    return intersection;
}

void rSetFree(rSet *set) {
    free(set->set);
    free(set);
}

typedef struct {
    int n;
    rSet *winningNumbers;
    rSet *ownNumbers;
} Card;

Card *cardNew(uint8_t maxNumbers) {
    Card *card = calloc(1, sizeof(Card));

    card->n = 1;
    card->winningNumbers = rSetNew(maxNumbers);
    card->ownNumbers = rSetNew(maxNumbers);

    return card;
}

uint8_t cardsWon(Card *card) {
    rSet *sharedNumbers = rSetIntersection(card->winningNumbers, card->ownNumbers);
    uint8_t cardsWon = sharedNumbers->elements;

    rSetFree(sharedNumbers);

    return cardsWon;
}

uint64_t cardScore(Card *card) {
    rSet *sharedNumbers = rSetIntersection(card->winningNumbers, card->ownNumbers);
    uint64_t score = 0;

    if (sharedNumbers->elements > 0) {
        score = 1 << (sharedNumbers->elements - 1);
    }

    rSetFree(sharedNumbers);

    return score;
}

void cardFree(Card *card) {
    rSetFree(card->winningNumbers);
    rSetFree(card->ownNumbers);
    free(card);
}

uint64_t partOne();
uint64_t partTwo();

int main() {
    printf("Part One: %" PRIu64 "\n", partOne());
    printf("Part Two: %" PRIu64 "\n", partTwo());

    return 0;
}

void parseNumberSet(rSet *set, char *line) {
    char *it = strpbrk(line, DIGITS);

    while (it != NULL) {
        uint8_t value = strtol(it, &it, 10);
        rSetAdd(set, value);

        it = strpbrk(it, DIGITS);
    }
}

Card *parseCard(const char *line) {
    char *winningNumbersStart = strchr(line, ':');
    char *winningNumbersString = rStringCopyUntil(winningNumbersStart, '|');
    char *ownNumbersString = strchr(line, '|');

    Card *card = cardNew(32);
    parseNumberSet(card->winningNumbers, winningNumbersString);
    parseNumberSet(card->ownNumbers, ownNumbersString);

    return card;
}

uint64_t partOne() {
    FILE *file = rFileOpen("input.txt");

    const uint8_t nCards = 223;
    Card **cards = calloc(nCards, sizeof(Card));

    const uint8_t bufferSize = 255;
    char buffer[bufferSize];
    for (int i = 0; fgets(buffer, bufferSize, file) != NULL; ++i) {
        // Extract the line
        const char *line = rStringCopyUntil(buffer, '\n');

        // Parse the card
        *(cards + i) = parseCard(line);
    }

    uint64_t sum = 0;
    for (int i = 0; i < nCards; ++i) {
        sum += cardScore(*(cards + i));
        cardFree(*(cards + i));
    }
    fclose(file);

    return sum;
}

uint64_t countCards(Card **cards, uint8_t nCards) {
    uint64_t sum = 0;

    for (int i = 0; i < nCards; ++i) {
        Card *aCard = *(cards + i);
        uint64_t nCardsWon = cardsWon(aCard);

        for (int j = i + 1; j < i + 1 + nCardsWon; ++j) {
            Card *bCard = *(cards + j);
            bCard->n += aCard->n;
        }

        sum += aCard->n;
    }

    return sum;
}

uint64_t partTwo() {
    FILE *file = rFileOpen("input.txt");

    const uint8_t nCards = 223;
    Card **cards = calloc(nCards, sizeof(Card));

    const uint8_t bufferSize = 255;
    char buffer[bufferSize];
    for (int i = 0; fgets(buffer, bufferSize, file) != NULL; ++i) {
        // Extract the line
        const char *line = rStringCopyUntil(buffer, '\n');

        // Parse the card
        *(cards + i) = parseCard(line);
    }

    uint64_t sum = countCards(cards, nCards);

    for (int i = 0; i < nCards; ++i) {
        cardFree(*(cards + i));
    }
    free(cards);
    fclose(file);

    return sum;
}