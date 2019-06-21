#include <math.h>
#include "recovery.h"
#include "steganography.h"
#include "bmp.h"

#define MOD ((uint8_t) 251)
#define WATERMARK_FILE "Watermark.bmp"

//TODO: Show errors
bool recoverSecret(const char * image, uint8_t k, uint8_t n, const char * dir, const char * rw)
{
  generateModInverses(MOD);
  size_t secretCount, watermarkCount;
  matrix_t ** shares = recover_shares(dir, k, n);
  BITMAP * rw_image = read_bmp(rw, false, true);
  size_t rows = rw_image->matrix->rows;
  size_t columns = rw_image->matrix->columns;
  matrix_t ** rwMatrices = getNxMMatrices(rw_image->matrix, n, n, &watermarkCount);
  matrix_t *** Sh = splitShares(shares, n, k, &secretCount);
  deleteMatrices(n, shares);

  matrix_t ** secret = calloc(secretCount, sizeof(matrix_t *));
  matrix_t ** watermark = calloc(watermarkCount, sizeof(matrix_t *));

  matrix_t * lefty = createLeftyMatrix(Sh, n, k);
  for(size_t i = 0; i < secretCount; i++)
  {
    matrix_t * Rw = rwMatrices[i];
    matrix_t * B = createBMatrix(Sh, i, n, k);
    matrix_t * Sproj = proj(B, MOD);
    matrix_t ** G = createGMatricesRecovery(Sh, i, n, k);
    matrix_t * R = createRMatrixRecovery(G, lefty, n, k);
    secret[i] = sum(Sproj, R, MOD);
    watermark[i] = sum(Sproj, Rw, MOD);

    delete(Rw);
    delete(B);
    delete(Sproj);
    delete(R);
    deleteMatrices(k, G);
  }
  free(rwMatrices);

  for (int i = 0; i < n; i++) {
    deleteMatrices(secretCount, Sh[i]);
  }
  free(Sh);

  matrix_t * watermarkMatrix = joinMatrices(watermark, watermarkCount, rows, columns);
  matrix_t * secretMatrix = joinMatrices(secret, secretCount, rows, columns);

  delete(rw_image->matrix);
  rw_image->matrix = secretMatrix;
  write_bmp(image, rw_image);
  rw_image->matrix = watermarkMatrix;
  write_bmp(WATERMARK_FILE, rw_image);

  delete(secretMatrix);
  free_bmp(rw_image);

  return true;
}

matrix_t *** splitShares(matrix_t ** shares, uint8_t n, uint8_t k, size_t* secretCount)
{
  matrix_t *** matrices = calloc(n, sizeof(matrix_t**));
  size_t t = (size_t) ceil(((double)n)/k) + 1;

  for (uint8_t i = 0; i < n; i++)
  {
    if(shares[i] != NULL)
    {
      matrices[i] = getNxMMatrices(shares[i], n, t, secretCount);
    }
  }
  return matrices;
}

matrix_t * createBMatrix(matrix_t *** Sh, size_t secretIndex, uint8_t n, uint8_t k)
{
  matrix_t * B = NULL;
  matrix_t * aux = NULL;
  uint8_t shares = 0;
  for (uint8_t i = 0; i < n && shares < k; i++)
  {
    if (Sh[i] != NULL)
    {
      shares++;
      matrix_t * v = getColumn(Sh[i][secretIndex], 0);
      if (B != NULL)
      {
        aux = augment(B, v);
        delete(B);
        delete(v);
        B = aux;
      }
      else
      {
        B = v;
      }
    }
  }

  return B;
}

matrix_t ** createGMatricesRecovery(matrix_t *** Sh, size_t secretIndex, uint8_t n, uint8_t k)
{
  matrix_t ** matrices = calloc(k, sizeof(matrix_t*));
  uint8_t shares = 0;
  for (uint8_t i = 0; i < n && shares < k; i++)
  {
    if (Sh[i] != NULL)
    {
      matrices[shares] = reduce(Sh[i][secretIndex], 0, Sh[i][secretIndex]->rows, 1, Sh[i][secretIndex]->columns);
      shares++;
    }
  }
  return matrices;
}

matrix_t * createRMatrixRecovery(matrix_t ** G, matrix_t * lefty, uint8_t n, uint8_t k)
{
  matrix_t * R = NULL;
  size_t columns = G[0]->columns;
  matrix_t ** results = calloc(n * columns, sizeof(matrix_t*));
  size_t idx = 0;
  for (uint8_t i = 0; i < n; i++)
  {
    for (uint8_t j = 0; j < columns; j++)
    {
      matrix_t * righty = createRightyMatrix(G, k, i, j);
      matrix_t * gaussy = augment(lefty, righty);
      matrix_t * result = gaussElimination(gaussy, MOD);
      results[idx++] = getColumn(result, result->columns - 1);
      delete(righty);
      delete(gaussy);
      delete(result);
    }
  }

  R = joinMatrices(results, n * columns, n, n);
  deleteMatrices(n * columns, results);

  return R;
}

matrix_t * createLeftyMatrix(matrix_t *** Sh, uint8_t n, uint8_t k)
{
  matrix_t * lefty = create(k, k);
  uint8_t j = 0;
  for (uint8_t i = 0; i < n && j < k; i++)
  {
    if(Sh[i] != NULL)
    {
      for (uint8_t l = 0; l < k; l++)
      {
        lefty->data[j][l] = (uint8_t) ((uint64_t) pow(i + 1, l) % MOD);
      }
      j++;
    }
  }
  return lefty;
}

matrix_t * createRightyMatrix(matrix_t ** G, uint8_t k, size_t row, size_t column)
{
  matrix_t * righty = create(k, 1);
  for (uint8_t i = 0; i < k; i++)
  {
    righty->data[i][0] = G[i]->data[row][column];
  }
  return righty;
}