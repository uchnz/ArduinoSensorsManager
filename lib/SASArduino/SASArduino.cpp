#include <SASArduino.h>

SASArduino::SASArduino()
{
    SetPIN(sas_nm::UNINITIALIZED_PIN_VALUE);
}

SASArduino::SASArduino(uint8_t signalPIN)
{
    SetPIN(signalPIN);
}

void SASArduino::SetPIN(uint8_t signalPIN)
{
    _signalPIN = signalPIN;
    _sensorInitCompleted = false;
    _sensorValue = sas_nm::UNINITIALIZED_MEASUREMENT_VALUE;
    _currentSavingItemInArray = 0;
    _startReadMillis = 0;
    _readingInterval = sas_nm::DEFAULT_READING_INTERVAL;
    for (uint8_t i = 0; i < sas_nm::NUMBER_OF_MEASUREMENTS; i++)
        _sensorValueArray[i] = sas_nm::UNINITIALIZED_MEASUREMENT_VALUE;
}

bool SASArduino::init(uint16_t ReadingInterval)
{
    if (sas_nm::UNINITIALIZED_PIN_VALUE == _signalPIN)
        return false;

    pinMode(_signalPIN, INPUT);
    _readingInterval = ReadingInterval;
    _sensorInitCompleted = true;
    return true;
}

uint8_t SASArduino::getNumberOfConnectedSensors()
{
    return sas_nm::NUMBER_OF_SENSORS_ON_BUS;
}

void SASArduino::saveAverageMeasurement()
{
    _sensorValue = 0;
    for (uint8_t i = 0; i < sas_nm::NUMBER_OF_MEASUREMENTS; i++)
        _sensorValue += _sensorValueArray[i];

    _sensorValue = _sensorValue / sas_nm::NUMBER_OF_MEASUREMENTS;
    _currentSavingItemInArray = 0;
}
bool SASArduino::isReadyForNextRead(uint32_t now)
{
    return (now - _startReadMillis) >= _readingInterval;
}
bool SASArduino::isArrayFull()
{
    return (_currentSavingItemInArray > sas_nm::NUMBER_OF_MEASUREMENTS - 1);
}
void SASArduino::requestCurrentMeasurement()
{
    if (!_sensorInitCompleted)
        return;

    uint32_t now = millis();
    if (!isReadyForNextRead(now))
        return;

    if (isArrayFull())
        saveAverageMeasurement();

    _sensorValueArray[_currentSavingItemInArray++] = analogRead(_signalPIN);
    _startReadMillis = millis();
}

float SASArduino::getCurrentMeasurementByID(uint8_t id)
{
    if (!_sensorInitCompleted)
        return sas_nm::UNINITIALIZED_MEASUREMENT_VALUE;

    return _sensorValue;
}
