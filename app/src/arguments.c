#include "arguments.h"
#include "resources.h"
#include "debug.h"
#include "net.h"

#define SUCCESS 0
#define ERROR -1
#define OCTETS_NUM 6
#define MAC_CHAR_LEN 17

struct s_arguments g_arguments;
//мне надо перевести строку AA:BB:CC:DD:EE:FF в какой-то массив uint8_t mac[] и как-то соблюсти сетевую последовательно

/*  source ip           - this is an ip for which victim wants to know mac
    source mac address  - spoofed mac
    victim ip           - who sends an ARP request
    victim mac address  - who sends an ARP request
    example of call: ./ft_malcolm 172.18.0.4 aa:aa:aa:aa:aa:aa 172.18.0.2 02:42:ac:12:00:02
    victim - computer # 1
*/
void parse_args(int ac, char **av)
{
    if (ac != 5)
        clean_exit_failure("Wrong number of arguments. Expected: source ip, source mac address, target ip, target mac address");

    if (inet_pton(AF_INET, av[1], &g_arguments.requested_in_addr) != 1)
        clean_exit_failure("Failed to parse source ip");

    ipv4_net_to_str(&g_arguments.requested_in_addr, g_arguments.requested_str_ip);

    str_mac_to_binary(av[2], g_arguments.spoofed_mac);
    ft_strlcpy(g_arguments.spoofed_mac_str, av[2], MAC_CHAR_LEN);

    if (inet_pton(AF_INET, av[3], &g_arguments.victim_in_addr) != 1)
        clean_exit_failure("Failed to parse destination ip");

    ipv4_net_to_str(&g_arguments.victim_in_addr, g_arguments.victim_str_ip);

    str_mac_to_binary(av[4], g_arguments.victim_mac);
    ft_strlcpy(g_arguments.victim_mac_str, av[4], MAC_CHAR_LEN);

    g_arguments.interface = "eth0";
}

void debug_print_arguments() {
    DEBUG_LOG("Source mac: %x:%x:%x:%x:%x:%x", g_arguments.spoofed_mac[0], g_arguments.spoofed_mac[1], g_arguments.spoofed_mac[2], g_arguments.spoofed_mac[3], g_arguments.spoofed_mac[4], g_arguments.spoofed_mac[5]);
    DEBUG_LOG("Target mac: %x:%x:%x:%x:%x:%x", g_arguments.victim_mac[0], g_arguments.victim_mac[1], g_arguments.victim_mac[2], g_arguments.victim_mac[3], g_arguments.victim_mac[4], g_arguments.victim_mac[5]);
}