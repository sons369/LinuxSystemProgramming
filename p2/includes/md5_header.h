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

typedef struct s_myStat
{
    off_t st_size;
    char *atim;
    char *mtim;
    char *md5;
    char *sha1;
    char real_path[PATH_MAX];
    struct s_myStat *next;
} t_myStat;

typedef struct stat t_stat;
typedef t_myStat *t_myStatptr;

/* global variable */
long g_min_size;
long g_max_size;
long g_file_cnt;
long g_file;
t_myStatptr g_head;

/* ssu_find-md5.c function */
void convert_file_size(char *min, char *max);
void do_md5(FILE *fp, char *path, long fsize);
int search_same_file(char *path, char *ext);

/* split.c function */
int in_sep(char c, char *charset);
int cnt_word(char *str, char *charset);
int len_word(char *str, char *charset);
void make_split(char **arr, char *str, char *charset, int word_cnt);
char **ft_split(char *str, char *charset);

#endif