#include <sys/socket.h> //socket
#include <linux/if_ether.h> //ETH_P_ARP
#include <errno.h> //errno
#include <stdlib.h> //exit
#include <stdio.h> //printf
#include <arpa/inet.h> //htons
#include <string.h> //strerror

#include "resources.h"
#include "libft.h"
#include "net.h"
#include <stdio.h>

//Узнавать какие интерфейсы доступны и печатать их
//Печатать какие интерфейсы доступны
int main(int ac, char **av)
{
    (void)ac;
    (void)av;
    char arr[3];
    char *arr1 = arr;
    
    g_resources.socket = socket(AF_PACKET, SOCK_DGRAM, htons(ETH_P_ARP));
    if (g_resources.socket == -1)
    {
        printf("%s : error when opening the socket %s\n", av[0], strerror(errno));
        free_resources();
        exit(EXIT_FAILURE);
    }
    
    struct s_arp_packet arp_packet;
    
    while (1)
    {
        ft_bzero(&arp_packet, sizeof(struct s_arp_packet));
        printf("Start recvfrom\n");
        int result = recvfrom(g_resources.socket, &arp_packet, sizeof(struct s_arp_packet), 0, NULL, NULL);
        printf("Received %d bytes\n", result);
        printf("Packet data. Hardware addres len: %d protocol addr len %d\n", arp_packet.hardware_addr_len, arp_packet.protocol_addr_len);
    }
    free_resources();
    return EXIT_SUCCESS;
}