#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>

void ssu_check_pending(int signo, char *signame);
void ssu_signal_handler(int signo);

int main()
{
    struct sigaction sig_act;
    sigset_t sig_set;

    sigemptyset(&sig_act.sa_mask);
    sig_act.sa_flags = 0;
    //핸들러 함수 주소 넘겨주기
    sig_act.sa_handler = ssu_signal_handler;

    // sigaction을 통해 SIGUSR1 시그널에 액션 지정
    if (sigaction(SIGUSR1, &sig_act, NULL) != 0)
    {
        fprintf(stderr, "sigaction() error\n");
        exit(1);
    }
    else
    {
        sigemptyset(&sig_set);
        // sig_set에 SIGUSR1 추가
        sigaddset(&sig_set, SIGUSR1);
        //해당 시그널 블록이 안됐을 경우 에러 출력
        if (sigprocmask(SIG_SETMASK, &sig_set, NULL) != 0)
        {
            fprintf(stderr, "sigprocmask() error\n");
            exit(1);
        }
        else
        {
            printf("SIGUSR1 signals are now blocked\n");
            kill(getpid(), SIGUSR1);
            printf("after kill()\n");
            // ssu_check_pending함수를 통해
            //시그널이 블록된 상황인지 확인
            ssu_check_pending(SIGUSR1, "SIGUSR1");
            //시그널 맴버 변수 비워줌
            sigemptyset(&sig_set);
            sigprocmask(SIG_SETMASK, &sig_set, NULL);
            printf("SIGUSR1 signals are no longer blocked\n");
            //재차 블록된 상황인지 확인
            ssu_check_pending(SIGUSR1, "SIGUSR1");
        }
    }
    exit(0);
}

void ssu_check_pending(int signo, char *signame)
{
    sigset_t sig_set;
    if (sigpending(&sig_set) != 0)
        printf("sigpending() error\n");
    else if (sigismember(&sig_set, signo))
        printf("a %s signal is pending\n", signame);
    else
        printf("%s signals are not pending\n", signame);
}

void ssu_signal_handler(int signo)
{
    printf("in ssu_signal_handler function\n");
}