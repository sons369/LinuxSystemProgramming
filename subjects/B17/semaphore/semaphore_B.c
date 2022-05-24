#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/types.h>

#if defined(_SEM_SEMUN_UNDEFINED) && _SEM_SEMUN_UNDEFINED

union ssu_sema_undef
{
    struct semid_ds *buf;
    ushort *array;
    int val;
};

#endif

int main(void)
{
    union ssu_sema_undef sema;
    key_t key;
    int sema_id;

    // key_t 자료형의 키값을 받는 함수
    if ((key = ftok("semademo.c", 'J')) == -1)
    {
        fprintf(stderr, "ftok error\n");
        exit(1);
    }
    //사용이 끝난 세마포어를 커널에서 제거한다.
    //한번 더 호출 시 이미 제거됐기에 에러가 호출된다.
    if ((sema_id = semget(key, 1, 0)) == -1)
    {
        fprintf(stderr, "semget error\n");
        exit(1);
    }
    // IPC_RMID는 세마포어를 커널에서 제거하는 동작이다.
    if (semctl(sema_id, 0, IPC_RMID, sema) == -1)
    {
        fprintf(stderr, "semctl error\n");
        exit(1);
    }
    exit(0);
}