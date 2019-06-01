#include <stdio.h>
#include <limits.h>
#include <stdlib.h>
#include <stdint.h>
#include "random.h"

#define SET 10 
#define MAX 50 
/*variable global*/
int64_t seed; /*seed debe ser de 48 bits; se elige este tipo de 64 bits*/ 

int 
main(void)
{

    int i;
    uint8_t num;
    setSeed(SET);
    for (i = 0; i < MAX;i++)
    {
        num = nextChar(seed);
        printf("%d\t", num);
    }
    return EXIT_SUCCESS; 
    return 1;
}