#include "random.h"

int64_t
setSeed(int64_t s)
{
    return (s ^ 0x5DEECE66DL) & ((1LL << 48) - 1);
}

uint8_t
nextChar(int64_t* seed)
{
    *seed = (*seed * 0x5DEECE66DL + 0xBL) & ((1LL << 48) - 1);
    return (uint8_t)(*seed>>40);
} 