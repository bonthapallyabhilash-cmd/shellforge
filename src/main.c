#define _POSIX_C_SOURCE 200809L

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* Structure to store the command and its arguments */
typedef struct {
    char *args[64];  /* Words entered by the user */
    int count;       /* Number of words */
} Command;

/* Function to parse the input line */
void parse_command(char *line, Command *cmd)
{
    cmd->count = 0;

    char *token = strtok(line, " \t");

    while (token != NULL && cmd->count < 63)
    {
        cmd->args[cmd->count] = token;
        cmd->count++;

        token = strtok(NULL, " \t");
    }

    /* Argument list must end with NULL */
    cmd->args[cmd->count] = NULL;
}

int main(void)
{
    char *line = NULL;
    size_t len = 0;

    Command cmd;

    while (1)
    {
        printf("shellforge$ ");
        fflush(stdout);

        /* Read a line from the user */
        if (getline(&line, &len, stdin) == -1)
        {
            break;
        }

        /* Remove the trailing newline */
        size_t line_length = strlen(line);

        if (line_length > 0 && line[line_length - 1] == '\n')
        {
            line[line_length - 1] = '\0';
        }

        /* Parse the command */
        parse_command(line, &cmd);

        /* Ignore empty input */
        if (cmd.count == 0)
        {
            continue;
        }

        /* Exit command */
        if (strcmp(cmd.args[0], "exit") == 0)
        {
            break;
        }

        /* Display command information */
        printf(
            "Structure Log -> command : %s | Arguments found: %d\n",
            cmd.args[0],
            cmd.count - 1
        );
    }

    /* Free memory allocated by getline */
    free(line);

    return 0;
}
