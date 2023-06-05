#include<stdio.h>
int main() 
{ 
	char array[] = "DataFlair"; 
	char *pointer = array; 
	char ch;

	while(*pointer != '\0') 
	{
		++*pointer++; 
		
		/*
		 // Equivalent to above is below 4 statements;
		 ch = *pointer;
		 ++ch;
		 *pointer = ch;
		 pointer++;
		*/
	
		//++*pointer--; 
	}

	printf("%s\n",array); 
	
	return 0; 
}
