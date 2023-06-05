#include<stdio.h>
int display();

#pragma startup display
#pragma exit display

int main() {
   printf("\nI am in main function\n");
   return 0;
}

int display() {
   printf("\nI am in display function\n");
   return 0;
}
