#ifndef A6D81A16_2CEF_4D1F_AA7F_9C4AD332AFCB
#define A6D81A16_2CEF_4D1F_AA7F_9C4AD332AFCB

#include <fstream>
#include <string>

#include "openssl/rsa.h"
#include "openssl/pem.h"
#include "openssl/err.h"

namespace LicenseModule {
class Encipher {
 public:
  void GenerateRSAKey(std::string &out_pub_key, std::string &out_pri_key);

  std::string RsaPriEncrypt(const std::string &clear_text,
                            std::string &pri_key);

  std::string RsaPubDecrypt(const std::string &cipher_text,
                            const std::string &pub_key);
};
}  // namespace LicenseModule

#endif /* A6D81A16_2CEF_4D1F_AA7F_9C4AD332AFCB */
