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
    printf("Enter lines of text, ^D to quit: \n");
    buf.msg_type = 1;

    while (fgets(buf.msg_text, sizeof(buf.msg_text), stdin) != NULL)
    {
        int length = strlen(buf.msg_text);

        if (buf.msg_text[length - 1] == '\n')
            buf.msg_text[length - 1] = '\0';

        //입력받은 문자열을 메세지 큐를 통해 데이터를 보낸다.
        // msgsnd를 통해서 데이터를 보낸다.
        if (msgsnd(msg_queueid, &buf, length + 1, 0) == -1)
        {
            fprintf(stderr, "msgsnd error");
        }
    }
    //메세지 큐를 조작하는 함수
    //사용이 끝난 메세지 큐를 커널에서 제거해준다.
    if (msgctl(msg_queueid, IPC_RMID, NULL) == -1)
    {
        fprintf(stderr, "msgctl error");
        exit(1);
    }
    exit(0);
}