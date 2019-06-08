#include <stdio.h>
#include "matrix.h"
#include "random.h"

#define SET 10
#define MAX 50 
/*variable global*/
int64_t seed; /*seed debe ser de 48 bits; se elige este tipo de 64 bits*/ 

int distributeImage(const char * image, int k, int n, const char * dir, const char * watermark)
{
    setSeed(SET);
    matrix_t * A  = createAMatrix(k);
}

matrix_t * createAMatrix(int k)
{
    int m =  nextChar(seed) + 2 * (k - 1) - 1; // TODO: revise this
    matrix_t * A = create(m, k);
    fillAMatrix(A);
    while (!isMatrixOfRange(A, k))
    {
        fillRandomMatrix(A);
    }
    return A;
}

void fillRandomMatrix(matrix_t * matrix)
{
    for (int i = 0; i < matrix->rows; i++)
    {
        for (int j = 0; j < matrix->columns; j++)
        {
            matrix->data[i][j] = nextChar(seed);
        }
    }
}
