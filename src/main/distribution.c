#include <stdio.h>
#include "distribution.h"
#include "matrix.h"
#include "random.h"
#include "bmp.h"

#define SET 10  // TODO: revise this, set with system clock
#define MAX 50 
#define MOD ((uint8_t) 251) // closes prime

// seed must be 48 bits;
int64_t seed; 

int distributeSecret(const char * image, int k, int n, const char * dir, const char * watermark)
{
    setSeed(SET);
    generateModInverses(MOD);
    matrix_t * S = read_bmp(image, false);
    matrix_t * A  = createAMatrix((uint8_t) k, (uint8_t) n);
    matrix_t * Sproj = proj(A, MOD);
    matrix_t * R = substract(Sproj, S, MOD);

    return 1;
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
