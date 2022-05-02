#include <SHT20Arduino.h>

SHT20Arduino::SHT20Arduino(iarduino_I2C_SHT &sht, const char *name)
    : _sht(sht)
{
    _sensorName[0] = 0;
    _sensorInitCompleted = false;
    _startReadMillis = 0;
    _readingInterval = sht_nm::DEFAULT_READING_INTERVAL;
    _temperatureAverage = _humidityAverage = sht_nm::UNINITIALIZED_MEASUREMENT_VALUE;
    _currentSavingItemInArray = 0;
    for (uint8_t i = 0; i < sht_nm::NUMBER_OF_MEASUREMENTS; i++)
        _sensorTemperatureArray[i] = _sensorHumidityArray[i] = sht_nm::UNINITIALIZED_MEASUREMENT_VALUE;
    setName(name);
}

// void SHT20Arduino::initName(const char *name)
// {
//     int nameLengthWithNull = strlen(name) + 1;
//     if (nameLengthWithNull > sht_nm::MAX_SENSOR_NAME)
//         nameLengthWithNull = sht_nm::MAX_SENSOR_NAME - 1;

//     memcpy(_sensorName, name, nameLengthWithNull);
// }
bool SHT20Arduino::setName(const char *name)
{
    if (!name || (0 == strlen(name)))
        return false;

    int nameLengthWithNull = strlen(name) + 1;
    if (nameLengthWithNull > sht_nm::MAX_SENSOR_NAME)
        nameLengthWithNull = sht_nm::MAX_SENSOR_NAME - 1;
    memcpy(_sensorName, name, nameLengthWithNull);

    return true;
}
bool SHT20Arduino::init(uint16_t ReadingInterval)
{
    if (0 == strlen(_sensorName))
        return false;
    // if ((!name) || (strlen(name) < 1))
    //     return false;

    // memcpy(_name, name, strlen(name));
    _readingInterval = ReadingInterval;
    _sensorInitCompleted = _sht.begin();

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
bool SHT20Arduino::requestCurrentMeasurement()
{
    if (!_sensorInitCompleted)
        return false;

    uint32_t now = millis();
    if (!isReadyForNextRead(now))
        return false;

    if (isArrayFull())
        saveAverageMeasurement();

    _sensorTemperatureArray[_currentSavingItemInArray] = _sht.getTem();
    _sensorHumidityArray[_currentSavingItemInArray++] = _sht.getHum();
    _startReadMillis = millis();

    return true;
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
    if (!_sensorInitCompleted)
        return 0;

    int nameLength = strlen(_sensorName);
    memcpy(name, _sensorName, nameLength + 1);
    return nameLength;
}