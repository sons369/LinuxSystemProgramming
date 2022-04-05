#include "../includes/md5_header.h"

int main(int argc, char *argv[])
{
    convert_file_size(argv[2], argv[3]);
    search_same_file(argv[4]);
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

int search_same_file(char *path)
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
    printf("enq: %s\n", path);
    while (!is_queue_empty(q))
    {
        dir_path = delqueue(q);
        printf("dir: %s\n", dir_path);
        if ((count = scandir(dir_path, &namelist, NULL, alphasort)) == -1)
        {
            printf("count: %d\n", count);
            fprintf(stderr, "failed open %s\n", dir_path);
            return 0;
        }
        else
        {
            printf("count: %d\n", count);
            for (i = 0; i < count; i++)
            {
                printf("name[%d] : %s\n", i, namelist[i]->d_name);
                strcpy(buf2, dir_path);
                if (strcpy(buf2, "/"))
                    strcat(buf2, "/");
                strcat(buf2, namelist[i]->d_name);
                lstat(buf2, &tmp);
                if (S_ISLNK(tmp.st_mode) || !strcmp(namelist[i]->d_name, ".") || !strcmp(namelist[i]->d_name, ".."))
                {
                    free(namelist[i]);
                    continue;
                }
                if (namelist[i]->d_type == 4 /*&& (tmp.st_mode & S_ISVTX) != 01000 && (tmp.st_mode & S_ISUID) != 04000 && (tmp.st_mode & S_ISGID) != 02000*/)
                {
                    if (!strcmp(dir_path, "/"))
                    {
                        strcpy(buf, dir_path);
                        strcat(buf, namelist[i]->d_name);
                        printf("dir buf: %s\n", buf);
                        enqueue(q, buf);
                    }
                    else
                    {
                        strcpy(buf, dir_path);
                        strcat(buf, "/");
                        strcat(buf, namelist[i]->d_name);
                        printf("dir buf: %s\n", buf);
                        enqueue(q, buf);
                    }
                }
                else
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
                        do_md5(fp);
                    fclose(fp);
                }
                memset(buf, 0, PATH_MAX);
                free(namelist[i]);
            }
        }
        free(dir_path);
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

void do_md5(FILE *fp)
{
    MD5_CTX c;
    unsigned char md[MD5_DIGEST_LENGTH];
    int fd;
    int i;
    static unsigned char buf[BUFSIZE];

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
    pt(md);
}

void pt(unsigned char *md)
{
    int i;

    for (i = 0; i < MD5_DIGEST_LENGTH; i++)
        printf("%02x", md[i]);
    printf("\n");
}