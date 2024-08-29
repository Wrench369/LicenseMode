#include "License.h"

#include <gtest/gtest.h>

// Test the constructor and basic accessors
TEST(LicenseTest, ConstructorAndAccessors) {
  License license("2024-08-29T15:26:44", "2025-08-29T15:26:44", "machine-12345",
                  RoleType::PAID, "OpenAI");

  EXPECT_EQ(license.getAuthorizationTime(), "2024-08-29T15:26:44");
  EXPECT_EQ(license.getValidTime(), "2025-08-29T15:26:44");
  EXPECT_EQ(license.getMachineId(), "machine-12345");
  EXPECT_EQ(license.getRoleType(), RoleType::PAID);
  EXPECT_EQ(license.getCompanyName(), "OpenAI");
}

// Test serialization of the License object
TEST(LicenseTest, Serialization) {
  License license("2024-08-29T15:26:44", "2025-08-29T15:26:44", "machine-12345",
                  RoleType::PAID, "OpenAI");
  std::string json = license.serialize();

  nlohmann::json expected = {{"authorizationTime", "2024-08-29T15:26:44"},
                             {"validTime", "2025-08-29T15:26:44"},
                             {"machineId", "machine-12345"},
                             {"roleType", static_cast<int>(RoleType::PAID)},
                             {"companyName", "OpenAI"}};

  EXPECT_EQ(json, expected.dump());
}

// Test deserialization of JSON into a License object
TEST(LicenseTest, Deserialization) {
  std::string json = R"({
        "authorizationTime": "2024-08-29T15:26:44",
        "validTime": "2025-08-29T15:26:44",
        "machineId": "machine-12345",
        "roleType": 1,
        "companyName": "OpenAI"
    })";

  License license = License::deserialize(json);

  EXPECT_EQ(license.getAuthorizationTime(), "2024-08-29T15:26:44");
  EXPECT_EQ(license.getValidTime(), "2025-08-29T15:26:44");
  EXPECT_EQ(license.getMachineId(), "machine-12345");
  EXPECT_EQ(license.getRoleType(), RoleType::PAID);
  EXPECT_EQ(license.getCompanyName(), "OpenAI");
}

// Test deserialization of a License with an empty validTime (permanent license)
TEST(LicenseTest, PermanentLicenseDeserialization) {
  std::string json = R"({
        "authorizationTime": "2024-08-29T15:26:44",
        "validTime": "",
        "machineId": "machine-98765",
        "roleType": 2,
        "companyName": "PermanentCorp"
    })";

  License license = License::deserialize(json);

  EXPECT_EQ(license.getAuthorizationTime(), "2024-08-29T15:26:44");
  EXPECT_EQ(license.getValidTime(), "");  // Check empty valid time
  EXPECT_EQ(license.getMachineId(), "machine-98765");
  EXPECT_EQ(license.getRoleType(), RoleType::PERMANENT);
  EXPECT_EQ(license.getCompanyName(), "PermanentCorp");
}

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}