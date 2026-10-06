#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

int main(void)
{
    // Pointer used by getline() to store the input line
    char *line = NULL;

    // Size of the allocated input buffer
    size_t len = 0;

    // Array to store command and its arguments
    char *args[64];

    while (1)
    {
        // Display shell prompt
        printf("shellforge$ ");
        fflush(stdout);

        // Read user input
        // Ctrl+D causes getline() to return -1
        if (getline(&line, &len, stdin) == -1)
        {
            printf("\n");
            break;
        }

        // Remove newline character
        line[strcspn(line, "\n")] = '\0';

        int i = 0;

        // Split input into tokens using space and tab
        char *token = strtok(line, " \t");

        while (token != NULL && i < 63)
        {
            args[i++] = token;
            token = strtok(NULL, " \t");
        }

        // Last element must be NULL for execvp()
        args[i] = NULL;

        // If user enters nothing, show prompt again
        if (i == 0)
        {
            continue;
        }

        // Exit command
        if (strcmp(args[0], "exit") == 0)
        {
            break;
        }

        // Create child process
        pid_t pid = fork();

        if (pid < 0)
        {
            // fork() failed
            perror("Fork creation error");
            continue;
        }

        if (pid == 0)
        {
            // Child process

            execvp(args[0], args);

            // execvp() returns only if an error occurs
            perror("Command execution error");
            exit(EXIT_FAILURE);
        }
        else
        {
            // Parent process
            // Wait for child to finish
            if (waitpid(pid, NULL, 0) == -1)
            {
                perror("waitpid error");
            }
        }
    }

    // Free memory allocated by getline()
    free(line);

    return 0;
}
