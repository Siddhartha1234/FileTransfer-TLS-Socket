#include "application.h"

Application::Application(string mode) { this->mode = mode; }
void Application::config_server(int port_number, int max_num_clients,
                                int max_buffer_size) {
  this->server = new Server(port_number, max_num_clients, max_buffer_size);
}

void Application::config_client(string server_addr, int serv_port_number) {
  this->client = new Client(server_addr, serv_port_number);
}

void Application::config_certificates(string cert_path, string prv_path,
                                      string chain_path) {
  if (this->mode == "server") {
    this->server->set_certificates(cert_path, prv_path, chain_path);
  } else {
    this->client->set_certificates(cert_path, prv_path, chain_path);
  }
}

void Application::run() {
  if (this->mode == "server") {
    this->server->accept_clients();
  } else {
    this->client->connect_server();
    int cnt = 0;
    while (cnt < 5) {
      this->client->send_msg("Hello!");
      cnt++;
    }
  }
}
