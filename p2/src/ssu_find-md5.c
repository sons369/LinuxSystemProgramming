#include "../includes/md5_header.h"

int main(int argc, char *argv[])
{
    char *extension;

    extension = strrchr(argv[1], '.');
    convert_file_size(argv[2], argv[3]);
    search_same_file(argv[4], extension);

    exit(0);
}

void convert_file_size(char *min, char *max)
{
    if (strchr(min, '~'))
        g_min_size = 0;
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
                printf("name[%d] : %s\n", i, namelist[i]->d_name);
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
                if (!strcmp(namelist[i]->d_name, ".") || !strcmp(namelist[i]->d_name, ".."))
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
