// #include <iostream>

// #include "Encipher.h"
// #include "HardwareInfo.h"
// #include "LicenseInfo.h"
// #include "base64.hpp"

// bool ValidationMachineCode(json licenseText) {
//   std::string uuid;
//   char buffer[128];
//   FILE* pipe = popen("dmidecode -s system-uuid", "r");
//   if (pipe) {
//     while (fgets(buffer, sizeof(buffer), pipe) != nullptr) {
//       uuid += buffer;
//     }
//     pclose(pipe);
//   }
//   // Remove trailing newline character
//   if (!uuid.empty() && uuid.back() == '\n') {
//     uuid.pop_back();
//   }
//   std::cout << uuid << std::endl;

//   std::string machineCode;
//   if (licenseText.contains("machineId")) {
//     machineCode = licenseText["machineId"];
//   }

//   if (machineCode == LicenseModule::HardwareInfo::GetMachineCode()) {
//     return true;
//   } else {
//     return false;
//   }
// }

// int main(int argc, char** argv) {
//   LicenseModule::Encipher encipher;
//   /*
//   1.解密获取证书信息
//     a.获取证书密文
//     b.获取公钥并解密证书
//   */
//   std::string encrypt_text;
//   std::ifstream cipherText("cipherText.txt");
//   if (!cipherText.is_open()) {
//     std::cout << "文件打开失败！" << std::endl;
//   }
//   cipherText >> encrypt_text;
//   std::cout << "encrypt_text: " << encrypt_text << std::endl;
//   cipherText.close();

//   std::string pubKey;
//   std::ifstream pemFile("pubkey.pem");
//   if (!pemFile.is_open()) {
//     std::cerr << "Error opening file;" << std::endl;
//   }
//   std::string publicKeyStr((std::istreambuf_iterator<char>(pemFile)),
//                            std::istreambuf_iterator<char>());
//   pemFile.close();

//   //解密
//   // 解BASE64加密
//   auto decoded_str = base64::from_base64(encrypt_text);
//   // 解RSA非对称加密
//   std::string decrypt_text = encipher.RsaPubDecrypt(decoded_str,
//   publicKeyStr); std::cout << decrypt_text << std::endl;

//   auto j = json::parse(decrypt_text.c_str());

//   /*
//   2.证书验证
//     a.验证机器码
//     b.验证证书时效性
//   */
//   std::cout << ValidationMachineCode(j) << std::endl;

//   return 0;
// }

#include <ctime>
#include <iostream>
#include <string>
#include <thread>

#include "Encipher.h"

// LicenseException类
class LicenseException : public std::exception {
 public:
  LicenseException(const std::string& message) : message_(message) {}
  const char* what() const noexcept override { return message_.c_str(); }

 private:
  std::string message_;
};

// LicenseGenerator类
class LicenseGenerator {
 public:
  static std::string generateLicenseKey(const std::string& customerInfo,
                                        int validityPeriod) {
    // 实现生成许可证密钥的逻辑
    // ...
    return "generated_license_key";
  }
};

// LicenseValidator类
class LicenseValidator {
 public:
  static bool validateLicenseKey(const std::string& licenseKey) {
    // 实现验证许可证密钥的逻辑
    // ...
    return true;
  }
};

// LicenseModule类
class LicenseModule {
 public:
  LicenseModule(const std::string& customerInfo, int validityPeriod) {
    licenseKey_ =
        LicenseGenerator::generateLicenseKey(customerInfo, validityPeriod);
    customerInfo_ = customerInfo;
    expiryDate_ = std::time(nullptr) +
                  validityPeriod * 24 * 60 * 60;  // 假设validityPeriod为天数
  }

  bool isLicenseValid() {
    return std::time(nullptr) < expiryDate_ &&
           LicenseValidator::validateLicenseKey(licenseKey_);
  }

  bool generateLicense(const std::string& customerInfo, int validityPeriod) {
    try {
      licenseKey_ =
          LicenseGenerator::generateLicenseKey(customerInfo, validityPeriod);
      customerInfo_ = customerInfo;
      expiryDate_ = std::time(nullptr) + validityPeriod * 24 * 60 * 60;
      return true;
    } catch (const std::exception& e) {
      throw LicenseException("Failed to generate license: " +
                             std::string(e.what()));
    }
  }

  void validateLicense(const std::string& licenseKey) {
    if (!LicenseValidator::validateLicenseKey(licenseKey)) {
      throw LicenseException("Invalid license key");
    }
  }

 private:
  std::string licenseKey_;
  std::string customerInfo_;
  std::time_t expiryDate_;
};

int main() {
  try {
    // 创建LicenseModule实例
    std::string customerInfo = "John Doe, Acme Inc.";
    int validityPeriod = 30;  // 30天有效期
    LicenseModule licenseModule(customerInfo, validityPeriod);

    // 测试isLicenseValid函数
    std::cout << "License valid: " << std::boolalpha
              << licenseModule.isLicenseValid() << std::endl;

    // 模拟许可证过期
    std::tm expiredTime = {};
    expiredTime.tm_year = 2023 - 1900;  // 年份从1900开始计算
    expiredTime.tm_mon = 11;            // 月份从0开始
    expiredTime.tm_mday = 31;
    std::time_t expiredTimestamp = std::mktime(&expiredTime);
    std::cout << "Simulating license expiration..." << std::endl;
    std::this_thread::sleep_until(
        std::chrono::system_clock::from_time_t(expiredTimestamp));

    // 测试isLicenseValid函数
    std::cout << "License valid: " << std::boolalpha
              << licenseModule.isLicenseValid() << std::endl;

    // 生成新的许可证
    std::string newCustomerInfo = "Jane Smith, Umbrella Corp.";
    int newValidityPeriod = 60;  // 60天有效期
    licenseModule.generateLicense(newCustomerInfo, newValidityPeriod);

    // 测试isLicenseValid函数
    std::cout << "License valid: " << std::boolalpha
              << licenseModule.isLicenseValid() << std::endl;

    // 测试validateLicense函数
    std::string invalidLicenseKey = "invalid_license_key";
    try {
      licenseModule.validateLicense(invalidLicenseKey);
    } catch (const LicenseException& e) {
      std::cout << "Caught LicenseException: " << e.what() << std::endl;
    }
  } catch (const std::exception& e) {
    std::cout << "Caught exception: " << e.what() << std::endl;
  }

  return 0;
}