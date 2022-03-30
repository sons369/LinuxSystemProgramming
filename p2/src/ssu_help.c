#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>

void print_help();

int main(int argc, char *argv[])
{
	print_help();
	exit(0);
}

/* print manual page */
void print_help()
{
	printf("Usage:\n");
	printf("\t> fmd5/fsha1 [FILE_EXTENSION] [MINSIZE] [MAXSIZE] [TARGET_DIRECTORY]\n");
	printf("\t  >> [SET_INDEX] [OPTION ...]\n");
	printf("\t     [OPTION ...]\n");
	printf("\t     d [LIST_IDX] : delete [LIST_IDX] file\n");
	printf("\t     i : ask for confirmation before delete\n");
	printf("\t     f : force delete except the recently modified file\n");
	printf("\t     t : force move to Trash except the recently modified file\n");
	printf("\t> help\n");
	printf("\t> exit\n");
}