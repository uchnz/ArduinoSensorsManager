#include <BaseSensor.h>
#include <string.h>
#include <iostream>

void BaseSensor::reset()
{
    _sensorName[0] = 0;
    _sensorInitCompleted = false;
}
bool BaseSensor::saveName(const char *name)
{
    if ((!name) || (0 == strlen(name)))
        return false;

    int nameLength = strlen(name);
    if (nameLength >= basesensor_nm::MAX_SENSOR_NAME)
        nameLength = basesensor_nm::MAX_SENSOR_NAME - 1;
    memcpy(_sensorName, name, nameLength);
    _sensorName[nameLength] = 0;

    return true;
}
BaseSensor::BaseSensor(const char *name, IIO &io)
    : _io(io)
{
    reset();
    saveName(name);
}

uint8_t BaseSensor::getName(char *name)
{
    if (!name)
        return 0;

    memcpy(name, _sensorName, strlen(_sensorName) + 1);
    return strlen(_sensorName);
}

bool BaseSensor::setName(const char *name)
{
    if (!saveName(name))
        return false;

    return true;
}

bool BaseSensor::init(ITimer *timer)
{
    if (0 == strlen(_sensorName))
        return false;

    if (!timer)
        return false;

    _timer = timer;
    _sensorInitCompleted = true;

    return true;
}

bool BaseSensor::isInitialized()
{
    return _sensorInitCompleted;
}

uint8_t BaseSensor::getNumberOfConnectedSensors()
{
    if (!_sensorInitCompleted)
        return basesensor_nm::NOT_INITIALIZED_SENSOR;

    return 0;
}

bool BaseSensor::requestCurrentMeasurement()
{
    if (!isInitialized())
        return false;

    if (!_timer)
        return false;

    if (!_timer->isReadyForNextRead())
        return false;

    return true;
}

double BaseSensor::getCurrentMeasurementByID(uint8_t id)
{
    if (!isInitialized())
        return basesensor_nm::UNINITIALIZED_MEASUREMENT_VALUE;

    return 0;
}
