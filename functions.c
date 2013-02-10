#include "functions.h"
#include "structs.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <assert.h>
#include <ctype.h>

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
    memset(input, 0, MAX_LINE);
    read(STDIN_FILENO, input, MAX_LINE);

    // Chomp trailing newline
    strtok(input, "\n");
    if (strcmp(input, "\n") == 0) {
        input[0] = '\0';
    }

    // Chomp trailing whitespace
    char *end = input + strlen(input) - 1; 
    while (end > input && isspace(*end)) {
        --end;
    }
    *(end + 1) = '\0';
    //printf("Chomped:  '%s'\n", input);

    // Parse input into struct line -----------------------

    // Ends in '+'? 
    const char last = *(input + strlen(input));
    if (last == '+')
        line->endsInPlus = 1;
    else
        line->endsInPlus = 0;

    // Determine the number of sequences in the input line
    int numPluses = 0;
    char *ch = input;
    while (*ch != '\0') {
        if (*ch++ == '+')
            ++numPluses;
    }
    const int numSequences = (line->endsInPlus) ? numPluses : numPluses + 1;
    //printf("Num +: %d  Num seqs: %d\n", numPluses, numSequences);

    // Allocate memory for the sequences in this line
    const size_t size = sizeof(struct sequence) * numSequences;
    if ((line->sequences = malloc(size)) == NULL) {
        perror("malloc");
        exit(1);
    }

    // Parse the sequences from the line
    int i = 0; 
    char *seq = strtok(input, "+");
    while (seq != NULL && i < numSequences) {
        // TODO: parse out each sequence string into a struct command
        line->sequences->str = seq;
        printf("Sequence[%d] = '%s'\n", i++, line->sequences->str);
        seq = strtok(NULL, "+");
    }

    // TODO: temporary cleanup
    free(line->sequences);
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

