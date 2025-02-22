#pragma once
#include "HybridTorSpec.hpp"
#include "lib.rs.h"
#include <memory>

namespace margelo::nitro::nitrotor {

  class HybridTor : public HybridTorSpec {
  public:
    HybridTor() : HybridTorSpec() {}

    std::shared_ptr<Promise<bool>> initTorService(const TorConfig &config) override {
      return Promise<bool>::async([config]() {
        return tor::init_tor_service(static_cast<uint16_t>(config.socks_port),
                                     rust::Str(config.data_dir),
                                     static_cast<uint64_t>(config.timeout_ms));
      });
    }

    std::shared_ptr<Promise<std::string>>
    createHiddenService(const HiddenServiceParams &params) override {
      return Promise<std::string>::async([params]() {
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

        return std::string(result.c_str());
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