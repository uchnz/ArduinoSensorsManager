#include <OnOffSensorArduino.h>

OnOffSensorArduino::OnOffSensorArduino(const char *name, IIOSensor &io)
    : BaseSensor(name, io)
{
}

double OnOffSensorArduino::getCurrentMeasurementByID(uint8_t id)
{
    return round(BaseSensor::getCurrentMeasurementByID(id));
}
