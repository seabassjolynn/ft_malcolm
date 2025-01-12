#ifndef ARGUMENTS_H
#define ARGUMENTS_H

#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/ip.h>

#include <stdlib.h>
#include "resources.h"
#include "net.h"
#include "libft.h"

/*
target - who sends the arp request
source - the host which information (mac) is requested
parameters:
"source_ip" - the ip of the host for which mac addre is requested
"source_mac" - the mac address that we provide as a replacer (as a poisoning)*/
struct s_arguments
{
    struct in_addr source_ip;
    uint8_t source_mac[6];
    struct in_addr target_ip;
    uint8_t target_mac[6];
};

extern struct s_arguments g_arguments;

void parse_args(int ac, char **av);

void debug_print_arguments();

#endif