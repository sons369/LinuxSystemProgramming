#include "../includes/header.h"

void print_help()
{
	printf("Usage:\n");
	printf("\t> find [FILENAME] [PATH]\n");
	printf("\t> help\n");
	printf("\t> exit\n\n");
	printf("\t[OPTION ... ]\n");
	printf("\tq : report only when files differ\n");
	printf("\ts : report when two files are the same\n");
	printf("\ti : ignore case differences in file contents\n");
	printf("\tr : recursively compare any subdirectories found\n");
}