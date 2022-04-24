#include <SASArduino.h>

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
SASArduino::SASArduino()
{
    SetPIN(sas_nm::UNINITIALIZED_PIN_VALUE);
}
SASArduino::SASArduino(uint8_t signalPIN)
{
    SetPIN(signalPIN);
}
void SASArduino::initName(const char *name)
{
    int nameLengthWithNull = strlen(name) + 1;
    if (nameLengthWithNull > sas_nm::MAX_SENSOR_NAME)
        nameLengthWithNull = sas_nm::MAX_SENSOR_NAME - 1;

    memcpy(_sensorName, name, nameLengthWithNull);
}
bool SASArduino::init(const char *name, uint16_t ReadingInterval)
{
    if (sas_nm::UNINITIALIZED_PIN_VALUE == _signalPIN)
        return false;
    if (!name || strlen(name) < 1)
        return false;

    pinMode(_signalPIN, INPUT);
    initName(name);
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

double SASArduino::getCurrentMeasurementByID(uint8_t id)
{
    if (!_sensorInitCompleted)
        return sas_nm::UNINITIALIZED_MEASUREMENT_VALUE;

    return _sensorValue;
}

uint8_t SASArduino::getName(char *name)
{
    if (!_sensorInitCompleted)
        return 0;

    int nameLength = strlen(_sensorName);
    memcpy(name, _sensorName, nameLength + 1);
    return nameLength;
}