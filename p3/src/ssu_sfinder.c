#include "../includes/ssu_sfinder.h"

int main(void)
{
    char buf[BUFF_SIZE];
    char **split, **split2;
    int argc, argc2;
    pthread_t thid[5];
    uid_t uid = getuid();
    struct passwd *pw = getpwuid(uid);
    strcpy(g_user_name, pw->pw_name);
    // printf("user : %s\n", g_user_name);
    if (!strcmp(g_user_name, "root"))
    {
        char buf[PATH_MAX];
        char buf2[PATH_MAX];
        char buf3[PATH_MAX];
        FILE *fp;
        memset(buf, 0, PATH_MAX);
        strcpy(buf, pw->pw_dir);
        strcat(buf, "/");
        strcpy(buf2, buf);
        strcpy(buf3, buf);
        strcat(buf3, ".duplicate_20182624.log");
        strcpy(g_duplicat_log, buf3);
        if ((fp = fopen(buf3, "a+")) != NULL)
            fclose(fp);

        strcat(buf, ".Trash");
        mkdir(buf, 0775);
        strcat(buf, "/files/");
        strcpy(g_trash_file_path, buf);
        strcat(buf2, ".Trash/info");
        strcpy(g_trash_info_path, buf2);
        mkdir(buf, 0775);
        mkdir(buf2, 0775);
    }
    else
    {
        char buf[PATH_MAX];
        char buf2[PATH_MAX];
        char buf3[PATH_MAX];
        FILE *fp;
        memset(buf, 0, PATH_MAX);
        strcpy(buf, pw->pw_dir);
        strcat(buf, "/");
        strcpy(buf2, buf);
        strcpy(buf3, buf);
        strcat(buf3, ".duplicate_20182624.log");
        strcpy(g_duplicat_log, buf3);
        if ((fp = fopen(buf3, "a+")) != NULL)
            fclose(fp);
        strcat(buf, ".Trash");
        mkdir(buf, 0775);
        strcat(buf, "/files/");
        g_trash_file_path = buf;

        strcat(buf2, ".Trash/info");
        g_trash_info_path = buf2;
        mkdir(buf, 0775);
        mkdir(buf2, 0775);
    }
    q = create_queue_link();
    struct timeval start, end;

    while (1)
    {

        printf("20182624> ");
        fgets(buf, BUFF_SIZE, stdin);
        argc = user_input(buf, &split);
        /*for (int i = 0; i < argc; i++)
        {
            printf("%s\n", split[i]);
        }*/
        if (argc == 0)
        {
            continue;
        }

        else if (!strncmp(split[0], "fmd5", 4))
        {

            g_function = 1;
            if (get_opt(argc, split) && chk_arg_error())
            {
                char *ext;
                convert_file_size(g_min_size, g_max_size);
                if (g_maxsize < g_minsize)
                {
                    printf("size error\n");
                }
                // printf("ext: %s min : %ld max : %ld dir: %s home: %s\n", g_ext_arg, g_minsize, g_maxsize, g_real_path, g_home_dir);
                else
                {
                    gettimeofday(&start, NULL);
                    ext = strrchr(g_ext_arg, '.');
                    // printf("%s\n", ext);
                    enqueue(q, g_real_path);
                    search_same_file(g_real_path, ext);
                    check_same_file();
                    if (g_head != NULL)
                    {
                        print_node(g_head);
                        cnt_set_idx_num(g_head);
                    }
                    else
                    { //동일 파일이 없을 경우
                        printf("No duplicates in %s\n", g_real_path);
                        gettimeofday(&end, NULL);
                        end.tv_sec -= start.tv_sec;
                        if (end.tv_usec < start.tv_usec)
                        {
                            end.tv_sec--;
                            end.tv_usec += 1000000;
                        }
                        end.tv_usec -= start.tv_usec;
                        printf("Searching time: %ld:%06ld(sec:usec)\n", end.tv_sec, end.tv_usec);
                        clear_g_var();
                        continue;
                    }
                    gettimeofday(&end, NULL);
                    end.tv_sec -= start.tv_sec;
                    if (end.tv_usec < start.tv_usec)
                    {
                        end.tv_sec--;
                        end.tv_usec += 1000000;
                    }
                    end.tv_usec -= start.tv_usec;
                    printf("Searching time: %ld:%06ld(sec:usec)\n", end.tv_sec, end.tv_usec);
                    while (1)
                    {
                        if (g_total_node == g_total_set)
                        {
                            free_all_node(&g_head);
                            break;
                        }
                        printf(">> ");
                        fgets(buf, BUFF_SIZE, stdin);
                        argc2 = user_input(buf, &split2);
                        if (argc2 == 0)
                            continue;
                        if (!input_error(argc2, split2))
                        {
                            printf("Input Error!!\n");
                        }
                        else
                        {
                            if (!strcmp(split2[0], "exit"))
                            {
                                if (*split2)
                                {
                                    for (int i = 0; split2[i]; i++)
                                        free(split2[i]);
                                    free(split2);
                                }
                                free_all_node(&g_head);
                                printf(">> Back to Prompt\n");
                                break;
                            }
                            else if (g_del_opt_l == 1 && g_del_opt_d == 1)
                            {
                                option_d(g_del_set, g_del_idx);
                            }
                            else if (g_del_opt_l == 1 && g_del_opt_i == 1)
                            {
                                option_i(g_del_set);
                            }
                            else if (g_del_opt_l == 1 && g_del_opt_f == 1)
                            {
                                option_f_t(g_del_set, 0);
                            }
                            else if (g_del_opt_l == 1 && g_del_opt_t == 1)
                            {
                                option_f_t(g_del_set, 1);
                            }
                            else
                            {
                                printf("error!\n");
                            }
                        }
                        if (*split2)
                        {
                            for (int i = 0; split2[i]; i++)
                                free(split2[i]);
                            free(split2);
                        }
                        print_node(g_head);
                    }
                }
            }
        }
        else if (!strncmp(split[0], "fsha1", 5))
        {
            g_function = 2;
            if (get_opt(argc, split) && chk_arg_error())
            {
                char *ext;
                convert_file_size(g_min_size, g_max_size);
                if (g_maxsize < g_minsize)
                {
                    printf("size error\n");
                }
                // printf("ext: %s min : %ld max : %ld dir: %s home: %s\n", g_ext_arg, g_minsize, g_maxsize, g_real_path, g_home_dir);
                else
                {
                    gettimeofday(&start, NULL);

                    ext = strrchr(g_ext_arg, '.');
                    enqueue(q, g_real_path);
                    search_same_file(g_real_path, ext);
                    check_same_file();
                    if (g_head != NULL)
                    {
                        print_node(g_head);
                        cnt_set_idx_num(g_head);
                    }
                    else
                    { //동일 파일이 없을 경우
                        printf("No duplicates in %s\n", g_real_path);
                        gettimeofday(&end, NULL);
                        end.tv_sec -= start.tv_sec;
                        if (end.tv_usec < start.tv_usec)
                        {
                            end.tv_sec--;
                            end.tv_usec += 1000000;
                        }
                        end.tv_usec -= start.tv_usec;
                        printf("Searching time: %ld:%06ld(sec:usec)\n", end.tv_sec, end.tv_usec);
                        clear_g_var();
                        continue;
                    }
                    gettimeofday(&end, NULL);
                    end.tv_sec -= start.tv_sec;
                    if (end.tv_usec < start.tv_usec)
                    {
                        end.tv_sec--;
                        end.tv_usec += 1000000;
                    }
                    end.tv_usec -= start.tv_usec;
                    printf("Searching time: %ld:%06ld(sec:usec)\n", end.tv_sec, end.tv_usec);
                    while (1)
                    {
                        if (g_total_node == g_total_set)
                        {
                            free_all_node(&g_head);
                            break;
                        }
                        printf(">> ");
                        fgets(buf, BUFF_SIZE, stdin);
                        argc2 = user_input(buf, &split2);
                        if (argc2 == 0)
                            continue;
                        if (!input_error(argc2, split2))
                        {
                            printf("Input Error!!\n");
                        }
                        else
                        {
                            if (!strcmp(split2[0], "exit"))
                            {
                                if (*split2)
                                {
                                    for (int i = 0; split2[i]; i++)
                                        free(split2[i]);
                                    free(split2);
                                }
                                free_all_node(&g_head);
                                printf(">> Back to Prompt\n");
                                break;
                            }
                            else if (g_del_opt_l == 1 && g_del_opt_d == 1)
                            {
                                option_d(g_del_set, g_del_idx);
                            }
                            else if (g_del_opt_l == 1 && g_del_opt_i == 1)
                            {
                                option_i(g_del_set);
                            }
                            else if (g_del_opt_l == 1 && g_del_opt_f == 1)
                            {
                                option_f_t(g_del_set, 0);
                            }
                            else if (g_del_opt_l == 1 && g_del_opt_t == 1)
                            {
                                option_f_t(g_del_set, 1);
                            }
                            else
                            {
                                printf("error!\n");
                            }
                        }
                        if (*split2)
                        {
                            for (int i = 0; split2[i]; i++)
                                free(split2[i]);
                            free(split2);
                        }
                        print_node(g_head);
                    }
                }
            }
        }
        else if (!strncmp(split[0], "list", 4))
        {
            g_function = 3;
            if (get_opt(argc, split))
            {
            }
        }
        else if (!strncmp(split[0], "trash", 5))
        {
            g_function = 4;
            if (get_opt(argc, split))
            {
            }
        }
        else if (!strncmp(split[0], "restore", 7))
        {
        }
        else if (!strncmp(split[0], "exit", 4))
        {
            printf("Prompt End\n");
            break;
        }
        else
        {
            print_help();
        }
        // printf("fun: %d ext : %s min : %s max : %s dest : %s t : %d\n", g_function, g_ext_arg, g_min_size, g_max_size, g_dir_arg, g_thread);
        clear_g_var();
        if (*split)
        {
            for (int i = 0; split[i]; i++)
                free(split[i]);
            free(split);
        }
    }
}

