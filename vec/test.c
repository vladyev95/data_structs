#include <stdio.h>
#include <string.h>
#include "vec.h"


int cmp(const void *a, const void *b)
{
	return strcmp(a, b);
}

int main(void)
{
	struct vec vec;
	char *ret;
	char buf[1024];
	vec_init(&vec, cmp, free);
	while (fgets(buf, 1024, stdin)) {
		if (buf[strlen(buf) - 1] == '\n')
			buf[strlen(buf) - 1] = 0;
		vec_add_i(&vec, strdup(buf), vec.size);
	}

	while(vec.size) {
		ret = vec_rm_i(&vec, vec.size-1);
		printf("%s\n", ret);
		free(ret);
	}


	vec_free_all(&vec);
	return EXIT_SUCCESS;
}
