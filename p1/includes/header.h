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

#define BUFF 1024
#define PATH_MAX 4096

typedef struct s_fileinfo
{
    char filename[BUFF];
    char real_filename[PATH_MAX];
    char path[PATH_MAX];
    char real_path[PATH_MAX];
} t_fileinfo;

typedef struct stat t_stat;

/* global variable */
t_fileinfo g_info;

/* print.c function */
void prompt();
char *input_function();
void get_info(char *str[]);
int ft_case(char *str);

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
int find_index_zero();
void index_zero_file();

#endif