#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <signal.h>

static void ssu_func(int signo);
void ssu_print_mask(const char *str);

int main(void)
{
    sigset_t new_mask, old_mask, wait_mask;
    ssu_print_mask("program start: ");

    if (signal(SIGINT, ssu_func) == SIG_ERR)
    {
        fprintf(stderr, "signal(SIGINT) error\n");
        exit(1);
    }

    // waitmask를 초기화
    sigemptyset(&wait_mask);
    // SIGUSR1 추가
    sigaddset(&wait_mask, SIGUSR1);
    // newmask를 초기화
    sigemptyset(&new_mask);
    // SIGINT 추가
    sigaddset(&new_mask, SIGINT);

    // newmask에 있는 신호 블록 시킨 후 oldmask에 저장
    if (sigprocmask(SIG_BLOCK, &new_mask, &old_mask) < 0)
    {
        fprintf(stderr, "SIG_BLOCK() error\n");
        exit(1);
    }
    ssu_print_mask("in critical region: ");
    // SIGINT 기달리는 중
    if (sigsuspend(&wait_mask) != -1)
    {
        fprintf(stderr, "sigsuspend() error\n");
        exit(1);
    }

    ssu_print_mask("after return from sigsuspend: ");
    // oldmask에 무슨 시그널들이 있는지 확인
    if (sigprocmask(SIG_SETMASK, &old_mask, NULL) < 0)
    {
        fprintf(stderr, "SIG_SETMASK() error\n");
        exit(1);
    }

    ssu_print_mask("program exit: ");
    exit(0);
}

void ssu_print_mask(const char *str)
{
    sigset_t sig_set;
    int err_num;

    err_num = errno;

    if (sigprocmask(0, NULL, &sig_set) < 0)
    {
        fprintf(stderr, "sigprocmask() error\n");
        exit(1);
    }
    printf("%s", str);
    //들어온 시그널 맴버별로 출력
    if (sigismember(&sig_set, SIGINT))
        printf("SIGINT ");
    if (sigismember(&sig_set, SIGQUIT))
        printf("SIGQUIT ");
    if (sigismember(&sig_set, SIGUSR1))
        printf("SIGUSR1 ");
    if (sigismember(&sig_set, SIGALRM))
        printf("SIGALRM ");
    printf("\n");
    errno = err_num;
}

static void ssu_func(int signo)
{
    ssu_print_mask("\nin ssu_func: ");
}