#ifndef MATRIX_LIBRARY_H
#define MATRIX_LIBRARY_H

#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include <string.h>

typedef struct {
    uint8_t ** data;
    size_t columns;
    size_t rows;
} matrix_t;

matrix_t* create(size_t rows, size_t columns);

bool delete(matrix_t * matrix);

matrix_t * sum(const matrix_t * m1, const matrix_t * m2, uint8_t mod);

matrix_t * substract(const matrix_t * m1, const matrix_t * m2, uint8_t mod);

matrix_t * multiply(const matrix_t * m1, const matrix_t * m2, uint8_t mod);

matrix_t * transpose(const matrix_t * m);

bool setPosition(matrix_t * matrix, const size_t row, const size_t column, const uint8_t value);

bool generateModInverses(int mod);

matrix_t * identity(const size_t size);

bool equals(const matrix_t * m1, const matrix_t * m2);

matrix_t * gaussElimination(const matrix_t * matrix, const uint8_t mod);

size_t getRank(const matrix_t * matrix, const uint8_t mod);

matrix_t * inverseMatrix(const matrix_t * matrix, const uint8_t mod);

matrix_t * proj(const matrix_t * matrix, const uint8_t mod);

matrix_t * augment(const matrix_t * m1, const matrix_t * m2);

matrix_t * getColumn(const matrix_t * matrix, size_t column);

matrix_t * sumInPlace(matrix_t * m1, const matrix_t * m2, uint8_t mod);

matrix_t * multiplyByScalar(const matrix_t * m1, const uint8_t scalar, uint8_t mod);

matrix_t ** getNxMMatrices(matrix_t * matrix, size_t n, size_t m, size_t *count);

matrix_t * joinMatrices(matrix_t ** matrices, size_t count, size_t rows, size_t columns);

void deleteMatrices(size_t count, matrix_t ** matrices);

matrix_t * reduce(const matrix_t * matrix, const size_t rowStart, const size_t rowEnd,
                  const size_t colStart, const size_t colEnd);

void deleteModInverses();

#endif