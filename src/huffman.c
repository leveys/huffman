#include "huffman.h"
#include "huffman_util.h"
#include "trie.h"
#include <string.h>

// internal functions
Node* add_new(Tree* tree, char* str, int length);
void swap_nodes(Node* node1, Node* node2);
void swap_list(Node* A, Node* B);
void refresh_outer_pointers(Node* A);


// initialize huffman tree with root node, root node is always nyt node at start
Tree* init_tree() {
    Tree* tree = (Tree*)safe_malloc(sizeof(Tree));

    // initialize root(=nyt) node
    tree->root = (Node*)safe_calloc(1, sizeof(Node));  // calloc sets everything to 0
    tree->nyt = tree->root;

    // initialize trie
    tree->trie = init_trie();

    tree->nodes = 1;

    return tree;
}

// update huffman tree with a given string
void update_tree(Tree* tree, Node* node, char* str, int length) {
    
    // check if tree contains character
    if (node == tree->nyt || !node) {
        // character not in tree, add character to tree
        node = add_new(tree, str, length);
        // if node is null, update is done
        if (!node) return;
    }

    // while node is not root
    while (node->parent) {

        // find node with weight == node->weight and order minimal
        Node* min_node = node;
        while (min_node->prev_ord && min_node->prev_ord->weight == node->weight) {
            min_node = min_node->prev_ord;
        }

        if (min_node != node && min_node != node->parent) {
            // swap min_node and node
            swap_nodes(min_node, node);
        }
        
        // update weight
        node->weight++;
        node = node->parent;
    }

    // node is root, update weight
    node->weight++;
}

// add a new character to a huffman tree
// this is done by replacing the nyt node with a small tree of 3 nodes: an internal node with 2 children, the nyt node and the new character node
// returns the parent of the new internal node, this will be null if this node has no parent
Node* add_new(Tree* tree, char* str, int length) {

    // get nyt node
    Node* nyt_node = tree->nyt;

    // create and initialize new internal node and new leaf node
    Node* internal_node = (Node*)safe_calloc(1, sizeof(Node));
    Node* leaf_node = (Node*)safe_calloc(1, sizeof(Node));
    
    internal_node->weight = 1;
    internal_node->order = nyt_node->order;
    internal_node->parent = nyt_node->parent;
    internal_node->left = nyt_node;
    internal_node->right = leaf_node;

    leaf_node->string = safe_malloc(sizeof(char)*(length+1));    // length +1 for null termination
    memcpy(leaf_node->string, str, length);
    leaf_node->string[length] = '\0';
    leaf_node->strlength = length;
    leaf_node->weight = 1;
    leaf_node->order = nyt_node->order + 1;
    leaf_node->parent = internal_node;

    // check if nyt node has a parent
    if (nyt_node->parent) {
        // check which child is nyt
        if (nyt_node->parent->left == nyt_node) {
            nyt_node->parent->left = internal_node;
        } else {
            nyt_node->parent->right = internal_node;
        }
    } else {    // else nyt node is root
        // update root
        tree->root = internal_node;
    }

    nyt_node->order = nyt_node->order + 2;
    nyt_node->parent = internal_node;

    // update order list
    internal_node->prev_ord = nyt_node->prev_ord;
    internal_node->next_ord = leaf_node;
    if (nyt_node->prev_ord) {
        nyt_node->prev_ord->next_ord = internal_node;
    }

    leaf_node->prev_ord = internal_node;
    leaf_node->next_ord = nyt_node;

    nyt_node->prev_ord = leaf_node;
    // nyt_node->next_ord is always null

    // add new leaf node to trie
    trie_add_string_node(tree->trie, str, length, leaf_node);

    // increment node counter
    tree->nodes += 2;

    return internal_node->parent;
}

