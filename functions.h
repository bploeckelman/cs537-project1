#ifndef _FUNCTIONS_H_
#define _FUNCTIONS_H_

#include <stdio.h>

#include "structs.h"


void prompt();
void error();

int get_line(struct line *line, FILE *target_stream);
void free_line(struct line *line);

int  process_line(struct line *line);

int run_sequence(struct sequence *seq);
int run_command(struct command *cmd);
int run_builtin(struct command *cmd);

#endif

