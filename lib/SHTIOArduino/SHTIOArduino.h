#pragma once
#include <IIO.h>
#include <iarduino_I2C_SHT.h>

namespace shtio_nm
{
    const double UNINITIALIZED_SENSOR_VALUE = 0xFFFFFFFF;
    const uint8_t TEMPERATURE_ID = 0;
    const uint8_t HUMIDITY_ID = 1;
    const uint8_t TOTAL_SENSORS_ON_BUS = 2;
}

class SHTIOArduino : public IIO
{
private:
    iarduino_I2C_SHT &_sht;
    bool _initCompleted;

public:
    SHTIOArduino(iarduino_I2C_SHT &sht);
    bool isInitCompleted();

    // IIO
    virtual bool init() override;
    virtual double read(uint8_t id = 0) override;
    virtual uint8_t getTotalSensors() override;
};