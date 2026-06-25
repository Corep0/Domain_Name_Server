# include "dns.h"
# include "tls.h"
/* 
    DoT Client:
    DNS over TLS connection (TCP)
    Take a domain name and return is IPV4 adress 
*/
/*
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
*/

void    clean_program(s_tls_session* sess, int sockfd){
    if (sess->ssl){
        SSL_shutdown(sess->ssl);
        SSL_free(sess->ssl);
    }
    if (sess->ctx)
        SSL_CTX_free(sess->ctx);
    close(sockfd);

}

int main(int ac, char **av){
    s_tls_session* sess;
    sess = malloc(sizeof(*sess));
    if (!sess){
        printf("Malloc on session tls failed");
        return 1;
    }
    if (ac != 2){
        printf("Usage: ./dns_resolver [domain name]");
        return 1;
    }
    uint8_t     buf[BUFFER_SIZE];
    int         len = build_query(av[1], buf);
    // Adding Hand shake, protocol TLS
    //          TCP connection instead of UDP
    int         socketfd = build_socket_tcp();
    if (socketfd == -1)
        return 1;

    build_tls_session(socketfd,  sess);

    if (!sess->ctx || !sess->ssl)
        return (clean_program(sess, socketfd), 1);
    send_tls_query(sess, buf, len);

    // send_query(socketfd, buf, len, ssl_object);
    if (errno != 0)
        return (clean_program(sess, socketfd), 1);
    uint8_t     response[BUFFER_SIZE];
    int         r_tls_len = receive_tls_response(sess, response);

    if (r_tls_len <= 0){
        clean_program(sess, socketfd);
        return 1;
    }
    // int         rlen = receive_response(socketfd, response);
    // if (rlen <= 0)
    //     return 1;
    parse_response(response);

    clean_program(sess, socketfd);
    free(sess);
    return 0;
}
