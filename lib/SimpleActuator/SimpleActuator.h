#pragma once
#include <IActuator.h>
#include <Arduino.h>
#include <IIOActuator.h>
//#include <IOArduino.h>
//#include <TimerArduino.h>

namespace simpleactuator_nm
{
    const uint8_t MAX_SENSOR_NAME = 30;
    const uint8_t MAX_STATE_LENGTH = 8;
    const char UNKNOWN_STATE[] = "UNKNOWN";
    const char OFF_STATE_COMMAND[] = "OFF";
    const char ON_STATE_COMMAND[] = "ON";

    enum SWITCH_STATES
    {
        OFF,
        ON
    };
}

class SimpleActuator : public IActuator
{
private:
    char _sensorName[simpleactuator_nm::MAX_SENSOR_NAME];
    bool _actuatorInitCompleted;
    char _actuatorState[simpleactuator_nm::MAX_STATE_LENGTH];

    bool saveName(const char *name);
    bool changeState(uint8_t state);

protected:
    IIOActuator &_io;

public:
    SimpleActuator(const char *name, IIOActuator &io);

    bool setName(const char *name);
    bool init();

    // IActuator
    virtual uint8_t getName(char *name) override;
    virtual bool execute(const char *command) override;
    virtual uint8_t getState(char *state) override;
};