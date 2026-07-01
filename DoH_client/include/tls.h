/*                  TLS                     */

#ifndef TLS_H
# define TLS_H

# include <openssl/ssl.h>
# include <openssl/err.h>

# define TIMEOUT_CONNECTION 120 // 120seconds to respect, closing connection due to idle connection security
# define TLS_PORT           853
# define TLS_SERVER         "1.1.1.1" // Used in the DoT_client

typedef struct tls_session{
    SSL*        ssl;
    SSL_CTX*    ctx;
}   s_tls_session;

void        build_tls_session(int socketfd, s_tls_session* sess);
void        send_tls_query(s_tls_session* sess, uint8_t* query, int len);
void        clean_program(s_tls_session* sess, int sockfd);
int         receive_tls_response(s_tls_session* sess, uint8_t* response);

#endif //TLS_H
