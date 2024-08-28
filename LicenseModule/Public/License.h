#pragma once

#include <nlohmann/json.hpp>
#include <string>

// Enum representing the role type of a user.
enum class RoleType {
  TRIAL,     // Trial User
  PAID,      // Paid User
  PERMANENT  // Permanent User
};

// Represents a license with details about authorization, validity, machine,
// role, and company. Provides methods for serialization and deserialization
// into JSON format.
class License {
 public:
  // Constructs a License with specified details.
  // @param authorizationTime The time when the license was authorized.
  // @param validTime The expiration time of the license.
  //      Empty if it's a permanent license.
  // @param machineId The unique identifier of the machine.
  // @param roleType The type of role assigned.
  // @param companyName The name of the company.
  License(const std::string& authorizationTime, const std::string& validTime,
          const std::string& machineId, RoleType roleType,
          const std::string& companyName)
      : authorizationTime(authorizationTime),
        validTime(validTime),
        machineId(machineId),
        roleType(roleType),
        companyName(companyName) {}

  // Serializes the License object to a JSON string.
  // @return A string representing the license in JSON format.
  std::string serialize() const {
    nlohmann::json j;
    j["authorizationTime"] = authorizationTime;
    j["validTime"] = validTime;
    j["machineId"] = machineId;
    j["roleType"] = static_cast<int>(roleType);
    j["companyName"] = companyName;
    return j.dump();
  }

  // Deserializes a JSON string into a License object.
  // @param data A JSON string representing the license.
  // @return A License object parsed from the JSON string.
  static License deserialize(const std::string& data) {
    nlohmann::json j = nlohmann::json::parse(data);
    return License(j["authorizationTime"], j["validTime"], j["machineId"],
                   static_cast<RoleType>(j["roleType"]), j["companyName"]);
  }

  // Accessors to get license details.
  const std::string& getAuthorizationTime() const { return authorizationTime; }
  const std::string& getValidTime() const { return validTime; }
  const std::string& getMachineId() const { return machineId; }
  RoleType getRoleType() const { return roleType; }
  const std::string& getCompanyName() const { return companyName; }

 private:
  std::string authorizationTime;  // The time when the license was authorized.
  std::string validTime;          // The time until the license remains valid.
                                  // Empty if it's a permanent license.
  std::string machineId;    // The unique identifier for the target machine.
  RoleType roleType;        // The role type associated with the license.
  std::string companyName;  // The name of the company holding the license.
};