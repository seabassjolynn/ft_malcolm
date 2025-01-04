#ifndef RESOURCES_H
#define RESOURCES_H

#include <unistd.h>

struct s_resources
{
    int socket;
};

extern struct s_resources g_resources;

void init_resources();

void free_resources();

#endif