#pragma once

#include <openssl/err.h>
#include <openssl/pem.h>
#include <iostream>
#include <string>

// Provides RSA encryption and decryption using OpenSSL.
// Supports loading keys from specified file paths.
class RSACrypto {
 public:
  // Constructs an RSACrypto object using paths to public and private key files.
  // @param publicKeyPath Path to the public key file.
  // @param privateKeyPath Path to the private key file.
  RSACrypto(const std::string& publicKeyPath, const std::string& privateKeyPath)
      : rsaPublicKey(nullptr), rsaPrivateKey(nullptr) {
    rsaPublicKey = loadPublicKey(publicKeyPath);
    rsaPrivateKey = loadPrivateKey(privateKeyPath);
  }

  // Destructor to free allocated RSA key resources.
  ~RSACrypto() {
    if (rsaPublicKey) RSA_free(rsaPublicKey);
    if (rsaPrivateKey) RSA_free(rsaPrivateKey);
  }

  // Encrypts data using the public key.
  // @param data The plaintext data to encrypt.
  // @return The encrypted data as a string.
  std::string encrypt(const std::string& data) {
    if (!rsaPublicKey) return "";

    std::string encrypted(RSA_size(rsaPublicKey), '\0');
    int result = RSA_public_encrypt(
        data.size(), reinterpret_cast<const unsigned char*>(data.c_str()),
        reinterpret_cast<unsigned char*>(&encrypted[0]), rsaPublicKey,
        RSA_PKCS1_OAEP_PADDING);

    if (result == -1) {
      printLastError("Encryption failed");
      return "";
    }

    return encrypted;
  }

  // Decrypts data using the private key.
  // @param data The encrypted data to decrypt.
  // @return The decrypted plaintext data as a string.
  std::string decrypt(const std::string& data) {
    if (!rsaPrivateKey) return "";

    std::string decrypted(RSA_size(rsaPrivateKey), '\0');
    int result = RSA_private_decrypt(
        data.size(), reinterpret_cast<const unsigned char*>(data.c_str()),
        reinterpret_cast<unsigned char*>(&decrypted[0]), rsaPrivateKey,
        RSA_PKCS1_OAEP_PADDING);

    if (result == -1) {
      printLastError("Decryption failed");
      return "";
    }

    return decrypted.substr(0, result);  // Truncate to actual length
  }

 private:
  RSA* rsaPublicKey;   // The RSA public key used for encryption.
  RSA* rsaPrivateKey;  // The RSA private key used for decryption.

  // Loads the public key from a specified file path.
  // @param publicKeyPath Path to the public key file.
  // @return A pointer to the RSA structure containing the public key.
  RSA* loadPublicKey(const std::string& publicKeyPath) {
    FILE* fp = fopen(publicKeyPath.c_str(), "rb");
    if (!fp) {
      std::cerr << "Failed to open public key file\n";
      return nullptr;
    }
    RSA* rsa = PEM_read_RSA_PUBKEY(fp, nullptr, nullptr, nullptr);
    fclose(fp);
    return rsa;
  }

  // Loads the private key from a specified file path.
  // @param privateKeyPath Path to the private key file.
  // @return A pointer to the RSA structure containing the private key.
  RSA* loadPrivateKey(const std::string& privateKeyPath) {
    FILE* fp = fopen(privateKeyPath.c_str(), "rb");
    if (!fp) {
      std::cerr << "Failed to open private key file\n";
      return nullptr;
    }
    RSA* rsa = PEM_read_RSAPrivateKey(fp, nullptr, nullptr, nullptr);
    fclose(fp);
    return rsa;
  }

  // Prints the last OpenSSL error message.
  // @param msg A custom message to include with the error output.
  void printLastError(const char* msg) {
    char* err = ERR_error_string(ERR_get_error(), nullptr);
    std::cerr << msg << ": " << err << "\n";
  }
};