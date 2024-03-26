#include "../main.h"
#include "loader.h"
#include "../data_structures/queue.h"
#include "../data_structures/heap.h"

struct heap process_heap;

void *load_queue_from_file(void *args) {
	
	printf("loader called\n");	
	struct queue* job_queue = ((struct loader_thread_data*)args)->job_queue;
	pthread_mutex_t *lock = ((struct loader_thread_data*)args)->lock;
	pthread_cond_t *loading_finished = ((struct loader_thread_data*)args)->condition;
	int *max_load_time = ((struct loader_thread_data*)args)->max_load_time;
	int *total_processes = ((struct loader_thread_data*)args)->num_processes;
	char file_path[MAXPATHSIZE]; 
	strcpy(file_path, ((struct loader_thread_data*)args)->path);

	pthread_mutex_lock(lock); //take lock once released
	printf("loading...\n");
	
	//load from jobs.txt
	
	/* 
	 * Jobs.txt format:
	 * ===================================================================================================================================
	 * <arrival time>,	<priority>,	<processor time>,	<Mbytes>,	<#printers>,	<#scanners>,	<#modems>,	<#CDs>
	 * INT			INT		INT			INT		INT		INT		INT		INT
	 * [0, inf)		[0, 3]		[0, inf)		[0, 1024]	[0, 2]		[0, 1]		[0, 1]		[0, 2]
	 *
	 * examples:
	 * 12,0,1,64,0,0,0,0
	 * 12,1,2,128,1,0,0,1
	 * 13,3,6,128,1,0,1,2
	 *
	 */

	FILE *fp = fopen(file_path, "r");

	if (fp == NULL) {
		perror("Error opening file: ");
		exit(EXIT_FAILURE);
	}
	else {
		printf("file loaded\n");
	}
	
	//Get process info from file and store in N buckets, where N is the max arrival time for all processes in jobs.txt
	//Each process is stored in a bucket according to their arrival time

	struct process* temp_process;
	Hinit(&process_heap);

	int id_iterator = 1;

	while(1) { //read processes from file
		
		temp_process = (struct process*)malloc(sizeof(struct process));
		
		temp_process->job_id = id_iterator++;
		temp_process->process_id = 0;
		temp_process->suspended = false;
		fscanf(fp, "%d,%d,%d,%d,%d,%d,%d,%d ", &(temp_process->arrival_time), &(temp_process->priority), &(temp_process->time_to_live), &(temp_process->Nmegabytes), &(temp_process->Nprinters), &(temp_process->Nscanners), &(temp_process->Nmodems), &(temp_process->Ncds));

		Hinsert(&process_heap, temp_process); //insert process into heap
		//Hprint(&process_heap);

		if (feof(fp)) //if end of file, finished reading
			break;

	}
	fclose(fp);
	
	*total_processes = process_heap.size;
	Hsort(&process_heap); //convert heap into array sorted by arrival time
	//Hprint(&process_heap);
	printf("file read and closed\n");

	pthread_cond_signal(loading_finished); //tell scheduler loading is finished
	pthread_mutex_unlock(lock);
	
	int i = 0;
	*max_load_time = process_heap.head[*total_processes - 1].arrival_time;
	
	for (int t = 0; t < *max_load_time + 1; t++) //each tick
	{
		//printf("L%d:", t);
		
		//foreach job in sorted array where arrival time == current tick
		while(process_heap.head[i].arrival_time == t)
		{
			//printf(" %d", process_heap.head[i].process_id);

			//create new node from process
			struct node* temp = malloc(sizeof(struct node));
			temp->data = &process_heap.head[i];
			temp->next = NULL;

			//add node to job_queue
			Qenqueue(job_queue,temp); 
			
			i++; //go to next job
			if (i >= *total_processes)
				break; //end of process list
		}
		
		//printf("\n");
		//Qheadinfo(job_queue);

		if (i >= *total_processes)
			break; //end of process list

		sleep(1); //simulate tick
	}

	printf("LOADER FINISHED, goodbye!\n");
	return NULL;
}
