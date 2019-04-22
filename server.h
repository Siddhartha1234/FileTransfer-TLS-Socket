#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#include <openssl/bio.h>
#include <openssl/err.h>
#include <openssl/ssl.h>

#include "ssl.h"

using namespace std;

class Server : public SSLSocket {
private:
  int sockfd;
  struct sockaddr_in server_address;

  int port_number;
  int max_num_clients;
  int max_buffer_size;
  string file_location;

public:
  Server(int port_number, int max_num_clients, int max_buffer_size,
         string file_location);

  void accept_file();
  void send_msg(string message);
};
