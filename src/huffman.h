#ifndef HUFFMAN_H
#define HUFFMAN_H

#include <stdio.h>

typedef struct Trie Trie;   // forward declaration

// path type contains a path from a node to the root (or vice versa)
// type depends on max depth of tree, i.e. int -> max depth = 31 (= 32 - 1)
typedef unsigned long path;
static const path MAX_PATH = ~ ((path)~0 >> 1);    // 0b100...00


typedef struct Node {   // Huffman tree node

    // string represented by this node in the Huffman tree
    // NULL for internal nodes and nyt node
    char* string;
    int strlength; // length of the string

    unsigned int weight;
    unsigned int order;

    struct Node* parent; // parent node

    // a node must always have 0 or 2 children
    struct Node* left;  // left child
    struct Node* right; // right child

    struct Node* prev_ord; // node with order -1
    struct Node* next_ord; // node with order +1
} Node;     // 64 bytes total

typedef struct Tree {   // Huffman tree

    Node* root; // root node
    Node* nyt; // nyt node

    // ternary trie with all strings added to the tree
    // for finding the leaf node with a given string
    Trie* trie;

    int nodes; // number of nodes in the tree
} Tree;     // 32 bytes total


// huffman tree functions

Tree* init_tree();
void update_tree(Tree* tree, Node* node, char* str, int length);
Node* tree_find_node(Tree* tree, char* str, int length);
unsigned long tree_size(Tree* tree);
void print_tree(Tree* tree);
void free_tree(Tree* tree);
//


void compress(int max_chars, int max_mem, FILE* inputfile, FILE* outputfile);

void decompress(FILE* inputfile, FILE* outputfile);

#endif //HUFFMAN_H