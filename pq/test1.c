#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <time.h>
#include <stdlib.h>
#include "pq.h"

float *new_float(float x)
{
	float *ptr;
	ptr = malloc(sizeof(*ptr));
	*ptr = x;
	return ptr;
}

int *new_int(int x)
{
	int *ptr;
	ptr = malloc(sizeof(*ptr));
	*ptr = x;
	return ptr;
}

int cmp_val(const void *a, const void *b)
{
	int x, y;
	x = *(int*)a;
	y = *(int*)b;
	if (x > y)
		return 1;
	else if (x < y)
		return -1;
	return 0;
}
char *read_word(void)
{
	char buf[1024];
	int i, c;
	i = 0;
	while (isspace(c = getchar())) ;

	while (c != EOF && !isspace(c) && i < 1023) {
		buf[i++] = c;
		c = getchar();
	}
	if (c == EOF && i == 0)
		return NULL;
	buf[i] = '\0';
	return strdup(buf);
}

void pq_print(struct pq *pq)
{
	int i;
	for (i=0;i<pq->size;i++)
		printf("%d ", *(int*)pq->arr[i]);
}

int main(void)
{
	int i, *ret;
	struct pq pq;
	pq_init(&pq, cmp_val, free);
	srand(time(NULL));
	for (i=0;i<15;i++) {
		pq_enq(&pq, new_int(rand() % 15));
	}
	while (ret = pq_deq(&pq)) {
		printf("%d\n", *(int *) ret);
		free(ret);
	} 
	pq_free(&pq);
	return 0;
}
