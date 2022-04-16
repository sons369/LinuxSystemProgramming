#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(void)
{
    char *argv[] = {"ssu_execl_test_1", "param1", "param2", (char *)0};
    printf("this is the original program\n");
    // ssu_execl_test_1 실행 파일을 실행.
    //인자로 백터를 넘겨줌, 차례대로 실행파일 이름, 파람1, 파람2, \0
    execv("./ssu_execl_test_1", argv);
    printf("%s\n", "This line should never get printed\n");
    exit(0);
}