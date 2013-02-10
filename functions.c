#include "functions.h"
#include "structs.h"

#include <string.h>
#include <unistd.h>
#include <assert.h>

const char *PROMPT_STR = "537sh% ";
const char ERROR_STR[22] = "An error has occurred\n";

const int   MAX_LINE = 512;


void prompt()
{
    write(STDOUT_FILENO, PROMPT_STR, sizeof(PROMPT_STR));
}

void error()
{
    write(STDERR_FILENO, ERROR_STR, sizeof(ERROR_STR));
}

void get_line(struct line *line)
{
    assert(line);

    // Read in a line from stdin
    char input[MAX_LINE];
    read(STDIN_FILENO, input, MAX_LINE);

    // Chomp trailing newline
    strtok(input, "\n");

    // Handle the case where the input is only a newline (missed by strtok)
    if (strcmp(input, "\n") == 0) {
        input[0] = '\0';
    }

    // TODO: parse input into struct line    
}

int process_line(struct line *line)
{
    assert(line);

    // TODO
    return 0;
}

void run_command(struct command *cmd)
{
    assert(cmd);

    // TODO
}

void run_sequence(struct sequence *seq)
{
    assert(seq);

    // TODO
}

