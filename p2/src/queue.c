#include "../includes/md5_header.h"
//#include "../includes/sha1_header.h"

/* 큐 초기화 */
t_Qtype *create_queue_link(void)
{
    t_Qtype *q;
    q = (t_Qtype *)malloc(sizeof(t_Qtype));
    q->front = NULL;
    q->rear = NULL;
    return q;
}

/* 만약 큐가 비어있으면 1 리턴 */
int is_queue_empty(t_Qtype *q)
{
    if (q->front == NULL)
        return 1;
    else
        return 0;
}

/* 디렉토리path를 인자로 받아서 큐에 삽입해줌 */
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

/* 삽입된 큐를 꺼내줌. 디렉토리 path를 리턴해줌 */
char *delqueue(t_Qtype *q)
{
    t_Qnode *del = q->front;
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