#include "server.h"

Server::Server(int port_number, int max_num_clients, int max_buffer_size,
               string file_location) {
  this->port_number = port_number;
  this->max_num_clients = max_num_clients;
  this->max_buffer_size = max_buffer_size;
  this->file_location = file_location;
  if (this->file_location[this->file_location.length() - 1] != '/')
    this->file_location += '/';

  this->pbar = new ProgressBar(50);
  this->sslctx = SSL_CTX_new(TLS_server_method());
  this->sockfd = socket(AF_INET, SOCK_STREAM, 0);

  if (this->sockfd <= 0) {
    perror("Error initializing socket: ");
    exit(-1);
  }

  bzero(&this->server_address, sizeof(this->server_address));
  this->server_address.sin_family = AF_INET;
  this->server_address.sin_addr.s_addr = INADDR_ANY;
  this->server_address.sin_port = htons(this->port_number);

  if (bind(this->sockfd, (struct sockaddr *)&this->server_address,
           sizeof(this->server_address)) < 0) {
    perror("Error on binding to socket: ");
    exit(-1);
  }

  if (listen(this->sockfd, this->max_num_clients) < 0) {
    perror("Error listening on socket: ");
    exit(-1);
  }
}

void Server::accept_file() {
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

  auto start_time = chrono::high_resolution_clock::now();

  // Receive file name
  char file_name[this->max_buffer_size];

  int flen = SSL_read(this->cSSL, file_name, this->max_buffer_size);
  file_name[flen] = '\0';

  printf("Receiving file : %s\n", file_name);

  // Make new file buffer at location
  char *path = (char *)(this->file_location + string(file_name)).c_str();
  FILE *fp = fopen(path, "wb");

  // Receive md5 hash
  char sha_buffer[65];
  int sha_size = SSL_read(this->cSSL, sha_buffer, this->max_buffer_size);


  // Receive file size
  char fs[this->max_buffer_size];
  int x = SSL_read(this->cSSL, fs, this->max_buffer_size);
  fs[x] = '\0';
  int file_size = atoi(fs);

  int bytes_recvd = 0;
  while (true) {
    char recv_msg[this->max_buffer_size];
    int x = SSL_read(this->cSSL, recv_msg, this->max_buffer_size);
    if (x == 0)
      break;
    
    bytes_recvd += x;
    this->pbar->update_progress((bytes_recvd * 100) / file_size);
    if (x == 0)
      break;
    // write chunk to file buffer

    fwrite(recv_msg, 1, x, fp);
  }

  auto end_time = chrono::high_resolution_clock::now();
  // close the file
  fclose(fp);

  char check_sha[65];
  sha256_file(path, check_sha);
  printf("Sent SHA-256 hash:%s\n", sha_buffer);
  printf("Received SHA-256 hash:%s\n", check_sha);
  if (strcmp(check_sha, sha_buffer) == 0) {
    printf("SHA 256 hash verified !\n");
  } else {
    printf("SHA 256 hash doesn't match, file sent is corrupt!\n");
  }
  chrono::duration<double, milli> elapsed = end_time - start_time;
  cout << "File received in : " << elapsed.count() / 1000.0 << " seconds\n";
}

void Server::send_msg(string message) {
  const char *mess_buffer = message.c_str();
  SSL_write(this->cSSL, mess_buffer, strlen(mess_buffer));
}