void clear_g_var()
{
    memset(g_dir_arg, 0, PATH_MAX);
    memset(g_ext_arg, 0, BUFF_SIZE);
    memset(g_max_size, 0, BUFF_SIZE);
    memset(g_min_size, 0, BUFF_SIZE);
    memset(g_opt_c_arg, 0, BUFF_SIZE);
    memset(g_opt_l_arg, 0, BUFF_SIZE);
    memset(g_real_path, 0, PATH_MAX);
    g_opt_o_arg = 0;
    g_thread = 0;
    g_function = 0;
    g_minsize = 0;
    g_maxsize = 0;
    g_file = 0;
    g_cnt_same_file = 0;
    optind = 0;
}

int user_input(char buf[], char ***split)
{
    int argc;

    argc = 0;
    *split = ft_split(buf, " \n");
    while ((*split)[argc])
        argc++;
    return argc;
}

int get_opt(int argc, char *argv[])
{
    int opt;
    int is_e = 0;
    int is_l = 0; // list, fmd5, fsha1
    int is_h = 0;
    int is_d = 0;
    int is_t = 0;
    int is_c = 0; // list, trash
    int is_o = 0; // list, trash

    if (g_function == 1 || g_function == 2)
    {
        while ((opt = getopt(argc, argv, "e:l:h:d:t:")) != -1)
        {
            switch (opt)
            {
            case 'e':
                is_e = 1;
                strcpy(g_ext_arg, optarg);
                break;
            case 'l':
                is_l = 1;
                strcpy(g_min_size, optarg);
                break;
            case 'h':
                is_h = 1;
                strcpy(g_max_size, optarg);
                break;
            case 'd':
                is_d = 1;
                strcpy(g_dir_arg, optarg);
                break;
            case 't':
                is_t = 1;
                g_thread = atoi(optarg);
                break;
            case '?':
                break;
            }
        }
        if (!is_t)
            g_thread = 1;
        if (!is_e || !is_l || !is_h || !is_d)
            return 0;
    }
    else if (g_function == 3)
    {
        while ((opt = getopt(argc, argv, "l:c:o:")) != -1)
        {
            switch (opt)
            {
            case 'l':
                is_l = 1;
                strcpy(g_opt_l_arg, optarg);
                break;
            case 'c':
                is_c = 1;
                strcpy(g_opt_c_arg, optarg);
                break;
            case 'o':
                is_o = 1;
                g_opt_o_arg = atoi(optarg);
                break;
            case '?':
                break;
            }
        }
        if (!is_l)
            strcpy(g_opt_l_arg, "fileset");
        if (!is_c)
            strcpy(g_opt_c_arg, "size");
        if (!is_o)
            g_opt_o_arg = 1;
    }
    else if (g_function == 4)
    {
        while ((opt = getopt(argc, argv, "c:o:")) != -1)
        {
            while ((opt = getopt(argc, argv, "l:c:o:")) != -1)
            {
                switch (opt)
                {
                case 'c':
                    is_c = 1;
                    strcpy(g_opt_c_arg, optarg);
                    break;
                case 'o':
                    is_o = 1;
                    g_opt_o_arg = atoi(optarg);
                    break;
                case '?':
                    break;
                }
            }
            if (!is_c)
                strcpy(g_opt_c_arg, "size");
            if (!is_o)
                g_opt_o_arg = 1;
        }
    }
    return 1;
}

