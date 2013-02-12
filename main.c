#include <stdlib.h>
#include <string.h>

#include "functions.h"
#include "structs.h"


const int INCOMPLETE = 0;
const int COMPLETE = 1;


void usage(char *prog)
{
    fprintf(stderr, "Usage: %s [batchFile]\n", prog);
    exit(1);
}



int main(int argc, char *argv[])
{
    // Parse argument
    char *batchFile = NULL;
    if (argc == 2) {
        batchFile = argv[1];
    } else if (argc > 2) {
        usage(argv[0]);
        exit(1);
    }

    // Setup target stream
    FILE *target_stream = NULL;
    if (batchFile != NULL) {
        if ((target_stream = fopen(batchFile, "r")) == NULL) {
            error();
            exit(1);
        }
    } else {
        target_stream = stdin;
    }

    // Main input loop
    int done = INCOMPLETE;
    while (!done) {
        prompt();

        struct line line;
        memset(&line, 0, sizeof(struct line));

        if (get_line(&line, target_stream) == 0)
            done = process_line(&line);
        else
            done = COMPLETE;

        free_line(&line);
    }

    return 0;
}

