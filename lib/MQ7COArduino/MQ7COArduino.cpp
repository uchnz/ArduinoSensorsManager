#include <MQ7COArduino.h>
#include <Arduino.h>

MQ7COArduino::MQ7COArduino()
{
    changeSignalAndHeaterPINs(0, 0);
}

MQ7COArduino::MQ7COArduino(uint8_t signalPIN, uint8_t signalHeaterPIN)
{
    changeSignalAndHeaterPINs(signalPIN, signalHeaterPIN);
}

void MQ7COArduino::changeSignalAndHeaterPINs(uint8_t signalPIN, uint8_t signalHeaterPIN)
{
    _signalPIN = signalPIN;
    _signalHeaterPIN = signalHeaterPIN;
    _initCompleted = false;
    _phaseStartedMls = 0;
    _coolingPhaseRunning = _heatingPhaseRunning = _readingPhaseRunning = false;
}

bool MQ7COArduino::init()
{
    if (_signalPIN == _signalHeaterPIN)
        return false;

    pinMode(_signalPIN, OUTPUT);
    pinMode(_signalHeaterPIN, OUTPUT);
    _initCompleted = true;

    return true;
}

void MQ7COArduino::requestCurrentMeasurement()
{
}

bool MQ7COArduino::requestNewMeasurement()
{
    if (_initCompleted)
        return true;

    return false;
}

void MQ7COArduino::setHeaterPhase(VOLTAGE voltage)
{
    _phaseStartedMls = millis();
    _coolingPhaseRunning = _heatingPhaseRunning = _readingPhaseRunning = false;

    if (HIGH_5_0 == voltage)
        _heatingPhaseRunning = true;

    if (LOW_1_4 == voltage)
        _coolingPhaseRunning = true;
}

bool MQ7COArduino::setHeaterVoltage(VOLTAGE voltage)
{
    if (!_initCompleted)
        return false;

    analogWrite(_signalHeaterPIN, voltage);
    setHeaterPhase(voltage);

    return true;
}

bool MQ7COArduino::isOutsideMillisInterval(CYCLE_INTERVALS interval, uint32_t delta)
{
    return interval <= delta;
}

bool MQ7COArduino::isPhaseCompleted()
{
    uint32_t now = millis();
    uint32_t delta = now - _phaseStartedMls;

    if (_heatingPhaseRunning)
        return isOutsideMillisInterval(HEATING_INTERVAL, delta);

    if (_coolingPhaseRunning)
        return isOutsideMillisInterval(COOLING_INTERVAL, delta);

    if (_readingPhaseRunning)
        return isOutsideMillisInterval(READING_INTERVAL, delta);

    return false;
}

bool MQ7COArduino::isInHeatingPhase()
{
    return _heatingPhaseRunning;
}

bool MQ7COArduino::isInCoolingPhase()
{
    return _coolingPhaseRunning;
}

bool MQ7COArduino::isInReadingPhase()
{
    return _readingPhaseRunning;
}

bool MQ7COArduino::readMeasurement()
{
    if (_readingPhaseRunning)
        return false; // implement readings here

    _phaseStartedMls = millis();

    _coolingPhaseRunning = _heatingPhaseRunning = false;
    _readingPhaseRunning = true;

    // implement readings here
    return false;
}

uint8_t MQ7COArduino::getNumberOfConnectedSensors()
{
    return 1;
}

float MQ7COArduino::getCurrentMeasurementByID(uint8_t id)
{
    return analogRead(_signalPIN);
}
