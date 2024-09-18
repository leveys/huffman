#ifndef TEST_TRIE_H
#define TEST_TRIE_H

#include "../src/trie.h"

static char* DEFAULT_STRINGS[] = {"test", "testing", "trie", "t", "ttt", "esting", "huffman", "oooooooooooooooooooooooooooooh", "testign", "test", "test", "huffman"};
static int DEFAULT_STRINGS_LENGTH = sizeof(DEFAULT_STRINGS) / sizeof(DEFAULT_STRINGS[0]);
static int DEFAULT_STRINGS_COUNTS[] = {3, 1, 1, 1, 1, 1, 2, 1, 1, 3, 3, 2};

void make_test_trie(Trie* trie, char** strings, int length);
int trie_check_order(Trie *trie, int verbose);
int trie_check_count(Trie *trie, char** strings, int length, int* counts, int verbose);


#endif // TEST_TRIE_H