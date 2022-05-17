#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>

int main(void)
{
    sigset_t old_set;
    sigset_t sig_set;

    // sig_set 초기화
    sigemptyset(&sig_set);
    // sig_set에 SIGINT 추가
    sigaddset(&sig_set, SIGINT);
    // sig_set을 블록후 기존 집합은 올드셋에 저장
    sigprocmask(SIG_BLOCK, &sig_set, &old_set);
    //블록시킬 시그널을 재설정 및 캐치
    //즉 SIGINT 블록 해제하고 도착할 떄 까지 프로세스 잠시 중단
    sigsuspend(&old_set);
    exit(0);
}