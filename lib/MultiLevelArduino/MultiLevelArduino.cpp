#include <MultiLevelArduino.h>

MultiLevelArduino::MultiLevelArduino(const char *name, IIOSensor &io)
{
}

MultiLevelArduino::MultiLevelArduino(const char *name, IIOSensor **io, uint8_t ioNum)
{
    _totalIOs = ioNum;
}

uint8_t MultiLevelArduino::getNumberOfConnectedSensors()
{
    return _totalIOs;
}

bool MultiLevelArduino::requestCurrentMeasurement()
{
    return false;
}
double MultiLevelArduino::getCurrentMeasurementByID(uint8_t id)
{
    return 0;
}
uint8_t MultiLevelArduino::getName(char *name)
{
    return 0;
}

MultiPortIOArduino::MultiPortIOArduino(IIOSensor **ios, uint8_t num)
{
    _totalSensors = num;
}

bool MultiPortIOArduino::init()
{
    return false;
}
double MultiPortIOArduino::read(uint8_t id)
{
    return 0;
}

uint8_t MultiPortIOArduino::getTotalSensors()
{
    return _totalSensors;
}
