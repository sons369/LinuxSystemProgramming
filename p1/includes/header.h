#ifndef HEADER_H
#define HEADER_H

#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <sys/time.h>
#include <unistd.h>
#include <string.h>

#define BUFF 1024

/* print.c function */
void prompt();
char *input_function();
int ft_case(char *str);

/* help.c function */
void print_help();

#endif