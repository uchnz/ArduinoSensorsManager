#pragma once
#include <Arduino.h>
#include <ISensor.h>

// enum VOLTAGE
// {
//     HIGH_5_0 = 255,
//     LOW_1_4 = 72
// };

namespace mq7co_nm
{
    enum VOLTAGE
    {
        HIGH_5_0 = 255,
        LOW_1_4 = 72
    };

    const uint8_t UNINITIALIZED_PIN_VALUE = 255;
    const uint8_t NUMBER_OF_MEASUREMENTS = 10;
    const int UNINITIALIZED_MEASUREMENT_VALUE = -1;
    const uint8_t NUMBER_OF_SENSORS_ON_BUS = 1;
    const uint16_t DEFAULT_READING_INTERVAL = 3000;
    const uint8_t MAX_SENSOR_NAME = 30;
}

class MQ7Impl;
class MQ7COArduino : public ISensor
{
private:
    MQ7Impl *_pimpl;

    uint8_t _signalPIN;
    uint8_t _signalHeaterPIN;
    bool _sensorInitCompleted;
    char _sensorName[mq7co_nm::MAX_SENSOR_NAME];
    int _valueArray[mq7co_nm::NUMBER_OF_MEASUREMENTS];
    uint8_t _currentSavingItemInArray;
    int _averageOfAllMeasuredValues;

    void initName(const char *name);
    void setHeaterVoltageForPhase(mq7co_nm::VOLTAGE voltage);
    void readNextMeasurement();
    void saveAverageMeasurement();
    void resetArrayCounter();

public:
    ~MQ7COArduino();
    MQ7COArduino();
    MQ7COArduino(uint8_t signalPIN, uint8_t signalHeaterPIN);
    void changeSignalAndHeaterPINs(uint8_t signalPIN, uint8_t signalHeaterPIN);
    bool init(const char *name, uint16_t ReadingInterval = mq7co_nm::DEFAULT_READING_INTERVAL);

    void requestCurrentMeasurement() override;
    uint8_t getNumberOfConnectedSensors() override;
    double getCurrentMeasurementByID(uint8_t id = 0) override;
    uint8_t getName(char *name);
    // uint8_t getName(char *&name);
};
