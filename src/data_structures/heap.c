#include "queue.h"
#include "../main.h"
#include "heap.h"

void Hinit(struct heap* h)
{
	h->head = NULL;
	h->size = 0;
}

void Hswap(struct process *a, struct process *b)
{
	struct process temp = *a;
	*a = *b;
	*b = temp;
}

void Hheapify(struct heap *h, int i)
{
	int largest = i;
	int left = 2 * i + 1;
	int right = 2 * i + 2;

	if (left < h->size && h->head[left].arrival_time > h->head[largest].arrival_time)
		largest = left;

	if (right < h->size && h->head[right].arrival_time > h->head[largest].arrival_time)
		largest = right;

	if (largest != i) {
		Hswap(&h->head[i], &h->head[largest]);
		Hheapify(h, largest);
	}
}

void Hinsert(struct heap *h, struct process *p)
{
	h->size++;
	h->head = (struct process*)realloc(h->head, h->size * sizeof(struct process));

	h->head[h->size - 1] = *p;

	int i = h->size - 1;
	while ( i != 0 && h->head[(i - 1)/2].arrival_time < h->head[i].arrival_time) {
		Hswap(&h->head[i], &h->head[(i - 1) / 2]);
		i = (i - 1) / 2;
	}
}

void Hsort(struct heap *h) {
	
	int size = h->size;

	for (int i = size - 1; i >= 0; i--) {
		Hswap(&h->head[0], &h->head[i]);
		h->size--;
		Hheapify(h, 0);
	}

	h->size = size;
}

void Hprint(struct heap *h) {
	printf("heap: ");
	for(int i = 0; i < h->size; i++)
		printf("[%d, %d],", h->head[i].process_id, h->head[i].arrival_time);
	printf("\n");
}
