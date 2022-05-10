#ifndef __TESTS__
#include <LibPrintf.h>
#endif
#include "Arduino.h"

#include <DallasIOArduino.h>

DallasIOArduino::DallasIOArduino(DallasTemperature &dallas)
    : _dallas(dallas)
{
    _initCompleted = false;
}

bool DallasIOArduino::isInitCompleted()
{
    return _initCompleted;
}
bool DallasIOArduino::init()
{
    _dallas.begin();
    _initCompleted = true;

    return _initCompleted;
}

uint8_t DallasIOArduino::getTotalSensors()
{
    return _dallas.getDeviceCount();
}

double DallasIOArduino::read(uint8_t id)
{
    if (!isInitCompleted())
        return dallas_nm::UNINITIALIZED_SENSOR_VALUE;

    _dallas.requestTemperatures();
    return _dallas.getTempCByIndex(id);
}