#include <SHT20Arduino.h>

SHT20Arduino::SHT20Arduino(iarduino_I2C_SHT &sht)
    : _sht(sht)
{
    _sensorInitCompleted = false;
    _startReadMillis = 0;
    _readingInterval = sht_nm::DEFAULT_READING_INTERVAL;
    _temperatureAverage = _humidityAverage = sht_nm::UNINITIALIZED_MEASUREMENT_VALUE;
    _currentSavingItemInArray = 0;
    for (uint8_t i = 0; i < sht_nm::NUMBER_OF_MEASUREMENTS; i++)
        _sensorTemperatureArray[i] = _sensorHumidityArray[i] = sht_nm::UNINITIALIZED_MEASUREMENT_VALUE;
}

void SHT20Arduino::initName(const char *name)
{
    int nameLengthWithNull = strlen(name) + 1;
    if (nameLengthWithNull > sht_nm::MAX_SENSOR_NAME)
        nameLengthWithNull = sht_nm::MAX_SENSOR_NAME - 1;

    memcpy(_sensorName, name, nameLengthWithNull);
}
bool SHT20Arduino::init(const char *name, uint16_t ReadingInterval)
{
    if ((!name) || (strlen(name) < 1))
        return false;

    memcpy(_name, name, strlen(name));
    _sensorInitCompleted = _sht.begin();
    _readingInterval = ReadingInterval;

    return _sensorInitCompleted;
}

uint8_t SHT20Arduino::getAddress()
{
    if (!_sensorInitCompleted)
        return 0;

    return _sht.getAddress();
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
bool SHT20Arduino::isReadyForNextRead(uint32_t now)
{
    return (now - _startReadMillis) >= _readingInterval;
}
bool SHT20Arduino::isArrayFull()
{
    return (_currentSavingItemInArray > sht_nm::NUMBER_OF_MEASUREMENTS - 1);
}
void SHT20Arduino::requestCurrentMeasurement()
{
    if (!_sensorInitCompleted)
        return;

    uint32_t now = millis();
    if (!isReadyForNextRead(now))
        return;

    if (isArrayFull())
        saveAverageMeasurement();

    _sensorTemperatureArray[_currentSavingItemInArray] = _sht.getTem();
    _sensorHumidityArray[_currentSavingItemInArray++] = _sht.getHum();
    _startReadMillis = millis();
}

uint8_t SHT20Arduino::getNumberOfConnectedSensors()
{
    return sht_nm::NUMBER_OF_SENSORS_ON_BUS;
}

double SHT20Arduino::getCurrentMeasurementByID(uint8_t id)
{
    if (!_sensorInitCompleted)
        return sht_nm::UNINITIALIZED_MEASUREMENT_VALUE;

    if (sht_nm::TEMPERATURE_ID == id)
        return _temperatureAverage;
    if (sht_nm::HUMIDITY_ID == id)
        return _humidityAverage;

    return sht_nm::UNINITIALIZED_MEASUREMENT_VALUE;
}

// uint8_t SHT20Arduino::getName(char *&name)
uint8_t SHT20Arduino::getName(char *name)
{
    return 0;
}