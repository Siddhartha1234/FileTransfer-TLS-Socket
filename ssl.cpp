#include "ssl.h"

SSLSocket::SSLSocket() {
  // Inititalize SSL Libraries
  SSL_load_error_strings();
  SSL_library_init();
  OpenSSL_add_all_algorithms();

  this->sslctx = SSL_CTX_new(TLS_method());
  SSL_CTX_set_options(this->sslctx, SSL_OP_SINGLE_DH_USE);
}

void SSLSocket::set_certificates(string cert_path, string prv_key_path,
                                 string chain_path) {
  // Set certificates used by SSL
  int use_cert = SSL_CTX_use_certificate_file(this->sslctx, cert_path.c_str(),
                                              SSL_FILETYPE_PEM);
  int use_prv = SSL_CTX_use_PrivateKey_file(this->sslctx, prv_key_path.c_str(),
                                            SSL_FILETYPE_PEM);
  int use_chain =
      SSL_CTX_use_certificate_chain_file(this->sslctx, chain_path.c_str());
}

SSLSocket::~SSLSocket() {
  ERR_free_strings();
  EVP_cleanup();
}
