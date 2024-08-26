#include "LicenseInfo.h"

const std::string LicenseModule::LicenseInfo::GetCustomMachineCode() const {
  return customMachineCode;
}

void LicenseModule::LicenseInfo::SetCustomMachineCode(
    std::string _customMachineCode) {
  customMachineCode = _customMachineCode;
}

double LicenseModule::LicenseInfo::GetLastUseTime() { return lastUseTime; }

void LicenseModule::LicenseInfo::SetLastUseTime(double _lastUseTime) {
  lastUseTime = _lastUseTime;
}

double LicenseModule::LicenseInfo::GetExpireTime() { return expireTime; }

void LicenseModule::LicenseInfo::SetExpireTime(double _expireTime) {
  expireTime = _expireTime;
}

LicenseModule::RoleType LicenseModule::LicenseInfo::GetRoleType() {
  return customRole;
}

void LicenseModule::LicenseInfo::SetRoleType(RoleType _customRole) {
  customRole = _customRole;
}
