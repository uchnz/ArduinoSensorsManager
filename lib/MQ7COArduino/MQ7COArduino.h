#pragma once
#include <Arduino.h>
#include <ISensor.h>

enum VOLTAGE
{
    HIGH_5_0 = 255,
    LOW_1_4 = 72
};

#define NUMBER_OF_MEASUREMENTS 10
#define UNINITIALIZED_MEASUREMENT_VALUE -1
#define NUMBER_OF_SENSORS_ON_BUS 1

class MQ7Impl;
class MQ7COArduino : public ISensor
{
private:
    MQ7Impl *_pimpl;

    uint8_t _signalPIN;
    uint8_t _signalHeaterPIN;
    bool _sensorInitCompleted;
    int _valueArray[NUMBER_OF_MEASUREMENTS];
    uint8_t _currentSavingItemInArray;
    int _averageOfAllMeasuredValues;

    void setHeaterVoltageForPhase(VOLTAGE voltage);
    void readNextMeasurement();
    void saveAverageMeasurement();
    void resetArrayCounter();

public:
    ~MQ7COArduino();
    MQ7COArduino();
    MQ7COArduino(uint8_t signalPIN, uint8_t signalHeaterPIN);
    void changeSignalAndHeaterPINs(uint8_t signalPIN, uint8_t signalHeaterPIN);
    bool init();

    void requestCurrentMeasurement() override;
    uint8_t getNumberOfConnectedSensors() override;
    float getCurrentMeasurementByID(uint8_t id = 0) override;
};
