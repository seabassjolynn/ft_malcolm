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

static uint8_t hex_to_byte(unsigned char hex)
{
    if (hex >= '0' && hex <= '9')
        return hex - '0';
    if (hex >= 'a' && hex <= 'f')
        return hex - 'a' + 10;
    if (hex >= 'A' && hex <= 'F')
        return hex - 'A' + 10;

    clean_exit_failure("Failed to convert hex to byte");    
    return 0;
}

static int is_hex(char c)
{
    return (c >= '0' && c <= '9') || (c >= 'a' && c <= 'f') || (c >= 'A' && c <= 'F');
}

static void parse_mac(char *mac_str, uint8_t mac_net[])
{
    if (ft_strlen(mac_str) != MAC_CHAR_LEN)
        clean_exit_failure("Mac address len is not 17");
    
    int i = 0;

    while (i < OCTETS_NUM)
    {
        int octet_start = i * 3;
        
        char first_hex = mac_str[octet_start];
        char second_hex = mac_str[octet_start + 1];
        
        if (!is_hex(first_hex) || !is_hex(second_hex))
            clean_exit_failure("Error when parsing mac: symbol doesn't represent hex");
        
        uint8_t octet = 0x00;
        octet |= hex_to_byte(first_hex) << 4;
        octet |= (hex_to_byte(second_hex) & 0xf);
        
        mac_net[i] = octet;
        char c = mac_str[octet_start + 2];
        (void)c;
        if (!(mac_str[octet_start + 2] == ':' ||  mac_str[octet_start + 2] == 0))
            clean_exit_failure("Error when parsing mac: haven't found delimiter \":\"");
        i++;
    }
}

void parse_args(int ac, char **av)
{
    if (ac != 5)
        clean_exit_failure("Wrong number of arguments. Expected: source ip, source mac address, target ip, target mac address");

    
    if (inet_pton(AF_INET, av[1], &g_arguments.source_ip) != 1)
        clean_exit_failure("Failed to parse source ip");

    parse_mac(av[2], g_arguments.source_mac);

    if (inet_pton(AF_INET, av[3], &g_arguments.target_ip) != 1)
        clean_exit_failure("Failed to parse destination ip");

    parse_mac(av[4], g_arguments.target_mac);
}

void debug_print_arguments() {
    DEBUG_LOG("Source mac: %x:%x:%x:%x:%x:%x", g_arguments.source_mac[0], g_arguments.source_mac[1], g_arguments.source_mac[2], g_arguments.source_mac[3], g_arguments.source_mac[4], g_arguments.source_mac[5]);
    DEBUG_LOG("Target mac: %x:%x:%x:%x:%x:%x", g_arguments.target_mac[0], g_arguments.target_mac[1], g_arguments.target_mac[2], g_arguments.target_mac[3], g_arguments.target_mac[4], g_arguments.target_mac[5]);
}