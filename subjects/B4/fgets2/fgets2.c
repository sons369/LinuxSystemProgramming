#include <stdio.h>
#include <stdlib.h>
#define BUFFER_SIZE 256

int main(void)
{
    char command[BUFFER_SIZE];
    char *prompt = "Prompt>>";

    while (1)
    {
        fputs(prompt, stdout);

        if (fgets(command, sizeof(command), stdin) == NULL)
            break;

        system(command);
    }
    fprintf(stdout, "Good bye...\n");
    fflush(stdout);
    exit(0);
}