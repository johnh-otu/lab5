#ifndef LT_SCHEDULER_HEADER_GUARD
#define LT_SCHEDULER_HEADER_GUARD

#include "../main.h"
#include "../data_structures/queue.h"

struct LTscheduler_thread_data {
	struct queue* job_queue; //job dispatch list
	struct queue* RT_queue; //real-time processes
	struct queue* UJ_queue; //user job queue
	pthread_mutex_t* lock;
	pthread_cond_t* condition;
	int* max_load_time;
};

void *LTscheduler(void* args);

#endif
