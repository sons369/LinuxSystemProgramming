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
    printf("daemon process initialization\n");

    if (ssu_daemon_init() < 0)
    {
        fprintf(stderr, "ssu_daemon_init failed\n");
        exit(1);
    }
    while (1)
    {
        // syslog를 사용하기 위한 환경 초기화 함수
        //  LOG_PID는 메세지마다 PID 기록
        //  LOG LPR은 라인 프린터 시스템을 의미
        openlog("lpd", LOG_PID, LOG_LPR);
        // /dev/log에 있는 데이터그램 소켓에 메세지를 전달하는 역할
        // 에러상태를 저장해준다. %m은 errno에 따른 에러 문자열로치환
        syslog(LOG_ERR, "open failed lpd %m");
        closelog();
        sleep(5);
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