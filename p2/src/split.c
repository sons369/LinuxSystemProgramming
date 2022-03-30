#include "../includes/header.h"

/* check char c in sperator */
/* if c is sep then return 1 */
int in_sep(char c, char *charset)
{
    int i;

    i = 0;
    while (charset[i] != '\0')
    {
        if (c == charset[i])
            return (1);
        i++;
    }
    return (0);
}

/* count number of word */
int cnt_word(char *str, char *charset)
{
    int i;
    int cnt;

    i = 0;
    cnt = 0;
    while (str[i] != '\0')
    {
        if (i == 0 && !in_sep(str[i], charset))
            cnt++;
        else if (in_sep(str[i - 1], charset) && !in_sep(str[i], charset))
            cnt++;
        i++;
    }
    return (cnt);
}

/* Measure length of each word */
int len_word(char *str, char *charset)
{
    int i;
    int len;

    i = 0;
    len = 0;
    if (*charset == 0)
    {
        while (str[i] != '\0')
            i++;
        return (i);
    }
    while (!in_sep(str[i], charset))
    {
        len++;
        i++;
    }
    return (len);
}

/* fill word in array */
void make_split(char **arr, char *str, char *charset, int word_cnt)
{
    int i;
    int j;
    int k;

    i = 0;
    j = 0;
    k = 0;
    while (word_cnt)
    {
        while (in_sep(str[i], charset) && str[i])
            i++;
        while (!in_sep(str[i], charset) && str[i])
        {
            arr[j][k] = str[i];
            k++;
            i++;
        }
        arr[j][k] = '\0';
        k = 0;
        j++;
        word_cnt--;
    }
    arr[j] = 0;
}

/* return char[][] */
/* if input ft_split("hello new world!", " !") */
/* return char[][] = {{hello}, {new}, {world}} */
char **ft_split(char *str, char *charset)
{
    char **voca;
    int word_cnt;
    int word_len;
    int k;
    int i;

    word_cnt = cnt_word(str, charset);
    voca = (char **)malloc(sizeof(char *) * (word_cnt + 1));
    k = 0;
    i = 0;
    while (str[i] != '\0')
    {
        if (i == 0 && !in_sep(str[i], charset))
        {
            word_len = len_word(&str[i], charset);
            voca[k++] = (char *)malloc(sizeof(char *) * (word_len + 1));
        }
        else if (in_sep(str[i - 1], charset) && !in_sep(str[i], charset))
        {
            word_len = len_word(&str[i], charset);
            voca[k++] = (char *)malloc(sizeof(char *) * (word_len + 1));
        }
        i++;
    }
    make_split(voca, str, charset, word_cnt);
    i = 0;
    return (voca);
}