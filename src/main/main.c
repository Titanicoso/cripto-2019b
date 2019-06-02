#include <stdio.h>
#include <limits.h>
#include <stdlib.h>
#include <stdint.h>
#include "matrix.h"
#include "bmp.h"

int 
main(void)
{
    matrix_t * matrix = read_bmp("backtofutureshare.bmp", true);
    //printf("%d %d\n", matrix->rows, matrix->columns);
    int i, j;
    for (i = 0; i < matrix->rows; i++) {
        for (j = 0; j < matrix->columns; j++) {
            printf("%.2x ", matrix->data[i][j]);
        }
        printf("\n");
    }
}