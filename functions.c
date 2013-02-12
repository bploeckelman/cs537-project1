#include "functions.h"
#include "structs.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <assert.h>
#include <ctype.h>
#include <sys/types.h>
#include <sys/wait.h>

const char *PROMPT_STR = "537sh% ";
const char ERROR_STR[22] = "An error has occurred\n";

const int   MAX_LINE = 512;


void prompt()
{
#ifdef DEBUG
    printf("(%d) %s", getpid(), PROMPT_STR);
    fflush(stdout);
#else
    write(STDOUT_FILENO, PROMPT_STR, sizeof(PROMPT_STR));
#endif
}

void error()
{
    write(STDERR_FILENO, ERROR_STR, sizeof(ERROR_STR));
}

int get_line(struct line *line, FILE *target_stream)
{
    assert(line);

    // Read in a line from target_stream
    char input[MAX_LINE];
    memset(input, 0, MAX_LINE);
    //read(STDIN_FILENO, input, MAX_LINE);
    if (fgets(input, MAX_LINE, target_stream) == NULL) {
        if (feof(target_stream)) {
            return 1;
        } else {
#ifdef DEBUG
            perror("fgets");
#endif
            error();
            return 0;
        }
    }

    // Chomp trailing newline
    strtok(input, "\n");
    if (strcmp(input, "\n") == 0) {
        input[0] = '\0';
    }

    // Chomp trailing whitespace
    char *end = input + strlen(input) - 1; 
    while (end > input && isspace(*end)) {
        --end;
    }
    *(end + 1) = '\0';
#ifdef DEBUG
    printf("Chomped:  '%s'\n", input);
#endif

    // If in batch mode, echo command back to stdout
    if (target_stream != stdin) {
        write(STDOUT_FILENO, input, strlen(input);
        write(STDOUT_FILENO, "\n", 1):
    }

    // Parse input into struct line
    parse_line(line, input);

#ifdef DEBUG
    print_line(line);
#endif

    return 0;
}

void free_line(struct line *line)
{
    assert(line);

    int seq_i, cmd_i;//, word_i;
    for (seq_i = 0; seq_i < line->numSequences; ++seq_i) {
        struct sequence *seq = &line->sequences[seq_i];
        assert(seq);
        for (cmd_i = 0; cmd_i < seq->numCommands; ++cmd_i) {
            struct command *cmd = &seq->commands[cmd_i];
            assert(cmd);
            free(cmd->cmd_str);
            free(cmd->words);
            //free(cmd);
        }
        free(seq->seq_str);
        free(seq->commands);
        //free(seq);
    }
    free(line->sequences);
    free(line->input_str);

    // Note: line is a stack variable
}

int process_line(struct line *line)
{
    assert(line);

    int seq_i;
    for (seq_i = 0; seq_i < line->numSequences; ++seq_i) {
        struct sequence *seq = &line->sequences[seq_i];
        // Fork here if !(last sequence && line->endsInPlus)
        if (seq_i == line->numSequences - 1 && line->endsInPlus) {
            pid_t child_pid = fork();
            if (child_pid == 0) {
                if (run_sequence(seq) == 1)
                    return 1;
            } else {
                int status;
                if (waitpid(child_pid, &status, 0) == -1) {
                    perror("waitpid");
                    exit(1);
                }
            }
        } else {
            // Just run the sequence in the current process
            if (run_sequence(seq) == 1)
                return 1;
        }
    }

    return 0;
}

int run_sequence(struct sequence *seq)
{
    assert(seq);

    int cmd_i;
    for (cmd_i = 0; cmd_i < seq->numCommands; ++cmd_i) {
        struct command *cmd = &seq->commands[cmd_i];
        if (run_command(cmd) == 1) {
            return 1;
        }
    }

    return 0;
}

int run_command(struct command *cmd)
{
    assert(cmd);

    // Check for and handle built-in commands
    const int result = run_builtin(cmd);

    if (result == -1) {
        // The specified command was not a built-in command...
        pid_t child_pid = fork();
        if (child_pid == 0) {
#ifdef DEBUG
            printf("Child (%d) of (%d)\n", getpid(), getppid());
#endif
            execvp(cmd->words[0], cmd->words);

            // Exec only returns on an error...
            perror("execvp");
            error();
            exit(1);
        } else {
            int status;
            if (waitpid(child_pid, &status, 0) == -1) {
                perror("waitpid");
                exit(1);
            }
            // TODO: handle redirection, check status
#ifdef DEBUG
            printf("(%d) Child return status = %d\n", child_pid, status);
            fflush(stdout);
#endif
        }
    }

    return result;
}

int run_builtin(struct command *cmd)
{
    assert(cmd);

    if (cmd->numWords == 0) {
#ifdef DEBUG
        printf("* Warning : running builtin on command with no words!\n");
#endif
        return 0;
    }

    // Handle exit --------------------
    if (strcmp(cmd->words[0], "exit") == 0) {
        return 1; 
    }
    // Handle pwd ---------------------
    else if (strcmp(cmd->words[0], "pwd") == 0) {
        char cwd[1024];
        if (getcwd(cwd, sizeof(cwd)) == NULL) {
            error();
        } else {
            write(STDOUT_FILENO, cwd, strlen(cwd));
            write(STDOUT_FILENO, "\n", 1); 
        }
    } // Handle chdir -----------------
    else if (strcmp(cmd->words[0], "cd") == 0) {
        if (cmd->numWords == 1) {
            // Change to home directory
            const char *home = getenv("HOME");
            if (home == NULL) {
                error();
            } else if (chdir(home) == -1) {
                perror("cd");
            }
        } else if (cmd->numWords >= 2) {
            // Change to specified directory
            if (chdir(cmd->words[1]) == -1) {
                perror("cd");
            }
        }
    }
    else { // Unhandled --------------- 
        return -1;
    }

    // Handled
    return 0;
}

