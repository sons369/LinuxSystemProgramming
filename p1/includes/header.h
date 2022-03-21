#ifndef HEADER_H
#define HEADER_H

#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <unistd.h>
#include <string.h>
#include <time.h>
#include <fcntl.h>
#include <libgen.h> //dirname()
#include <errno.h>
#include <stdbool.h>
#include <ctype.h> //isdigit()

#define BUFF 1024
#define PATH_MAX 4096

typedef struct s_fileinfo
{
    char filename[BUFF];
    char real_filename[PATH_MAX];
    char path[PATH_MAX];
    char real_path[PATH_MAX];
} t_fileinfo;

typedef struct s_myStat
{
    mode_t st_mode;          /* File type and mode */
    nlink_t st_nlink;        /* Number of hard links */
    uid_t st_uid;            /* User ID of owner */
    gid_t st_gid;            /* Group ID of owner */
    off_t st_size;           /* Total size, in bytes */
    blksize_t st_blksize;    /* Block size for filesystem I/O */
    blkcnt_t st_blocks;      /* Number of 512B blocks allocated */
    struct timespec st_atim; /* Time of last access */
    struct timespec st_mtim; /* Time of last modification */
    struct timespec st_ctim; /* Time of last status change */

#define st_atime st_atim.tv_sec /* Backward compatibility */
#define st_mtime st_mtim.tv_sec
#define st_ctime st_ctim.tv_sec
    char *atim;
    char *ctim;
    char *mtim;
    char *permission;
    char filename[BUFF];
    char real_path[PATH_MAX];
    char path_filename[PATH_MAX];
    struct s_myStat *next;
} t_myStat;

typedef struct stat t_stat;
typedef t_myStat *t_myStatptr;

/* global variable */
t_fileinfo g_info;
t_stat g_tmp;
t_myStat g_zero_file;
t_myStat g_mtmp;
t_myStatptr g_head;
int g_chk_find;
int g_index;
int *g_cnt_line_zero;
int g_line_zero;
int *g_cnt_line_idx;
int g_line_idx;
int *g_diff_result_arr;
int g_diff_result_arr_len;
char g_home_dir[BUFF];
char g_dir_path_buf[BUFF];
char g_file1_content[BUFF][BUFF];
char g_file2_content[BUFF][BUFF];
bool g_option[4]; /* 0: q, 1: s, 2: i, 3: r */
bool g_used_idx[BUFF];
bool g_is_zero_dir_flag;

/* print.c function */
void prompt();
void index_prompt();
char *input_function();
void get_info(char *str[]);
int ft_case(char *str);
void print_file(t_myStat file, int i);

/* split.c function */
int in_sep(char c, char *charset);
int cnt_word(char *str, char *charset);
int len_word(char *str, char *charset);
void make_split(char **arr, char *str, char *charset, int word_cnt);
char **ft_split(char *str, char *charset);

/* help.c function */
void print_help();

/* find.c function */
char *convert_path(char *path);
void print_first_line();
void get_info_file(t_stat st, t_myStat *myst);
char *get_string_time(t_stat st_t, int flag);
char *get_permission(mode_t mode);
int find_file();
void index_zero_file();
void index_same_file(char *name);
int filter(const struct dirent *info);
void index_same_dir(char *name);
long get_dir_size(char *path);

/* diff.c function */
int input_index_option(char *str);
void make_arr_from_files();
void find_same_line();
void verification_arr();
void make_result_arr();
void memset_diff_global_variable();
void print_diff_result();
void print_diff_add(int idx, int start, int end);
void print_diff_change(int n_start, int n_end, int p_start, int p_end);
void print_diff_delete(int idx, int start, int end);

/* list.c function */
void insert(t_myStatptr *sPtr, char *path);
void free_all_node(t_myStatptr *sPtr);
void print_node(t_myStatptr sPtr);
int sort_filter(char *s1, char *s2);
t_myStatptr get_idx_node(int idx);

#endif