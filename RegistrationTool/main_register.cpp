#include <fstream>
#include <iostream>

#include "License.h"
#include "LicenseManager.h"
#include "MachineCodeGenerator.h"

void saveEncryptedLicense(std::string encryptedLicense) {
  std::string filename = "License.LIC";
  std::ofstream file(filename, std::ios::out);
  if (file.is_open()) {
    file << encryptedLicense;
  }
}

int main(int argc, char** argv) {
  MachineCodeGenerator mcg;

  int choicedNumber = -1;
  std::string authorizationTime, roleType, companyName;
  std::cout << "请选择注册权限（1.试用，2.年费，3.终身会员）：" << std::endl;
  std::cout << "填入对应的数字：";
  std::cin >> choicedNumber;

  // Paths to public and private RSA keys (dummy paths for demonstration).
  std::string publicKeyPath = "pubkey.pem";
  std::string privateKeyPath = "prikey.pem";

  // Create an instance of LicenseManager with given RSA key paths.
  LicenseManager licenseManager(publicKeyPath, privateKeyPath);
  std::string machineCode = mcg.generateMachineCode();
  // std::string machineCode = "Test";
  std::cout << "machineCode: " << machineCode << std::endl;

  switch (choicedNumber) {
    case 1: {
      std::cout << "试用版" << std::endl;
      License trialLicense = licenseManager.createLicense(
          RoleType::TRIAL, "Example Company", machineCode);
      // Encrypt and then decrypt the trial license.
      std::string encryptedData = licenseManager.encryptLicense(trialLicense);
      saveEncryptedLicense(encryptedData);
    } break;
    case 2: {
      std::cout << "年费版" << std::endl;
      License paidLicense = licenseManager.createLicense(
          RoleType::PAID, "Example Company", machineCode);
      // Encrypt and then decrypt the trial license.
      std::string encryptedData = licenseManager.encryptLicense(paidLicense);
      saveEncryptedLicense(encryptedData);
    } break;
    case 3: {
      std::cout << "终身免费版" << std::endl;
      License permanentLicense = licenseManager.createLicense(
          RoleType::PERMANENT, "Example Company", machineCode);
      // Encrypt and then decrypt the trial license.
      std::string encryptedData =
          licenseManager.encryptLicense(permanentLicense);
      saveEncryptedLicense(encryptedData);
    } break;
    default: {
      std::cout << "选项错误，请选择正确的选项。" << std::endl;
    } break;
  }
  return 0;
}