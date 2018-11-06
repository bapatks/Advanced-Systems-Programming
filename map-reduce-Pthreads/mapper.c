//
// Created by kaust on 1/19/2018.
//
#include "mapper.h"

/*

*/

void* mapper(void *buff)
{
    char str[25];
    char action;
    char* line;
    FILE *infp;
    int flag = 0;
    int track_ptr = 0, find_user = 0;
    bool done_status;


    struct buff_ctrl* arr = (struct buff_ctrl *)buff;
    struct buff_ctrl* temp_ptr;
    struct tuple T;
    int track_user_id[user_num];
    
    sleep(1);
    
    infp = fopen(fname,"r");

    for(int j=0; j<user_num;++j)
    {
        track_user_id[j] = -1;
    }
    
    
    //arr->numItems = 4;

    //print array elements from mapper thread
    /*
    printf("######################################################\n");
    for(int k=0; k<user_num; k++)
    {
        //temp_ptr = arr + k;
        printf("buffer ptr =  %p\n", arr[k].buffer_ptr);
        printf("in =  %d\n", arr[k].in);
        printf("out =  %d\n", arr[k].out);
        printf("numItems =  %d\n", arr[k].numItems);
    }
    printf("######################################################\n");
    */
    
    if (infp==NULL)
    {
        printf("File not found");
        return ((void*) buff);
    }
    

    while(1)
    {
        
        flag = 0;
        line = fgets(str,26,infp);
        temp_ptr = arr;

        if(line != NULL)
        {
            if(str[0]=='(' && str[23]==')')
            {
                if(str[24]==',' || str[24]=='\0')
                {
                    if(str[5]==',' && str[7]==',')
                    {
                        //printf("\nchecked successfully");
                        for(int j=0;j<4;j++)
                        {
                            T.userID[j] = str[1+j];
                        }
                        T.userID[4] = '\0';

                        //printf("%d\n", atoi(T.userID));
                        
                        for(int j=0; j<user_num;++j)
                        {
                            if(track_user_id[j] == atoi(T.userID))
                            {
                                flag = 1;
                                if(track_ptr==0)
                                {
                                    track_ptr++;
                                }
                                break;
                            }
                        }
                        //printf("flag = %d\n",flag);
                        if(flag == 0)
                        {
                            track_user_id[track_ptr] = atoi(T.userID);
                            if(track_ptr<user_num)
                            {
                                track_ptr++;
                            }
                            
                        }
                        
                        //printf("track_ptr = %d\n",track_ptr);
                        action = str[6];
                        switch(action)
                        {
                            case 'P': T.weight = 50;
                                break;
                            case 'L': T.weight = 20;
                                break;
                            case 'D': T.weight = -10;
                                break;
                            case 'C': T.weight = 30;
                                break;
                            case 'S': T.weight = 40;
                                break;
                            default: pthread_exit((void*) buff);

                        }

                        for(int i=0;i<15;i++)
                        {
                            T.topic[i] = str[8+i];
                        }
                        T.topic[15] = '\0';

                        //printf("(%s,%s,%d)",T.userID,T.topic,T.weight);
                        for(int k=0; k<user_num; ++k)
                        {
                            if(track_user_id[k] == atoi(T.userID))
                            {
                                find_user = k;
                                break;
                            }
                        }
                        //printf("\tfind_user = %d\t",find_user);

                        temp_ptr = temp_ptr + find_user;
                        pthread_mutex_lock(&(temp_ptr->lock));

                        struct tuple *cell = temp_ptr->buffer_ptr; 
                        
                        //printf("buffer ptr =  %p\n", cell);
                        while(temp_ptr->numItems == buffer_size)
                        {
                            //printf("producer waiting on full buffer of of userID = %s\n", cell->userID);
                            pthread_cond_wait(&(temp_ptr->full), &(temp_ptr->lock));
                        }


                        cell[temp_ptr->in].weight = T.weight;
                        strcpy(cell[temp_ptr->in].userID, T.userID);
                        strcpy(cell[temp_ptr->in].topic, T.topic);
                        (temp_ptr->in) = ((temp_ptr->in) + 1) % buffer_size;
                        (temp_ptr->numItems)++;
                        (temp_ptr->numProduced)++;

                        //printf("TID: %d numProduced = %d numItems = %d\n", temp_ptr->myID, temp_ptr->numProduced, temp_ptr->numItems);
                        pthread_cond_signal(&(temp_ptr->empty));

                        pthread_mutex_unlock(&(temp_ptr->lock));
                    }
                }
                else
                {
                    printf("\nSkipping read tuple, Format exception observed");
                }
            }
            else
            {
                printf("\nSkipping read tuple, Format exception observed");
            }
            //printf("\n");
        }

        else if (line == NULL)
        {
            for(int i=0; i<user_num; i++)
            {
                pthread_mutex_lock(&((arr+i)->lock));
                (arr+i)->done = 1;
                pthread_cond_broadcast(&((arr+i)->empty)); 
                pthread_cond_broadcast(&((arr+i)->full));
                pthread_mutex_unlock(&((arr+i)->lock));
            }
            
        }


        for(int j=0; j<user_num; j++)
        {
            pthread_mutex_lock(&((arr+j)->lock));
            if(j==0)
            {
                done_status = arr->done;
            }
            else
            {
                done_status = done_status && (arr+j)->done;
            }
            pthread_mutex_unlock(&((arr+j)->lock));
        }

        if (done_status) 
        {
            //printf("producer is exiting\n");
            //pthread_mutex_unlock(&lock);      
            break;
        }
    }
    /*
    for(int j=0; j<user_num;++j)
    {
        printf("%d\t",track_user_id[j]);
    }
    printf("\n");*/
    pthread_exit((void*) buff);
}


