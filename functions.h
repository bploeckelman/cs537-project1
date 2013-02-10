#ifndef _FUNCTIONS_H_
#define _FUNCTIONS_H_

#include "structs.h"


void prompt();
void error();

void get_line(struct line *line);
int  process_line(struct line *line);

void run_command(struct command *cmd);
void run_sequence(struct sequence *seq);

#endif

