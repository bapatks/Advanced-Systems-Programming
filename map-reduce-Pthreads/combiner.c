#include "mapper.h"
#include "reducer.h"


int main(int argc, char* argv[])
{
    void *status;
    pthread_t mapThd;

	if(argc != 4)
    {
        printf("Input format: ./combiner <buffer_size> <number_of_users> <input_file>\n");
        return -1;
    }

    buffer_size = atoi(argv[1]);
    user_num = atoi(argv[2]);
    strcpy(fname, argv[3]);
    pthread_t reduceThd[user_num];
    
    //create an array buff, with each element as a structure (struct control_buff)
    /*For user_num=5 and buffer_size = 10
    	buff = [   struct1   ,   struct2   ,...............,   struct4   ]
                    ||
                    ||
                    \/
        struct => { buffer_ptr, in, out, numItems,......}
    	               ||
    		       ||		
    	     actual    ||		
    	   buffer of   ||
    	   each reducer||
    	   thread      ||
                       \/
    	 =>        [ struct tuple0 , struct tuple1  , struct tuple2................., struct tuple9]....the same for every ptr in buff

    */
    //printf("%d\n", user_num);
    struct buff_ctrl *buff = malloc(user_num * sizeof(struct buff_ctrl)); 
    for(int i=0; i<user_num; i++)
    {
    	/*Initialize each member(struct) of buff_ctrl array 'buff'*/
        buff[i].buffer_ptr = malloc(buffer_size * sizeof *buff[i].buffer_ptr); //points to actual buffer of the comsumer thread
        buff[i].in = 0;
        buff[i].out = 0;
        buff[i].numItems = 0;
        buff[i].numProduced = 0;
        buff[i].numConsumed = 0;
        buff[i].done = 0;
    	//printf("%p\n",&buff[i]);
    }

    //Check i th element of 'buff' array
    /*
    for(int k=0; k<user_num;k++)
    {
        printf("address of element is %p in main\n", &buff[k]);
        printf("in = %d\n", buff[k].in);
        printf("out = %d\n", buff[k].out);
        printf("ptr = %p\n", buff[k].buffer_ptr);
    }
    */
    pthread_create(&mapThd, NULL, mapper, (void*)buff);

    for(int j=0; j<user_num; j++)
    {
        /*Create reducer threads*/
        buff[j].myID = j;
        pthread_create(&reduceThd[j], NULL, reducer, &buff[j]);
        //printf("created reducer thread %d\n",j);
    }

    pthread_join(mapThd, &status);

    
    for(int j=0; j<user_num; j++)
    {
        //Join reducer threads
        pthread_join(reduceThd[j], &status);
        //printf("created reducer thread %d",j);
    }
    /*
    printf("******************************************************\n");
    for(int i=0; i<user_num; i++)
    {
        struct tuple *cell = (buff+i)->buffer_ptr;
        for(int j=0; j<buffer_size; j++)
        {
            printf("| userID = %s, topic = %s, weight = %d|\n",(cell+j)->userID, (cell+j)->topic, (cell+j)->weight);
        }
        printf("numProduced = %d\n", (buff+i)->numProduced);
        printf("numItems = %d\n", (buff+i)->numItems);
        printf("-------\n");
    }
    printf("******************************************************\n");
    */
    //printf("%s\n", status);

    
    free(buff);

    return 0;
}
