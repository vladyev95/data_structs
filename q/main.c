#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <alloca.h>
#include "q.h"

#define LOW 0
#define HIGH 10000000

int *malloc_int(int x)
{
	int *p;
	p = malloc(sizeof(*p));
	*p = x;
	return p;
}


int main(void)
{
	int i;
	struct q q;
	q_init(&q, free);

	for (i=0; i<100000; i++)
		q_enq(&q, malloc_int(i));
	
	printf("%d\n", *(int*)q_peek(&q));
	
	for (i=0; i<100000; i++)
		q_deq(&q);
	
	q_free(&q, Q_FREE_ELEMS);
	return 0;
}
