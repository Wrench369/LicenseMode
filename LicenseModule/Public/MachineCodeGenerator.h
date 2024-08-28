#pragma once

#include <array>
#include <cstdlib>
#include <fstream>
#include <memory>
#include <sstream>
#include <string>

// Generates a unique machine code based on system hardware information.
// This class is designed to run on different architectures of Linux
// environments.
class MachineCodeGenerator {
 public:
  // Generates a machine code using a combination of system information.
  // @return A string representing the unique machine code.
  std::string generateMachineCode() {
    std::string cpuInfo = getCPUInfo();
    std::string macAddress = getMacAddress();

    // Combine the extracted information to form a unique machine code.
    return hashCode(cpuInfo + macAddress);
  }

 private:
  // Retrieves CPU information from /proc/cpuinfo file.
  // @return A string containing the CPU model name.
  std::string getCPUInfo() {
    std::ifstream cpuInfoFile("/proc/cpuinfo");
    std::string line;
    if (cpuInfoFile.is_open()) {
      while (std::getline(cpuInfoFile, line)) {
        if (line.find("model name") != std::string::npos) {
          return line.substr(line.find(':') + 2);
        }
      }
    }
    return "unknown";
  }

  // Retrieves the MAC address of the network adapter.
  // Uses the "ip" command to query network interface details.
  // @return A string representing the MAC address if found; otherwise,
  // "unknown".
  std::string getMacAddress() {
    std::array<char, 128> buffer;
    std::string result;
    std::unique_ptr<FILE, decltype(&pclose)> pipe(
        popen("ip link show | grep 'link/ether' | awk '{print $2}'", "r"),
        pclose);
    if (!pipe) {
      return "unknown";
    }
    while (fgets(buffer.data(), buffer.size(), pipe.get()) != nullptr) {
      result += buffer.data();
    }
    return result.empty() ? "unknown" : result.substr(0, result.find('\n'));
  }

  // Generates a simple hash code from a string.
  // @param str A string to hash.
  // @return A string representing the hash code.
  std::string hashCode(const std::string& str) {
    std::hash<std::string> hashFunc;
    return std::to_string(hashFunc(str));
  }
};