#include "../includes/header.h"

void prompt()
{
    printf("20182624> ");
}

char *input_function()
{
    char *str;
    if ((str = (char *)malloc(sizeof(char) * BUFF)) == NULL)
        exit(1);
    fgets(str, BUFF, stdin);

    return (str);
}

int ft_case(char *str)
{
    if (str[0] == '\n')
    {
        free(str);
        return 1;
    }

    else if (strncmp(str, "exit", 4) == 0)
    {
        free(str);
        return 2;
    }

    else if (strncmp(str, "find", 4) == 0)
    {
        free(str);
        return 3;
    }

    else
    {
        free(str);
        return 4;
    }
    return 0;
}