#include <stdio.h>
#include <stdlib.h>

#define MAX_LINE 4096

int main(void)
{
    char buf[MAX_LINE];
    char name[L_tmpnam];
    FILE *fp;

    printf("Temporary filename <<%s>>\n", tmpnam(name));

    if ((fp = tmpfile()) == NULL)
    {
        fprintf(stderr, "tmpfile create error!!\n");
        exit(1);
    }

    fputs("create tmpfile success!!\n", fp);
    rewind(fp);
    fgets(buf, sizeof(buf), fp);
    puts(buf);
    exit(0);
}