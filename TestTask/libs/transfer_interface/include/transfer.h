//
// Created by misha on 17.09.2025.
//

#ifndef RFI_BASE_ON_LIBHACKRF_TEST_ISDR_H
#define RFI_BASE_ON_LIBHACKRF_TEST_ISDR_H

#ifndef RFI_BASE_ON_LIBHACKRF_ITRANSFERCONTROL_H
#define RFI_BASE_ON_LIBHACKRF_ITRANSFERCONTROL_H

#include <functional>
#include <stdint.h>
struct TransferParams {
  enum class Type { single, loop };

  TransferParams() = default;
  TransferParams(size_t id, Type type, size_t buffSize, size_t chunkSize)
      : id(id), type(type), bufferSize(buffSize), packageSize(chunkSize) {}
  size_t id;
  Type type;
  std::size_t bufferSize;
  std::size_t packageSize;
  std::size_t packageCount = 0;
};

inline bool operator==(const TransferParams &left,
                       const TransferParams &right) {
  return std::tie(left.bufferSize, left.id, left.type) ==
         std::tie(right.bufferSize, right.id, right.type);
};

class ITransferControl {
public:
  using Handler = std::function<void(void *ptr, std::size_t sz)>;
  explicit ITransferControl(const TransferParams &params) {};
  virtual ~ITransferControl() = default;
  /**
   * @brief setHandler
   * @param h обработчик прерывания, вызываемый при получении прерывания от
   * источника (например, драйвера)
   */
  virtual void setHandler(Handler handler) = 0;

  /**
   * @brief setPacketCount
   * @param packetCount количество пакетов в пакетном режиме работы
   */
  virtual void setPacketCount(std::size_t packetCount) = 0;

  /**
   * @brief getPacketSize
   * @param packetSize размер пакета в прерывании
   */
  virtual void setPacketSize(std::size_t packetSize) = 0;

  /**
   * @brief getPacketSize
   * @return
   */
  virtual std::size_t getPacketSize() const = 0;

  /**
   * @brief setType тип приема -- пакетный или в цикле
   * @param t
   */
  virtual void setType(TransferParams::Type t) = 0;
  /**
   * @brief initalize инициализирует динамические с-ры данных (типа буфера)
   */
  virtual void initialize() = 0;

  /**
   * @brief finalize освобождает память из-под динамических с-р (типа буфера)
   */
  virtual void finalize() = 0;
};

class ISDRStreamTransfer : public ITransferControl {
public:
  explicit ISDRStreamTransfer(const TransferParams &params)
      : ITransferControl(params) {}
  virtual void start() = 0;
  /**
   * @brief startCounter получить и обработать кол-во пакетов,
   * переданное методом setPacketCount
   */
  virtual void startCounter() = 0;
  virtual void stop() = 0;
};

class SdrStreamControlWrapper {
  SdrStreamControlWrapper(std::function<ISDRStreamTransfer &(uint32_t)> &&,
                          uint32_t mask);
  ~SdrStreamControlWrapper();
  void startByMask(uint32_t mask);
  void stopByMask(uint32_t mask);
  void startCntrByMask(uint32_t mask);
  void resetByMask(uint32_t mask);
  void unresetByMask(uint32_t mask);
  uint32_t getState(size_t) const;

  std::function<ISDRStreamTransfer &(size_t)> getter;
  uint32_t max_dev;
  uint32_t mask = 0;
};
#endif // RFI_BASE_ON_LIBHACKRF_ITRANSFERCONTROL_H
#endif // RFI_BASE_ON_LIBHACKRF_TEST_ISDR_H
