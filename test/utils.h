#ifndef UTILS_H
#define UTILS_H

#include <stdint.h>

struct wstat {
	int freq;
	float perc;
};

struct wstat *new_wstat(void);

int *new_int(int x);
int cmp_str(const void *a, const void *b);
char *read_lword(char *buf, FILE *fp);
uint32_t hash_str(const void *key);


#endif
