#pragma once
#include <BaseSensor.h>

class OnOffSensorArduino : public BaseSensor
{
public:
    OnOffSensorArduino(const char *name, IIO &io);
    double getCurrentMeasurementByID(uint8_t id = 0) override;
};