#include "../main.h"
#include "STscheduler.h"
#include "../data_structures/queue.h"

int mem[TOTALMEMORYMB] = {0};
int memavailable = TOTALMEMORYMB;
int printers = NPRINTERS;
int scanners = NSCANNERS;
int modems = NMODEMS;
int cds = NCDDRIVES;

struct queue* ST_RT_queue;
struct queue* ST_UJ_queue;
struct queue* ST_P1_queue;
struct queue* ST_P2_queue;
struct queue* ST_P3_queue;
int* total_completed;

void *STscheduler(void* args)
{
	ST_RT_queue = ((struct STscheduler_thread_data*)args)->RT_queue;
	ST_UJ_queue = ((struct STscheduler_thread_data*)args)->UJ_queue;
	ST_P1_queue = ((struct STscheduler_thread_data*)args)->P1_queue;
	ST_P2_queue = ((struct STscheduler_thread_data*)args)->P2_queue;
	ST_P3_queue = ((struct STscheduler_thread_data*)args)->P3_queue;
	total_completed = ((struct STscheduler_thread_data*)args)->num_completed;
	bool* DONE_FLAG = ((struct STscheduler_thread_data*)args)->flag;

	printf("short-term scheduler called\n");

	struct node* current_proc_node;

	while (!(*DONE_FLAG)) {
		
		current_proc_node = NULL;

		//check for RT processes and run
		if(emptyRTqueue(current_proc_node, 0))
			continue;

		//check for P1 processes and run if able
		if (!Qisempty(ST_P1_queue))
		{
			//note: P1 jobs will never preempt other jobs
			current_proc_node = Qdequeue(ST_P1_queue);

			if (!current_proc_node->data->suspended && !allocateMemoryAndResources(current_proc_node)) { //if new but no room
				Qenqueue(ST_P1_queue, current_proc_node);
				current_proc_node = NULL;
				continue;
			}
			else {
				executeProcess(current_proc_node->data);
				
				for (int t = 0; t < QUANTUM1 && t != current_proc_node->data->time_to_live; t++) 
				{
					sleep(1); //simulate tick;
					if (emptyRTqueue(current_proc_node, t))
						continue;
				}

				if (current_proc_node->data->time_to_live <= QUANTUM1) { //end ofprocess				    
					kill(current_proc_node->data->process_id, SIGINT);
					int status;
					waitpid(current_proc_node->data->process_id, &status, 0);
					if (WIFEXITED(status) || WIFSIGNALED(status)) 
						printf("Process %d terminated\n", current_proc_node->data->process_id);
					else
						printf("PROCESS TERMINATED INCORRECTLY: %d", status);
					freeMemoryAndResources(current_proc_node);
				}
				else
				{
					kill(current_proc_node->data->process_id, SIGTSTP);
					current_proc_node->data->priority++;
					current_proc_node->data->time_to_live = current_proc_node->data->time_to_live - QUANTUM1;
					current_proc_node->data->suspended = true;
					Qenqueue(ST_UJ_queue, current_proc_node);
					current_proc_node = NULL;
				}

				continue;

			}
		}

		//check for P2 processes and run if able
		if (!Qisempty(ST_P2_queue))
		{
			//note: P2 jobs will never preempt other jobs
			current_proc_node = Qdequeue(ST_P2_queue);
			if (!current_proc_node->data->suspended && !allocateMemoryAndResources(current_proc_node)) { //if new but no room
				Qenqueue(ST_P2_queue, current_proc_node);
				current_proc_node = NULL;
				continue;
			}
			else {
				executeProcess(current_proc_node->data);
				
				for (int t = 0; t < QUANTUM2 && t != current_proc_node->data->time_to_live; t++) 
				{
					sleep(1); //simulate tick;
					if (emptyRTqueue(current_proc_node, t))
						continue;
				}

				if (current_proc_node->data->time_to_live <= QUANTUM2) { //end ofprocess				    
					kill(current_proc_node->data->process_id, SIGINT);
					int status;
					waitpid(current_proc_node->data->process_id, &status, 0);
					if (WIFEXITED(status) || WIFSIGNALED(status)) 
						printf("Process %d terminated\n", current_proc_node->data->process_id);
					else
						printf("PROCESS TERMINATED INCORRECTLY: %d", status);
					freeMemoryAndResources(current_proc_node);
				}
				else
				{
					kill(current_proc_node->data->process_id, SIGTSTP);
					current_proc_node->data->priority++;
					current_proc_node->data->time_to_live = current_proc_node->data->time_to_live - QUANTUM2;
					current_proc_node->data->suspended = true;
					Qenqueue(ST_UJ_queue, current_proc_node);
					current_proc_node = NULL;
				}

				continue;

			}
		}

		//check for P3 processes and run if able
		if (!Qisempty(ST_P3_queue))
		{
			//note: P3 jobs will never preempt other jobs
			current_proc_node = Qdequeue(ST_P3_queue);
			if (!current_proc_node->data->suspended && !allocateMemoryAndResources(current_proc_node)) { //if new but no room
				Qenqueue(ST_P3_queue, current_proc_node);
				current_proc_node = NULL;
				continue;
			}
			else {
				executeProcess(current_proc_node->data);
				
				for (int t = 0; t < 1; t++) 
				{
					sleep(1); //simulate tick;
					if (emptyRTqueue(current_proc_node, t))
						continue;
					if (current_proc_node->data->time_to_live == 1)
						break;
				}

				if (current_proc_node->data->time_to_live == 1) { //end ofprocess				    
					kill(current_proc_node->data->process_id, SIGINT);
					int status;
					waitpid(current_proc_node->data->process_id, &status, 0);
					if (WIFEXITED(status) || WIFSIGNALED(status)) 
						printf("Process %d terminated\n", current_proc_node->data->process_id);
					else	
						printf("PROCESS TERMINATED INCORRECTLY: %d", status);
					freeMemoryAndResources(current_proc_node);
				}
				else
				{
					kill(current_proc_node->data->process_id, SIGTSTP);
					current_proc_node->data->priority++;
					current_proc_node->data->time_to_live = current_proc_node->data->time_to_live - 1;
					current_proc_node->data->suspended = true;
					Qenqueue(ST_UJ_queue, current_proc_node);
					current_proc_node = NULL;
				}

				continue;

			}
		} 		
	}	
	
	printf("NO MORE PROCESSES (STS)\n");	
	return NULL;
}

