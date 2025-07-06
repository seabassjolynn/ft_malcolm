#ifndef NET_H
#define NET_H

#include <stdint.h>
#include <arpa/inet.h> //for inet_pton

#include <sys/types.h> //getifaddrs, freeifaddrs
#include <ifaddrs.h> //getifaddrs, freeifaddrs
#include "resources.h"
#include <stdio.h>
#include "libft.h"
#include <sys/socket.h>
#include <linux/if_packet.h>
#include <stdbool.h>
#include <net/if.h> //for if_nametoindex, for constants that represent data link layer protocols
#include <linux/if_arp.h> //for ARPHRD_ETHER
#include "libft.h"
#include "debug.h"
#include "arguments.h"
#define IPV4_PROTO 0x0800
#define MAC_ADDR_BYTE_LEN 6
#define IPV4_ADDR_BYTE_LEN 4
#define ARP_OPERATION_REQUERST 1
#define ARP_OPERATION_REPLY 2

//this is a compiler directive that is used to avoid padding of fields in struct.
//e.g. if I don't add the directive here, the compiler moves sender_protocol_addr to bytes forward
#pragma pack(push, 1)
struct s_arp_packet
{
    uint16_t hardware_type;
    uint16_t protocol_type;
    uint8_t hardware_addr_len;
    uint8_t protocol_addr_len;
    uint16_t operation;
    uint8_t sender_hardware_addr[6];
    uint32_t sender_protocol_addr;
    uint8_t target_hardware_addr[6];
    uint32_t target_protocol_addr;
};
#pragma pack(pop) 

void print_arp_packet(struct s_arp_packet *packet);

void print_mac(uint8_t *arr);

bool macs_equal(uint8_t mac1[], uint8_t mac2[]);

void ipv4_net_to_str(struct in_addr *addr, char ip_str[]);

bool is_broadcast_mac(uint8_t mac[]);

bool is_arp_request(struct s_arp_packet *packet);

void get_local_mac_and_ip(void);

void binary_mac_to_str(const uint8_t *mac, char *str);

void str_mac_to_binary(char *mac_str, uint8_t mac_net[]);

void print_local_mac_and_ip();

#endif