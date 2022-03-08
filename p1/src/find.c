#include "../includes/header.h"

char *convert_path(char *path)
{
    char *real_path;

    real_path = realpath(path, NULL);
    return real_path;
}
