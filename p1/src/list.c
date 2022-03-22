#include "../includes/header.h"

/* insert node function */
void insert(t_myStatptr *sPtr, char *path)
{
    t_myStatptr newPtr;
    t_myStatptr previousPtr;
    t_myStatptr currentPtr;

    newPtr = malloc(sizeof(t_myStat));

    if (newPtr != NULL)
    {
        strcpy(newPtr->real_path, path);
        get_info_file(g_tmp, newPtr);
        if (g_is_zero_dir_flag == 1)
        {
            newPtr->st_size = g_mtmp.st_size;
        }
        newPtr->permission = get_permission(newPtr->st_mode);
        newPtr->atim = get_string_time(g_tmp, 1);
        newPtr->ctim = get_string_time(g_tmp, 2);
        newPtr->mtim = get_string_time(g_tmp, 3);
        newPtr->next = NULL;

        previousPtr = NULL;
        currentPtr = *sPtr;
        /*find node position order to ASCII and PATH*/
        while (currentPtr != NULL && (sort_filter(path, currentPtr->real_path) > 0 || strcmp(path, currentPtr->real_path) > 0))
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

void print_node(t_myStatptr sPtr)
{
    int i;

    i = 1;
    while (sPtr != NULL)
    {
        print_file(*sPtr, i);
        i++;
        sPtr = sPtr->next;
    }
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