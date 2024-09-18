#ifndef HUFFMAN_UTIL_H
#define HUFFMAN_UTIL_H

#ifdef DEBUG
#define DEBUG_PRINT printf
#else
#define DEBUG_PRINT do_nothing  // dont do anything
#endif

#include <stdlib.h>

void do_nothing();

#define safe_malloc(s) safe_malloc_internal(s, __FILE__, __LINE__)
#define safe_calloc(n, s) safe_calloc_internal(n, s, __FILE__, __LINE__)

void* safe_malloc_internal(size_t size, char* file, unsigned int line);
void* safe_calloc_internal(size_t count, size_t size, char* file, unsigned int line);

char* convert_whitespace(char* str);

#endif // HUFFMAN_UTIL_H