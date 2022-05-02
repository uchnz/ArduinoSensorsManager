#include <SHTIOArduino.h>

SHTIOArduino::SHTIOArduino(iarduino_I2C_SHT &sht)
    : _sht(sht)
{
    _initCompleted = false;
}

bool SHTIOArduino::isInitCompleted()
{
    return _initCompleted;
}
bool SHTIOArduino::init()
{
    _initCompleted = _sht.begin();

    return _initCompleted;
}

uint8_t SHTIOArduino::getTotalSensors()
{
    return shtio_nm::TOTAL_SENSORS_ON_BUS;
}

double SHTIOArduino::read(uint8_t id)
{
    if (!isInitCompleted())
        return shtio_nm::UNINITIALIZED_SENSOR_VALUE;

    if (shtio_nm::TEMPERATURE_ID == id)
        return _sht.getTem();

    if (shtio_nm::HUMIDITY_ID == id)
        return _sht.getHum();

    return shtio_nm::UNINITIALIZED_SENSOR_VALUE;
}