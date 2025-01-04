#include "resources.h"

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