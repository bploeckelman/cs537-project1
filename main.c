#include <stdlib.h>
#include <string.h>

#include "functions.h"
#include "structs.h"


const int INCOMPLETE = 0;
const int COMPLETE = 1;


int main(int argc, char *argv[])
{
    int done = INCOMPLETE;
    while (!done) {
        prompt();

        struct line line;
        get_line(&line);

        if (line.numSequences > 0) {
            if (line.sequences[0].numCommands > 0) {
                if (line.sequences[0].commands[0].numWords > 0) {
                    if (!strcmp(line.sequences[0].commands[0].words[0], "exit"))
                        done = COMPLETE;
                }
            }
        }

        //done = process_line(line);

        free_line(&line);
    }

    return 0;
}

