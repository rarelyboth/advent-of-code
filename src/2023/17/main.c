#include "rlib/string.h"
#include "rlib/file.h"
#include "rlib/matrix.h"
#include "rlib/tree.h"
#include "rlib/graph.h"

#include <stdio.h>
#include <inttypes.h>

int64_t part_one(char const * filename, size_t const map_size);
int64_t part_two(char const * filename, size_t const map_size);

int main() {
    printf("Part One: %" PRIi64 "\n", part_one("input.txt", 141));
    printf("Part Two: %" PRIi64 "\n", part_two("input.txt", 141));

    return 0;
}

void construct_graph(rGraph ** graph, rMatrix_rGraphNode * map_h, rMatrix_rGraphNode * map_v, size_t const map_size, size_t const min_steps, size_t const max_steps) {
    for (size_t m = 0; m < map_size; ++m) {
        for (size_t n = 0; n < map_size; ++n) {
            int64_t up_weight = 0;
            int64_t down_weight = 0;
            int64_t left_weight = 0;
            int64_t right_weight = 0;

            for (size_t steps = min_steps; steps <= max_steps; ++steps) {
                rGraphNode * node_h = r_matrix_get_rGraphNode(map_h, m, n);
                rGraphNode * node_v = r_matrix_get_rGraphNode(map_v, m, n);

                // Up
                if (m - steps < map_size) {
                    rGraphNode * node_to = r_matrix_get_rGraphNode(map_h, m - steps, n);
                    up_weight += node_to->value;

                    rGraphArc * arc = r_graph_arc_new(node_v, node_to, up_weight);
                    r_graph_add_arc(*graph, arc);
                }

                // Down
                if (m + steps < map_size) {
                    rGraphNode * node_to = r_matrix_get_rGraphNode(map_h, m + steps, n);
                    down_weight += node_to->value;

                    rGraphArc * arc = r_graph_arc_new(node_v, node_to, down_weight);
                    r_graph_add_arc(*graph, arc);
                }

                // Left
                if (n - steps < map_size) {
                    rGraphNode * node_to = r_matrix_get_rGraphNode(map_v, m, n - steps);
                    left_weight += node_to->value;

                    rGraphArc * arc = r_graph_arc_new(node_h, node_to, left_weight);
                    r_graph_add_arc(*graph, arc);
                }

                // Right
                if (n + steps < map_size) {
                    rGraphNode * node_to = r_matrix_get_rGraphNode(map_v, m, n + steps);
                    right_weight += node_to->value;

                    rGraphArc * arc = r_graph_arc_new(node_h, node_to, right_weight);
                    r_graph_add_arc(*graph, arc);
                }
            }
        }
    }

    // Connect the start & end nodes
    rGraphNode * start_h = r_matrix_get_rGraphNode(map_h, 0, 0);
    rGraphNode * start_v = r_matrix_get_rGraphNode(map_v, 0, 0);

    r_graph_add_arc(*graph, r_graph_arc_new(start_h, start_v, 0));
    r_graph_add_arc(*graph, r_graph_arc_new(start_v, start_h, 0));

    rGraphNode * end_h = r_matrix_get_rGraphNode(map_h, 0, 0);
    rGraphNode * end_v = r_matrix_get_rGraphNode(map_v, 0, 0);

    r_graph_add_arc(*graph, r_graph_arc_new(end_h, end_v, 0));
    r_graph_add_arc(*graph, r_graph_arc_new(end_v, end_h, 0));

}

void dijkstra(rGraph * graph, rArray_int64_t * distances) {
    rBTree * visited = r_btree_new(4);

    int64_t n_visited = 0;

    while (n_visited < r_array_length_int64_t(distances)) {
        size_t min_id = 0;
        int64_t min_dist = INT64_MAX;
        for (size_t i = 0; i < r_array_length_int64_t(distances); ++i) {
            if (r_btree_search(visited, i) ) {
                continue;
            }

            int64_t value = r_array_get_int64_t(distances, i);
            if (value < min_dist) {
                min_dist = value;
                min_id = i;
            }
        }

        ++n_visited;
        r_btree_insert(visited, min_id);

        for (size_t i = 0; i < r_array_length_rArc(graph->arcs); ++i) {
            rGraphArc * arc = r_array_get_rArc(graph->arcs, i);

            if (arc->from->id != min_id || r_btree_search(visited, arc->to->id)) {
                continue;
            }

            int64_t dist = min_dist + arc->value;
            if (dist < r_array_get_int64_t(distances, arc->to->id)) {
                r_array_set_int64_t(distances, arc->to->id, dist);
            }
        }
    }
}

