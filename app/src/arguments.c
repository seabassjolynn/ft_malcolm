#include "arguments.h"
#define SUCCESS 0
#define ERROR 1
#define OCTETS_NUM 6
#define MAC_CHAR_LEN 17

struct s_arguments g_arguments;
//мне надо перевести строку AA:BB:CC:DD:EE:FF в какой-то массив uint8_t mac[] и как-то соблюсти сетевую последовательно

static int is_hex(char c)
{
    return c >= '0' && c <= '9' || c >= 'a' && c <= 'f' || c >= 'A' && c <= 'F';
}

static int parse_mac(char *mac_str, uint8_t mac_net[])
{
    if (ft_strlen(mac_str) != MAC_CHAR_LEN)
        return ERROR;
    
    int i = 0;

    while (i < OCTETS_NUM)
    {
        int octet_start = i * 3;
        
        char first_hex = mac_str[octet_start];
        char second_hex = mac_str[octet_start + 1];
        
        if (!is_hex(first_hex) || !is_hex(second_hex))
            return ERROR;
        
        uint8_t octet = 0x00;
        octet |= first_hex << 4;
        octet |= (second_hex & 0xf);
        
        mac_net[i] = octet;    
        
        if (mac_str[octet_start + 3] != ':' ||  mac_str[octet_start + 3] != 0)
            return ERROR;
        i++;
    }
    return SUCCESS;
}

void parse_args(int ac, char **av)
{
    if (ac != 4)
        clean_exit_failure("Wrong number of arguments. Expected: source ip, source mac address, target ip, target mac address");

    
    if (inet_pton(AF_INET, av[1], &g_arguments.source_ip) != 1)
        clean_exit_failure("Failed to parse source ip");

}