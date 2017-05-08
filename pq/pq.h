#ifndef PQ_H
#define PQ_H

#include <stdlib.h>
#include <sys/types.h>

#define PQ_INIT_MAX_SIZE 64

#define PQ_GROWTH_FACTOR 4

struct pq {
	void **arr;
	size_t size, max_size;
	int (*cmp_v)(const void *v1, const void *v2);
	void (*free_v)(void *v);
};



void pq_init(struct pq *pq, int (*cmp_v)(const void *v1, const void *v2),
		void (*free_v)(void *v));

struct pq *pq_new(int (*cmp_v)(const void *v1, const void *v2),
		void (*free_v)(void *v));

void pq_free(struct pq *pq);
void pq_free_all(struct pq *pq);

void pq_enq(struct pq *pq, void *v);
void *pq_deq(struct pq *pq);
void *pq_peek(struct pq *pq);


#endif