void print_help()
{
    printf("Usage:\n");
    printf("  >  fmd5/fsha1 -e [FILE_EXTENSION] -l [MINSIZE] -h [MAXSIZE] -d [TARGET_DIRECTORY] -t [THREAD_NUM]\n");
    printf("        >> delete -l [SET_INDEX] -d [OPTARG] -i -f- t\n");
    printf("  > trash -c [CATEGORY] -o [ORDER]\n");
    printf("  > restore [RESTORE_INDEX]\n");
    printf("  > help\n");
    printf("  > exit\n\n");
}

int chk_arg_error()
{
    int is_float = 0;
    int flag = 0;
    int i = 0;
    char *homedir;

    if (g_ext_arg[0] != '*')
        return 0;
    if (strlen(g_ext_arg) > 1 && g_ext_arg[1] != '.')
        return 0;
    if (!strcmp(g_min_size, "~"))
    {
        g_minsize = 1;
    }
    else
    {
        for (int i = 0; g_min_size[i]; i++)
        {
            if (isdigit(g_min_size[i]))
                continue;
            else if (g_min_size[i] == '.')
                is_float++;
            else
                flag++;
        }
        printf("float : %d flag : %d\n", is_float, flag);
        if (is_float > 1)
            return 0;
        if (flag > 2)
            return 0;
        if (flag == 2 && !(strstr(g_min_size, "MB") || strstr(g_min_size, "mb") || strstr(g_min_size, "kb") || strstr(g_min_size, "KB") || strstr(g_min_size, "GB") || strstr(g_min_size, "gb")))
            return 0;
    }
    is_float = 0;
    flag = 0;
    if (!strcmp(g_max_size, "~"))
    {
        g_maxsize = 2048000000000;
    }
    else
    {
        for (int i = 0; g_max_size[i]; i++)
        {
            if (isdigit(g_max_size[i]))
                continue;
            else if (g_max_size[i] == '.')
                is_float++;
            else
                flag++;
        }
        printf("float : %d flag : %d\n", is_float, flag);
        if (is_float > 1)
            return 0;
        if (flag > 3)
            return 0;
        if (flag == 2 && !(strstr(g_max_size, "MB") || strstr(g_max_size, "mb") || strstr(g_max_size, "kb") || strstr(g_max_size, "KB") || strstr(g_max_size, "GB") || strstr(g_max_size, "gb")))
            return 0;
    }
    if (g_dir_arg[0] == '~')
    {
        uid_t uid = getuid();
        struct passwd *pw = getpwuid(uid);
        memset(g_home_dir, 0, PATH_MAX);
        strcpy(g_home_dir, pw->pw_dir);
        memset(g_real_path, 0, PATH_MAX);
        strcpy(g_real_path, g_home_dir);
        strcat(g_real_path, &g_dir_arg[1]);
    }
    else
    {
        char buf[PATH_MAX];
        realpath(g_dir_arg, buf);
        memset(g_real_path, 0, PATH_MAX);
        strcpy(g_real_path, buf);
    }
    if ((i = open(g_real_path, O_RDONLY)) < 0)
    {
        perror("path error");
        close(i);
        return 0;
    }
    return 1;
}

