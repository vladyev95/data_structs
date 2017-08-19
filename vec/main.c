#include <stdlib.h>
#include <stdio.h>
#include "vec.h"


int *malloc_int(int x)
{
	int *p;
	if (!(p = malloc(sizeof(*p))))
		perror("malloc_int() malloc()");
	*p = x;
	return p;
}
int main(void)
{
	int i;
	struct vec v;
	vec_init(&v, NULL, free);

	for (i=0; i<10000; i++)
		vec_add(&v, malloc_int(i));
	
	for (i=9999; i>=0; i--)
		printf("%d ", *(int*) vec_rm(&v, i));


	vec_free(&v, VEC_FREE_ELEMS);
	return 0;
}
