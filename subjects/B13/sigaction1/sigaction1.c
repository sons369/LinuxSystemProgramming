#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>

void ssu_signal_handler(int signo)
{
    printf("ssu_signal_handler control\n");
}

int main()
{
    struct sigaction sig_act;
    sigset_t sig_set;

    sigemptyset(&sig_act.sa_mask);
    sig_act.sa_flags = 0;
    //시그널 핸들러 적용
    sig_act.sa_handler = ssu_signal_handler;
    sigaction(SIGUSR1, &sig_act, NULL);
    printf("before first kill()\n");
    //시그널 발생 시 핸들러 호출
    kill(getpid(), SIGUSR1);
    sigemptyset(&sig_set);
    // SIGSUR1 시그널 추가
    sigaddset(&sig_set, SIGUSR1);
    // SIGUSR1을 블록시킴
    sigprocmask(SIG_SETMASK, &sig_set, NULL);
    printf("before second kill()\n");
    // kill을 통해서 시그널이 블록이 제대로 됐는지 확인
    //핸들러가 실행되지 않음.
    kill(getpid(), SIGUSR1);
    printf("after second kill()\n");
    exit(0);
}