// swap 2 nodes in tree but keep order numbers
void swap_nodes(Node* A, Node* B) {

    // swap child pointers of parents
    if (A->parent->left == A) {     // A is left child
        A->parent->left = B;
    } else {                        // A is right child
        A->parent->right = B;
    }
    if (B->parent->left == B) {     // B is left child
        B->parent->left = A;
    } else {                        // B is right child
        B->parent->right = A;
    }

    // swap parents
    Node* tmp = A->parent;
    A->parent = B->parent;
    B->parent = tmp;

    // swap order numbers back
    unsigned int tmp_ord = A->order;
    A->order = B->order;
    B->order = tmp_ord;

    // swap nodes in order list
    swap_list(A, B);
}

// swap 2 nodes in order list
void swap_list(Node* A, Node* B) {

    // shouldnt be possible
    if (A == B) return;

    // if B right before A, swap A and B
    if (B->next_ord == A) {
        Node* tmp = A;
        A = B;
        B = tmp;
    }

    Node* tmp_vec[4];
    tmp_vec[0] = A->prev_ord;
    tmp_vec[1] = B->prev_ord;
    tmp_vec[2] = A->next_ord;
    tmp_vec[3] = B->next_ord;

    // check if A and B are neighbours
    if (A->next_ord == B && B->prev_ord == A) {
        A->prev_ord = tmp_vec[2];
        B->prev_ord = tmp_vec[0];
        A->next_ord = tmp_vec[3];
        B->next_ord = tmp_vec[1];
    } else {
        A->prev_ord = tmp_vec[1];
        B->prev_ord = tmp_vec[0];
        A->next_ord = tmp_vec[3];
        B->next_ord = tmp_vec[2];
    }

    refresh_outer_pointers(A);
    refresh_outer_pointers(B);
}

// helper function for swapping nodes in order list
void refresh_outer_pointers(Node* node) {
    if (node->prev_ord){
        node->prev_ord->next_ord = node;
    }
    if (node->next_ord){
        node->next_ord->prev_ord = node;
    }
}


// find leaf node in tree with given string
Node* tree_find_node(Tree* tree, char* str, int length) {
    Trie_node* t_node = trie_find_string(tree->trie, str, length);
    return t_node ? t_node->data.huff_node : NULL;
}

// get the size of the tree in bytes
unsigned long tree_size(Tree* tree) {
    return tree->nodes * sizeof(Node) + sizeof(Tree);
}

// recursively print nodes to stdout
void print_node_rec(Node* node) {

    // check if node is nyt node
    if (node->weight == 0) {
        printf("--- nyt node %i ---\n", node->order);
    // check if node is root node (no parent)
    } else if (!node->parent) {
        printf("--- root node %i ---\n", node->order);
    // check if node is internal node (has children)
    } else if (node->left) {
        printf("--- internal node %i ---\n", node->order);
    // else node is leaf node
    } else {    
        printf("--- leaf node %i, %s ---\n", node->order, node->string);
    }
    printf("weight: %i\n", node->weight);
    // check if node has parent
    if (node->parent) {
        printf("parent: %i\n", node->parent->order);
    }
    // check if node has children
    if (node->left && node->right) {
        printf("left child: %i\n", node->left->order);
        printf("right child: %i\n", node->right->order); 
    }
    printf("-------\n\n");
    // print children
    if (node->left) {
        print_node_rec(node->left);
    }
    if (node->right) {
        print_node_rec(node->right);
    }
}

// print huffman tree to stdout
void print_tree(Tree* tree) {
    printf("--- Huffman tree ---\n\n");
    print_node_rec(tree->root);
    printf("--------------------\n\n");
}

// recursively free memory allocated by nodes
void free_tree_node_rec(Node* node) {
    if (node) {
        free_tree_node_rec(node->left);
        free_tree_node_rec(node->right);
        free(node->string);
        free(node);
    }
}

// free memory allocated by huffman tree
void free_tree(Tree* tree) {
    if (tree) {
        free_tree_node_rec(tree->root);
        free_trie(tree->trie);
        free(tree);
    }
}