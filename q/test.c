#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "q.h"



int main(void)
{
	char buf[1024], *ret;
	struct q q;
	q_init(&q, free);
	while (fgets(buf, 1024, stdin)) {
		if (buf[strlen(buf) - 1] == '\n')
			buf[strlen(buf) - 1] = 0;
		q_enq(&q, strdup(buf));
	}
	while (ret = q_deq(&q)) {
		printf("%s\n", ret);
		free(ret);
	}
	q_free(&q);
	return 0;
}
