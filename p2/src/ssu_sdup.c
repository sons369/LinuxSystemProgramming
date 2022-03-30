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
        printf("argc : %d\n", argc);
        if (argc == 0)
            continue;
        else if (strcmp(split[0], "fmd5") == 0)
        {
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
                execl("/home/ubuntu/linux/p2/help", "/home/ubuntu/linux/p2/help", (char *)NULL);
                fprintf(stderr, "Error\n");
                exit(1);
            }
            child = wait(&status);
            printf("parent pid : %d\n", getpid());
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