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
#include <time.h>
#include <sys/time.h>

#ifndef PATH_MAX
#define PATH_MAX 4097
#endif
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

#ifndef LIST_H
#define LIST_H
typedef struct s_myStat
{
    off_t size;
    char *atim;
    char *mtim;
    char hash[60];
    char real_path[PATH_MAX];
    struct s_myStat *next;
} t_myStat;

typedef struct stat t_stat;
typedef t_myStat *t_myStatptr;

/* list.c function */
void insert(t_myStatptr *sPtr, char *path, char *hash, long size);
void free_all_node(t_myStatptr *sPtr);
t_myStatptr get_idx_node(int idx);
void print_node(t_myStatptr sPtr);
int sort_filter(char *s1, char *s2);
char *get_string_time(t_stat st_t, int flag);
int is_hash(char *hash);
void print_file(t_myStat file);
#endif

/* global variable */
long g_min_size;
long g_max_size;
long g_file_cnt;
long g_file;
int g_cnt_same_file;
t_myStatptr g_head;

/* ssu_find-md5.c function */
void convert_file_size(char *min, char *max);
void do_md5(FILE *fp, char *path, long fsize);
int search_same_file(char *path, char *ext);
void check_same_file(void);

#ifndef SPLIT
#define SPLIT
/* split.c function */
int in_sep(char c, char *charset);
int cnt_word(char *str, char *charset);
int len_word(char *str, char *charset);
void make_split(char **arr, char *str, char *charset, int word_cnt);
char **ft_split(char *str, char *charset);
#endif

#endif