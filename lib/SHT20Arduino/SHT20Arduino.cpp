#include <SHT20Arduino.h>

void SHT20Arduino::reset()
{
    _temperatureAverage = _humidityAverage = basesensor_nm::UNINITIALIZED_MEASUREMENT_VALUE;
    _currentSavingItemInArray = 0;
    for (uint8_t i = 0; i < sht_nm::NUMBER_OF_MEASUREMENTS; i++)
        _sensorTemperatureArray[i] = _sensorHumidityArray[i] = basesensor_nm::UNINITIALIZED_MEASUREMENT_VALUE;
}
SHT20Arduino::SHT20Arduino(const char *name, IIO &io)
    : BaseSensor(name, io)
{
    this->reset();
}

bool SHT20Arduino::init(ITimer *timer)
{
    if (!BaseSensor::init(timer))
        return false;

    return true;
}

uint8_t SHT20Arduino::getNumberOfConnectedSensors()
{
    return sht_nm::NUMBER_OF_SENSORS_ON_BUS;
}

void SHT20Arduino::saveAverageMeasurement()
{
    _temperatureAverage = _humidityAverage = 0;
    for (uint8_t i = 0; i < _currentSavingItemInArray; i++)
    {
        _temperatureAverage += _sensorTemperatureArray[i];
        _humidityAverage += _sensorHumidityArray[i];
    }
    _temperatureAverage = _temperatureAverage / _currentSavingItemInArray;
    _humidityAverage = _humidityAverage / _currentSavingItemInArray;
    _currentSavingItemInArray = 0;
}
bool SHT20Arduino::isArrayFull()
{
    return (_currentSavingItemInArray > sht_nm::NUMBER_OF_MEASUREMENTS - 1);
}
bool SHT20Arduino::requestCurrentMeasurement()
{
    if (!BaseSensor::requestCurrentMeasurement())
        return false;

    if (isArrayFull())
        saveAverageMeasurement();

    _sensorTemperatureArray[_currentSavingItemInArray] = _io.read(0);
    _sensorHumidityArray[_currentSavingItemInArray++] = _io.read(1);

    return true;
}

double SHT20Arduino::getCurrentMeasurementByID(uint8_t id)
{
    if (basesensor_nm::UNINITIALIZED_MEASUREMENT_VALUE == BaseSensor::getCurrentMeasurementByID())
        return basesensor_nm::UNINITIALIZED_MEASUREMENT_VALUE;

    if (sht_nm::TEMPERATURE_ID == id)
        return _temperatureAverage;
    if (sht_nm::HUMIDITY_ID == id)
        return _humidityAverage;

    return basesensor_nm::UNINITIALIZED_MEASUREMENT_VALUE;
}
