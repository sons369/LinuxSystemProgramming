#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{
    extern char **environ;
    char **str;
    int i;

    //넘겨받은 인자들을 출력해줌
    for (i = 0; i < argc; i++)
        printf("argv[%d]: %s\n", i, argv[i]);
    //환경변수들을 출력해줌
    for (str = environ; *str != 0; str++)
        printf("%s\n", *str);
    exit(0);
}