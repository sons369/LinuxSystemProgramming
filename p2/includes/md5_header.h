#ifndef MD5_HEADER_H
#define MD5_HEADER_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define PATH_MAX 4097
#define FILE_MAX 256

#ifndef QUEUE_H
#define QUEUE_H
typedef struct Qnode
{
    char dirpath[PATH_MAX];
    struct Qnode *link;
} t_Qnode;

typedef struct
{
    t_Qnode *front, *rear;
} t_Qtype;

/* queue.c functions */
t_Qtype *create_queue_link(void);
int is_queue_empty(t_Qtype *Q);
void enqueue(t_Qtype *Q, char *dirpath);
char *delqueue(t_Qtype *Q);
#endif

#endif