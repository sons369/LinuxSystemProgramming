#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/types.h>

#define MAX_RETRIES 8

union ssu_sema_undef
{
    struct semid_ds *buf;
    ushort *array;
    int val;
};

int ssu_initsema(key_t key, int sema_nums);

int main(void)
{
    struct sembuf sema_buf;
    key_t key;
    int sema_id;

    sema_buf.sem_num = 0;
    sema_buf.sem_op = -1;
    sema_buf.sem_flg = SEM_UNDO;

    // key_t 자료형의 키값을 받는 함수
    if ((key = ftok("semademo.c", 'J')) == -1)
    {
        fprintf(stderr, "ftok error\n");
        exit(1);
    }

    if ((sema_id = ssu_initsema(key, 1)) == -1)
    {
        fprintf(stderr, "initsem error\n");
        exit(1);
    }

    printf("Press return to lock: ");
    getchar();
    printf("Trying to lock... \n");
    //세마포어 접근을 제어하는 함수
    //연산은 P()와 V()가 있다.
    // sem_op 맴버값이 음수면 P 양수면 V를 수행.
    if (semop(sema_id, &sema_buf, 1) == -1)
    {
        fprintf(stderr, "semop error\n");
        exit(1);
    }
    printf("Locked.\n");
    printf("Press return to unlock: ");
    getchar();
    sema_buf.sem_op = 1;

    if (semop(sema_id, &sema_buf, 1) == -1)
    {
        fprintf(stderr, "semop error\n");
        exit(1);
    }
    printf("Unlocked \n");
    exit(0);
}

int ssu_initsema(key_t key, int sema_nums)
{
    union ssu_sema_undef sema;
    struct semid_ds buf;
    struct sembuf sema_buf;
    int i;
    int sema_id;

    //세마포어 ID를 얻기위한 함수.
    // P()와 V()만 사용 가능한 변수이다.
    sema_id = semget(key, sema_nums, IPC_CREAT | IPC_EXCL | 0666);

    if (sema_id >= 0)
    {
        sema_buf.sem_op = 1;
        sema_buf.sem_flg = 0;
        sema.val = 1;
        printf("press return \n");
        getchar();

        for (sema_buf.sem_num = 0; sema_buf.sem_num < sema_nums; sema_buf.sem_num++)
        {
            if (semop(sema_id, &sema_buf, 1) == -1)
            {
                int err_num = errno;
                //세마포어를 조작하는 함수
                //몇번째 세마포어를 조작할지 인자를 통해 알려준다.
                //어떤 조작을 할지는 cmd 인자를 통해 결정되는데
                // IPC_RMID는 세마포어를 커널에서 제거하는 동작이다.
                semctl(sema_id, 0, IPC_RMID);
                errno = err_num;
                return -1;
            }
        }
    }
    else if (errno == EEXIST)
    {
        int ready = 0;
        sema_id = semget(key, sema_nums, 0);
        if (sema_id < 0)
            return sema_id;

        sema.buf = &buf;

        for (i = 0; i < MAX_RETRIES && !ready; i++)
        {
            // IPC_STAT는 세마포어를 위한 상태값을 semid_ds에서 얻어서 arg.buf에 저장해준다.
            semctl(sema_id, sema_nums - 1, IPC_STAT, sema);
            if (sema.buf->sem_otime != 0)
                ready = 1;
            else
                sleep(1);
        }
        if (!ready)
        {
            errno = ETIME;
            return -1;
        }
    }
    return sema_id;
}