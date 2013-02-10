#ifndef _STRUCTS_H_
#define _STRUCTS_H_

struct command {
    char **words;
    char *outputFile;
};

struct sequence {
    struct command *commands;
};

struct line {
    struct sequence *sequences;
    int endsInPlus;
};

#endif

