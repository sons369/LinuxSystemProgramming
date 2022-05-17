#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>

int main()
{
    sigset_t pendingset;
    sigset_t sig_set;
    int count = 0;

    sigfillset(&sig_set);
    sigprocmask(SIG_SETMASK, &sig_set, NULL);

    while (1)
    {
        printf("count: %d\n", count++);
        sleep(1);
        // sigpending을 호출하여 팬딩 중인 집합을 찾고
        // SIGINT가 있는지 검사.
        if (sigpending(&pendingset) == 0)
        {
            if (sigismember(&pendingset, SIGINT))
            {
                //만약 SIGINT가 있으면 무한루프 탈출.
                printf("SIGINT가 블록되어 대기중. 무한 루프를 종료.\n");
                break;
            }
        }
    }
    exit(0);
}