#include "rlib/string.h"
#include "rlib/file.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <inttypes.h>

typedef struct node {
    uint16_t id;
    struct node * leftNode;
    struct node * rightNode;
} Node;

Node * node_new(uint16_t id, Node * leftNode, Node * rightNode) {
    Node * node = calloc(1, sizeof(Node));

    node->id = id;
    node->leftNode = leftNode;
    node->rightNode = rightNode;

    return node;
}

int64_t part_one();
void part_two();

int main() {
    printf("Part One: %" PRIi64 "\n", part_one());
    part_two();

    return 0;
}

uint16_t parse_node_id(char const * string) {
    uint16_t const alphaBits = 5;

    uint16_t id = 0;
    for (char const * it = string; *it != '\0'; ++it) {
        uint16_t i = *it - 'A';
        id = id << alphaBits | i;
    }

    return id;
}

Node * get_or_create_node(Node * nodes[], size_t * nNodes, uint16_t id) {
    for (size_t i = 0; i < *nNodes; ++i) {
        if (nodes[i]->id == id) {
            return nodes[i];
        }
    }

    Node * node = node_new(id, NULL, NULL);
    nodes[(*nNodes)++] = node;

    return node;
}

int64_t count_steps_to_node_id(char const * instructions, Node * start_node, uint16_t id) {
    int64_t steps = 0;
    size_t n_instructions = strlen(instructions);

    Node * it = start_node;
    for (size_t i = 0; it->id != id; i = (i + 1) % n_instructions, ++steps) {
        switch (instructions[i]) {
        case 'L':
            it = it->leftNode;
            break;
        case 'R':
            it = it->rightNode;
            break;
        default:
            fprintf(stderr, "Unknown instruction!");
            exit(EXIT_FAILURE);
        }
    }

    return steps;
}

int64_t count_steps_to_node_mask(char const * instructions, Node * start_node) {
    int64_t steps = 0;
    size_t n_instructions = strlen(instructions);
    uint16_t alphaMask = 0b11111;


    Node * it = start_node;
    for (size_t i = 0; ((it->id & alphaMask) != (0b11001 & alphaMask)); i = (i + 1) % n_instructions, ++steps) {
        switch (instructions[i]) {
        case 'L':
            it = it->leftNode;
            break;
        case 'R':
            it = it->rightNode;
            break;
        default:
            fprintf(stderr, "Unknown instruction!");
            exit(EXIT_FAILURE);
        }
    }

    return steps;
}

void count_cycles(char const * instructions, Node * nodes[], size_t * nNodes) {
    uint16_t alphaMask = 0b11111;

    for (size_t i = 0; i < *nNodes; ++i) {
        Node * node = nodes[i];
        if ((node->id & alphaMask) == (0b00000 & alphaMask)) {
            int64_t steps = count_steps_to_node_mask(instructions, node);
            printf("Node %" PRIu16 ": %" PRIi64 "\n", node->id, steps);
        }
    }
}

int64_t part_one() {
    FILE * file = rFileOpen("input.txt");
    if (file == NULL) {
        fprintf(stderr, "Failed to open file!");
        exit(EXIT_FAILURE);
    }

    size_t const bufferSize = 512;
    char buffer[bufferSize] = {0};

    // Read the instruction line
    char * instructions;
    {
        fgets(buffer, bufferSize, file);
        instructions = rStringCopyUntil(buffer, '\n');
    }

    size_t const maxNodes = 800;
    size_t nNodes = 0;

    // Read the nodes
    // Assume the newline has been removed from the input file for convenience
    Node * nodes[maxNodes] = {NULL};
    Node * start_node = NULL;
    {
        char *line;
        char *nodeString;
        char *it;
        char *leftNodeString;
        char *rightNodeString;

        while (fgets(buffer, bufferSize, file) != NULL) {
            line = rStringCopyUntil(buffer, '\n');

            // Primary node
            nodeString = rStringCopyUntil(line, ' ');
            uint16_t nodeID = parse_node_id(nodeString);
            Node *node = get_or_create_node(nodes, &nNodes, nodeID);
            if (nodeID == 0) {
                start_node = node;
            }

            // Left node
            it = strchr(line, '(') + 1;
            leftNodeString = rStringCopyUntil(it, ',');
            uint16_t leftNodeID = parse_node_id(leftNodeString);
            Node *leftNode = get_or_create_node(nodes, &nNodes, leftNodeID);

            // Right node
            it = strchr(it, ' ') + 1;
            rightNodeString = rStringCopyUntil(it, ')');
            uint16_t rightNodeID = parse_node_id(rightNodeString);
            Node *rightNode = get_or_create_node(nodes, &nNodes, rightNodeID);

            node->leftNode = leftNode;
            node->rightNode = rightNode;
        }

        // Dealloc
        {
            free(line);
            free(nodeString);
            free(leftNodeString);
            free(rightNodeString);
        }
    }

    // Count the number of steps required to reach ZZZ
    uint16_t zzz_id = parse_node_id("ZZZ");
    int64_t steps = count_steps_to_node_id(instructions, start_node, zzz_id);

    // Dealloc
    fclose(file);
    free(instructions);

    return steps;
}

void part_two() {
    FILE * file = rFileOpen("input.txt");
    if (file == NULL) {
        fprintf(stderr, "Failed to open file!");
        exit(EXIT_FAILURE);
    }

    size_t const bufferSize = 512;
    char buffer[bufferSize] = {0};

    // Read the instruction line
    char * instructions;
    {
        fgets(buffer, bufferSize, file);
        instructions = rStringCopyUntil(buffer, '\n');
    }

    size_t const maxNodes = 800;
    size_t nNodes = 0;

    // Read the nodes
    // Assume the newline has been removed from the input file for convenience
    Node * nodes[maxNodes] = {NULL};
    Node * start_node = NULL;
    {
        char *line;
        char *nodeString;
        char *it;
        char *leftNodeString;
        char *rightNodeString;

        while (fgets(buffer, bufferSize, file) != NULL) {
            line = rStringCopyUntil(buffer, '\n');

            // Primary node
            nodeString = rStringCopyUntil(line, ' ');
            uint16_t nodeID = parse_node_id(nodeString);
            Node *node = get_or_create_node(nodes, &nNodes, nodeID);
            if (nodeID == 0) {
                start_node = node;
            }

            // Left node
            it = strchr(line, '(') + 1;
            leftNodeString = rStringCopyUntil(it, ',');
            uint16_t leftNodeID = parse_node_id(leftNodeString);
            Node *leftNode = get_or_create_node(nodes, &nNodes, leftNodeID);

            // Right node
            it = strchr(it, ' ') + 1;
            rightNodeString = rStringCopyUntil(it, ')');
            uint16_t rightNodeID = parse_node_id(rightNodeString);
            Node *rightNode = get_or_create_node(nodes, &nNodes, rightNodeID);

            node->leftNode = leftNode;
            node->rightNode = rightNode;
        }

        // Dealloc
        {
            free(line);
            free(nodeString);
            free(leftNodeString);
            free(rightNodeString);
        }
    }

    // Count the number of steps required to reach ZZZ
    uint16_t zzz_id = parse_node_id("ZZZ");
    count_cycles(instructions, nodes, &nNodes);

    // Dealloc
    fclose(file);
    free(instructions);
}
