#include "application.h"
#include <string>

using namespace std;

int main(int argc, char *argv[]) {
  if (string(argv[1]) == "client") {
    string serv_address = string(argv[2]);
    int serv_port_number = stoi(string(argv[3]));

    Application app("client");
    app.config_client(serv_address, serv_port_number);
    app.config_certificates("/etc/ssl/certs/inter-ee.pem",
                            "/etc/ssl/private/inter-ee.key",
                            "/etc/ssl/certs/chain-ee.pem");
    app.run();
  } else {
    int port_number = stoi(string(argv[2]));
    int max_conns = stoi(string(argv[3]));

    Application app("server");
    app.config_server(port_number, max_conns, 1000);
    app.config_certificates("/etc/ssl/certs/cse-inter.pem",
                            "/etc/ssl/private/cse-inter.key",
                            "/etc/ssl/certs/cse-chain.pem");
    app.run();
  }
}
