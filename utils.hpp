#pragma once

#include <iomanip>
#include <iostream>
#include <openssl/sha.h>
#include <sstream>
#include <string>

using namespace std;

// get file name for a given file path
inline string get_file_name(string file_path) {
  int ind = file_path.length() - 1;
  string file_name = "";
  while (ind >= 0 && file_path[ind] != '/' && file_path[ind] != '\\') {
    file_name = file_path[ind] + file_name;
    ind--;
  }
  return file_name;
}

// get hash string string from a hash digest
inline void sha256_hash_string(unsigned char hash[SHA256_DIGEST_LENGTH],
                               char outputBuffer[65]) {
  int i = 0;

  for (i = 0; i < SHA256_DIGEST_LENGTH; i++) {
    sprintf(outputBuffer + (i * 2), "%02x", hash[i]);
  }

  outputBuffer[64] = 0;
}
// get SHA-256 hash for a given file
inline int sha256_file(char *path, char outputBuffer[65]) {
  FILE *file = fopen(path, "rb");
  if (!file)
    return -534;

  unsigned char hash[SHA256_DIGEST_LENGTH];
  SHA256_CTX sha256;
  SHA256_Init(&sha256);
  const int bufSize = 32768;
  unsigned char *buffer = (unsigned char *)malloc(bufSize);
  int bytesRead = 0;
  if (!buffer)
    return ENOMEM;
  while ((bytesRead = fread(buffer, 1, bufSize, file))) {
    SHA256_Update(&sha256, buffer, bytesRead);
  }
  SHA256_Final(hash, &sha256);

  sha256_hash_string(hash, outputBuffer);
  fclose(file);
  free(buffer);
  return 0;
}
