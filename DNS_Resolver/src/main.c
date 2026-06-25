#include "dns.h"

/* 
    DNS Resolver:
    Take a domain name and return is IPV4 adress 
*/

int main(int ac, char **av){
// Validate Input
    if (ac != 2){
        printf("Usage: ./dns_resolver [domain name]");
        return 1;
    }
// Create question query
    uint8_t     buf[BUFFER_SIZE];
    int         len = build_query(av[1], buf);

// Socket, send, receive
    int         socketfd = build_socket();
    if (socketfd == -1)
        return 1;
    send_query(socketfd, buf, len);
    if (errno != 0)
        return 1;
    uint8_t     response[BUFFER_SIZE];
    int         rlen = receive_response(socketfd, response);
    if (rlen <= 0)
        return 1;
// print_response(response, rlen);
 
// Parse + Print
    parse_response(response);
// Clean
    close(socketfd);
    return 0;
}
