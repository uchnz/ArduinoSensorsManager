#include <MQ7COArduino.h>

MQ7COArduino::MQ7COArduino(const char *name, IIO &io)
    : BaseSensor(name, io)
{
}

bool MQ7COArduino::requestCurrentMeasurement()
{
    if (!BaseSensor::isInitialized())
        return false;

    if (!_timer)
        return false;

    if (!_timer->isReadyForNextRead())
        return false;

    if (BaseSensor::isArrayFull())
        BaseSensor::saveAverageMeasurement();

    // uint8_t num = _io.getTotalSensors();
    // for (uint8_t i = 0; i < num; i++)
    // {
    // double sensorValue = _io.read(i);
    // _measurements2DArray[i][_currentSavingItemInArray] = sensorValue;
    // }
    double sensorValue = _io.read(0);
    if (basesensor_nm::UNINITIALIZED_MEASUREMENT_VALUE == sensorValue)
        return false;

    _measurements2DArray[0][_currentSavingItemInArray] = sensorValue;
    _currentSavingItemInArray++;

    return true;
}
