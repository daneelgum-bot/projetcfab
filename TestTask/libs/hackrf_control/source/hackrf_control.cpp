#include "hackrf_control.h"
#include <iostream>
#include <stdexcept>


HackRFControl::HackRFControl(size_t dev_index): ISDRStreamTransfer(TransferParams()) {
    int result = hackrf_init();
    if (result != HACKRF_SUCCESS) {
        throw std::runtime_error("lib error");
    }
    
    result = hackrf_open(&device_);
    if (result != HACKRF_SUCCESS) {
        hackrf_exit();
        throw std::runtime_error("open device error");
    }
}

HackRFControl::~HackRFControl() {
    if (device_) {
        hackrf_close(device_);
        device_ = nullptr;
        std::cout << "HackRF device closed" << std::endl;
    }
    hackrf_exit();
    std::cout << "HackRF library exited" << std::endl;
}


void HackRFControl::check(int result) {
    if (result != HACKRF_SUCCESS) {
        throw std::runtime_error("HackRF error: " + std::to_string(result));
    }
}
/*
void HackRFControl::setFrequency(uint64_t freqHz) {

    check(hackrf_set_freq(device_, freqHz));

    frequency_ = freqHz;


}
*/
void HackRFControl::setFrequency(uint64_t freqHz) {
    if (device_){
    int result = hackrf_set_freq(device_, freqHz);
        if (result == HACKRF_SUCCESS) {
            std::cout<<"FREQ_YES"<<std::endl;
        }
    } else {
        std::cout<<"Freq_ERROR"<<std::endl;
    }
    frequency_ = freqHz;
    }

uint64_t HackRFControl::getFrequency() const {
    return frequency_;
}

void HackRFControl::setSampleRate(double sr) {
        if (device_){
    check(hackrf_set_sample_rate(device_, sr));
    }
    SampleRatefrequency_ = sr;
}

void HackRFControl::setParam(TransferParams &params) { params_ = params; }

void *HackRFControl::toContext() noexcept {return nullptr;};

void HackRFControl::setLna(uint32_t levelDb) {
    if (device_){
    check(hackrf_set_lna_gain(device_, levelDb));
    }
    levelDb_ = levelDb;
}


uint32_t HackRFControl::getLna() const{
    return levelDb_;
}

void HackRFControl::setHandler(Handler hdl) {}
void HackRFControl::setPacketCount(size_t packetCount) {}
void HackRFControl::setPacketSize(size_t size) {}
std::size_t HackRFControl::getPacketSize() const { return 1024; }
void HackRFControl::setType(TransferParams::Type t) {}
void HackRFControl::initialize() {}
void HackRFControl::finalize() {}
void HackRFControl::start() {}
void HackRFControl::startCounter() {}
void HackRFControl::stop() {}
void HackRFControl::setPower(uint32_t power) {}
uint32_t HackRFControl::getPower() const { return 0; }
void HackRFControl::setAttenuator(uint32_t attDb) {}
uint32_t HackRFControl::getAttenuator() const { return 0; }
void HackRFControl::setLevel(uint32_t power) {}
uint32_t HackRFControl::getLevel() const { return levelDb_; }




