#include <stdio.h>

int main()
{
	static int n = 60;

	if(n <= 80)
	{
		printf("%d ", n++);
		main();
	}

	return 0;
}
