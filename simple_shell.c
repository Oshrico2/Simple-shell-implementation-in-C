#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>
#include <stdlib.h>
#define MAX 100
#define MAX_COMMANDS 5

int isFileExist(char* str)
{
    FILE *f;
    f=fopen(str,"r");
    if(f){
        fclose(f);
        return 1;
    }
    else
        return 0;
}

char* isValidCommand(char* command)
{
    char *Path = getenv("PATH");
    char *tempPath=strtok(Path,":");
    char cmd[MAX];
    while(tempPath!=NULL)
    {
        strcpy(cmd,tempPath);
        strcat(cmd,"/");
        strcat(cmd,command);
        if(isFileExist(cmd)){
            printf("\nValid Command: %s\nOutput:\n\n",cmd);
            char* Command_to_Send=(char*)calloc(1,strlen(cmd)+1);
            strcpy(Command_to_Send,cmd);
            return Command_to_Send;
        }
        printf("Invalid Command: %s\n",cmd);
        tempPath=strtok(NULL,":");
    }
    return NULL;
}


int main(int argc, char *argv[])
{
    
    char input[MAX], *tempinput=(char*)calloc(sizeof(char),MAX), *command, *filePath;
    int pid, i = 0;
    char* arr[MAX_COMMANDS];
    while (1) {
        printf("\n$ ");
        fgets(input, MAX, stdin);
        input[strlen(input)-1] = '\0';
        if (strcmp(input, "leave") == 0) {
            printf("Exiting...\n");
            exit(1);
        }
        pid = fork();
        if (pid == 0) {
            i = 0;
            strcpy(tempinput,input);
            char* temp2input=tempinput;
            while (*temp2input) {
                arr[i] = strtok(temp2input, " \t");
                temp2input += strlen(arr[i]) + 1;
                i++;
            }

            filePath = isValidCommand(arr[0]);
            if(filePath == NULL) {
                printf("\nWrong command\n");
                free(tempinput);
                exit(1);
            } else {
                execv(filePath, arr);
            }
        } else {
            wait(NULL);
        }


    }
    return 0;
}