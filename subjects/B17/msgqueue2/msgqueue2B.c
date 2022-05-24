#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/types.h>

#define BUFFER_SIZE 1024

struct ssu_msgbuf
{
    char msg_text[BUFFER_SIZE];
    long msg_type;
};

int main(void)
{
    struct ssu_msgbuf buf;
    key_t key;
    int msg_queueid;

    // key_t 타입의 키값 생성
    if ((key = ftok("msgqueue_1A.c", 'B')) == -1)
    {
        fprintf(stderr, "ftok error\n");
        exit(1);
    }
    // 메세지 큐의 ID를 얻는 함수
    // 메세지 큐는 커널에서 관리하는 FIFO형태의 자료구조.
    // key와 msgflg를 인자로 받는다.
    if ((msg_queueid = msgget(key, 0644 | IPC_CREAT)) == -1)
    {
        fprintf(stderr, "msgget error\n");
        exit(1);
    }
    printf("spock: ready to receive message, captain.\n");

    while (1)
    {
        // 1A로부터 메세지를 얻는다.
        if (msgrcv(msg_queueid, &buf, sizeof(buf.msg_text), 0, 0) == -1)
        {
            fprintf(stderr, "msgrcv: Identifier removed\n");
            exit(1);
        }

        printf("spock : \"%s\"\n", buf.msg_text);
    }

    exit(0);
}