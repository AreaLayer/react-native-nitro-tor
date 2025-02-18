#pragma once

#include "HybridTorSpec.hpp" // Assuming this will be auto-generated
#include <ReactCommon/CallInvoker.h>
#include <ReactCommon/TurboModule.h>
#include <jni.h>
#include <jsi/jsi.h>
#include <memory>
#include <string>

// Include the generated Rust header file (adjust path if needed)
#include "sifir_tor.h"

namespace margelo::nitro::nitrotor {
  class HybridTor : public HybridTorSpec {
  public:
    HybridTor(); // Default constructor
    ~HybridTor() override;

    // Tor Service Functions (Corrected signatures based on likely HybridTorSpec)
    double createTorService(const std::string &dataDir, double socksPort,
                            double bootstrapTimeoutMs) override; // <--- Corrected
    double intoOwnedTorService(double servicePtr) override;      // <--- Corrected
    void freeTorServiceInstance(double servicePtr) override;     // <--- Corrected

    // Owned Tor Service Functions (Corrected Signatures - likely also need correction)
    std::string getTorStatus(double ownedServicePtr) override; // <--- Corrected
    double createHiddenService(
        double ownedServicePtr, double toPort, double hsPort,
        const std::optional<std::vector<double>> &secretKey) override; // <--- Corrected
    std::string deleteHiddenService(double ownedServicePtr,
                                    const std::string &onion) override; // <--- Corrected
    void freeOwnedTorServiceInstance(double ownedServicePtr) override;  // <--- Corrected

    // Tor Hidden Service Functions (Corrected Signatures - likely also need correction)
    std::string getHiddenServiceOnionUrl(double hiddenServicePtr) override;  // <--- Corrected
    std::string getHiddenServiceSecretKey(double hiddenServicePtr) override; // <--- Corrected
    void freeTorHiddenServiceInstance(double hiddenServicePtr) override;     // <--- Corrected

  private:
    TorService *rustTorServicePtr = nullptr;             // Store Rust TorService pointer
    OwnedTorService *rustOwnedTorServicePtr = nullptr;   // Store Rust OwnedTorService pointer
    TorHiddenService *rustTorHiddenServicePtr = nullptr; // Store Rust TorHiddenService pointer

    // Helper functions (consider moving to a separate utility header if needed)
    std::string jsiValueToString(jsi::Runtime &runtime, const jsi::Value &value);
    double jsiValueToDouble(jsi::Runtime &runtime, const jsi::Value &value);
    std::vector<uint8_t>
    jsiValueToUint8Vector(jsi::Runtime &runtime,
                          const jsi::Value &value); // For secret key if you pass as array
    jsi::Value stringToJSIValue(jsi::Runtime &runtime,
                                const std::string &str); // Convert std::string to jsi::Value string
  };

} // namespace margelo::nitro::nitrotor