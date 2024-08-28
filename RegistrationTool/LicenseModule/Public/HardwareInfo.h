#ifndef D005BA4A_1794_46A2_B3EF_6171233A01EB
#define D005BA4A_1794_46A2_B3EF_6171233A01EB
#ifndef CD1D3D85_B993_4D94_805A_C2ACE9F17F62
#define CD1D3D85_B993_4D94_805A_C2ACE9F17F62

#include <string>

namespace LicenseModule {
class HardwareInfo {
 public:
  static std::string GetMachineCode();

 private:
  static std::string myMachineCode;
};
}  // namespace LicenseModule

#endif /* CD1D3D85_B993_4D94_805A_C2ACE9F17F62 */


#endif /* D005BA4A_1794_46A2_B3EF_6171233A01EB */
