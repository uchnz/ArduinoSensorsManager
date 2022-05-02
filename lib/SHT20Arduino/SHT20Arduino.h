#pragma once
#include <BaseSensor.h>
#include <iarduino_I2C_SHT.h> // SHT20 is a device with 2 integrated sensors: temperature and humidity.
                              //  id 0 - will be temperature
                              //  id 1 - will be humidity

namespace sht_nm
{
    const uint8_t NUMBER_OF_MEASUREMENTS = 3;
    const uint8_t NUMBER_OF_SENSORS_ON_BUS = 2;
    const uint8_t TEMPERATURE_ID = 0;
    const uint8_t HUMIDITY_ID = 1;
}

class SHT20Arduino : public BaseSensor
{
private:
    double _sensorTemperatureArray[sht_nm::NUMBER_OF_MEASUREMENTS];
    double _sensorHumidityArray[sht_nm::NUMBER_OF_MEASUREMENTS];
    uint8_t _currentSavingItemInArray;
    double _humidityAverage;
    double _temperatureAverage;

    void saveAverageMeasurement();
    bool isArrayFull();

protected:
    void reset();

public:
    SHT20Arduino(const char *name, IIO &io);

    virtual bool init(ITimer *timer) override;

    bool requestCurrentMeasurement() override;
    uint8_t getNumberOfConnectedSensors() override;
    double getCurrentMeasurementByID(uint8_t id = 0) override;
};
