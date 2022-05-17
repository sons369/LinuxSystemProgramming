#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <signal.h>
#include <syslog.h>
#include <sys/stat.h>
#include <sys/types.h>

int ssu_daemon_init(void);
int main(void)
{
    pid_t pid;

    pid = getpid();
    printf("parent process : %d\n", pid);
    printf("daemon process initialization\n");

    if (ssu_daemon_init() < 0)
    {
        fprintf(stderr, "ssu_daemon_init failed\n");
        exit(1);
    }
    exit(0);
}

int ssu_daemon_init(void)
{
    pid_t pid;
    int fd, maxfd;

    if ((pid = fork()) < 0)
    {
        fprintf(stderr, "fork error\n");
        exit(1);
    }
    // 1. 부모 프로세스 종료
    else if (pid != 0)
        exit(0);

    pid = getpid();
    printf("process %d running as daemon\n", pid);
    // 2, 3. 프로세스 그룹 탈퇴 및 새로운 세션 생성
    //  터미널 입출력 시그널 무시
    setsid();
    signal(SIGTTIN, SIG_IGN);
    signal(SIGTTOU, SIG_IGN);
    signal(SIGTSTP, SIG_IGN);
    maxfd = getdtablesize();
    // 6. 오픈된 모든 fd 닫기
    for (fd = 0; fd < maxfd; fd++)
    {
        close(fd);
    }
    // 4. 파일 모드 생성 마스크 해제
    umask(0);
    // 5. 현재 디렉토리를 루트로 변경
    chdir("/");
    // 모든 표준 입력 출력 에러를 /dev/null로 지정
    fd = open("/dev/null", O_RDWR);
    dup(0);
    dup(0);
    return 0;
}