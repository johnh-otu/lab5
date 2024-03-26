#ifndef MT_SCHEDULER_HEADER_GUARD
#define MT_SCHEDULER_HEADER_GUARD

#include "../main.h"
#include "../data_structures/queue.h"

struct MTscheduler_thread_data {
	struct queue* UJ_queue; //user job queue
	struct queue* P1_queue;
	struct queue* P2_queue;
	struct queue* P3_queue;
	bool* flag;
	//pthread_mutex_t* lock;
	//pthread_cond_t* condition;
	//int* max_load_time;
};

void *MTscheduler(void* args);

#endif
