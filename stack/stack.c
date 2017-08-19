#include <stdlib.h>
#include <stdio.h>
#include "stack.h"


void stack_init(struct stack *stack, void (*free)(void *))
{
	stack->size = 0;
	stack->max_size = STACK_INIT_MAX_SIZE;
	stack->free = free;
	if (!(stack->arr = malloc(sizeof(stack->arr[0]) * stack->max_size))) {
		fprintf(stderr, "stack_init() malloc() FAILURE\n");
	}
}

struct stack *stack_new(void (*free_v)(void *v))
{
	struct stack *stack;
	if (!(stack = malloc(sizeof(*stack))))
		fprintf(stderr, "stack_new() malloc() FAILURE");
	stack_init(stack, free_v);
	return stack;
}


static void stack_free_has_elems(struct stack *stack);

void stack_free(struct stack *stack, int flags)
{
	if (flags & STACK_FREE_ELEMS);
		stack_free_has_elems(stack);
	free(stack->arr);
	if (flags & STACK_FREE_PTR)
		free(stack);
}

static void stack_free_has_elems(struct stack *stack)
{
	int i;
	for (i=0; i<stack->size; i++)
		stack->free(stack->arr[i]);
}






static void stack_resize(struct stack *stack, int new_max);

void stack_push(struct stack *stack, void *v)
{
	if (stack->size == stack->max_size)
		stack_resize(stack, stack->max_size * STACK_GROWTH_FACTOR);
	stack->arr[stack->size++] = v;
}

static void stack_resize(struct stack *stack, int new_max)
{
	stack->arr = realloc(stack->arr, sizeof(stack->arr[0]) * new_max);
	if (!stack->arr)
		fprintf(stderr, "stack_resize() realloc() FAILURE\n");
	stack->max_size = new_max;
}


void *stack_pop(struct stack *stack)
{
	void *ret;
	ret = stack->size ? stack->arr[--stack->size] : NULL;
	if (stack->size / STACK_TRUNCATE_THRESHOLD >= STACK_INIT_MAX_SIZE &&
		stack->max_size / STACK_TRUNCATE_THRESHOLD > stack->size)
		stack_resize(stack, stack->max_size / STACK_TRUNCATE_FACTOR);
	return ret;
}



const void *stack_peek(struct stack *stack)
{
	return stack->size ? stack->arr[stack->size-1] : NULL;
}
