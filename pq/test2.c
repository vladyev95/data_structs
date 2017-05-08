#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <time.h>
#include <stdlib.h>
#include "pq.h"

int cmp_val(const void *a, const void *b)
{
	return -strcmp(a,b);
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

int main(void)
{
	char *word;
	struct pq pq;
	pq_init(&pq, cmp_val, free);
	while (word = read_word()) {
		pq_enq(&pq, word);
	}
	while (word = pq_deq(&pq)) {
		printf("%s\n",  word);
		free(word);
	} 
	pq_free(&pq);
	return 0;
}
