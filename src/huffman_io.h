#ifndef HUFFMAN_IO_H
#define HUFFMAN_IO_H

#include <stdio.h>

typedef unsigned char uint8;

typedef enum io_mode {
    READ, 
    WRITE
} io_mode;

typedef struct huffman_io {

    uint8 curr_byte;
    int bits_set;
    FILE* file;
    int eof_reached;

} huffman_io;

huffman_io init_io(FILE* file, io_mode mode);

// read
uint8 read_bit(huffman_io* io);
uint8 read_byte(huffman_io* io);

// write
void write_bit(huffman_io* io, uint8 bit);
void write_byte(huffman_io* io, uint8 byte);
void flush(huffman_io* io);


#endif //HUFFMAN_IO_H