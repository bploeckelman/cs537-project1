#include "functions.h"
#include "structs.h"

#include <unistd.h>

const char *PROMPT_STR = "537sh% ";

void prompt()
{
    write(STDOUT_FILENO, PROMPT_STR, sizeof(PROMPT_STR));
}

void get_line(struct line *line)
{
    // TODO 
}

void parse_line(struct line *line)
{
    // TODO
}

void run_command(struct command *cmd)
{
    // TODO
}

void run_sequence(struct sequence *seq)
{
    // TODO
}

