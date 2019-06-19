#ifndef RANDOM_LIBRARY_H
#define RANDOM_LIBRARY_H

#include <stdint.h> 

int64_t setSeed(int64_t s);

uint8_t nextChar(int64_t* seed);

#endif
