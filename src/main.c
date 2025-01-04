#include <sys/socket.h> //socket
#include <linux/if_ether.h> //ETH_P_ARP
#include <errno.h> //errno
#include <stdlib.h> //exit
#include <stdio.h> //printf
#include <arpa/inet.h> //htons
#include <string.h> //strerror

#include "resources.h"
#include "libft.h"

int main(int ac, char **av)
{
    (void)ac;
    (void)av;
    
    
    g_resources.socket = socket(AF_PACKET, SOCK_RAW, htons(ETH_P_ARP));
    if (g_resources.socket == -1)
    {
        printf("%s : error when opening the socket %s\n", av[0], strerror(errno));
        free_resources();
        exit(EXIT_FAILURE);
    }


    free_resources();
    return EXIT_SUCCESS;
}