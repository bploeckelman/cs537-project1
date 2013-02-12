#ifndef _STRUCTS_H_
#define _STRUCTS_H_

#define DEBUG

struct command {
    char *outputFile;
    char *cmd_str;
    char **words;
    int numWords;
};

struct sequence {
    struct command *commands;
    int numCommands;
    char *seq_str;
};

struct line {
    struct sequence *sequences;
    int numSequences;
    int endsInPlus;
    char *input_str;
};

void print_line(struct line *line);

void parse_line(struct line *line, const char *input);
void parse_sequence(struct sequence *seq, char *input);
void parse_command(struct command *cmd, char *input);

#endif

