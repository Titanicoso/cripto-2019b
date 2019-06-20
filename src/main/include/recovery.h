#ifndef RECOVERY_H
#define RECOVERY_H

#include <stdint.h>
#include "matrix.h"

int recoverSecret(const char * image, uint8_t k, uint8_t n, const char * dir, const char * watermark);
matrix_t *** splitShares(matrix_t ** shares, uint8_t n, uint8_t k, size_t* secretCount);
matrix_t * createBMatrix(matrix_t *** Sh, size_t secretIndex, uint8_t n, uint8_t k);
matrix_t ** createGMatricesRecovery(matrix_t *** Sh, size_t secretIndex, uint8_t n, uint8_t k);
matrix_t * createLeftyMatrix(matrix_t *** Sh, uint8_t n, uint8_t k);
matrix_t * createRightyMatrix(matrix_t ** G, uint8_t k, size_t row, size_t column);
matrix_t * createRMatrixRecovery(matrix_t ** G, matrix_t * lefty, uint8_t n, uint8_t k);

#endif