#include <MQ7COArduino.h>
#include <Arduino.h>

void MQ7COArduino::changeSignalAndHeaterPINs(uint8_t signalPIN, uint8_t signalHeaterPIN)
{
    _signalPIN = signalPIN;
    _signalHeaterPIN = signalHeaterPIN;
    _sensorInitCompleted = false;
    _phaseStartedMls = 0;
    _currentPhase = NONE;
    _currentSavingItemInArray = 0;
    _averageofAllMeasuredValues = -1;
    _previusReadingMillis = 0;
    _nextReadIntervalMillis = 3000;

    for (uint8_t i = 0; i < 10; i++)
        _valueArray[i] = -1;
}
MQ7COArduino::MQ7COArduino()
{
    changeSignalAndHeaterPINs(0, 0);
}
MQ7COArduino::MQ7COArduino(uint8_t signalPIN, uint8_t signalHeaterPIN)
{
    changeSignalAndHeaterPINs(signalPIN, signalHeaterPIN);
}

bool MQ7COArduino::init()
{
    if (_signalPIN == _signalHeaterPIN)
        return false;

    pinMode(_signalPIN, OUTPUT);
    pinMode(_signalHeaterPIN, OUTPUT);
    _sensorInitCompleted = true;

    return true;
}

void MQ7COArduino::setHeatingVoltageForPhase(PHASES phase)
{
    analogWrite(_signalHeaterPIN, phase == HEATING ? HIGH_5_0 : LOW_1_4);
}
bool MQ7COArduino::setPhase(PHASES phase)
{
    if (!_sensorInitCompleted)
        return false;

    _phaseStartedMls = millis();
    _currentPhase = phase;
    setHeatingVoltageForPhase(phase);

    return true;
}

bool MQ7COArduino::isOutsideMillisInterval(PHASES phase, uint32_t delta)
{
    return phase <= delta;
}
bool MQ7COArduino::isPhaseCompleted(PHASES phase)
{
    uint32_t now = millis();
    uint32_t delta = now - _phaseStartedMls;

    return isOutsideMillisInterval(phase, delta);
}

bool MQ7COArduino::isInPhase(PHASES phase)
{
    return _currentPhase == phase;
}

bool MQ7COArduino::isReadyToReadNextMeasurement(uint32_t now)
{
    if (now - _previusReadingMillis >= _nextReadIntervalMillis)
        return true;

    return false;
}
bool MQ7COArduino::readNextMeasurement()
{
    if (READING != _currentPhase)
        return false;

    uint32_t now = millis();
    if (!isReadyToReadNextMeasurement(now))
        return false;

    _valueArray[_currentSavingItemInArray++] = analogRead(_signalPIN);
    _previusReadingMillis = now;

    return true;
}

int MQ7COArduino::calculateAverageOfAllMeasurements()
{
    int sumOfMeasurements = 0;
    for (uint8_t i = 0; i < 10; i++)
        sumOfMeasurements += _valueArray[i];

    return sumOfMeasurements / 10;
}
bool MQ7COArduino::saveAverageMeasurement()
{
    if (!isPhaseCompleted(READING))
        return false;

    _averageofAllMeasuredValues = calculateAverageOfAllMeasurements();

    return true;
}

uint8_t MQ7COArduino::getNumberOfConnectedSensors()
{
    return 1;
}

float MQ7COArduino::getCurrentMeasurementByID(uint8_t id)
{
    return _averageofAllMeasuredValues;
}

void MQ7COArduino::requestCurrentMeasurement()
{
    if (isInPhase(HEATING))
    {
        if (!isPhaseCompleted(HEATING))
            return;
        setPhase(COOLING);
    }
    if (isInPhase(COOLING))
    {
        if (!isPhaseCompleted(COOLING))
            return;
        setPhase(READING);
    }
    if (isInPhase(READING))
    {
        readNextMeasurement();
        if (!isPhaseCompleted(READING))
            return;
        saveAverageMeasurement();
    }
    setPhase(HEATING);
}
