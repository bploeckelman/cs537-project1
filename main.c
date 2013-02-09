#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

const char  *PROMPT     = "537sh% ";
const size_t PROMPT_LEN = 8;
const size_t MAX_INPUT  = 512; // excluding \n and \0

const size_t ERROR_LEN = 23;
const char  *ERROR_MESSAGE = "An error has occurred\n";

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

// ---- Handle Built-in Commands ----------------------------------------------
int handle_builtin(const char *input)
{
    if (strcmp(input, "exit") == 0) {
        return COMPLETE;
    } else if (strcmp(input, "pwd") == 0) {
        char cwd[1024];
        if (getcwd(cwd, sizeof(cwd)) == NULL) {
            write(STDERR_FILENO, ERROR_MESSAGE, ERROR_LEN);
        } else {
            write(STDOUT_FILENO, cwd, strlen(cwd));
            write(STDOUT_FILENO, "\n", 1); 
        }
    } else if (strcmp(input, "cd") == 0) {
        // Change to home directory
        const char *home = getenv("HOME");
        if (home == NULL)
            write(STDERR_FILENO, ERROR_MESSAGE, ERROR_LEN);
        else
            chdir(home);
    } else if (strcmp(input, "cd[spaces+][dir]") == 0) {
        // TODO: this won't actually work... needs moar regex
        write(STDERR_FILENO, ERROR_MESSAGE, ERROR_LEN);
    }

    return INCOMPLETE;
}


// ---- Process Input ---------------------------------------------------------
int process_input(const char *input)
{
    // TODO: tokenize and handle bits individually

    // Echo command
    write(STDOUT_FILENO, input, strlen(input));
    write(STDOUT_FILENO, "\n", 1); 

    // Handle built-in commands
    handle_builtin(input);
    if (strcmp(input, "exit") == 0) {
        return COMPLETE;
    }

    return INCOMPLETE;
}


// ---- Shell Prompt ----------------------------------------------------------
void prompt()
{
    write(STDOUT_FILENO, PROMPT, PROMPT_LEN);
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

