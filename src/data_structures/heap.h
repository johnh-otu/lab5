#ifndef HEAP_HEADER_GUARD
#define HEAP_HEADER_GUARD

#include "queue.h"

struct heap 
{
	struct process* head;
	int size;
};

void Hinit(struct heap* h);

void Hswap(struct process *a, struct process *b);

void Hheapify(struct heap *h, int i);

void Hinsert(struct heap *h, struct process *p);

void Hdeleteroot(struct heap *h);

void Hsort(struct heap *h);

void Hprint(struct heap *h);

#endif
