#ifndef _STRUCTS_H_
#define _STRUCTS_H_

struct command {
    char **words;
    char *outputFile;
    int numWords;
};

struct sequence {
    struct command *commands;
    int numCommands;
};

struct line {
    struct sequence *sequences;
    int numSequences;
    int endsInPlus;
};


void parse_line(struct line *line, char *input);
void parse_sequence(struct sequence *seq, char *input);
void parse_command(struct command *cmd, char *input);

#endif

