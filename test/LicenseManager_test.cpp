#include "LicenseManager.h"

#include <gtest/gtest.h>

#include "License.h"
#include "RSACrypto.h"

class LicenseManagerTest : public ::testing::Test {
 protected:
  LicenseManager licenseManager = LicenseManager("public.pem", "private.pem");
};

TEST_F(LicenseManagerTest, CreateLicenseTrial) {
  License license =
      licenseManager.createLicense(RoleType::TRIAL, "CompanyA", "Machine1");
  EXPECT_EQ(license.getRoleType(), RoleType::TRIAL);
}

TEST_F(LicenseManagerTest, CreateLicensePaid) {
  License license =
      licenseManager.createLicense(RoleType::PAID, "CompanyB", "Machine2");
  EXPECT_EQ(license.getRoleType(), RoleType::PAID);
}

TEST_F(LicenseManagerTest, EncryptDecryptLicense) {
  License originalLicense =
      licenseManager.createLicense(RoleType::TRIAL, "CompanyA", "Machine1");
  std::string encryptedData = licenseManager.encryptLicense(originalLicense);
  License decryptedLicense = licenseManager.decryptLicense(encryptedData);

  EXPECT_EQ(originalLicense.getRoleType(), decryptedLicense.getRoleType());
  EXPECT_EQ(originalLicense.getMachineId(), decryptedLicense.getMachineId());
}

TEST_F(LicenseManagerTest, ValidateLicenseValidTrial) {
  License license =
      licenseManager.createLicense(RoleType::TRIAL, "CompanyA", "Machine1");
  std::string currentDate = licenseManager.getCurrentTime();

  EXPECT_TRUE(licenseManager.validateLicense(license, "Machine1", currentDate));
}

TEST_F(LicenseManagerTest, ValidateLicenseExpiredTrial) {
  License license =
      licenseManager.createLicense(RoleType::TRIAL, "CompanyA", "Machine1");
  std::string expiredDate = "2000-01-01";  // A past date

  EXPECT_FALSE(
      licenseManager.validateLicense(license, "Machine1", expiredDate));
}

TEST_F(LicenseManagerTest, ValidateLicenseWrongMachine) {
  License license =
      licenseManager.createLicense(RoleType::TRIAL, "CompanyA", "Machine1");
  std::string currentDate = licenseManager.getCurrentTime();

  EXPECT_FALSE(
      licenseManager.validateLicense(license, "DifferentMachine", currentDate));
}

TEST_F(LicenseManagerTest, ValidatePermanentLicense) {
  License license =
      licenseManager.createLicense(RoleType::PERMANENT, "CompanyA", "Machine1");

  EXPECT_TRUE(licenseManager.validateLicense(license, "Machine1",
                                             "2050-01-01"));  // Far future date
}

int main(int argc, char** argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}