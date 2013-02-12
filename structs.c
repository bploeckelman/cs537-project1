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
    line->numSequences = (line->endsInPlus) ? numPluses : numPluses + 1;
    //printf("Num +: %d  Num seqs: %d\n", numPluses, numSequences);

    // Allocate memory for the sequences in this line
    const size_t size = sizeof(struct sequence) * line->numSequences;
    if ((line->sequences = (struct sequence *) malloc(size)) == NULL) {
        perror("malloc");
        exit(1);
    }
    memset(line->sequences, 0, size);

    // Parse the sequences from the input, note: uses strtok_r()
    int i = 0; 
    char *end_seq;
    char *seq = strtok_r(input, "+", &end_seq);
    while (seq != NULL && i < line->numSequences) {
#ifdef DEBUG
        printf("Sequence[%d] = '%s'\n", i++, seq);
#endif
        parse_sequence(line->sequences, seq);
        seq = strtok_r(NULL, "+", &end_seq);
    }
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
    seq->numCommands = numCommands;
    free(str);

    // Allocate memory for the commands in this sequence
    const size_t size = sizeof(struct command) * numCommands;
    if ((seq->commands = (struct command *) malloc(size)) == NULL) {
        perror("malloc");
        exit(1);
    }
    memset(seq->commands, 0, size);

    // Parse the commands from the input, note: uses strtok_r()
    int i = 0; 
    char *end_cmd;
    cmd = strtok_r(input, ";", &end_cmd);
    while (cmd != NULL && i < numCommands) {
#ifdef DEBUG
        printf("  Command[%d] = '%s'\n", i++, cmd);
#endif
        parse_command(seq->commands, cmd);
        cmd = strtok_r(NULL, ";", &end_cmd);
    }
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
    cmd->numWords = numWords;
    free(str);

    // Allocate memory for the words in this command
    const size_t size = sizeof(char **) * numWords + 1;
    if ((cmd->words = (char **) malloc(size)) == NULL) {
        perror("malloc");
        exit(1);
    }
    memset(cmd->words, 0, size);

    // Parse the words from the input
    int i = 0;
    char *end_word;
    word = strtok_r(input, " ", &end_word);
    while (word != NULL && i < numWords) {
#ifdef DEBUG
        printf("    Word[%d] = '%s'\n", i, word);
#endif
        cmd->words[i++] = strdup(word);
        word = strtok_r(NULL, " ", &end_word);
    }

    // TODO: look for redirection operator and handle it if found
}

