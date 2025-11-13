#include <libhackrf/hackrf.h>
#include "../sdr_rf_interface/include/sdr_rf.h"
#include "../transfer_interface/include/transfer.h"

#include <atomic>
#include <cstddef>
#include <cstdint>
#include <thread>

class HackRFControl : public ISDRStreamTransfer, public ISdrDevice, public IDeviceRF {

    public:
    enum class HackRFState {
        waiting,
        recieving
    };

    explicit HackRFControl(size_t dev_index);
    ~HackRFControl();

    void initialize() override;
    void finalize() override;

    // ITransferControl interface
    void setHandler(Handler hdl) override;
    Handler getHandler();

    void setPacketCount(size_t packetCount) override;
    void setPacketSize(size_t size) override;
    std::size_t getPacketSize() const override;
    void setType(TransferParams::Type t) override;

    // ISDRStreamTransfer interface
    void start() override;
    void startCounter() override;
    void stop() override;

    // transfer parameters
    void setParam(TransferParams &params);

    // IDeviceRF interface
    void setFrequency(uint64_t frequencyHz) override;
    uint64_t getFrequency() const override;
    void setLevel(uint32_t levelDb) override;
    uint32_t getLevel() const override;
    void setLna(uint32_t lna) override;
    uint32_t getLna() const override;
    void setPower(uint32_t power) override;
    uint32_t getPower() const override;
    void setAttenuator(uint32_t attDb) override;
    uint32_t getAttenuator() const override;
    void setSampleRate(double sr);

    // ISdrDevice interface
    void *toContext() noexcept override;

private:
    void check(int result);
    hackrf_device* device_ = nullptr;
    TransferParams params_;
    uint64_t frequency_;
    double SampleRatefrequency_;
    uint32_t levelDb_;
};
