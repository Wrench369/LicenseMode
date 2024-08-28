#pragma once

#include <ctime>
#include <string>
#include "License.h"
#include "RSACrypto.h"

// Manages the creation, encryption, decryption, and validation of licenses.
// Utilizes RSACrypto for secure operations.
class LicenseManager {
 public:
  // Constructs a LicenseManager with specified RSA key paths.
  // @param publicKeyPath Path to the public RSA key.
  // @param privateKeyPath Path to the private RSA key.
  LicenseManager(const std::string& publicKeyPath,
                 const std::string& privateKeyPath)
      : crypto(publicKeyPath, privateKeyPath) {}

  // Creates a License with specified role, company, and machine identifier.
  // @param roleType Type of role for the license.
  // @param companyName Name of the company owning the license.
  // @param machineId Identifier of the machine the license is assigned to.
  // @return A License object containing the specified details.
  License createLicense(RoleType roleType, const std::string& companyName,
                        const std::string& machineId) {
    // Generate current time as the authorization time.
    std::string authorizationTime = getCurrentTime();
    std::string validTime;

    // Determine the validity period based on the role type.
    switch (roleType) {
      case RoleType::TRIAL:
        validTime = getDaysAheadTime(15);  // 15 days for trial users
        break;
      case RoleType::PAID:
        validTime = getDaysAheadTime(365);  // 1 year for paid users
        break;
      case RoleType::PERMANENT:
        validTime = "";  // No expiration for permanent users
        break;
    }

    return License(authorizationTime, validTime, machineId, roleType,
                   companyName);
  }

  // Encrypts the given License object.
  // @param license A License object to be encrypted.
  // @return A string containing the encrypted license data.
  std::string encryptLicense(const License& license) {
    std::string serializedLicense = license.serialize();
    return crypto.encrypt(serializedLicense);
  }

  // Decrypts the encrypted license data.
  // @param encryptedData The encrypted license data as a string.
  // @return A License object derived from the decrypted data.
  License decryptLicense(const std::string& encryptedData) {
    std::string decryptedData = crypto.decrypt(encryptedData);
    return License::deserialize(decryptedData);
  }

  // Validates a License against current machine ID and time.
  // @param license The License to be validated.
  // @param currentMachineId The current machine ID to validate against.
  // @param currentTime The current time to compare with license validity.
  // @return True if the license is valid; otherwise, false.
  bool validateLicense(const License& license,
                       const std::string& currentMachineId,
                       const std::string& currentTime) {
    if (license.getRoleType() == RoleType::PERMANENT) {
      // Permanent licenses are always valid for their machine.
      return license.getMachineId() == currentMachineId;
    }
    return (license.getMachineId() == currentMachineId) &&
           (license.getValidTime() > currentTime);
  }

  // Gets the current time as a string.
  // @return A string representing the current time in "YYYY-MM-DD" format.
  std::string getCurrentTime() {
    std::time_t t = std::time(nullptr);
    char buffer[100];
    if (std::strftime(buffer, sizeof(buffer), "%Y-%m-%d", std::localtime(&t))) {
      return buffer;
    }
    return "";
  }

 private:
  RSACrypto crypto;  // An instance of RSACrypto for encryption and decryption.

  // Calculates a time a specified number of days from now and returns it as a
  // string.
  // @param daysAhead The number of days ahead to calculate the future date.
  // @return A string representing the time a specified number of days ahead in
  // "YYYY-MM-DD" format.
  std::string getDaysAheadTime(int daysAhead) {
    std::time_t t = std::time(nullptr);
    std::tm* tm = std::localtime(&t);
    tm->tm_mday += daysAhead;
    std::mktime(tm);
    char buffer[100];
    if (std::strftime(buffer, sizeof(buffer), "%Y-%m-%d", tm)) {
      return buffer;
    }
    return "";
  }
};