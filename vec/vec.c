#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include "vec.h"

static void vec_resize(struct vec *vec);

void vec_init(struct vec *vec, int (*cmp)(const void *v1, const void *v2),
			void (*free_v)(void *v))
{
	vec->size = 0;
	vec->max_size = VEC_INIT_MAX_SIZE;
	vec->cmp = cmp;
	vec->free_v = free_v;
	if (!(vec->arr = malloc(sizeof(vec->arr[0]) * VEC_INIT_MAX_SIZE)))
		fprintf(stderr, "vec malloc() failure");
}

struct vec *vec_new(int (*cmp)(const void *v1, const void *v2),
			void (*free_v)(void *v))
{
	struct vec *vec;
	if (!(vec = malloc(sizeof(*vec))))
		fprintf(stderr, "vec malloc() failure");
	vec_init(vec, cmp, free_v);
	return vec;
}

void vec_free(struct vec *vec)
{
	free(vec->arr);
}

void vec_free_all(struct vec *vec)
{
	int i;
	for (i=0; i<vec->size; i++)
		vec->free_v(vec->arr[i]);
	free(vec->arr);
}

bool vec_contains(const struct vec *vec, const void *v)
{
	int i;
	for (i=0; i<vec->size && vec->cmp(vec->arr[i], v); i++) ;
	return i == vec->size;
}

void vec_add(struct vec *vec, void *v)
{
	if (vec->size == vec->max_size)
		vec_resize(vec);
	vec->arr[vec->size++] = v;
}

void vec_add_i(struct vec *vec, void *v, unsigned index)
{
	int i;
	if (vec->size == vec->max_size)
		vec_resize(vec);
	for (i=vec->size++; i>index; i--)
		vec->arr[i] = vec->arr[i-1];
	vec->arr[i] = v;
}

void *vec_rm_i(struct vec *vec, unsigned index)
{
	int i;
	void *ret;
	ret = vec->arr[index];
	vec->size--;
	for (i=index; i<vec->size; i++)
		vec->arr[i] = vec->arr[i+1];
	return ret;
}

void *vec_get(struct vec *vec, unsigned index)
{
	return vec->arr[index];
}

static void vec_resize(struct vec *vec)
{
	vec->max_size *= VEC_GROWTH_FACTOR;
	if (!(vec->arr = realloc(vec->arr, sizeof(vec->arr[0]) * 
						vec->max_size))) {
		fprintf(stderr, "vec_resize() failure");
	}
}
