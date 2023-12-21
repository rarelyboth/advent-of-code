#include <math.h>
#include <string.h>
#include "tree.h"

// region rBTree

#define TreeT rBTree
#define NodeT rBTreeNode
#define KeyT int64_t

struct TreeT {
    size_t order;
    NodeT * root;
};

struct NodeT {
    TreeT * tree;
    bool leaf;

    size_t n_keys;
    size_t n_children;
    KeyT * keys;
    NodeT ** children;
};

TreeT * r_btree_new(size_t order) {
    TreeT * tree = malloc(sizeof(TreeT));

    tree->order = order;
    tree->root = r_btree_node_new(tree, true);

    return tree;
}

void r_btree_add(TreeT * tree, KeyT key) {

}

NodeT * r_btree_node_new(TreeT * tree, bool leaf) {
    NodeT * node = malloc(sizeof(NodeT));

    node->tree = tree;
    node->leaf = leaf;
    node->n_keys = 0;
    node->n_children = 0;

    node->keys = calloc(tree->order - 1, sizeof(KeyT));
    node->children = calloc(tree->order, sizeof(NodeT *));

    return node;
}

void r_btree_node_insert(rBTreeNode * node, int64_t key) {
    size_t i = node->n_keys - 1;

    if (node->leaf) {
        while (i < node->n_keys && key < node->keys[i]) {
            node->keys[i + 1] = node->keys[i];
            --i;
        }
        node->keys[i + 1] = key;
    } else {
        while (i < node->n_keys && key < node->keys[i]) {
            --i;
        }
        ++i;

        if (node->children[i]->n_keys == (2 * node->tree->order) - 1) {
            r_btree_node_split_child(node, i);
            if (key > node->keys[i]) {
                ++i;
            }
        }

        r_btree_node_insert(node->children[i], key);
    }
}

void r_btree_node_split_child(NodeT * parent, size_t child_i) {
    NodeT * child = parent->children[child_i];

    NodeT * sibling = r_btree_node_new(parent->tree, child->leaf);

    memmove(parent->children + child_i + 1, parent->children + child_i, (parent->n_children - child_i) * sizeof(NodeT *));
    ++parent->n_children;



    parent->children[child - *parent->children + 1]
}

NodeT * r_btree_node_search(NodeT * node, KeyT key) {
    size_t i = 0;

    while (i < node->n_keys && key > node->keys[i]) {
        ++i;
    }

    if (i < node->n_keys && key == node->keys[i]) {
        return node;
    }  else if (node->leaf) {
        return NULL;
    } else {
        return r_btree_node_search(node->children[i], key);
    }
}


#undef KeyT
#undef NodeT
#undef TreeT

// endregion rBTree