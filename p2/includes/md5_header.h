#ifndef MD5_HEADER_H
#define MD5_HEADER_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <openssl/md5.h>
#include <dirent.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/stat.h>

#define PATH_MAX 4096
#define FILE_MAX 256
#define BUFSIZE 1024 * 16

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

/* global variable */
long g_min_size;
long g_max_size;

/* ssu_find-md5.c function */
void convert_file_size(char *min, char *max);
int filter(const struct dirent *info);
void pt(unsigned char *md);
void do_md5(FILE *fp);
int search_same_file(char *path);

#endif