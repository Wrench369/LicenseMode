#include "HardwareInfo.h"

#include <iostream>
#include <array>
#include <memory>
#include <fstream>

std::string LicenseModule::HardwareInfo::GetMachineCode() {
  std::string MachineCode;
  MachineCode = GetCPUID() + GetMacAddress() + GetMacAddress();
  return MachineCode;
}

std::string LicenseModule::HardwareInfo::GetCPUID() {
  std::string getCPUIDCmd = "sudo dmidecode -t 4 | grep ID";
  return execute(getCPUIDCmd);
}

std::string LicenseModule::HardwareInfo::GetMotherboardID() {
  std::string getMOtherboardIDCmd = "sudo dmidecode -t 2 | grep Serial";
  return execute(getMOtherboardIDCmd);
}

std::string LicenseModule::HardwareInfo::GetMacAddress(){
  std::string getMacAddressCmd = "sudo lshw -c network | grep serial | head -n 1";
  return execute(getMacAddressCmd);
}

std::string LicenseModule::HardwareInfo::execute(const std::string &command) {
  std::array<char, 128> buffer;
  std::string result;
  std::shared_ptr<FILE> pipe(popen(command.c_str(), "r"), pclose);
  if (!pipe) {
    throw std::runtime_error("popen() failed!");
  }
  while (!feof(pipe.get())) {
    if (fgets(buffer.data(), buffer.size(), pipe.get()) != nullptr) {
      result += buffer.data();
    }
  }
  return result;
}