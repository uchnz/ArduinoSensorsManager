#pragma once
#include <BaseSensor.h>

class OnOffSensorArduino : public BaseSensor
{
public:
    OnOffSensorArduino(const char *name, IIOSensor &io);
    double getCurrentMeasurementByID(uint8_t id = 0) override;
};