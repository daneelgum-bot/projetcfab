#include <iostream>
#include "factory.h"


int main() {
    std::cout << "Start" << std::endl;
    
 
    
        auto device = SDRFactory::createDevice(SDRFactory::DeviceType::HACK_RF, 0);
        std::cout << "Device created" << std::endl;
 std::cout<<"set1111------------"<<std::endl;
 device.stream ->start();
        device.rf ->setFrequency(100000002);
        std::cout<<"set222------------"<<std::endl;

        uint64_t actualFrequency = device.rf->getFrequency();


        std::cout<<"actual = "<< actualFrequency <<std::endl;

    

    
    std::cout << "End" << std::endl;
    return 0;
};