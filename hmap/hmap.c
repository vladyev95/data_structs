#include <stdlib.h>
#include <stdio.h>
#include "hmap.h"

/*
 * initializes hmap with given arguments and other necessary data
 */
void hmap_init(struct hmap *hmap, 
		unsigned (*hash)(const void *),
		int (*cmp)(const void *, const void *), 
		void (*free_k)(void *), 
		void (*free_v)(void *))
{
	hmap->hash = hash;
	hmap->cmp = cmp;
	hmap->free_k = free_k;
	hmap->free_v = free_v;
	hmap->n_bkts = HMAP_INIT_N_BKTS;
	hmap->size = 0;
	hmap->bkts = calloc(hmap->n_bkts,sizeof(hmap->bkts[0]));
	if (!(hmap->bkts))
		fprintf(stderr, "hmap_init() calloc() FAILURE\n");
}

/*
 * returns a ptr to a dynamically allocated initialized hmap
 */
struct hmap *hmap_new(unsigned (*hash)(const void *), 
		int (*cmp)(const void *, const void *), 
		void (*free_k)(void *), 
		void (*free_v)(void *))
{
	struct hmap *hmap;
	if (!(hmap = malloc(sizeof(*hmap))))
		fprintf(stderr, "hmap_new() malloc() FAILURE\n");
	hmap_init(hmap, hash, cmp, free_k, free_v);
	return hmap;
}

/*
 * frees memory behind the hmap and the keys and values in the hmap
 * uses hmap->free_k and hmap->free_v
 */
void hmap_free(struct hmap *hmap, int flags)
{
	int i, free_ks, free_vs;
	struct hmap_entry *walk, *next;
	free_ks = flags & HMAP_FREE_KEYS;
	free_vs = flags & HMAP_FREE_VALS;
	for (i=0; i<hmap->n_bkts; i++) {
		walk = hmap->bkts[i];
		while (walk) {
			next = walk->next;
			if (free_ks)
			    hmap->free_k(walk->k);
			if (free_vs)
			    hmap->free_v(walk->v);
			free(walk);
			walk = next;
		}
	}
	free(hmap->bkts);
	if (flags & HMAP_FREE_PTR)
	    free(hmap);
}

static void hmap_resize(struct hmap *hmap, int new_n_bkts);

void hmap_put(struct hmap *hmap, void *k, void *v, void **orig_v)
{
	unsigned bucket;
	struct hmap_entry *entry;
	bucket = hmap->hash(k) % hmap->n_bkts;
	for (entry = hmap->bkts[bucket]; entry; entry=entry->next) {
		if (!hmap->cmp(k, entry->k)) {
			if (orig_v)
				*orig_v = entry->v;
			entry->v = v;
			return;
		}

	}
	if (!(entry = malloc(sizeof(*entry))))
		fprintf(stderr, "hmap malloc() failure");
	entry->k = k;
	entry->v = v;
	entry->next = hmap->bkts[bucket];
	hmap->bkts[bucket] = entry;
	hmap->size++;
	if (orig_v)
		*orig_v = NULL;
	if (HMAP_SHOULD_ENLARGE(hmap))
		hmap_resize(hmap, HMAP_ENLARGE_SIZE(hmap));
}

/*
 * returns pointer to hmap_entry containing key k
 * key and value are accessed by entry->k & entry->v
 * returns NULL if the key is not found
 */
void *hmap_get(struct hmap *hmap, const void *k)
{
	struct hmap_entry *walk;
	walk = hmap->bkts[hmap->hash(k) % hmap->n_bkts];
	while (walk) {
		if (!hmap->cmp(k, walk->k))
			return walk->v;
		walk = walk->next;
	}
	return NULL;
}

void *hmap_rm(struct hmap *hmap, const void *k, void **orig_k)
{
	unsigned bucket;
	struct hmap_entry *walk, *tofree;
	void *v;

	bucket = hmap->hash(k) % hmap->n_bkts;
	walk = hmap->bkts[bucket];
	if (!walk) {
		if (orig_k)
			*orig_k = NULL;
		return NULL;
	}
	if (!hmap->cmp(k, walk->k)) {
		hmap->bkts[bucket] = walk->next;
		hmap->size--;
		v = walk->v;
		if (orig_k)
			*orig_k = walk->k;
		free(walk);

		if (HMAP_CAN_TRUNCATE(hmap) && HMAP_SHOULD_TRUNCATE(hmap))
			hmap_resize(hmap, HMAP_TRUNCATE_SIZE(hmap));
		return v;
	}
	while (walk->next) {
		if (!hmap->cmp(k, walk->next->k)) {
			tofree = walk->next;
			v = tofree->v;
			if (orig_k)
				*orig_k = tofree->k;
			walk->next = walk->next->next;
			hmap->size--;
			free(tofree);
			if (HMAP_CAN_TRUNCATE(hmap) && 
				HMAP_SHOULD_TRUNCATE(hmap))
				hmap_resize(hmap, HMAP_TRUNCATE_SIZE(hmap));
			return v;
		}
		walk = walk->next;
	}
	if (orig_k)
		*orig_k = NULL;
	return NULL;
}



static void hmap_rehash(struct hmap *hmap, struct hmap_entry **new_bkts,
		int new_n_bkts);

/*
 * resizes the number of buckets to new_max
 */
static void hmap_resize(struct hmap *hmap, int new_n_bkts)
{
	struct hmap_entry **new_bkts;
	if (!(new_bkts = calloc(new_n_bkts, sizeof(new_bkts[0]))))
		fprintf(stderr, "hmap_resize() calloc() FAILURE\n");
	hmap_rehash(hmap, new_bkts, new_n_bkts);
	free(hmap->bkts);
	hmap->n_bkts = new_n_bkts;
	hmap->bkts = new_bkts;
}

static void hmap_rehash(struct hmap *hmap, struct hmap_entry **new_bkts,
		int new_n_bkts)
{
	int i, bkt;
	struct hmap_entry *walk, *next;
	for (i=0; i < hmap->n_bkts; i++) {
		for (walk = hmap->bkts[i]; walk; 
				next = walk->next, walk = next) {
			bkt = hmap->hash(walk->k) % new_n_bkts;
			walk->next = new_bkts[bkt];
			new_bkts[bkt] = walk;
		}
	}
}


void hmap_for_each(struct hmap *hmap, void (*for_k)(void *),
			void (*for_v)(void *))
{
	int i;
	struct hmap_entry *walk;

	for (i=0; i<hmap->n_bkts; i++) {
		for (walk=hmap->bkts[i]; walk; walk = walk->next) {
			if (for_k)
				for_k(walk->k);
			if (for_v)
				for_v(walk->v);
		}
	}
}
