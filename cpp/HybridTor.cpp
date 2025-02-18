#include "HybridTor.hpp"
#include <cstdint> // For intptr_t
#include <jsi/jsi.h>
#include <optional> // Include for std::optional
#include <sstream>
#include <vector>

#include "log.h"

namespace margelo::nitro::nitrotor {

  HybridTor::HybridTor() // **Corrected Constructor: Call HybridObject(TAG) directly**
      : HybridObject(TAG) {}

  HybridTor::~HybridTor() {
    // Resource cleanup in destructor
    if (rustOwnedTorServicePtr != nullptr) {
      free_owned_tor_service(rustOwnedTorServicePtr);
      rustOwnedTorServicePtr = nullptr;
    }
    if (rustTorServicePtr != nullptr) {
      free_tor_service(rustTorServicePtr);
      rustTorServicePtr = nullptr;
    }
    if (rustTorHiddenServicePtr != nullptr) {
      free_tor_hidden_service(rustTorHiddenServicePtr);
      rustTorHiddenServicePtr = nullptr;
    }
  }

  // ---------------- TorService Functions ------------------

  double
  HybridTor::createTorService(const std::string &dataDir, double socksPort,
                              double bootstrapTimeoutMs) { // Signature matches HybridTorSpec.hpp
    NITROTOR_LOG_INFO("createTorService");

    uint16_t socksPort_u16 = static_cast<uint16_t>(socksPort);
    uint64_t bootstrapTimeoutMs_u64 = static_cast<uint64_t>(bootstrapTimeoutMs);

    const char *dataDirCStr = dataDir.c_str();
    rustTorServicePtr = create_tor_service(dataDirCStr, socksPort_u16, bootstrapTimeoutMs_u64);

    if (rustTorServicePtr == nullptr) {
      return 0.0;
    }
    return static_cast<double>(reinterpret_cast<intptr_t>(rustTorServicePtr));
  }

  double HybridTor::intoOwnedTorService(double servicePtr) { // Signature matches HybridTorSpec.hpp
    TorService *servicePtrRaw = reinterpret_cast<TorService *>(static_cast<intptr_t>(servicePtr));
    rustOwnedTorServicePtr = into_owned_tor_service(servicePtrRaw);
    rustTorServicePtr = nullptr;

    if (rustOwnedTorServicePtr == nullptr) {
      return 0.0;
    }
    return static_cast<double>(reinterpret_cast<intptr_t>(rustOwnedTorServicePtr));
  }

  void HybridTor::freeTorServiceInstance(double servicePtr) { // Signature matches HybridTorSpec.hpp
    TorService *servicePtrRaw = reinterpret_cast<TorService *>(static_cast<intptr_t>(servicePtr));
    if (servicePtrRaw != nullptr) {
      free_tor_service(servicePtrRaw);
      if (rustTorServicePtr == servicePtrRaw) {
        rustTorServicePtr = nullptr;
      }
    }
  }

  // ---------------- OwnedTorService Functions ------------------

  std::string
  HybridTor::getTorStatus(double ownedServicePtr) { // Signature matches HybridTorSpec.hpp
    OwnedTorService *ownedServicePtrRaw =
        reinterpret_cast<OwnedTorService *>(static_cast<intptr_t>(ownedServicePtr));
    if (ownedServicePtrRaw == nullptr) {
      return "";
    }

    char *statusCStr = get_tor_status(ownedServicePtrRaw);
    if (statusCStr == nullptr) {
      return "";
    }

    std::string statusStdString(statusCStr);
    free_cstring(statusCStr);
    return statusStdString;
  }

  double HybridTor::createHiddenService(double ownedServicePtr, double toPort, double hsPort,
                                        const std::optional<std::vector<double>>
                                            &secretKey) { // **EXACT MATCH with HybridTorSpec.hpp**
    OwnedTorService *ownedServicePtrRaw =
        reinterpret_cast<OwnedTorService *>(static_cast<intptr_t>(ownedServicePtr));
    if (ownedServicePtrRaw == nullptr) {
      return 0.0;
    }

    uint16_t toPort_u16 = static_cast<uint16_t>(toPort);
    uint16_t hsPort_u16 = static_cast<uint16_t>(hsPort);
    std::vector<uint8_t> secretKeyBytes;

    if (secretKey.has_value()) {
      const auto &secretKeyDoubleVec = secretKey.value();
      secretKeyBytes.reserve(secretKeyDoubleVec.size());
      for (double val : secretKeyDoubleVec) {
        if (val >= 0 && val <= 255 && val == static_cast<uint8_t>(val)) {
          secretKeyBytes.push_back(static_cast<uint8_t>(val));
        } else {
          return 0.0;
        }
      }
      if (secretKeyBytes.size() != 64) {
        return 0.0;
      }
    }

    TorHiddenService *hiddenServicePtr = nullptr;
    if (secretKeyBytes.empty()) {
      hiddenServicePtr = create_hidden_service(ownedServicePtrRaw, toPort_u16, hsPort_u16, nullptr);
    } else {
      hiddenServicePtr =
          create_hidden_service(ownedServicePtrRaw, toPort_u16, hsPort_u16, secretKeyBytes.data());
    }

    if (hiddenServicePtr == nullptr) {
      return 0.0;
    }
    rustTorHiddenServicePtr = hiddenServicePtr;
    return static_cast<double>(reinterpret_cast<intptr_t>(hiddenServicePtr));
  }

