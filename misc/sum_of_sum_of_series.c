#include<stdio.h>

int main()
{
	printf("Welcome to DataFlair tutorials!\n\n");
	int n, sum1, sum2 = 0, i, j;
	printf("Enter the number of terms in the series n: ");
	scanf("%d",&n);
	for(i = 1; i <= n; i++)
	{
		sum1 = 0;
		for(j = 1; j <= i ; j++)
			sum1 = sum1 + j;

		sum2 = sum2 + sum1;
	}
	printf("The sum of the series is: %d\n",sum2);

	return 0;
}
