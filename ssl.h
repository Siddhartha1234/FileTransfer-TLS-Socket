#pragma once
#include <openssl/bio.h>
#include <openssl/err.h>
#include <openssl/ssl.h>

#include <string>
using namespace std;

//SSL Socket class
class SSLSocket {
protected:
  SSL_CTX *sslctx;
  SSL *cSSL;

public:
  SSLSocket();
  void set_certificates(string cert_path, string prv_key_path,
                        string chain_path);
  ~SSLSocket();
};
