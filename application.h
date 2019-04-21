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

#include "client.h"
#include "server.h"

using namespace std;

class Application {
  Server *server;
  Client *client;
  string mode;

public:
  Application(string mode = "client");
  void config_server(int port_number, int max_num_clients, int max_buffer_size);
  void config_client(string server_addr, int serv_port_number);
  void config_certificates(string cert_path, string prv_path,
                           string chain_path);
  void run();
};
