#ifndef VEC_H
#define VEC_H

#include <stdlib.h>
#include <stdbool.h>


#define VEC_INIT_N_SIZE 64

#define VEC_GROWTH_FACTOR 4


struct vec {
	void **arr;
	size_t size, max_size;
	int (*cmp)(const void *v1, const void *v2);
	void (*free_v)(void *v);
};


void vec_init(struct vec *vec, int (*cmp)(const void *v1, const void *v2),
			void (*free_v)(void *v));

struct vec *vec_new(int (*cmp)(const void *v1, const void *v2),
			void (*free_v)(void *v));

void vec_free(struct vec *vec);

void vec_free_all(struct vec *vec);

bool vec_contains(const struct vec *vec, const void *v);

void vec_add(struct vec *vec, void *v);

void vec_add_i(struct vec *vec, void *val, size_t index);

void *vec_rm_i(struct vec *vec, size_t index);

void *vec_get(struct vec *vec, size_t index);

#endif