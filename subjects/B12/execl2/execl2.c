#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(void)
{
    if (fork() == 0)
    {
        //자식 프로세스 생성 후 bin/echo 실행
        execl("/bin/echo", "echo", "this is", "message one", (char *)0);
        //정상작동시 원래 프로세스를 덮어써버리기 때문에 실행x
        fprintf(stderr, "exec error\n");
        exit(1);
    }
    if (fork() == 0)
    {
        //자식 프로세스 생성 후 bin/echo 실행
        execl("/bin/echo", "echo", "this is", "message two", (char *)0);
        fprintf(stderr, "exec error\n");
        exit(1);
    }
    if (fork() == 0)
    {
        //자식 프로세스 생성 후 bin/echo 실행
        execl("/bin/echo", "echo", "this is", "message three", (char *)0);
        fprintf(stderr, "exec error\n");
        exit(1);
    }
    printf("Parent program ending\n");
    exit(0);
}