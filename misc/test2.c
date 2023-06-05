// C program to illustrate _builtin_popcount(x)
 
#include <stdio.h>
int main()
{
    int n = 5;
 
    printf("\nCount of 1s in binary of %d is %d \n", n,
           __builtin_popcount(n));
    return 0;
}
