#pragma once

#include <ISensor.h>

enum VOLTAGE
{
    HIGH_5_0 = 255,
    LOW_1_4 = 72
};

enum PHASES
{
    NONE = 0,
    HEATING = 59999, // phases should be different in milliseconds, otherwise
    COOLING = 60000, // comparing different phases won't work for isInPhase()
    READING = 30000
};

#define NUMBER_OF_MEASUREMENTS 10
#define READING_INTERVAL 3000
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
    // uint32_t _phaseStartedMls;
    // PHASES _currentPhase;
    // uint32_t _previusReadingMillis;
    int _valueArray[NUMBER_OF_MEASUREMENTS];
    uint8_t _currentSavingItemInArray;
    int _averageOfAllMeasuredValues;

    // bool isOutsideMillisInterval(PHASES phase, uint32_t delta);
    void setHeaterVoltageForPhase(PHASES phase);
    // bool isReadyToReadNextMeasurement(uint32_t now);
    int calculateAverageOfAllMeasurements();
    void readNextMeasurement_new();
    void saveAverageMeasurement();

public:
    ~MQ7COArduino();
    MQ7COArduino();
    MQ7COArduino(uint8_t signalPIN, uint8_t signalHeaterPIN);
    void changeSignalAndHeaterPINs(uint8_t signalPIN, uint8_t signalHeaterPIN);
    // bool init();
    bool init_new();

    // bool isPhaseCompleted(PHASES phase);
    // bool isInPhase(PHASES phase);
    // bool setPhase(PHASES phase);
    // bool readNextMeasurement();
    // bool saveAverageMeasurement();
    // void setHeaterVoltageForPhase_new(PHASES phase);

    void requestCurrentMeasurement() override;
    void requestCurrentMeasurement_new();
    uint8_t getNumberOfConnectedSensors() override;
    float getCurrentMeasurementByID(uint8_t id = 0) override;
};
