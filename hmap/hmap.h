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
	int n_buckets, size;
	unsigned (*hash)(const void *);
	int (*cmp)(const void *, const void *);
	void (*free_k)(void *);
	void (*free_v)(void *);
};

/*
 * initializes a struct hashmap
 * useful for automatically allocated hashmaps
 * free_key and free_val args may be NULL but hmap_free_..()
 * may not work if this is done
 */
void hmap_init(struct hmap *hmap, 
		unsigned (*hash)(const void *), 
		int (*cmp)(const void *, const void *), 
		void (*free_k)(void *), 
		void (*free_v)(void *));

/*
 * dynamically allocates a struct hashmap
 * initializes everything required
 * only free_key and free_value may be given as NULL
 * if the key-value pairs have automatic storage
 * hashmap_free_all may not be called if free_key or free_value are NULL
 */
struct hmap *hmap_new(unsigned (*hash)(const void *), 
		int (*cmp)(const void *, const void *), 
		void (*free_k)(void *), 
		void (*free_v)(void *));


/*
 * frees the struct hmap * given to the free function
 * useful for freeing of hmaps created by hmap_new()
 */
#define HMAP_FREE_PTR (1)

/*
 * frees the keys inside the hmap using the hmap->free_k() function
 */
#define HMAP_FREE_KEYS (1<<1)

/*
 * frees the values inside the hmap using the hmap->free_v() function
 */
#define HMAP_FREE_VALS (1<<2)

/*
 * frees the resources behind the hmap
 * whether keys/values/pointer are to be freed is specified in flags
 */
void hmap_free(struct hmap *hmap, int flags);


/*
 * hashes key using hashmap->hash_key and places it into hashmap
 * mapped to value
 * NULL keys or values are not suggested to be used
 * have to be careful if you do
 * if key already exists in hashmap, existing value is replaced
 * with the value argument and the old v ptr is put in *orig_v
 * if entry was not replaced, *orig_v is set to NULL
 * if orig_v is not NULL
 * otherwise NULL is returned
 */
void hmap_put(struct hmap *hmap, void *k, void *v, void **orig_v);

/*
 * returns the struct hashmap_entry containing the 
 * key-value pair corresponding to key from hashmap
 * hashmap_entry->key is the key
 * hashmap_entry->value is the value
 * returns NULL when the key-value is not present
 */
void *hmap_get(struct hmap *hmap, const void *k);

/*
 * returns the value corresponding to the given key k
 * makes *orig_k point to the original key used
 * orig_k may be given as NULL if the original key doesnt matter
 * removes the entry from the hashmap
 * returns NULL when the key-value is not present
 * sets *orig_k to NULL when the key-value is not present
 */
void *hmap_rm(struct hmap *hmap, const void *k, void **orig_k);


#define HMAP_PROCESS_KEYS (1)
#define HMAP_PROCESS_VALS (1<<1)
void hmap_for_each(struct hmap *hmap, void (*process_k)(void *k), 
			void (*process_v)(void *v), int flags);

#endif
