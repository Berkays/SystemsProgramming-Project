#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <sys/wait.h>
#include <errno.h>
/*
Author: Berkay Gürosy
*/

//Joins all the strings array into a single string
char *join_strings(char *strings[], int count, int start)
{
    char *newString = NULL; /* Pointer to the joined strings  */
    size_t totalSize = 0;

    //Calculate total size of the new string
    for (int i = start; i < count; i++)
    {
        totalSize += strlen(strings[i]);
        if (strings[i][strlen(strings[i]) - 1] != '\n')
            ++totalSize;
    }
    ++totalSize;

    newString = (char *)malloc(totalSize);
    newString[0] = '\0';

    //Join strings
    size_t length = 0;
    for (int i = start; i < count; i++)
    {
        strcat(newString, strings[i]);
        length = strlen(newString);

        if (newString[length - 1] != '\n')
        {
            newString[length] = ' ';
            newString[length + 1] = '\0';
        }
    }
    return newString;
}

//Joins two strings and returns the appended string
char *append_strings(const char *a, const char *b)
{
    size_t size = 0;

    size += strlen(a);
    size += strlen(b) + 2;

    char *newString = (char *)malloc(size);

    strcpy(newString, a);
    strcat(newString, " ");
    strcat(newString, b);
    return newString;
}

void removeSpecialCharacter(char *str)
{
    int source = 0;
    int destination = 0;

    while (str[source] != '\0')
    {
        if (isalnum(str[source]))
        {
            str[destination] = str[source];
            ++destination;
        }
        else if (isspace(str[source]))
        {
            str[destination] = ' ';
            ++destination;
        }
        ++source;
    }
    str[destination] = '\0';
}

//Returns the first word in the input which is the command name
void parseFunction(const char *input,char *function)
{
    memset(function, '\0', sizeof(function));
    strcpy(function, input);
    strtok(function, "\x20\n");
}

//Splits words in input and returns argument count
int parseTokens(char *input, char *args[])
{
    int i = 0;
    char *a = strtok(input, "\x20\n");
    for (a = strtok(NULL, "\x20\n"); a != NULL; a = strtok(NULL, "\x20\n"))
    {
        args[i] = a;
        i++;
    }
    args[i] = '\0';
    return i;
}

int executeStd(char *function, char *arguments[], int argc)
{
    char *tokens = join_strings(arguments, argc, 0);
    char *command = append_strings(function, tokens);
    printf("\nCommand: %s",command);
    char *cmdargs[] = {
        "/bin/bash",
        "-c",
        command,
        NULL};

    execvp(cmdargs[0], cmdargs);

    free(tokens);
    free(command);

    for (int i = 0; i < argc; i++)
        free(arguments[i]);
}

int checkInput(const char *command)
{
    if (strlen(command) <= 1)
    {
        printf("Empty command. No actions taken.\n");
        return -1;
    }
    else
    {
        return 1;
    }
}

int sysCommand(const char *command)
{
    if (strcmp(command, "clear") == 0)
    {
        system("clear");
    }
    else if (strcmp(command, "exit") == 0)
    {
        printf("Terminated.\n");
        exit(0);
    }
    else
    {
        return -1;
    }

    return 1;
}

int main(int argc, char *argv[])
{
    char input[64];
    char *tokens[32];
    char out[1024];

    printf("Custom Shell\nType 'exit' to quit\n");

    printf(">>: ");
    while (fgets(input, sizeof input, stdin) != NULL)
    {
        //Remove new line character on input
        input[strlen(input) - 1] = '\0';

        if (sysCommand(input) == 1)
            continue;

        if (checkInput(input) == 1)
        {
            char cmd[64];
            int argCount = 0;

            int fd[2];
            pid_t pid;

            int result = 0;

            //Parse function
            parseFunction(input,cmd);

            //Parse arguments
            argCount = parseTokens(input, tokens);

            if (pipe(fd) == -1)
            {
                perror("Fatal error on pipe");
                exit(1);
            }

            pid = fork();

            if (pid == -1)
            {
                perror("Fatal error");
                exit(1);
            }
            else if (pid == 0)
            {
                dup2(fd[0], STDIN_FILENO);
                close(fd[0]);
                close(fd[1]);

                printf("\nOutput:\n");

                result = executeStd(cmd, tokens, argCount);

                perror("Command error");
                exit(1);
            }

            wait(NULL); //Wait for child process to finish

            if (result == -1)
                printf("Command not found.");

        }


        printf(">>:");
    }

    return 0;
}