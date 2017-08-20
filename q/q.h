#ifndef Q_H
#define Q_H

#define Q_INIT_MAX_SIZE (64)

/*
 * when the q is full, q->max_size *= Q_GROWTH_FACTOR
 */
#define Q_GROWTH_FACTOR (4)

#define Q_ENLARGE_SIZE(pq) ((pq)->max_size * Q_GROWTH_FACTOR)

/*
 * q is resized if q->size <= q->max_size / Q_TRUNCATE_THRESHOLD
 */
#define Q_TRUNCATE_THRESHOLD (16)

#define Q_TRUNCATE_FACTOR (2)

#define Q_TRUNCATE_SIZE(q) ((q)->max_size / Q_TRUNCATE_FACTOR)

#define Q_CAN_TRUNCATE(q) ((q)->max_size / Q_TRUNCATE_FACTOR >= Q_INIT_MAX_SIZE)

#define Q_SHOULD_TRUNCATE(q) ((q)->size < (q)->max_size / Q_TRUNCATE_THRESHOLD)

struct q {
	void **arr;
	int size, max_size, front, back;
	void (*free)(void *);
};


/*
 * initializes the q, free_v may be NULL, don't use q_free_v() if so
 */
void q_init(struct q *q, void (*free)(void *));

/*
 * returns dynamically allocated and initialized q
 */
struct q *q_new(void (*free)(void *));

#define Q_FREE_PTR (1)
#define Q_FREE_ELEMS (1<<1)
void q_free(struct q *q, int flags);

void q_enq(struct q *q, void *elem);
void *q_deq(struct q *q);
const void *q_peek(struct q *q);

#endif
