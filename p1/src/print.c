#include "../includes/header.h"

void prompt()
{
    printf("20182624> ");
}

/* Input Functions from User */
char *input_function()
{
    char *str;
    if ((str = (char *)malloc(sizeof(char) * BUFF)) == NULL)
        exit(1);
    fgets(str, BUFF, stdin);

    return (str);
}

void get_info(char *str[])
{
    int i;

    strcpy(g_info.filename, str[1]);
    strcpy(g_info.path, str[2]);
    for (i = 0; str[i]; i++)
    {
        free(str[i]);
    }
    free(str);
}

int ft_case(char *str)
{
    char **result;
    int i;
    int j;

    result = ft_split(str, " \n");
    j = 0;
    while (result[j])
        j++;
    printf("j is %d\n", j);
    if (str[0] == '\n')
    {
        free(str);
        return 1;
    }

    else if (strcmp(result[0], "exit") == 0 && j == 1)
    {
        free(str);
        return 2;
    }

    else if (strcmp(result[0], "find") == 0 && j == 3)
    {
        get_info(result);
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