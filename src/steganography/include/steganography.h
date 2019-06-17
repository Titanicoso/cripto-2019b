#ifndef STEGANOGRAPHY_LIBRARY_H
#define STEGANOGRAPHY_LIBRARY_H

#include <dirent.h> 
#include <string.h>
#include "bmp.h"

void distribute_shares(const matrix_t ** Sh, const char * dir, uint8_t k, uint8_t n);
void distribute_bits(const matrix_t * Sh, BITMAP * bitmap, uint8_t bits, uint8_t mask);
matrix_t ** recover_shares(const char * dir, uint8_t k, uint8_t n);

#endif