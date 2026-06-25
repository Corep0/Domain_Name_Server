#include "dns.h"

// Building section
struct dns_header   dns_header_struct(int id, int flag, bool request, uint16_t counts){
    struct dns_header   header;

    memset(&header, 0, sizeof(header));
    header.id = htons(id);
    header.flags = htons(flag);
    if (request)
        header.qdcount = htons(counts);
    else
        header.ancount = htons(counts);
    return header;
}

// uint16_t    hex(dns_header* header){
//     uint16_t* head = malloc(sizeof(uint16_t) * (2 * (sizeof(header.id) + sizeof(header.flags) + sizeof(header.qdcount) + sizeof(header.ancount)) + 1));
//     if (!head)
//         return "00";

// }

int     build_query(char *domain, uint8_t* buff){
    int         pos = 0;
    struct      dns_header  header;
    uint16_t    qtype;
    uint16_t    qclass; // 16bits -> 2 bytes (of 8 bits)
    size_t      bits_16 = 2;

// Build and Copy header
    header = dns_header_struct(ID, 0x0100, true, 1);
    //uint16_t head = hex(header);
    memcpy(buff + pos ,&header, 12);
    pos += 12;
// Encode the domain name
    pos += encoding(domain, buff + pos);
// write QTYPE
    qtype = htons(1);
// printf("QTYPE : \\x%04x ", qtype);

    memcpy(buff + pos, &qtype, bits_16);
    pos += 2;
// write QCLASS
    qclass = htons(1);
    memcpy(buff + pos, &qclass, bits_16);
    pos += 2;
// printf("Pos: %d in Build_Query\n", pos);
// print_hexdump(buff, pos);
    return pos;
}

void    send_query(int socketfd, uint8_t* query, int len){
    struct sockaddr_in     server;
    memset(&server, 0, sizeof(server));
    server.sin_family = AF_INET;
    server.sin_port = htons(DNS_PORT);
    server.sin_addr.s_addr = inet_addr(DNS_SERVER);
    // printf("google dns: %s : %d\n", DNS_SERVER, DNS_PORT);

    ssize_t buf_sent = sendto(socketfd, (const void*)query, len, 0, (struct sockaddr *)&server, sizeof(server));
    if (buf_sent < 0 || errno != 0){
        printf("sendto() failed due to: \n");
        printf("%s\n", strerror(errno));
        return;
    }
    // printf("Query content have been send successfully.\n");
}

int     receive_response(int socketfd, uint8_t* response){
    uint8_t    buf[BUFFER_SIZE];
    struct sockaddr_in     server;
    
    memset(&server, 0, sizeof(server));
    server.sin_family = AF_INET;
    server.sin_port = htons(DNS_PORT);
    server.sin_addr.s_addr = inet_addr(DNS_SERVER);
    socklen_t   len_server = sizeof(server);
    ssize_t buf_receiv = recvfrom(socketfd, buf, sizeof(buf), 0, (struct sockaddr *)&server, &len_server);
    if (buf_receiv <= 0){
        printf("recvfrom() failed due to: \n %s\n", strerror(errno));
        return -1;
    }
    // print_hexdump(buf, buf_receiv);
    memcpy(response, &buf, buf_receiv);
    return buf_receiv;
}

