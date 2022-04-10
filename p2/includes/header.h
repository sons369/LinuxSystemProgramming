#ifndef HEADER_H
#define HEADER_H

#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <pwd.h>
#include <sys/types.h>
#include <unistd.h>
#include <string.h>
#include <time.h>
#include <errno.h>
#include <fcntl.h>
#include <ctype.h>

#define BUFF_SIZE 1025
#ifndef PATH_MAX
#define PATH_MAX 4097
#endif

/* global variable */
char g_homedir_path[PATH_MAX];
char g_target_dir[PATH_MAX];

/* ssu_sdup.c function */
int user_input(char buf[], char ***split);
int input_error(int argc, char **split);

#ifndef SPLIT
#define SPLIT
/* split.c function */
int in_sep(char c, char *charset);
int cnt_word(char *str, char *charset);
int len_word(char *str, char *charset);
void make_split(char **arr, char *str, char *charset, int word_cnt);
char **ft_split(char *str, char *charset);
#endif

/* help.c function */
void print_help();

#endif