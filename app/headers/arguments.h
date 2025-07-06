#ifndef ARGUMENTS_H
#define ARGUMENTS_H

#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/ip.h>

#include <stdlib.h>
#include "resources.h"
#include "net.h"
#include "libft.h"

#define MAC_ADDRSTRLEN 18

/*
target - who sends the arp request
source - the host which information (mac) is requested
parameters:
"source_ip" - the ip of the host for which mac addre is requested
"source_mac" - the mac address that we provide as a replacer (as a poisoning)*/
struct s_arguments
{
    struct in_addr requested_in_addr;
    char requested_str_ip[INET_ADDRSTRLEN];
    uint8_t spoofed_mac_binary[6];
    char spoofed_mac_str[MAC_ADDRSTRLEN];
    struct in_addr victim_in_addr;
    char victim_str_ip[INET_ADDRSTRLEN];
    uint8_t victim_mac[6];
    char victim_mac_str[MAC_ADDRSTRLEN];
    char *local_network_interface;
    struct in_addr local_ip_net;
    char local_ip_str[INET_ADDRSTRLEN];
    uint8_t local_mac_binary[6];
    char local_mac_str[MAC_ADDRSTRLEN];
};

extern struct s_arguments g_arguments;

void parse_args(int ac, char **av);

#endif