#include <stdlib.h>
#include <stdio.h>
#include "hmap.h"

static void hmap_rehash(struct hmap *hmap);

/*
 * initializes hmap with given arguments and other necessary data
 */
void hmap_init(struct hmap *hmap, 
		unsigned (*hash_k)(const void *k), 
		int (*cmp_k)(const void *k1, const void *k2), 
		void (*free_k)(void *k), 
		void (*free_v)(void *v))
{
	hmap->hash_k = hash_k;
	hmap->cmp_k = cmp_k;
	hmap->free_k = free_k;
	hmap->free_v = free_v;
	hmap->n_buckets = HMAP_INIT_N_BUCKETS;
	hmap->size = 0;
	if (!(hmap->buckets = calloc(hmap->n_buckets,
					sizeof(hmap->buckets[0])))) {
		fprintf(stderr, "hmap calloc() failure");
	}
}

/*
 * returns a ptr to a dynamically allocated initialized hmap
 */
struct hmap *hmap_new(unsigned (*hash_k)(const void *k), 
		int (*cmp_k)(const void *k1, const void *k2), 
		void (*free_k)(void *k), 
		void (*free_v)(void *v))
{
	struct hmap *hmap;
	if (!(hmap = malloc(sizeof(*hmap))))
		fprintf(stderr, "hmap malloc() failure");
	hmap_init(hmap, hash_k, cmp_k, free_k, free_v);
	return hmap;
}

/*
 * frees memory behind hmap but neither keys or values
 */
void hmap_free(struct hmap *hmap)
{
	int i;
	struct hmap_entry *walk, *next;
	for (i=0; i<hmap->n_buckets; i++) {
		walk = hmap->buckets[i];
		while (walk) {
			next = walk->next;
			free(walk);
			walk = next;
		}
	}
	free(hmap->buckets);
}

/*
 * frees memory behind hmap and the keys in the hmap
 * uses hmap->free_k to free keys
 */
void hmap_free_k(struct hmap *hmap)
{
	int i;
	struct hmap_entry *walk, *next;
	for (i=0; i<hmap->n_buckets; i++) {
		walk = hmap->buckets[i];
		while (walk) {
			next = walk;
			hmap->free_k(walk->k);
			free(walk);
			walk = next;
		}
	}
	free(hmap->buckets);
}

/*
 * frees memory behind hmap and the values in the hmap
 * uses hmap->free_v to free values
 */
void hmap_free_v(struct hmap *hmap)
{
	int i;
	struct hmap_entry *walk, *next;
	for (i=0; i<hmap->n_buckets; i++) {
		walk = hmap->buckets[i];
		while (walk) {
			next = walk->next;
			hmap->free_v(walk->v);
			free(walk);
			walk = next;
		}
	}
	free(hmap->buckets);
}

/*
 * frees memory behind the hmap and the keys and values in the hmap
 * uses hmap->free_k and hmap->free_v
 */
void hmap_free_all(struct hmap *hmap)
{
	int i;
	struct hmap_entry *walk, *next;
	for (i=0; i<hmap->n_buckets; i++) {
		walk = hmap->buckets[i];
		while (walk) {
			next = walk->next;
			hmap->free_k(walk->k);
			hmap->free_v(walk->v);
			free(walk);
			walk = next;
		}
	}
	free(hmap->buckets);
}

/*
 * puts key value pair k->v into hmap
 * uses hmap->hash_k
 */
void hmap_put(struct hmap *hmap, void *k, void *v)
{
	unsigned bucket;
	struct hmap_entry *entry;
	bucket = hmap->hash_k(k) % hmap->n_buckets;
	if (!(entry = malloc(sizeof(*entry))))
		fprintf(stderr, "hmap malloc() failure");
	entry->k = k;
	entry->v = v;
	entry->next = hmap->buckets[bucket];
	hmap->buckets[bucket] = entry;
	hmap->size++;
	if ((float) hmap->size / hmap->n_buckets > HMAP_LOAD_FACTOR)
		hmap_rehash(hmap);
}

/*
 * returns pointer to hmap_entry containing key k
 * key and value are accessed by entry->k & entry->v
 * returns NULL if the key is not found
 */
struct hmap_entry *hmap_get(struct hmap *hmap, const void *k)
{
	struct hmap_entry *walk;
	walk = hmap->buckets[hmap->hash_k(k) % hmap->n_buckets];
	while (walk) {
		if (!hmap->cmp_k(k, walk->k))
			return walk;
		walk = walk->next;
	}
	return NULL;
}

/*
 * returns a pointer to a hmap_entry containing key k
 * the hmap_entry has been removed from the hmap
 * key and value can be accessed by entry->k & entry->v
 * returns NULL if the key is not found
 */
struct hmap_entry *hmap_rm(struct hmap *hmap, const void *k)
{
	unsigned bucket;
	struct hmap_entry *walk, *ret;
	bucket = hmap->hash_k(k) % hmap->n_buckets;
	walk = hmap->buckets[bucket];
	if (!walk)
		return NULL;
	if (!hmap->cmp_k(k, walk->k)) {
		hmap->buckets[bucket] = walk->next;
		hmap->size--;
		return walk;
	}
	while (walk->next) {
		if (!hmap->cmp_k(k, walk->next->k)) {
			ret = walk->next;
			walk->next = walk->next->next;
			hmap->size--;
			return ret;
		}
		walk = walk->next;
	}
	return NULL;
}

static void hmap_rehash(struct hmap *hmap)
{
	int i;
	unsigned new_n_buckets, bucket;
	struct hmap_entry **new_buckets, *walk, *next;
	new_n_buckets = hmap->n_buckets * HMAP_GROWTH_FACTOR;
	if (!(new_buckets = calloc(new_n_buckets, sizeof(new_buckets[0]))))
		fprintf(stderr, "hmap calloc() failure");
	/*
	 * rehash each hashmap_entry individually into new buckets
	 */
	for (i=0; i < hmap->n_buckets; i++) {
		for (walk = hmap->buckets[i]; walk; 
				next = walk->next, walk = next) {
			bucket = hmap->hash_k(walk->k) % new_n_buckets;
			walk->next = new_buckets[bucket];
			new_buckets[bucket] = walk;
		}
	}
	free(hmap->buckets);
	hmap->n_buckets = new_n_buckets;
	hmap->buckets = new_buckets;
}
