#include <stdio.h>
#include <stdint.h>
#include <math.h>
#include "distribution.h"
#include "matrix.h"
#include "random.h"
#include "bmp.h"

#define SET 10  // TODO: revise this, set with system clock
#define MAX 50 
#define MOD ((uint8_t) 251) // closes prime
#define MATRIX_X_COLS 1

// seed must be 48 bits;
int64_t seed; 

int distributeSecret(const char * image, uint8_t k, uint8_t n, const char * dir, const char * watermark)
{
    setSeed(SET);
    generateModInverses(MOD);
    matrix_t * S = read_bmp(image, false);
    matrix_t * A  = createAMatrix(k, n);
    matrix_t * Sproj = proj(A, MOD);
    matrix_t * R = substract(Sproj, S, MOD);
    matrix_t ** X = createXMatrices(k, n);
    matrix_t ** V = createVMatrices(A, X, n);
    matrix_t ** G = createGMatrices(R, V, n);
    matrix_t ** Sh = createShMatrices(V, G, n);
    // Generate watermark and make remainder Rw public
    matrix_t * W = read_bmp(image, true);
    matrix_t * Rw = substract(W, Sproj, MOD);

    return 1;
}

matrix_t ** createShMatrices(matrix_t ** V, matrix_t ** G, uint8_t n)
{
    matrix_t ** Sh = calloc(n, sizeof(uint32_t));
    for (uint8_t i = 0; i < n; i++)
    {
        Sh[i] = append(V[i], G[i]);
    }
    return Sh;
}

matrix_t ** createGMatrices(matrix_t * A, matrix_t ** V, uint8_t n)
{
    for (uint8_t i = 0; i < n; i++)
    {
        // TODO: complete
    }
    return NULL;
}

matrix_t ** createVMatrices(matrix_t * A, matrix_t ** X, uint8_t n)
{
    matrix_t ** matrices = calloc(n, sizeof(uint32_t));
    for (uint8_t i = 0; i < n; i++)
    {
        matrices[i] = multiply(A, X[i], MOD);
    }
    return matrices;
}

matrix_t ** createXMatrices(uint8_t k, uint8_t n)
{
    matrix_t ** matrices = calloc(n, sizeof(uint32_t));
    for (uint8_t i = 0; i < n; i++)
    {
        matrices[i] = create(k, MATRIX_X_COLS);
        fillLinearlyIndependentMatrix(matrices[i], (i + 1) % MOD); // TODO: revise this
    }
    return matrices;
}

matrix_t * createAMatrix(uint8_t k, uint8_t n)
{
    matrix_t * A = NULL, * At = NULL, * aux = NULL;
    int m =  2 * (k - 1); // TODO: revise this
    A = create(m, k);
    do 
    {   
        delete(At);
        delete(A);
        delete(aux);
        At = transpose(A);
        aux = multiply(A, At, MOD);
        fillRandomMatrix(A);
    }
    while (getRank(aux, MOD) == n && getRank(A, MOD) != k);
    delete(aux);
    delete(At);
    return A;
}

void fillRandomMatrix(matrix_t * matrix)
{
    for (uint8_t i = 0; i < matrix->rows; i++)
    {
        for (uint8_t j = 0; j < matrix->columns; j++)
        {
            matrix->data[i][j] = nextChar(seed) % MOD;
        }
    }
}

void fillLinearlyIndependentMatrix(matrix_t * matrix, int a)
{
    for (uint8_t i = 0; i < matrix->rows; i++)
    {
        for (uint8_t j = 0; j < matrix->columns; j++)
        {
            matrix->data[i][j] = (uint8_t) pow(a, i);
        }
    }
}

