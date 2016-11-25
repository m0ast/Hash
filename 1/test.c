#include <stdio.h>

int main()
{
	int *a;
	int b;

	a = &b;
	printf("<%p>:<%p>\n", a, a + 10);
	return (0);
}