void executeProcess(struct process* proc)
{
	if(proc->suspended) {
		kill(proc->process_id, SIGCONT);
	}
	else {
		printf("Starting process %d - priority:%d, time-to-live:%d, memoryaddress: %x, memory size: %d, resources: p:%d,s:%d,m:%d,c:%d\n", proc->job_id, proc->priority, proc->time_to_live, proc->address, proc->Nmegabytes, proc->Nprinters, proc->Nscanners, proc->Nmodems, proc->Ncds);
		fflush(stdout);

		int pid = fork();

		if (pid < 0)
			printf("Error: couldn't fork\n");
		else if (pid == 0) {
			//child process
			char *time_to_live = malloc(5 * sizeof(char));
			char *job_id = malloc(7 * sizeof(char));
			snprintf(time_to_live, 5*sizeof(char), "%d", proc->time_to_live); //convert to string
			snprintf(job_id, 7*sizeof(char), "%d", proc->job_id);
			char *args[] = {"./dispatcher.exe", time_to_live, job_id, NULL};
			int statuscode = execvp(args[0], args); //simulate process
			if (statuscode == -1)
				printf("EXITED INCORRECTLY");
		}
		else {
			proc->process_id = pid;
		}
	}
}

bool emptyRTqueue(struct node* proc_node, int t)
{
	struct node* new_proc_node;

	if (!Qisempty(ST_RT_queue))
	{
		if (proc_node != NULL) { //swap with current
			new_proc_node = Qdequeue(ST_RT_queue);
			kill(proc_node->data->process_id, SIGTSTP);
			proc_node->data->priority++;
			proc_node->data->time_to_live = proc_node->data->time_to_live - t - 1;
			Qenqueue(ST_UJ_queue, proc_node);
			proc_node = new_proc_node;
		}
		else { //pop
			proc_node = Qdequeue(ST_RT_queue);
		}
		
		allocateMemoryAndResources(proc_node);
		executeProcess(proc_node->data);
		waitpid(proc_node->data->process_id, NULL, 0);
		printf("Process %d terminated\n", proc_node->data->process_id);
		freeMemoryAndResources(proc_node);
		return true;
	}
	
	return false;
}

bool allocateMemoryAndResources(struct node* proc_node)
{
	int mem_size = proc_node->data->Nmegabytes;
	
	if (proc_node->data->priority > 0) { //not realtime

		//check resources

		if (printers - proc_node->data->Nprinters < 0)
			return false; //not enough printers
		if (scanners - proc_node->data->Nscanners < 0)
			return false; //not enough scanners
		if (modems - proc_node->data->Nmodems < 0)
			return false; //not enough modems
		if (cds - proc_node->data->Ncds < 0)
			return false; //not enough cddrives
	
		//check for memory
	
		if (memavailable - mem_size < MAXRTMEMORY)
			return false; //not enough memory
		
	}

	//search for memory space
	int free_mem_count = 0;
	int start_index = 0;

	for (int i = 0; i < TOTALMEMORYMB; ++i) {
		if (mem[i] == 0) {
			free_mem_count++;
			if (free_mem_count == mem_size) { //found a spot
				start_index = i - mem_size + 1;
				proc_node->data->address = start_index;
					
				//fill memory
				for (int j = start_index; j < start_index + mem_size; j++) {mem[j] = 1;}
				memavailable -= mem_size;

				//take resources
				if (proc_node->data->priority > 0) {
					printers -= proc_node->data->Nprinters;
					scanners -= proc_node->data->Nscanners;
					modems -= proc_node->data->Nmodems;
					cds -= proc_node->data->Ncds;
				}

				return true;
			}
		}
		else {
			free_mem_count = 0;
		}
	}

	//no memory available
	return false;
}

void freeMemoryAndResources(struct node* proc_node)
{
	//free resources
	if (proc_node->data->priority > 0) {
		printers += proc_node->data->Nprinters;
		scanners += proc_node->data->Nscanners;
		modems += proc_node->data->Nmodems;
		cds += proc_node->data->Ncds;
	}
	
	//free memory
	int start_address = proc_node->data->address;
	int mem_size = proc_node->data->Nmegabytes;
	for (int i = start_address; i < start_address + mem_size; i++)
		mem[i] = 0;
	memavailable += mem_size;

	//mark completed and free / set to NULL
	*total_completed = *total_completed + 1;
	//printf("Total completed: %d\n", *total_completed);
	//free(proc_node->data);
	//free(proc_node);
	proc_node = NULL;
}


