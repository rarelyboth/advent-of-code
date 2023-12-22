#include "tree.h"

#include <stdio.h>
#include <assert.h>

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

    size_t n_keys;
    KeyT * keys;

    size_t n_children;
    NodeT ** children;

    bool leaf;
};

TreeT * r_btree_new(size_t order) {
    assert(order % 2 == 0);  // Require an even order for preemptive splitting during insertion

    TreeT * tree = malloc(sizeof(TreeT));

    tree->order = order;
    tree->root = r_btree_node_new(tree, true);  // Initially the root will be a leaf node

    return tree;
}

void r_btree_insert(TreeT * tree, KeyT key) {
    // Split the root if necessary
    if (tree->root->n_keys == tree->order - 1) {
        NodeT * old_root = tree->root;
        NodeT * new_root = r_btree_node_new(tree, false);
        tree->root = new_root;

        new_root->children[0] = old_root;
        new_root->n_children = 1;
        r_btree_node_split(new_root, old_root, 0);
    }

    r_btree_node_insert(tree->root, key);
}

NodeT * r_btree_node_new(TreeT * tree, bool leaf) {
    NodeT * node = malloc(sizeof(NodeT));

    node->tree = tree;

    node->n_keys = 0;
    node->keys = calloc(tree->order - 1, sizeof(KeyT));

    node->n_children = 0;
    node->children = calloc(tree->order, sizeof(NodeT *));

    node->leaf = leaf;

    return node;
}

void r_btree_node_insert(NodeT * node, KeyT key) {
    if (node->leaf) {
        // Shift the existing keys
        size_t i = node->n_keys;
        while (i >= 1 && key < node->keys[i - 1]) {
            node->keys[i] = node->keys[i - 1];
            --i;
        }

        node->keys[i] = key;
        ++node->n_keys;
    } else {
        size_t i = node->n_keys;
        while (i >= 1 && key < node->keys[i - 1]) {
            --i;
        }

        NodeT * child = node->children[i];
        if (child->n_keys == node->tree->order - 1) {
            r_btree_node_split(node, child, i);

            if (key > node->keys[i]) {
                ++i;
            }
        }

        r_btree_node_insert(node->children[i], key);
    }
}

void r_btree_node_split(NodeT * parent, NodeT * child, size_t child_i) {
    NodeT * sibling = r_btree_node_new(child->tree, child->leaf);

    KeyT median_key = child->keys[child->n_keys / 2];

    // Copy larger keys into the sibling
    sibling->n_keys = child->n_keys / 2;
    for (size_t i = 0; i < sibling->n_keys; ++i) {
        sibling->keys[i] = child->keys[sibling->n_keys + i + 1];
    }

    child->n_keys = sibling->n_keys;

    // Copy larger children into the sibling
    if (!child->leaf) {
        sibling->n_children = child->n_children / 2;
        for (size_t i = 0; i < sibling->n_children; ++i) {
            sibling->children[i] = child->children[sibling->n_children + i];
        }

        child->n_children = sibling->n_children;
    }

    // Insert the child's median key into the parent
    for (size_t i = parent->n_keys; i > child_i; --i) {
        parent->keys[i] = parent->keys[i - 1];
    }
    parent->keys[child_i] = median_key;
    ++parent->n_keys;

    // Link the sibling to the parent
    for (size_t i = parent->n_children; i > child_i; --i) {
        parent->children[i] = parent->children[i - 1];
    }
    parent->children[child_i + 1] = sibling;
    ++parent->n_children;
}

void r_btree_print(TreeT * tree) {
    r_btree_node_print(stderr, tree->root);
}

void r_btree_node_print(FILE * buffer, NodeT * node) {
    fprintf(buffer, "[");
    for (size_t i = 0; i < node->n_keys; ++i) {
        fprintf(buffer, "%" PRIi64, node->keys[i]);
        if (i != node->n_keys - 1) {
            fprintf(buffer, " ");
        }
    }
    fprintf(buffer, "]\n");

    if (node->n_children == 0) {
        return;
    }

    for (size_t i = 0; i < node->n_children; ++i) {
        fprintf(buffer, "[");
        for (size_t j = 0; j < node->children[i]->n_keys; ++j) {
            fprintf(buffer, "%" PRIi64, node->children[i]->keys[j]);
            if (i != node->children[i]->n_keys - 1) {
                fprintf(buffer, " ");
            }
        }
        fprintf(buffer, "]");
    }
}

bool r_btree_search(TreeT * tree, KeyT key) {
    return r_btree_node_search(tree->root, key);
}

bool r_btree_node_search(NodeT * node, KeyT key) {
    size_t i = 0;
    while (i < node->n_keys && key > node->keys[i]) {
        ++i;
    }

    if (i < node->n_keys && key == node->keys[i]) {
        return true;
    }

    if (node->leaf) {
        return false;
    }

    return r_btree_node_search(node->children[i], key);
}


#undef KeyT
#undef NodeT
#undef TreeT

// endregion rBTree