void convert_file_size(char *min, char *max)
{
    if (strchr(min, '~'))
        g_minsize = 1;
    else if (strstr(min, "KB") || strstr(min, "kb"))
    {
        g_minsize = atof(min) * 1024;
    }
    else if (strstr(min, "MB") || strstr(min, "mb"))
    {
        g_minsize = atof(min) * 1024 * 1024;
    }
    else if (strstr(min, "GB") || strstr(min, "gb"))
    {
        g_minsize = atof(min) * 1024 * 1024 * 1024;
    }
    else if (atof(min) == 0)
        g_minsize = 1;
    else
        g_minsize = atof(min);

    if (strchr(max, '~'))
        g_maxsize = 2048000000000;
    else if (strstr(max, "KB") || strstr(max, "kb"))
    {
        g_maxsize = atof(max) * 1024;
    }
    else if (strstr(max, "MB") || strstr(max, "mb"))
    {
        g_maxsize = atof(max) * 1024 * 1024;
    }
    else if (strstr(max, "GB") || strstr(max, "gb"))
    {
        g_maxsize = atof(max) * 1024 * 1024 * 1024;
    }
    else
        g_maxsize = atof(max);
}

/* 입력받은 path로 부터 bfs로 탐색을 시작함 */
/* 입력받은 확장자 파일을 찾으면 일단 암호화를 함 */
/* /proc, /sys, /run 디렉토리는 스킵 */
/* 만약 디렉토리 파일을 만나면 큐에 삽입 */
/* 입력받은 확장자에 부합한 정규 파일을 만날 경우, 사이즈 비교를 통해서 */
/* 사이즈도 입력값 사이라면 암호화를 하고 해당 정보를 buf.txt에 저장 */
int search_same_file(char *path, char *ext)
{
    char buf[PATH_MAX];
    char buf2[PATH_MAX];
    char *dir_path;
    FILE *fp;
    struct dirent **namelist;
    struct stat tmp;
    int count;
    int i;

    while (!is_queue_empty(q))
    {
        dir_path = delqueue(q);
        if ((count = scandir(dir_path, &namelist, NULL, alphasort)) == -1)
        {
            fprintf(stderr, "failed open %s\n", dir_path);
            return 0;
        }
        else
        {
            for (i = 0; i < count; i++)
            {
                strcpy(buf2, dir_path);
                if (strcmp(buf2, "/"))
                    strcat(buf2, "/");
                strcat(buf2, namelist[i]->d_name);
                lstat(buf2, &tmp);
                if (!strcmp(dir_path, "/") && (!strcmp(namelist[i]->d_name, "proc") || !strcmp(namelist[i]->d_name, "sys") || !strcmp(namelist[i]->d_name, "run")))
                {
                    free(namelist[i]);
                    continue;
                }
                if (!strcmp(namelist[i]->d_name, ".") || !strcmp(namelist[i]->d_name, "..") || !strcmp(namelist[i]->d_name, "trashcan"))
                {
                    free(namelist[i]);
                    continue;
                }
                if (namelist[i]->d_type == 4 && (tmp.st_mode & S_ISVTX) != 01000 && (tmp.st_mode & S_ISUID) != 04000 && (tmp.st_mode & S_ISGID) != 02000)
                {
                    if (!strcmp(dir_path, "/"))
                    {
                        strcpy(buf, dir_path);
                        strcat(buf, namelist[i]->d_name);
                        enqueue(q, buf);
                    }
                    else
                    {
                        strcpy(buf, dir_path);
                        strcat(buf, "/");
                        strcat(buf, namelist[i]->d_name);
                        enqueue(q, buf);
                    }
                }
                else if (S_ISREG(tmp.st_mode) && tmp.st_size >= g_minsize && tmp.st_size <= g_maxsize)
                {
                    char *ex, cpy[FILE_MAX];
                    int flag = 0;

                    if (ext == NULL)
                        flag = 1;

                    else
                    {
                        strcpy(cpy, namelist[i]->d_name);
                        ex = strrchr(cpy, '.');
                        if (ex == NULL)
                        {
                            flag = 0;
                        }

                        else if (!strcmp(ex, ext))
                            flag = 1;
                    }
                    if (flag == 1)
                    {
                        if (!strcmp(dir_path, "/"))
                        {
                            strcpy(buf, dir_path);
                            strcat(buf, namelist[i]->d_name);
                        }
                        else
                        {
                            strcpy(buf, dir_path);
                            strcat(buf, "/");
                            strcat(buf, namelist[i]->d_name);
                        }
                        if ((fp = fopen(buf, "r")) == NULL)
                        {
                            perror("file open");
                        }
                        else
                        {
                            if (g_function == 2)
                                do_sha1(fp, buf, tmp.st_size);
                            else if (g_function == 1)
                                do_md5(fp, buf, tmp.st_size);
                        }
                        fclose(fp);
                    }
                }
                memset(buf, 0, PATH_MAX);
                free(namelist[i]);
            }
        }
        free(dir_path);
    }
}

