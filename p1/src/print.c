#include "../includes/header.h"

void prompt()
{
    printf("20182624> ");
}

void index_prompt()
{
    printf(">> ");
}

/* Input Functions from User */
char *input_function()
{
    char *str;
    if ((str = (char *)malloc(sizeof(char) * BUFF)) == NULL)
        exit(1);
    fgets(str, BUFF, stdin);

    return str;
}

/* put file's name and file's path to g_info struct*/
/* put real path to g_info struct */
/* if [FILENAME]'s path and [PATH]'s path aren't available */
/* then print error */
void get_info(char *str[])
{
    int i;
    int fd;
    char buf[BUFF];

    strcpy(g_info.filename, str[1]);
    strcpy(g_info.path, str[2]);
    chdir(g_home_dir);
    if ((fd = open(str[2], O_RDONLY)) < 0 || (fd = open(str[1], O_RDONLY)) < 0)
    {
        perror("open");

        return;
    }
    else
    {
        if (realpath(str[1], buf))
        {
            strcpy(g_info.real_filename, buf);
            strcpy(g_dir_path_buf, buf);
        }

        if (realpath(str[2], buf))
        {

            strcpy(g_info.real_path, buf);
        }
        else
        {
            strcpy(g_info.real_path, str[2]);
        }
    }
    close(fd);
    for (i = 0; str[i]; i++)
    {
        free(str[i]);
    }
    free(str);
}

/* case return according user iput */
/* exit => return 2 */
/* help => return 4 */
/* input enter with noting => return 1 */
/* find [FILENAME] [PATH] => return 3 */
int ft_case(char *str)
{
    char **result;
    int i;
    int j;

    result = ft_split(str, " \n\t");
    j = 0;
    while (result[j])
        j++;
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

/* print file's infomation ex) UID, SIZE, CTIME... */
void print_file(t_myStat file, int i)
{
    printf("%-6d%-7ld%-11s%-7ld%-6ld%-5d%-7d%-16s%-16s%-16s%-2s\n", i, file.st_size, file.permission, file.st_blocks, file.st_nlink, file.st_uid, file.st_gid, file.atim, file.ctim, file.mtim, file.path_filename);
}