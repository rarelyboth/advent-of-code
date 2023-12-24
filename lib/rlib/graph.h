#pragma once

#include <stdlib.h>
#include <inttypes.h>

// region rGraph

#define GraphT rGraph
#define NodeT rGraphNode
#define ArcT rGraphArc
#define idT uint32_t
#define ValueT int64_t

typedef struct GraphT GraphT;
typedef struct NodeT NodeT;
typedef struct ArcT ArcT;

typedef struct rBTree rBTree;
typedef struct rArray_rArc rArray_rArc;

struct GraphT {
    rBTree * node_ids;
    rArray_rArc * arcs;
};

struct NodeT {
    idT id;
    ValueT value;
};

struct ArcT {
    NodeT * from;
    NodeT * to;
    ValueT value;
};

extern GraphT * r_graph_new(size_t n_nodes_hint, size_t n_arcs_hint);
extern void r_graph_add_node(GraphT * graph, NodeT * node);
extern void r_graph_add_arc(GraphT * graph, ArcT * arc);

extern NodeT * r_graph_node_new(idT id, ValueT value);

extern ArcT * r_graph_arc_new(NodeT * from, NodeT * to, ValueT value);

#undef ValueT
#undef idT
#undef ArcT
#undef NodeT
#undef GraphT

// endregion rGraph