// md5 암호화 함수
// buf.txt파일에 md5값과 경로 그리고 파일 사이즈를 넣어줌
void do_md5(FILE *fp, char *path, long fsize)
{
    MD5_CTX c;
    unsigned char md[MD5_DIGEST_LENGTH];
    int fd;
    int i;
    static unsigned char buf[BUFF_SIZE];
    FILE *fp2;

    fd = fileno(fp);
    MD5_Init(&c);
    for (;;)
    {
        i = read(fd, buf, BUFF_SIZE);
        if (i <= 0)
            break;
        MD5_Update(&c, buf, (unsigned long)i);
    }
    MD5_Final(&(md[0]), &c);
    if ((fp2 = fopen("./buf.txt", "a+")) != NULL)
    {
        for (int i = 0; i < MD5_DIGEST_LENGTH; i++)
            fprintf(fp2, "%02x", md[i]);
        fprintf(fp2, "|%s|%ld\n", path, fsize);
    }
    fclose(fp2);
}

// sha1 암호화 함수
// buf.txt파일에 sha1값과 경로 그리고 파일 사이즈를 넣어줌
void do_sha1(FILE *fp, char *path, long fsize)
{
    SHA_CTX c;
    unsigned char md[SHA_DIGEST_LENGTH];
    int fd;
    int i;
    static unsigned char buf[BUFF_SIZE];
    FILE *fp2;

    fd = fileno(fp);
    SHA1_Init(&c);
    for (;;)
    {
        i = read(fd, buf, BUFF_SIZE);
        if (i <= 0)
            break;
        SHA1_Update(&c, buf, (unsigned long)i);
    }
    SHA1_Final(&(md[0]), &c);
    if ((fp2 = fopen("./buf.txt", "a+")) != NULL)
    {
        for (int i = 0; i < SHA_DIGEST_LENGTH; i++)
            fprintf(fp2, "%02x", md[i]);
        fprintf(fp2, "|%s|%ld\n", path, fsize);
    }
    fclose(fp2);
}

