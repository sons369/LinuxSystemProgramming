#include "../includes/header.h"

int main()
{
    struct timeval start, end;
    int select;
    long sec;
    long long usec;

    realpath(".", g_home_dir);

    /* start time check */
    gettimeofday(&start, NULL);
    while (1)
    {
        prompt();
        select = ft_case(input_function());
        if (select == 2)
        {
            /* end time check */
            gettimeofday(&end, NULL);
            sec = (end.tv_sec - start.tv_sec);
            usec = (end.tv_usec - start.tv_usec);
            if (usec < 0)
                usec += 1000000;
            printf("Prompt End\nRuntime : %ld:%lld(sec:usec)", sec, usec);
            exit(1);
        }
        else if (select == 4)
        {
            print_help();
        }
        else if (select == 1)
        {
            continue;
        }
        else if (select == 3)
        {
            find_file();
        }
        /* Initialize to 0 */
        memset(&g_mtmp, 0, sizeof(g_mtmp));
        memset(&g_tmp, 0, sizeof(g_tmp));
        memset(&g_info, 0, sizeof(g_info));
    }
    return (0);
}