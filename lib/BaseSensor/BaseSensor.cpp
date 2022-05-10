#ifndef __TESTS__
#include <LibPrintf.h>
#endif

#include <BaseSensor.h>
#include <string.h>

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
    _currentSavingItemInArray = 0;
    _averageMeasurementsArray = nullptr;
    _measurements2DArray = nullptr;
    _sensorName[0] = 0;
    saveName(name);
}

void BaseSensor::freeMemory()
{
    if (_averageMeasurementsArray)
        delete _averageMeasurementsArray;

    if (_measurements2DArray)
    {
        uint8_t num = _io.getTotalSensors();
        for (uint8_t i = 0; i < num; i++)
            if (_measurements2DArray[i])
                delete[] _measurements2DArray[i];
        delete[] _measurements2DArray;
    }
}
BaseSensor::~BaseSensor()
{
    _timer = nullptr;
    freeMemory();
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

void BaseSensor::createMeasurementsNewArrays()
{
    uint8_t num = _io.getTotalSensors();
    _averageMeasurementsArray = new double[num];
    _measurements2DArray = new double *[num];
    for (uint8_t i = 0; i < num; i++)
        _measurements2DArray[i] = new double[basesensor_nm::NUMBER_OF_MEASUREMENTS];
}
void BaseSensor::reset()
{
    _sensorInitCompleted = false;
    _currentSavingItemInArray = 0;

    uint8_t num = _io.getTotalSensors();
    for (uint8_t i = 0; i < num; i++)
    {
        _averageMeasurementsArray[i] = basesensor_nm::UNINITIALIZED_MEASUREMENT_VALUE;
        for (uint8_t j = 0; j < basesensor_nm::NUMBER_OF_MEASUREMENTS; j++)
            _measurements2DArray[i][j] = basesensor_nm::UNINITIALIZED_MEASUREMENT_VALUE;
    }
}
bool BaseSensor::init(ITimer *timer)
{
    if (0 == strlen(_sensorName))
        return false;

    if (!timer)
        return false;
    _timer = timer;

    bool sensorInitStatus = _io.init();
    freeMemory();
    createMeasurementsNewArrays();
    reset();

    _sensorInitCompleted = sensorInitStatus;
    return _sensorInitCompleted;
}

bool BaseSensor::isInitialized()
{
    return _sensorInitCompleted;
}

uint8_t BaseSensor::getNumberOfConnectedSensors()
{
    if (!_sensorInitCompleted)
        return basesensor_nm::NOT_INITIALIZED_SENSOR;

    return _io.getTotalSensors();
}

void BaseSensor::saveAverageMeasurement()
{
    uint8_t num = _io.getTotalSensors();
    for (uint8_t i = 0; i < num; i++)
        _averageMeasurementsArray[i] = 0;

    for (uint8_t i = 0; i < num; i++)
        for (uint8_t j = 0; j < _currentSavingItemInArray; j++)
            _averageMeasurementsArray[i] += _measurements2DArray[i][j];

    for (uint8_t i = 0; i < num; i++)
        _averageMeasurementsArray[i] = _averageMeasurementsArray[i] / _currentSavingItemInArray;

    _currentSavingItemInArray = 0;
}
bool BaseSensor::isArrayFull()
{
    return (_currentSavingItemInArray > basesensor_nm::NUMBER_OF_MEASUREMENTS - 1);
}
bool BaseSensor::requestCurrentMeasurement()
{
    if (!isInitialized())
        return false;

    if (!_timer)
        return false;

    if (!_timer->isReadyForNextRead())
        return false;

    if (isArrayFull())
        saveAverageMeasurement();

    uint8_t num = _io.getTotalSensors();
    for (uint8_t i = 0; i < num; i++)
        _measurements2DArray[i][_currentSavingItemInArray] = _io.read(i);

    _currentSavingItemInArray++;

    return true;
}

double BaseSensor::getCurrentMeasurementByID(uint8_t id)
{
    if (!isInitialized())
        return basesensor_nm::NOT_INITIALIZED_SENSOR;

    uint8_t num = _io.getTotalSensors();
    if (!num)
        return basesensor_nm::NOT_INITIALIZED_SENSOR;

    if (id >= num)
        return basesensor_nm::UNINITIALIZED_MEASUREMENT_VALUE;

    return _averageMeasurementsArray[id];
}