/* buf.txt파일에 저장된 정보들을 읽어오면서 같은 파일이 있는지 확인함 */
/* 만약 같은 파일이 있으면 링크드리스트에 넣어줌. */
/* buf.txt파일을 다 읽었다면 buf.txt파일을 삭제해줌 */
void check_same_file(void)
{
    FILE *fp;
    char md_buf1[33];
    char md_buf2[33];
    char path_buf1[PATH_MAX];
    char path_buf2[PATH_MAX];
    char size_buf1[15];
    char size_buf2[15];
    long current;
    int flag;

    current = 0;
    flag = 0;
    if ((fp = fopen("./buf.txt", "r+")) != NULL)
    {
        while (fscanf(fp, "%[^|]|%[^|]|%[^\n]\n", md_buf1, path_buf1, size_buf1) != EOF)
        {
            current = ftell(fp);
            if (!is_hash(md_buf1))
            {
                while (fscanf(fp, "%[^|]|%[^|]|%[^\n]\n", md_buf2, path_buf2, size_buf2) != EOF)
                {
                    if (!strcmp(md_buf1, md_buf2))
                    {
                        insert(&g_head, path_buf2, md_buf2, atol(size_buf2));
                        g_cnt_same_file++;
                        flag = 1;
                    }
                    memset(md_buf2, 0, 33);
                    memset(path_buf2, 0, PATH_MAX);
                    memset(size_buf2, 0, 15);
                }
            }
            if (flag == 1)
            {
                insert(&g_head, path_buf1, md_buf1, atol(size_buf1));
                g_cnt_same_file++;
            }
            flag = 0;
            fseek(fp, current, SEEK_SET);
            memset(md_buf1, 0, 33);
            memset(path_buf1, 0, PATH_MAX);
            memset(size_buf1, 0, 15);
        }
        fclose(fp);
        remove("./buf.txt");
    }
    else
    {
        return;
    }
}

