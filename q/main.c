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
	int i, *ret;
	struct q q;
	srand(time(NULL));
	q_init(&q, free);
	for (i=LOW; i<=HIGH; i++) {
		if (rand() % 2) {
			q_enq(&q, malloc_int(i));
		} else {
			ret = q_deq(&q);
			free(ret);
		}
	}
	fprintf(stderr, "size: %u front: %u back: %u max_size: %u\n", q.size,
			q.front, q.back, q.max_size);
	q_free_v(&q);
	return 0;
}
