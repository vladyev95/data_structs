#ifndef Q_H
#define Q_H

#include <sys/types.h>

struct q_node {
	struct q_node *next;
	void *v;
};

struct q {
	struct q_node *front, *back;
	size_t size;
	void (*free_v)(void *v);
};


void q_init(struct q *q, void (*free_v)(void *v));
struct q *q_new(void (*free_v)(void *v));

void q_free(struct q *q);
void q_free_all(struct q *q);

void q_enq(struct q *q, void *v);
void *q_deq(struct q *q);
void *q_peek(struct q *q);

#endif
