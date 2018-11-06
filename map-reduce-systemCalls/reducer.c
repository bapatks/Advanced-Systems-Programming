#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<math.h>

int str_int(int,int,char*);

struct Node
{
    char userID[5];
    char topic[16];
    int score;
    struct Node *next;
};

int str_int(int start, int end, char *string)
{
    int count = end-start;
    int num = 0;
    char *substring;
    substring = (char*)malloc(count);
    for(int i=0;i<count;i++)
    {
        substring[i] = string[start+i];
    }
    /*
    for (int i=0;i<count;i++)
    {
        num = num + ((substring[i]-48) * pow(10,(count-1-i)));
    }*/
    num = atoi(substring);
    free(substring);
    return num;
}


int main()
{
    char str[26], prev_ID[4];
    int count=0,temp, next_ID_start=0;
    int iterate_nextID = 0, sum=0;
    //FILE *infp;

    struct Node* head = NULL;
    head = (struct Node*)malloc(sizeof(struct Node));

    /*
    infp = fopen("reducer_in.txt","r");
    if (infp==NULL)
    {
        printf("File not found");
        return -1;
    }
    */
    while(fgets(str,28,stdin)!=NULL)
    {
        //printf(str);
        int flag = 0;

        if(str[0]=='(' && (str[24]==')' || str[25]==')'))
        {
            struct Node* new_node = (struct Node*)malloc(sizeof(struct Node));

            if(str[5]==',' && str[21]==',')
            {
                struct Node* current_node = (struct Node*)malloc(sizeof(struct Node));
                //current_node = (struct Node*)malloc(sizeof(struct Node));

                for(int j=0;j<4;j++)
                {
                    new_node->userID[j] = str[1+j];
                }
                new_node->userID[4] = '\0';
                
                for(int i=0;i<15;i++)
                {
                    new_node->topic[i] = str[6+i];
                }
                new_node->topic[15]='\0';

                if(str[22]=='-')
                {
                    temp = str_int(23,25,&str[0]);
                    new_node->score = -temp;
                }
                else
                {
                    new_node->score = str_int(22,24,&str[0]);
                }

                //printf("(%d,%d,%s)\n",new_node->userID,new_node->score,new_node->topic);

                //add a node

                if(count==0)
                {
                    head = new_node;
                    head->next = NULL;
                    strcpy(prev_ID,head->userID);
                    //prev_ID[4] = '\0';
                }

                else if (count>0)
                {
                    current_node = head;
                    flag = 0;
                    while(1)
                    {
                        if(!strcmp((current_node->userID),(new_node->userID)))
                        {
                            if(!strcmp((current_node->topic),(new_node->topic)))
                            {
                                current_node->score += new_node->score;
                                flag = 1;
                                break;
                            }

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

                    if(flag == 0 )
                    {
                        new_node->next = NULL;
                        current_node->next = new_node;
                    }

                }

            }

            if(count>0)
            {
                if(strcmp(prev_ID,new_node->userID))
                {
                    //printf("\n\n\n***** Output\n");
                    while(1)
                    {
                        struct Node* check_node =(struct Node*)malloc(sizeof(struct Node));
                        check_node = head;

                        if(strcmp(check_node->userID, new_node->userID))
                        {

                            
                            printf("(%s,%s,%d)\n",check_node->userID,check_node->topic,check_node->score);
                            head = check_node->next;
                            //check_node->next = NULL;
                            free(check_node);
                        }
                        else
                        {
                            head = check_node;
                            strcpy(prev_ID,head->userID);
                            break;
                        }
                    }

                    //printf("*****\n");


                }
            }
        }
        count++;

    }

    //Code to check traversal of linked list

    struct Node* check_node =(struct Node*)malloc(sizeof(struct Node));
    check_node = head;
    //printf("\n\n\nLinked list\n");
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
    //printf("weird");
    return 0;
}