#include <string.h>
#include "test_tree.h"

// make huffman tree with given text, with given number of characters per node
void make_test_tree(Tree* tree, const char* text, int chars_per_node) {

    // string buffer
    char str[chars_per_node+1];
    str[chars_per_node] = '\0';

    // update tree for each characer in string
    for (int i=0; i<strlen(text); i+=chars_per_node) {
        strncpy(str, &text[i], chars_per_node);
        Node* node = tree_find_node(tree, str, chars_per_node);
        if (!node) {
            node = tree->nyt;
        }
        update_tree(tree, node, str, chars_per_node);
    }
}

// recursively check if nodes have either 0 or 2 children
int is_binary_rec(Node* node, int verbose) {
    // check if node has 2 children
    if (node->left && node->right) {
        if (verbose) printf("node %i, 2 children\n", node->order);
        // check children
        return is_binary_rec(node->left, verbose) && is_binary_rec(node->right, verbose);
    }
    // if either child exists, node has 1 child, return false
    if (node->left || node->right) return 0;
    if (verbose) printf("node %i, no children\n", node->order);
    // else return true
    return 1;
}

// check if tree is binary (if each node has 0 or 2 children)
int tree_is_binary(Tree* tree, int verbose){
    if (verbose) printf("checking if tree is binary ...\n");
    int res = is_binary_rec(tree->root, verbose);
    if (verbose) printf("-----------\n\n");
    return res;
}

// recursively check if weight of nodes is sum of childrens weights
int check_weights_rec(Node* node, int verbose) {
    // check if node has children
    if (node->left && node->right) {
        if (verbose) printf("node %i, weight %i, weight of children %i + %i\n", node->order, node->weight, node->left->weight, node->right->weight);
        // check if weight of node is equal to the sum of the weights of the children
        if ((node->left->weight + node->right->weight) != node->weight) {
            return 0;
        }
        // check children
        return check_weights_rec(node->left, verbose) && check_weights_rec(node->right, verbose);
    }
    // else return true
    return 1;
}

// check if the weight of all internal nodes in tree is the sum of their children
int tree_check_weights(Tree* tree, int verbose) {
    if (verbose) printf("checking weights ...\n");
    int res = check_weights_rec(tree->root, verbose);
    if (verbose) printf("-----------\n\n");
    return res;
}

// check for all nodes in the order list if their successor has a greater order number and a lesser or equal weight
int tree_check_order(Tree* tree, int verbose) {
    if (verbose) printf("checking order list ...\n");

    Node* node = tree->root;
    if (node->prev_ord) return 0;   // root should be first in order list

    // while there is a successor
    while (node->next_ord) {
        if (verbose) printf("node %i, weight %i\n", node->order, node->weight);

        // check order number
        if (node->order >= node->next_ord->order) return 0;
        // check weight
        if (node->weight < node->next_ord->weight) return 0;

        node = node->next_ord;
    }
    if (verbose) printf("node %i, weight %i\n", node->order, node->weight);
    if (verbose) printf("-----------\n\n");
    return 1;
}

// check if the number of nodes and leaves is correct
int tree_check_num_nodes(Tree* tree, int verbose){
    if (verbose) printf("checking number of nodes ...\n");
    // count nodes
    int nodes = 0;
    int leaves = 0;
    Node* node = tree->root;
    while (node) {
        // node has no children -> leaf node
        if (!node->left && !node->right) {
            leaves++;
        }
        nodes++;
        node = node->next_ord;
    }
    if (verbose) printf("%i nodes, of which %i leaf nodes\n", nodes, leaves);
    if (verbose) printf("-----------\n\n");
    return nodes == 2*leaves - 1;
}

// check if nodes with order numbers 2i - 1 and 2i are brothers (same parent) for i in 1..n-1 (n = number of leaves)
int tree_check_brothers(Tree* tree, int verbose) {
    if (verbose) printf("checking brothers ...\n");
    // start with successor of root
    Node* node = tree->root->next_ord;
    while (node) {
        // check if node has a successor
        if (!node->next_ord) return 0;
        // check if current node and next node have the same parent node
        if (node->parent != node->next_ord->parent) return 0;
        if (verbose) printf("node %i (parent %i), node %i (parent %i)\n", node->order, node->parent->order, node->next_ord->order, node->next_ord->parent->order);
        node = node->next_ord->next_ord;
    }
    if (verbose) printf("-----------\n\n");
    return 1;
}

unsigned long size_rec(Node* node) {
    return node ? sizeof(*node) + size_rec(node->left) + size_rec(node->right) : 0;
}

// check if size of tree matches the number of nodes
int tree_check_size(Tree* tree, int verbose) {

    unsigned long size = size_rec(tree->root) + sizeof(Tree);

    if (verbose) {
        printf("checking tree size ...\n");
        printf("size of tree struct: %lu bytes\n", sizeof(Tree));
        printf("size of tree (total): %lu bytes\n", size);
        printf("number of nodes: %i\n", tree->nodes);
        printf("size of node: %lu bytes\n", sizeof(Node));
        printf("-----------\n\n");
    }
    return size == tree_size(tree);
}