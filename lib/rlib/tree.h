#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <inttypes.h>

// region rBTree

#define TreeT rBTree
#define NodeT rBTreeNode
#define KeyT int64_t

typedef struct TreeT TreeT;
typedef struct NodeT NodeT;

extern TreeT * r_btree_new(size_t order);
extern void r_btree_insert(TreeT * tree, KeyT key);
extern void r_btree_print(TreeT * tree);
extern bool r_btree_search(TreeT * tree, KeyT key);


extern NodeT * r_btree_node_new(TreeT * tree, bool leaf);
extern bool r_btree_node_search(NodeT * node, KeyT key);
extern void r_btree_node_insert(NodeT * node, KeyT key);
extern void r_btree_node_split(NodeT * parent, NodeT * child, size_t child_i);
extern void r_btree_node_print(FILE * buffer, NodeT * node);

#undef KeyT
#undef NodeT
#undef TreeT

// endregion rBTree
