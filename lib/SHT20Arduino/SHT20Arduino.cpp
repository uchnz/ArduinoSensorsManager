#include <SHT20Arduino.h>

void SHT20Arduino::freeMemory()
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
SHT20Arduino::~SHT20Arduino()
{
    freeMemory();
}

void SHT20Arduino::createMeasurementsNewArrays()
{
    uint8_t num = _io.getTotalSensors();
    _averageMeasurementsArray = new double[num];
    _measurements2DArray = new double *[num];
    for (uint8_t i = 0; i < num; i++)
        _measurements2DArray[i] = new double[sht_nm::NUMBER_OF_MEASUREMENTS];
}

SHT20Arduino::SHT20Arduino(const char *name, IIO &io)
    : BaseSensor(name, io)
{
    _currentSavingItemInArray = 0;
    _averageMeasurementsArray = nullptr;
    _measurements2DArray = nullptr;
}

void SHT20Arduino::reset()
{
    _currentSavingItemInArray = 0;
    uint8_t num = _io.getTotalSensors();
    for (uint8_t i = 0; i < num; i++)
    {
        _averageMeasurementsArray[i] = basesensor_nm::UNINITIALIZED_MEASUREMENT_VALUE;
        for (uint8_t j = 0; j < sht_nm::NUMBER_OF_MEASUREMENTS; j++)
            _measurements2DArray[i][j] = basesensor_nm::UNINITIALIZED_MEASUREMENT_VALUE;
    }
}
bool SHT20Arduino::init(ITimer *timer)
{
    if (!BaseSensor::init(timer))
        return false;

    freeMemory();
    createMeasurementsNewArrays();
    reset();

    return true;
}

uint8_t SHT20Arduino::getNumberOfConnectedSensors()
{
    return _io.getTotalSensors();
}

void SHT20Arduino::saveAverageMeasurement()
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

    uint8_t num = _io.getTotalSensors();
    for (uint8_t i = 0; i < num; i++)
        _measurements2DArray[i][_currentSavingItemInArray] = _io.read(i);

    _currentSavingItemInArray++;

    return true;
}

double SHT20Arduino::getCurrentMeasurementByID(uint8_t id)
{
    if (basesensor_nm::UNINITIALIZED_MEASUREMENT_VALUE == BaseSensor::getCurrentMeasurementByID())
        return basesensor_nm::UNINITIALIZED_MEASUREMENT_VALUE;

    uint8_t num = _io.getTotalSensors();
    if (!num)
        return basesensor_nm::UNINITIALIZED_MEASUREMENT_VALUE;

    if (id >= num)
        return basesensor_nm::UNINITIALIZED_MEASUREMENT_VALUE;

    return _averageMeasurementsArray[id];
}
