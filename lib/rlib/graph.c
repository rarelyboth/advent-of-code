#include "graph.h"
#include "tree.h"
#include "array.h"

// region rGraph

#define GraphT rGraph
#define NodeT rGraphNode
#define ArcT rGraphArc
#define idT uint32_t
#define ValueT int64_t

GraphT * r_graph_new(size_t n_nodes_hint, size_t n_arcs_hint) {
    GraphT * graph = malloc(sizeof(GraphT));

    graph->node_ids = r_btree_new(4);
    graph->arcs = r_array_new_rArc(n_arcs_hint);

    return graph;
}

void r_graph_add_node(GraphT * graph, NodeT * node) {
    r_btree_insert(graph->node_ids, node->id);
}

void r_graph_add_arc(GraphT * graph, ArcT * arc) {
    r_array_append_rArc(graph->arcs, arc);
}

NodeT * r_graph_node_new(idT id, ValueT value) {
    NodeT * node = malloc(sizeof(NodeT));

    node->id = id;
    node->value = value;

    return node;
}

ArcT * r_graph_arc_new(NodeT * from, NodeT * to, ValueT value) {
    ArcT * arc = malloc(sizeof(ArcT));

    arc->from = from;
    arc->to = to;
    arc->value = value;

    return arc;
}

#undef ValueT
#undef idT
#undef ArcT
#undef NodeT
#undef GraphT

// endregion rGraph