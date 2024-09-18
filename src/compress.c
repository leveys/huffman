#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "huffman.h"
#include "huffman_io.h"
#include "huffman_util.h"
#include "trie.h"

const int MEM_LIMIT[] = {75000, 75000, 100000, 200000, 1000000, 20000000, 50000000, 100000000, 500000000, 1000000000};

// internal functions
path path_to_root(Tree* tree, Node* node);
void strshiftl(char* str, int length, int n);
int calc_max_tree_nodes(int max_mem, int max_chars);
int calc_max_trie_nodes(int max_mem);

// compress an input file using huffman coding
void compress(int max_chars, int max_mem, FILE* inputfile, FILE* outputfile) {

    // calculate maximum number of tree and trie nodes
    int max_tree_nodes = calc_max_tree_nodes(max_mem, max_chars);
    DEBUG_PRINT("max tree nodes: %i\n", max_tree_nodes);
    int max_trie_nodes = calc_max_trie_nodes(max_mem);
    DEBUG_PRINT("max trie nodes: %i\n", max_trie_nodes);

    Tree* tree = init_tree();
    huffman_io io = init_io(outputfile, WRITE);
    Trie* trie = init_trie();

    // loop over input, encode a number of characters in each iteration
    char input_str[max_chars+1];    // input string buffer
    int buf_size = max_chars;   // input buffer size
    int chars_in_buf = 0;   // number of chars to still in buffer
    int chars_encoded = max_chars;  // number of characters encoded in previous iteration
    int total_encoded = 0;  // total number of characters encoded
    int reading = 1;
    while (reading) {

        // if max tree nodes reached, encode only strings of length 1
        if (max_chars > 1 && tree->nodes >= max_tree_nodes) {
            max_chars = 1;
            DEBUG_PRINT("max nodes (%i) reached after %i bytes encoded\n", max_tree_nodes, total_encoded);
        }
        if (trie->nodes >= max_trie_nodes) {
            clear_trie(trie);
        }

        // shift string if there are still characters in buffer
        if (chars_in_buf > 0) {
            strshiftl(input_str, buf_size, chars_encoded);
        }
        // read characters
        int chars_read = fread(&input_str[chars_in_buf], sizeof(char), chars_encoded, inputfile);
        chars_in_buf += chars_read;
        // set terminating byte
        input_str[chars_in_buf] = '\0';

        DEBUG_PRINT("string: \"%s\"\n", input_str);
        DEBUG_PRINT("chars_in_buf: %d\n", chars_in_buf);

        // find string in tree and output path + string if necessary
        int nyt = 0;    // if character was not found in tree (Not Yet Transferred), write character to output
        path p;
        if (chars_in_buf > 0) {

            int best_length = 1;
            int best_count = 0;
            Node* node;
            if (max_chars > 1) {
                // choose number of characters to add in node
                for (int len = chars_in_buf; len > 0; len--) {

                    // add string to trie
                    int count = trie_increment_string_count(trie, input_str, len);

                    // check if string already in tree
                    if ((node = tree_find_node(tree, input_str, len))) {
                        best_length = len;
                        break;
                    }

                    if (count*len/2 > best_count) {
                        best_length = len;
                        best_count = count*len/2;     // times length to favor longer strings
                    }
                }
            } else {
                node = tree_find_node(tree, input_str, 1);
            }
            DEBUG_PRINT("length: %d\n", best_length);
            
            // get path and encode string with huffman tree
            // if string not in tree, output nyt path 
            if (!node) {
                node = tree->nyt;
                nyt = 1;
            }
            // first, get path from node to root
            p = path_to_root(tree, node);
            DEBUG_PRINT("path: %lu\n", p);
            // then update tree
            update_tree(tree, node, input_str, best_length);

            // <best_length> characters encoded, "remove" them from buffer
            chars_in_buf -= best_length;
            chars_encoded = best_length;
            total_encoded += chars_encoded;

        } else {
            // end of file reached, output nyt + zero byte
            reading = 0;    // stop reading
            p = path_to_root(tree, tree->nyt);  // get nyt path
            nyt = 1; // write null byte
            chars_encoded = 0;  // write only null byte
        }

        // output path
        while (p > 1) {
            write_bit(&io, (uint8)(p & 1));
            p >>= 1;
        }

        if (nyt) {

            // first write length in bytes
            uint8 output_byte = (uint8)chars_encoded;   // max length is 255

            for (int i = 0; i <= chars_encoded; i++) {
                // output byte
                write_byte(&io, output_byte);
                // get next character
                output_byte = input_str[i];
            }
        }
        DEBUG_PRINT("\n");
    }

    DEBUG_PRINT("nodes used: %d\n", tree->nodes);
    DEBUG_PRINT("size of tree: %lu\n", tree_size(tree));

    flush(&io);
    free_tree(tree);
    free_trie(trie);
}

// returns path from node to root in huffman tree
path path_to_root(Tree* tree, Node* node) {

    // initialize with 1, this way there is always a 1 in front, this eliminates the need to know the length of the path
    path p = 1;

    // while node is not root
    while (node->parent) {
        if (p >= MAX_PATH) {
            fprintf(stderr, "[%s:%i]exceeded maximum path length\n", __FILE__, __LINE__);
            exit(1);
        }
        p <<= 1;
        // if node is right child, put 1, else 0
        p |= node->parent->right == node;
        node = node->parent;
    }

    return p;
}

// shift string n places to the left
void strshiftl(char* str, int length, int n) {
    n %= length;
    if (n <= 0) return;

    // loop over string starting at position n
    for (int i = n; i < length; i++) {
        str[i-n] = str[i];
    }
}

// calculate maximum number of tree nodes with respect to given memory limit and max chars per node
int calc_max_tree_nodes(int max_mem, int max_chars) {

    int max = MEM_LIMIT[max_mem] * 2 / 4;   // 2/4 memory for tree, 1/4 for trie, 1/4 margin

    max /= sizeof(Node) + max_chars/2;
    int margin = 256;   // margin >= 256 to be sure all strings of length 1 can stil be encoded
    max -= margin;

    return max;
}

// calculate maximum number of trie nodes with respect to given memory limit
int calc_max_trie_nodes(int max_mem) {

    int max = MEM_LIMIT[max_mem] * 1 / 4;   // 2/4 memory for tree, 1/4 for trie, 1/4 margin
    return max/sizeof(Trie_node);
}