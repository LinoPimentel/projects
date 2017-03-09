#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include "queue.h"

#include <errno.h>
#define MAX_THREADS_READING 4

struct queueElement
{
	struct request *requestPtr;
	struct queueElement *next;
};

struct queue
{
	//insert the needed items to operate the queue -> next / last / head etc
	struct queueElement *head, *tail; // its a pointer since we only know the size on runtime
	pthread_mutex_t writeMutex;
	sem_t readSem;
};

struct queue* create_queue()
{
	int i;
	struct queue *queue = (struct queue*)malloc(sizeof(struct queue)); // creates the queue
	queue->head = NULL;
	queue->tail = NULL;


	if(pthread_mutex_init(&(queue->writeMutex),NULL))
		printf("Unexpected error has occured in creation of queue mutex, program funcionality may be impaired\n");

	if(sem_init(&(queue->readSem),0,MAX_THREADS_READING))
		printf("Unexpected error on creating queue semaphore, errno n %d\n",errno);

	//add all the initializations for the queue here
	return queue;
}

char add_to_queue(struct queue *queue,struct request *req)
{
	struct queueElement *insert;

	insert = (struct queueElement*)malloc(sizeof(struct queueElement));
	insert->requestPtr = req;
	insert->next = NULL;

	if(pthread_mutex_lock(&(queue->writeMutex))) 
	{
		free(insert);
		return 0; // couldnt get lock, go do other things
	}

	//once we get here we managed to get the lock
	if(!queue->tail) // if tail is null
	{		
		queue->tail = insert;
		//if tail was null then so was the head
		queue->head = queue->tail;
		pthread_mutex_unlock(&(queue->writeMutex)); // unlock the mutex before returning
		return 1;
	}
	else // tail was not null
	{
		queue->tail->next = insert; // the next of the previous tails now points to our new elemnt
		queue->tail = insert; // the tail is now our new element
		pthread_mutex_unlock(&(queue->writeMutex)); // unlock the mutex before returning
		return 1;
	}
}

struct request* get_from_queue(struct queue *queue)
{
	struct queueElement *queueEle;
	struct request *req;
	if(!sem_trywait(&(queue->readSem))) // managed to get into the queue
	{
		if(!pthread_mutex_trylock(&(queue->writeMutex))) // if it fails to get the lock
		{
			//if it gets here then we got our lock
			if(queue->head) // if head is not null
			{
				queueEle = queue->head;
				if(queue->head == queue->tail)
				{
					queue->head = NULL;
					queue->tail = NULL;
				}
				else
					queue->head = queueEle->next;
				pthread_mutex_unlock(&(queue->writeMutex)); // we can unlock our mutex at this point as we already isolated what we'll be reading
				sem_post(&(queue->readSem));
				req = queueEle->requestPtr;
				free(queueEle);
				return req;
			}
			else
			{
				pthread_mutex_unlock(&(queue->writeMutex));
				sem_post(&(queue->readSem));
				return NULL; // null means queue was empty, NULL = 0
			}
		}
		else
		{
			sem_post(&(queue->readSem));
			return NULL; // someone is reading or writing, go check other things
		}
	}
	else
	{	
		return NULL;
	}
}

void destroy_queue(struct queue *queue)
{
	struct queueElement *ele;
	while(queue->head) // while head exists
	{
		ele = queue->head;
		if(queue->head->next)
			queue->head = queue->head->next;
		if(ele->requestPtr)
			free(ele->requestPtr);
		free(ele);
	}
	if(pthread_mutex_destroy(&(queue->writeMutex))) // cleans semaphores and mutex
		printf("Unexpected error has occured in destruction of queue mutex, program may not cleanup properly\n");
	if(sem_destroy(&(queue->readSem)))
		printf("Unexpected error on destroying queue semaphore, errno n %d\n",errno);
	return;
}
