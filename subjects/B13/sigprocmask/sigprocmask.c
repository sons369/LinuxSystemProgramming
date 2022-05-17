#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>

int main()
{
    sigset_t sig_set;
    int count;

    sigemptyset(&sig_set);
    sigaddset(&sig_set, SIGINT); // SIGINT를 맴버로 추가해줌
    /* sig_set에 담겨진 맴버들을 블록 시켜줌 */
    sigprocmask(SIG_BLOCK, &sig_set, NULL);
    /* 해당 카운트 동안 SIGINT를 호출해도 반응이 없음 */
    for (count = 3; 0 < count; count--)
    {
        printf("count %d\n", count);
        sleep(1);
    }
    printf("Ctrl-C에 대한 블록을 해제\n");
    /*블록 마스킹 해제 */
    sigprocmask(SIG_UNBLOCK, &sig_set, NULL);
    printf("count중 Ctrl-C입력하면 이 문장은 출력 되지 않음.\n");
    while (1)
        ;
    exit(0);
}