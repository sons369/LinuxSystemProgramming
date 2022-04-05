#include "../includes/header.h"

int main(void)
{
    struct timeval start, end;
    char buf[BUFF_SIZE];
    char **split;
    int argc;
    int status;
    int child;
    pid_t pid;

    gettimeofday(&start, NULL);
    while (1)
    {
        printf("20182624> ");
        fgets(buf, BUFF_SIZE, stdin);
        argc = user_input(buf, &split);
        if (argc == 0)
            continue;
        else if (strcmp(split[0], "fmd5") == 0)
        {
            if (!input_error(argc, split))
            {
                printf("error\n");
            }
            else
            {
                if ((pid = fork()) < 0)
                    perror("fork error\n");
                else if (pid == 0)
                {
                    execl("./fmd5", "./fmd5", split[1], split[2], split[3], g_target_dir, (char *)NULL);
                    fprintf(stderr, "Error\n");
                    exit(1);
                }
                child = wait(&status);
            }
        }

        else if (strcmp(split[0], "fsha1") == 0)
        {
        }
        else if (strcmp(split[0], "exit") == 0)
        {
            printf("\nPrompt End\n");
            break;
        }
        else
        {
            if ((pid = fork()) < 0)
                perror("fork error\n");
            else if (pid == 0)
            {
                execl("./help", "./help", (char *)NULL);
                fprintf(stderr, "Error\n");
                exit(1);
            }
            child = wait(&status);
        }
        if (*split)
        {
            for (int i = 0; split[i]; i++)
                free(split[i]);
            free(split);
        }
    }
    gettimeofday(&end, NULL);
    end.tv_sec -= start.tv_sec;
    if (end.tv_sec < start.tv_usec)
    {
        end.tv_sec--;
        end.tv_usec += 1000000;
    }

    exit(0);
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

int input_error(int argc, char **split)
{
    int i;
    int flag;
    int is_float;
    char *homedir;

    if (argc != 5)
        return 0;
    if ((strlen(split[1]) > 1 && !(split[1][0] == '*' && split[1][1] == '.')) || (strlen(split[1]) == 1 && !strchr(split[1], '*')))
        return 0;
    if (split[2])
    {
        is_float = 0;
        flag = 0;
        if (strlen(split[2]) > 1 && strchr(split[2], '~'))
            return 0;
        for (i = 0; split[2][i]; i++)
        {
            if (isdigit(split[2][i]) || split[2][0] == '~')
                continue;
            else if (split[2][i] == '.')
                is_float++;
            else if (isalpha(split[2][i]))
                flag++;
        }
        if (is_float > 1)
            return 0;
        if (flag > 2)
            return 0;
        if (flag == 2 && !(strstr(split[2], "MB") || strstr(split[2], "mb") || strstr(split[2], "kb") || strstr(split[2], "KB") || strstr(split[2], "GB") || strstr(split[2], "gb")))
        {
            return 0;
        }
    }
    if (split[3])
    {
        is_float = 0;
        flag = 0;
        if (strlen(split[3]) > 1 && strchr(split[3], '~'))
            return 0;
        for (i = 0; split[3][i]; i++)
        {
            if (isdigit(split[3][i]) || split[3][0] == '~')
                continue;
            else if (split[3][i] == '.')
                is_float++;
            else if (isalpha(split[3][i]))
                flag++;
        }
        if (is_float > 1)
            return 0;
        if (flag > 2)
            return 0;
        if (flag == 2 && !(strstr(split[3], "MB") || strstr(split[3], "mb") || strstr(split[3], "kb") || strstr(split[3], "KB") || strstr(split[3], "GB") || strstr(split[3], "gb")))
        {
            return 0;
        }
    }

    if (split[4])
    {
        if (split[4][0] == '~')
        {
            homedir = getenv("HOME");
            if (homedir != NULL)
            {
                memset(g_homedir_path, 0, PATH_MAX);
                strcpy(g_homedir_path, homedir);
            }
            else
            {
                uid_t uid = getuid();
                struct passwd *pw = getpwuid(uid);
                if (pw == NULL)
                {
                    printf("Failed get homedir path\n");
                }
                else
                {
                    memset(g_homedir_path, 0, PATH_MAX);
                    strcpy(g_homedir_path, pw->pw_dir);
                }
            }
            memset(g_target_dir, 0, PATH_MAX);
            strcpy(g_target_dir, g_homedir_path);
            strcat(g_target_dir, &split[4][1]);
        }
        else
        {
            char buf[PATH_MAX];
            realpath(split[4], buf);
            memset(g_target_dir, 0, PATH_MAX);
            strcpy(g_target_dir, buf);
        }
        printf("home: %s target: %s\n", g_homedir_path, g_target_dir);
        if ((i = open(g_target_dir, O_RDONLY)) < 0)
        {
            perror("open");
            return 0;
        }
        close(i);
    }
    return 1;
}