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
const size_t MAX_SEQS   = 64;

const size_t ERROR_LEN = 23;
const char  *ERROR_MESSAGE = "An error has occurred\n";

const int INCOMPLETE = 0;
const int COMPLETE = 1;


// ------------------------------------
//               TODO
// ------------------------------------
/*
    - Tabs need to be chomped off of commands
      eg.  % who\t    results in an error
    - Process sequential commands
    - Output redirection
    - Test robustness
*/

// ---- Get Input -------------------------------------------------------------
void get_input(char *input)
{
    // Read in a line from stdin
    read(STDIN_FILENO, input, MAX_INPUT);
    // Chomp trailing newline
    strtok(input, "\n");
}


// ---- Process a Single Sequence of Commands ---------------------------------
int process_sequence(char *sequence)
{
    // Parse out args
    int  argc = 0;
    char *arg = strtok(sequence, " ");
    char *args[MAX_ARGS];
    bzero(args, sizeof(args));
    while (arg != NULL && argc < MAX_ARGS) {
        args[argc++] = arg;
        arg = strtok(NULL, " ");
    }
    
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
        } else if (argc >= 2) {
            // Change to specified directory
            chdir(args[1]);
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


// ---- Process Parallel Sequences of Commands --------------------------------
int process_sequences(char *seqs[], size_t numseqs)
{
    if (seqs == NULL) return INCOMPLETE;

    // For each parallel sequence of commands...
    int i = 0, res;
    for (; i < numseqs; ++i) {
        res = process_sequence(seqs[i]);
    }

    return res;
}


// ---- Process Input ---------------------------------------------------------
int process_input(char *input)
{
    // Parse out parallel sequences of commands and process them 
    int  numseqs = 0;
    char *seq  = strtok(input, "+");
    char *seqs[MAX_SEQS];
    bzero(seqs, sizeof(seqs));
    while (seq != NULL && numseqs < MAX_SEQS) {
        seqs[numseqs++] = seq;
        seq = strtok(NULL, "+");
    }
    int res = process_sequences(seqs, numseqs);

    return res;
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

