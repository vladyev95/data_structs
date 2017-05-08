#include <string.h>
#include <stdint.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include "utils.h"

struct wstat *new_wstat(void)
{
	struct wstat *p;
	if (!(p = malloc(sizeof(*p)))) {
		perror("malloc()");
		exit(EXIT_FAILURE);
	}
	return p;
}

int cmp_str(const void *a, const void *b)
{
	return strcmp(a, b);
}

char *read_lword(char *buf, FILE *fp)
{
	int i, c;
	i = 0;
	while (isspace(c = fgetc(fp)) || ispunct(c)) ;

	while (c != EOF && !isspace(c) && (!ispunct(c) || c == '\'') && i < 1023) {
		buf[i++] = tolower(c);
		c = getc(fp);
	}
	if (c == EOF && i == 0)
		return NULL;
	buf[i] = '\0';
	return buf;
}

uint32_t hash_str(const void *key)
{
	const char *str = key;
	uint64_t hash = 5381;
	while(*str) {
		hash = ((hash << 5) + hash) + *str;
		str++;
	}
	return hash;
}
