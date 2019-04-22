#include "client.h"

Client::Client(string server_addr, int serv_port_number, int max_buffer_size) {
  this->sockfd = socket(AF_INET, SOCK_STREAM, 0);
  this->max_buffer_size = max_buffer_size;

  if (this->sockfd <= 0) {
    perror("Error initializing socket: ");
  }

  bzero(&this->server_address, sizeof(this->server_address));
  this->server_address.sin_family = AF_INET;
  this->server_address.sin_port = htons(serv_port_number);
  inet_pton(AF_INET, server_addr.c_str(), &server_address.sin_addr);

  this->sslctx = SSL_CTX_new(TLS_client_method());
}

void Client::connect_server() {
  if (connect(this->sockfd, (struct sockaddr *)&this->server_address,
              sizeof(this->server_address)) < 0) {
    perror("Connection to server failed");
    exit(-1);
  }
  this->cSSL = SSL_new(this->sslctx);
  SSL_set_fd(this->cSSL, this->sockfd);

  if (SSL_connect(this->cSSL) < 0) {
    ERR_print_errors_fp(stderr);
    exit(-1);
  }
}

void Client::send_file_chunk(char *message, int bytes) {
  SSL_write(this->cSSL, message, bytes);
}
