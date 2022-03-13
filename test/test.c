#include <stdio.h>

int main()
{
    int n;
    int *ptr;

    ptr = &n; // n의 주소값을 담았다.
    *ptr = 5;
    printf("%d\n", n);
}
