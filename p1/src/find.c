#include "../includes/header.h"

char *convert_path(char *path)
{
    char *real_path;

    if ((real_path = realpath(path, NULL)) == NULL)
        return "-1";
    return real_path;
}

char *get_string_time(t_stat st_t, int flag)
{
    char *s;
    struct tm *t;

    s = (char *)malloc(sizeof(char) * 30);
    if (flag == 1)
        t = localtime(&st_t.st_atim);
    else if (flag == 2)
        t = localtime(&st_t.st_ctim);
    else if (flag == 3)
        t = localtime(&st_t.st_mtim);
    // strftime(s, sizeof(s), "%y-%m-%d %H:%M", t);
    sprintf(s, "%02d-%02d-%02d %02d:%02d",
            t->tm_year - 100, t->tm_mon + 1, t->tm_mday,
            t->tm_hour, t->tm_min, t->tm_sec);
    return s;
}

void print_first_line()
{
    printf("Index Size Mode\t\t Blocks Links UID\tGID\tAccess\t\t\tChange\t\t\tModify\t\t\tPath\n");
}

int find_index_zero()
{
    int cnt;
    int i;
    struct dirent **namelist;
    t_stat buf;

    if ((cnt = scandir(g_info.real_filename, &namelist, NULL, alphasort)) == -1)
    {
        index_zero_file();
        return 0;
    }
    else
    {
        // print_first_line();
        for (i = cnt - 1; i >= 0; i--)
        {
            printf("%s\n", namelist[i]->d_name);
        }
    }
    return 1;
}

void index_zero_file()
{
    t_stat zero_file;

    if (stat(g_info.real_filename, &zero_file) == -1)
    {
        printf("enter valid file please\n");
    }
    else
    {
        print_first_line();
        printf("0     %ld  %d\t %ld      %ld     %d\t%d\t%s\t\t%s\t\t%s\t\t%s\n", zero_file.st_size, zero_file.st_mode, zero_file.st_blocks, zero_file.st_nlink, zero_file.st_uid, zero_file.st_gid, get_string_time(zero_file, 1), get_string_time(zero_file, 2), get_string_time(zero_file, 3), g_info.real_filename);
    }
}