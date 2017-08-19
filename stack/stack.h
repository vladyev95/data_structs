#ifndef STACK_H
#define STACK_H

#define STACK_INIT_MAX_SIZE 64

#define STACK_GROWTH_FACTOR 4

#define STACK_TRUNCATE_THRESHOLD 16

#define STACK_TRUNCATE_FACTOR 2

struct stack {
	void **arr;
	int size, max_size;
	void (*free)(void *);
};

void stack_init(struct stack *stack, void (*free)(void *));

struct stack *stack_new(void (*free)(void *));

#define STACK_FREE_PTR (1)
#define STACK_FREE_ELEMS (1<<1)
void stack_free(struct stack *stack, int flags);

void stack_push(struct stack *stack, void *elem);
void *stack_pop(struct stack *stack);
const void *stack_peek(struct stack *stack);


#endif
