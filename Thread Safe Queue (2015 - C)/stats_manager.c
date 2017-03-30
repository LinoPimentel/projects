#include "stats_manager.h"
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <pthread.h>
#include <signal.h>
#include <errno.h>


// Entry point for the stats manager process

pthread_mutex_t stats = PTHREAD_MUTEX_INITIALIZER;
unsigned int reqCount = 0, reqAccepted = 0, reqRefused = 0,localReqReplied = 0, externReqReplied = 0;
struct tm startTime;
struct tm lastUpdateTime;
int fd;

void exit_stats()
{
	printf("Exited stats manager,  %d\n",getpid()); // cleanup and exit
	close(fd);
	exit(0);
}


void show_stats() // put this in function so wecan also show stats on user input
{
	pthread_mutex_lock(&stats);
	printf("Server start time %d:%d:%d\n",startTime.tm_hour, startTime.tm_min, startTime.tm_sec);
	printf("	-Total requests: %d\n",reqCount);
	printf("	-Total accepted requests: %d\n",reqAccepted);
	printf("	-Total refused requests: %d\n",reqRefused);
	printf("	-Total local requests replied to: %d\n",localReqReplied);
	printf("	-Total external requests replied to: %d\n",externReqReplied);
	printf("	-Time of last information update: %d:%d:%d\n",lastUpdateTime.tm_hour, lastUpdateTime.tm_min, lastUpdateTime.tm_sec);
	pthread_mutex_unlock(&stats);
}

void *show_stats_time(void* args)
{
	while(1) // thread that will show stats every 30s
	{
		sleep(30);
		show_stats();
	}
}

void updateTime()
{
	time_t t = time(NULL);
	struct tm *currentTime;

	currentTime = localtime(&t);
	lastUpdateTime.tm_hour = currentTime->tm_hour;
	lastUpdateTime.tm_min = currentTime->tm_min;
	lastUpdateTime.tm_sec = currentTime->tm_sec;
}

void stats_manager_main(struct configuration *config)
{
	pthread_t showStats;
	time_t t = time(NULL);
	struct tm *currentTime;
	char recv;

	pthread_create(&showStats,NULL,show_stats_time,NULL);

	currentTime = localtime(&t); // gets our server start time
	startTime.tm_hour = currentTime->tm_hour;
	startTime.tm_min = currentTime->tm_min;
	startTime.tm_sec = currentTime->tm_sec;


	if((fd = open(config->stats_pipe, O_RDONLY)) == -1 )
		printf("Error opening stats pipe for reading, errno n %d\n",errno);
	signal(SIGUSR2, exit_stats);
	signal(SIGUSR1, show_stats);

	while(1) // infinite loop to read from pipe to update stats
	{
		read(fd,&recv,1);
		pthread_mutex_lock(&stats);
		switch(recv)
		{
			case 0: {	
						reqCount++;
						updateTime();
					}break;
			case 1: {
						reqAccepted++;
						updateTime();
					}break;
			case 2: {
						reqRefused++;
						updateTime();
					}break;
			case 3: {
						localReqReplied++;
						updateTime();
					}break;
			case 4: {
						externReqReplied++;
						updateTime();
					}break;
		}
		pthread_mutex_unlock(&stats);
	}
}
