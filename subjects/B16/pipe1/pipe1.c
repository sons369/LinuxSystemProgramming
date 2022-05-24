#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>

#define BUFFER_SIZE 1024

int main(void)
{
    char buf[BUFFER_SIZE];
    int pipe_fd[2];

    // pipe 함수 호출, pipe_fd를 인자로 줌
    // pipe_fd[0] 은 read용
    // pipe_fd[1] 은 write용으로 반환이 된다.
    if (pipe(pipe_fd) == -1)
    {
        fprintf(stderr, "pipe error\n");
        exit(1);
    }
    printf("writing to file descriptor #%d\n", pipe_fd[1]);
    // pipe1에 OSLAB을 쓴다.
    write(pipe_fd[1], "OSLAB", 6);
    printf("reading from file descriptor #%d\n", pipe_fd[0]);
    // pipe0으로 읽어온다.
    read(pipe_fd[0], buf, 6);
    printf("read \"%s\"\n", buf);
    exit(0);
}