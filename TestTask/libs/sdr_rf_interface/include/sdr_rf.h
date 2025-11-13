//
// Created by misha on 17.09.2025.
//

#ifndef RFI_BASE_ON_LIBHACKRF_TEST_SDR_RF_H
#define RFI_BASE_ON_LIBHACKRF_TEST_SDR_RF_H

#include <cstdint>

class IDeviceRF {
public:
  virtual void setPower(uint32_t power) = 0;
  [[nodiscard]] virtual uint32_t getPower() const = 0;

  virtual void setFrequency(uint64_t frequencyHz) = 0;
  [[nodiscard]] virtual uint64_t getFrequency() const = 0;

  virtual void setAttenuator(uint32_t attDb) = 0;
  [[nodiscard]] virtual uint32_t getAttenuator() const = 0;

  virtual void setLevel(uint32_t levelDb) = 0;
  [[nodiscard]] virtual uint32_t getLevel() const = 0;

  virtual void setLna(uint32_t lna) = 0;
  [[nodiscard]] virtual uint32_t getLna() const = 0;

  ~IDeviceRF() = default;
};

class ISdrDevice {
public:
  virtual void *toContext() noexcept = 0;
  virtual ~ISdrDevice() = default;
  ISdrDevice() = default;
  ISdrDevice(const ISdrDevice &) = delete;
  ISdrDevice &operator=(const ISdrDevice &) = delete;
};
#endif // RFI_BASE_ON_LIBHACKRF_TEST_SDR_RF_H
