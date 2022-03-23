#include "../includes/header.h"

/* input index, options from user */
/* with exception handling */
/* Options are always separated by spaces. */
/* Index value must be numeric */
/* The number of input values must be 4 or less. */
int input_index_option(char *str)
{
    char **result;
    int i;
    int j;
    int digit;

    result = ft_split(str, " \n\t");
    free(str);
    i = 0;
    while (result[i])
        i++;

    j = 0;
    digit = 1;
    /* Index value must be numeric */
    while (result[0][j])
    {
        if (!isdigit(result[0][j]))
            digit = 0;
        j++;
    }
    if (digit == 1 && i <= 5)
    {
        g_index = atoi(result[0]);
    }
    else
    {
        printf("err\n");
        return 0;
    }

    /* processing options */
    if (i > 1)
    {

        for (j = 1; result[j]; j++)
        {
            if (strlen(result[j]) != 1)
            {
                for (int k = 0; result[k]; k++)
                {
                    free(result[k]);
                }
                free(result);
                return 0;
            }
            else if (strcmp(result[j], "q") && strcmp(result[j], "s") && strcmp(result[j], "i") && strcmp(result[j], "r"))
            {
                for (int k = 0; result[k]; k++)
                {
                    free(result[k]);
                }
                free(result);
                return 0;
            }
            else
            {
                if (!strcmp(result[j], "q"))
                    g_option[0] = 1;
                else if (!strcmp(result[j], "s"))
                    g_option[1] = 1;
                else if (!strcmp(result[j], "i"))
                    g_option[2] = 1;
                else if (!strcmp(result[j], "r"))
                    g_option[3] = 1;
            }
        }
    }

    for (int k = 0; result[k]; k++)
    {
        free(result[k]);
    }
    free(result);

    return 1;
}

/* Open the files, count the number of lines, and create array. */
void make_arr_from_files()
{
    t_myStatptr tmp;
    FILE *fp1, *fp2;
    int zero_line;
    int idx_line;
    int i;

    tmp = get_idx_node(g_index);
    zero_line = 0;
    idx_line = 0;
    i = 0;
    if ((fp1 = fopen(g_zero_file.path_filename, "r")) == NULL)
    {
        perror("fp1");
        return;
    }
    if ((fp2 = fopen(tmp->path_filename, "r")) == NULL)
    {
        perror("fp2");
        return;
    }
    else
    {
        /* count line */
        while (fgets(g_file1_content[zero_line], BUFF, fp1))
        {
            zero_line++;
        }
        g_line_zero = zero_line;
        while (fgets(g_file2_content[idx_line], BUFF, fp2))
        {
            idx_line++;
        }
        g_line_idx = idx_line;
    }
    fclose(fp1);
    fclose(fp2);

    if ((g_cnt_line_zero = (int *)calloc(zero_line, sizeof(int))) == NULL)
    {
        printf("malloc err!\n");
        return;
    }

    if ((g_cnt_line_idx = (int *)calloc(idx_line, sizeof(int))) == NULL)
    {
        printf("malloc err!\n");
        return;
    }
}

/* Find the same line through strcmp() and write down the corresponding line number. */
/* If there are multiple identical sentences, put them in the fastest order. */
void find_same_line()
{
    FILE *fp1, *fp2;
    char buf1[BUFF], buf2[BUFF];
    int cnt;
    int i;
    t_myStatptr tmp;

    i = 0;
    tmp = get_idx_node(g_index);
    if ((fp1 = fopen(g_zero_file.path_filename, "r")) == NULL)
    {
        perror("fp1");
        return;
    }
    if ((fp2 = fopen(tmp->path_filename, "r")) == NULL)
    {
        perror("fp2");
        return;
    }

    while (fgets(buf1, BUFF, fp1))
    {
        cnt = 1;
        fseek(fp2, 0, SEEK_SET);
        while (fgets(buf2, BUFF, fp2))
        {
            if (!strcmp(buf1, buf2) && g_used_idx[cnt] != 1)
            {
                g_cnt_line_zero[i] = cnt;
                /* Check the used line */
                g_used_idx[cnt] = 1;
                break;
            }
            cnt++;
        }
        i++;
    }
    memset(&g_used_idx, 0, BUFF);
    fclose(fp1);
    fclose(fp2);
    if ((fp1 = fopen(g_zero_file.path_filename, "r")) == NULL)
    {
        perror("fp1");
        return;
    }
    if ((fp2 = fopen(tmp->path_filename, "r")) == NULL)
    {
        perror("fp2");
        return;
    }
    i = 0;
    while (fgets(buf1, BUFF, fp2))
    {
        cnt = 1;
        fseek(fp1, 0, SEEK_SET);
        while (fgets(buf2, BUFF, fp1))
        {
            if (!strcmp(buf1, buf2) && g_used_idx[cnt] != 1)
            {
                g_cnt_line_idx[i] = cnt;
                g_used_idx[cnt] = 1;
                break;
            }
            cnt++;
        }
        i++;
    }
    fclose(fp1);
    fclose(fp2);

    memset(&g_used_idx, 0, BUFF);
}

