#ifndef CD1D3D85_B993_4D94_805A_C2ACE9F17F62
#define CD1D3D85_B993_4D94_805A_C2ACE9F17F62

#include <string>

namespace LicenseModule {
class HardwareInfo {
 public:
  static std::string GetMachineCode();

  static std::string GetCPUID();

  static std::string GetMotherboardID();

  static std::string GetMacAddress();

 private:
  static std::string execute(const std::string &command);

  static std::string myMachineCode;
};
}  // namespace LicenseModule

#endif /* CD1D3D85_B993_4D94_805A_C2ACE9F17F62 */
