#include <stdio.h>
#include <unistd.h>

int main()
{
    long i;
    i = write(1, "bcd", 1);
    printf("byte : %ld", i);
}