#include "../includes/md5_header.h"

int main(int argc, char *argv[])
{
    char *extension;
    char **split;
    char buf[BUFF_SIZE];
    struct timeval start, end;
    int argc2;

    gettimeofday(&start, NULL);
    extension = strrchr(argv[1], '.');
    convert_file_size(argv[2], argv[3]);
    search_same_file(argv[4], extension);
    check_same_file();
    if (g_head != NULL)
    {
        print_node(g_head);
        cnt_set_idx_num(g_head);
    }
    else
    {
        printf("No duplicates in %s\n", argv[4]);
        gettimeofday(&end, NULL);
        end.tv_sec -= start.tv_sec;
        if (end.tv_usec < start.tv_usec)
        {
            end.tv_sec--;
            end.tv_usec += 1000000;
        }
        end.tv_usec -= start.tv_usec;
        printf("Searching time: %ld:%06ld(sec:usec)\n", end.tv_sec, end.tv_usec);
        exit(1);
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
        argc2 = user_input(buf, &split);
        if (argc2 == 0)
            continue;
        if (!input_error(argc2, split))
        {
            printf("Input Error!!\n");
        }
        else
        {
            if (!strcmp(split[0], "exit"))
            {
                if (*split)
                {
                    for (int i = 0; split[i]; i++)
                        free(split[i]);
                    free(split);
                }
                free_all_node(&g_head);
                printf(">> Back to Prompt\n");
                break;
            }
            else if (!strcmp(split[1], "d"))
            {
                option_d(atoi(split[0]), atoi(split[2]));
            }
            else if (!strcmp(split[1], "i"))
            {
                option_i(atoi(split[0]));
            }
            else if (!strcmp(split[1], "f"))
            {
                option_f_t(atoi(split[0]), 0);
            }
            else if (!strcmp(split[1], "t"))
            {
                option_f_t(atoi(split[0]), 1);
            }
        }
        if (*split)
        {
            for (int i = 0; split[i]; i++)
                free(split[i]);
            free(split);
        }
        print_node(g_head);
    }
    exit(0);
}

int user_input(char buf[], char ***split)
{
    int argc;

    argc = 0;
    *split = ft_split(buf, " \n\t");
    while ((*split)[argc])
        argc++;
    return argc;
}

int input_error(int argc, char **split)
{
    if (argc > 3)
        return 0;
    else if (argc == 3)
    {
        for (int i = 0; split[0][i]; i++)
        {
            if (!isdigit(split[0][i]))
                return 0;
        }
        for (int i = 0; split[2][i]; i++)
        {
            if (!isdigit(split[2][i]))
                return 0;
        }
        if (strcmp(split[1], "d"))
            return 0;
        if (!is_set_idx_node(atoi(split[0]), atoi(split[2])))
            return 0;
    }
    else if (argc == 2)
    {
        for (int i = 0; split[0][i]; i++)
        {
            if (!isdigit(split[0][i]))
                return 0;
        }
        if (strcmp(split[1], "i") && strcmp(split[1], "f") && strcmp(split[1], "t"))
            return 0;
        if (!is_set_node(atoi(split[0])))
            return 0;
    }
    else if (argc == 1)
    {
        if (strcmp(split[0], "exit"))
            return 0;
    }
    return 1;
}

void convert_file_size(char *min, char *max)
{
    if (strchr(min, '~'))
        g_min_size = 1;
    else if (strstr(min, "KB") || strstr(min, "kb"))
    {
        g_min_size = atof(min) * 1024;
    }
    else if (strstr(min, "MB") || strstr(min, "mb"))
    {
        g_min_size = atof(min) * 1024 * 1024;
    }
    else if (strstr(min, "GB") || strstr(min, "gb"))
    {
        g_min_size = atof(min) * 1024 * 1024 * 1024;
    }
    else if (atof(min) == 0)
        g_min_size = 1;
    else
        g_min_size = atof(min);

    if (strchr(max, '~'))
        g_max_size = 2048000000000;
    else if (strstr(max, "KB") || strstr(max, "kb"))
    {
        g_max_size = atof(max) * 1024;
    }
    else if (strstr(max, "MB") || strstr(max, "mb"))
    {
        g_max_size = atof(max) * 1024 * 1024;
    }
    else if (strstr(max, "GB") || strstr(max, "gb"))
    {
        g_max_size = atof(max) * 1024 * 1024 * 1024;
    }
    else
        g_max_size = atof(max);
}

int search_same_file(char *path, char *ext)
{
    char buf[PATH_MAX];
    char buf2[PATH_MAX];
    char *dir_path;
    t_Qtype *q;
    FILE *fp;
    struct dirent **namelist;
    struct stat tmp;
    int count;
    int i;

    q = create_queue_link();
    enqueue(q, path);
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
                else if (S_ISREG(tmp.st_mode) && tmp.st_size >= g_min_size && tmp.st_size <= g_max_size)
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
                            flag = 0;
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
                            do_md5(fp, buf, tmp.st_size);
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

void do_md5(FILE *fp, char *path, long fsize)
{
    MD5_CTX c;
    unsigned char md[MD5_DIGEST_LENGTH];
    int fd;
    int i;
    static unsigned char buf[BUFSIZE];
    FILE *fp2;

    fd = fileno(fp);
    MD5_Init(&c);
    for (;;)
    {
        i = read(fd, buf, BUFSIZE);
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

void option_d(int set, int idx)
{
    char *path;

    path = delete_node(&g_head, set, idx);
    printf("\"%s\" has been deleted in #%d\n", path, set);
    remove(path);
    cnt_set_idx_num(g_head);
    // free(path);
}

int option_i(int set)
{
    char *path;
    char buf[5];
    int total_node;

    total_node = cnt_set_node(set);
    printf("total node: %d\n", total_node);
    for (int i = 1; i <= total_node; i++)
    {
        path = get_node_path(set, i);
        printf("Delete \"%s\"? [y/n] ", path);
        free(path);
        fgets(buf, 5, stdin);
        if (!strcmp(buf, "y\n") || !strcmp(buf, "Y\n"))
        {
            path = delete_node(&g_head, set, i);
            remove(path);
            // free(path);
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
    int latest_mtim_idx;
    int total_node;

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
            path = delete_node(&g_head, set, i);
            // remove(path);
            free(path);
        }
        if (flag == 1)
        {
            mkdir("./trashcan", 0775);
            path = delete_node(&g_head, set, i);
            strcpy(buf, "./trashcan/");
            strcat(buf, strchr(path, '/'));
            link(path, buf);
            // unlink(path);
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
}