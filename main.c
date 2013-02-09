#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

const char *PROMPT = "537sh% ";
const size_t PROMPT_SIZE = 8;
const int MAX_INPUT = 512; // excluding \n and \0

const int INCOMPLETE = 0;
const int COMPLETE = 1;


// ---- Get Input -------------------------------------------------------------
void get_input(char *input)
{
    // Read in a line from stdin
    char line[MAX_INPUT + 1];
    bzero(line, MAX_INPUT + 1);
    read(STDIN_FILENO, line, MAX_INPUT);

    // Chomp trailing newline
    size_t i = 0, len = strlen(line);
    for (; i < len; ++i) {
        if (line[i] == '\n')
            break;
        else
            input[i] = line[i];
    }
    input[i] = '\0';
}


// ---- Process Input ---------------------------------------------------------
int process_input(const char *input)
{
    // TODO: tokenize and handle bits individually

    // Echo command
    write(STDOUT_FILENO, input, strlen(input));
    write(STDOUT_FILENO, "\n", 1); 

    // Handle exit command
    if (strcmp(input, "exit") == 0) {
        return COMPLETE;
    }

    return INCOMPLETE;
}


// ---- Shell Prompt ----------------------------------------------------------
void prompt()
{
    write(STDOUT_FILENO, PROMPT, PROMPT_SIZE);
}


// ---- Main Loop -------------------------------------------------------------
void main_loop()
{
    int  done = INCOMPLETE;
    char input[MAX_INPUT + 1];
    bzero(input, MAX_INPUT + 1);

    while (!done) {
        prompt();
        get_input(input);
        done = process_input(input);
    }
}


// ---- Entry -----------------------------------------------------------------
int main(int argc, char *argv[])
{
    main_loop();        
    return 0;
}

