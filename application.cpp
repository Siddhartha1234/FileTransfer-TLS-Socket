#include "application.h"

Application::Application(string mode) { this->mode = mode; }
void Application::config_server(int port_number, int max_num_clients,
                                int max_buffer_size, string file_location) {
  this->server =
      new Server(port_number, max_num_clients, max_buffer_size, file_location);
}

void Application::config_client(string server_addr, int serv_port_number,
                                int max_buffer_size, string file_path) {
  this->client = new Client(server_addr, serv_port_number, max_buffer_size);
  this->file_path = file_path;
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
    this->server->accept_file();
  } else {
    this->client->connect_server();

    string file_name = get_file_name(this->file_path);
    this->client->send_file_chunk((char *)file_name.c_str(),
                                  file_name.length());

    char sha_buffer[65];
    sha256_file((char *)this->file_path.c_str(), sha_buffer);

    this->client->send_file_chunk(sha_buffer, 65);

    FILE *fp = fopen(file_path.c_str(), "rb");
    int bytes;
    char send_buffer[this->client->max_buffer_size];

    while ((bytes = fread(send_buffer, 1, this->client->max_buffer_size, fp)) >
           0) {
      this->client->send_file_chunk(send_buffer, bytes);
    }
  }
}
