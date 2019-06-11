#ifndef __MNIST_READ_H__
#define __MNIST_READ_H__

#include <stdio.h>
void read_header_MNISTimage(FILE *fp, int *nimg, int *nrow, int *ncol);
double read_grayscale_pixel(FILE *fp);

#endif