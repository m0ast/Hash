#include <stdio.h>
#include <stdlib.h>
typedef struct S S;

struct S
{
	const S *himself;
	int data;
};

S newS(int data)
{
	S *p;

	printf("<%p> <%p> <%d> \n", p, p->himself, p->data);
	p = malloc(sizeof *p);
	printf("<%p> <%p> <%d> \n", p, p->himself, p->data);
	*((S **)p) = p;
	p->data = data;
	printf("<%p> <%p> <%d> \n", p, p->himself, p->data);
	return (*p);
}

int main()
{
	int tab[3];
	void *ptr;
	S p;
	int o;

	ptr = &ptr;
	tab[0] = 5;
	p = newS(tab[0]);
	printf("<%p><%p><%p>\n", ptr, &tab ,&(tab[0]));
	printf("<%p>\n", p);
	ptr = (void *)o;
	printf("<%p><%d>\n", p.himself, p.data);
	return (0);
}
