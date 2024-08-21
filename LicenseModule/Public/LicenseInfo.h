#ifndef A778032C_56E3_483C_A0B6_510E95FC4D13
#define A778032C_56E3_483C_A0B6_510E95FC4D13

#include <string>

namespace LicenseModule {

enum class RoleType { Trial = 0, Expiration, Free };

class LicenseInfo {
 public:
  const std::string GetCustomMachineCode() const;

  void SetCustomMachineCode(std::string _customMachineCode);

  double GetLastUseTime();

  void SetLastUseTime(double _lastUseTime);

  double GetExpireTime();

  void SetExpireTime(double _expireTime);

  RoleType GetRoleType();

  void SetRoleType(RoleType _customRole);

 private:
  const std::string customMachineCode;

  double lastUseTime;

  double expireTime;

  RoleType customRole;
};
}  // namespace LicenseModule

#endif /* A778032C_56E3_483C_A0B6_510E95FC4D13 */
