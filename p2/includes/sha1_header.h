#ifndef SHA1_HEADER_H
#define SHA1_HEADER_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <openssl/sha.h>
#include <dirent.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/stat.h>
#include <time.h>
#include <sys/time.h>
#include <ctype.h>

#ifndef PATH_MAX
#define PATH_MAX 4097
#endif
#define FILE_MAX 256
#define BUFSIZE 1024 * 16
#define BUFF_SIZE 1024

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
    int set;
    int idx;
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
int is_set_idx_node(int set, int idx);
void print_node(t_myStatptr sPtr);
int is_set_node(int set);
int sort_filter(char *s1, char *s2);
char *get_string_time(t_stat st_t, int flag);
int is_hash(char *hash);
void print_file(t_myStat file, int i);
void make_comma_num(long num, char *result);
void cnt_set_idx_num(t_myStatptr sPtr);
char *delete_node(t_myStatptr *sPtr, int set, int idx);
int cnt_hash(char *hash);
int cnt_set_node(int set);
char *get_node_path(int set, int idx);
int get_latest_mtim_idx(int set);
char *get_latest_mtim(int set, int idx);
#endif

/* global variable */
long g_min_size;
long g_max_size;
long g_file_cnt;
long g_file;
int g_cnt_same_file;
int g_total_set;
int g_total_node;
t_myStatptr g_head;

/* ssu_find-sha1.c function */
void convert_file_size(char *min, char *max);
void do_sha1(FILE *fp, char *path, long fsize);
int search_same_file(char *path, char *ext);
void check_same_file(void);
int input_error(int argc, char **split);
int user_input(char buf[], char ***split);
void option_d(int set, int idx);
int option_i(int set);
void option_f_t(int set, int flag);

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