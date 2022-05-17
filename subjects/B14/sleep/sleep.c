#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>

void ssu_timestamp(char *str);

int main(void)
{
    unsigned int ret;

    // sleep 호출 전 시간 출력
    ssu_timestamp("before sleep()");
    // 10초가 지나면 0리턴
    // 10초보다 빨리 리턴되면 남은 초 리턴
    // 10초간 프로세스 실행 지연
    ret = sleep(10);
    // sleep 이후 시간 출력
    ssu_timestamp("after sleep()");
    printf("sleep() returned %d\n", ret);
    exit(0);
}

void ssu_timestamp(char *str)
{
    time_t time_val;

    time(&time_val);
    printf("%s the time is %s\n", str, ctime(&time_val));
}