#include "../main.h"
#include "LTscheduler.h"
#include "../data_structures/queue.h"

void *LTscheduler(void* args)
{
	struct queue* job_queue = ((struct LTscheduler_thread_data*)args)->job_queue;
	struct queue* RT_queue = ((struct LTscheduler_thread_data*)args)->RT_queue;
	struct queue* UJ_queue = ((struct LTscheduler_thread_data*)args)->UJ_queue;
	pthread_mutex_t* lock = ((struct LTscheduler_thread_data*)args)->lock;
	pthread_cond_t* loading_finished = ((struct LTscheduler_thread_data*)args)->condition;
	int *max_load_time = ((struct LTscheduler_thread_data*)args)->max_load_time;

	printf("long-term scheduler called\n");
	pthread_mutex_lock(lock); //take lock first
	pthread_cond_wait(loading_finished, lock); //release lock and wait for loader to finish loading

	int t = 0;
	struct node* temp;
	while((t <= *max_load_time) || !Qisempty(job_queue))
	{
		//pop from job_queue
		temp = Qdequeue(job_queue);
		
		
		if (temp != NULL)
		{
			//printf("LTS%d: pid:%d priority:%d\n", t, temp->data->process_id, temp->data->priority);
			
			if (temp->data->priority > 0)
				Qenqueue(UJ_queue, temp);
			else
				Qenqueue(RT_queue, temp);
		}
		else {
			//printf("LTS%d: \n", t);
		}

		//Qheadinfo(job_queue);

		sleep(1); //simulate tick
		t++;
	}
	
	printf("NO MORE PROCESSES (LTS)\n");	
	return NULL;
}
