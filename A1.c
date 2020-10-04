#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <sys/wait.h>

#define MAX_LINE 80

int pidArray[10];
int a = 1;
int b;
int count=0;

void parsing(char *buffer, char **args){
    while( *buffer != '\0'){
        while(*buffer == ' '){
            *buffer++ = '\0';
        }
        *args++= buffer;
        while(*buffer != '\0' && *buffer !=' '){
            buffer++;
        }
        
    }
    *args = '\0';
}

void running(char **args){
    pid_t pid;
    pid = fork();
    pidArray[a] = getpid();
    a++;
    
    if(pid <0){
        printf("Error");
    }
    else if (pid == 0){
        execvp(*args, args);
    }
    else {
        wait(NULL);
    }
}


int main(void){
    char *args[MAX_LINE/2+1];
    int should_run =1;
    char command[64];
    char buffer[64];
    int i;
    char historyCommand[10][64];
    int x;
    while(should_run){
        
        printf("CSCI3120>");
        fflush(stdout);
        scanf("%s",command);
        for(i=0;i<64;i++){
            buffer[i]=command[i];
        }
        
        parsing(buffer,args);
        //printf("%s", args[0]);
        running(args);
        
        if(strcmp(args[0], "history")==0)
        {
            
            if(count < 1){
                printf("No command in history");
            }
            else
            {
                printf("ID\tPID\tCommand\n");
                for (b=1; b<=count; b++) {
                    printf("%d\t%d\t%s\n",b,pidArray[b],historyCommand[b]);
            }
                
            }
    
        }
        else if (strcmp(args[0], "exit")==0){
            should_run = 0;
        }
        else if (strcmp(args[0],"!!")==0){
            printf("%s",historyCommand[count]);
            parsing(historyCommand[count],args);
            running(args);
            printf("\n");
        }
        else if(strcmp(args[0],"!1")==0){
            parsing(historyCommand[1],args);
            running(args);
            printf("\n");
        }
        else if(strcmp(args[0],"!2")==0){
            parsing(historyCommand[2],args);
            running(args);
            printf("\n");
        }
        else if(strcmp(args[0],"!3")==0){
            parsing(historyCommand[3],args);
            running(args);
            printf("\n");
        }
        else if(strcmp(args[0],"!4")==0){
            parsing(historyCommand[4],args);
            running(args);
            printf("\n");
        }
        else if(strcmp(args[0],"!5")==0){
            parsing(historyCommand[5],args);
            running(args);
            printf("\n");
        }
        else if(strcmp(args[0],"!6")==0){
            parsing(historyCommand[6],args);
            running(args);
            printf("\n");
        }
        else if(strcmp(args[0],"!7")==0){
            parsing(historyCommand[7],args);
            running(args);
            printf("\n");
        }
        else if(strcmp(args[0],"!8")==0){
            parsing(historyCommand[1],args);
            running(args);
            printf("\n");
        }
        else if(strcmp(args[0],"!8")==0){
            parsing(historyCommand[8],args);
            running(args);
            printf("\n");
        }
        else if(strcmp(args[0],"!9")==0){
            parsing(historyCommand[9],args);
            running(args);
            printf("\n");
        }
        else {
            count ++;
            strcpy(historyCommand[count], command);
        }
        
    }
    return 0;
}
