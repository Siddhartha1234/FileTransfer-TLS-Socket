#include "ssl.h"

SSLSocket::SSLSocket() {
  // Inititalize SSL Libraries
  SSL_library_init();
  SSL_load_error_strings();
  OpenSSL_add_all_algorithms();
}

void SSLSocket::set_certificates(string cert_path, string prv_key_path,
                                 string chain_path) {
  // Set certificates used by SSL
  int use_cert = SSL_CTX_use_certificate_file(this->sslctx, cert_path.c_str(),
                                              SSL_FILETYPE_PEM);
  if (use_cert <= 0) {
    ERR_print_errors_fp(stderr);
    exit(-1);
  }

  int use_prv = SSL_CTX_use_PrivateKey_file(this->sslctx, prv_key_path.c_str(),
                                            SSL_FILETYPE_PEM);

  if (use_prv <= 0) {
    ERR_print_errors_fp(stderr);
    exit(-1);
  }

  int use_chain =
      SSL_CTX_use_certificate_chain_file(this->sslctx, chain_path.c_str());
  if (use_chain <= 0) {
    ERR_print_errors_fp(stderr);
    exit(-1);
  }

  if (SSL_CTX_check_private_key(this->sslctx) < 0) {
    ERR_print_errors_fp(stderr);
    exit(-1);
  }
}

SSLSocket::~SSLSocket() {
  ERR_free_strings();
  EVP_cleanup();
  SSL_shutdown(this->cSSL);
  SSL_free(this->cSSL);
}
