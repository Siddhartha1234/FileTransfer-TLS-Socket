#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#include <openssl/bio.h>
#include <openssl/err.h>
#include <openssl/ssl.h>

#include "ssl.h"

using namespace std;

class Client : public SSLSocket {
private:
  int sockfd;
  struct sockaddr_in server_address;

public:
  int max_buffer_size;
  Client(string server_addr, int serv_port_number, int max_buffer_size);
  void connect_server();
  void send_file_chunk(char *message, int bytes);
};
