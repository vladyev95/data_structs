#include <stdio.h>
#include <string.h>
#include "stack.h"



int main(void)
{
	struct stack stack;
	char buf[1024], *ret;
	stack_init(&stack, free);
	while (fgets(buf, 1024, stdin)) {
		if (buf[strlen(buf) - 1] == '\n')
			buf[strlen(buf) - 1] = 0;
		stack_push(&stack, strdup(buf));
	}
	while (stack.size) {
		ret = stack_pop(&stack);
		printf("%s\n", ret);
		free(ret);
	}
	stack_free(&stack);
	return 0;
}