/* 해당 세트의 해당 인덱스 노드를 지워주고 해당 파일도 삭제 */
void option_d(int set, int idx)
{
    char *path;
    FILE *fp;
    char logbuf[BUFF_SIZE];
    char times[BUFF_SIZE];
    time_t timer = time(NULL);
    struct tm *t = localtime(&timer);
    sprintf(times, "%d-%d-%d %d:%d:%d ", t->tm_year + 1900, t->tm_mon + 1, t->tm_mday, t->tm_hour, t->tm_min, t->tm_sec);
    fp = fopen(g_duplicat_log, "a+");

    path = delete_node(&g_head, set, idx);
    printf("\"%s\" has been deleted in #%d\n", path, set);
    strcpy(logbuf, "[DELETE] ");
    strcat(logbuf, path);
    strcat(logbuf, " ");
    strcat(logbuf, times);
    strcat(logbuf, g_user_name);
    fprintf(fp, "%s\n", logbuf);
    remove(path);
    cnt_set_idx_num(g_head);
    free(path);
    fclose(fp);
}

/* 삭제할 때 Y/y 혹은 N/n 값을 입력받아서 Y/y 입력시에만 삭제 */
int option_i(int set)
{
    char *path;
    char buf[5];
    int total_node;
    FILE *fp;
    char logbuf[BUFF_SIZE];
    char times[BUFF_SIZE];
    time_t timer = time(NULL);
    struct tm *t = localtime(&timer);
    sprintf(times, "%d-%d-%d %d:%d:%d ", t->tm_year + 1900, t->tm_mon + 1, t->tm_mday, t->tm_hour, t->tm_min, t->tm_sec);

    total_node = cnt_set_node(set);
    fp = fopen(g_duplicat_log, "a+");
    for (int i = 1; i <= total_node; i++)
    {
        path = get_node_path(set, i);
        printf("Delete \"%s\"? [y/n] ", path);
        free(path);
        fgets(buf, 5, stdin);
        if (!strcmp(buf, "y\n") || !strcmp(buf, "Y\n"))
        {
            memset(logbuf, 0, BUFF_SIZE);
            path = delete_node(&g_head, set, i);
            strcpy(logbuf, "[DELETE] ");
            strcat(logbuf, path);
            strcat(logbuf, " ");
            strcat(logbuf, times);
            strcat(logbuf, g_user_name);
            fprintf(fp, "%s\n", logbuf);
            remove(path);
            free(path);
        }
        else if (!strcmp(buf, "n\n") || !strcmp(buf, "N\n"))
        {
            continue;
        }
        else
        {
            printf("Input error!!\n");
            return 1;
        }
    }
    cnt_set_idx_num(g_head);
    fclose(fp);
    return 0;
}

