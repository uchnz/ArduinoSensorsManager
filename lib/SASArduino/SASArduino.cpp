#include <SASArduino.h>

void SASArduino::reset()
{
    _sensorValueAveraged = sas_nm::UNINITIALIZED_MEASUREMENT_VALUE;
    _currentSavingItemInArray = 0;
    for (uint8_t i = 0; i < sas_nm::NUMBER_OF_MEASUREMENTS; i++)
        _sensorValueArray[i] = sas_nm::UNINITIALIZED_MEASUREMENT_VALUE;
}
SASArduino::SASArduino(const char *name, IIO &io)
    : BaseSensor(name, io)
{
    this->reset();
}

bool SASArduino::init(ITimer *timer)
{

    if (!BaseSensor::init(timer))
        return false;

    return true;
}

uint8_t SASArduino::getNumberOfConnectedSensors()
{
    return sas_nm::NUMBER_OF_SENSORS_ON_BUS;
}

void SASArduino::saveAverageMeasurement()
{
    _sensorValueAveraged = 0;
    for (uint8_t i = 0; i < sas_nm::NUMBER_OF_MEASUREMENTS; i++)
        _sensorValueAveraged += _sensorValueArray[i];

    _sensorValueAveraged = _sensorValueAveraged / sas_nm::NUMBER_OF_MEASUREMENTS;
    _currentSavingItemInArray = 0;
}
bool SASArduino::isArrayFull()
{
    return (_currentSavingItemInArray > sas_nm::NUMBER_OF_MEASUREMENTS - 1);
}
bool SASArduino::requestCurrentMeasurement()
{
    if (!BaseSensor::requestCurrentMeasurement())
        return false;

    if (isArrayFull())
        saveAverageMeasurement();

    _sensorValueArray[_currentSavingItemInArray++] = _io.read();

    return true;
}

double SASArduino::getCurrentMeasurementByID(uint8_t id)
{
    if (basesensor_nm::UNINITIALIZED_MEASUREMENT_VALUE == BaseSensor::getCurrentMeasurementByID())
        return basesensor_nm::UNINITIALIZED_MEASUREMENT_VALUE;

    return _sensorValueAveraged;
}
