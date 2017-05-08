#include <stdio.h>
#include <string.h>
#include "pq.h"

struct student {
	char name[64];
	int age;
	char eth[64];
};


int cmp_student(const void *a, const void *b)
{
	const struct student *ap, *bp;
	int res;
	ap = a;
	bp = b;
	res = strcmp(ap->name, bp->name);
	if (res < 0)
		return 1;
	else if (res > 0)
		return -1;
	if (ap->age < bp->age)
		return 1;
	else if (ap->age > bp->age)
		return -1;
	return strcmp(ap->eth, bp->eth);
}

void print_student(const struct student *s)
{
	printf("%s %d %s\n", s->name, s->age, s->eth);
}

int main(void)
{
	struct student dave, vlad, mike, harr, *ret;
	struct pq pq;
	strcpy(dave.name, "David Barrera");
	dave.age = 22;
	strcpy(dave.eth, "Black");
	strcpy(vlad.name, "Vladimir Yevseenko");
	vlad.age = 21;
	strcpy(vlad.eth, "Russian");
	strcpy(mike.name, "Michael Curratola");
	mike.age = 20;
	strcpy(mike.eth, "Italian");
	strcpy(harr.name, "Harrison Termotto");
	harr.age = 21;
	strcpy(harr.eth, "Italian");
	pq_init(&pq, cmp_student, NULL);
	pq_enq(&pq, &dave);
	pq_enq(&pq, &mike);
	pq_enq(&pq, &vlad);
	pq_enq(&pq, &harr);
	while (ret = pq_deq(&pq)) {
		print_student(ret);
	}
	pq_free(&pq);
	return 0;
}
