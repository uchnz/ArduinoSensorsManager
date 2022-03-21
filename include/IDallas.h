#pragma once
#include <stdint.h>

namespace sensor_manager
{
    class IDallas
    {
    public:
        virtual ~IDallas() = default;
        virtual void init() = 0;
        virtual void setSensorsPrecision(int precision) = 0;
        virtual void requestCurrentTemperatures() = 0;
        virtual uint8_t getNumberOfSensors() = 0;
        virtual float getTemperatureByID(uint8_t id) = 0;
        virtual char *getStringAddressByID(uint8_t id) = 0;
        //        virtual bool updateTemperature() = 0;
    };
}