#include "application.h"

// Initialize Application
Application::Application(string mode) {
  this->mode = mode;
  this->pbar = new ProgressBar(50);
}

//configure server 
void Application::config_server(int port_number, int max_num_clients,
                                int max_buffer_size, string file_location) {
  this->server =
      new Server(port_number, max_num_clients, max_buffer_size, file_location);
}

//configure client
void Application::config_client(string server_addr, int serv_port_number,
                                int max_buffer_size, string file_path) {
  this->client = new Client(server_addr, serv_port_number, max_buffer_size);
  this->file_path = file_path;
}

//Specify the certificates being used
void Application::config_certificates(string cert_path, string prv_path,
                                      string chain_path) {
  if (this->mode == "receive") {
    this->server->set_certificates(cert_path, prv_path, chain_path);
  } else {
    this->client->set_certificates(cert_path, prv_path, chain_path);
  }
}

//Start server on receive mode and send file for client
void Application::run() {
  if (this->mode == "receive") {
    this->server->accept_file();
  } else {
    this->client->connect_server();

    string file_name = get_file_name(this->file_path);
    this->client->send_file_chunk((char *)file_name.c_str(),
                                  file_name.length());

    cout << "Sending file : " << file_name << "\n";

    auto start_time = chrono::high_resolution_clock::now();
    
    //Send SHA-256 hash
    char sha_buffer[65];
    sha256_file((char *)this->file_path.c_str(), sha_buffer);
    
    this->client->send_file_chunk(sha_buffer, 65);
  
    //Get file size
    struct stat st;
    stat(file_path.c_str(), &st);
    int file_size = st.st_size;
    
    //Send file size
    char *fs = (char *)to_string(file_size).c_str();
    this->client->send_file_chunk(fs, strlen(fs));
    
    //Receive file
    FILE *fp = fopen(file_path.c_str(), "rb");
    int bytes;
    char send_buffer[this->client->max_buffer_size];
    int bytes_sent = 0;
    while ((bytes = fread(send_buffer, 1, this->client->max_buffer_size, fp)) >
           0) {
      this->client->send_file_chunk(send_buffer, bytes);
      bytes_sent += bytes;

      int pct = (bytes_sent * 100) / file_size;
      pbar->update_progress(pct);
    }

    auto end_time = chrono::high_resolution_clock::now();
    chrono::duration<double, milli> elapsed = end_time - start_time;
    cout << "File sent in : " << elapsed.count() / 1000.0 << " seconds\n";
  }
}
