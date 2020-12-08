// Copyright 2020 Intel Corporation
#include <vector>

#include "pmlc/rt/level_zero/register.h"
#include "pmlc/rt/runtime.h"
#include "pmlc/rt/runtime_registry.h"
#include "pmlc/util/logging.h"

#include "pmlc/rt/level_zero/level_zero_device.h"

namespace pmlc::rt::level_zero {

class LevelZeroRuntime final : public pmlc::rt::Runtime {
public:
  LevelZeroRuntime() {
    std::vector<ze_device_handle_t> supportedDevices =
        pmlc::rt::level_zero::getSupportedDevices();
    for (ze_device_handle_t &device : supportedDevices)
      devices.emplace_back(std::make_shared<LevelZeroDevice>(device));
  }

  ~LevelZeroRuntime() {}

  std::size_t deviceCount() const noexcept final { return devices.size(); }
  std::shared_ptr<pmlc::rt::Device> device(std::size_t idx) override {
    return devices.at(idx);
  }

private:
  std::vector<std::shared_ptr<LevelZeroDevice>> devices;
};

extern void registerSymbols();

void registerRuntime() {
  try {
    registerRuntime("level_zero", std::make_shared<LevelZeroRuntime>());
    registerSymbols();
  } catch (std::exception &ex) {
    IVLOG(2, "Failed to register 'level_zero' runtime: " << ex.what());
  }
}

} // namespace pmlc::rt::level_zero