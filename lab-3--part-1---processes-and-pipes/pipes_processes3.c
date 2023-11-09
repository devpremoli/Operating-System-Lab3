#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <string.h>

/**
 * Executes the command "cat scores | grep Lakers | sort".
 */

int main(int argc, char **argv)
{
    if (argc < 2) {
        fprintf(stderr, "Usage: %s <string to grep>\n", argv[0]);
        return 1;
    }

    char* grep_arg = argv[1];
    int pipefd1[2], pipefd2[2];

    // Create two pipes
    if (pipe(pipefd1) == -1 || pipe(pipefd2) == -1) {
        perror("pipe");
        return 1;
    }

    // First child process to handle "cat scores"
    if (fork() == 0) {
        // Replace standard output with the output part of the first pipe
        dup2(pipefd1[1], 1);
        // Close the unused pipe ends
        close(pipefd1[0]);
        close(pipefd1[1]);
        close(pipefd2[0]);
        close(pipefd2[1]);

        // Execute the "cat" command
        execlp("cat", "cat", "scores", NULL);
        perror("execlp cat");
        exit(1);
    }

    // Second child process to handle "grep Lakers"
    if (fork() == 0) {
        // Replace standard input with the input part of the first pipe
        dup2(pipefd1[0], 0);
        // Replace standard output with the output part of the second pipe
        dup2(pipefd2[1], 1);
        // Close the unused pipe ends
        close(pipefd1[0]);
        close(pipefd1[1]);
        close(pipefd2[0]);
        close(pipefd2[1]);

        // Execute the "grep" command
        execlp("grep", "grep", grep_arg, NULL);
        perror("execlp grep");
        exit(1);
    }

    // Third child process to handle "sort"
    if (fork() == 0) {
        // Replace standard input with the input part of the second pipe
        dup2(pipefd2[0], 0);
        // Close the unused pipe ends
        close(pipefd1[0]);
        close(pipefd1[1]);
        close(pipefd2[0]);
        close(pipefd2[1]);

        // Execute the "sort" command
        execlp("sort", "sort", NULL);
        perror("execlp sort");
        exit(1);
    }

    // Close the unused ends of pipes in the parent process
    close(pipefd1[0]);
    close(pipefd1[1]);
    close(pipefd2[0]);
    close(pipefd2[1]);

    // Wait for all child processes to finish
    int status;
    while (wait(&status) > 0);

    return 0;
}

