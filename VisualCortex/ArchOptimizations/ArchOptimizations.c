#include "ArchOptimizations.h"

#include <stdio.h>
#include <string.h>


/*
   THIS IS AN ASSEMBLY PLACE HOLDER FOR LOW LEVEL OPTIMIZATIONS
   PROBABLY CONCERNING INTEGRAL IMAGES , CONVOLUTION FILTERS , MEMCPY
   This subdir of VisualCortex serves this purpose..
*/

void * memcpy_ext(void *  b, const void *  a, size_t n)
{
    char *s1 = b;
    const char *s2 = a;
    for(; 0<n; --n)*s1++ = *s2++;
    return b;
}


int testArchOptimizations(int add_A,int add_B)
{
        printf("%u+%u",add_A,add_B);
        __asm__ __volatile__("addl  %%ebx,%%eax"
                             :"=a"(add_A)
                             :"a"(add_A), "b"(add_B)
                             );
        printf("=%d\n",add_A);

    return add_A;
}
