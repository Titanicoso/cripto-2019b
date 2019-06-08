#ifndef DISTRIBUTION_H
#define DISTRIBUTION_H

#include "matrix.h"

int distributeSecret(const char * image, int k, int n, const char * dir, const char * watermark);
void fillRandomMatrix(matrix_t * matrix);
matrix_t * createAMatrix(uint8_t k, uint8_t n);

#endif