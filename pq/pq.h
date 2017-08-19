#ifndef PQ_H
#define PQ_H

/*
 * initial capacity of a pq
 */
#define PQ_INIT_MAX_SIZE 64

/*
 * the factor at which the capacity of a pq will grow once
 * it has reached max capacity
 * pq->max_size *= PQ_GROWTH_FACTOR
 */
#define PQ_GROWTH_FACTOR 4

/*
 * the threshold at which a pq will be decreased in max size
 * done when pq->size < pq->max_size / PQ_REDUCE_THRESHOLD
 * fairly lenient threshold so as not to cause too many reallocations
 * the threshold for enlarging is simply when the pq is full
 */
#define PQ_TRUNCATE_THRESHOLD 8

/*
 * factor by which pq capacity decreases
 */
#define PQ_TRUNCATE_FACTOR 2


struct pq {
	void **arr;
	int size, max_size;
	int (*cmp)(const void *, const void *);
	void (*free)(void *);
};



void pq_init(struct pq *pq, int (*cmp)(const void *, const void *),
		void (*free)(void *));

struct pq *pq_new(int (*cmp)(const void *, const void *),
		void (*free)(void *));

/*
 * flags shall be 0 or some ORed combination of the below defines
 * pq_free() frees all the memory behind the instance of pq
 * PQ_FREE_PTR is whether the pq pointer itself should be freed
 * for example if it had been acquired via pq_new()
 * PQ_FREE_VALS is whether the elements in pq should be freed
 * they are freed using pq->free(), which must be non-NULL
 * in order to use this flag
 */
#define PQ_FREE_PTR (1)
#define PQ_FREE_ELEMS (1<<1)
void pq_free(struct pq *pq, int flags);

/*
 * places v into pq in its ordered location using cmp_v
 */
void pq_enq(struct pq *pq, void *elem);

/*
 * returns and removes the highest priority v as decided by pq->cmp()
 */
void *pq_deq(struct pq *pq);

/*
 * returns the highest priority v as decided by pq->cmp();
 */
const void *pq_peek(struct pq *pq);

#endif
