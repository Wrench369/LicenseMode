#include <iostream>
#include "License.h"
#include "LicenseManager.h"
#include "MachineCodeGenerator.h"

MachineCodeGenerator mcg;

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
    licenseManager.crypto.generateKeys();

    std::string machineCode = mcg.generateMachineCode();
    std::cout << "machineCode: " << machineCode << std::endl;

    // Create trial user license.
    License trialLicense = licenseManager.createLicense(RoleType::TRIAL, "Example Company", machineCode);
    std::cout << "Trial License Created:\n";
    printLicenseDetails(trialLicense);
    std::cout << std::endl;

    // Create paid user license.
    License paidLicense = licenseManager.createLicense(RoleType::PAID, "Example Company", machineCode);
    std::cout << "Paid License Created:\n";
    printLicenseDetails(paidLicense);
    std::cout << std::endl;

    // Create permanent user license.
    License permanentLicense = licenseManager.createLicense(RoleType::PERMANENT, "Example Company", machineCode);
    std::cout << "Permanent License Created:\n";
    printLicenseDetails(permanentLicense);
    std::cout << std::endl;

    // Encrypt and then decrypt the trial license.
    std::string encryptedData = licenseManager.encryptLicense(trialLicense);
    std::cout << "Encrypted Trial License: " << encryptedData << std::endl;

    License decryptedLicense = licenseManager.decryptLicense(encryptedData);
    std::cout << "Decrypted Trial License:\n";
    printLicenseDetails(decryptedLicense);
    std::cout << std::endl;

    // Validate the licenses using the current machine ID and time.
    std::string currentTime = licenseManager.getCurrentTime();
    std::string currentMachineId = mcg.generateMachineCode();

    bool isTrialLicenseValid = licenseManager.validateLicense(trialLicense, currentMachineId, currentTime);
    std::cout << "Is Trial License Valid? " << (isTrialLicenseValid ? "Yes" : "No") << std::endl;
    
    bool isPaidLicenseValid = licenseManager.validateLicense(paidLicense, currentMachineId, currentTime);
    std::cout << "Is Paid License Valid? " << (isPaidLicenseValid ? "Yes" : "No") << std::endl;
    
    bool isPermanentLicenseValid = licenseManager.validateLicense(permanentLicense, currentMachineId, currentTime);
    std::cout << "Is Permanent License Valid? " << (isPermanentLicenseValid ? "Yes" : "No") << std::endl;

    return 0;
}