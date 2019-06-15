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
    row = calloc(columns, sizeof(uint8_t));

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

bool delete(matrix_t * matrix) {
  size_t i;

  if(NULL == matrix)
    return false;

  for (i = 0; i < matrix->rows; i++) {
    free(matrix->data[i]);
  }
  free(matrix->data);
  free(matrix);

  return true;
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
      uint16_t value = 0;
      for (k = 0; k < m2T->columns; k++) {
        value += (m1->data[i][k] * m2T->data[j][k]);
      }
      result->data[i][j] = (uint8_t) (value % mod);
    }
  }

  delete(m2T);
  return result;
}

matrix_t * multiplyByScalar(const matrix_t * m1, const uint8_t scalar, uint8_t mod) {

  if(NULL == m1)
    return NULL;

  matrix_t * result = create(m1->rows, m1->columns);

  if(NULL == result)
    return NULL;

  size_t i;
  for (i = 0; i < m1->rows; i++) {
    rowTimesScalar(m1->data[i], result->data[i], mod, m1->columns, scalar);
  }

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

bool setPosition(matrix_t * matrix, const size_t row, const size_t column, const uint8_t value) {
  if(NULL == matrix)
    return false;

  if(row >= matrix->rows || column >= matrix->columns)
    return false;

  matrix->data[row][column] = value;
  return true;
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

bool generateModInverses(int mod) {

  inverse = malloc(sizeof(uint8_t) * mod);
  if(NULL == inverse) {
    return false;
  }

  int i;
  for (i = 1; i < mod; i++) {
    inverse[i] = (uint8_t)modInverse(i, mod);
  }

  return true;
}

matrix_t * identity(const size_t size) {

  matrix_t * matrix = create(size, size);

  if(NULL == matrix)
    return NULL;

  size_t i;
  for (i = 0; i < size; i++) {
      setPosition(matrix, i, i, 1);
  }

  return matrix;
}

bool copyRow(uint8_t * dest, const uint8_t * src, const size_t size) {

  if(NULL == src || NULL == dest) {
    return false;
  }

  size_t i;
  for (i = 0; i < size; i++) {
    dest[i] = src[i];
  }

  return true;
}

matrix_t * augment(const matrix_t * m1, const matrix_t * m2)
{

  if(NULL == m1 || NULL == m2) {
    return NULL;
  }

  size_t rows = m1->rows > m2->rows ? m1->rows : m2->rows;
  size_t columns = m1->columns + m2->columns;

  matrix_t * augmented = create(rows, columns);

  if(NULL == augmented)
    return NULL;

  size_t i;
  for (i = 0; i < rows; i++) {
    if(m1->rows > i) {
      copyRow(augmented->data[i], m1->data[i], m1->columns);
    }
    if(m2->rows > i) {
      copyRow(augmented->data[i] + m1->columns, m2->data[i], m2->columns);
    }
  }
  return augmented;
}

bool swapRows(matrix_t * matrix, const size_t row1, const size_t row2) {

  if(NULL == matrix) {
    return false;
  }

  if(row1 >= matrix->rows || row2 >= matrix->rows) {
    return false;
  }

  if(row1 == row2){
    return true;
  }

  size_t columns = matrix->columns;
  uint8_t * aux = malloc(sizeof(uint8_t) * columns);

  if(NULL == aux)
    return false;

  copyRow(aux, matrix->data[row2], columns);
  copyRow(matrix->data[row2], matrix->data[row1], columns);
  copyRow(matrix->data[row1], aux, columns);
  free(aux);

  return true;
}

bool equals(const matrix_t * m1, const matrix_t * m2) {

  if(m1->rows != m2->rows)
    return false;

  if(m1->columns != m2->columns)
    return false;

  size_t i, j;
  for (i = 0; i < m1->rows; i++) {
    for (j = 0; j < m1->columns; j++) {
      if(m1->data[i][j] != m2->data[i][j])
        return false;
    }
  }

  return true;
}

matrix_t * clone(const matrix_t * matrix) {

  if(NULL == matrix)
    return NULL;

  size_t rows = matrix->rows;
  size_t columns = matrix->columns;

  matrix_t * result = create(rows, columns);

  if(NULL == result)
    return NULL;

  size_t i;
  for (i = 0; i < rows; i++) {
    copyRow(result->data[i], matrix->data[i], columns);
  }

  return result;
}

matrix_t * reduce(const matrix_t * matrix, const size_t rowStart, const size_t rowEnd,
        const size_t colStart, const size_t colEnd) {

  if(NULL == matrix)
    return NULL;

  size_t rows = matrix->rows;
  size_t columns = matrix->columns;

  if(rowStart > rowEnd || rowStart > rows || rowEnd > rows)
    return NULL;

  if(colStart > colEnd || colStart > columns || colEnd > columns)
    return NULL;

  matrix_t * result = create(rowEnd - rowStart, colEnd - colStart);

  if(NULL == result)
    return NULL;

  size_t i;
  for (i = rowStart; i < rowEnd; i++) {
    copyRow(result->data[i - rowStart], matrix->data[i] + colStart, colEnd - colStart);
  }

  return result;
}

long findPivot(const matrix_t * matrix, const size_t startRow, const size_t column) {

  if(NULL == matrix)
    return -1;

  size_t rows = matrix->rows;
  size_t columns = matrix->columns;

  if(startRow >= rows || column >= columns)
    return -1;

  size_t i;
  long index = -1;
  bool found = false;
  for (i = startRow; i < rows && !found; i++) {
    if(matrix->data[i][column] != 0) {
      found = true;
      index = i;
    }
  }

  return index;
}

matrix_t * gaussElimination(const matrix_t * matrix, const uint8_t mod) {

  if(NULL == matrix) {
    return NULL;
  }

  size_t rows = matrix->rows;
  size_t columns = matrix->columns;

  matrix_t * result = clone(matrix);

  if(NULL == result)
    return NULL;

  uint8_t * aux = malloc(sizeof(uint8_t) * columns);

  if(NULL == aux)
    return NULL;

  size_t i = 0, j = 0;
  uint8_t value = 0;
  size_t row;

  while(i < rows && j < columns) {

    long index = findPivot(result, i, j);

    if(index >= 0) {
      value = result->data[index][j];
      swapRows(result, (size_t) index, i);

      if(value != 1) {
        rowDividedByScalar(result->data[i], result->data[i], mod, columns, value);
      }

      for (row = 0; row < rows; row++) {
        uint8_t element = result->data[row][j];
        if(row != i && element != 0) {
          aux = rowTimesScalar(result->data[i], aux, mod, columns, element);
          result->data[row] = rowSubstract(result->data[row], aux, result->data[row], mod, columns);
        }
      }
      i++;
    }
    j++;
  }

  return result;
}

bool nonZeroRow(const uint8_t * row, const size_t columns) {

  if(NULL == row)
    return false;

  size_t i;
  for (i = 0; i < columns; i++) {
    if(row[i] != 0) {
      return true;
    }
  }

  return false;
}

size_t getRowEchelonRank(const matrix_t * matrix) {

  if (NULL == matrix)
    return 0;

  size_t i;
  size_t counter = 0;
  for (i = 0; i < matrix->rows; i++) {
    if (nonZeroRow(matrix->data[i], matrix->columns)) {
      counter++;
    }
  }

  return counter;
}

size_t getRank(const matrix_t * matrix, const uint8_t mod) {

  if (NULL == matrix)
    return 0;

  matrix_t * rowEchelon = gaussElimination(matrix, mod);

  size_t rank = getRowEchelonRank(rowEchelon);

  delete(rowEchelon);

  return rank;
}

matrix_t * inverseMatrix(const matrix_t * matrix, const uint8_t mod) {

  if(NULL == matrix) {
    return NULL;
  }

  size_t rows = matrix->rows;
  size_t columns = matrix->columns;

  if(rows != columns) {
    return NULL;
  }

  matrix_t * iden = identity(rows);

  if(NULL == iden) {
    return NULL;
  }

  matrix_t * augmented = augment(matrix, iden);

  if(NULL == augmented) {
    delete(iden);
    return NULL;
  }

  matrix_t * result = gaussElimination(augmented, mod);

  matrix_t * inv = reduce(result, 0, rows, columns, augmented->columns);
  matrix_t * aux = reduce(result, 0, rows, 0, columns);

  size_t rank = getRowEchelonRank(aux);

  delete(aux);
  delete(result);
  delete(augmented);
  delete(iden);

  if(rank != columns) {
    delete(inv);
    return NULL;
  }

  return inv;
}

matrix_t * proj(const matrix_t * matrix, const uint8_t mod) {

  matrix_t * transposed = transpose(matrix);
  matrix_t * multiplication = multiply(transposed, matrix, mod);
  matrix_t * inverse = inverseMatrix(multiplication, mod);
  matrix_t * aux = multiply(matrix, inverse, mod);
  matrix_t * result = multiply(aux, transposed, mod);

  delete(transposed);
  delete(multiplication);
  delete(inverse);
  delete(aux);

  return result;
}
