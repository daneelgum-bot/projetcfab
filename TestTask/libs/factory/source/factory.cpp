#include "factory.h"
#include "rtlsdr_control.h"
#include "hackrf_control.h"

SDRFactory::DeviceInterfaces::DeviceInterfaces(
    std::shared_ptr<ISDRStreamTransfer> stream_ptr,
    std::shared_ptr<ISdrDevice> device_ptr,
    std::shared_ptr<IDeviceRF> rf_ptr
) : stream(stream_ptr), device(device_ptr), rf(rf_ptr) {}

SDRFactory::DeviceInterfaces SDRFactory::createDevice(DeviceType type, size_t dev_index) {
    switch (type) {
        case DeviceType::RTL_SDR: {
            auto rtlsdr = std::make_shared<RtlsdrControl>(dev_index);
            return DeviceInterfaces(rtlsdr,rtlsdr,rtlsdr);
        }
        case DeviceType::HACK_RF: {
            auto hackrf = std::make_shared<HackRFControl>(dev_index);
            return DeviceInterfaces(hackrf,hackrf,hackrf);
        }
        default:
            throw std::invalid_argument("Unknown device type");
    }
}