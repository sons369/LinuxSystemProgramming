#include "../includes/header.h"

char *convert_path(char *path)
{
    char *real_path;

    if ((real_path = realpath(path, NULL)) == NULL)
        return "-1";
    return real_path;
}

/* flag == 1 is conver st_atim  to date */
/* flag == 2 is conver st_ctim  to date */
/* flag == 3 is conver st_mtim  to date */
char *get_string_time(t_stat st_t, int flag)
{
    char *s;
    struct tm *t;

    s = (char *)malloc(sizeof(char) * 30);
    if (flag == 1)
        t = localtime(&st_t.st_atim.tv_sec);
    else if (flag == 2)
        t = localtime(&st_t.st_ctim.tv_sec);
    else if (flag == 3)
        t = localtime(&st_t.st_mtim.tv_sec);
    // strftime(s, 30, "%y-%m-%d %H:%M", t);
    sprintf(s, "%02d-%02d-%02d %02d:%02d",
            t->tm_year - 100, t->tm_mon + 1, t->tm_mday,
            t->tm_hour, t->tm_min);
    return s;
}

/* S_IFMT == bit mask for the file type */
/* convert mode_t value to permission symbols */
char *get_permission(mode_t mode)
{
    char *per = "xwr";
    char *permission;
    int i;

    permission = (char *)malloc(sizeof(char) * 11);
    memset(permission, '-', 10);
    if ((mode & S_IFMT) == S_IFDIR)
        permission[0] = 'd';
    for (i = 0; i < 9; i++)
    {
        if (mode & (1 << i))
            permission[10 - i - 1] = per[i % 3];
    }
    permission[10] = '\0';
    return permission;
}

void get_info_file(t_stat st, t_myStat *myst)
{
    int i;
    char **split_file_name;

    i = 0;
    myst->st_size = st.st_size;
    myst->st_mode = st.st_mode;
    myst->st_blocks = st.st_blocks;
    myst->st_nlink = st.st_nlink;
    myst->st_uid = st.st_uid;
    myst->st_gid = st.st_gid;
    myst->st_atim = st.st_atim;
    myst->st_ctim = st.st_ctim;
    myst->st_mtim = st.st_mtim;
    split_file_name = ft_split(g_info.real_filename, "/");
    while (split_file_name[i])
    {
        i++;
    }
    strcpy(myst->filename, split_file_name[--i]);
    strcpy(myst->path_filename, myst->real_path);
    strcat(myst->path_filename, "/");
    strcat(myst->path_filename, myst->filename);
}

void print_first_line()
{
    printf("Index Size   Mode       Blocks Links UID  GID    Access          Change          Modify          Path\n");
}

int find_file()
{
    int cnt;
    int i;
    struct dirent **namelist;
    struct dirent **pathlist;
    t_stat buf;
    char buff[BUFF];

    // input filename type is file
    if ((cnt = scandir(g_info.real_filename, &namelist, filter, alphasort)) == -1)
    {
        index_zero_file();
        index_same_file(g_info.real_path);
        if (g_chk_find == 0)
        {
            printf("(None)\n");
        }
        else
        {
            print_node(g_head);
            index_prompt();
            free_all_node(&g_head);
            g_chk_find = 0;
        }
        return 0;
    }
    // input filename type is dir
    else
    {
        // print_first_line();
        /*for (i = cnt - 1; i >= 0; i--)
        {
            printf("%s\n", namelist[i]->d_name);
        }*/
        index_zero_file();
    }
    return 1;
}

void index_zero_file()
{
    t_stat zero_file;

    if (stat(g_info.real_filename, &zero_file) == -1)
    {
        perror("");
        return;
    }
    else
    {
        char *str;
        str = strdup(g_info.real_filename);
        strcpy(g_zero_file.real_path, dirname(str));
        get_info_file(zero_file, &g_zero_file);
        print_first_line();
        g_zero_file.permission = get_permission(zero_file.st_mode);
        g_zero_file.atim = get_string_time(zero_file, 1);
        g_zero_file.ctim = get_string_time(zero_file, 2);
        g_zero_file.mtim = get_string_time(zero_file, 3);
        print_file(g_zero_file, 0);
        free(str);
    }
}

void index_same_file(char *name)
{
    int cnt;
    struct dirent **namelist;
    t_stat same_file;
    char buf[BUFF];

    if (!realpath(name, buf))
    {
        perror("");
        return;
    }
    if (chdir(buf) < 0)
    {
        perror("");
        return;
    }

    if ((cnt = scandir(name, &namelist, NULL, alphasort)) == -1 & g_chk_find == 0)
    {
        perror("scna");
        g_chk_find = -1;
        return;
    }
    else
    {
        if (cnt > 0)
            cnt--;
        else if (cnt <= 0)
            cnt = 0;
        while (cnt != -1)
        {
            for (int i = 0; i < BUFF; i++)
                buf[i] = 0;
            realpath(namelist[cnt]->d_name, buf);
            // printf("current buf(%p): %s\n", &buf, buf);
            if (!strcmp(namelist[cnt]->d_name, ".") || !strcmp(namelist[cnt]->d_name, ".."))
            {
                free(namelist[cnt]);
                cnt--;
                continue;
            }
            lstat(namelist[cnt]->d_name, &g_tmp);
            if (S_ISLNK(g_tmp.st_mode))
            {
                free(namelist[cnt]);
                cnt--;
                continue;
            }
            // printf("file name: %s\nconvert: %s\nreal_path: %s\nfilepath: %s\n", namelist[cnt]->d_name, buf, g_zero_file.real_path, name);
            //  printf("special: %o\n", S_ISDIR(g_tmp.st_mode));
            if (namelist[cnt]->d_type == 8 && !strcmp(g_zero_file.filename, namelist[cnt]->d_name) && strcmp(name, g_zero_file.real_path))
            {

                get_info_file(g_tmp, &g_mtmp);
                realpath(name, buf);
                strcpy(g_mtmp.real_path, buf);
                if (strcmp(g_zero_file.filename, g_mtmp.filename) == 0 && g_zero_file.st_size == g_mtmp.st_size)
                {
                    insert(&g_head, g_mtmp.real_path);
                    g_chk_find++;
                }
            }
            else if (namelist[cnt]->d_type == 4 && (g_tmp.st_mode & S_ISVTX) != 01000 && (g_tmp.st_mode & S_ISUID) != 04000 && (g_tmp.st_mode & S_ISGID) != 02000)
            {
                index_same_file(buf);
                chdir("..");
            }
            free(namelist[cnt]);
            cnt--;
        }
        free(namelist);
    }
}

int filter(const struct dirent *info)
{
    char *except;

    strcpy(except, info->d_name);
    if (!strcmp(except, ".") || !strcmp(except, ".."))
        return 0;
    else
        return 1;
}