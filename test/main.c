#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>
#include "hmap.h"
#include "vec.h"
#include "utils.h"

struct vec *read_tokens(const char *file)
{
	FILE *fp;
	char buf[1024];
	struct vec *tokens;
	tokens = vec_new(NULL, free);
	if (!(fp = fopen(file, "r"))) {
		perror("fopen()");
		exit(EXIT_FAILURE);
	}
	while (read_lword(buf, fp))
		vec_add(tokens, strdup(buf));

	if (fclose(fp)) {
		perror("fclose()");
		exit(EXIT_FAILURE);
	}
	return tokens;
}

struct hmap *make_wstats(struct vec *tokens, int ntok)
{
	int i;
	struct hmap *wstats;
	struct hmap_entry *ent;
	struct wstat *ws;
	wstats = hmap_new(hash_str, cmp_str, free, free);
	for (i=0; i<tokens->size; i++) {
		if ( (ent = hmap_get(wstats, vec_get(tokens, i))) ) {
			ws = ent->v;
			ws->freq++;
		} else {
			ws = new_wstat();
			ws->freq = 1;
			hmap_put(wstats, vec_get(tokens, i), ws);
		}
	}
	for (i=0; i<wstats->n_buckets; i++) {
		ent = wstats->buckets[i];
		while (ent) {
			ws = ent->v;
			ws->perc = (float) ws->freq / ntok;
			ent = ent->next;
		}
	}
	return wstats;
}

void query(struct hmap *wstats)
{
	char buf[1024];
	struct hmap_entry *ent;
	struct wstat *ws;
	while (read_lword(buf, stdin)) {
		if ( (ent = hmap_get(wstats, buf)) ) {
			ws = ent->v;
			printf("%-20s %d %.3f%%\n",
				buf, ws->freq, 100 * ws->perc);
		} else {
			printf("\"%s\" unseen\n", buf);
		}
	}
}

int main(int argc, char *argv[])
{
	struct hmap *wstats;
	struct vec *tokens;
	if (argc != 2) {
		fprintf(stderr, "usage: ./%s file\n", argv[0]);
		exit(EXIT_FAILURE);
	}
	tokens = read_tokens(argv[1]);
	wstats = make_wstats(tokens, tokens->size);
	query(wstats);
	vec_free_all(tokens);
	hmap_free_v(wstats);
	free(tokens);
	free(wstats);
	return EXIT_SUCCESS;
}