/* 여기만 한글 주석 */
/* 각 파일의 같은 줄 번호가 적혀있는 g_cnt_line_zero, g_cnt_line_idx 배열 이용 */
/* 어떤 줄 번호를 0으로 지워줘야 할 지 검증해주는 함수 */
/* zero의 0번부터 값을 읽어서 0이 아닌 숫자를 만나면 해당 숫자의 */
/* idx 인덱스 번호로 이동 */
/* idx 인덱스 번호로 이동했으면 인덱스번호 - 1 씩 하면서 체크가 안된 0이 아닌 숫자 */
/* 를 만나면 0으로 바꾸고 zero에서도 0으로 바꿔줌 */
/* 단 \n은 나중에 처리함으로 해당 줄이 \n이면 패스 */
/* 또한 idx 인덱스-1 씩 이동하면서 만난 0이 아닌 숫자를 보고 다시 zero로 해당 번호로 가서 */
/* 위로 올라가면서 자기보다 큰 숫자가 있으면 0 */
void verification_arr()
{
    int i;
    int j;
    int flag;
    int max;

    i = 0;
    j = 0;
    flag = 0;
    max = 0;
    /*필요한 인덱스 번호들만 남기게끔 검증하는 부분 */
    for (i = 0; i < g_line_zero; i++)
    {
        if (g_cnt_line_zero[i] > 0)
        {
            if (g_line_zero == 1 && !strcmp(g_file1_content[0], "\0"))
            {
                continue;
            }
            else if (g_cnt_line_zero[i] == 1)
            {
                g_used_idx[g_cnt_line_zero[i] - 1] = 1;
            }
            else
            {

                for (j = g_cnt_line_zero[i] - 2; j >= 0; j--)
                {
                    if (strcmp(g_file2_content[j], "\n") && g_cnt_line_idx[j] > 0 && g_used_idx[j] == 0)
                    {
                        if (g_cnt_line_zero[i] > g_cnt_line_zero[g_cnt_line_idx[j] - 1])
                        {
                            for (int l = g_cnt_line_idx[j] - 2; l > i; l--)
                            {
                                if (g_cnt_line_zero[g_cnt_line_idx[j] - 1] < g_cnt_line_zero[l])
                                {
                                    flag = 2;
                                    g_cnt_line_zero[g_cnt_line_idx[j] - 1] = 0;
                                    g_cnt_line_idx[j] = 0;
                                    break;
                                }
                            }
                        }
                        if (flag == 0)
                        {
                            flag = 1;
                            g_cnt_line_idx[g_cnt_line_zero[i] - 1] = 0;
                            break;
                        }
                    }
                }
            }
            if (flag == 1)
            {
                g_cnt_line_zero[i] = 0;
            }
            else
            {
                g_used_idx[g_cnt_line_zero[i] - 1] = 1;
            }
        }
        flag = 0;
    }
    /* 처리안된 개행 처리 해주는 부분 */
    for (i = 0; i < g_line_zero; i++)
    {
        if (g_cnt_line_zero[i] > 0)
        {
            max = max < g_cnt_line_zero[i] ? g_cnt_line_zero[i] : max;
            if (g_cnt_line_zero[i] < max)
            {
                g_cnt_line_idx[g_cnt_line_zero[i] - 1] = 0;
                g_cnt_line_zero[i] = 0;
            }
        }
    }
}

