#include "reducer.h"
#include "mapper.h"
#include<string.h>

void* reducer(void *member)
{
    int node_count=0;

    struct buff_ctrl *control_buff = (struct buff_ctrl *)member;
    struct Node* head = malloc(sizeof(struct Node));

    sleep(1);

    //printf("My thread ID is --- %d ---\n", control_buff->myID);
    //printf("** GHOST ** assigned me buffer at --- %p ---\n", control_buff->buffer_ptr);
    struct tuple *cell = control_buff->buffer_ptr;

    while(1)
    {
        pthread_mutex_lock(&(control_buff->lock));
        int flag = 0;

        while((control_buff->numItems == 0) && !(control_buff->done))
        {
            //printf("Thread - %d - waiting on empty\n",control_buff->myID);
            pthread_cond_wait(&(control_buff->empty), &(control_buff->lock));
        }

        if ((control_buff->numItems == 0) && (control_buff->done)) 
        {
            //printf("Thread -- %d -- exiting\n", control_buff->myID);
            pthread_mutex_unlock(&(control_buff->lock));
            break;  
        }

        /*----------------------------------------------------------------------------------------------*/
        //Extract tuples from buffer and populate into a linked list

        struct Node* new_node = (struct Node*)malloc(sizeof(struct Node));
        new_node->score = cell[control_buff->out].weight;
        strcpy(new_node->userID, cell[control_buff->out].userID);
        strcpy(new_node->topic, cell[control_buff->out].topic);
        new_node->next = NULL;
        
        if(node_count == 0)
        {
            head = new_node;
            node_count++;
        }
        else if(node_count>0)
        {
            struct Node* current_node; //pointer to iterate over linked list
            current_node = head;
            //printf("node_count = %d\n",node_count);
            while(1)
            {
                if(!strcmp(current_node->topic, new_node->topic))
                {
                    current_node->score += new_node->score;
                    flag = 1;
                    break;
                }

                if(current_node->next == NULL)
                {
                    break;
                }
                else
                {
                    current_node = current_node->next;
                }
            }

            if(flag == 0)
            {
                current_node->next = new_node;
                node_count++;
            }
        }

        //End of extract and populate code
        /*---------------------------------------------------------------------------------------------*/


        (control_buff->out) = ((control_buff->out) + 1) % buffer_size;
        (control_buff->numItems)--; 
        (control_buff->numConsumed)++;
        //printf("TID: %d numConsumed = %d numItems = %d\n", control_buff->myID, control_buff->numConsumed, control_buff->numItems);
        pthread_cond_signal(&(control_buff->full));
        pthread_mutex_unlock(&(control_buff->lock));
    }

    //Code to check traversal of linked list

    //printf("Node count is %d\n",node_count);
    struct Node* check_node;
    check_node = head;
    //printf("$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$\n");
    while(1)
    {
        if(check_node->next != NULL)
        {
            printf("(%s,%s,%d)\n",check_node->userID,check_node->topic,check_node->score);
            check_node = check_node->next;
        }
        else if(check_node->next == NULL)
        {
            printf("(%s,%s,%d)\n",check_node->userID,check_node->topic,check_node->score);
            break;
        }

    }
    //printf("$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$\n");

    pthread_exit((void*)member);
}