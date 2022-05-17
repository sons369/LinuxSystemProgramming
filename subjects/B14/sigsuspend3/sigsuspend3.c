#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <time.h>

void ssu_signal_handler(int signo);
void ssu_timestamp(char *str);

int main(void)
{
    struct sigaction sig_act;
    sigset_t blk_set;

    //시그널들 blk_set에 추가
    sigfillset(&blk_set);
    // SIGALRM 시그널 삭제
    sigdelset(&blk_set, SIGALRM);
    //핸들러 적용
    sigemptyset(&sig_act.sa_mask);
    sig_act.sa_flags = 0;
    sig_act.sa_handler = ssu_signal_handler;
    sigaction(SIGALRM, &sig_act, NULL);
    ssu_timestamp("before sigsuspend()");
    // SIGALRM 발생
    alarm(5);
    // suspending을 호출하여 잠시 팬딩했다가 시그널 핸들러 실행
    sigsuspend(&blk_set);
    ssu_timestamp("after sigsuspend()");
    exit(0);
}

void ssu_signal_handler(int signo)
{
    printf("in ssu_signal_handler() function\n");
}

void ssu_timestamp(char *str)
{
    time_t time_val;

    time(&time_val);
    printf("%s the time is %s\n", str, ctime(&time_val));
}