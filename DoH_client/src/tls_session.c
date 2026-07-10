# include "tls.h"
# include "dns.h"
# include "https.h"

/*
TLS handshake + cert verif
Fails HS -> error
Done HS -> send_query(),...
TCP SYN      → your PC to 1.1.1.1    ← TCP handshake
TCP SYN-ACK  ← 1.1.1.1 to your PC
TCP ACK      → your PC to 1.1.1.1
TLS Client Hello → your PC           ← TLS starts
TLS Server Hello ← 1.1.1.1           ← TLS responds
*/
/*
        TLS handshake + SNI
*/
void    build_tls_session(int socketfd, s_tls_session* sess){
    SSL_CTX     *ctx_tmp;
    SSL         *ssl_tmp;
    struct sockaddr_in server;
    const unsigned char *alpn = (const unsigned char*)"\x02h2";

//TCP Socket
    memset(&server, 0, sizeof(server));
    server.sin_family = AF_INET;
    server.sin_port = htons(TLS_PORT);
    server.sin_addr.s_addr = inet_addr(TLS_SERVER);
    if (connect(socketfd, (struct sockaddr*)&server, sizeof(server)) != 0){
        printf("connect() failed due to: \n %s\n", strerror(errno));
        return;
    }
// TLS session
    SSL_library_init();
    ctx_tmp = SSL_CTX_new(TLS_client_method());
    if (!ctx_tmp){
        printf("SSL_CTX_new failed");
        return ;
    }
    SSL_CTX_set_verify(ctx_tmp, SSL_VERIFY_PEER, NULL);
    if (!SSL_CTX_load_verify_locations(ctx_tmp, "/etc/ssl/certs/ca-certificates.crt", NULL)){
        printf("Cannot found ceritificate");
        return;
    }
    if (SSL_CTX_set_alpn_protos(ctx_tmp, alpn, 3) != 0){
        printf("Failed to apply \"h2\" protocol on ctx");
        return;
    }
    ssl_tmp = SSL_new(ctx_tmp);
    if (!ssl_tmp){
        return ;
    }
// Wrap TCP socket into TLS session
    SSL_set_fd(ssl_tmp, socketfd);
    if (SSL_set_tlsext_host_name(ssl_tmp, "cloudflare-dns.com") == 0){
        printf("Servername got refused by the server");
        return;
    }
    if (SSL_connect(ssl_tmp) != 1){
        printf("HandShake failed - Connection isn't secure");
        int err = SSL_get_error(ssl_tmp, -1);
        printf("SSL get error returned: %d \n", err);
        ERR_print_errors_fp(stderr);
        return ;
    }
    const unsigned char *negotiated = NULL;
    unsigned int len = 0;
    SSL_get0_alpn_selected(ssl_tmp, &negotiated, &len);
   	if (len != 2 || memcmp(negotiated, "h2", 2) != 0 || !negotiated){
        printf("The Protocol h2 has been refused. | In need to add a backup protocol");
        return;
    }
// Verify validity of the certificates /certs/ca-bundle.rt
    if (SSL_get_verify_result(ssl_tmp) != X509_V_OK){
        printf("Furnished Certificates by peer is invalid");
        return ;
    }

    sess->ssl = ssl_tmp;
    sess->ctx = ctx_tmp;
}

