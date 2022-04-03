#include "../includes/md5_header.h"

int main(int argc, char *argv[])
{
    printf("fmd5 argc : %d\n", argc);
    for (int i = 1; argv[i]; i++)
    {
        printf("%s\n", argv[i]);
    }
    exit(0);
}