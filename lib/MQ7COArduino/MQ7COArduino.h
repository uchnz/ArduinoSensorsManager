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

class MQ7COArduino : public ISensor
{
private:
    uint8_t _signalPIN;
    uint8_t _signalHeaterPIN;
    bool _sensorInitCompleted;
    uint32_t _phaseStartedMls;
    PHASES _currentPhase;
    int _valueArray[10];
    uint8_t _currentSavingItemInArray;
    uint32_t _previusReadingMillis;
    int _averageofAllMeasuredValues;
    uint32_t _nextReadIntervalMillis;

    bool isOutsideMillisInterval(PHASES phase, uint32_t delta);
    void setHeatingVoltageForPhase(PHASES phase);
    bool isReadyToReadNextMeasurement(uint32_t now);
    int calculateAverageOfAllMeasurements();

public:
    MQ7COArduino();
    MQ7COArduino(uint8_t signalPIN, uint8_t signalHeaterPIN);
    void changeSignalAndHeaterPINs(uint8_t signalPIN, uint8_t signalHeaterPIN);
    bool init();

    bool isPhaseCompleted(PHASES phase);
    bool isInPhase(PHASES phase);
    bool setPhase(PHASES phase);
    bool readNextMeasurement();
    bool saveAverageMeasurement();

    void requestCurrentMeasurement() override;
    uint8_t getNumberOfConnectedSensors() override;
    float getCurrentMeasurementByID(uint8_t id = 0) override;
};
