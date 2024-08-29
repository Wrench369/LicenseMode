# LicenseManager 

LicenseManager is a C++ library designed to manage software licensing, including the creation, encryption, decryption, and validation of licenses. It uses RSA encryption to secure license information. 

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
   git clone https://github.com/yourusername/LicenseManager.git
   cd LicenseManager
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
   ./test/LicenseManagerTest
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

## Contributing

We welcome contributions! Follow these steps to contribute:

1. Fork the repository.
2. Create a feature branch.
3. Commit your changes.
4. Push to the branch.
5. Create a Pull Request.

Please ensure that your contributions include tests where applicable.

## License

This project is licensed under the MIT License. See the `LICENSE` file for details.

## Contact

For any questions or suggestions, please feel free to contact the repository owner or open an issue on GitHub.

```
### Notes

- Feel free to customize any of the sections to match your project's specifics such as actual feature sets, repository URL, or licensing terms.
- Ensure the paths and instructions align with your project's structure and development environment.
```