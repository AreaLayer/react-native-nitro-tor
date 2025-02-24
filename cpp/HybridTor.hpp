#pragma once
#include "HybridTorSpec.hpp"
#include "lib.rs.h"
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
                                     rust::Str(config.data_dir),
                                     static_cast<uint64_t>(config.timeout_ms));
      });
    }

    std::shared_ptr<Promise<HiddenServiceResponse>>
    createHiddenService(const HiddenServiceParams &params) override {
      return Promise<HiddenServiceResponse>::async([params]() {
        std::array<uint8_t, 64> key_data{};
        bool has_key = params.key_data.has_value();
        if (has_key) {
          const auto &key_vec = params.key_data.value();
          for (size_t i = 0; i < 64 && i < key_vec.size(); i++) {
            key_data[i] = static_cast<uint8_t>(key_vec[i]);
          }
        }
        auto result = tor::create_hidden_service(static_cast<uint16_t>(params.port),
                                                 static_cast<uint16_t>(params.target_port),
                                                 key_data, has_key);

        return HiddenServiceResponse(result.is_success, std::string(result.onion_address.c_str()),
                                     std::string(result.control.c_str()));
      });
    }

    std::shared_ptr<Promise<StartTorResponse>>
    startTorIfNotRunning(const StartTorParams &params) override {
      return Promise<StartTorResponse>::async([params]() {
        std::array<uint8_t, 64> key_data{};
        bool has_key = params.key_data.has_value();
        if (has_key) {
          const auto &key_vec = params.key_data.value();
          for (size_t i = 0; i < 64 && i < key_vec.size(); i++) {
            key_data[i] = static_cast<uint8_t>(key_vec[i]);
          }
        }

        // Simply call the Rust function that handles the complete logic
        auto result = tor::start_tor_if_not_running(
            rust::Str(params.data_dir), key_data, has_key, static_cast<uint16_t>(params.socks_port),
            static_cast<uint16_t>(params.target_port), static_cast<uint64_t>(params.timeout_ms));

        return StartTorResponse(result.is_success, std::string(result.onion_address.c_str()),
                                std::string(result.control.c_str()),
                                std::string(result.error_message.c_str()));
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
          [onionAddress]() { return tor::delete_hidden_service(rust::Str(onionAddress)); });
    }

    std::shared_ptr<Promise<bool>> shutdownService() override {
      return Promise<bool>::async([]() { return tor::shutdown_service(); });
    }
  };
} // namespace margelo::nitro::nitrotor