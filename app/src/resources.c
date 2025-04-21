#include "resources.h"
#include "stdlib.h"
#include <string.h>
#include <errno.h>
#include <stdio.h>
struct s_resources g_resources;

void init_resources()
{
    g_resources.socket = -1;
}

void free_resources()
{
    if (g_resources.socket != -1)
    {
        close(g_resources.socket);
    }
}

void clean_exit_failure(char *error)
{
    printf(COLOR_RESET);
    printf("%s. Errno value: %s\n", error, strerror(errno));
    free_resources();
    exit(EXIT_FAILURE);
}

void clean_exit_success()
{
    free_resources();
    exit(EXIT_FAILURE);
}