#ifndef ST_SCHEDULER_HEADER_GUARD
#define ST_SCHEDULER_HEADER_GUARD

#include "../main.h"
#include "../data_structures/queue.h"

struct STscheduler_thread_data {
	struct queue* RT_queue; //real-time queue
	struct queue* UJ_queue; //user jobs queue
	struct queue* P1_queue;
	struct queue* P2_queue;
	struct queue* P3_queue;
	int* num_completed;
	bool* flag;
};

void *STscheduler(void* args);

void executeProcess(struct process* proc);
bool emptyRTqueue(struct node* proc_node, int t);
bool allocateMemoryAndResources(struct node* proc_node);
void freeMemoryAndResources(struct node* proc_node);

#endif
