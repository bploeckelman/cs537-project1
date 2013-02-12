#include "structs.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <assert.h>
#include <ctype.h>


void print_line(struct line *line)
{
    assert(line);

    printf("---------------------------\n");
    printf("    Line @ [%p]\n", line);
    printf("---------------------------\n");
    printf("input_str = '%s'\n", line->input_str);
    printf("num sequences = %d\n", line->numSequences);
    printf("ends in plus? = %d\n", line->endsInPlus);
    printf("sequences @ (%p)\n", line->sequences);
    int seq_i, cmd_i, word_i;
    for (seq_i = 0; seq_i < line->numSequences; ++seq_i) {
        const struct sequence *seq = &line->sequences[seq_i];
        printf("  [%d] : '%s' @ (%p)\n", seq_i, seq->seq_str, seq);
        printf("    num cmds = %d\n", seq->numCommands);
        for(cmd_i = 0; cmd_i < seq->numCommands; ++cmd_i) {
            const struct command *cmd = &seq->commands[cmd_i];
            printf("    [%d] : '%s' @ (%p)\n", cmd_i, cmd->cmd_str, cmd);
            printf("      num words = %d\n", cmd->numWords);
            for(word_i = 0; word_i < cmd->numWords; ++word_i) {
                const char *word = cmd->words[word_i];
            printf("      [%d] : '%s' @ (%p)\n", word_i, word, word);
            }
        }
    }
    printf("---------------------------\n");
}

void parse_line(struct line *line, const char *input)
{
    assert(line && input);

    // Save copy of input to line
    line->input_str = strdup(input);

    // Ends in '+'? 
    const char last = *(input + strlen(input));
    line->endsInPlus = (last == '+') ? 1 : 0;

    // Determine the number of sequences in the input line
    int numPluses = 0;
    const char *ch = input;
    while (*ch != '\0') {
        if (*ch++ == '+')
            ++numPluses;
    } 
    line->numSequences = (line->endsInPlus) ? numPluses : numPluses + 1;
#ifdef DEBUG
    printf("Num +: %d  Num seqs: %d\n", numPluses, line->numSequences);
#endif

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
    char *seq = strtok_r(line->input_str, "+", &end_seq);
    while (seq != NULL && i < line->numSequences) {
#ifdef DEBUG
        printf("Sequence[%d] = '%s'\n", i, seq);
#endif
        parse_sequence(&line->sequences[i++], seq);
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
    seq->seq_str = strdup(input);
    cmd = strtok_r(seq->seq_str, ";", &end_cmd);
    while (cmd != NULL && i < numCommands) {
#ifdef DEBUG
        printf("  Command[%d] = '%s'\n", i, cmd);
#endif
        parse_command(&seq->commands[i++], cmd);
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
    const size_t size = sizeof(char *) * (numWords + 1);
    if ((cmd->words = (char **) malloc(size)) == NULL) {
        perror("malloc");
        exit(1);
    }
    memset(cmd->words, 0, size);

    // Parse the words from the input
    int i = 0;
    char *end_word;
    cmd->cmd_str = strdup(input);
    word = strtok_r(cmd->cmd_str, " ", &end_word);
    while (word != NULL && i < numWords) {
#ifdef DEBUG
        printf("    Word[%d] = '%s'\n", i, word);
#endif
        cmd->words[i++] = word;
        word = strtok_r(NULL, " ", &end_word);
    }
    cmd->words[i] = NULL;

    // TODO: look for redirection operator and handle it if found
}

