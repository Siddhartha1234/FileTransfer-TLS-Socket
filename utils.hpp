#include <iomanip>
#include <iostream>
#include <openssl/sha.h>
#include <sstream>
#include <string>

using namespace std;

inline string get_file_name(string file_path) {
  int ind = file_path.length() - 1;
  string file_name = "";
  while (ind >= 0 && file_path[ind] != '/' && file_path[ind] != '\\') {
    file_name = file_path[ind] + file_name;
    ind--;
  }
  return file_name;
}

inline string sha256(const string str) {
  unsigned char hash[SHA256_DIGEST_LENGTH];
  SHA256_CTX sha256;
  SHA256_Init(&sha256);
  SHA256_Update(&sha256, str.c_str(), str.size());
  SHA256_Final(hash, &sha256);
  stringstream ss;
  for (int i = 0; i < SHA256_DIGEST_LENGTH; i++) {
    ss << hex << setw(2) << setfill('0') << (int)hash[i];
  }
  return ss.str();
}
