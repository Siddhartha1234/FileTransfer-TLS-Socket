#include "application.h"
#include <string>

using namespace std;

// Config of where to
int serv_port_number = 65080;
int max_buffer_size = 1000;
int max_conns = 5;

int main(int argc, char *argv[]) {
  if (string(argv[1]) == "send") {
    string serv_address = string(argv[1]);
    string file_path = string(argv[2]);

    Application app("send");
    app.config_client(serv_address, serv_port_number, max_buffer_size,
                      file_path);
    app.config_certificates("/etc/ssl/certs/inter-ee.pem",
                            "/etc/ssl/private/inter-ee.key",
                            "/etc/ssl/certs/chain-ee.pem");
    app.run();
  } else {
    string file_location = string(argv[1]);

    Application app("receive");
    app.config_server(serv_port_number, max_conns, max_buffer_size,
                      file_location);
    app.config_certificates("/etc/ssl/certs/cse-inter.pem",
                            "/etc/ssl/private/cse-inter.key",
                            "/etc/ssl/certs/cse-chain.pem");
    app.run();
  }
}
