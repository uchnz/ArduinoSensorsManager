#include <BMP280Arduino.h>

BMP280Arduino::BMP280Arduino(iarduino_Pressure_BMP &bmp)
    : _bmp(bmp)
{
    _sensorInitCompleted = false;
    _startReadMillis = 0;
    _readingInterval = bmp_nm::DEFAULT_READING_INTERVAL;
    _temperatureAverage = _pressureAverage = _altitudeAverage = bmp_nm::UNINITIALIZED_MEASUREMENT_VALUE;
    _currentSavingItemInArray = 0;
    for (uint8_t i = 0; i < bmp_nm::NUMBER_OF_MEASUREMENTS; i++)
        _sensorTemperatureArray[i] = _sensorPressureArray[i] = _sensorAltitudeArray[i] = bmp_nm::UNINITIALIZED_MEASUREMENT_VALUE;
}

bool BMP280Arduino::init(uint16_t ReadingInterval)
{
    _sensorInitCompleted = _bmp.begin();
    _readingInterval = ReadingInterval;

    return _sensorInitCompleted;
}

uint8_t BMP280Arduino::getAddress()
{
    if (!_sensorInitCompleted)
        return 0;

    return 0x77; //_bmp.getAddress();
}

void BMP280Arduino::saveAverageMeasurement()
{
    _temperatureAverage = _pressureAverage = _altitudeAverage = 0;
    for (uint8_t i = 0; i < _currentSavingItemInArray; i++)
    {
        _temperatureAverage += _sensorTemperatureArray[i];
        _pressureAverage += _sensorPressureArray[i];
        _altitudeAverage += _sensorAltitudeArray[i];
    }
    _temperatureAverage = _temperatureAverage / _currentSavingItemInArray;
    _pressureAverage = _pressureAverage / _currentSavingItemInArray;
    _altitudeAverage = _altitudeAverage / _currentSavingItemInArray;
    _currentSavingItemInArray = 0;
}
bool BMP280Arduino::isReadyForNextRead(uint32_t now)
{
    return (now - _startReadMillis) >= _readingInterval;
}
bool BMP280Arduino::isArrayFull()
{
    return (_currentSavingItemInArray > bmp_nm::NUMBER_OF_MEASUREMENTS - 1);
}
void BMP280Arduino::requestCurrentMeasurement()
{
    if (!_sensorInitCompleted)
        return;

    uint32_t now = millis();
    if (!isReadyForNextRead(now))
        return;

    if (isArrayFull())
        saveAverageMeasurement();

    _bmp.read();
    _sensorTemperatureArray[_currentSavingItemInArray] = _bmp.temperature;
    _sensorPressureArray[_currentSavingItemInArray] = _bmp.pressure;
    _sensorAltitudeArray[_currentSavingItemInArray++] = _bmp.altitude;
    _startReadMillis = millis();
}

uint8_t BMP280Arduino::getNumberOfConnectedSensors()
{
    return bmp_nm::NUMBER_OF_SENSORS_ON_BUS;
}

float BMP280Arduino::getCurrentMeasurementByID(uint8_t id)
{
    if (!_sensorInitCompleted)
        return bmp_nm::UNINITIALIZED_MEASUREMENT_VALUE;

    if (bmp_nm::TEMPERATURE_ID == id)
        return _temperatureAverage;
    if (bmp_nm::PRESSURE_ID == id)
        return _pressureAverage;
    if (bmp_nm::ALTITUDE_ID == id)
        return _altitudeAverage;

    return bmp_nm::UNINITIALIZED_MEASUREMENT_VALUE;
}
