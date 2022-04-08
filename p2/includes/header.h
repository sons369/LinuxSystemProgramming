#ifndef HEADER_H
#define HEADER_H

#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <sys/wait.h>
#include <pwd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#include <time.h>
#include <errno.h>
#include <fcntl.h>
#include <ctype.h>

#define BUFF_SIZE 1025
#define PATH_MAX 4097

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

typedef t_myStat *t_myStatptr;

/* global variable */
char g_homedir_path[PATH_MAX];
char g_target_dir[PATH_MAX];

/* ssu_sdup.c function */
int user_input(char buf[], char ***split);
int input_error(int argc, char **split);

/* split.c function */
int in_sep(char c, char *charset);
int cnt_word(char *str, char *charset);
int len_word(char *str, char *charset);
void make_split(char **arr, char *str, char *charset, int word_cnt);
char **ft_split(char *str, char *charset);

/* help.c function */
void print_help();

#endif