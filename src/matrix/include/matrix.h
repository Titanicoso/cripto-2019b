#ifndef MATRIX_LIBRARY_H
#define MATRIX_LIBRARY_H

#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

typedef struct {
    uint8_t ** data;
    size_t columns;
    size_t rows;
} matrix_t;

matrix_t* create(size_t rows, size_t columns);

void delete(matrix_t * matrix);

matrix_t * sum(const matrix_t * m1, const matrix_t * m2, uint8_t mod);

matrix_t * substract(const matrix_t * m1, const matrix_t * m2, uint8_t mod);

matrix_t * multiply(const matrix_t * m1, const matrix_t * m2, uint8_t mod);

matrix_t * transpose(const matrix_t * m);

#endif