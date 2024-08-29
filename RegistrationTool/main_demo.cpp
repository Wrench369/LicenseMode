#include <iostream>

#include "License.h"
#include "LicenseManager.h"
#include "MachineCodeGenerator.h"

int main(int argc, char** arcv) {
  MachineCodeGenerator mcg;

  // Paths to public and private RSA keys (dummy paths for demonstration).
  std::string publicKeyPath = "pubkey.pem";
  std::string privateKeyPath = "prikey.pem";

  // Create an instance of LicenseManager with given RSA key paths.
  LicenseManager licenseManager(publicKeyPath, privateKeyPath);
  std::string machineCode = mcg.generateMachineCode();
  std::cout << "machineCode: " << machineCode << std::endl;
  // Validate the licenses using the current machine ID and time.
  std::string currentTime = licenseManager.getCurrentTime();
  std::string currentMachineId = mcg.generateMachineCode();

  std::string encryptedlicenseFilePath = "License.LIC";
  std::string encryptedlicense;
  std::ifstream file(encryptedlicenseFilePath, std::ios::binary);
  if (!file) {
    std::cerr << "Error opening file.\n";
  }
  file >> encryptedlicense;
  file.close();

  bool isTrialLicenseValid = licenseManager.validateLicense(
      encryptedlicense, currentMachineId, currentTime);
  std::cout << "Is Trial License Valid? "
            << (isTrialLicenseValid ? "Yes" : "No") << std::endl;
 
  if (isTrialLicenseValid) {
    // main program
    std::cout << "Hello World!!" << std::endl;
  }

  return 0;
}