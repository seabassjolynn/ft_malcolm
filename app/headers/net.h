#ifndef NET_H
#define NET_H

#include <stdint.h>
#include <arpa/inet.h> //for inet_pton

struct s_arp_packet
{
    uint16_t hardware_type;
    uint16_t protocol_type;
    uint8_t hardware_addr_len;
    uint8_t protocol_addr_len;
    uint16_t operation;
    unsigned char sender_hardware_addr[6];
    uint32_t sender_protocol_addr;
    unsigned char target_hardware_addr[6];
    uint32_t target_protocol_addr;
};

#endif