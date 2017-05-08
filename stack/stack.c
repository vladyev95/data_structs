#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include "stack.h"

void stack_init(struct stack *stack, void (*free_v)(void *v))
{
	stack->size = 0;
	stack->free_v = free_v;
	stack->top = NULL;
}

struct stack *stack_new(void (*free_v)(void *v))
{
	struct stack *stack;
	if (!(stack = malloc(sizeof(*stack))))
		fprintf(stderr, "stack malloc() failure");
	stack_init(stack, free_v);
	return stack;
}

void stack_free(struct stack *stack)
{
	struct stack_node *walk, *next;
	for (walk = stack->top; walk; walk = next) {
		next = walk->next;
		free(walk);
	}
}

void stack_free_all(struct stack *stack)
{
	struct stack_node *walk, *next;
	for (walk = stack->top; walk; walk = next) {
		next = walk->next;
		stack->free_v(walk->v);
		free(walk);
	}
}

void *stack_peek(struct stack *stack)
{
	return stack->top ? stack->top->v : NULL;
}

void stack_push(struct stack *stack, void *v)
{
	struct stack_node *new;
	if (!(new = malloc(sizeof(*new))))
		fprintf(stderr, "stack malloc() failure");
	new->v = v;
	new->next = stack->top;
	stack->top = new;
	stack->size++;
}

void *stack_pop(struct stack *stack)
{
	void *ret;
	struct stack_node *top;
	if (!stack->size)
		return NULL;
	top = stack->top;
	stack->top = top->next;
	ret = top->v;
	free(top);
	stack->size--;
	return ret;
}
