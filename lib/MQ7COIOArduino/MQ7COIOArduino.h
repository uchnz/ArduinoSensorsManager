#pragma once
#include <Arduino.h>
#include <IIO.h>
#include <IPhaseTimer.h>

namespace mq7coio_nm
{
    enum VOLTAGE
    {
        HIGH_5_0 = 255,
        LOW_1_4 = 72
    };

    const double UNINITIALIZED_SENSOR_VALUE = 0xFFFFFFFF;
    const uint8_t TOTAL_SENSORS_ON_BUS = 1;
}

class MQ7COIOArduino : public IIO
{
private:
    uint8_t _signalPIN;
    uint8_t _heaterPIN;
    bool _initCompleted;
    IPhaseTimer &_timer;

    void setHeaterVoltageForPhase(mq7coio_nm::VOLTAGE voltage);

public:
    MQ7COIOArduino(uint8_t pinSignal, uint8_t pinHeater, IPhaseTimer &timer);

    bool isInitCompleted();

    // IIO
    virtual bool init() override;
    virtual double read(uint8_t id = 0) override;
    virtual uint8_t getTotalSensors() override;
};
