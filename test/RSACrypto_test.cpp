#include <gtest/gtest.h>
#include <fstream>
#include <string>
#include <cstdio>
#include "RSACrypto.h"

// You might need to adjust the include path for the RSACrypto class.

class RSACryptoTest : public ::testing::Test {
protected:
    void SetUp() override {
        publicKeyPath = "test_public_key.pem";
        privateKeyPath = "test_private_key.pem";
    }

    void TearDown() override {
        // Remove the files after test
        std::remove(publicKeyPath.c_str());
        std::remove(privateKeyPath.c_str());
    }

    std::string publicKeyPath;
    std::string privateKeyPath;
};

TEST_F(RSACryptoTest, KeyGeneration) {
    RSACrypto rsa(publicKeyPath, privateKeyPath);
    EXPECT_TRUE(rsa.generateKeys());
    
    // Ensure the keys are stored
    std::ifstream pubFile(publicKeyPath);
    std::ifstream privFile(privateKeyPath);
    EXPECT_TRUE(pubFile.is_open());
    EXPECT_TRUE(privFile.is_open());
}

TEST_F(RSACryptoTest, EncryptDecrypt) {
    RSACrypto rsa(publicKeyPath, privateKeyPath);
    ASSERT_TRUE(rsa.generateKeys());

    std::string message = "This is a secret message!";
    std::string encrypted = rsa.encrypt(message);
    ASSERT_FALSE(encrypted.empty());

    std::string decrypted = rsa.decrypt(encrypted);
    EXPECT_EQ(message, decrypted);
}

TEST_F(RSACryptoTest, InvalidPublicKey) {
    RSACrypto rsa("nonexistent_public_key.pem", privateKeyPath);
    std::string message = "Testing with invalid key";
    std::string encrypted = rsa.encrypt(message);
    EXPECT_TRUE(encrypted.empty());  // Encryption should fail
}

TEST_F(RSACryptoTest, InvalidPrivateKey) {
    RSACrypto rsa(publicKeyPath, "nonexistent_private_key.pem");
    std::string message = "Testing with valid public, invalid private";
    ASSERT_TRUE(rsa.generateKeys()); // Generate valid keys

    std::string encrypted = rsa.encrypt(message);
    ASSERT_FALSE(encrypted.empty());

    std::string decrypted = rsa.decrypt(encrypted);
    EXPECT_TRUE(decrypted.empty());  // Decryption should fail
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}