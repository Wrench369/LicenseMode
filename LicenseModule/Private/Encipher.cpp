#include "Encipher.h"
#include <cstring>

#define KEY_LENGTH 2048            // 密钥长度
#define PUB_KEY_FILE "pubkey.pem"  // 公钥路径
#define PRI_KEY_FILE "prikey.pem"  // 私钥路径

void LicenseModule::Encipher::GenerateRSAKey(std::string &out_pub_key,
                                             std::string &out_pri_key) {
  size_t pri_len = 0;       // 私钥长度
  size_t pub_len = 0;       // 公钥长度
  char *pri_key = nullptr;  // 私钥
  char *pub_key = nullptr;  // 公钥

  // 生成密钥对
  RSA *keypair = RSA_generate_key(KEY_LENGTH, RSA_3, NULL, NULL);

  BIO *pri = BIO_new(BIO_s_mem());
  BIO *pub = BIO_new(BIO_s_mem());

  // 生成私钥
  PEM_write_bio_RSAPrivateKey(pri, keypair, NULL, NULL, 0, NULL, NULL);
  // 注意------生成第1种格式的公钥
  // PEM_write_bio_RSAPublicKey(pub, keypair);
  // 注意------生成第2种格式的公钥（此处代码中使用这种）
  PEM_write_bio_RSA_PUBKEY(pub, keypair);

  // 获取长度
  pri_len = BIO_pending(pri);
  pub_len = BIO_pending(pub);

  // 密钥对读取到字符串
  pri_key = (char *)malloc(pri_len + 1);
  pub_key = (char *)malloc(pub_len + 1);

  BIO_read(pri, pri_key, pri_len);
  BIO_read(pub, pub_key, pub_len);

  pri_key[pri_len] = '\0';
  pub_key[pub_len] = '\0';

  out_pub_key = pub_key;
  out_pri_key = pri_key;

  // 将公钥写入文件
  std::ofstream pub_file(PUB_KEY_FILE, std::ios::out);
  if (!pub_file.is_open()) {
    perror("pub key file open fail:");
    return;
  }
  pub_file << pub_key;
  pub_file.close();

  // 将私钥写入文件
  std::ofstream pri_file(PRI_KEY_FILE, std::ios::out);
  if (!pri_file.is_open()) {
    perror("pri key file open fail:");
    return;
  }
  pri_file << pri_key;
  pri_file.close();

  // 释放内存
  RSA_free(keypair);
  BIO_free_all(pub);
  BIO_free_all(pri);

  free(pri_key);
  free(pub_key);
}

std::string LicenseModule::Encipher::RsaPriEncrypt(
    const std::string &clear_text, std::string &pri_key) {
  std::string encrypt_text;
  BIO *keybio = BIO_new_mem_buf((unsigned char *)pri_key.c_str(), -1);
  RSA *rsa = RSA_new();
  rsa = PEM_read_bio_RSAPrivateKey(keybio, &rsa, NULL, NULL);
  if (!rsa) {
    BIO_free_all(keybio);
    return std::string("");
  }

  // 获取RSA单次可以处理的数据的最大长度
  int len = RSA_size(rsa);

  // 申请内存：存贮加密后的密文数据
  char *text = new char[len + 1];
  memset(text, 0, len + 1);

  // 对数据进行私钥加密（返回值是加密后数据的长度）
  int ret = RSA_private_encrypt(clear_text.length(),
                                (const unsigned char *)clear_text.c_str(),
                                (unsigned char *)text, rsa, RSA_PKCS1_PADDING);
  if (ret >= 0) {
    encrypt_text = std::string(text, ret);
  }

  // 释放内存
  free(text);
  BIO_free_all(keybio);
  RSA_free(rsa);

  return encrypt_text;
}

std::string LicenseModule::Encipher::RsaPubDecrypt(
    const std::string &cipher_text, const std::string &pub_key) {
  std::string decrypt_text;
  BIO *keybio = BIO_new_mem_buf((unsigned char *)pub_key.c_str(), -1);
  RSA *rsa = RSA_new();

  // 注意--------使用第1种格式的公钥进行解密
  // rsa = PEM_read_bio_RSAPublicKey(keybio, &rsa, NULL, NULL);
  // 注意--------使用第2种格式的公钥进行解密（我们使用这种格式作为示例）
  rsa = PEM_read_bio_RSA_PUBKEY(keybio, &rsa, NULL, NULL);
  if (!rsa) {
    unsigned long err = ERR_get_error();  //获取错误号
    char err_msg[1024] = {0};
    ERR_error_string(err, err_msg);  // 格式：error:errId:库:函数:原因
    printf("err msg: err:%ld, msg:%s\n", err, err_msg);
    BIO_free_all(keybio);
    return decrypt_text;
  }

  int len = RSA_size(rsa);
  char *text = new char[len + 1];
  memset(text, 0, len + 1);
  // 对密文进行解密
  int ret = RSA_public_decrypt(cipher_text.length(),
                               (const unsigned char *)cipher_text.c_str(),
                               (unsigned char *)text, rsa, RSA_PKCS1_PADDING);
  if (ret >= 0) {
    decrypt_text.append(std::string(text, ret));
  }

  // 释放内存
  delete text;
  BIO_free_all(keybio);
  RSA_free(rsa);

  return decrypt_text;
}
