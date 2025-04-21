#include "net.h"

void get_local_mac_and_ip(void)
{
    struct ifaddrs *ifaddrs;
    struct ifaddrs *ifaddrs_tmp;

    if (getifaddrs(&ifaddrs) == -1)
        clean_exit_failure("Failed to read available network interfaces");
    
    ifaddrs_tmp = ifaddrs;
    while (ifaddrs_tmp)
    {
        if (ifaddrs_tmp->ifa_addr != NULL)
        {
            if (ifaddrs_tmp->ifa_addr->sa_family == AF_PACKET && ft_strncmp(ifaddrs_tmp->ifa_name, "eth0", 4) == 0)
            {
                struct sockaddr_ll *addr = (struct sockaddr_ll *)ifaddrs_tmp->ifa_addr;
                ft_memcpy(&g_arguments.local_mac_binary, addr->sll_addr, MAC_ADDR_BYTE_LEN);
                binary_mac_to_str(g_arguments.local_mac_binary, g_arguments.local_mac_str);
            }
            else if (ifaddrs_tmp->ifa_addr->sa_family == AF_INET && ft_strncmp(ifaddrs_tmp->ifa_name, "eth0", 4) == 0)
            {
                struct sockaddr_in *addr = (struct sockaddr_in *)ifaddrs_tmp->ifa_addr;
                ft_memcpy(&g_arguments.local_ip_net, &addr->sin_addr, sizeof(struct in_addr));
                ipv4_net_to_str(&g_arguments.local_ip_net, g_arguments.local_ip_str);
            }
        }

        ifaddrs_tmp = ifaddrs_tmp->ifa_next;
    }
    freeifaddrs(ifaddrs);
}

static void print_address(struct sockaddr *sockaddr)
{
    printf("Address family: ");
    switch (sockaddr->sa_family)
    {
        case AF_INET:
        {
            printf("AF_INET. ");
            struct sockaddr_in *addr = (struct sockaddr_in*) sockaddr;
            char str_addr[INET_ADDRSTRLEN];
            ft_bzero(str_addr, sizeof(str_addr));
            if (inet_ntop(AF_INET, &addr->sin_addr, str_addr, sizeof(str_addr)) != NULL)
                printf("Address: %s\n", str_addr);
            else
                printf("Address: %s\n", "NULL");
            break;
        }
        case AF_INET6:  // IPv6 address
        {
            printf("AF_INET6. ");
            struct sockaddr_in6 *addr = (struct sockaddr_in6 *) sockaddr;
            char str_addr[INET6_ADDRSTRLEN];
            memset(str_addr, 0, sizeof(str_addr));  // Use memset or bzero
            if (inet_ntop(AF_INET6, &addr->sin6_addr, str_addr, sizeof(str_addr)) != NULL)
                printf("Address: %s\n", str_addr);
            else
                printf("Address: %s\n", "NULL");
            break;
        }
        case AF_PACKET:  // Link-layer (MAC) address
        {
            printf("AF_PACKET. ");
            struct sockaddr_ll *addr = (struct sockaddr_ll *) sockaddr;
            printf("MAC Address: ");
            for (int i = 0; i < addr->sll_halen; i++) {
                printf("%02x", addr->sll_addr[i]);
                if (i < addr->sll_halen - 1)
                    printf(":");
            }
            printf("\n");
            break;
        }
        default:
            printf("Unknown address family\n");
    
    }
}

void print_network_intefaces()
{
    struct ifaddrs *ifaddrs;
    struct ifaddrs *ifaddrs_tmp;

    if (getifaddrs(&ifaddrs) == -1)
        clean_exit_failure("Failed to read available network interfaces");
    printf("--- Available network interfaces---\n");
    
    ifaddrs_tmp = ifaddrs;
    
    while (ifaddrs_tmp)
    {
        printf("name: %s\n", ifaddrs_tmp->ifa_name);
        if (ifaddrs_tmp->ifa_addr != NULL)
            print_address(ifaddrs_tmp->ifa_addr);
        else
            printf("No address info");
        
        ifaddrs_tmp = ifaddrs_tmp->ifa_next;
    }
    
    freeifaddrs(ifaddrs);
}



