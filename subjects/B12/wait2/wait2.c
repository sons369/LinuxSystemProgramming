#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

#define EXIT_CODE 1

int main(void)
{
    pid_t pid;
    int ret_val, status;

    //자식 프로세서 생성
    if ((pid = fork()) == 0)
    {
        printf("child: pid = %d ppid = %d exit_code = %d\n", getpid(), getppid(), EXIT_CODE);
        exit(EXIT_CODE);
    }
    printf("parent: waiting for chile = %d\n", pid);
    ret_val = wait(&status);
    printf("parent: return value = %d", ret_val);
    printf(" child's status = %x", status);
    // wait 인자로 넘겨준 status에  exit() 하위 8비트가 들어간다.
    //따라서 비트 연산을 하면 2진수 값인 1을 얻고 10진수로 출력시 1이 출력된다.
    printf(" and shifted = %x\n", (status >> 8));
    exit(0);
}