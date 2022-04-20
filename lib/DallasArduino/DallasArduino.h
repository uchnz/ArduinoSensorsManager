#pragma once

#include <DallasTemperature.h>
#include <ISensor.h>
#include <Arduino.h>

namespace dallas_nm
{
    const uint8_t UNINITIALIZED_PIN_VALUE = 255;
    const uint8_t NUMBER_OF_MEASUREMENTS = 3;
    const int UNINITIALIZED_MEASUREMENT_VALUE = -127;
    const uint16_t DEFAULT_READING_INTERVAL = 100;
}

class DallasArduino : public ISensor
{
private:
    //    OneWire *_ow;
    DallasTemperature *_ds;
    OneWire _oneWire;
    DallasTemperature _sensors;
    uint8_t _totalConnectedSensors;
    float _average[3];
    float *_averageTemperatures;
    // void showNumberOfFoundSensors();
    // void showParasitePowerMode();
    // void showSensorsAddresses();
    // String getAddressString(DeviceAddress deviceAddress);

    float **_temperatures2DArray; //[dallas_nm::NUMBER_OF_MEASUREMENTS];

    uint8_t _currentSavingItemInArray;
    uint32_t _startReadMillis;
    uint16_t _readingInterval;

    void saveAverageMeasurement();
    bool isReadyForNextRead(uint32_t now);
    bool isArrayFull();

public:
    DallasArduino();
    ~DallasArduino();
    DallasArduino(DallasTemperature &dt); //, OneWire &ow);
    DallasArduino(uint8_t sensorPIN);
    bool init(uint16_t readingInterval = dallas_nm::DEFAULT_READING_INTERVAL);
    // void setSensorsPrecision(int precision);

    // ISensor
    void requestCurrentMeasurement() override;
    uint8_t getNumberOfConnectedSensors() override;
    float getCurrentMeasurementByID(uint8_t id = 0) override;
};
