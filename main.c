#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

const char  *PROMPT     = "537sh% ";
const size_t PROMPT_LEN = 8;
const size_t MAX_INPUT  = 512; // excluding \n and \0
const size_t MAX_ARGS   = 256;

const size_t ERROR_LEN = 23;
const char  *ERROR_MESSAGE = "An error has occurred\n";

const int INCOMPLETE = 0;
const int COMPLETE = 1;


// ---- Get Input -------------------------------------------------------------
void get_input(char *input)
{
    // Read in a line from stdin
    read(STDIN_FILENO, input, MAX_INPUT);
    // Chomp trailing newline
    strtok(input, "\n");
}


// ---- Process Input ---------------------------------------------------------
int process_input(char *input)
{
    // Parse out args
    int  argc = 0;
    char *arg = strtok(input, " ");
    char *args[MAX_ARGS];
    bzero(args, sizeof(args));
    while (arg != NULL && argc < MAX_ARGS) {
        args[argc++] = arg;
        arg = strtok(NULL, " ");
    }
    
    // Echo command
    // TODO: if in 'verbose' mode (./537sh -v)
    write(STDOUT_FILENO, "@----> ", 7); 
    write(STDOUT_FILENO, args[0], strlen(args[0]));
    write(STDOUT_FILENO, "\n", 1); 

    // Handle built-in commands ---------------------------

    // Handle exit --------------------
    if (strcmp(args[0], "exit") == 0) {
        return COMPLETE;
    }
    // Handle pwd ---------------------
    else if (strcmp(args[0], "pwd") == 0) {
        char cwd[1024];
        if (getcwd(cwd, sizeof(cwd)) == NULL) {
            write(STDERR_FILENO, ERROR_MESSAGE, ERROR_LEN);
        } else {
            write(STDOUT_FILENO, cwd, strlen(cwd));
            write(STDOUT_FILENO, "\n", 1); 
        }
    }
    // Handle chdir -------------------
    else if (strcmp(args[0], "cd") == 0) {
        if (argc == 1) {
            // Change to home directory
            const char *home = getenv("HOME");
            if (home == NULL)
                write(STDERR_FILENO, ERROR_MESSAGE, ERROR_LEN);
            else
                chdir(home);
        } else if (argc == 2) {
            // Change to specified directory
            chdir(args[1]);
        } else {
            // TODO: not sure what to do with extra args?
            // ignore?  error?  segfault?
            write(STDOUT_FILENO, "too many args\n", 14);
        }
    }

    // Handle external commands ---------------------------
    else {
        pid_t child_pid = fork();
        if (child_pid == 0) {
            // Run it!
            execvp(args[0], args);
            // Exec returns on error: print error msg, kill child proc
            write(STDERR_FILENO, ERROR_MESSAGE, ERROR_LEN);
            exit(1);
        } else {
            int status;
            waitpid(child_pid, &status, 0);
            // TODO: handle redirection
            // TODO: check status
        }
    }

    return INCOMPLETE;
}


// ---- Main Loop -------------------------------------------------------------
void main_loop()
{
    int  done = INCOMPLETE;
    char input[MAX_INPUT + 1];
    bzero(input, MAX_INPUT + 1);

    while (!done) {
        write(STDOUT_FILENO, PROMPT, PROMPT_LEN);
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

