#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include "ll.h"

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

int cmp_str(const void *a, const void *b)
{
	return strcmp(a, b);
}
void printstr(const void *val)
{
	printf("%s -> ", (char *) val);
}

int main(void)
{
	char op, pt, *word, buf[1024];
	struct ll ll;
	ll_init(&ll, cmp_str, free);
	while (1) {
		if (scanf("%c %c", &op, &pt) != 2)
			break;
		if (op == 'a') {
			if (pt == 'h') {
				word = read_lword(buf, stdin);
				ll_add_head(&ll, strdup(word));
			} else if (pt == 't') {
				word = read_lword(buf, stdin);
				ll_add_tail(&ll, strdup(word));
			}
		} else if (op == 'r') {
			if (pt == 'h') {
				ll_rm_head(&ll);
			} else if (pt == 't') {
				ll_rm_tail(&ll);
			}
		}
		ll_print(&ll, printstr);
		printf("\n\n");
	}
	ll_free_all(&ll);
	return 0;
}
