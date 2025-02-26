#pragma once
#include "HybridTorSpec.hpp"
#include "tor_ffi.h"
#include <cstring> // For std::memcpy
#include <memory>

namespace margelo::nitro::nitrotor {
  class HybridTor : public HybridTorSpec {
  public:
    HybridTor() : HybridObject(TAG) {}

    std::shared_ptr<Promise<bool>> initTorService(const TorConfig &config) override {
      return Promise<bool>::async([config]() {
        // First check if library is initialized
        if (!tor::initialize_tor_library()) {
          return false; // Failed to initialize library
        }
        // Then proceed with service initialization
        return tor::init_tor_service(static_cast<uint16_t>(config.socks_port),
                                     config.data_dir.c_str(),
                                     static_cast<uint64_t>(config.timeout_ms));
      });
    }

    std::shared_ptr<Promise<HiddenServiceResponse>>
    createHiddenService(const HiddenServiceParams &params) override {
      return Promise<HiddenServiceResponse>::async([params]() {
        // Create a C array of bytes from the vector
        const uint8_t *key_data_ptr = nullptr;
        std::array<uint8_t, 64> key_data{};
        bool has_key = params.key_data.has_value();

        if (has_key) {
          const auto &key_vec = params.key_data.value();
          for (size_t i = 0; i < 64 && i < key_vec.size(); i++) {
            key_data[i] = static_cast<uint8_t>(key_vec[i]);
          }
          key_data_ptr = key_data.data();
        }

        // Call the FFI function
        auto result = tor::create_hidden_service(static_cast<uint16_t>(params.port),
                                                 static_cast<uint16_t>(params.target_port),
                                                 key_data_ptr, has_key);

        // Create our response object and copy the strings
        std::string onion_address = result.onion_address ? result.onion_address : "";
        std::string control = result.control ? result.control : "";

        // Important: Free the C strings to avoid memory leaks
        if (result.onion_address)
          tor::free_string(result.onion_address);
        if (result.control)
          tor::free_string(result.control);

        return HiddenServiceResponse(result.is_success, onion_address, control);
      });
    }

    std::shared_ptr<Promise<StartTorResponse>>
    startTorIfNotRunning(const StartTorParams &params) override {
      return Promise<StartTorResponse>::async([params]() {
        // Create a C array of bytes from the vector
        const uint8_t *key_data_ptr = nullptr;
        std::array<uint8_t, 64> key_data{};
        bool has_key = params.key_data.has_value();

        if (has_key) {
          const auto &key_vec = params.key_data.value();
          for (size_t i = 0; i < 64 && i < key_vec.size(); i++) {
            key_data[i] = static_cast<uint8_t>(key_vec[i]);
          }
          key_data_ptr = key_data.data();
        }

        // Call the Rust function
        auto result = tor::start_tor_if_not_running(params.data_dir.c_str(), key_data_ptr, has_key,
                                                    static_cast<uint16_t>(params.socks_port),
                                                    static_cast<uint16_t>(params.target_port),
                                                    static_cast<uint64_t>(params.timeout_ms));

        // Create our response object and copy the strings
        std::string onion_address = result.onion_address ? result.onion_address : "";
        std::string control = result.control ? result.control : "";
        std::string error_message = result.error_message ? result.error_message : "";

        // Important: Free the C strings to avoid memory leaks
        if (result.onion_address)
          tor::free_string(result.onion_address);
        if (result.control)
          tor::free_string(result.control);
        if (result.error_message)
          tor::free_string(result.error_message);

        return StartTorResponse(result.is_success, onion_address, control, error_message);
      });
    }

    std::shared_ptr<Promise<double>> getServiceStatus() override {
      return Promise<double>::async([]() {
        // Convert int32_t to double since the spec requires double
        return static_cast<double>(tor::get_service_status());
      });
    }

    std::shared_ptr<Promise<bool>> deleteHiddenService(const std::string &onionAddress) override {
      return Promise<bool>::async(
          [onionAddress]() { return tor::delete_hidden_service(onionAddress.c_str()); });
    }

    std::shared_ptr<Promise<bool>> shutdownService() override {
      return Promise<bool>::async([]() { return tor::shutdown_service(); });
    }
  };
} // namespace margelo::nitro::nitrotor