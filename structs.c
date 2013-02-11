#include "structs.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <assert.h>
#include <ctype.h>

void parse_line(struct line *line, char *input)
{
    assert(line && input);

    // Ends in '+'? 
    const char last = *(input + strlen(input));
    line->endsInPlus = (last == '+') ? 1 : 0;

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

    // Parse the sequences from the input, note: uses strtok_r()
    int i = 0; 
    char *end_seq;
    char *seq = strtok_r(input, "+", &end_seq);
    while (seq != NULL && i < numSequences) {
        printf("Sequence[%d] = '%s'\n", i++, seq);
        parse_sequence(line->sequences, seq);
        seq = strtok_r(NULL, "+", &end_seq);
    }

    // TODO: temporary cleanup
    // for later, make a cleanup(struct line *line) function that frees all the things
    free(line->sequences);
}

void parse_sequence(struct sequence *seq, char *input)
{
    assert(seq && input);
    
    // Determine the number of commands in this sequence 
    int numCommands = 0;
    char *str = strdup(input);
    char *cmd = strtok(str, ";");
    while (cmd != NULL) {
        ++numCommands;
        cmd = strtok(NULL, ";");
    }
    free(str);

    // Allocate memory for the commands in this sequence
    const size_t size = sizeof(struct command) * numCommands;
    if ((seq->commands = malloc(size)) == NULL) {
        perror("malloc");
        exit(1);
    }

    // Parse the commands from the input, note: uses strtok_r()
    int i = 0; 
    char *end_cmd;
    cmd = strtok_r(input, ";", &end_cmd);
    while (cmd != NULL && i < numCommands) {
        printf("  Command[%d] = '%s'\n", i++, cmd);
        parse_command(seq->commands, cmd);
        cmd = strtok_r(NULL, ";", &end_cmd);
    }

    // TODO: temporary cleanup
    free(seq->commands); 
    // for later, make a cleanup(struct line *line) function that frees all the things
}

void parse_command(struct command *cmd, char *input)
{
    assert(cmd && input);
    //printf("Parsing command %p from input '%s'\n", cmd, input);

    // Determine the number of words in this command
    int numWords = 0;
    char *str = strdup(input);
    char *word = strtok(str, " ");
    while (word != NULL) {
        ++numWords;
        word = strtok(NULL, " ");
    }
    free(str);

    // Allocate memory for the words in this command
    const size_t size = sizeof(char *) * numWords;
    if ((cmd->words = malloc(size)) == NULL) {
        perror("malloc");
        exit(1);
    }
    memset(cmd->words, 0, size);

    // Parse the words from the input
    int i = 0;
    char *end_word;
    word = strtok_r(input, " ", &end_word);
    while (word != NULL && i < numWords) {
        printf("    Word[%d] = '%s'\n", i, word);
        cmd->words[i++] = word;
        word = strtok_r(NULL, " ", &end_word);
    }

    // TODO: look for redirection operator and handle it if found

    // TODO: temporary cleanup
    free(cmd->words);
    // for later, make a cleanup(struct line *line) function that frees all the things
}

