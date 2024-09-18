#include <assert.h>
#include <string.h>
#include <getopt.h>
#include <stdlib.h>
#include "test_tree.h"
#include "test_trie.h"

int test_tree(int argc, char** argv);
int test_trie(int argc, char** argv);

int main(int argc, char** argv) {
    
    if (argc < 2) {
        fprintf(stderr, "Error: first argument must be either tree or trie\n");
        exit(1);
    }

    if (strcmp(argv[1], "tree") == 0) {
        return test_tree(argc, argv);
    } else if (strcmp(argv[1], "trie") == 0) {
        return test_trie(argc, argv);
    } else {
        fprintf(stderr, "Error: first argument must be either tree or trie\n");
        exit(1);
    }
}

int test_tree(int argc, char** argv) {
    
    int vflag = 0;
    char* test_string = DEFAULT_STRING;
    int chars_per_node = 1;

    int opt;

    while ((opt = getopt(argc, argv, "vi:c:")) != -1) {
        switch (opt) {
            case 'v':
                vflag = 1;
                break;
            
            case 'i':
                test_string = optarg;
                break;
            
            case 'c':
                chars_per_node = (int)strtol(optarg, NULL, 10);
                if (chars_per_node < 1 || chars_per_node > 255) {
                    fprintf(stderr, "Error: c argument must be between 1 and 255\n");
                    exit(1);
                }
                break;
            
            case '?':
                fprintf(stderr, "Error: unknown option or missing argument\n");
                exit(1);
                break;
        }
    }

    // create huffman tree to test with
    Tree* tree = init_tree();
    make_test_tree(tree, test_string, 1);

    if (vflag) printf("testing with text: %s\n", test_string);
    if (vflag) printf("testing with %i characters per node\n\n", chars_per_node);

    // test with asserts
    assert(tree_is_binary(tree, vflag));
    assert(tree_check_weights(tree, vflag));
    assert(tree_check_order(tree, vflag));
    assert(tree_check_num_nodes(tree, vflag));
    assert(tree_check_brothers(tree, vflag));
    assert(tree_check_size(tree, vflag));

    printf("all tests succeeded!\n");

    free_tree(tree);
    return 0;
}

int test_trie(int argc, char** argv) {

    int vflag = 0;
    int opt;

    while ((opt = getopt(argc, argv, "vi:c:")) != -1) {
        switch (opt) {
            case 'v':
                vflag = 1;
                break;
            
            case '?':
                fprintf(stderr, "Error: unknown option or missing argument\n");
                exit(1);
                break;
        }
    }

    // create huffman tree to test with
    Trie* trie = init_trie();
    make_test_trie(trie, DEFAULT_STRINGS, DEFAULT_STRINGS_LENGTH);

    if (vflag) {
        printf("testing with strings: ");
        for (int i = 0; i < DEFAULT_STRINGS_LENGTH-1; i++) {
            printf("%s, ", DEFAULT_STRINGS[i]);
        }
        printf("%s\n", DEFAULT_STRINGS[DEFAULT_STRINGS_LENGTH-1]);
    }
    
    // test with asserts
    assert(trie_check_order(trie, vflag));
    assert(trie_check_count(trie, DEFAULT_STRINGS, DEFAULT_STRINGS_LENGTH, DEFAULT_STRINGS_COUNTS, vflag));

    printf("all tests succeeded!\n");

    free_trie(trie);
    return 0;
}