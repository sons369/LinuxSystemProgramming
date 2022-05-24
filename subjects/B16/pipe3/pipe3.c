#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#define BUFFER_SIZE 1024

int main(void)
{
    char buf[BUFFER_SIZE];
    int pid;
    int pipe_fd[2];

    //파이프 함수 호출
    if (pipe(pipe_fd) < 0)
    {
        fprintf(stderr, "pipe error\n");
        exit(1);
    }
    //자식 프로세스 생성
    if ((pid = fork()) < 0)
    {
        fprintf(stderr, "fork error\n");
        exit(1);
    }
    //자식 프로세스 일 경우
    else if (pid == 0)
    { // pipe0 닫기
        close(pipe_fd[0]);

        while (1)
        { //계속 buf에다가 데이터를 씀
            memset(buf, 0x00, BUFFER_SIZE);
            sprintf(buf, "Hello Mother Process. My name is %d\n", getpid());
            write(pipe_fd[1], buf, strlen(buf));
            sleep(1);
        }
    }
    else
    { //부모 프로세스의 경우 pipe1을 닫음
        close(pipe_fd[1]);

        while (1)
        { //그리고 계속 pipe0을 통해서 읽어옴
            memset(buf, 0x00, BUFFER_SIZE);
            read(pipe_fd[0], buf, BUFFER_SIZE);
            fprintf(stderr, "%s", buf);
        }
    }
    exit(0);
}