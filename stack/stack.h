#ifndef STACK_H
#define STACK_H

#define STACK_INIT_MAX_SIZE 64

#define STACK_GROWTH_FACTOR 4

struct stack {
	void **arr;
	unsigned size, max_size;
	void (*free_v)(void *v);
};

void stack_init(struct stack *stack, void (*free_v)(void *v));

struct stack *stack_new(void (*free_v)(void *v));

void stack_free(struct stack *stack);
void stack_free_all(struct stack *stack);

void stack_push(struct stack *stack, void *v);
void *stack_pop(struct stack *stack);
void *stack_peek(struct stack *stack);


#endif
