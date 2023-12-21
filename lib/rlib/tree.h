#pragma once

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
extern void r_btree_add(TreeT * tree, KeyT key);

extern NodeT * r_btree_node_new(TreeT * tree, bool leaf);
extern void r_btree_node_insert(NodeT * node, KeyT key);
extern void r_btree_node_split_child(NodeT * parent, size_t child_i);

extern NodeT * r_btree_node_search(NodeT * node, KeyT key);

#undef KeyT
#undef NodeT
#undef TreeT

// endregion rBTree