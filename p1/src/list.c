#include "../includes/header.h"

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
        newPtr->permission = get_permission(newPtr->st_mode);
        newPtr->atim = get_string_time(g_tmp, 1);
        newPtr->ctim = get_string_time(g_tmp, 2);
        newPtr->mtim = get_string_time(g_tmp, 3);
        newPtr->next = NULL;

        previousPtr = NULL;
        currentPtr = *sPtr;

        while (currentPtr != NULL && (strcmp(path, currentPtr->real_path) > 0))
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