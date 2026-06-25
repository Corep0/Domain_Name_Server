#include "dns.h"
#include "tls.h"
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
    return pos;
}

void    send_tls_query(s_tls_session* sess, uint8_t* query, int len){
    uint16_t buf = htons(len);
    
    //Send the length of the query first
    ssize_t len_lengthQ = SSL_write(sess->ssl, &buf, 2);
    if (len_lengthQ <= 0){
        printf("Failed to sent the length of the query\n");
        return;
    }
    ssize_t buf_sent = SSL_write(sess->ssl, (const void*)query, len);
    if (buf_sent <= 0){
        printf("Failed to sent the query: %d\n", SSL_get_error(sess->ssl, buf_sent));
        return;
    }
}

int     receive_tls_response(s_tls_session* sess, uint8_t* response){
    uint8_t     buf[BUFFER_SIZE];
    uint16_t    len_read;
    ssize_t buf_receiv = SSL_read(sess->ssl, &len_read, 2);
    if (buf_receiv <= 0){
        printf("Didn't received the length of the message\n");
        printf("SSL_read() failed due to : \n %s \n", strerror(errno));
        return -1;
    }
    uint16_t    resp_len = ntohs(len_read);
    buf_receiv = SSL_read(sess->ssl, buf, resp_len);
    if (buf_receiv <= 0){
        printf("SSL_read() failed due to : \n %s \n", strerror(errno));
        return -1;
    }
    memcpy(response, &buf, resp_len);
    return resp_len;
}


