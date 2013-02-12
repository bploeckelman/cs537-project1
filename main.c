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
        memset(&line, 0, sizeof(struct line));

        get_line(&line);
        done = process_line(&line);
        free_line(&line);
    }

    return 0;
}

