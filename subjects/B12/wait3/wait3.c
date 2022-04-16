#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int main(void)
{
    //자식 프로세서를 생성!
    if (fork() == 0)
        //자식프로세서에 새로운 프로세서를 실행한다.
        // list형식으로 인자를 전달해주는데 bin/echo를 실행시킨다.
        // bin/echo 는 실행파일 위치, 순서대로 echo this is message one이 argv로 들어간다.
        execl("/bin/echo", "echo", "this is", "message one", (char *)0);
    if (fork() == 0)
        execl("/bin/echo", "echo", "this is", "message two", (char *)0);

    printf("parent: waiting for children\n");

    while (wait((int *)0) != -1)
        ;
    printf("parent: all children terminated\n");
    exit(0);
}