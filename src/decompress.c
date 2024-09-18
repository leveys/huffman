#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "huffman.h"
#include "huffman_io.h"

// decompress an input file encoded by huffman coding
void decompress(FILE* inputfile, FILE* outputfile) {

    Tree* tree = init_tree();
    huffman_io io = init_io(inputfile, READ);

    // read input bit by bit
    uint8 bit;
    while (!io.eof_reached) {
        
        // search node in tree via path
        Node* node = tree->root;
        // while node is not a leaf
        while (node->left && node->right) {
            // read next bit
            bit = read_bit(&io);
            // next node is child of node
            node = bit ? node->right : node->left;  // if bit is 1, take right path, if bit is 0, take left path
        }

        // if node is nyt node, read character(s)
        if (node == tree->nyt) {

            // first read length in bytes
            uint8 length = read_byte(&io);

            // if length is 0, this is the last character, stop reading
            if (length == 0) {
                io.eof_reached = 1;
                break;
            }

            // now read <length> characters
            char str[length+1]; // +1 for terminating null byte
            str[length] = '\0';
            for (int i = 0; i < length; i++) {

                uint8 c = read_byte(&io);

                // write character back to output 
                fputc(c, outputfile);
                str[i] = c;
            }
            update_tree(tree, node, str, length);

        } else {
            // write characters of leaf node to output
            fwrite(node->string, sizeof(char), node->strlength, outputfile);
            update_tree(tree, node, NULL, 0);
        }
    }

    free_tree(tree);
}