/* flag == 0 => option_f */
/* flag == 1 => option_t */
void option_f_t(int set, int flag)
{
    char *path;
    char *latest_mtim_path;
    char *latest_mtim;
    char buf[PATH_MAX];
    struct dirent **namelist;
    int cnt;
    int latest_mtim_idx;
    int total_node;
    FILE *fp;
    char logbuf[BUFF_SIZE];
    char times[BUFF_SIZE];
    time_t timer = time(NULL);
    struct tm *t = localtime(&timer);
    sprintf(times, "%d-%d-%d %d:%d:%d ", t->tm_year + 1900, t->tm_mon + 1, t->tm_mday, t->tm_hour, t->tm_min, t->tm_sec);
    fp = fopen(g_duplicat_log, "a+");

    //가장 최근에 수정한 파일의 인덱스 번호, 경로, 수정한 날짜를 얻어옴
    latest_mtim_idx = get_latest_mtim_idx(set);
    latest_mtim_path = get_node_path(set, latest_mtim_idx);
    latest_mtim = get_latest_mtim(set, latest_mtim_idx);
    total_node = cnt_set_node(set);
    for (int i = 1; i <= total_node; i++)
    {
        if (i == latest_mtim_idx)
            continue;
        if (flag == 0)
        {
            memset(logbuf, 0, BUFF_SIZE);
            path = delete_node(&g_head, set, i);
            strcpy(logbuf, "[DELETE] ");
            strcat(logbuf, path);
            strcat(logbuf, " ");
            strcat(logbuf, times);
            strcat(logbuf, g_user_name);
            fprintf(fp, "%s\n", logbuf);
            remove(path);
            free(path);
        }
        //만약 휴지통이 없으면 해당 실행 디렉토리 위치에 휴지통을 만들고 거기에 파일을 링크해줌.
        //원본 파일은 unlink를 해줌
        if (flag == 1)
        {
            memset(logbuf, 0, BUFF_SIZE);
            path = delete_node(&g_head, set, i);
            strcpy(buf, g_trash_file_path);
            strcat(buf, "/");
            strcat(buf, strrchr(path, '/'));
            strcpy(logbuf, "[REMOVE] ");
            strcat(logbuf, path);
            strcat(logbuf, " ");
            strcat(logbuf, times);
            strcat(logbuf, g_user_name);
            fprintf(fp, "%s\n", logbuf);
            link(path, buf);
            unlink(path);
            free(path);
        }
    }
    if (flag == 0)
        printf("Left file in #%d : %s (%s)\n\n", set, latest_mtim_path, latest_mtim);
    if (flag == 1)
        printf("All files in #%d have moved to Trash except \"%s\" (%s)\n\n", set, latest_mtim_path, latest_mtim);
    free(latest_mtim);
    free(latest_mtim_path);
    cnt_set_idx_num(g_head);
    fclose(fp);
}

/* 사용자 입력 예외처리 해주는 함수 */
/* argc가 3이라면 무조건 숫자 d 숫자 형태여야함 */
/* argc가 2라면 무조건 숫자 i 혹은 t 혹은 f가 와야함 */
/* 해당 세트나 인덱스 번호도 링크드 리스트에 있는지 체크 */
int input_error(int argc, char **split)
{
    int opt;
    int flag = 0;
    optind = 0;
    g_del_idx = 0;
    g_del_set = 0;
    g_del_opt_d = 0;
    g_del_opt_f = 0;
    g_del_opt_i = 0;
    g_del_opt_l = 0;
    g_del_opt_t = 0;

    while ((opt = getopt(argc, split, "l:d:ift")) != -1)
    {
        switch (opt)
        {
        case 'l':
            g_del_opt_l = 1;
            g_del_set = atoi(optarg);
            if (!is_set_node(g_del_set))
                flag = 1;
            break;

        case 'd':
            g_del_opt_d = 1;
            g_del_idx = atoi(optarg);
            if (!is_set_idx_node(g_del_set, g_del_idx))
                flag = 1;
            break;

        case 'i':
            g_del_opt_i = 1;
            break;

        case 'f':
            g_del_opt_f = 1;
            break;

        case 't':
            g_del_opt_t = 1;
            break;

        case '?':
            return 0;
        }
    }

    if (flag == 1)
    {
        return 0;
    }
    return 1;
}