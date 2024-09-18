#include <stdio.h>
#include <string.h>
#include "huffman_util.h"

void do_nothing() {}

void* safe_malloc_internal(size_t size, char* file, unsigned int line) {
    void* ptr = malloc(size);
    if (!ptr) {
        fprintf(stderr, "[%s:%u] Out of memory (%lu bytes)\n", file, line, (unsigned long)size);
        exit(1);
    }
    return ptr;
}

void* safe_calloc_internal(size_t count, size_t size, char* file, unsigned int line) {
    void* ptr = calloc(count, size);
    if (!ptr) {
        fprintf(stderr, "[%s:%u] Out of memory (%lu bytes)\n", file, line, (unsigned long)size);
        exit(1);
    }
    return ptr;
}