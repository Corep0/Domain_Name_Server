#ifndef DNS_H
# define DNS_H

// Socket -> socket, sento, close, recvfrom
# include <sys/types.h>
# include <sys/socket.h>

// Reseau -> htons, htonl, nthos, inet_ntoa, memset, memcpy
# include <arpa/inet.h>
# include <netinet/in.h>
# include <string.h>

// Display / Error -> printf, perror, strerror
# include <stdio.h>
# include <stdlib.h>
# include <stdbool.h>
# include <unistd.h>
# include <errno.h>

# define ID                 22

# define MAX_DOMAIN_LENGTH  256
# define BUFFER_SIZE        512
# define DNS_PORT           53
# define DNS_SERVER        "8.8.8.8" // Used in the DNS_Resolver

// Every different layer of the header (in order)
struct __attribute__((packed)) dns_header{
    uint16_t    id;        // Identifiant of the request
    uint16_t    flags;     // QR, OPCODE, [AA, TC, RD, RA], Z, RCODE
    uint16_t    qdcount;   // Count of request (asking)
    uint16_t    ancount;   // Count of response (answer)
    uint16_t    nscount;       //May not be in used yet
    uint16_t    arcount;       //May not be in used yet
    //Complete from RFC 1035 $4.1.1
};

struct  dns_query{
    int     id;
    char*   domain;
    int     query_count;
    int     account_count;
    int     dns_port;
    char*   dns_server_ip;
};

// Build Query
int     build_query(char *domain, uint8_t* buff);
int     encoding(char* s, uint8_t* res);


// Socket, send, receive
int     build_socket_tcp(void);

// Parse + Print
void    parse_response(uint8_t* response);

// Tools
size_t  count_char(char *s, char c);
size_t	ft_strlcat(char *dest, const char *src, size_t size);
char*   ft_itoa(int n);
char**  ft_split(char *s, char c);
// Print uint8_t* in chars
void    print_hexdump(const void* data, int size);
void    print_response(uint8_t* response, int rlen);

#endif //DNS_H