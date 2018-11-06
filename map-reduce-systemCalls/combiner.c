#include<stdlib.h>
#include<unistd.h>
#include<stdio.h>
#include<sys/types.h>


int main(int argc, char* argv[])
{
    /* initialize pipe file descriptors */
    int pipefd[2]; 
    /* Create pipe */                                    
    pipe(pipefd);
    if(argc != 3)
    {
        printf("Input format: ./combiner <mapper path> <reducer path>\n");
        return -1;
    }

    /*Create child process for mapper*/
    switch (fork())
    {
    	case 0:
        //printf("created child 1");
        close(pipefd[0]);
        dup2(pipefd[1], STDOUT_FILENO);
        close(pipefd[1]);
        execvp(argv[1], argv);
        break;

        case -1:
        write(1, "Fork1 resulted in error", 23);
        break;

        default:
        break;
    }

    /*Create child process for reducer*/
    switch (fork()) 
    {
        case 0:             
        close(pipefd[1]);
        dup2(pipefd[0], STDIN_FILENO);
        close(pipefd[0]);
        execvp(argv[2], argv);
        break;

    	case -1:
        write(1, "Fork2 resulted in error", 23);
        break;

    	default:
        break;
    }

	close(pipefd[0]);
	close(pipefd[1]);
	exit(EXIT_SUCCESS);
}
