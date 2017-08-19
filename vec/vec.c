#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include "vec.h"


void vec_init(struct vec *vec, int (*cmp)(const void *, const void *),
			void (*free)(void *))
{
	vec->size = 0;
	vec->max_size = VEC_INIT_MAX_SIZE;
	vec->cmp = cmp;
	vec->free = free;
	if (!(vec->arr = malloc(sizeof(vec->arr[0]) * vec->max_size)))
		fprintf(stderr, "vec_init() malloc() FAILURE\n");
}

struct vec *vec_new(int (*cmp)(const void *, const void *),
			void (*free)(void *))
{
	struct vec *vec;
	if (!(vec = malloc(sizeof(*vec))))
		fprintf(stderr, "vec_new() malloc() FAILURE\n");
	vec_init(vec, cmp, free);
	return vec;
}


static void vec_free_has_elems(struct vec *vec);

void vec_free(struct vec *vec, int flags)
{
	if (flags & VEC_FREE_ELEMS)
		vec_free_has_elems(vec);
	free(vec->arr);
	if (flags & VEC_FREE_PTR)
		free(vec);
}

static void vec_free_has_elems(struct vec *vec)
{
	int i;
	for (i=0; i<vec->size; i++)
		vec->free(vec->arr[i]);
}

int vec_index_of(const struct vec *vec, const void *v)
{
	int i;
	for (i=0; i<vec->size; i++)
		if (!vec->cmp(vec->arr[i], v))
			return i;
	return -1;
}

bool vec_contains(const struct vec *vec, const void *v)
{
	return vec_index_of(vec, v) != -1;
}


static void vec_resize(struct vec *vec, int new_max);

void vec_add(struct vec *vec, void *v)
{
	if (vec->size == vec->max_size)
		vec_resize(vec, vec->max_size * VEC_GROWTH_FACTOR);
	vec->arr[vec->size++] = v;
}

void vec_add_i(struct vec *vec, void *v, int idx)
{
	int i;
	if (vec->size == vec->max_size)
		vec_resize(vec, vec->max_size * VEC_GROWTH_FACTOR);
	for (i=vec->size++; i>idx; i--)
		vec->arr[i] = vec->arr[i-1];
	vec->arr[i] = v;
}

void *vec_rm(struct vec *vec, int idx)
{
	int i;
	void *ret;
	ret = vec->arr[idx];
	vec->size--;
	for (i=idx; i<vec->size; i++)
		vec->arr[i] = vec->arr[i+1];
	if (vec->max_size / VEC_TRUNCATE_FACTOR >= VEC_INIT_MAX_SIZE &&
		vec->size < vec->max_size / VEC_TRUNCATE_THRESHOLD)
		vec_resize(vec, vec->max_size / VEC_TRUNCATE_FACTOR);
	return ret;
}

void *vec_get(struct vec *vec, int idx)
{
	return vec->arr[idx];
}


static void vec_resize(struct vec *vec, int new_max)
{
	if (!(vec->arr = realloc(vec->arr, sizeof(vec->arr[0]) * new_max)))
		fprintf(stderr, "vec_resize() realloc() FAILURE\n");
	vec->max_size = new_max;
}
