#include <stdio.h>
#include <stdlib.h>
#include <signal.h>

int main()
{
    sigset_t set;

    sigemptyset(&set);       // make empty signal set
    sigaddset(&set, SIGINT); // add SIGINT in set

    /* SIGINT가 있는지 체크 */
    switch (sigismember(&set, SIGINT))
    {
    case 1:
        printf("SIGINT is included. \n");
        break;
    case 0:
        printf("SIGINT is not included\n");
        break;
    default:
        printf("failed to call sigismember()\n");
    }
    /* SIGSYS가 맴버로 있는지 체크 */
    switch (sigismember(&set, SIGSYS))
    {
    case 1:
        printf("SIGSYS is included. \n");
        break;
    case 0:
        printf("SIGSYS is not included\n");
        break;
    default:
        printf("failed to call sigismember()\n");
    }
    exit(0);
}