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
        stat(path, &statbuf);
        strcpy(newPtr->real_path, path);
        strcpy(newPtr->hash, hash);
        newPtr->atim = get_string_time(statbuf, 1);
        newPtr->mtim = get_string_time(statbuf, 2);
        newPtr->size = size;
        newPtr->next = NULL;

        previousPtr = NULL;
        currentPtr = *sPtr;
        /*find node position order to ASCII and PATH*/
        while (currentPtr != NULL && (newPtr->size > currentPtr->size || strcmp(hash, currentPtr->hash) > 0 || sort_filter(path, currentPtr->real_path) > 0 || strcmp(path, currentPtr->real_path) > 0))
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
t_myStatptr get_idx_node(int idx)
{
    t_myStatptr sPtr;
    int i;

    i = 1;
    sPtr = g_head;
    while (i != idx)
    {
        sPtr = sPtr->next;
        i++;
    }
    return sPtr;
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

void print_node(t_myStatptr sPtr)
{
    while (sPtr != NULL)
    {
        print_file(*sPtr);
        sPtr = sPtr->next;
    }
}

void print_file(t_myStat file)
{
    printf("[%d] %s (mtime : %s) (atime : %s)\n", 1, file.real_path, file.mtim, file.atim);
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