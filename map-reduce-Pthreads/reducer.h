//reducer.h file
#include<stdio.h>
#include<pthread.h>
#include<string.h>
#include<stdlib.h>

#ifndef reducer_h_
#define reducer_h_
 
struct Node
{
    char userID[5];
    char topic[16];
    int score;
    struct Node *next;
};

void* reducer(void *);
 
#endif