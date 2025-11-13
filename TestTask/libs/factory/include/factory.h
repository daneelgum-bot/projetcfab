#pragma once

#include <memory>
#include <stdexcept>


#include "transfer.h"
#include "sdr_rf.h"


class SDRFactory {
public:
    enum class DeviceType {
        RTL_SDR,
        HACK_RF
    };


    struct DeviceInterfaces {
        std::shared_ptr<ISDRStreamTransfer> stream;
        std::shared_ptr<ISdrDevice> device;
        std::shared_ptr<IDeviceRF> rf;


        DeviceInterfaces(
            std::shared_ptr<ISDRStreamTransfer> stream_ptr,
            std::shared_ptr<ISdrDevice> device_ptr,
            std::shared_ptr<IDeviceRF> rf_ptr
        );
    };


    static DeviceInterfaces createDevice(DeviceType type, size_t dev_index = 0);
};



/* проблема в том что использвоание для пользователя корявое из-за доп обращения
 решение-наследование от общего интерфейса и возрат базового класса фабрикой иначе хз*/
