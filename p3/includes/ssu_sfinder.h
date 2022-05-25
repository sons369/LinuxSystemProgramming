#ifndef SSU_SFINDER_H
#define SSU_SFINDER_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>
#include <string.h>
#include <openssl/md5.h>
#include <dirent.h>
#include <sys/types.h>
#include <fcntl.h>
#include <errno.h>
#include <pthread.h>
#include <openssl/md5.h>
#include <openssl/sha.h>
#include <sys/time.h>
#include <time.h>
#include <ctype.h>
#include <pwd.h>

#ifndef PATH_MAX
#define PATH_MAX 4097
#endif
#define FILE_MAX 256
#ifndef BUFF_SIZE
#define BUFF_SIZE 1024
#endif

typedef struct Qnode
{
    char dirpath[PATH_MAX];
    struct Qnode *link;
} t_Qnode;

typedef struct
{
    t_Qnode *front, *rear;
} t_Qtype;

typedef struct s_myStat
{
    off_t size;
    int set;
    int idx;
    gid_t gid;
    uid_t uid;
    mode_t mode;
    char *atim;
    char *mtim;
    char hash[60];
    char real_path[PATH_MAX];
    struct s_myStat *next;
} t_myStat;

typedef struct stat t_stat;
typedef t_myStat *t_myStatptr;

/* global variable */
char g_ext_arg[BUFF_SIZE];
char g_min_size[BUFF_SIZE];
char g_max_size[BUFF_SIZE];
char g_dir_arg[PATH_MAX];
char g_opt_c_arg[BUFF_SIZE];
char g_opt_l_arg[BUFF_SIZE];
char g_home_dir[PATH_MAX];
char g_real_path[PATH_MAX];
char g_user_name[BUFF_SIZE];
char *g_trash_file_path;
char *g_trash_info_path;
char g_duplicat_log[BUFF_SIZE];
long g_minsize;
long g_maxsize;
long g_file;
int g_cnt_same_file;
int g_opt_o_arg;
int g_thread;
int g_function; // 1=> fmd5 | 2=> fsha1 | 3=> list | 4=> trash
int g_total_node;
int g_total_set;
int g_del_opt_l;
int g_del_opt_d;
int g_del_opt_i;
int g_del_opt_f;
int g_del_opt_t;
int g_del_set;
int g_del_idx;

t_Qtype *q;
t_myStatptr g_head;

/* split.c function */
int in_sep(char c, char *charset);
int cnt_word(char *str, char *charset);
int len_word(char *str, char *charset);
void make_split(char **arr, char *str, char *charset, int word_cnt);
char **ft_split(char *str, char *charset);

/* queue.c functions */
t_Qtype *create_queue_link(void);
int is_queue_empty(t_Qtype *Q);
void enqueue(t_Qtype *Q, char *dirpath);
char *delqueue(t_Qtype *Q);

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

/* ssu_sfinder.c function */
int get_opt(int argc, char *argv[]);
void print_help();
int user_input(char buf[], char ***split);
void clear_g_var();
void convert_file_size(char *min, char *max);
int chk_arg_error();
void do_md5(FILE *fp, char *path, long fsize);
int search_same_file(char *path, char *ext);
void check_same_file(void);
void option_d(int set, int idx);
int option_i(int set);
void do_sha1(FILE *fp, char *path, long fsize);
void option_f_t(int set, int flag);
int input_error(int argc, char **split);

#endif