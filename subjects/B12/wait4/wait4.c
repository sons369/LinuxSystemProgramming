#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int main(void)
{
    pid_t child1, child2;
    int pid, status;

    //자식 프로세서 생성!
    if ((child1 = fork()) == 0)
        // 경로정보 없이 date란 프로그램을 실행한다.
        // PATH 환경변수에 나열된 디렉토리들에서 해당 실행 파일을 찾아서
        // 실행 가능한 형태 파일이면 실행을,
        // 만일 실행 가능한 파일의 형태가 아니면 "date"를 인자로 bin/sh 실행
        execlp("date", "date", (char *)0);

    if ((child2 = fork()) == 0)
        execlp("who", "who", (char *)0);

    printf("parent: waiting for children\n");

    //자식 프로세서들이 끝난 후 부모 프로세스 출력 수행
    while ((pid = wait(&status)) != -1)
    {
        if (child1 == pid)
            printf("parent: first child: %d\n", (status >> 8));
        else if (child2 == pid)
            printf("parent: second child: %d\n", (status >> 8));
    }
    printf("parent: all children terminated\n");
    exit(0);
}