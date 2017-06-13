#include <stdlib.h>
#include <stdio.h>
#include "stack.h"

static void stack_resize(struct stack *stack);

void stack_init(struct stack *stack, void (*free_v)(void *v))
{
	stack->size = 0;
	stack->max_size = STACK_INIT_MAX_SIZE;
	stack->free_v = free_v;
	if (!(stack->arr = malloc(sizeof(stack->arr[0]) * STACK_INIT_MAX_SIZE))) {
		fprintf(stderr, "stack_init() malloc() failure");
	}
}

struct stack *stack_new(void (*free_v)(void *v))
{
	struct stack *stack;
	if (!(stack = malloc(sizeof(*stack))))
		fprintf(stderr, "stack_new() malloc() failure");
	stack_init(stack, free_v);
	return stack;
}

void stack_free(struct stack *stack)
{
	free(stack->arr);
}

void stack_free_all(struct stack *stack)
{
	int i;
	for (i=0; i<stack->size; i++)
		stack->free_v(stack->arr[i]);
	free(stack->arr);
}

void *stack_peek(struct stack *stack)
{
	return stack->arr[(int) stack->size-1];
}

void stack_push(struct stack *stack, void *v)
{
	if (stack->size == stack->max_size)
		stack_resize(stack);
	stack->arr[stack->size++] = v;
}

void *stack_pop(struct stack *stack)
{
	return stack->arr[(int) --stack->size];
}

static void stack_resize(struct stack *stack)
{
	stack->max_size *= STACK_GROWTH_FACTOR;
	if (!(stack->arr = realloc(stack->arr, sizeof(stack->arr[0]) * 
					stack->max_size))) {
		fprintf(stderr, "stack_resize() realloc() failure");
	}
}