/* Create an answer table using only verified index numbers. */
/* The required size is g_cnt_line_zero's lines and cnt idx's zero value */
/* Starting with number 0, if both are numbers, go down together, */
/* and if both are zero, put zero first, stop when you meet a number, */
/* and put the idx value in. */
/* ex) zero: 0 0 1 2 3 0 idx: 3 4 5 0 0 */
/* result arr: -1 -2 0 0 0 -6 4 5 */
void make_result_arr()
{
    int cnt_zero_from_file2;
    int i;
    int j;
    int k;

    cnt_zero_from_file2 = 0;
    for (i = 0; i < g_line_idx; i++)
    {
        if (g_cnt_line_idx[i] == 0)
            cnt_zero_from_file2++;
    }
    g_diff_result_arr_len = cnt_zero_from_file2 + g_line_zero;
    if ((g_diff_result_arr = (int *)malloc(sizeof(int) * (cnt_zero_from_file2 + g_line_zero))) == NULL)
    {
        printf("malloc err\n");
        return;
    }

    i = 0;
    j = 0;
    k = 0;
    while (i < g_line_zero && j < g_line_idx)
    {
        if (g_cnt_line_zero[i] == 0 && g_cnt_line_idx[j] == 0)
        {
            for (int l = i; !g_cnt_line_zero[l] && l < g_line_zero; l++)
            {
                g_diff_result_arr[k] = -(l + 1);
                k++;
                i++;
            }
            for (int l = j; !g_cnt_line_idx[l] && l < g_line_idx; l++)
            {
                g_diff_result_arr[k] = l + 1;
                k++;
                j++;
            }
        }
        else if (g_cnt_line_zero[i] != 0 && g_cnt_line_idx[j] == 0)
        {
            for (int l = j; !g_cnt_line_idx[l] && l < g_line_idx; l++)
            {
                g_diff_result_arr[k] = l + 1;
                k++;
                j++;
            }
        }
        else if (g_cnt_line_zero[i] == 0 && g_cnt_line_idx[j] != 0)
        {
            for (int l = i; !g_cnt_line_zero[l] && l < g_line_zero; l++)
            {
                g_diff_result_arr[k] = -(l + 1);
                k++;
                i++;
            }
        }
        else if (g_cnt_line_zero[i] != 0 && g_cnt_line_idx[j] != 0)
        {
            for (int l = i, m = j; g_cnt_line_zero[l] && g_cnt_line_idx[m] && l < g_line_zero && m < g_line_zero; l++, m++)
            {
                g_diff_result_arr[k] = 0;
                k++;
                i++;
                j++;
            }
        }
    }
}

/* make diff.c 's every global variable value 0 */
void memset_diff_global_variable()
{
    memset(&g_used_idx, 0, BUFF);
    free(g_cnt_line_idx);
    g_cnt_line_idx = NULL;
    free(g_cnt_line_zero);
    g_cnt_line_zero = NULL;
    g_line_zero = 0;
    g_line_idx = 0;
    g_diff_result_arr_len = 0;
    for (int i = 0; i < BUFF; i++)
    {
        for (int j = 0; j < BUFF; j++)
        {
            g_file1_content[i][j] = 0;
            g_file2_content[i][j] = 0;
        }
    }
}

/* Print result use g_diff_result_arr */
/* meet a positive number first, print add */
/* If there is only a negative number, print delete. */
/* positive after a negative is a change. */
/* Print when 0 is met. */
void print_diff_result()
{
    int idx = 0;
    int add_idx = 0;
    int n_min = 0;
    int n_max = 0;
    int p_min = 0;
    int p_max = 0;
    int flag = 0;

    for (int i = 0; i < g_diff_result_arr_len; i++)
    {
        if (g_diff_result_arr[i] > 0 && flag == 0)
        {
            flag = 1;
            p_min = g_diff_result_arr[i];
            p_max = g_diff_result_arr[i];
            idx++;
        }
        else if (g_diff_result_arr[i] > 0 && flag == 1)
        {
            p_max = g_diff_result_arr[i];
            idx++;
        }
        if (g_diff_result_arr[i] < 0 && flag == 0)
        {
            flag = 2;
            n_max = g_diff_result_arr[i];
            n_min = g_diff_result_arr[i];
            add_idx++;
        }
        else if (g_diff_result_arr[i] < 0 && flag == 2)
        {
            n_max = g_diff_result_arr[i];
            add_idx++;
        }
        else if (g_diff_result_arr[i] > 0 && flag == 2)
        {
            flag = 3;
            p_min = g_diff_result_arr[i];
            p_max = g_diff_result_arr[i];
            idx++;
        }
        else if (g_diff_result_arr[i] > 0 && flag == 3)
        {
            p_max = g_diff_result_arr[i];
            idx++;
        }
        if (g_diff_result_arr[i] == 0)
        {
            if (flag == 1)
            {
                print_diff_add(add_idx, p_min, p_max);
            }
            else if (flag == 2)
            {
                print_diff_delete(idx, -n_min, -n_max);
            }
            else if (flag == 3)
            {
                print_diff_change(-n_min, -n_max, p_min, p_max);
            }
            flag = 0;
            p_min = 0;
            p_max = 0;
            n_max = 0;
            n_min = 0;
            idx++;
            add_idx++;
        }
    }
    /* If there is no zero at the end and the output is not possible, it will be output. */
    if (flag != 0)
    {
        if (flag == 1)
        {
            print_diff_add(p_min - 1, p_min, p_max);
        }
        else if (flag == 2)
        {
            print_diff_delete(idx, -n_min, -n_max);
        }
        else if (flag == 3)
        {
            print_diff_change(-n_min, -n_max, p_min, p_max);
        }
    }
}

