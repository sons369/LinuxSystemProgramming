#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <fcntl.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>

#define BUFFER_SIZE 1024
#define FIFO_NAME "ssu_fifofile"

int main(void)
{
    char buf[BUFFER_SIZE];
    int fd;
    int length;

    //디바이스 파일을 생성할 때 사용
    // S_IFIFO 플래그를 같이 넣어서 FIFO파일을 생성할 수 있다.
    mknod(FIFO_NAME, S_IFIFO | 0666, 0);
    printf("waiting for writers... \n");

    if ((fd = open(FIFO_NAME, O_RDONLY)) < 0)
    {
        fprintf(stderr, "open error for %s\n", FIFO_NAME);
        exit(1);
    }
    printf("got a writer \n");

    do
    {
        // FIFO파일에 쓰여진 데이터를 읽어온다.
        if ((length = read(fd, buf, BUFFER_SIZE)) == -1)
        {
            fprintf(stderr, "read error\n");
            exit(1);
        }
        //몇 바이트를 읽었는지, 무슨 문자를 읽었는지 출력
        else
        {
            buf[length] = '\0';
            printf("tick : read %d bytes : \"%s\"\n", length, buf);
        }
    } while (length > 0);
    exit(0);
}