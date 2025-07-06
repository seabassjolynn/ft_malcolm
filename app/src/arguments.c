#include "arguments.h"
#include "resources.h"
#include "debug.h"
#include "net.h"

#define SUCCESS 0
#define ERROR -1
#define OCTETS_NUM 6

struct s_arguments g_arguments;

void parse_args(int ac, char **av)
{
    if (ac != 5)
    {
        char error[] = "Wrong number of arguments. Expected following arguments:\n"
        "1. ip for which victim wants to know mac\n"
        "2. spoofed mac - mac that will be returned to victim instead of mac of real requested host\n"
        "3. victim ip\n"
        "4. victim mac address\n";
        clean_exit_failure(error);
    }

    if (inet_pton(AF_INET, av[1], &g_arguments.requested_in_addr) != 1)
        clean_exit_failure("Failed to parse source ip");

    ipv4_net_to_str(&g_arguments.requested_in_addr, g_arguments.requested_str_ip);

    str_mac_to_binary(av[2], g_arguments.spoofed_mac_binary);
    ft_strlcpy(g_arguments.spoofed_mac_str, av[2], MAC_ADDRSTRLEN);

    if (inet_pton(AF_INET, av[3], &g_arguments.victim_in_addr) != 1)
        clean_exit_failure("Failed to parse destination ip");

    ipv4_net_to_str(&g_arguments.victim_in_addr, g_arguments.victim_str_ip);

    str_mac_to_binary(av[4], g_arguments.victim_mac);
    ft_strlcpy(g_arguments.victim_mac_str, av[4], MAC_ADDRSTRLEN);

    g_arguments.local_network_interface = "eth0";
}
