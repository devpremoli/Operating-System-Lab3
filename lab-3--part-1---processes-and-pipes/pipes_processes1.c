//resource :  https://www.geeksforgeeks.org/c-program-demonstrate-fork-and-pipe/
// C program to demonstrate the use of fork() and pipe()
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <string.h>
#include <sys/wait.h>

int main(void)
{
    // We use two pipes
    // First pipe to send an input string from the parent
    // Second pipe to send the new user input from the child to the parent

    int fd1[2];  // Used to store two ends of the first pipe
    int fd2[2];  // Used to store two ends of the second pipe

    char fixed_str[] = "howard.edu";
    char input_str[100];
    pid_t p;

    if (pipe(fd1) == -1)
    {
        fprintf(stderr, "Pipe Failed");
        return 1;
    }
    if (pipe(fd2) == -1)
    {
        fprintf(stderr, "Pipe Failed");
        return 1;
    }
    printf("Other string is: %s\n ",fixed_str);
    printf("Input 1 :");
    scanf("%s", input_str);
    p = fork();

    if (p < 0)
    {
        fprintf(stderr, "fork Failed");
        return 1;
    }

        // Parent process
    else if (p > 0)
    {
        close(fd1[0]);  // Close the reading end of pipe1 and the writing end of pipe2
        close(fd2[1]);

        write(fd1[1], input_str, strlen(input_str) + 1);  // Write the input string to pipe 1

        wait(NULL);        // Wait for the child to send a string
        close(fd1[1]);

        char input_str2[100];
        read(fd2[0], input_str2, 100);
        close(fd2[0]);

        // Concatenating the first input string with "howard.edu" again
        int k = strlen(input_str);
        int i;
        for (i = 0; i < strlen(fixed_str); i++)
            input_str[k++] = fixed_str[i];
        input_str[k] = '\0';

        // Concatenating the second input string with the first concatenated string
        int j = strlen(input_str);
        for (i = 0; i < strlen(input_str2); i++)
            input_str[j++] = input_str2[i];
        input_str[j] = '\0';   // The string ends with '\0'

        // concatenated with "howard.edu"
        printf("Concatenated string %s\n", input_str);
    }

        // Child process
    else
    {
        close(fd1[1]);  // Close the writing end of the first pipe
        close(fd2[0]);  // Close the reading end of the second pipe

        char concat_str[100];
        read(fd1[0], concat_str, 100);   // Read a string using the first pipe

        // Concatenate the fixed string "howard.edu" with the first input string
        int k = strlen(concat_str);
        int i;
        for (i = 0; i < strlen(fixed_str); i++)
            concat_str[k++] = fixed_str[i];

        concat_str[k] = '\0';   // The string ends with '\0'

        printf("Concatenated string %s\n", concat_str);

        // Prompting the user for the second input
        char input_str2[100];
        printf("Input 2: ");
        scanf("%s", input_str2);

        write(fd2[1], input_str2, strlen(input_str2) + 1); // Write to pipe 2

        close(fd1[0]);  // Close the reading end of pipe1 and the writing end of pipe2
        close(fd2[1]);
        exit(0);
    }
}
