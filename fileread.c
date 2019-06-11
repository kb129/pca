#include <stdio.h>
#include "fileread.h"

typedef union {
    int l;
    struct {
        short h, l;
    }s;
}SwapEndian;

typedef union {
    unsigned short s;
    struct {
        unsigned char h, l;
    }c;
}SwapEndianShort;

int readInt_SwapEndianShort(FILE *fp){
    SwapEndianShort s;
    fread(&s.c.l, 1, 1, fp);
    fread(&s.c.h, 1, 1, fp);
    return s.s;
}

int readInt_SwapEndian(FILE *fp){
    SwapEndian d;
    d.s.l = readInt_SwapEndianShort(fp);
    d.s.h = readInt_SwapEndianShort(fp);
    return d.l;
}

void read_header_MNISTimage(FILE *fp, int *nimg, int *nrow, int *ncol){
    int magic_data;
    magic_data = readInt_SwapEndian(fp);
    *nimg = readInt_SwapEndian(fp);
    *nrow = readInt_SwapEndian(fp);
    *ncol = readInt_SwapEndian(fp);
    fprintf(stderr, "CHECK: magic = %08x (%d)\n", magic_data, magic_data);
    fprintf(stderr, "CHECK: nimg  = %08x (%d)\n", *nimg);
    fprintf(stderr, "CHECK: nrow  = %08x (%d)\n", *nrow);
    fprintf(stderr, "CHECK: ncol  = %08x (%d)\n", *ncol);
}

double read_grayscale_pixel(FILE *fp){
    unsigned char d;
    fread(&d, 1, 1, fp);
    return d / (double)255.0;
}
