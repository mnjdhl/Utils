/* rshift_negval.c -- What happens on right arithmetic shift on -ve int */
#include <stdio.h>
int main(void)
{
    int x = -1000, y, nbits;
 
    printf("By what no of bits u wanna right shift on signed integer, "
           "enter no of bits...\n");
    puts("Non integer value terminates the program.");
 
    while (scanf("%d", &nbits) == 1) {
        y = x >> nbits;
        printf("The value of x in y = x >> %d is %d and y is %d\n", nbits, x, y);
        y = x << nbits;
        printf("The value of x in y = x << %d is %d and y is %d\n", nbits, x, y);
    }
    printf("Program Terminated! Good Bye!\n");
    return 0;
}
