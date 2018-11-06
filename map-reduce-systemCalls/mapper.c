//
// Created by kaust on 1/19/2018.
//

#include<stdio.h>
#include<stdlib.h>
#include<string.h>

int str_int(int,int,char*);

struct tuple
{
    char userID[5];
    char action;
    char topic[15];
};


int main()
{
    int weight;
    char str[25],temp[25];
    struct tuple T;
    FILE *infp;
    infp = fopen("input.txt","r");
    if (infp==NULL)
    {
        printf("File not found");
        return -1;
    }

    while(fgets(str,26,infp)!=NULL)
    {
        //printf(str);
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

                    //printf("\n%d",T.userID);
                    T.action = str[6];
                    switch(T.action)
                    {
                        case 'P': weight = 50;
                            break;
                        case 'L': weight = 20;
                            break;
                        case 'D': weight = -10;
                            break;
                        case 'C': weight = 30;
                            break;
                        case 'S': weight = 40;
                            break;
                        default: return -1;

                    }

                    for(int i=0;i<15;i++)
                    {
                        temp[i] = str[8+i];
                    }
                    printf("(%s,%s,%d)",T.userID,temp,weight);
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
        printf("\n");
    }
    return 0;
}


