// ACADEMIC INTEGRITY PLEDGE
//
// - I have not used source code obtained from another student nor
//   any other unauthorized source, either modified or unmodified.
//
// - All source code and documentation used in my program is either
//   my original work or was derived by me from the source code
//   published in the textbook for this course or presented in
//   class.
//
// - I have not discussed coding details about this project with
//   anyone other than my instructor. I understand that I may discuss
//   the concepts of this program with other students and that another
//   student may help me debug my program so long as neither of us
//   writes anything during the discussion or modifies any computer
//   file during the discussion.
//
// - I have violated neither the spirit nor letter of these restrictions.
//
//
//
// Signed:____Nicholas Gallimore________ Date:___04/19/2019___

// 3460:426 Lab 4A - Basic C shell

/* Basic shell */

/*
 * This is a very minimal shell. It finds an executable in the
 * PATH, then loads it and executes it (using execv). Since
 * it uses "." (dot) as a separator, it cannot handle file
 * names like "minishell.h"
 *
 * The focus on this exercise is to use fork, PATH variables,
 * and execv. 
 */

#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>

#define MAX_ARGS  64
#define MAX_ARG_LEN 16
#define MAX_LINE_LEN  80
#define WHITESPACE  " ,\t\n"

struct command_t {
    char *name;
    int argc;
    char *argv[MAX_ARGS];
    char *linuxCommand;
};

/* Function prototypes */
int parseCommand(char *, struct command_t *);
void printPrompt();
void readCommand(char *);

int main(int argc, char *argv[]) {
    int pid;
    int status;
    char cmdLine[MAX_LINE_LEN];
    struct command_t command;

    while (1) {
        printPrompt();
        /* Read the command line and parse it */
        readCommand(cmdLine);
        parseCommand(cmdLine, &command);
        command.argv[command.argc] = NULL;

        if (strcmp(command.linuxCommand, "Q") == 0)
        {
            break;
        }
        /* Create a child process to execute the command */
        if ((pid = fork()) == 0) {
            /* Child executing command */
            if (strcmp(command.linuxCommand, "H") == 0)
            {
                printf("C file1 file2 Copy; create file2, copy all bytes of file1 to file2 without deleting file1.\nD file Delete the named file.\nE comment Echo; display comment on screen followed by a new line (multiple\nspaces/tabs may be reduced to a single space); if no argument simply\nissue a new prompt.\nH Help; display the user manual.\nL List the contents of the current directory.\nM file Make; create the named text file by launching a text editor.\nP file Print; display the contents of the named file on screen.\nQ Quit the shell.\nS Surf the web by launching a browser as a background process.\nW Wipe; clear the screen.\nX program Execute the named program.\n");
            }
            else if (strcmp(command.linuxCommand, "X") == 0)
            {
                char* program = malloc((strlen(command.argv[1]) + 2) * sizeof(char));
                program[0] = '.';
                program[1] = '/';

                // Create argv without 'X' as argv[0]
                char* pArgv[MAX_ARGS];
                pArgv[argc] = (char *) malloc(MAX_ARG_LEN);
                for (int i = 0; i < command.argc - 1; i++)
                {
                    pArgv[i] = (char *) malloc(MAX_ARG_LEN);
                    strcpy(pArgv[i], command.argv[i + 1]);
                }

                strcat(program, command.argv[1]);
                execvp(program, pArgv);
                exit(0);
            }
            else if (strcmp(command.linuxCommand, "L") == 0)
            {
                if (fork() == 0)
                {
                    execlp("echo", "echo", NULL);
                    exit(0);
                }
                wait(&status);

                if (fork() == 0)
                {
                    execlp("pwd", "pwd", NULL);
                    exit(0);
                }
                wait(&status);
    
                if (fork() == 0)
                {
                    execlp("echo", "echo", NULL);
                    exit(0);
                }
                wait(&status);

                if (fork() == 0)
                {
                    execlp("ls", "ls", "-l", NULL);
                    exit(0);
                }
                wait(&status);
            }
            else
            {
                execvp(command.linuxCommand, command.argv);
            }
            exit(0);
        }
        /* Wait for the child to terminate */
        waitpid(pid, &status, 0);
    }

    /* Shell termination */
    printf("\n\n shell: Terminating successfully\n");
    return 0;
}

/* End basic shell */

/* Parse Command function */

/* Determine command name and construct the parameter list.
 * This function will build argv[] and set the argc value.
 * argc is the number of "tokens" or words on the command line
 * argv[] is an array of strings (pointers to char *). The last
 * element in argv[] must be NULL. As we scan the command line
 * from the left, the first token goes in argv[0], the second in
 * argv[1], and so on. Each time we add a token to argv[],
 * we increment argc.
 */
int parseCommand(char *cLine, struct command_t *cmd) {
    int argc;
    char **clPtr;
    /* Initialization */
    clPtr = &cLine;  /* cLine is the command line */
    argc = 0;
    cmd->argv[argc] = (char *) malloc(MAX_ARG_LEN);
    /* Fill argv[] */
    while ((cmd->argv[argc] = strsep(clPtr, WHITESPACE)) != NULL) {
        cmd->argv[++argc] = (char *) malloc(MAX_ARG_LEN);
    }

    /* Set the command name and argc */
    cmd->argc = argc-1;
    cmd->name = (char *) malloc(sizeof(cmd->argv[0]));
    strcpy(cmd->name, cmd->argv[0]);

    if (strcmp(cmd->name, "C") == 0)
    {
        cmd->linuxCommand = "cp";
    }
    else if (strcmp(cmd->name, "D") == 0)
    {
        cmd->linuxCommand = "rm";
    }
    else if (strcmp(cmd->name, "E") == 0)
    {
        cmd->linuxCommand = "echo";
    }
    else if (strcmp(cmd->name, "M") == 0)
    {
        cmd->linuxCommand = "nano";
    }
    else if (strcmp(cmd->name, "P") == 0)
    {
        cmd->linuxCommand = "more";
    }
    else if (strcmp(cmd->name, "S") == 0)
    {
        cmd->linuxCommand = "firefox";
    }
    else if (strcmp(cmd->name, "W") == 0)
    {
        cmd->linuxCommand = "clear";
    }
    else
    {
        cmd->linuxCommand = (char *) malloc(sizeof(cmd->name));
        cmd->linuxCommand = cmd->name;
    }
    return 1;
}

/* End parseCommand function */

/* Print prompt and read command functions - pp. 79-80 */

void printPrompt() {
    /* Build the prompt string to have the machine name,
     * current directory, or other desired information
     */
    char* const promptString = "linux (nfg3)|>";
    printf("%s ", promptString);
}

void readCommand(char *buffer) {
    /* This code uses any set of I/O functions, such as those in
     * the stdio library to read the entire command line into
     * the buffer. This implementation is greatly simplified,
     * but it does the job.
     */
    fgets(buffer, 80, stdin);
}

/* End printPrompt and readCommand */