# LicenseModule 

LicenseModule is a C++ library designed to manage software licensing, including the creation, encryption, decryption, and validation of licenses. It uses RSA encryption to secure license information. 

## Features

- **Create Licenses**: Supports generating licenses for different roles, such as Trial, Paid, and Permanent.
- **Encrypt and Decrypt Licenses**: Uses RSA encryption to protect license data.
- **Validate Licenses**: Verifies the validity of a license based on machine ID and expiration date.

## Requirements

- C++14 or later
- Google Test (for running unit tests)

## Installation

1. **Clone the Repository**

   ```bash
   git clone https://xxx.git
   cd LicenseModule
   ```

2. **Build the Project**
   Ensure you have a C++ compiler installed (e.g., GCC or Clang) along with CMake for building the project. Then, use the following commands to build:

   ```bash
   mkdir build
   cd build
   cmake ..
   make
   ```

3. **Run Tests**

   To run tests using Google Test, ensure it is installed, then run:

   ```bash
   ./build/test/LicenseManager_test
   ./build/test/License_test
   ./build/test/RSACryto_test
   ```

## Usage

1. **Include the Header**

   In your C++ project, include the LicenseManager header.

   ```cpp
   #include "LicenseManager.h"
   ```

2. **Initialize LicenseManager**

   Create an instance of `LicenseManager` with paths to your RSA public and private key files.

   ```cpp
   LicenseManager licenseManager("path/to/public.pem", "path/to/private.pem");
   ```

3. **Create a License**

   Generate a new license for a user.

   ```cpp
   License license = licenseManager.createLicense(RoleType::TRIAL, "CompanyA", "Machine123");
   ```

4. **Encrypt a License**

   Secure the license data using encryption.

   ```cpp
   std::string encryptedLicense = licenseManager.encryptLicense(license);
   ```

5. **Decrypt a License**

   Retrieve original license data from an encrypted string.

   ```cpp
   License decryptedLicense = licenseManager.decryptLicense(encryptedLicense);
   ```

6. **Validate a License**

   Check if a license is valid for a specific machine at a given time.

   ```cpp
   bool isValid = licenseManager.validateLicense(decryptedLicense, "Machine123", "2024-12-31");
   ```

## Configuration

- **RSA Keys**: Make sure to generate your RSA key pairs (`public.pem`, `private.pem`) for encryption and decryption. You can use tools like OpenSSL to generate these keys.
