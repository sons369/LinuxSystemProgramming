#include "../includes/header.h"

int main(void)
{
    char buf[BUFF_SIZE];
    char **split;
    int argc;
    int status;
    int child;
    pid_t pid;

    while (1)
    {
        printf("20182624> ");
        fgets(buf, BUFF_SIZE, stdin);
        argc = user_input(buf, &split);
        // input enter
        if (argc == 0)
            continue;
        // if input 'fmd5' then, fork process and exec fmd5
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
        // if input 'fsha1' then, fork process and exec fsha1
        else if (strcmp(split[0], "fsha1") == 0)
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
                    execl("./fsha1", "./fsha1", split[1], split[2], split[3], g_target_dir, (char *)NULL);
                    fprintf(stderr, "Error\n");
                    exit(1);
                }
                child = wait(&status);
            }
        }
        // input 'exit' then exit program
        else if (strcmp(split[0], "exit") == 0)
        {
            printf("\nPrompt End\n");
            break;
        }
        // input others then print help
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

    exit(0);
}

/* Get input argc, argv and split it. */
/* Then, return it */
int user_input(char buf[], char ***split)
{
    int argc;

    argc = 0;
    *split = ft_split(buf, " \n");
    while ((*split)[argc])
        argc++;
    return argc;
}

/* 사용자 입력에서 예외 처리 */
int input_error(int argc, char **split)
{
    int i;
    int flag;
    int flag2;
    int is_float;
    int is_float2;
    char *homedir;

    if (argc != 5)
        return 0;
    if ((strlen(split[1]) > 1 && !(split[1][0] == '*' && split[1][1] == '.')) || (strlen(split[1]) == 1 && !strchr(split[1], '*')))
        return 0;
    // MIN_SIZE 예외처리. 정수, ~, kb, mb, gb에서 소수점 처리 그외 입력시 에러
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
        if (is_float == 1 && flag != 2)
            return 0;

        if (flag == 2 && !(strstr(split[2], "MB") || strstr(split[2], "mb") || strstr(split[2], "kb") || strstr(split[2], "KB") || strstr(split[2], "GB") || strstr(split[2], "gb")))
        {
            return 0;
        }
    }
    // MAX_SIZE 예외처리. 정수, ~, kb, mb, gb에서 소수점 처리 그외 입력시 에러
    if (split[3])
    {

        is_float2 = 0;
        flag2 = 0;
        if (strlen(split[3]) > 1 && strchr(split[3], '~'))
            return 0;
        for (i = 0; split[3][i]; i++)
        {
            if (isdigit(split[3][i]) || split[3][0] == '~')
                continue;
            else if (split[3][i] == '.')
                is_float2++;
            else if (isalpha(split[3][i]))
                flag2++;
        }
        if (is_float2 > 1)
            return 0;
        if (flag2 > 2)
            return 0;
        if (is_float2 == 1 && flag2 != 2)
            return 0;
        if (flag2 == 2 && !(strstr(split[3], "MB") || strstr(split[3], "mb") || strstr(split[3], "kb") || strstr(split[3], "KB") || strstr(split[3], "GB") || strstr(split[3], "gb")))
        {
            return 0;
        }
    }
    // ~ 입력시 홈 디렉토리 얻고, 그외 경로 입력시 해당 절대경로를 넘겨줌
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
        if ((i = open(g_target_dir, O_RDONLY)) < 0)
        {
            perror("open");
            return 0;
        }
        close(i);
    }
    return 1;
}