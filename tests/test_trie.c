#include <string.h>
#include <stdio.h>
#include "test_trie.h"

void make_test_trie(Trie* trie, char** strings, int length) {
    for (int i = 0; i < length; i++) {
        trie_increment_string_count(trie, strings[i], strlen(strings[i]));
    }
}

int check_order_rec(Trie_node *node, int verbose) {

    if (!node) return 1;

    // check left and right node
    if (verbose) printf("node %c", node->character);
    if (node->left) {
        if (verbose) printf(", left %c", node->left->character);
        if (node->character <= node->left->character) {
            return 0;
        }
    }
    if (node->right) {
        if (verbose) printf(", right %c", node->right->character);
        if (node->character >= node->right->character) {
            return 0;
        }
    }
    if (verbose) printf("\n");
    // check left node
    if(!check_order_rec(node->left, verbose)) {
        return 0;
    }
    // check right node
    if(!check_order_rec(node->right, verbose)) {
        return 0;
    }
    // check next node
    if(!check_order_rec(node->next, verbose)) {
        return 0;
    }
    return 1;
}

int trie_check_order(Trie *trie, int verbose) {
    if (verbose) printf("checking order of nodes ...\n");
    int res = check_order_rec(trie->root, verbose);
    if (verbose) printf("-----------\n\n");
    return res;
}

int trie_check_count(Trie *trie, char** strings, int length, int* counts, int verbose) {
    if (verbose) printf("checking count of strings ...\n");
    for (int i = 0; i < length; i++) {
        Trie_node* node = trie_find_string(trie, strings[i], strlen(strings[i]));
        int count = node->data.count;
        if (verbose) printf("string %s, count %d\n", strings[i], count);
        if (count != counts[i]) {
            return 0;
        }
    }
    if (verbose) printf("-----------\n\n");
    return 1;
}