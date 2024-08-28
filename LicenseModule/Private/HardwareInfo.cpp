#include "HardwareInfo.h"

#include <iostream>
#include <array>
#include <memory>
#include <fstream>

std::string LicenseModule::HardwareInfo::GetMachineCode() {
  std::string MachineCode;

  char buffer[128];
  FILE* pipe = popen("dmidecode -s system-uuid", "r");
  if (pipe) {
    while (fgets(buffer, sizeof(buffer), pipe) != nullptr) {
      MachineCode += buffer;
    }
    pclose(pipe);
  }
  // Remove trailing newline character
  if (!MachineCode.empty() && MachineCode.back() == '\n') {
    MachineCode.pop_back();
  }
  return MachineCode;
}