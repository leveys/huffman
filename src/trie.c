#include "trie.h"
#include "huffman_util.h"
#include <string.h>

// initialize ternary trie
Trie* init_trie() {
    Trie* trie = (Trie*)safe_calloc(1, sizeof(Trie));
    return trie;
}

// internal enum to indicate what type is added to the trie,
// this can be a huffman node pointer or an integer
typedef enum ADD_TYPE {
    HUFF_NODE,
    INT
} ADD_TYPE;

// add a node to the trie, either with a given huffman tree node, or an integer
// in case of the huffman node, add new node if string not yet in trie, else overwrite the huffman node
// in case of integer, add new node with count=1 if not yet string not yey in trie, else increment the counter
// returns the counter of the node, if type is HUFF_NODE, returns 0
int trie_add_internal(Trie* trie, char* str, int length, ADD_TYPE type, Node* huff_node) {

    // handle special case where root does not yet exist
    if (!trie->root) {
        // root does not exist, create it
        trie->root = (Trie_node*)safe_calloc(1, sizeof(Trie_node));
        trie->root->character = str[0];
        trie->nodes++;
    }

    Trie_node* node = trie->root;

    for (int i = 0; i < length; i++) {

        char c = str[i];

        // find node of current character, if it does not exist, create it
        while (c != node->character) {

            if (c > node->character) {
                // character is greater than current one, go right

                if (!node->right) {
                    // node with character does not exist, create new node
                    node->right = (Trie_node*)safe_calloc(1, sizeof(Trie_node));    // calloc sets data to 0
                    node->right->character = c;
                    trie->nodes++;
                }
                // go to next node
                node = node->right;

            } else {
                // character is lesser than current one, go left

                if (!node->left) {
                    // node with character does not exist, create new node
                    node->left = (Trie_node*)safe_calloc(1, sizeof(Trie_node));    // calloc sets data to 0
                    node->left->character = c;
                    trie->nodes++;
                }
                // go to next node
                node = node->left;
            }
        }

        if (i < length-1) {
            // string not yet completed, go to next character
            if (!node->next) { 
                // if next node does not exist, create new node
                node->next = (Trie_node*)safe_calloc(1, sizeof(Trie_node));     // calloc sets data to 0
                node->next->character = c;
                trie->nodes++;
            }
            node = node->next;
        }
    }
    // string completed, check type
    switch (type) {
        case (INT):
            // string completed, increment counter
            node->data.count++;
            return node->data.count;
        case (HUFF_NODE):
            // string completed, set huffman node pointer
            node->data.huff_node = huff_node;
            return 0;
    }
    return -1;  // should never reach here
}

// find a string in given trie and increment its counter
// if the string does not yet exist in trie, add it
// return the counter
int trie_increment_string_count(Trie* trie, char* str, int length) {
    return trie_add_internal(trie, str, length, INT, NULL);
}

// add a string to the trie with given huffman node
void trie_add_string_node(Trie* trie, char* str, int length, Node* huff_node) {
    trie_add_internal(trie, str, length, HUFF_NODE, huff_node);
}

// find trie node with given string
Trie_node* trie_find_string(Trie* trie, char* str, int length) {

    Trie_node* node = trie->root;

    for (int i = 0; i < length; i++) {

        if (!node) {
            // node does not exist, string is not in trie
            return NULL;
        }

        char c = str[i];

        // find node of current character, if it does not exist, create it
        while (node && c != node->character) {
            // if character is greater than current one, go right, else go left
            node = (c > node->character) ? node->right : node->left;
        }

        if (node && i < length-1) {
            // if not last character of string, go to next node
            node = node->next;
        }
    }
    return node;
}

// recursively free memory allocated by nodes
void free_trie_node_rec(Trie_node* node) {
    if (node) {
        free_trie_node_rec(node->left);
        free_trie_node_rec(node->right);
        free_trie_node_rec(node->next);
        free(node);
    }
}

// free memory allocated by ternary trie
void free_trie(Trie* trie) {
    if (trie) {
        free_trie_node_rec(trie->root);
        free(trie);
    }
}

// remove all nodes in trie but not the trie itself
void clear_trie(Trie* trie) {
    if (trie) {
        free_trie_node_rec(trie->root);
        trie->nodes = 0;
        trie->root = NULL;
    }
}