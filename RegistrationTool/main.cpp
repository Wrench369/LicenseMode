#include <fstream>
#include <iostream>
#include <nlohmann/json.hpp>
#include <string>

#include "Encipher.h"

using json = nlohmann::json;

int main(int argc, char** argv) {
  LicenseModule::Encipher encipher;

  int choicedNumber = -1;
  std::string authorizationTime, roleType, companyName;
  std::cout << "请选择注册权限（1.试用，2.年费，3.终身会员）：" << std::endl;
  std::cout << "填入对应的数字：";
  std::cin >> choicedNumber;
  switch (choicedNumber) {
    case 1: {
      std::cout << "试用版" << std::endl;
      // 生成默认的有15天使用权限的授权证书
      std::ifstream probationaryCertificate(
          "AuthorizedFile/ProbationaryCertificate.json");
      json certificate;
      probationaryCertificate >> certificate;
      // std::cout << certificate.dump() << std::endl;

      std::string encrypt_text;
      std::string decrypt_text;

      // 生成密钥对
      std::string pub_key;
      std::string pri_key;
      encipher.GenerateRSAKey(pub_key, pri_key);
      printf("public key:\n");
      printf("%s\n", pub_key.c_str());
      printf("private key:\n");
      printf("%s\n", pri_key.c_str());

      // 私钥加密
      // 1.先RSA非对称加密
      encrypt_text = encipher.RsaPriEncrypt(certificate.dump(), pri_key);
      std::cout << "encrypt_text: " << encrypt_text << std::endl;
      // 2.再BASE64加密
      auto encoded_str = base64::to_base64(encrypt_text);
      std::cout << "BASE64-encoded_str: " << encoded_str << std::endl;
      std::ofstream cipherText("cipherText.txt");
      cipherText << encoded_str;

      // 公钥解密
      // 1.解RSA非对称加密
      auto decoded_str = base64::from_base64(encoded_str);
      // 2.解BASE64加密
      decrypt_text = encipher.RsaPubDecrypt(decoded_str, pub_key);
      std::cout << decrypt_text << std::endl;

    } break;
    case 2: {
      std::cout << "年费版" << std::endl;
      // 生成使用权限为1年的授权证书
    } break;
    case 3: {
      std::cout << "终身免费版" << std::endl;
    } break;
    default: {
      std::cout << "选项错误，请选择正确的选项。" << std::endl;
    } break;
  }
  return 0;
}