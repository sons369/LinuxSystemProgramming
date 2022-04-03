#include "../includes/md5_header.h"
#include "../includes/sha1_header.h"

int is_queue_empty(t_Qtype *q)
{
    if (q->front == NULL)
        return 1;
    else
        return 0;
}

void enqueue(t_Qtype *q, char *dirpath)
{
    t_Qnode *new;

    new = (t_Qnode *)malloc(sizeof(t_Qnode));
    strcpy(new->dirpath, dirpath);
    new->link = NULL;
    if (q->front == NULL)
    {
        q->front = new;
        q->rear = new;
    }
    else
    {
        q->rear->link = new;
        q->rear = new;
    }
}

char *delqueue(t_Qtype *q)
{
    t_Qnode *del;
    char *path;

    if (is_queue_empty(q))
        return NULL;
    else
    {
        if ((path = (char *)malloc(PATH_MAX)) == NULL)
        {
            printf("malloc error\n");
            return NULL;
        }
        strcpy(path, del->dirpath);
        q->front = q->front->link;
        if (q->front == NULL)
        {
            q->rear = NULL;
        }
        free(del);
        return path;
    }
}