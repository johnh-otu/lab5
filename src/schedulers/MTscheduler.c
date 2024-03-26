#include "../main.h"
#include "MTscheduler.h"
#include "../data_structures/queue.h"

void *MTscheduler(void* args)
{
	struct queue* UJ_queue = ((struct MTscheduler_thread_data*)args)->UJ_queue;
	struct queue* P1_queue = ((struct MTscheduler_thread_data*)args)->P1_queue;
	struct queue* P2_queue = ((struct MTscheduler_thread_data*)args)->P2_queue;
	struct queue* P3_queue = ((struct MTscheduler_thread_data*)args)->P3_queue;
	bool* DONE_FLAG = ((struct MTscheduler_thread_data*)args)->flag;
	//pthread_mutex_t* lock = ((struct scheduler_thread_data*)args)->lock;
	//pthread_cond_t* loading_finished = ((struct scheduler_thread_data*)args)->condition;
	//int *max_load_time = ((struct scheduler_thread_data*)args)->max_load_time;

	printf("medium-term scheduler called\n");
	//pthread_mutex_lock(lock); //take lock first
	//pthread_cond_wait(loading_finished, lock); //release lock and wait for loader to finish loading

	int t = 0;
	struct node* temp;
	while(!(*DONE_FLAG))
	{
		//pop from job_queue
		temp = Qdequeue(UJ_queue);
		
		if (temp != NULL)
		{
			//printf("MTS%d: pid:%d priority:%d\n", t, temp->data->process_id, temp->data->priority);

			if (temp->data->priority == 1)
				Qenqueue(P1_queue, temp);
			else if (temp->data->priority == 2)
				Qenqueue(P2_queue, temp);
			else
				Qenqueue(P3_queue, temp);
		}
		else
			//printf("MTS%d: \n", t);
		
		//Qheadinfo(UJ_queue);

		sleep(1); //simulate tick
		t++;
	}
	
	printf("NO MORE PROCESSES (MTS)\n");	
	return NULL;
}
