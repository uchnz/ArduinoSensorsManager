#pragma once
#include <Arduino.h>
#include <BaseSensor.h>

namespace sas_nm
{
    const uint8_t NUMBER_OF_MEASUREMENTS = 3;
    const uint8_t NUMBER_OF_SENSORS_ON_BUS = 1;
    const int UNINITIALIZED_MEASUREMENT_VALUE = 0x8000;
}

class SASArduino : public BaseSensor
{
private:
    int _sensorValueAveraged;
    int _sensorValueArray[sas_nm::NUMBER_OF_MEASUREMENTS];
    uint8_t _currentSavingItemInArray;

    void saveAverageMeasurement();
    bool isArrayFull();

protected:
    virtual void reset() override;

public:
    SASArduino(const char *name, IIO &io);

    virtual bool init(ITimer *timer) override;

    virtual bool requestCurrentMeasurement() override;
    virtual uint8_t getNumberOfConnectedSensors() override;
    virtual double getCurrentMeasurementByID(uint8_t id = 0) override;
};