int64_t part_one(char const * filename, size_t const map_size) {
    FILE * file = r_file_open(filename);

    rMatrix_rGraphNode * map_h = r_matrix_new_rGraphNode(map_size, map_size);
    rMatrix_rGraphNode * map_v = r_matrix_new_rGraphNode(map_size, map_size);

    rGraph * graph = r_graph_new(map_size * map_size, map_size * map_size);

    size_t const buffer_size = 256;
    char buffer[buffer_size];
    for (size_t i = 0; fgets(buffer, buffer_size, file) != NULL; ++i) {
        char * line = r_string_copy_until(buffer, '\n');

        printf("%s\n", line);

        char * it = line;
        for (size_t j = 0; *it != '\0'; ++it, ++j) {
            rGraphNode * node_h = r_graph_node_new(i * map_size + j, *it - '0');
            rGraphNode * node_v = r_graph_node_new(map_size * map_size + i * map_size + j, *it - '0');

            r_matrix_set_rGraphNode(map_h, i, j, node_h);
            r_matrix_set_rGraphNode(map_v, i, j, node_v);

            r_graph_add_node(graph, node_h);
            r_graph_add_node(graph, node_v);
        }
    }

    // Construct the graph
    construct_graph(&graph, map_h, map_v, map_size, 1, 3);

    // Calculate the shortest paths
    rArray_int64_t * distances = r_array_new_int64_t(map_size * map_size * 2);
    for (size_t i = 0; i < map_size * map_size * 2; ++i) {
        r_array_append_int64_t(distances, INT64_MAX);
    }
    r_array_set_int64_t(distances, 0, 0);
    dijkstra(graph, distances);

    // 1040: too high
    // 1039: correct!
    // 1038: incorrect
    // 1037: incorrect
    // 1036: too low!!!!
    return r_array_get_int64_t(distances, (map_size * map_size * 2) - 1);
}

int64_t part_two(char const * filename, size_t const map_size) {
    FILE * file = r_file_open(filename);

    rMatrix_rGraphNode * map_h = r_matrix_new_rGraphNode(map_size, map_size);
    rMatrix_rGraphNode * map_v = r_matrix_new_rGraphNode(map_size, map_size);

    rGraph * graph = r_graph_new(map_size * map_size, map_size * map_size);

    size_t const buffer_size = 256;
    char buffer[buffer_size];
    for (size_t i = 0; fgets(buffer, buffer_size, file) != NULL; ++i) {
        char * line = r_string_copy_until(buffer, '\n');

        printf("%s\n", line);

        char * it = line;
        for (size_t j = 0; *it != '\0'; ++it, ++j) {
            rGraphNode * node_h = r_graph_node_new(i * map_size + j, *it - '0');
            rGraphNode * node_v = r_graph_node_new(map_size * map_size + i * map_size + j, *it - '0');

            r_matrix_set_rGraphNode(map_h, i, j, node_h);
            r_matrix_set_rGraphNode(map_v, i, j, node_v);

            r_graph_add_node(graph, node_h);
            r_graph_add_node(graph, node_v);
        }
    }

    // Construct the graph
    construct_graph(&graph, map_h, map_v, map_size, 4, 10);

    // Calculate the shortest paths
    rArray_int64_t * distances = r_array_new_int64_t(map_size * map_size * 2);
    for (size_t i = 0; i < map_size * map_size * 2; ++i) {
        r_array_append_int64_t(distances, INT64_MAX);
    }
    r_array_set_int64_t(distances, 0, 0);
    dijkstra(graph, distances);

    // 368: too low
    // 367: too low
    return r_array_get_int64_t(distances, (map_size * map_size * 2) - 1);
}