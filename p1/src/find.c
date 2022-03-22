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

/* find directory file size use to DFS(with opendir, readdir) */
/* only regular file's size plus and total size return. */
/* if file's type dircectory then skip */
long get_dir_size(char *path)
{
    long size;
    t_stat tmp;
    char buf[BUFF];
    struct dirent *file;
    DIR *dir;

    size = 0;
    chdir(path);
    if ((dir = opendir(path)) != NULL)
    {

        while ((file = readdir(dir)) != NULL)
        {
            if (!strcmp(file->d_name, ".") || !strcmp(file->d_name, ".."))
            {
                continue;
            }

            if (S_ISLNK(tmp.st_mode))
            {
                continue;
            }
            strcpy(buf, path);
            strcat(buf, "/");
            strcat(buf, file->d_name);
            stat(file->d_name, &tmp);
            if (file->d_type == 4)
            {
                realpath(file->d_name, g_dir_path_buf);
                size += get_dir_size(g_dir_path_buf);
            }
            else if (file->d_type == 8)
            {
                size += tmp.st_size;
            }
        }
    }
    closedir(dir);
    chdir("..");
    return size;
}

/* copy information from struct stat to struct myStat */
void get_info_file(t_stat st, t_myStat *myst)
{
    int i;
    long size;
    char **split_file_name;

    i = 0;
    myst->st_mode = st.st_mode;
    if (g_is_zero_dir_flag == 1)
    {
        size = get_dir_size(g_dir_path_buf);
        myst->st_size = size;
    }
    else
        myst->st_size = st.st_size;
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

/* find same size, same name's files */
int find_file()
{
    int cnt;
    int i;
    int select;
    struct dirent **namelist;
    struct dirent **pathlist;
    t_stat buf;
    char buff[BUFF];

    // input filename type is file
    if ((cnt = scandir(g_dir_path_buf, &namelist, filter, alphasort)) == -1)
    {
        index_zero_file();                 // find index zero
        index_same_file(g_info.real_path); // find same file using idx zero file
        if (g_chk_find == 0)
        {
            printf("(None)\n");
        }
        else
        {
            print_node(g_head);
            while (1)
            {
                index_prompt(); // >>
                select = input_index_option(input_function());
                if (select == 0)
                {
                    continue;
                }
                else
                {
                    /* find diff and print */
                    memset_diff_global_variable();
                    make_arr_from_files();
                    find_same_line();
                    verification_arr();
                    make_result_arr();
                    if (g_option[0] == 1 && g_option[1] == 1)
                    {
                        print_diff_option_q();
                        print_diff_option_s();
                        g_option[0] = 0;
                        g_option[1] = 0;
                    }
                    else if (g_option[0] == 1)
                    {
                        print_diff_option_q();
                        g_option[0] = 0;
                    }
                    else if (g_option[1] == 1)
                    {
                        print_diff_option_s();
                        g_option[1] = 0;
                    }
                    else if (!g_option[0] && !g_option[1] && !g_option[2] && !g_option[3])
                    {
                        print_diff_result();
                    }

                    break;
                }
            }
            free_all_node(&g_head);
            g_chk_find = 0;
        }
        return 0;
    }
    // input filename type is dir
    else
    {
        g_is_zero_dir_flag = 1;
        index_zero_file();
        index_same_dir(g_info.real_path);
        if (g_chk_find == 0)
        {
            printf("(None)\n");
        }
        else
        {
            print_node(g_head);
            while (1)
            {
                index_prompt(); // >>
                select = input_index_option(input_function());
                if (select == 0)
                {
                    continue;
                }
                else
                {
                    memset_diff_global_variable();
                    call_dir_diff();
                    break;
                }
            }
        }
        g_chk_find = 0;
        g_is_zero_dir_flag = 0;
        free_all_node(&g_head);
        return 0;
    }
    return 1;
}

/* find index: 0 's file and get infomation. */
/* idx 0 file save in g_zero_file */
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

        memset(&g_zero_file, 0, sizeof(g_zero_file));
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

/* find same size, same name directory from [PATH] */
/* This function use DFS(with opendir, readdir) */
void index_same_dir(char *name)
{
    DIR *dp;
    struct dirent *file;
    t_stat same_file;
    char buf[BUFF];
    long size;
    char **split_file_name;
    char filename[BUFF];
    int i = 0;

    if ((dp = opendir(name)) != NULL)
    {
        chdir(name);
        while ((file = readdir(dp)) != NULL)
        {
            if (!strcmp(file->d_name, ".") || !strcmp(file->d_name, ".."))
                continue;
            strcpy(buf, name);
            strcat(buf, "/");
            strcat(buf, file->d_name);
            stat(buf, &g_tmp);

            split_file_name = ft_split(g_info.real_filename, "/");
            while (split_file_name[i])
            {
                i++;
            }
            i--;
            strcpy(filename, split_file_name[i]);
            i = 0;
            while (split_file_name[i])
            {
                free(split_file_name[i]);
                i++;
            }
            free(split_file_name);
            if (!strcmp(filename, file->d_name))
            {

                strcpy(g_dir_path_buf, buf);
                realpath(name, buf);
                strcpy(g_mtmp.real_path, buf);
                if (file->d_type == 4)
                {
                    size = get_dir_size(g_dir_path_buf);
                    get_info_file(g_tmp, &g_mtmp);
                    g_mtmp.st_size = size;
                }
                if (g_zero_file.st_size == g_mtmp.st_size && strcmp(g_mtmp.real_path, g_zero_file.real_path))
                {
                    insert(&g_head, g_mtmp.real_path);
                    g_chk_find++;
                }
            }
            else if (file->d_type == 4)
            {
                strcpy(buf, name);
                strcat(buf, "/");
                strcat(buf, file->d_name);
                index_same_dir(buf);
                chdir("..");
            }
        }
        closedir(dp);
    }
}

/* Find same size, same name files from [PATH] */
/* This function use DFS(with scandir) */
/* parameter is path */
void index_same_file(char *name)
{
    int cnt;
    struct dirent **namelist;
    t_stat same_file;
    char buf[BUFF];

    if (!realpath(name, buf))
    {
        perror("realpath");
        return;
    }
    /* change work directory */
    if (chdir(buf) < 0)
    {
        perror("idx_same_chdir");
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

            /* if file type, file name are same, path diff, get the file's info */
            if (g_zero_file.st_mode == g_tmp.st_mode && !strcmp(g_zero_file.filename, namelist[cnt]->d_name) && strcmp(name, g_zero_file.real_path))
            {
                strcpy(g_dir_path_buf, buf);
                get_info_file(g_tmp, &g_mtmp);
                realpath(name, buf);
                strcpy(g_mtmp.real_path, buf);
                /*if same size then input linked list node */
                if (strcmp(g_zero_file.filename, g_mtmp.filename) == 0 && g_zero_file.st_size == g_mtmp.st_size)
                {

                    insert(&g_head, g_mtmp.real_path);
                    g_chk_find++;
                }
            }
            /* if namelist[cnt] type dir then DFS */
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

/* scandir()'s filter function */
/* exclude '.' and '..' */
int filter(const struct dirent *info)
{
    char *except;

    strcpy(except, info->d_name);
    if (!strcmp(except, ".") || !strcmp(except, ".."))
        return 0;
    else
        return 1;
}

/* dir diff function */
void call_dir_diff()
{
    t_myStatptr tmp;
    t_stat zero_file;
    t_stat idx_file;
    int zero_cnt;
    int idx_cnt;
    int i;
    int j;
    int m;
    int n;
    int diff_path1;
    int diff_path2;
    struct dirent **zero_namelist;
    struct dirent **idx_namelist;
    char buf[BUFF];
    char buf2[BUFF];
    char **path;
    char **path2;

    tmp = get_idx_node(g_index);

    if ((zero_cnt = scandir(g_zero_file.path_filename, &zero_namelist, filter, alphasort)) == -1)
    {
        perror("zero_cnt");
        return;
    }
    if ((idx_cnt = scandir(tmp->path_filename, &idx_namelist, filter, alphasort)) == -1)
    {
        perror("idx_cnt");
        return;
    }
    if (zero_cnt > 0)
        zero_cnt--;
    else if (zero_cnt <= 0)
        zero_cnt = 0;
    if (idx_cnt > 0)
        idx_cnt--;
    else if (idx_cnt <= 0)
        idx_cnt = 0;

    path = ft_split(g_zero_file.path_filename, "/");
    path2 = ft_split(tmp->path_filename, "/");
    i = 0;
    j = 0;
    m = 0;
    n = 0;
    diff_path1 = 0;
    diff_path2 = 0;
    while (path[diff_path1])
    {
        m++;
        diff_path1++;
    }
    diff_path1--;
    while (path2[diff_path2])
    {
        n++;
        diff_path2++;
    }
    diff_path2--;
    /* 파일 경로 안겹치는 부분까지 반복문 돌리기 */
    while (!strcmp(path[diff_path1], path2[diff_path2]))
    {
        diff_path1--;
        diff_path2--;
    }
    while (i <= zero_cnt && j <= idx_cnt)
    {
        /* strcmp가 음수이면 index 0번 디렉토리에만 있는 파일이므로 i 값만 더해주고 정보 출력 */
        if (strcmp(zero_namelist[i]->d_name, idx_namelist[j]->d_name) < 0)
        {
            printf("Olny in ");
            for (int k = diff_path1; path[k]; k++)
            {
                printf("%s", path[k]);
                if (k != m)
                {
                    printf("/");
                }
            }
            printf(": %s\n", zero_namelist[i]->d_name);
            i++;
        }
        /* strcmp가 양수이면 입력받은 index 디렉토리에만 있는 파일이므로 j 값만 더해주고 정보 출력 */
        else if (strcmp(zero_namelist[i]->d_name, idx_namelist[j]->d_name) > 0)
        {
            printf("Olny in ");
            for (int k = diff_path2; path2[k]; k++)
            {
                printf("%s", path2[k]);
                if (k != n)
                {
                    printf("/");
                }
            }
            printf(": %s\n", idx_namelist[j]->d_name);
            j++;
        }
        /* 이름이 똑같을 때 비교하고 i값과 j값 동시에 증가*/
        else
        {
            strcpy(buf, g_zero_file.path_filename);
            strcat(buf, "/");
            strcat(buf, zero_namelist[i]->d_name);
            stat(buf, &zero_file);
            strcpy(buf2, tmp->path_filename);
            strcat(buf2, "/");
            strcat(buf2, idx_namelist[j]->d_name);
            stat(buf2, &idx_file);
            /* 둘 다 디렉토리일 경우 */
            if (S_ISDIR(zero_file.st_mode) && S_ISDIR(idx_file.st_mode))
            {
                printf("Common subdirectories : ");
                for (int k = diff_path1; path[k]; k++)
                {
                    printf("%s", path[k]);
                    if (k != m)
                    {
                        printf("/");
                    }
                }
                printf("%s and ", zero_namelist[i]->d_name);
                for (int k = diff_path2; path2[k]; k++)
                {
                    printf("%s", path2[k]);
                    if (k != n)
                    {
                        printf("/");
                    }
                }
                printf("%s\n", idx_namelist[j]->d_name);
            }
            /* 하나는 정규파일, 하나는 디렉토리일 경우 */
            else if ((zero_namelist[i]->d_type == 4 && idx_namelist[j]->d_type == 8) || (zero_namelist[i]->d_type == 8 && idx_namelist[j]->d_type == 4))
            {
                printf("File ");
                if (zero_namelist[i]->d_type == 4)
                {
                    for (int k = diff_path1; path[k]; k++)
                    {
                        printf("%s", path[k]);
                        if (k != m)
                        {
                            printf("/");
                        }
                    }
                    printf("%s is a directory ", zero_namelist[i]->d_name);
                }
                else if (zero_namelist[i]->d_type == 8)
                {
                    for (int k = diff_path1; path[k]; k++)
                    {
                        printf("%s", path[k]);
                        if (k != m)
                        {
                            printf("/");
                        }
                    }
                    printf("%s is a regular file ", zero_namelist[i]->d_name);
                }
                printf("while file ");
                if (idx_namelist[j]->d_type == 4)
                {
                    for (int k = diff_path2; path2[k]; k++)
                    {
                        printf("%s", path2[k]);
                        if (k != n)
                        {
                            printf("/");
                        }
                    }
                    printf("%s is a directory\n", idx_namelist[j]->d_name);
                }
                else if (idx_namelist[j]->d_type == 8)
                {
                    for (int k = diff_path2; path2[k]; k++)
                    {
                        printf("%s", path2[k]);
                        if (k != n)
                        {
                            printf("/");
                        }
                    }
                    printf("%s is a regular file\n", idx_namelist[j]->d_name);
                }
            }
            /* 둘 다 정규파일일 경우 diff 출력 */
            if (zero_namelist[i]->d_type == 8 && idx_namelist[j]->d_type == 8)
            {
                int flag = 0;
                memset_diff_global_variable();
                dir_make_arr_from_files(buf, buf2);
                dir_find_same_line(buf, buf2);
                verification_arr();
                make_result_arr();
                for (int q = 0; q < g_diff_result_arr_len; q++)
                {
                    if (g_diff_result_arr[i] != 0)
                    {
                        flag = 1;
                        break;
                    }
                }
                if (flag == 1)
                {
                    printf("diff ");
                    for (int k = diff_path1; path[k]; k++)
                    {
                        printf("%s", path[k]);
                        if (k != m)
                        {
                            printf("/");
                        }
                    }
                    printf("%s ", zero_namelist[i]->d_name);
                    for (int k = diff_path2; path2[k]; k++)
                    {
                        printf("%s", path2[k]);
                        if (k != n)
                        {
                            printf("/");
                        }
                    }
                    printf("%s\n", idx_namelist[j]->d_name);
                }
                flag = 0;
                print_diff_result();
            }
            i++;
            j++;
        }
    }
}