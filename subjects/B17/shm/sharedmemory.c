#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/types.h>

#define SHARED_MEMORY_SIZE 1024

int main(int argc, char *argv[])
{
    key_t key;
    char *data;
    int shared_memory_id;

    if (argc > 2)
    {
        fprintf(stderr, "usage : %s [data_to_write] \n", argv[0]);
        exit(1);
    }
    //키값 생성
    if ((key = ftok("shmdemo.c", 'R')) == -1)
    {
        fprintf(stderr, "ftok error\n");
        exit(1);
    }
    //공유메모리의 ID를 얻기 위한 함수
    //키를 이용하여 ID를 얻고, 얻을 때 shmflg를 넘긴다.
    if ((shared_memory_id = shmget(key, SHARED_MEMORY_SIZE, 0644 | IPC_CREAT)) == -1)
    {
        fprintf(stderr, "shmget error\n");
        exit(1);
    }

    //생성한 공유 메모리를 프로세스 주소 공간의 일부로 설정해서 접근하기 위한 함수
    // 2번째 인자로 프로세스 어느 주소공간에 공유 메모리를 붇일지 결정
    // 0인 경우엔 자동으로 커널이 찾아 선택한다.
    if ((data = shmat(shared_memory_id, (void *)0, 0)) == (char *)(-1))
    {
        fprintf(stderr, "shmat error\n");
        exit(1);
    }
    if (argc == 2)
    {
        printf("writing to segment: \"%s\" \n", argv[1]);
        strncpy(data, argv[1], SHARED_MEMORY_SIZE);
    }
    else
        printf("segment contains: \"%s\"\n", data);
    // shmdt는 공유 메모리를 프로세스 주소 공간에서 분리시키는 함수이다.
    //제거하기 전에 먼저 분리를 시켜야한다.
    if (shmdt(data) == -1)
    {
        fprintf(stderr, "shmdt error\n");
        exit(1);
    }
    exit(0);
}