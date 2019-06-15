#ifndef DISTRIBUTION_H
#define DISTRIBUTION_H

#include <stdint.h>
#include "matrix.h"

int distributeSecret(const char * image, uint8_t k, uint8_t n, const char * dir, const char * watermark);
void fillRandomMatrix(matrix_t * matrix);
void fillLinearlyIndependentMatrix(matrix_t * matrix, int a);
bool isValueInArray(uint8_t * array, uint8_t value, int size);
uint8_t * generateAValues(uint8_t n);
matrix_t * createAMatrix(uint8_t k, uint8_t n);
matrix_t ** createShMatrices(matrix_t ** V, matrix_t ** G, uint8_t n);
matrix_t ** createGMatrices(matrix_t * R, uint8_t k, uint8_t n, uint8_t m);
matrix_t ** createVMatrices(matrix_t * A, matrix_t ** X, uint8_t n);
matrix_t ** createXMatrices(uint8_t k, uint8_t n);
matrix_t ** getRColumns(matrix_t * R);
void deleteRColumns(size_t columns, matrix_t ** RCols);
matrix_t * createGMatrix(matrix_t ** RCols, uint8_t k, uint8_t m, int c);

#endif