
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>    
#include <signal.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <errno.h>
// defining input constraints
#define MAXARGS 128
#define MAXLINE 1000
int errno;
// these functions are taken from the course textbook
// defining these functions prototype inorder to use them in each other
void eval(char *command);
int parseline(char *buf, char **argv);
int builtin_command(char **argv);
void eval(char *command)
{ // evaluates the command and parses using parse line
    char *argv[MAXARGS]; 
    char *buf= malloc((strlen(command) + 1) * sizeof(char)); //Stores modified command line
    int background; //Should the job run in background or foreground?
    pid_t pid; //Process id 
    int status=0; // for status
    strcpy(buf, command);
    background = parseline(buf, argv);
    if (argv[0] == NULL)
    {
        return; // to ignore empty command
    }
    
    if (builtin_command(argv)) 
    {
        pid = fork();
        if (pid == 0)
        {   
            //printf("pid:%d status:%d\n", getpid(), 0);
            execv(argv[0], argv);
            //printf("pid:%d status:%d\n", getpid(), 0);
            exit(EXIT_FAILURE);
            
        }
        
        // Parent waits for foreground job to terminate
        if (!background)
        {
            if (waitpid(pid, &status, 0) < 0)
            {
                printf("wait foreground: waitpid error");
                //wait(&status);
            }
            
        }
        else
        {
            printf("%d %s", pid-1, command);
        }
    printf("pid:%d status:%d\n", getpid()+1, 0);
    }
    return;
}
// function to parse the command line and build the argv array
int  parseline(char *buf, char **argv)
{ // to parse line
    char *delim; // first space delimiter pointer
    int argc; // number of arguments
    int background; // background job
    buf[strlen(buf)-1] = ' '; // replace the newline in the end with a space
    while (*buf && (*buf == ' '))// Ignore leading spaces
    {
        buf++;
    } 
    // build the argv list
    argc =0;
    while ((delim = strchr(buf, ' ')))
    {
        argv[argc++] = buf;
        *delim = '\0';
        buf = delim + 1;
        while(*buf && (*buf == ' ')) // Ignore spaces
        {
            buf++;
        }
    }
    argv[argc] = NULL;
    
    if (argc == 0)
    {
        return 0;
    }
    if ((background = (*argv[argc-1] == '&')) !=0 ){
        argv[--argc] = NULL;
    }
    return background;;
}
int builtin_command(char **argv)
{ // for built in commands
    if (strcmp(argv[0], "exit") == 0) // exit command
    {
        exit(1);
    }
    if (strcmp(argv[0], "cd") == 0) // cd command
    {   
        if (argv[1] == NULL)
        {
            printf("Expected argument to change into\n");
            return 0;
        }
        else
        {
            if(chdir(argv[1]) == -1)
	        {
		        printf("No such directory found\n");
                return 0;
	        }
        }
        
    }
    if (strcmp(argv[0], "pwd") == 0) // pwd
    {
        char path[MAXARGS];
        if(getcwd(path,MAXARGS) == NULL)
        {
            printf("Error getting current working directory\n");
            return 0;
        }
        printf("%s\n",path);
        return 0;
    }
         
    return 1;
}
/*void unix_error(char *msg)\
{ // function to give error details for forking
    printf(stderr, "%s: %s\n", msg, strerror(errno));
    exit(0);
}
pid_t Fork(void)
{// adding the error check in the function to call fork
    
    pid_t pid;
    pid = fork();
    if (pid < 0){
        unix_error("Fork Error");
        return pid;
    }
}*/
int main(){
    char command[MAXARGS];
    while(1){
        printf("CS361 >");
        fgets(command, MAXLINE, stdin);
        if (feof(stdin))
        {
            exit(0);
        }
        eval(command);
    }
    
}