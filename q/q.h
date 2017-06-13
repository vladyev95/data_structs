#ifndef Q_H
#define Q_H

#define Q_INIT_MAX_SIZE 64

/*
 * when the q is full, q->max_size *= Q_GROWTH_FACTOR
 */
#define Q_GROWTH_FACTOR 4

/*
 * q is resized if q->size <= q->max_size / Q_RESIZE_DECISION_FACTOR
 */
#define Q_DOWNSIZE_DECISION_FACTOR 16

/*
 * q->max_size is reduced to q->max_size / Q_DOWNSIZE_FACTOR
 */
#define Q_DOWNSIZE_FACTOR 4


/*
 * q must both satisfy Q_RESIZE_DECISION_FACTOR and must be at least
 * as large as Q_DOWNSIZE_LIMIT
 */
#define Q_DOWNSIZE_LIMIT 4096

/*
 * implemented using a circular array
 */

struct q {
	void **arr;
	unsigned size, max_size, front, back;
	void (*free_v)(void *v);
};


/*
 * initializes the q, free_v may be NULL, don't use q_free_v() if so
 */
void q_init(struct q *q, void (*free_v)(void *v));

/*
 * returns dynamically allocated and initialized q
 */
struct q *q_new(void (*free_v)(void *v));

void q_free(struct q *q);
void q_free_v(struct q *q);

void q_enq(struct q *q, void *v);
void *q_deq(struct q *q);
void *q_peek(struct q *q);

#endif
