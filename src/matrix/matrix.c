#include "matrix.h"

// TODO: define elsewhere
typedef unsigned int uint;

//Generate once
static uint inverse[MAX];

matrix_t *create(const size_t rows, const size_t columns) {
  matrix_t* matrix = malloc(sizeof(matrix_t));
  uint * row;
  int i, j;

  if(NULL == matrix) {
    return NULL;
  }

  matrix->columns = columns;
  matrix->rows = rows;

  uint ** data = malloc(sizeof(uint *) * rows);

  if(NULL == data) {
    free(matrix);
    return NULL;
  }

  bool nullRow = false;

  for (i = 0; i < rows; i++) {
    row = malloc(sizeof(uint) * columns);

    if(NULL == row) {
      nullRow = true;
    }

    data[i] = row;
  }

  if(nullRow) {
    for (j = 0; j < i; j++) {
      free(data[j]);
    }
    free(data);
    free(matrix);
    return NULL;
  }

  matrix->data = data;

  return matrix;
}

void delete(matrix_t * matrix) {
  int i;

  if(NULL == matrix)
    return ;

  for (i = 0; i < matrix->rows; i++) {
    free(matrix->data[i]);
  }
  free(matrix->data);
  free(matrix);
}

uint* rowSum(const uint * row1, const uint * row2, uint * result,  uint mod, size_t length) {

  if(NULL == row1 || NULL == row2 || NULL == result)
    return NULL;

  int i;
  for (i = 0; i < length; i++) {
    result[i] = (row1[i] + row2[i]) % mod;
  }

  return result;
}

uint* rowTimesScalar(const uint * row, uint * result,  uint mod, size_t length, int scalar) {

  if(NULL == row || NULL == result)
    return NULL;

  int i;
  for (i = 0; i < length; i++) {
    result[i] = (row[i] * scalar) % mod;
  }

  return result;
}

uint* rowDividedByScalar(const uint * row, uint * result,  uint mod, size_t length, int scalar) {

  if(NULL == row || NULL == result)
    return NULL;

  uint inv = inverse[scalar];

  result = rowTimesScalar(row, result, mod, length, inv);

  return result;
}

uint* rowSubstract(const uint * row1, const uint * row2, uint * result,  uint mod, size_t length) {

  if(NULL == row1 || NULL == row2 || NULL == result)
    return NULL;

  int i;
  for (i = 0; i < length; i++) {
    result[i] = (((row1[i] - row2[i]) % mod) + mod) % mod;
  }

  return result;
}

matrix_t * sum(const matrix_t * m1, const matrix_t * m2, matrix_t * result, uint mod) {

  if(NULL == m1 || NULL == m2 || NULL == result)
    return NULL;

  if(m1->columns != m2->columns || m1->rows != m2->rows)
    return NULL;

  size_t columns = m1->columns;
  size_t rows = m1->rows;

  if(result->columns != columns || result->rows != rows)
    return NULL;

  int i;
  for (i = 0; i < rows; i++) {
    rowSum(m1->data[i], m2->data[i], result->data[i], mod, columns);
  }
  return result;
}

matrix_t * substract(const matrix_t * m1, const matrix_t * m2, matrix_t * result, uint mod) {

  if(NULL == m1 || NULL == m2 || NULL == result)
    return NULL;

  if(m1->columns != m2->columns || m1->rows != m2->rows)
    return NULL;

  size_t columns = m1->columns;
  size_t rows = m1->rows;

  if(result->columns != columns || result->rows != rows)
    return NULL;

  int i;
  for (i = 0; i < rows; i++) {
    rowSubstract(m1->data[i], m2->data[i], result->data[i], mod, columns);
  }
  return result;
}

matrix_t * multiply(const matrix_t * m1, const matrix_t * m2, matrix_t * result, uint mod) {

  if(NULL == m1 || NULL == m2 || NULL == result)
    return NULL;

  if(m1->columns != m2->rows)
    return NULL;

  if(result->rows != m1->rows || result->columns != m2->columns)
    return NULL;

  int i, j, k;
  for (i = 0; i < m1->rows; i++) {
    for (j = 0; j < m2->columns; j++) {
      for (k = 0; k < m2->rows; k++) {
        result->data[i][j] += (m1->data[i][k] * m2->data[k][j]);
      }
      result->data[i][j] %= mod;
    }
  }
  return result;
}

int modInverse(int a, int mod) {

  if(a < 1)
    return -1;

  int remainder;
  int x[2] = {0, 1};
  int aux;
  int b = mod;

  while(a != 0) {
    aux = x[0];
    x[0] = x[1];
    x[1] = aux - b/a * x[1];

    remainder = b % a;
    b = a;
    a = remainder;
  }

  return (x[0] % mod + mod) % mod;
}

void print(matrix_t * m1) {
  int i, j;
  for (i = 0; i < m1->rows; i++) {
    for (j = 0; j < m1->columns; j++) {
      printf("%d ", m1->data[i][j]);
    }
    putchar('\n');
  }
}