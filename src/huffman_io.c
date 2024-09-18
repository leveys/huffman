#include "huffman_io.h"

// create huffman io struct
// mode is READ or WRITE
huffman_io init_io(FILE* file, io_mode mode) {
    huffman_io io;
    io.file = file;
    io.eof_reached = 0;
    io.curr_byte = 0;
    // if mode is read, initialize bits_set with 8 to read first byte
    io.bits_set = mode == READ ? 8 : 0;

    return io;
}

// read one bit from input
// if input could not be read, or end of input is reached, set eof_reached flag
uint8 read_bit(huffman_io* io) {

    // 8 bits read, read new byte
    if (io->bits_set >= 8) {

        int c = fgetc(io->file);
        // check for end of file
        if (c == EOF) {
            io->eof_reached = 1;
            return 0;
        }
        io->curr_byte = (uint8)c;
        io->bits_set = 0;
    }

    uint8 bit = io->curr_byte >> 7;
    io->curr_byte <<= 1;
    io->bits_set++;
    return bit;
}

// read one byte from input, taking the current byte into account
uint8 read_byte(huffman_io* io) {

    uint8 byte = io->curr_byte;

    // read new byte
    int c = fgetc(io->file);
    // check for end of file
    if (c == EOF) {
        io->eof_reached = 1;
        io->curr_byte = 0;
    } else {
        io->curr_byte = (uint8)c;
    }

    // set number of bits not yet set
    byte |= io->curr_byte >> (8 - io->bits_set);

    // remove all bits that were just read
    io->curr_byte <<= io->bits_set;

    return byte;
}

// write one bit to output 
void write_bit(huffman_io* io, uint8 bit) {
    
    // 8 bits set, output byte
    if (io->bits_set >= 8) {
        fputc(io->curr_byte, io->file);
        io->bits_set = 0;
    }

    io->curr_byte <<= 1;
    io->curr_byte |= bit;
    io->bits_set++;
}

void write_byte(huffman_io* io, uint8 byte) {

    // set number of bits not yet set
    io->curr_byte <<= 8 - io->bits_set;
    io->curr_byte |= byte >> io->bits_set;

    // output byte
    fputc(io->curr_byte, io->file);

    // set bits that are not yet written
    io->curr_byte = byte;
}

void flush(huffman_io* io) {
    
    // check if last byte not filled completely
    if (io->bits_set > 0) {
        // fill last byte with zeroes
        io->curr_byte <<= 8 - io->bits_set;
        fputc(io->curr_byte, io->file);
    }
}