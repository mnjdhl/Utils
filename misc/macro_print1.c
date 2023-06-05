#include <stdio.h> 

#define fgets "%s DataFlair" 
//If we don't define above, the printf will print garbage and also compiler will give warnings

int main() 
{
	printf(fgets, fgets);

	return 0; 
}
