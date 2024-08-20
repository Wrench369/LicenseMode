#ifndef CD1D3D85_B993_4D94_805A_C2ACE9F17F62
#define CD1D3D85_B993_4D94_805A_C2ACE9F17F62

#include <string>

namespace LicenseMode {
class HardwareInfo {
 public:
  static std::string GetMachineCode();

  static std::string MD5(std::string scr);

  static std::string GetCPUID();

  static std::string GetMOtherboardID();

  static std::string GetPhysicalMemoryID();

 private:
  static std::string myMachineCode;
};
}  // namespace LicenseMode

#endif /* CD1D3D85_B993_4D94_805A_C2ACE9F17F62 */
