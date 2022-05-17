#include <stdio.h>
#include <stdlib.h>

int main(void)
{
    printf("abort terminate this program\n");
    // abort호출하여 SIGABRT 시그널 발생. => 비정상 종료
    abort();
    printf("this line is never reached\n");
    exit(0);
}