/* print add */
void print_diff_add(int idx, int start, int end)
{
    printf("%da%d,%d\n", idx, start, end);
    for (int i = start - 1; i < end; i++)
    {
        printf("> %s", g_file2_content[i]);
        if (strchr(g_file2_content[i], '\n') == NULL)
        {
            printf("\n\\ No newline at end of file\n");
        }
    }
}

/* print change */
void print_diff_change(int n_start, int n_end, int p_start, int p_end)
{
    char *sep = "---\n";

    if ((n_end - n_start) == 0 && (p_end - p_start) == 0)
    {
        printf("%dc%d\n", n_end, p_end);
        printf("< %s", g_file1_content[n_end - 1]);
        if (strchr(g_file1_content[n_end - 1], '\n') == NULL)
        {
            printf("\n\\ No newline at end of file\n");
        }
        printf("%s", sep);
        printf("> %s", g_file2_content[p_end - 1]);
        if (strchr(g_file2_content[p_end - 1], '\n') == NULL)
        {
            printf("\n\\ No newline at end of file\n");
        }
    }
    else if ((n_end - n_start) == 0)
    {
        printf("%dc%d,%d\n", n_end, p_start, p_end);
        printf("< %s", g_file1_content[n_end - 1]);
        if (strchr(g_file1_content[n_end - 1], '\n') == NULL)
        {
            printf("\n\\ No newline at end of file\n");
        }
        printf("%s", sep);
        for (int i = p_start - 1; i < p_end; i++)
        {
            printf("> %s", g_file2_content[i]);
            if (strchr(g_file2_content[i], '\n') == NULL)
            {
                printf("\n\\ No newline at end of file\n");
            }
        }
    }
    else if ((p_end - p_start) == 0)
    {
        printf("%d,%dc%d\n", n_start, n_end, p_end);
        for (int i = n_start - 1; i < n_end; i++)
        {
            printf("< %s", g_file1_content[i]);
            if (strchr(g_file1_content[i], '\n') == NULL)
            {
                printf("\n\\ No newline at end of file\n");
            }
        }
        printf("%s", sep);
        printf("> %s", g_file2_content[p_end]);
        if (strchr(g_file2_content[p_end - 1], '\n') == NULL)
        {
            printf("\n\\ No newline at end of file\n");
        }
    }
    else
    {
        printf("%d,%dc%d,%d\n", n_start, n_end, p_start, p_end);
        for (int i = n_start - 1; i < n_end; i++)
        {
            printf("< %s", g_file1_content[i]);
            if (strchr(g_file1_content[i], '\n') == NULL)
            {
                printf("\n\\ No newline at end of file\n");
            }
        }
        printf("%s", sep);
        for (int i = p_start - 1; i < p_end; i++)
        {
            printf("> %s", g_file2_content[i]);
            if (strchr(g_file2_content[i], '\n') == NULL)
            {
                printf("\n\\ No newline at end of file\n");
            }
        }
    }
}

/* print delete */
void print_diff_delete(int idx, int start, int end)
{
    printf("%d,%dd%d\n", start, end, idx);
    for (int i = start - 1; i < end; i++)
    {
        printf("< %s", g_file1_content[i]);
        if (strchr(g_file1_content[i], '\n') == NULL)
        {
            printf("\n\\ No newline at end of file\n");
        }
    }
}

/* print option q */
void print_diff_option_q()
{
    t_myStatptr tmp;
    char **path_split;
    int flag;
    int i;

    tmp = get_idx_node(g_index);
    i = 0;
    flag = 0;
    path_split = ft_split(tmp->real_path, "/");
    while (path_split[i])
    {
        i++;
    }
    for (int k = 0; k < g_diff_result_arr_len; k++)
    {
        if (g_diff_result_arr[k] != 0)
        {
            flag = 1;
            break;
        }
    }
    if (flag == 1)
    {
        printf("File %s and %s/%s differ\n", g_zero_file.filename, path_split[--i], tmp->filename);
    }
    i = 0;
    while (path_split[i])
    {
        free(path_split[i]);
        i++;
    }
    free(path_split);
}

