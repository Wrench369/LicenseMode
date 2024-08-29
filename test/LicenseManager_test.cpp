#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include "LicenseManager.h"
#include "License.h"
#include "RSACrypto.h"

// Mock definitions for external dependencies

class MockRSACrypto {
public:
    MOCK_METHOD(std::string, encrypt, (const std::string&), (const));
    MOCK_METHOD(std::string, decrypt, (const std::string&), (const));
};

class MockLicense {
public:
    MOCK_METHOD(std::string, serialize, (), (const));
    MOCK_METHOD(static License, deserialize, (const std::string&), ());
    MOCK_METHOD(std::string, getMachineId, (), (const));
    MOCK_METHOD(std::string, getValidTime, (), (const));
    MOCK_METHOD(RoleType, getRoleType, (), (const));
};

// Fixture for LicenseManagerTest
class LicenseManagerTest : public ::testing::Test {
protected:
    LicenseManagerTest() : manager("public.key", "private.key"), mockCrypto(), mockLicense() {}

    void SetUp() override {
        manager.crypto = mockCrypto;  // Inject mock crypto
    }

    LicenseManager manager;
    MockRSACrypto mockCrypto;
    MockLicense mockLicense;
};

TEST_F(LicenseManagerTest, CreateLicenseTrial) {
    RoleType role = RoleType::TRIAL;
    std::string companyName = "ABC Corp";
    std::string machineId = "1234";

    License license = manager.createLicense(role, companyName, machineId);

    EXPECT_EQ(license.getRoleType(), role);
    EXPECT_EQ(license.getMachineId(), machineId);
    EXPECT_EQ(license.getCompanyName(), companyName);
    // Further checks can be added based on expected values for authorization and valid times
}

TEST_F(LicenseManagerTest, EncryptLicense) {
    std::string serializedData = "serialized_license_data";
    std::string encryptedData = "encrypted_data";

    ON_CALL(mockLicense, serialize()).WillByDefault(::testing::Return(serializedData));
    EXPECT_CALL(mockCrypto, encrypt(serializedData)).WillOnce(::testing::Return(encryptedData));

    std::string result = manager.encryptLicense(mockLicense);
    EXPECT_EQ(result, encryptedData);
}

TEST_F(LicenseManagerTest, DecryptLicense) {
    std::string encryptedData = "encrypted_data";
    std::string decryptedData = "decrypted_license_data";

    ON_CALL(mockCrypto, decrypt(encryptedData)).WillByDefault(::testing::Return(decryptedData));
    EXPECT_CALL(MockLicense::deserialize(decryptedData)).WillOnce(::testing::Return(mockLicense));

    License license = manager.decryptLicense(encryptedData);
    // Assuming the mockLicense will be returned, further checks can be made based on expected behavior
}

TEST_F(LicenseManagerTest, ValidateLicenseValid) {
    // Configure mock license behavior
    std::string currentMachineId = "1234";
    std::string currentTime = "2024-08-29";

    ON_CALL(mockLicense, getMachineId()).WillByDefault(::testing::Return(currentMachineId));
    ON_CALL(mockLicense, getValidTime()).WillByDefault(::testing::Return("2024-09-29"));
    ON_CALL(mockLicense, getRoleType()).WillByDefault(::testing::Return(RoleType::PAID));

    bool isValid = manager.validateLicense(mockLicense, currentMachineId, currentTime);
    EXPECT_TRUE(isValid);
}

TEST_F(LicenseManagerTest, ValidateLicenseExpired) {
    std::string currentMachineId = "1234";
    std::string currentTime = "2024-08-29";

    ON_CALL(mockLicense, getMachineId()).WillByDefault(::testing::Return(currentMachineId));
    ON_CALL(mockLicense, getValidTime()).WillByDefault(::testing::Return("2023-01-01"));
    ON_CALL(mockLicense, getRoleType()).WillByDefault(::testing::Return(RoleType::PAID));

    bool isValid = manager.validateLicense(mockLicense, currentMachineId, currentTime);
    EXPECT_FALSE(isValid);
}

TEST_F(LicenseManagerTest, ValidatePermanentLicense) {
    std::string currentMachineId = "1234";

    ON_CALL(mockLicense, getMachineId()).WillByDefault(::testing::Return(currentMachineId));
    ON_CALL(mockLicense, getRoleType()).WillByDefault(::testing::Return(RoleType::PERMANENT));

    bool isValid = manager.validateLicense(mockLicense, currentMachineId, "2024-08-29");
    EXPECT_TRUE(isValid);
}

int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}