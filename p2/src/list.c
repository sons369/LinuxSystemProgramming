#include "../includes/md5_header.h"

/* insert node function */
void insert(t_myStatptr *sPtr, char *path, char *hash, long size)
{
    t_myStatptr newPtr;
    t_myStatptr previousPtr;
    t_myStatptr currentPtr;
    t_stat statbuf;

    newPtr = malloc(sizeof(t_myStat));

    if (newPtr != NULL)
    {
        /* copy informations to node */
        if ((stat(path, &statbuf)) == -1)
            perror("stat link");
        strcpy(newPtr->real_path, path);
        strcpy(newPtr->hash, hash);
        newPtr->atim = get_string_time(statbuf, 1);
        newPtr->mtim = get_string_time(statbuf, 2);
        newPtr->size = size;
        newPtr->next = NULL;

        previousPtr = NULL;
        currentPtr = *sPtr;
        /*find node position order to ASCII and PATH*/
        while (currentPtr != NULL && newPtr->size > currentPtr->size)
        {
            previousPtr = currentPtr;
            currentPtr = currentPtr->next;
        }
        while (currentPtr != NULL && !strcmp(newPtr->hash, currentPtr->hash) && (sort_filter(newPtr->real_path, currentPtr->real_path) > 0 || strcmp(newPtr->real_path, currentPtr->real_path) > 0))
        {
            previousPtr = currentPtr;
            currentPtr = currentPtr->next;
        }
        if (previousPtr == NULL)
        {
            newPtr->next = *sPtr;
            *sPtr = newPtr;
        }
        else
        {
            previousPtr->next = newPtr;
            newPtr->next = currentPtr;
        }
    }
}

/* free head and head point NULL */
void free_all_node(t_myStatptr *sPtr)
{
    t_myStatptr tmp;

    while (*sPtr != NULL)
    {
        tmp = *sPtr;
        *sPtr = (*sPtr)->next;
        free(tmp->atim);
        free(tmp->mtim);
        free(tmp);
    }
    *sPtr = NULL;
}

/* if user input index then get idx's node */
int is_set_idx_node(int set, int idx)
{
    t_myStatptr sPtr;

    sPtr = g_head;
    while (sPtr != NULL)
    {
        if (sPtr->set == set && sPtr->idx == idx)
            return 1;
        sPtr = sPtr->next;
    }
    return 0;
}

int is_set_node(int set)
{
    t_myStatptr sPtr;

    sPtr = g_head;
    while (sPtr != NULL)
    {
        if (sPtr->set == set)
            return 1;
        sPtr = sPtr->next;
    }
    return 0;
}

int is_hash(char *hash)
{
    t_myStatptr sPtr;
    sPtr = g_head;
    if (sPtr == NULL)
        return 0;
    while (sPtr->next != NULL)
    {
        sPtr = sPtr->next;
        if (!strcmp(sPtr->hash, hash))
            return 1;
    }
    return 0;
}

int cnt_hash(char *hash)
{
    t_myStatptr sPtr;
    int cnt;

    cnt = 0;
    sPtr = g_head;
    if (sPtr == NULL)
        return 0;
    while (sPtr != NULL)
    {
        if (!strcmp(sPtr->hash, hash))
            cnt++;
        sPtr = sPtr->next;
    }
    return cnt;
}

void print_node(t_myStatptr sPtr)
{
    int flag;
    char hash_buf[50];
    char atol_num[30];
    long size_buf;
    int set;
    int i;
    t_myStat tmp;

    flag = 0;
    set = 1;
    while (sPtr != NULL)
    {
        tmp = *sPtr;
        if (cnt_hash(tmp.hash) == 1)
        {
            sPtr = sPtr->next;
            tmp.set = 0;
            tmp.idx = 0;
            continue;
        }
        if (flag == 0)
        {
            make_comma_num(sPtr->size, atol_num);
            printf("\n---- Identical files #%d (%s bytes - %s) ----\n", set, atol_num, sPtr->hash);
            flag = 1;
            i = 1;
            set++;
        }
        sPtr->set = set - 1;
        sPtr->idx = i;
        if (sPtr->next != NULL && strcmp(sPtr->hash, sPtr->next->hash))
        {
            flag = 0;
        }
        print_file(*sPtr, i);
        i++;
        sPtr = sPtr->next;
    }
}

void print_file(t_myStat file, int i)
{
    printf("[%d] %s (mtime : %s) (atime : %s)%d %d\n", i, file.real_path, file.mtim, file.atim, file.set, file.idx);
}

/* Linked list sort filter */
/* sort by number of '/' */
int sort_filter(char *s1, char *s2)
{
    int i;
    int cnt1;
    int cnt2;

    i = 0;
    cnt1 = 0;
    cnt2 = 0;
    while (s1[i])
    {
        if (s1[i] == '/')
            cnt1++;
        i++;
    }
    i = 0;
    while (s2[i])
    {
        if (s2[i] == '/')
            cnt2++;
        i++;
    }

    return (cnt1 - cnt2);
}

char *get_string_time(t_stat st_t, int flag)
{
    char *s;
    struct tm *t;

    s = (char *)malloc(sizeof(char) * 20);
    if (flag == 1)
        t = localtime(&st_t.st_atim.tv_sec);
    else if (flag == 2)
        t = localtime(&st_t.st_mtim.tv_sec);
    // strftime(s, 30, "%y-%m-%d %H:%M", t);
    sprintf(s, "%04d-%02d-%02d %02d:%02d:%02d",
            t->tm_year + 1900, t->tm_mon + 1, t->tm_mday,
            t->tm_hour, t->tm_min, t->tm_sec);
    return s;
}

void make_comma_num(long num, char *result)
{
    long i;
    char str[30], *p;

    if (num < 0)
    {
        num *= -1;
        *result++ = '-';
    }
    else if (num == 0)
    {
        *result++ = '0';
        *result = 0;
        return;
    }

    p = str;
    for (i = 0; i < 16 && num > 0; i++)
    {
        if (i && (i % 3) == 0)
            *p++ = ',';
        *p++ = (num % 10) + '0';
        num /= 10;
    }
    p--;
    while (p >= str)
        *result++ = *p--;
    *result = 0;
}

void cnt_set_idx_num(t_myStatptr sPtr)
{
    int flag;

    flag = 0;
    g_total_set = 0;
    g_total_node = 0;
    while (sPtr != NULL)
    {
        if (flag == 0)
        {
            g_total_set++;
            flag = 1;
        }
        if (sPtr->next != NULL && strcmp(sPtr->hash, sPtr->next->hash))
        {
            flag = 0;
        }
        g_total_node++;
        sPtr = sPtr->next;
    }
}

char *delete_node(t_myStatptr *sPtr, int set, int idx)
{
    t_myStatptr previous;
    t_myStatptr current;
    t_myStatptr temp;
    char *path;

    path = (char *)malloc(4048);
    if ((*sPtr)->set == set && (*sPtr)->idx == idx)
    {
        temp = *sPtr;
        strcpy(path, temp->real_path);
        *sPtr = (*sPtr)->next;
        free(temp->atim);
        free(temp->mtim);
        free(temp);
        return path;
    }
    else
    {
        previous = *sPtr;
        current = (*sPtr)->next;
        while (current != NULL && (current->set != set || current->idx != idx))
        {
            previous = current;
            current = current->next;
        }
        if (current != NULL)
        {
            temp = current;
            strcpy(path, temp->real_path);
            previous->next = current->next;
            free(temp->atim);
            free(temp->mtim);
            free(temp);
            return path;
        }
    }
    return NULL;
}