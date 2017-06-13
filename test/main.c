#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>
#include "ll.h"
#include "nlp.h"
#include "hmap.h"
#include "vec.h"
#include "utils.h"

struct ll *read_tokens(const char *file, int *uniq)
{
	FILE *fp;
	char buf[1024], *word;
	size_t len;
	struct ll *tokens;
	struct hmap seen;
	struct hmap_entry *ret;
	hmap_init(&seen, hash_str, cmp_str, NULL, NULL);
	tokens = ll_new(NULL, free);
	if (!(fp = fopen(file, "r"))) {
		perror("fopen()");
		exit(EXIT_FAILURE);
	}
	ll_add_head(tokens, strdup(START_SYM));
	*uniq = 2;
	while ( (len = read_lword(buf, fp)) ) {
		word = strdup(buf);
		ll_add_tail(tokens, word);
		if (word[len-1] == '.') {
			ll_add_tail(tokens, strdup(END_SYM));
			ll_add_tail(tokens, strdup(START_SYM));
		}
		if (!(ret = hmap_get(&seen, word))) {
			(*uniq)++;
			hmap_put(&seen, word, word);
		}
	}
	if (!strcmp(tokens->tail->v, START_SYM));
		ll_rm_tail(tokens);

	if (strcmp(tokens->tail->v, END_SYM))
		ll_add_tail(tokens, strdup(END_SYM));

	if (fclose(fp)) {
		perror("fclose()");
		exit(EXIT_FAILURE);
	}
	hmap_free(&seen);
	return tokens;
}

struct hmap *make_unifreqs(struct ll *tokens, int uniq)
{
	struct hmap *freqs;
	struct ll_node *walk;
	struct hmap_entry *ret;
	freqs = hmap_new(hash_str, cmp_str, NULL, free);
	for (walk = tokens->head; walk; walk = walk->next) {
		if (ret = hmap_get(freqs, walk->v))
			(*(int *) ret->v)++;
		else
			hmap_put(freqs, walk->v, new_int(1));
	}
	return freqs;
}

int main(int argc, char *argv[])
{
	int uniq;
	struct hmap *unifreqs;
	struct ll *tokens;
	if (argc != 2) {
		fprintf(stderr, "usage: ./%s file\n", argv[0]);
		exit(EXIT_FAILURE);
	}
	tokens = read_tokens(argv[1], &uniq);
	unifreqs = make_unifreqs(tokens, uniq);
	ll_free_all(tokens);
	hmap_free_v(unifreqs);
	return EXIT_SUCCESS;
}
