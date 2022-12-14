#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <openssl/ssl.h>

int main (int argc, char *argv[])
{
    int s, cs, len, result;
    struct sockaddr_in srv_addr, cli_addr;
    char buf[64];
    SSL_CTX *ctx;
    SSL *ssl;

    /* Create a TLS server context with certificates */
    ctx = SSL_CTX_new(TLS_server_method());
    SSL_CTX_use_certificate_file(ctx, "certificate.pem", SSL_FILETYPE_PEM);
    SSL_CTX_use_PrivateKey_file(ctx, "key.pem", SSL_FILETYPE_PEM);

    /* Set the address and port to listen on */
    srv_addr.sin_family = AF_INET;
    srv_addr.sin_port = 9001;
    inet_pton(AF_INET, "127.0.0.1", &srv_addr.sin_addr);

    /* Create a socket and listen */
    s = socket(AF_INET, SOCK_STREAM, 0);
    bind(s, (struct sockaddr *)&srv_addr, sizeof(srv_addr));
    listen(s, 1);

    /* Wait for a new client connection */
    printf("Waiting for a client…\n");
    len = sizeof(cli_addr);
    cs = accept(s, (struct sockaddr*)&cli_addr, &len);

    /* Create an SSL session for the new socket */
    ssl = SSL_new(ctx);
    SSL_set_fd(ssl, cs);

    /* Run the OpenSSL handshake */

    result = SSL_accept(ssl);

    if (result == 1) 
    {
        /* Exchange some data if the connection succeeded */
        sprintf(buf, "Hello from the server");
        SSL_write(ssl, buf, strlen(buf) + 1);
        len = SSL_read(ssl, buf, sizeof(buf));
        printf("Received message from client:%s\n", buf);
    }

    /* Done */
    close(cs);
    close(s);
    SSL_free(ssl);
    SSL_CTX_free(ctx);

    return 0;
}
