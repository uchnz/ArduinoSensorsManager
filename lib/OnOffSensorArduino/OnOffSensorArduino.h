#pragma once
#include <Arduino.h>
#include <ISensor.h>

namespace oos_nm
{
    const uint8_t UNINITIALIZED_PIN_VALUE = 255;
    const uint8_t NUMBER_OF_MEASUREMENTS = 3;
    const float UNINITIALIZED_MEASUREMENT_VALUE = -1;
    const uint8_t NUMBER_OF_SENSORS_ON_BUS = 1;
    const uint16_t DEFAULT_READING_INTERVAL = 500;
}

class OnOffSensorArduino : public ISensor
{
private:
    uint8_t _signalPIN;
    bool _sensorInitCompleted;
    float _sensorValue;
    float _sensorValueArray[oos_nm::NUMBER_OF_MEASUREMENTS];
    uint8_t _currentSavingItemInArray;
    uint32_t _startReadMillis;
    uint16_t _readingInterval;

    void saveAverageMeasurement();
    bool isReadyForNextRead(uint32_t now);
    bool isArrayFull();

public:
    OnOffSensorArduino();
    OnOffSensorArduino(uint8_t signalPIN);
    bool init(uint16_t ReadingInterval = oos_nm::DEFAULT_READING_INTERVAL);
    void SetPIN(uint8_t signalPIN);

    void requestCurrentMeasurement() override;
    uint8_t getNumberOfConnectedSensors() override;
    float getCurrentMeasurementByID(uint8_t id = 0) override;
};