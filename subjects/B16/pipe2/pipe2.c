#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/wait.h>

#define BUFFER_SIZE 1024

int main(void)
{
    char buf[BUFFER_SIZE];
    int pid;
    int pipe_fd[2];

    //파이프 함수 호출
    pipe(pipe_fd);

    // fork를 통해 자식 프로세스 생성
    if ((pid = fork()) < 0)
    {
        fprintf(stderr, "fork error\n");
        exit(1);
    }
    else if (pid > 0)
    {
        //부모 프로세스에서 pipe를 통해서 write를 함.
        printf(" PARENT : writing to the pipe \n");
        write(pipe_fd[1], "OSLAB", 6);
        printf(" PARENT : waiting \n");
        wait(NULL);
    }
    else
    {
        //자식 프로세스에선 pipe0을 통해서 부모프로세스가 쓴 데이터를 읽어옴
        printf(" CHILD : reading from pipe \n");
        read(pipe_fd[0], buf, 6);
        printf(" CHILD : read \"%s\"\n", buf);
        exit(0);
    }
    exit(0);
}