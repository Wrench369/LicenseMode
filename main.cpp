#include <iostream>
#include "License.h"
#include "LicenseManager.h"

// Function to print the license details.
void printLicenseDetails(const License& license) {
    std::cout << "Authorization Time: " << license.getAuthorizationTime() << "\n";
    std::cout << "Valid Time: " << (license.getValidTime().empty() ? "Unlimited" : license.getValidTime()) << "\n";
    std::cout << "Machine ID: " << license.getMachineId() << "\n";
    std::cout << "Role Type: " << static_cast<int>(license.getRoleType()) << "\n";
    std::cout << "Company Name: " << license.getCompanyName() << "\n";
}

int main() {
    // Paths to public and private RSA keys (dummy paths for demonstration).
    std::string publicKeyPath = "pubkey.pem";
    std::string privateKeyPath = "prikey.pem";
    
    // Create an instance of LicenseManager with given RSA key paths.
    LicenseManager licenseManager(publicKeyPath, privateKeyPath);

    // Create trial user license.
    License trialLicense = licenseManager.createLicense(RoleType::TRIAL, "Example Company", "MACHINE123");
    std::cout << "Trial License Created:\n";
    printLicenseDetails(trialLicense);
    std::cout << "\n";

    // Create paid user license.
    License paidLicense = licenseManager.createLicense(RoleType::PAID, "Example Company", "MACHINE123");
    std::cout << "Paid License Created:\n";
    printLicenseDetails(paidLicense);
    std::cout << "\n";

    // Create permanent user license.
    License permanentLicense = licenseManager.createLicense(RoleType::PERMANENT, "Example Company", "MACHINE123");
    std::cout << "Permanent License Created:\n";
    printLicenseDetails(permanentLicense);
    std::cout << "\n";

    // Encrypt and then decrypt the trial license.
    std::string encryptedData = licenseManager.encryptLicense(trialLicense);
    std::cout << "Encrypted Trial License: " << encryptedData << "\n\n";

    License decryptedLicense = licenseManager.decryptLicense(encryptedData);
    std::cout << "Decrypted Trial License:\n";
    printLicenseDetails(decryptedLicense);
    std::cout << "\n";

    // Validate the licenses using the current machine ID and time.
    std::string currentTime = licenseManager.getCurrentTime();
    std::string currentMachineId = "MACHINE123";

    bool isTrialLicenseValid = licenseManager.validateLicense(trialLicense, currentMachineId, currentTime);
    std::cout << "Is Trial License Valid? " << (isTrialLicenseValid ? "Yes" : "No") << "\n";
    
    bool isPaidLicenseValid = licenseManager.validateLicense(paidLicense, currentMachineId, currentTime);
    std::cout << "Is Paid License Valid? " << (isPaidLicenseValid ? "Yes" : "No") << "\n";
    
    bool isPermanentLicenseValid = licenseManager.validateLicense(permanentLicense, currentMachineId, currentTime);
    std::cout << "Is Permanent License Valid? " << (isPermanentLicenseValid ? "Yes" : "No") << "\n";

    return 0;
}