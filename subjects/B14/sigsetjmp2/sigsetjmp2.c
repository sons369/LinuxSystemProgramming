#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <setjmp.h>
#include <signal.h>
#include <time.h>

static void ssu_alarm(int signo);
static void ssu_func(int signo);
void ssu_mask(const char *str);

static volatile sig_atomic_t can_jump;
static sigjmp_buf jump_buf;

int main(void)
{
    // sigusr1 핸들러로 ssu_func 적용
    if (signal(SIGUSR1, ssu_func) == SIG_ERR)
    {
        fprintf(stderr, "SIGUSR1 error");
        exit(1);
    }
    // sigalrm 핸들러로 ssu_alarm 적용
    if (signal(SIGALRM, ssu_alarm) == SIG_ERR)
    {
        fprintf(stderr, "SIGALRM error");
        exit(1);
    }
    ssu_mask("starting main: ");
    // 점프될 위치 지정
    if (sigsetjmp(jump_buf, 1))
    {
        ssu_mask("ending main: ");
        exit(0);
    }

    can_jump = 1;
    while (1)
        pause();
    exit(0);
}

void ssu_mask(const char *str)
{
    sigset_t sig_set;
    int err_num;

    err_num = errno;
    if (sigprocmask(0, NULL, &sig_set) < 0)
    {
        printf("sigprocmask() error");
        exit(1);
    }

    printf("%s", str);
    //호출된 시그널에 따라 무슨 시그널인지 출력해주는 부분
    if (sigismember(&sig_set, SIGINT))
        printf("SIGINT ");

    if (sigismember(&sig_set, SIGQUIT))
        printf("SIGQUIT");

    if (sigismember(&sig_set, SIGUSR1))
        printf("SIGUSR1 ");

    if (sigismember(&sig_set, SIGALRM))
        printf("SIGALRM ");

    printf("\n");
    errno = err_num;
}

static void ssu_func(int signo)
{
    time_t start_time;

    if (can_jump == 0)
        return;
    ssu_mask("starting ssu_func : ");
    //알람 시그널 발생
    alarm(3);
    start_time = time(NULL);

    while (1)
    {
        if (time(NULL) > start_time + 5)
            break;
    }
    ssu_mask("ending ssu_func: ");
    can_jump = 0;
    // main에 지정한 위치로 점프
    siglongjmp(jump_buf, 1);
}

static void ssu_alarm(int signo)
{
    ssu_mask("in ssu_alarm: ");
}