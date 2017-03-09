#ifndef PROJECTOSO_QUEUE_H
#define PROJECTOSO_QUEUE_H

#include <netdb.h>
#include <arpa/inet.h>
#include <netinet/in.h>

struct request
{
	char resolvedIP[16];
	unsigned short dnsId;
	int sockfd;
	struct sockaddr_in dest;
	char *resolveName;
};

struct queue;

//Call this to create a new queue, returns a pointer to the new queue
struct queue* create_queue();

//Recieves a pointer to a request and a pointer to the queue to add to, adds it to the queue and returns 1 if succeed and 0 if failed
char add_to_queue(struct queue *queue,struct request *req);

//Recieves a pointer to the queue to get from, returns the last element if possible (maybe concurrent queue later on?) // returns null if couldnt get
struct request* get_from_queue(struct queue *queue);

void destroy_queue(struct queue *queue);

#endif