/* print option s */
void print_diff_option_s()
{
    t_myStatptr tmp;
    char **path_split;
    int flag;
    int i;

    tmp = get_idx_node(g_index);
    i = 0;
    flag = 0;
    path_split = ft_split(tmp->real_path, "/");
    while (path_split[i])
    {
        i++;
    }
    for (int k = 0; k < g_diff_result_arr_len; k++)
    {
        if (g_diff_result_arr[k] != 0)
        {
            flag = 1;
            break;
        }
    }
    if (flag == 0)
    {
        printf("File %s and %s/%s are identical\n", g_zero_file.filename, path_split[--i], tmp->filename);
    }
    i = 0;
    while (path_split[i])
    {
        free(path_split[i]);
        i++;
    }
    free(path_split);
}

/* Open the files, count the number of lines, and create array. */
void dir_make_arr_from_files(char *zero_path, char *idx_path)
{
    t_myStatptr tmp;
    FILE *fp1, *fp2;
    int zero_line;
    int idx_line;
    int i;

    tmp = get_idx_node(g_index);
    zero_line = 0;
    idx_line = 0;
    i = 0;
    if ((fp1 = fopen(zero_path, "r")) == NULL)
    {
        perror("fp1");
        return;
    }
    if ((fp2 = fopen(idx_path, "r")) == NULL)
    {
        perror("fp2");
        return;
    }
    else
    {
        /* count line */
        while (fgets(g_file1_content[zero_line], BUFF, fp1))
        {
            zero_line++;
        }
        g_line_zero = zero_line;
        while (fgets(g_file2_content[idx_line], BUFF, fp2))
        {
            idx_line++;
        }
        g_line_idx = idx_line;
    }
    fclose(fp1);
    fclose(fp2);
    if (g_line_zero == 0)
    {
        g_cnt_line_zero = (int *)calloc(1, sizeof(int));
        g_line_zero = 1;
    }
    else if ((g_cnt_line_zero = (int *)calloc(zero_line, sizeof(int))) == NULL)
    {
        printf("malloc err!\n");
        return;
    }
    if (g_line_idx == 0)
    {
        g_cnt_line_idx = (int *)calloc(1, sizeof(int));
        g_line_idx = 1;
    }
    else if ((g_cnt_line_idx = (int *)calloc(idx_line, sizeof(int))) == NULL)
    {
        printf("malloc err!\n");
        return;
    }
}

/* Find the same line through strcmp() and write down the corresponding line number. */
/* If there are multiple identical sentences, put them in the fastest order. */
void dir_find_same_line(char *zero_path, char *idx_path)
{
    FILE *fp1, *fp2;
    char buf1[BUFF], buf2[BUFF];
    int cnt;
    int i;
    t_myStatptr tmp;

    i = 0;
    tmp = get_idx_node(g_index);
    if ((fp1 = fopen(zero_path, "r")) == NULL)
    {
        perror("fp1");
        return;
    }
    if ((fp2 = fopen(idx_path, "r")) == NULL)
    {
        perror("fp2");
        return;
    }

    while (fgets(buf1, BUFF, fp1))
    {
        cnt = 1;
        fseek(fp2, 0, SEEK_SET);
        while (fgets(buf2, BUFF, fp2))
        {
            if (!strcmp(buf1, buf2) && g_used_idx[cnt] != 1)
            {
                g_cnt_line_zero[i] = cnt;
                /* Check the used line */
                g_used_idx[cnt] = 1;
                break;
            }
            cnt++;
        }
        i++;
    }
    memset(&g_used_idx, 0, BUFF);
    fclose(fp1);
    fclose(fp2);
    if ((fp1 = fopen(zero_path, "r")) == NULL)
    {
        perror("fp1");
        return;
    }
    if ((fp2 = fopen(idx_path, "r")) == NULL)
    {
        perror("fp2");
        return;
    }
    i = 0;
    while (fgets(buf1, BUFF, fp2))
    {
        cnt = 1;
        fseek(fp1, 0, SEEK_SET);
        while (fgets(buf2, BUFF, fp1))
        {
            if (!strcmp(buf1, buf2) && g_used_idx[cnt] != 1)
            {
                g_cnt_line_idx[i] = cnt;
                g_used_idx[cnt] = 1;
                break;
            }
            cnt++;
        }
        i++;
    }
    if (g_line_zero == 1 && !strcmp(g_file1_content[0], "\0"))
        g_cnt_line_zero[0] = 1050;
    if (g_line_idx == 1 && !strcmp(g_file2_content[0], "\0"))
        g_cnt_line_idx[0] = 1050;
    fclose(fp1);
    fclose(fp2);
    memset(&g_used_idx, 0, BUFF);
}