#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <getopt.h>
#include <time.h>
#include "huffman.h"

#define CLOCKS_PER_MS 1000

void print_usage(int disp_table) {
    printf("\nUsage: persen [-c LEN,MEM | -d] [-i INPUTFILE] [-o OUTPUTFILE] [-h]\n\n");
    printf("\t-c: compress a file using huffman coding, this option requires 2 additional arguments: LEN and MEM\n");
    printf("\t\tLEN is the upper bound for number of characters in one leaf node, this must be an integer between 1 and 255\n");
    printf("\t\tMEM is the index used to look up the maximum memory usage, this must be an integer between 0 and 9\n");
    printf("\t\tuse option -h to display lookup table\n\n");
    if (disp_table) {
        printf("\t\tmax memory lookup table:\n");
        printf("\t\t[ 0 | 75 KiB  ]\n");
        printf("\t\t[ 1 | 75 KiB  ]\n");
        printf("\t\t[ 2 | 100 KiB ]\n");
        printf("\t\t[ 3 | 200 KiB ]\n");
        printf("\t\t[ 4 | 1 MiB   ]\n");
        printf("\t\t[ 5 | 20 MiB  ]\n");
        printf("\t\t[ 6 | 50 MiB  ]\n");
        printf("\t\t[ 7 | 100 MiB ]\n");
        printf("\t\t[ 8 | 500 MiB ]\n");
        printf("\t\t[ 9 | 1 GiB   ]\n\n");
    }
    printf("\t-d: decompress a file, no further arguments required\n");
    printf("\t-i: specify input file, if not specified, standard input will be used\n");
    printf("\t-o: specify output file, if not specified, standard output will be used\n");
    printf("\t-h: display this help with memory lookup table and exit\n\n");
    exit(!disp_table);
}

int main(int argc, char **argv) {

    int cflag = 0;      // compress input file
    int max_chars = 0;  // max characters per node
    int max_mem = 0;    // max memory to be allocated
    int dflag = 0;      // decompress input file
    int iflag = 0;      // input file is given
    int oflag = 0;      // output file is given
    int tflag = 0;      // output execution time
    FILE* inputfile = stdin;
    FILE* outputfile = stdout;

    int opt;

    while ((opt = getopt(argc, argv, "c:di:o:th")) != -1) {
        switch (opt) {
            case 'c': {
                char* arg1 = strtok(optarg, ",");
                char* arg2 = strtok(NULL, ",");

                if (arg1 && arg2) {
                    max_chars = (int)strtol(arg1, NULL, 10);
                    max_mem = (int)strtol(arg2, NULL, 10);
                    
                    if (max_chars > 0 && max_chars < 256 && max_mem >= 0 && max_mem < 10) {
                        cflag = 1;
                    }
                } 
                if (!cflag) {
                    fprintf(stderr, "Error: incorrect arguments for -c option\n");
                    print_usage(0);
                }
                break;
            }
            
            case 'd':
                dflag = 1;
                break;
            
            case 'i':
                if (!(inputfile = fopen(optarg, "rb"))) {
                    fprintf(stderr, "Error: could not open input file\n");
                    exit(1);
                }
                iflag = 1;
                break;
            
            case 'o':
                if (!(outputfile = fopen(optarg, "wb"))) {
                    fprintf(stderr, "Error: could not open output file\n");
                    exit(1);
                }
                oflag = 1;
                break;
            
            case 't':
                tflag = 1;
                break;
            
            case 'h':
                print_usage(1);
                break;

            case '?':
                if (optopt == 'c' || optopt == 'i' || optopt == 'o') {
                    fprintf(stderr, "Error: option -%c requires an argument\n", optopt);
                } else {
                    fprintf(stderr, "Error: unknown option\n");
                }
                print_usage(0);
                
            default:
                print_usage(0);
                break;
        }
    }

    clock_t start = clock() / CLOCKS_PER_MS;

    if (cflag && dflag) {
        fprintf(stderr, "Error: cannot set both -c and -d option\n");
        print_usage(0);
    } else if (cflag) {
        compress(max_chars, max_mem, inputfile, outputfile);

        if (tflag) {
            clock_t end = clock() / CLOCKS_PER_MS;
            printf("compression time: %ld ms\n", end-start);
        }
    } else if (dflag) {
        decompress(inputfile, outputfile);

        if (tflag) {
            clock_t end = clock() / CLOCKS_PER_MS;
            printf("decompression time: %ld ms\n", end-start);
        }
    } else {
        fprintf(stderr, "Error: must set either -c or -d option\n");
        print_usage(0);
    }

    if (iflag) fclose(inputfile);
    if (oflag) fclose(outputfile);

    return 0;
}