void print_mac(uint8_t *arr)
{
    printf("%x:%x:%x:%x:%x:%x", arr[0], arr[1], arr[2], arr[3], arr[4], arr[5]);
}

void print_arp_packet(struct s_arp_packet *packet)
{
    printf("Received ARP packet\n");
    
    uint16_t link_level_proto = ntohs(packet->hardware_type);
    if (link_level_proto == ARPHRD_ETHER)
    {
        printf("Data link layer protocol: Ethernet\n");
    }
    else
    {
        printf("Data link layer protocol: unknown\n");
    }
    
    uint16_t internetwork_proto = ntohs(packet->protocol_type);
    if (internetwork_proto == 0x0800)
    {
        printf("Internetwork protocol for which the ARP request is intended: IPV4\n");
    }
    else
    {
        printf("Internetwork protocol for which the ARP request is intended: Unknown\n");
    }
    
    printf("Sender mac: ");
    print_mac(packet->sender_hardware_addr);
    printf("\n");

    char ipv4_addr_str[INET_ADDRSTRLEN];
    ft_bzero(ipv4_addr_str, INET_ADDRSTRLEN);
    
    ipv4_net_to_str((struct in_addr *)&packet->sender_protocol_addr, ipv4_addr_str);
    printf("Sender IPV4 addr is: %s\n", ipv4_addr_str);

    printf("Dst mac: ");
    print_mac(packet->target_hardware_addr);
    printf("\n");

    ipv4_net_to_str((struct in_addr *)&packet->target_protocol_addr, ipv4_addr_str);
    printf("Dst IPV4 addr is: %s\n", ipv4_addr_str);
    
    if (ntohs(packet->operation) == 1)
    {
        printf("Arp operation: request\n");
    }
    else if (ntohs(packet->operation) == ARP_OPERATION_REPLY)
    {
        printf("Arp operation: reply\n");
    }
    else
    {
        printf("Arp operation: unknown\n");
    }
}

bool macs_equal(uint8_t mac1[], uint8_t mac2[])
{
    int mac_len = 6;
    int i = 0;
    while(i < mac_len)
    {
        if (mac1[i] != mac2[i])
            return false;
        i++;
    }
    return true;
}

void ipv4_net_to_str(const struct in_addr *addr, char ip_str[]) {
    if (inet_ntop(AF_INET, addr, ip_str, INET_ADDRSTRLEN) == NULL) {
        clean_exit_failure("Failed to convert net ipv4 to str\n");
    }
}

bool is_broadcast_mac(uint8_t mac[])
{
    return mac[0] == 0 && mac[1] == 0 && mac[2] == 0 && mac[3] == 0 && mac[4] == 0 && mac[5] == 0;
}

bool is_arp_request(struct s_arp_packet *packet)
{
    return ntohs(packet->operation) == ARP_OPERATION_REQUERST;
}

void binary_mac_to_str(const uint8_t *mac, char *str) {
    char *hex_chars = "0123456789ABCDEF";
    int octet_index = 0;

    while (octet_index < MAC_ADDR_BYTE_LEN) {
        
        int hex_digit_index = mac[octet_index] >> 4;
        str[octet_index * 3] = hex_chars[hex_digit_index];
        
        hex_digit_index = mac[octet_index] & 0x0F;
        str[octet_index * 3 + 1] = hex_chars[hex_digit_index];
        
        if (octet_index < MAC_ADDR_BYTE_LEN - 1) {
            str[octet_index * 3 + 2] = ':';
        }
        octet_index++;
    }
    str[MAC_CHAR_LEN] = '\0';
}

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

void str_mac_to_binary(char *mac_str, uint8_t mac_net[])
{
    if (ft_strlen(mac_str) != MAC_CHAR_LEN)
        clean_exit_failure("Mac address len is not 17");
    
    int i = 0;

    while (i < MAC_ADDR_BYTE_LEN)
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

void print_local_mac_and_ip()
{
    printf("Local mac: %s\n", g_arguments.local_mac_str);
    printf("Local ip: %s\n", g_arguments.local_ip_str);
}