  std::string
  HybridTor::deleteHiddenService(double ownedServicePtr,
                                 const std::string &onion) { // Signature matches HybridTorSpec.hpp
    OwnedTorService *ownedServicePtrRaw =
        reinterpret_cast<OwnedTorService *>(static_cast<intptr_t>(ownedServicePtr));
    if (ownedServicePtrRaw == nullptr) {
      return "";
    }
    const char *onionCStr = onion.c_str();

    char *resultCStr = delete_hidden_service(ownedServicePtrRaw, onionCStr);
    if (resultCStr == nullptr) {
      return "";
    }

    std::string resultStdString(resultCStr);
    free_cstring(resultCStr);
    return resultStdString;
  }

  void HybridTor::freeOwnedTorServiceInstance(
      double ownedServicePtr) { // Signature matches HybridTorSpec.hpp
    OwnedTorService *ownedServicePtrRaw =
        reinterpret_cast<OwnedTorService *>(static_cast<intptr_t>(ownedServicePtr));
    if (ownedServicePtrRaw != nullptr) {
      free_owned_tor_service(ownedServicePtrRaw);
      if (rustOwnedTorServicePtr == ownedServicePtrRaw) {
        rustOwnedTorServicePtr = nullptr;
      }
    }
  }

  // ---------------- TorHiddenService Functions ------------------

  std::string HybridTor::getHiddenServiceOnionUrl(
      double hiddenServicePtr) { // Signature matches HybridTorSpec.hpp
    TorHiddenService *hiddenServicePtrRaw =
        reinterpret_cast<TorHiddenService *>(static_cast<intptr_t>(hiddenServicePtr));
    if (hiddenServicePtrRaw == nullptr) {
      return "";
    }

    char *onionUrlCStr = get_hidden_service_onion_url(hiddenServicePtrRaw);
    if (onionUrlCStr == nullptr) {
      return "";
    }

    std::string onionUrlStdString(onionUrlCStr);
    free_cstring(onionUrlCStr);
    return onionUrlStdString;
  }

  std::string HybridTor::getHiddenServiceSecretKey(
      double hiddenServicePtr) { // Signature matches HybridTorSpec.hpp
    TorHiddenService *hiddenServicePtrRaw =
        reinterpret_cast<TorHiddenService *>(static_cast<intptr_t>(hiddenServicePtr));
    if (hiddenServicePtrRaw == nullptr) {
      return "";
    }

    char *secretKeyCStr = get_hidden_service_secret_key(hiddenServicePtrRaw);
    if (secretKeyCStr == nullptr) {
      return "";
    }

    std::string secretKeyStdString(secretKeyCStr);
    free_cstring(secretKeyCStr);
    return secretKeyStdString;
  }

  void HybridTor::freeTorHiddenServiceInstance(
      double hiddenServicePtr) { // Signature matches HybridTorSpec.hpp
    TorHiddenService *hiddenServicePtrRaw =
        reinterpret_cast<TorHiddenService *>(static_cast<intptr_t>(hiddenServicePtr));
    if (hiddenServicePtrRaw != nullptr) {
      free_tor_hidden_service(hiddenServicePtrRaw);
      if (rustTorHiddenServicePtr == hiddenServicePtrRaw) {
        rustTorHiddenServicePtr = nullptr;
      }
    }
  }

  // ---------------- Helper Functions (These are now only used internally within HybridTor.cpp, not
  // in spec methods) ------------------

  std::string HybridTor::jsiValueToString(jsi::Runtime &runtime, const jsi::Value &value) {
    if (value.isNull() || value.isUndefined()) {
      return "";
    }
    if (value.isString()) {
      return value.getString(runtime).utf8(runtime);
    }
    return "";
  }

  double HybridTor::jsiValueToDouble(jsi::Runtime &runtime, const jsi::Value &value) {
    if (value.isNull() || value.isUndefined()) {
      return 0.0;
    }
    if (value.isNumber()) {
      return value.getNumber();
    }
    return 0.0;
  }

  std::vector<uint8_t> HybridTor::jsiValueToUint8Vector(jsi::Runtime &runtime,
                                                        const jsi::Value &value) {
    std::vector<uint8_t> byteArray;
    if (value.isNull() || value.isUndefined()) {
      return byteArray;
    }
    if (value.isObject()) {
      jsi::Object object = value.getObject(runtime);
      if (object.isArray(runtime)) {
        jsi::Array array = object.getArray(runtime);
        size_t length = array.length(runtime);
        byteArray.reserve(length);
        for (size_t i = 0; i < length; ++i) {
          jsi::Value elementValue = array.getValueAtIndex(runtime, i);
          if (elementValue.isNumber()) {
            double num = elementValue.getNumber();
            if (num >= 0 && num <= 255 && num == static_cast<uint8_t>(num)) {
              byteArray.push_back(static_cast<uint8_t>(num));
            } else {
              return std::vector<uint8_t>();
            }
          } else {
            return std::vector<uint8_t>();
          }
        }
      }
    }
    return byteArray;
  }

  jsi::Value HybridTor::stringToJSIValue(jsi::Runtime &runtime, const std::string &str) {
    return jsi::String::createFromUtf8(runtime, str);
  }

} // namespace margelo::nitro::nitrotor