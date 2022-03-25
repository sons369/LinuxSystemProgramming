#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>

#define BUFFER_SIZE 128

int main(int argc, char *argv[])
{
    char buf[BUFFER_SIZE];
    int fd1, fd2;
    ssize_t size;

    if (argc != 3)
    {
        fprintf(stderr, "error!\n");
        exit(1);
    }
    else
    {
        if ((fd1 = open(argv[1], O_RDONLY)) < 0)
        {
            fprintf(stderr, "open error!\n");
            exit(1);
        }
        else
        {
            size = 0;
            while (read(fd1, &buf[size], 1))
            {
                size++;
            }
        }
        if ((fd2 = creat(argv[2], 0666)) < 0)
        {
            fprintf(stderr, "creat error!\n");
            exit(1);
        }
        else
        {
            write(fd2, buf, size);
        }
    }
    exit(0);
}
