#include "client.h"

Client::Client(string server_addr, int serv_port_number) {
  this->sockfd = socket(AF_INET, SOCK_STREAM, 0);

  if (this->sockfd <= 0) {
    perror("Error initializing socket: ");
  }

  bzero(&this->server_address, sizeof(this->server_address));
  this->server_address.sin_family = AF_INET;
  this->server_address.sin_port = htons(serv_port_number);
  inet_pton(AF_INET, server_addr.c_str(), &server_address.sin_addr);

  if (connect(this->sockfd, (struct sockaddr *)&this->server_address,
              sizeof(this->server_address)) < 0) {
    perror("Connection to server failed");
    exit(-1);
  }
}

void Client::send_msg(string message) {
  const char *mess_buffer = message.c_str();
  SSL_write(this->cSSL, mess_buffer, strlen(mess_buffer));
}
