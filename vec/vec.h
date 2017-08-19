#ifndef VEC_H
#define VEC_H

#include <stdbool.h>

#define VEC_INIT_MAX_SIZE 64

#define VEC_GROWTH_FACTOR 4

#define VEC_TRUNCATE_THRESHOLD 16

#define VEC_TRUNCATE_FACTOR 2


struct vec {
	void **arr;
	int size, max_size;
	int (*cmp)(const void *, const void *);
	void (*free)(void *);
};


void vec_init(struct vec *vec, int (*cmp)(const void *, const void *),
			void (*free)(void *));

struct vec *vec_new(int (*cmp)(const void *, const void *),
			void (*free)(void *));


#define VEC_FREE_PTR (1)
#define VEC_FREE_ELEMS (1<<1)
void vec_free(struct vec *vec, int flags);

bool vec_contains(const struct vec *vec, const void *v);

int vec_index_of(const struct vec *vec, const void *v);

void vec_add(struct vec *vec, void *v);

void vec_add_i(struct vec *vec, void *v, int idx);

void *vec_rm(struct vec *vec, int idx);

void *vec_get(struct vec *vec, int idx);

#endif
