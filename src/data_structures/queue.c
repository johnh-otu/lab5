#include "../main.h"
#include "queue.h"

void Qenqueue(struct queue *queue, struct node *new_node)
{
	if(pthread_mutex_lock(queue->lock) < 0) //take lock
	{
		perror("MUTEX LOCK FAIL: ");
		exit(EXIT_FAILURE);
	}	

	new_node->next = NULL; //ensure next is null

	if (queue->head == NULL) { //if queue is empty
		queue->head = new_node;
		queue->tail = new_node; //set node to head and tail
	}
	else {	
		queue->tail->next = new_node; //add node to tail
		queue->tail = new_node;
	}

	if(pthread_mutex_unlock(queue->lock) < 0) //release lock
	{
		perror("MUTEX LOCK FAIL: ");
		exit(EXIT_FAILURE);
	}

}

struct node* Qdequeue(struct queue *queue)
{
	if(pthread_mutex_lock(queue->lock) < 0) //take lock
	{
		perror("MUTEX LOCK FAIL: ");
		exit(EXIT_FAILURE);
	}	
	if (queue->head == NULL) //if queue is empty
	{
		pthread_mutex_unlock(queue->lock);
		return NULL;
	}

	struct node *output = queue->head; //get head
	queue->head = queue->head->next; //set head to next node
	
	if (queue->head == NULL) //if queue is empty
		queue->tail = NULL; //reset tail
	
	if(pthread_mutex_unlock(queue->lock) < 0) //release lock
	{
		perror("MUTEX LOCK FAIL: ");
		exit(EXIT_FAILURE);
	}
		
	return output;
}

bool Qisempty(struct queue *queue)
{
	if(pthread_mutex_lock(queue->lock) < 0) //take lock
	{
		perror("MUTEX LOCK FAIL: ");
		exit(EXIT_FAILURE);
	}	

	bool output = (queue->head == NULL);
	
	if(pthread_mutex_unlock(queue->lock) < 0) //release lock
	{
		perror("MUTEX LOCK FAIL: ");
		exit(EXIT_FAILURE);
	}

	return output;
}

void Qwalk(struct queue *queue)
{
	if(pthread_mutex_lock(queue->lock) < 0) //take lock
	{
		perror("MUTEX LOCK FAIL: ");
		exit(EXIT_FAILURE);
	}	
	
	if (queue->head == NULL) {
		printf("Q:queue empty\n");
		pthread_mutex_unlock(queue->lock);
		return;
	}

	struct node* temp = queue->head;

	printf("Q:");
	while(temp != NULL) {
		printf(" %d", temp->data->job_id);
		temp = temp->next;
	}
	printf("\n");

	if(pthread_mutex_unlock(queue->lock) < 0) //release lock
	{
		perror("MUTEX LOCK FAIL: ");
		exit(EXIT_FAILURE);
	}

	return;
}

void Qheadinfo(struct queue *queue)
{
	if(pthread_mutex_lock(queue->lock) < 0) //take lock
	{
		perror("MUTEX LOCK FAIL: ");
		exit(EXIT_FAILURE);
	}	

	if (queue->head == NULL)
		printf("Q: queue empty\n");
	else if (queue->tail == NULL)
		printf("Q!!!: head->%d, tail->NULL\n", queue->head->data->job_id);
	else if (queue->head->next == NULL)
		printf("Q: head->%d, tail->%d, head->next->NULL\n", queue->head->data->job_id, queue->tail->data->job_id);
	else
		printf("Q: head->%d, tail->%d, head->next->%d\n", queue->head->data->job_id, queue->tail->data->job_id, queue->head->next->data->job_id);

	if(pthread_mutex_unlock(queue->lock) < 0) //release lock
	{
		perror("MUTEX LOCK FAIL: ");
		exit(EXIT_FAILURE);
	}

	return;
}
