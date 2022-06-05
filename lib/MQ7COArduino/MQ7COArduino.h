#pragma once
#include <BaseSensor.h>

class MQ7COArduino : public BaseSensor
{
public:
    MQ7COArduino(const char *name, IIOSensor &io);
    bool requestCurrentMeasurement() override;
};
