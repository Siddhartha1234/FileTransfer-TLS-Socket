#include "server.h"

Server::Server(int port_number, int max_num_clients, int max_buffer_size) {
  this->port_number = port_number;
  this->max_num_clients = max_num_clients;
  this->max_buffer_size = max_buffer_size;

  this->sockfd = socket(AF_INET, SOCK_STREAM, 0);

  if (this->sockfd <= 0) {
    perror("Error initializing socket: ");
    exit(-1);
  }

  bzero(&this->server_address, sizeof(this->server_address));
  this->server_address.sin_family = AF_INET;
  this->server_address.sin_addr.s_addr = INADDR_ANY;
  this->server_address.sin_port = htons(this->port_number);
}

void Server::accept_clients() {
  listen(this->sockfd, this->max_num_clients);
  struct sockaddr_in client_address;
  int len = sizeof(struct sockaddr_in);

  int newsockfd = accept(this->sockfd, (struct sockaddr *)&client_address,
                         (socklen_t *)&len);

  this->cSSL = SSL_new(this->sslctx);
  SSL_set_fd(this->cSSL, newsockfd);
  int ssl_err = SSL_accept(this->cSSL);

  if (ssl_err <= 0) {
    perror("Error connecting to client over SSL : ");
    exit(-1);
  }

  while (true) {
    char recv_msg[this->max_buffer_size];
    int x = SSL_read(this->cSSL, recv_msg, this->max_buffer_size);
    recv_msg[x] = '\0';
    printf("%s\n", recv_msg);
  }
}

void Server::send_msg(string message) {
  const char *mess_buffer = message.c_str();
  SSL_write(this->cSSL, mess_buffer, strlen(mess_buffer));
}
