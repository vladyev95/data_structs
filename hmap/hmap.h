#ifndef HMAP_H
#define HMAP_H


/*
 * num buckets/linked lists for values to be hashed into initially
 */
#define HMAP_INIT_N_BUCKETS 64

/*
 * when the number of key-value pairs / num_buckets reaches
 * more than LOAD_FACTOR, rehash is done to maintain put/get performance
 * rehashing takes O(n) time
 */
#define HMAP_LOAD_FACTOR .75f

/*
 * how much the num_buckets increases by when LOAD_FACTOR is reached
 * num_buckets increases to n_buckets * GROWTH_FACTOR
 */
#define HMAP_GROWTH_FACTOR 4

/*
 * key/value node for the linked list buckets of the hashmap
 */
struct hmap_entry {
	struct hmap_entry *next;
	void *k, *v;
};

struct hmap {
	struct hmap_entry **buckets;
	unsigned n_buckets, size;
	unsigned (*hash_k)(const void *k);
	int (*cmp_k)(const void *k1, const void *k2);
	void (*free_k)(void *k);
	void (*free_v)(void *v);
};

/*
 * initializes a struct hashmap
 * useful for automatically allocated hashmaps
 * free_key and free_val args may be NULL but hmap_free_..()
 * may not work if this is done
 */
void hmap_init(struct hmap *hmap, 
		unsigned (*hash_k)(const void *key), 
		int (*cmp_k)(const void *k1, const void *k2), 
		void (*free_k)(void *k), 
		void (*free_v)(void *v));

/*
 * dynamically allocates a struct hashmap
 * initializes everything required
 * only free_key and free_value may be given as NULL
 * if the key-value pairs have automatic storage
 * hashmap_free_all may not be called if free_key or free_value are NULL
 */
struct hmap *hmap_new(unsigned (*hash_k)(const void *k), 
		int (*cmp_k)(const void *k1, const void *k2), 
		void (*free_k)(void *k), 
		void (*free_v)(void *v));

/*
 * frees the resources behind the struct hashmap
 * does not free the struct hashmap pointer itself
 * only frees the internally used resources
 * does not free key-value pairs that are placed in the hashmap
 */
void hmap_free(struct hmap *hmap);

/*
 * frees hmap and keys only, not vals
 */
void hmap_free_k(struct hmap *hmap);

/*
 * frees hmap and vals only, not keys
 */
void hmap_free_v(struct hmap *hmap);


/*
 * frees both the resources behind the struct hashmap
 * and the key-value pairs that are placed in the hashmap
 * does not free the struct hmap pointer itself
 */
void hmap_free_all(struct hmap *hmap);

/*
 * hashes key using hashmap->hash_key and places it into hashmap
 * mapped to value
 * NULL values are not allowed for either key or value
 * if key already exists in hashmap, existing value is replaced
 * with the value argument and the old value is freed
 * otherwise NULL is returned
 */
void hmap_put(struct hmap *hmap, void *k, void *v);

/*
 * returns the struct hashmap_entry containing the 
 * key-value pair corresponding to key from hashmap
 * hashmap_entry->key is the key
 * hashmap_entry->value is the value
 * returns NULL when the key-value is not present
 */
struct hmap_entry *hmap_get(struct hmap *hmap, const void *k);

/*
 * returns the struct hashmap_entry containing the 
 * key-value pair corresponding to key from hashmap
 * removes the entry from the hashmap
 * hashmap_entry->key is the key
 * hashmap_entry->value is the value
 * returns NULL when the key-value is not present
 */
struct hmap_entry *hmap_rm(struct hmap *hmap, const void *k);


#endif
