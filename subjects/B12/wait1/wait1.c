#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

void ssu_echo_exit(int status);

int main(void)
{
    pid_t pid;
    int status;

    // 새로운 자식 프로세서 생성!!
    if ((pid = fork()) < 0)
    {
        fprintf(stderr, "fork error\n");
        exit(1);
    }
    else if (pid == 0)
    {
        exit(7);
    }
    // 자식 프로세서의 종료를 기달리는데,
    // 반환된 PID 값이 자식 프로세서ID와 다르면 에러 출력
    if (wait(&status) != pid)
    {
        fprintf(stderr, "wait error\n");
        exit(1);
    }
    ssu_echo_exit(status);
    exit(0);
}

/* 여러 매크로 함수를 통해서 exit 값 혹은 signal number 등을 출력해줌 */
/* pid == 0 일 때 exit(7) 이므로 정상 종료시 7의 값을 얻을 수 있음 */
void ssu_echo_exit(int status)
{
    if (WIFEXITED(status))
        printf("normal termination, exit status = %d\n", WEXITSTATUS(status));
    else if (WIFSIGNALED(status))
        printf("abnormal termination, signal number = %d%s\n", WTERMSIG(status),
#ifdef WCOREDUMP
               WCOREDUMP(status) ? " (core file generated)" : "");
#else
               "");
#endif
    else if (WIFSTOPPED(status))
        printf("child stopped, signal number = %d\n", WSTOPSIG(status));
}