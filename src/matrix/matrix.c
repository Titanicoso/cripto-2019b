#include "matrix.h"

//Generate once
uint8_t* inverse = NULL;

matrix_t *create(const size_t rows, const size_t columns) {
  matrix_t* matrix = malloc(sizeof(matrix_t));
  uint8_t * row;
  size_t i, j;

  if(NULL == matrix) {
    return NULL;
  }

  matrix->columns = columns;
  matrix->rows = rows;

  uint8_t ** data = malloc(sizeof(uint8_t *) * rows);

  if(NULL == data) {
    free(matrix);
    return NULL;
  }

  bool nullRow = false;

  for (i = 0; i < rows; i++) {
    row = malloc(sizeof(uint8_t) * columns);

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
  size_t i;

  if(NULL == matrix)
    return ;

  for (i = 0; i < matrix->rows; i++) {
    free(matrix->data[i]);
  }
  free(matrix->data);
  free(matrix);
}

uint8_t* rowSum(const uint8_t * row1, const uint8_t * row2, uint8_t * result,  uint8_t mod, size_t length) {

  if(NULL == row1 || NULL == row2 || NULL == result)
    return NULL;

  size_t i;
  for (i = 0; i < length; i++) {
    result[i] = (row1[i] + row2[i]) % mod;
  }

  return result;
}

uint8_t* rowTimesScalar(const uint8_t * row, uint8_t * result,  uint8_t mod, size_t length, uint8_t scalar) {

  if(NULL == row || NULL == result)
    return NULL;

  size_t i;
  for (i = 0; i < length; i++) {
    result[i] = (row[i] * scalar) % mod;
  }

  return result;
}

uint8_t* rowDividedByScalar(const uint8_t * row, uint8_t * result,  uint8_t mod, size_t length, uint8_t scalar) {

  if(NULL == row || NULL == result)
    return NULL;

  uint8_t inv = inverse[scalar];

  result = rowTimesScalar(row, result, mod, length, inv);

  return result;
}

uint8_t* rowSubstract(const uint8_t * row1, const uint8_t * row2, uint8_t * result,  uint8_t mod, size_t length) {

  if(NULL == row1 || NULL == row2 || NULL == result)
    return NULL;

  size_t i;
  for (i = 0; i < length; i++) {
    result[i] = (((row1[i] - row2[i]) % mod) + mod) % mod;
  }

  return result;
}

matrix_t * sum(const matrix_t * m1, const matrix_t * m2, uint8_t mod) {

  if(NULL == m1 || NULL == m2)
    return NULL;

  if(m1->columns != m2->columns || m1->rows != m2->rows)
    return NULL;

  size_t columns = m1->columns;
  size_t rows = m1->rows;

  matrix_t * result = create(rows, columns);

  if(NULL == result)
    return NULL;

  size_t i;
  for (i = 0; i < rows; i++) {
    rowSum(m1->data[i], m2->data[i], result->data[i], mod, columns);
  }
  return result;
}

matrix_t * substract(const matrix_t * m1, const matrix_t * m2, uint8_t mod) {

  if(NULL == m1 || NULL == m2)
    return NULL;

  if(m1->columns != m2->columns || m1->rows != m2->rows)
    return NULL;

  size_t columns = m1->columns;
  size_t rows = m1->rows;

  matrix_t * result = create(rows, columns);

  if(NULL == result)
    return NULL;

  size_t i;
  for (i = 0; i < rows; i++) {
    rowSubstract(m1->data[i], m2->data[i], result->data[i], mod, columns);
  }
  return result;
}

matrix_t * multiply(const matrix_t * m1, const matrix_t * m2, uint8_t mod) {

  if(NULL == m1 || NULL == m2 )
    return NULL;

  if(m1->columns != m2->rows)
    return NULL;

  matrix_t * result = create(m1->rows, m2->columns);
  matrix_t * m2T = transpose(m2);

  if(NULL == result)
    return NULL;

  size_t i, j, k;
  for (i = 0; i < m1->rows; i++) {
    for (j = 0; j < m2T->rows; j++) {
      for (k = 0; k < m2T->columns; k++) {
        result->data[i][j] += (m1->data[i][k] * m2T->data[j][k]);
      }
      result->data[i][j] %= mod;
    }
  }

  free(m2T);
  return result;
}

matrix_t * transpose(const matrix_t * m) {

  if(NULL == m)
    return NULL;

  size_t columns = m->columns;
  size_t rows = m->rows;

  matrix_t * result = create(columns, rows);

  if(NULL == result)
    return NULL;

  size_t i, j;
  for (i = 0; i < rows; i++) {
    for (j = 0; j < columns; j++) {
      result->data[j][i] = m->data[i][j];
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

void generateModInverses(int mod) {

  inverse = malloc(sizeof(uint8_t) * mod);
  if(NULL == inverse) {
    return ;
  }

  int i;
  for (i = 1; i < mod; i++) {
    inverse[i] = (uint8_t)modInverse(i, mod);
  }
}