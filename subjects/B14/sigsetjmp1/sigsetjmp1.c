#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <setjmp.h>

void ssu_signal_handler(int signo);

jmp_buf jump_buffer;

int main(void)
{
    // SIGINT 발생시 핸들러 발동
    signal(SIGINT, ssu_signal_handler);

    while (1)
    { // sigsetjmp 대신 setjmp, longjmp 사용
        // longjmp 호출 시 점프 될 위치 설정
        if (setjmp(jump_buffer) == 0)
        {
            printf("Hit Ctrl-c at anytime ... \n");
            pause();
        }
    }
    exit(0);
}

void ssu_signal_handler(int signo)
{
    char character;

    signal(signo, SIG_IGN);
    printf("Did you hit Ctrl-c?\n"
           "Do you really want to quit? [y/n] ");
    character = getchar();

    if (character == 'y' || character == 'Y')
        exit(0);
    else
    { // SIGINT 발생시 핸들러 발동
        signal(SIGINT, ssu_signal_handler);
        // longjmp 실행 => 메인의 setjmp로 이동
        // 1의값 리턴했지만 핸들러 함수는 리턴하지 않아,
        //계속 블럭 된 상태
        longjmp(jump_buffer, 1);
    }
}