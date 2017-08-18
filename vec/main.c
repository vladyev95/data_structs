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

void free_vec(void *vec)
{
	vec_free(vec, VEC_FREE_PTR | VEC_FREE_VALS);
}


int main(void)
{
	int i, j, k;
	struct vec vec, *ret;
	vec_init(&vec, NULL, free_vec);
	for (i=0; i<100; i++)
		vec_add(&vec, vec_new(NULL, free_vec));
	for (i=0; i<100; i++) {
		ret = vec_get(&vec, 0);
		for (j=0; j<100; j++)
			vec_add(ret, vec_new(NULL, free));
	}
	for (i=0; i<100; i++)
		for (j=0; j<100; j++) {
			ret = vec_get(vec_get(&vec, 0), 0);
			for (k=0; k<100; k++)
				vec_add(ret, malloc_int(k));
		}
				
		
	
	vec_free(&vec, VEC_FREE_VALS);
	return 0;
}
