//
// Created by misha on 05.10.2025.
//
#include "rtlsdr_control.h"
#include "../sdr_rf_interface/include/sdr_rf.h"
#include "../transfer_interface/include/transfer.h"
#include <cstddef>
#include <cstdint>
#include <iostream>
#include <rtl-sdr.h>
#include <stdexcept>
#include <string>

RtlsdrControl::RtlsdrControl(size_t dev_index)
    : ISDRStreamTransfer(TransferParams()) {
  rtlsdr_dev_t *dev = nullptr;
  int device_count = rtlsdr_get_device_count();

  check(rtlsdr_open(&device_, dev_index));

  numGains_ = rtlsdr_get_tuner_gains(device_, nullptr);
  gains_ = new uint64_t[numGains_];
}

RtlsdrControl::~RtlsdrControl() {
  delete gains_;
  rtlsdr_close(device_);
}
void RtlsdrControl::initialize() {
  if (!param_.bufferSize) {
    throw std::runtime_error("buffer must be > 0");
  }
  buf_ = new uint8_t[param_.bufferSize];
  readSz_ = new int;
}

void RtlsdrControl::finalize() {
  delete[] buf_;
  delete readSz_;
}

// transfer

void RtlsdrControl::recieveSingle(uint8_t *currentPos, size_t available) {
  int totalRead = 0;
  if (available < param_.packageSize) {
    int firstRead = 0;
    rtlsdr_reset_buffer(device_);
    rtlsdr_read_sync(device_, currentPos, available, readSz_);
    if (*readSz_ <= 0) {
      throw std::runtime_error("smth wrong with sdr, num samples read is" +
                               std::to_string(*readSz_));
    }
    hdl_(currentPos, *readSz_);
    firstRead = *readSz_;
    rtlsdr_read_sync(device_, buf_, param_.packageSize - available, readSz_);
    if (*readSz_ <= 0) {
      throw std::runtime_error("smth wrong with sdr, num samples read is" +
                               std::to_string(*readSz_));
    }
    hdl_(currentPos, *readSz_);
    *readSz_ += firstRead;
  } else {
    rtlsdr_reset_buffer(device_);
    rtlsdr_read_sync(device_, currentPos, param_.packageSize, readSz_);

    // TODO: Переписать на while
    // if(*readSz_ < param_.packageSize) {
    //   rtlsdr_read_sync(device_, buf_ + *readSz_, param_.packageSize -
    //   *readSz_, readSz_);
    // }
    if (*readSz_ <= 0) {
      throw std::runtime_error("smth wrong with sdr, num samples read is " +
                               std::to_string(*readSz_));
    }
    hdl_(currentPos, *readSz_);
  }
}

void RtlsdrControl::start() {
  if (!hdl_) {
    throw std::runtime_error("bad handler");
  }
  if (!buf_) {
    throw std::runtime_error("you must initialize obj  before recieve");
  }
  if (param_.type == TransferParams::Type::loop) {

    auto startInOtherThread = [this]() -> void {
      size_t posCounter = 0;
      isReceive.test_and_set();
      while (isReceive.test()) {
        recieveSingle(buf_ + posCounter, param_.bufferSize - posCounter);
        posCounter += param_.packageSize;
        posCounter = posCounter % param_.bufferSize;
        std::cout << "bytes read: " << std::to_string(*readSz_) << std::endl;
      }
    };

    rxThread = new std::thread(startInOtherThread);
  } else {
    auto startInOtherThreadSingle = [this]() -> void {
      // size_t posCounter = 0;
      isReceive.test_and_set();
      recieveSingle(buf_, param_.bufferSize);
      // posCounter += param_.packageSize;
      // posCounter %= param_.bufferSize;
      std::cout << "bytes read: " << std::to_string(*readSz_) << std::endl;
    };
    rxThread = new std::thread(startInOtherThreadSingle);
    isReceive.clear();
  }
}

void RtlsdrControl::startCounter() {}

void RtlsdrControl::stop() {
  if (isReceive.test()) {
    isReceive.clear();
  } else {
    throw std::runtime_error("nothing to stop");
  }
}

// set transfer params

void RtlsdrControl::setPacketCount(size_t packetCount) {}

std::size_t RtlsdrControl::getPacketSize() const {}

void RtlsdrControl::setPacketSize(size_t size) {}

void RtlsdrControl::setType(TransferParams::Type t) {}
void RtlsdrControl::setHandler(Handler hdl) { hdl_ = hdl; }
// set params

void RtlsdrControl::setFrequency(uint64_t fr) {
  if (fr < 100000 || fr > 1750000) {
    throw std::logic_error("freqency is not in the range 100kHz 1.75MHz");
  }
  check(rtlsdr_set_center_freq(device_, fr));
}

void RtlsdrControl::setLevel(uint32_t levelDb) {

  uint32_t maxGain = gains_[numGains_];

  if (levelDb > gains_[numGains_]) {
    check(rtlsdr_set_tuner_gain(device_, maxGain));
    check(
        // TODO: впихнуть сюда готовые таблицы со стадиями усиления
        rtlsdr_set_tuner_if_gain(device_, 6, levelDb * 10 - maxGain));

    ifGain_ = levelDb - maxGain > kMaxIfGain ? kMaxIfGain : levelDb - maxGain;

  } else {
    rtlsdr_set_tuner_gain(device_, levelDb);
  }
}

void RtlsdrControl::setLna(uint32_t levelDb) {}

void RtlsdrControl::setPower(uint32_t power) {}

void RtlsdrControl::setAttenuator(uint32_t attDb) {}

uint64_t RtlsdrControl::getFrequency() const {
  return rtlsdr_get_center_freq(device_);
}

uint32_t RtlsdrControl::getLevel() const {
  return rtlsdr_get_tuner_gain(device_) + ifGain_;
}

uint32_t RtlsdrControl::getLna() const {}

uint32_t RtlsdrControl::getPower() const {}

uint32_t RtlsdrControl::getAttenuator() const {}
// to context
//
void *RtlsdrControl::toContext() noexcept {};

void RtlsdrControl::check(int retVal) {
  if (retVal) {
    throw std::runtime_error("rtlsdr function return: " +
                             std::to_string(retVal));
  }
}


void RtlsdrControl::setParam(TransferParams &params) { param_ = params; }

ISDRStreamTransfer::Handler RtlsdrControl::getHandler() { return hdl_; }

void RtlsdrControl::setSampleRate(uint64_t sr) {
  check(rtlsdr_set_sample_rate(device_, sr));
}

