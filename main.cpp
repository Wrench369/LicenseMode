#include <iostream>

#include "HardwareInfo.h"
#include "LicenseInfo.h"
#include "Encipher.h"

int main(int argc, char** argv) {
  LicenseModule::Encipher encipher;

  // 原始明文
  std::string src_text =
      "test begin\n this is an rsa test example!!! this is an rsa test "
      "example!!! this is an rsa test example!!! this is an rsa test "
      "example!!! this is an rsa test example!!! this is an rsa test "
      "example!!! this is an rsa test example!!! \ntest end";
  // src_text = "rsa test";

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

  // 私钥加密-公钥解密
  encrypt_text = encipher.RsaPriEncrypt(src_text, pri_key);
  printf("encrypt: len=%d\n", encrypt_text.length());
  decrypt_text = encipher.RsaPubDecrypt(encrypt_text, pub_key);
  printf("decrypt: len=%d\n", decrypt_text.length());
  printf("decrypt: %s\n", decrypt_text.c_str());

  return 0;
}