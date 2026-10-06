#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

int main(void)
{
    char *line = NULL;
    size_t len = 0;
    char *args[64];

    while (1)
    {
        // Display prompt
        printf("shellforge$ ");
        fflush(stdout);

        // Read command
        if (getline(&line, &len, stdin) == -1)
        {
            printf("\n");
            break;
        }

        // Remove newline
        line[strcspn(line, "\n")] = '\0';

        int i = 0;

        // Split command into arguments
        char *token = strtok(line, " \t");

        while (token != NULL && i < 63)
        {
            args[i++] = token;
            token = strtok(NULL, " \t");
        }

        // execvp() requires NULL at the end
        args[i] = NULL;

        // Empty input
        if (i == 0)
        {
            continue;
        }

        // Exit shell
        if (strcmp(args[0], "exit") == 0)
        {
            break;
        }

        // =========================
        // WEEK 5: cd COMMAND
        // =========================
        if (strcmp(args[0], "cd") == 0)
        {
            if (args[1] == NULL)
            {
                fprintf(stderr, "shellforge: cd: missing path\n");
            }
            else
            {
                if (chdir(args[1]) != 0)
                {
                    perror("shellforge: cd");
                }
            }

            // Do not create a child process for cd
            continue;
        }

        // =========================
        // CREATE CHILD PROCESS
        // =========================
        pid_t pid = fork();

        if (pid < 0)
        {
            // fork() failed
            perror("shellforge: fork");
        }
        else if (pid == 0)
        {
            // Child process

            execvp(args[0], args);

            // execvp() only returns if an error occurs
            perror("shellforge: execution");
            exit(EXIT_FAILURE);
        }
        else
        {
            // Parent process
            waitpid(pid, NULL, 0);
        }
    }

    // Free memory allocated by getline()
    free(line);

    return 0;
}
