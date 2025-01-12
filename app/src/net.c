#include "net.h"

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
    
    while (ifaddrs_tmp->ifa_next)
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