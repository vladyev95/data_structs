#ifndef UTILS_H
#define UTILS_H

#include <stdint.h>

int *new_int(int x);
int cmp_str(const void *a, const void *b);
size_t read_lword(char *buf, FILE *fp);
uint32_t hash_str(const void *key);
void print_str(const void *s);


#endif
