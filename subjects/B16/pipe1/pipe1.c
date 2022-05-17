#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>

#define BUFFER_SIZE 1024

int main(void)
{
    char buf[BUFFER_SIZE];
    int pipe_fd[2];

    if (pipe(pipe_fd) == -1)
    {
        fprintf(stderr, "pipe error\n");
        exit(1);
    }
    printf("writing to file descriptor #%d\n", pipe_fd[1]);
    write(pipe_fd[1], "OSLAB", 6);
    printf("reading from file descriptor #%d\n", pipe_fd[0]);
    read(pipe_fd[0], buf, 6);
    printf("read \"%s\"\n", buf);
    exit(0);
}