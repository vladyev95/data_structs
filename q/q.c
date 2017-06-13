#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include "q.h"

static void q_resize(struct q *q, unsigned new_size);


/*
 * initializes q
 * free_v may be NULL, if so, q_free_v() may not be called
 */
void q_init(struct q *q, void (*free_v)(void *v))
{
	q->size = q->front = q->back = 0;
	q->max_size = Q_INIT_MAX_SIZE;
	q->free_v = free_v;
	if (!(q->arr = malloc(sizeof(q->arr[0]) * Q_INIT_MAX_SIZE)))
		fprintf(stderr, "q_init() malloc() failure");
}

/*
 * returns a dynamically allocated, initialized q
 */
struct q *q_new(void (*free_v)(void *v))
{
	struct q *q;
	if (!(q = malloc(sizeof(*q))))
		fprintf(stderr, "q_new() malloc() failure");
	q_init(q, free_v);
	return q;
}

/*
 * frees only the memory behind the q
 */
void q_free(struct q *q)
{
	free(q->arr);
}

/*
 * frees  the memory behind the q
 * and the items currently in the queue using q->free_v
 */
void q_free_v(struct q *q)
{
	unsigned i;
	
	/*
	 * free segment from start of array to back of q
	 * if the back of the q is before the front in the array
	 */
	if (q->back < q->front)
		for (i=0; i<=q->back; i++)
			q->free_v(q->arr[i]);

	/*
	 * free segment from front of q to end of array or back of q
	 * whichever comes first
	 */
	if (q->size)
		for (i=q->front; i<=q->back && i<q->max_size; i++)
			q->free_v(q->arr[i]);
	free(q->arr);
}

/*
 * returns front of q, NULL if no items in q
 */
void *q_peek(struct q *q)
{
	return q->size ? q->arr[q->front] : NULL;
}

/*
 * enqueues the item v into the q
 * does resizing of q->arr if necessary
 */
void q_enq(struct q *q, void *v)
{
	if (q->size == q->max_size)
		q_resize(q, q->max_size * Q_GROWTH_FACTOR);
	if (q->size) {
		q->back = (q->back + 1) % q->max_size;
		q->arr[q->back] = v;
	} else {
		q->arr[0] = v;
	}
	q->size++;
}

/*
 * dequeues and returns the item at the front of the q
 * returns NULL if no items in q
 */
void *q_deq(struct q *q)
{
	void *ret;
	if (q->size) {
		ret = q->arr[q->front];
		q->front = (q->front + 1) % q->max_size;
		q->size--;
		
		if (!q->size) {
			q->front = q->back = 0;

		} else if (q->max_size >= Q_DOWNSIZE_LIMIT && 
			q->size <= q->max_size / Q_DOWNSIZE_DECISION_FACTOR) {

			q_resize(q, q->max_size / Q_DOWNSIZE_FACTOR);
		}
		return ret;
	}
	return NULL;
}

static void q_resize(struct q *q, unsigned new_size)
{
	void **new_arr;
	unsigned f2e, s2b;

	if (q->front <= q->back) {
		f2e = q->back - q->front + 1;
		s2b = 0;
	} else {
		f2e = q->max_size - q->front;
		s2b = q->back + 1;
	}

	if (!(new_arr = malloc(sizeof(new_arr[0]) * new_size)))
		fprintf(stderr, "q_downsize() malloc() failure");
	
	memcpy(new_arr, &q->arr[q->front], f2e * sizeof(new_arr[0]));
	memcpy(&new_arr[f2e], q->arr, s2b * sizeof(new_arr[0]));

	free(q->arr);
	q->arr = new_arr;
	q->front = 0;
	q->back = f2e + s2b - 1;
	q->max_size = new_size;
}
