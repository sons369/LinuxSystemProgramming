#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(void)
{
    char *argv[] = {"ssu_execl_test_1", "param1", "param2", (char *)0};
    char *env[] = {"NAME = value", "nextname=nextvalue", "HOME=/home/oslab", (char *)0};
    printf("this is the original program\n");
    // v는 백터를 argv로 넘겨주고 e는 환경변수를 의미한다.
    // ssu_execl_test_1를 실행한다.
    execve("./ssu_execl_test_1", argv, env);
    printf("%s\n", "This line should never get printed\n");
    exit(0);
}