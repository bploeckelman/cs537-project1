#ifndef _FUNCTIONS_H_
#define _FUNCTIONS_H_

#include "structs.h"


void prompt();
void error();

void get_line(struct line *line);
void free_line(struct line *line);

int  process_line(struct line *line);

int run_sequence(struct sequence *seq);
int run_command(struct command *cmd);
int run_builtin(struct command *cmd);

#endif

