#pragma once

#include <ISensor.h>

enum VOLTAGE
{
    HIGH_5_0 = 255,
    LOW_1_4 = 72
};
enum CYCLE_INTERVALS
{
    HEATING_INTERVAL = 60000,
    COOLING_INTERVAL = 60000,
    READING_INTERVAL = 30000
};

class MQ7COArduino : public ISensor
{
private:
    uint8_t _signalPIN;
    uint8_t _signalHeaterPIN;
    bool _initCompleted;
    uint32_t _phaseStartedMls;
    bool _heatingPhaseRunning;
    bool _coolingPhaseRunning;
    bool _readingPhaseRunning;

    void setHeaterPhase(VOLTAGE voltage);
    bool isOutsideMillisInterval(CYCLE_INTERVALS interval, uint32_t delta);

public:
    MQ7COArduino();
    MQ7COArduino(uint8_t signalPIN, uint8_t signalHeaterPIN);
    void changeSignalAndHeaterPINs(uint8_t signalPIN, uint8_t signalHeaterPIN);
    bool init();

    void requestCurrentMeasurement() override;
    bool requestNewMeasurement();
    bool isPhaseCompleted();
    bool isInHeatingPhase();
    bool isInCoolingPhase();
    bool isInReadingPhase();

    bool setHeaterVoltage(VOLTAGE voltage);

    uint8_t getNumberOfConnectedSensors() override;
    float getCurrentMeasurementByID(uint8_t id = 0) override;
};
