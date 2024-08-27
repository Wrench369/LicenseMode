#include <iostream>

#include "Encipher.h"
#include "HardwareInfo.h"
#include "LicenseInfo.h"
#include "base64.hpp"

std::string extractPublicKey(const std::string& publicKeyStr) {
    RSA* rsa_key = nullptr;

    BIO* bio = BIO_new_mem_buf(publicKeyStr.c_str(), publicKeyStr.length());
    if (!bio) {
        std::cerr << "Error creating BIO." << std::endl;
        return "";
    }

    RSA* rsa = PEM_read_bio_RSA_PUBKEY(bio, &rsa_key, nullptr, nullptr);
    
    if (!rsa) {
        std::cerr << "Error reading RSA public key." << std::endl;
        BIO_free(bio);
        return "";
    }

    // Convert RSA public key to a string
    BIO* out = BIO_new(BIO_s_mem());
    PEM_write_bio_RSAPublicKey(out, rsa);

    char* pem_data;
    long pem_size = BIO_get_mem_data(out, &pem_data);

    std::string result(pem_data, pem_size);

    RSA_free(rsa);
    BIO_free_all(bio); // Free both input and output BIO
    BIO_free(out);

    return result;
}

int main(int argc, char** argv) {
  LicenseModule::Encipher encipher;
  /*
  1.解密获取证书信息
    a.获取证书密文
    b.获取公钥并解密证书
  */
  std::string encrypt_text;
  std::ifstream cipherText("cipherText.txt");
  if (!cipherText.is_open()) {
    std::cout << "文件打开失败！" << std::endl;
  }
  cipherText >> encrypt_text;
  std::cout << "encrypt_text: " << encrypt_text << std::endl;
  cipherText.close();

  std::string pubKey;
  std::ifstream pemFile("pubkey.pem");
  if (!pemFile.is_open()) {
    std::cerr << "Error opening file;" << std::endl;
  }
  std::string publicKeyStr((std::istreambuf_iterator<char>(pemFile)),
                           std::istreambuf_iterator<char>());
  pemFile.close();

  std::cout << extractPublicKey(publicKeyStr) << std::endl;
  publicKeyStr =
      R"(MIIBCAKCAQEAwUJB7erkm8seW6JnwWKZs2GMGCuGdIVG4ANaQcVnaUJhkHahpJzH
cgxwSv28ifhX8JkFG77/F+5G3fs9rrlggrR7VFOzyRWZcckm2noPoED/ywpPRxQZ
pkWcYu1sOh08QqK19mVsF/PzfnjuigR2WOD+Iahqy9eeukm+JAj/V+r/P8azm0sC
YzvShYKt4XaFWIsf9z6xBZrSFa4ZaYPWjQSJA480la/v0Vuv+pvHLmwyCdDJ6B1D
Fvn6VpJTZfOOlc6OaXvgRCt+AGS9dyL7JnpE8kn4Rj8Bxh59AkIt9qzBecLWhCwt
VR8ta4aRrftzc2lZHeGznfp0TFk0uMWsYwIBAw==)";

  //解密
  // 解BASE64加密
  auto decoded_str = base64::from_base64(encrypt_text);
  // 解RSA非对称加密
  std::string decrypt_text =
      encipher.RsaPubDecrypt(decoded_str, publicKeyStr);
  std::cout << decrypt_text << std::endl;

  /*
  2.证书验证
    a.验证机器码
    b.验证证书时效性
  */

  return 0;
}