#include <gtest/gtest.h>
#include <fstream>
#include <iostream>
#include <cstdio>
#include "RSACrypto.h"  // Include your RSACrypto header file

// Define paths for temporary key files
const std::string publicKeyPath = "test_public.pem";
const std::string privateKeyPath = "test_private.pem";

class RSACryptoTest : public ::testing::Test {
 protected:
  // Setup is called before each test is run
  void SetUp() override {
    // Create an RSACrypto object with the temporary key paths
    crypto = new RSACrypto(publicKeyPath, privateKeyPath);
  }

  // TearDown is called after each test is run
  void TearDown() override {
    // Clean up key files after each test
    delete crypto;
    std::remove(publicKeyPath.c_str());
    std::remove(privateKeyPath.c_str());
  }

  RSACrypto* crypto;
};

TEST_F(RSACryptoTest, GenerateKeysSuccess) {
  EXPECT_TRUE(crypto->generateKeys());
  std::ifstream pubFile(publicKeyPath);
  std::ifstream privFile(privateKeyPath);
  EXPECT_TRUE(pubFile.good());
  EXPECT_TRUE(privFile.good());
}

TEST_F(RSACryptoTest, EncryptionDecryption) {
  ASSERT_TRUE(crypto->generateKeys());

  std::string originalMessage = "Hello, RSA!";
  std::string encryptedMessage = crypto->encrypt(originalMessage);
  EXPECT_FALSE(encryptedMessage.empty());

  std::string decryptedMessage = crypto->decrypt(encryptedMessage);
  EXPECT_EQ(originalMessage, decryptedMessage);
}

TEST_F(RSACryptoTest, DecryptWithNoKey) {
  std::string encryptedMessage = "invalid-encrypted-text";
  std::string decryptedMessage = crypto->decrypt(encryptedMessage);
  EXPECT_TRUE(decryptedMessage.empty());
}

int main(int argc, char** argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}