#include <MQ7COArduino.h>
#include <MQ7Impl.h>

void MQ7COArduino::changeSignalAndHeaterPINs(uint8_t signalPIN, uint8_t signalHeaterPIN)
{
    _signalPIN = signalPIN;
    _signalHeaterPIN = signalHeaterPIN;
    _sensorInitCompleted = false;
    _currentSavingItemInArray = 0;
    _averageOfAllMeasuredValues = UNINITIALIZED_MEASUREMENT_VALUE;

    for (uint8_t i = 0; i < NUMBER_OF_MEASUREMENTS; i++)
        _valueArray[i] = UNINITIALIZED_MEASUREMENT_VALUE;
}

MQ7COArduino::~MQ7COArduino()
{
    delete _pimpl;
}
MQ7COArduino::MQ7COArduino()
    : _pimpl(new MQ7Impl())
{
    changeSignalAndHeaterPINs(0, 0);
}
MQ7COArduino::MQ7COArduino(uint8_t signalPIN, uint8_t signalHeaterPIN)
    : _pimpl(new MQ7Impl())
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
    _pimpl->setPhase(HEATING);
    setHeaterVoltageForPhase(HIGH_5_0);

    return true;
}
void MQ7COArduino::setHeaterVoltageForPhase(VOLTAGE voltage)
{
    analogWrite(_signalHeaterPIN, voltage);
}

void MQ7COArduino::readNextMeasurement()
{
    if (_currentSavingItemInArray < NUMBER_OF_MEASUREMENTS)
        _valueArray[_currentSavingItemInArray++] = analogRead(_signalPIN);
}

void MQ7COArduino::resetArrayCounter()
{
    _currentSavingItemInArray = 0;
}
void MQ7COArduino::saveAverageMeasurement()
{
    _averageOfAllMeasuredValues = 0;
    for (uint8_t i = 0; i < _currentSavingItemInArray; i++)
        _averageOfAllMeasuredValues += _valueArray[i];

    _averageOfAllMeasuredValues = _averageOfAllMeasuredValues / _currentSavingItemInArray;
}

uint8_t MQ7COArduino::getNumberOfConnectedSensors()
{
    return NUMBER_OF_SENSORS_ON_BUS;
}

float MQ7COArduino::getCurrentMeasurementByID(uint8_t id)
{
    return _averageOfAllMeasuredValues;
}

void MQ7COArduino::requestCurrentMeasurement()
{
    if (!_sensorInitCompleted)
        return;

    if (_pimpl->isInPhase(HEATING))
    {
        if (!_pimpl->isPhaseCompleted(HEATING))
            return;
        _pimpl->setPhase(COOLING);
        setHeaterVoltageForPhase(LOW_1_4);
    }
    if (_pimpl->isInPhase(COOLING))
    {
        if (!_pimpl->isPhaseCompleted(COOLING))
            return;
        _pimpl->setPhase(READING);
    }
    if (_pimpl->isInPhase(READING))
    {
        if (!_pimpl->isTimeToReadMeasurement())
            return;
        readNextMeasurement();
        if (!_pimpl->isPhaseCompleted(READING))
            return;
        saveAverageMeasurement();
        resetArrayCounter();
    }
    _pimpl->setPhase(HEATING);
    setHeaterVoltageForPhase(HIGH_5_0);
}