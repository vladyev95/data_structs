#include <string.h>
#include <stdint.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include "nlp.h"
#include "utils.h"


int *new_int(int x)
{
	int *p;
	p = malloc(sizeof(*p));
	*p = x;
	return p;
}


int cmp_str(const void *a, const void *b)
{
	return strcmp(a, b);
}

size_t read_lword(char *buf, FILE *fp)
{
	size_t i, c;
	i = 0;
	while (isspace(c = fgetc(fp)) || ispunct(c)) ;

	while (c != EOF && !isspace(c) && i < 1023) {
		buf[i++] = tolower(c);
		c = getc(fp);
	}
	if (c == EOF && i == 0)
		return 0;
	buf[i] = '\0';
	return i;
}

uint32_t hash_str(const void *str)
{
	const char *s = str;
	uint64_t hash = 5381;
	while(*s) {
		hash = ((hash << 5) + hash) + *s;
		s++;
	}
	return hash;
}

void print_str(const void *s)
{
	printf("%s\n", (char *) s);
}
