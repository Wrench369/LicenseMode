#include <openssl/err.h>
#include <openssl/pem.h>
#include <openssl/rsa.h>
#include "base64.hpp"
#include <fstream>
#include <iostream>
#include <string>

// Handles RSA encryption, decryption, and key generation.
// Utilizes the OpenSSL library for cryptographic operations.
class RSACrypto {
 public:
  // Constructs an RSACrypto object with given key paths.
  // @param publicKeyPath Path to the public RSA key.
  // @param privateKeyPath Path to the private RSA key.
  RSACrypto(const std::string& publicKeyPath, const std::string& privateKeyPath)
      : publicKeyPath(publicKeyPath), privateKeyPath(privateKeyPath) {}

  // Encrypts a message using the public key.
  // @param message The plaintext message to encrypt.
  // @return Encrypted message as a string.
  std::string encrypt(const std::string& message) {
    RSA* rsaPublic = loadPublicKey();
    if (!rsaPublic) return "";

    // Ensure the encrypted buffer size matches the key size
    std::string encrypted(RSA_size(rsaPublic), '\0');

    // Message size check for PKCS1_OAEP_PADDING
    if (message.size() > RSA_size(rsaPublic) - 42) {
      std::cerr << "Error: Message too large for the RSA key size.\n";
      RSA_free(rsaPublic);
      return "";
    }

    // Encrypt the message
    int result = RSA_public_encrypt(
        message.size(), reinterpret_cast<const unsigned char*>(message.c_str()),
        reinterpret_cast<unsigned char*>(&encrypted[0]), rsaPublic,
        RSA_PKCS1_OAEP_PADDING);

    RSA_free(rsaPublic);

    if (result == -1) {
      ERR_print_errors_fp(stderr);
      return "";
    }

    // base64 encryption
    auto encoded_str =  base64::to_base64(encrypted);

    return encoded_str;
  }

  // Decrypts a message using the private key.
  // @param encryptedMessage The encrypted message to decrypt.
  // @return Decrypted message as a string.
  std::string decrypt(const std::string& encryptedMessage) {
    // base64 decryption
    auto decoded_str = base64::from_base64(encryptedMessage);

    // Load private key
    RSA* rsaPrivate = loadPrivateKey();
    if (!rsaPrivate) return "";

    // Decrypt message
    std::string decrypted(RSA_size(rsaPrivate), '\0');
    int result = RSA_private_decrypt(
        decoded_str.size(),
        reinterpret_cast<const unsigned char*>(decoded_str.c_str()),
        reinterpret_cast<unsigned char*>(&decrypted[0]), rsaPrivate,
        RSA_PKCS1_OAEP_PADDING);
    RSA_free(rsaPrivate);

    if (result == -1) {
      handleError();
      return "";
    }

    decrypted.resize(result);
    return decrypted;
  }

  // Generates and saves a public-private RSA key pair.
  // @param bits The size of the keys, typically 2048.
  // @return True if key generation and saving is successful, otherwise false.
  bool generateKeys(int bits = 2048) {
    RSA* rsa = RSA_new();
    BIGNUM* bn = BN_new();
    if (!bn) {
      std::cerr << "Error creating BIGNUM.\n";
      RSA_free(rsa);
      return false;
    }

    if (!BN_set_word(bn, RSA_F4)) {
      std::cerr << "Error setting BIGNUM word.\n";
      RSA_free(rsa);
      BN_free(bn);
      return false;
    }

    if (RSA_generate_key_ex(rsa, bits, bn, nullptr) != 1) {
      handleError();
      RSA_free(rsa);
      BN_free(bn);
      return false;
    }

    // Save keys to files
    bool success = savePublicKey(rsa) && savePrivateKey(rsa);

    RSA_free(rsa);
    BN_free(bn);
    return success;
  }

 private:
  std::string publicKeyPath;   // Path to the public RSA key file.
  std::string privateKeyPath;  // Path to the private RSA key file.

  // Loads the public key from file.
  // @return A pointer to the RSA structure containing the public key. nullptr
  // on failure.
  RSA* loadPublicKey() {
    std::ifstream file(publicKeyPath, std::ios::binary);
    if (!file) {
      std::cerr << "Error opening public key file.\n";
      return nullptr;
    }
    std::string key((std::istreambuf_iterator<char>(file)),
                    std::istreambuf_iterator<char>());
    BIO* keyBio = BIO_new_mem_buf(key.c_str(), -1);
    if (!keyBio) {
      std::cerr << "Error creating BIO for public key.\n";
      return nullptr;
    }
    RSA* rsa = PEM_read_bio_RSA_PUBKEY(keyBio, NULL, NULL, NULL);
    BIO_free(keyBio);
    if (!rsa) {
      ERR_print_errors_fp(stderr);
    }
    return rsa;
  }

  // Loads the private key from file.
  // @return A pointer to the RSA structure containing the private key. nullptr
  // on failure.
  RSA* loadPrivateKey() {
    std::ifstream file(privateKeyPath, std::ios::binary);
    if (!file) {
      std::cerr << "Error opening private key file.\n";
      return nullptr;
    }
    std::string key((std::istreambuf_iterator<char>(file)),
                    std::istreambuf_iterator<char>());
    BIO* keyBio = BIO_new_mem_buf(key.c_str(), -1);
    if (!keyBio) {
      std::cerr << "Error creating BIO for private key.\n";
      return nullptr;
    }
    RSA* rsa = PEM_read_bio_RSAPrivateKey(keyBio, NULL, NULL, NULL);
    BIO_free(keyBio);
    if (!rsa) {
      handleError();
    }
    return rsa;
  }

  // Saves the public key to file.
  // @param rsa The RSA object containing the key pair.
  // @return True on success, False otherwise.
  bool savePublicKey(RSA* rsa) {
    BIO* bio = BIO_new_file(publicKeyPath.c_str(), "w+");
    if (!bio) {
      std::cerr << "Error opening file to write public key.\n";
      return false;
    }
    if (!PEM_write_bio_RSA_PUBKEY(bio, rsa)) {
      BIO_free(bio);
      handleError();
      return false;
    }
    BIO_free(bio);
    return true;
  }

  // Saves the private key to file.
  // @param rsa The RSA object containing the key pair.
  // @return True on success, False otherwise.
  bool savePrivateKey(RSA* rsa) {
    BIO* bio = BIO_new_file(privateKeyPath.c_str(), "w+");
    if (!bio) {
      std::cerr << "Error opening file to write private key.\n";
      return false;
    }
    if (!PEM_write_bio_RSAPrivateKey(bio, rsa, NULL, NULL, 0, NULL, NULL)) {
      BIO_free(bio);
      handleError();
      return false;
    }
    BIO_free(bio);
    return true;
  }

  // Handles OpenSSL errors by printing them to stderr.
  void handleError() { ERR_print_errors_fp(stderr); }
};