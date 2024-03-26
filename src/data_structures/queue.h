#ifndef QUEUE_HEADER_GUARD
#define QUEUE_HEADER_GUARD

#include "../main.h"

struct process
{
	int process_id;
	int job_id;
	int arrival_time;
	int priority;
	int time_to_live;
	int Nmegabytes;
	int Nprinters;
	int Nscanners;
	int Nmodems;
	int Ncds;
	bool suspended;
	int address;
};

struct node
{
	struct process* data;
	struct node* next;
};

struct queue
{ 
	struct node* head;
	struct node* tail;
	int quantum; //optional
	pthread_mutex_t* lock;
};

void Qenqueue(struct queue *queue, struct node *new_node);

struct node* Qdequeue(struct queue *queue);

bool Qisempty(struct queue *queue);

void Qwalk (struct queue *queue);
void Qheadinfo(struct queue *queue);

#endif
