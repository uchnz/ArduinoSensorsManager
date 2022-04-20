#include <OnOffSensorArduino.h>
#include <Arduino.h>

void OnOffSensorArduino::SetPIN(uint8_t signalPIN)
{
    _signalPIN = signalPIN;
    _sensorInitCompleted = false;
    _sensorValue = oos_nm::UNINITIALIZED_MEASUREMENT_VALUE;
    _currentSavingItemInArray = 0;
    _startReadMillis = 0;
    _readingInterval = oos_nm::DEFAULT_READING_INTERVAL;
    for (uint8_t i = 0; i < oos_nm::NUMBER_OF_MEASUREMENTS; i++)
        _sensorValueArray[i] = oos_nm::UNINITIALIZED_MEASUREMENT_VALUE;
}
OnOffSensorArduino::OnOffSensorArduino()
{
    SetPIN(oos_nm::UNINITIALIZED_PIN_VALUE);
}
OnOffSensorArduino::OnOffSensorArduino(uint8_t signalPIN)
{
    SetPIN(signalPIN);
}

bool OnOffSensorArduino::init(uint16_t ReadingInterval)
{

    if (oos_nm::UNINITIALIZED_PIN_VALUE == _signalPIN)
        return false;

    pinMode(_signalPIN, INPUT_PULLUP);
    _readingInterval = ReadingInterval;
    _sensorInitCompleted = true;
    return true;
}

uint8_t OnOffSensorArduino::getNumberOfConnectedSensors()
{
    return oos_nm::NUMBER_OF_SENSORS_ON_BUS;
}

void OnOffSensorArduino::saveAverageMeasurement()
{
    _sensorValue = 0;
    for (uint8_t i = 0; i < oos_nm::NUMBER_OF_MEASUREMENTS; i++)
        _sensorValue += _sensorValueArray[i];

    _sensorValue = round(_sensorValue / oos_nm::NUMBER_OF_MEASUREMENTS);
    _currentSavingItemInArray = 0;
}
bool OnOffSensorArduino::isReadyForNextRead(uint32_t now)
{
    return (now - _startReadMillis) >= _readingInterval;
}
bool OnOffSensorArduino::isArrayFull()
{
    return (_currentSavingItemInArray > oos_nm::NUMBER_OF_MEASUREMENTS - 1);
}
void OnOffSensorArduino::requestCurrentMeasurement()
{
    if (!_sensorInitCompleted)
        return;

    uint32_t now = millis();
    if (!isReadyForNextRead(now))
        return;

    if (isArrayFull())
        saveAverageMeasurement();

    _sensorValueArray[_currentSavingItemInArray++] = digitalRead(_signalPIN);
    _startReadMillis = millis();
}

float OnOffSensorArduino::getCurrentMeasurementByID(uint8_t id)
{
    if (!_sensorInitCompleted)
        return oos_nm::UNINITIALIZED_MEASUREMENT_VALUE;

    return _sensorValue;
}