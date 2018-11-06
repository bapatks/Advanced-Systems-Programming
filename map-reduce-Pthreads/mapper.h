//mapper.h file
#include<pthread.h>
#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<unistd.h>
#include<stdbool.h>

#ifndef mapper_h_
#define mapper_h_

int buffer_size;
int user_num;
char fname[20]; //file size cannot be more than 9 characters long (including file extension)

struct tuple
{
    char userID[5];
    char topic[16];
    int weight;
};

struct buff_ctrl	
{
	struct tuple* buffer_ptr;
	pthread_mutex_t lock;
	pthread_cond_t full;
	pthread_cond_t empty;
	int in;
	int out;
	int numItems;
	int numProduced;
	int numConsumed;
	int done;
	/*
	myID is just a number between 0 to user_num indicating/assigning the thread number to reducer threads
	Does not have any relation with userID assigned to this member of buff_ctrl 
	*/
	int myID;
};

void* mapper(void *);
 
#endif