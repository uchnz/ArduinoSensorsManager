#include <BMP280IOArduino.h>

BMP280IOArduino::BMP280IOArduino(iarduino_Pressure_BMP &bmp)
    : _bmp(bmp)
{
    _initCompleted = false;
}

bool BMP280IOArduino::isInitCompleted()
{
    return _initCompleted;
}
bool BMP280IOArduino::init()
{
    _initCompleted = _bmp.begin();

    return _initCompleted;
}

uint8_t BMP280IOArduino::getTotalSensors()
{
    return bmpio_nm::TOTAL_SENSORS_ON_BUS;
}

double BMP280IOArduino::read(uint8_t id)
{
    if (!isInitCompleted())
        return bmpio_nm::UNINITIALIZED_SENSOR_VALUE;

    if (id >= bmpio_nm::TOTAL_SENSORS_ON_BUS)
        return bmpio_nm::UNINITIALIZED_SENSOR_VALUE;

    if (!_bmp.read())
        return bmpio_nm::UNINITIALIZED_SENSOR_VALUE;

    if (bmpio_nm::TEMPERATURE_ID == id)
        return _bmp.temperature;

    if (bmpio_nm::PRESSURE_ID == id)
        return _bmp.pressure;

    if (bmpio_nm::ALTITUDE_ID == id)
        return _bmp.altitude;

    return bmpio_nm::UNINITIALIZED_SENSOR_VALUE;
}