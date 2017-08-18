#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include <stdio.h>
#include "stack.h"


struct bst {
	struct node *root;
};

struct node {
	struct node *left, *right;
	void *v;
};

char *read_word(void)
{
	int i, c;
	char buf[1024];
	i = 0;
	while (isspace(c = getchar())) ;

	while (c != EOF && !isspace(c) && i < 1023) {
		buf[i++] = c;
		c = getchar();
	}
	if (c == EOF && i == 0)
		return NULL;
	buf[i] = '\0';
	return strdup(buf);
}

void place_into_bst_rec(struct node *root, struct node *n)
{
	if (strcmp(root->v, n->v) >= 0) {
		if (root->left)
			place_into_bst_rec(root->left, n);
		else
			root->left = n;
	} else {
		if (root->right)
			place_into_bst_rec(root->right, n);
		else
			root->right = n;
	}
}

void place_into_bst(struct bst *bst, char *word)
{
	struct node *n;
	n = malloc(sizeof(*n));
	n->left = n->right = NULL;
	n->v = word;
	if (!bst->root)
		bst->root = n;
	else
		place_into_bst_rec(bst->root, n);
}

void do_dfs(struct bst *bst)
{
	struct stack stack;
	struct node *popped;
	if (!bst->root)
		return;
	stack_init(&stack, NULL);
	stack_push(&stack, bst->root);
	while (stack.size) {
		popped = stack_pop(&stack);
		printf("%s\n", (char *) popped->v);
		if (popped->right)
			stack_push(&stack, popped->right);
		if (popped->left)
			stack_push(&stack, popped->left);
	}
	stack_free(&stack);
}

void main(int argc, char *argv[])
{
	struct bst *bst;
	char *word;
	bst->root = NULL;
	while (word = read_word()) {
		place_into_bst(bst, word);
	}
	do_dfs(bst);
}



