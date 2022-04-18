#pragma once
#include <Arduino.h>
#include <ISensor.h>
#include <iarduino_I2C_SHT.h> // SHT20 is a device with 2 integrated sensors: temperature and humidity.
                              //  id 0 - will be temperature
                              //  id 1 - will be humidity

namespace sht_nm
{
    const uint8_t NUMBER_OF_MEASUREMENTS = 3;
    const int UNINITIALIZED_MEASUREMENT_VALUE = -127;
    const uint8_t NUMBER_OF_SENSORS_ON_BUS = 2;
    const uint16_t DEFAULT_READING_INTERVAL = 3000;
    const uint8_t TEMPERATURE_ID = 0;
    const uint8_t HUMIDITY_ID = 1;
}

class SHT20Arduino : public ISensor
{
private:
    iarduino_I2C_SHT &_sht;
    bool _sensorInitCompleted;
    float _sensorHumidityArray[sht_nm::NUMBER_OF_MEASUREMENTS];
    float _sensorTemperatureArray[sht_nm::NUMBER_OF_MEASUREMENTS];
    uint8_t _currentSavingItemInArray;
    float _humidityAverage;
    float _temperatureAverage;
    uint32_t _startReadMillis;
    uint16_t _readingInterval;

    void saveAverageMeasurement();
    bool isReadyForNextRead(uint32_t now);
    bool isArrayFull();

public:
    SHT20Arduino(iarduino_I2C_SHT &sht);
    bool init(uint16_t ReadingInterval = sht_nm::DEFAULT_READING_INTERVAL);
    uint8_t getAddress();

    void requestCurrentMeasurement() override;
    uint8_t getNumberOfConnectedSensors() override;
    float getCurrentMeasurementByID(uint8_t id = 0) override;
};
