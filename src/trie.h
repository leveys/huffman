#ifndef TRIE_H
#define TRIE_H

typedef struct Node Node;   // forward declaration

// node in ternary trie
typedef struct Trie_node {

    // character of this node
    char character;

    // data stored in the node
    union {
        // node in huffman tree
        Node* huff_node;
        // number of occurrences of the string
        int count;
    } data;

    // smaller and greater character
    struct Trie_node* left;
    struct Trie_node* right;

    // next character
    struct Trie_node* next;

} Trie_node;    // 40 bytes total

// ternary trie
typedef struct Trie {
    Trie_node* root;

    int nodes;  // number of nodes in the trie

} Trie;     // 16 bytes total


Trie* init_trie();

void trie_add_string_node(Trie* trie, char* str, int length, Node* huff_node);
int trie_increment_string_count(Trie* trie, char* str, int length);

Trie_node* trie_find_string(Trie* trie, char* str, int length);

void free_trie(Trie* trie);
void clear_trie(Trie* trie);

#endif // TRIE_H