#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>
#include "hmap.h"

int *new_int(int x)
{
	int *p;
	p = malloc(sizeof(*p));
	*p = x;
	return p;
}

int cmp_val(const void *a, const void *b)
{
	int x, y;
	x = *(int *) a;
	y = *(int *) b;
	if (x > y)
		return 1;
	else if (x < y)
		return -1;
	return 0;
}

int cmp_key(const void *a, const void *b)
{
	return strcmp(a, b);
}

char *str_to_lower(const char *s)
{
	char *l;
	int i;
	l = malloc(sizeof(*l) * (strlen(s) + 1));
	i = 0;
	while (l[i++] = tolower(*s++)) ;
	return l;
}

char *read_word(FILE *fp)
{
	char buf[1024];
	int i, c;
	i = 0;
	while (isspace(c = getc(fp))) ;

	while (c != EOF && !isspace(c) && i < 1023) {
		buf[i++] = c;
		c = getc(fp);
	}
	if (c == EOF && i == 0)
		return NULL;
	buf[i] = '\0';
	return strdup(buf);
}

uint32_t hash_key(const void *key)
{
	const char *str = key;
	uint64_t hash = 5381;
	while(*str) {
		hash = ((hash << 5) + hash) + *str;
		str++;
	}
	return hash;
}

struct hmap *construct_hmap(const char *file)
{
	FILE *fp;
	struct hmap *hmap;
	struct hmap_entry *ret;
	char *word, *lword;
	hmap = hmap_new(hash_key, cmp_key, free, free);
	if (!(fp = fopen(file, "r"))) {
		perror("fopen()");
		exit(EXIT_FAILURE);
	}
	while (word = read_word(fp)) {
		lword = str_to_lower(word);
		free(word);
		word = lword;
		if (ret = hmap_get(hmap, word)) {
			free(word);
			(*(int *) ret->v)++;
		} else {
			hmap_put(hmap, word, new_int(1));
		}
	}
	if (fclose(fp)) {
		perror("fclose()");
		exit(EXIT_FAILURE);
	}
	return hmap;
}

void do_querying(struct hmap *hmap)
{
	char *word, *lword;
	struct hmap_entry *ret;
	while (word = read_word(stdin)) {
		lword = str_to_lower(word);
		free(word);
		word = lword;
		if (ret = hmap_get(hmap, word)) {
			printf("\"%s\" appeared %d times\n",
				word, *(int *)ret->v);
		} else {
			printf("\"%s\" unseen\n", word);
		}
		free(word);
	}
}

int main(int argc, char *argv[])
{
	struct hmap *hmap;
	if (argc != 2) {
		fprintf(stderr, "usage: ./%s file\n", argv[0]);
		exit(EXIT_FAILURE);
	}
	hmap = construct_hmap(argv[1]);
	do_querying(hmap);
	hmap_free_all(hmap);
	free(hmap);
	return EXIT_SUCCESS;
}
