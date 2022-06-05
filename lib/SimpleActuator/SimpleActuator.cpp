#ifndef __TESTS__
#include <LibPrintf.h>
#endif

#include <SimpleActuator.h>
#include <string.h>

bool SimpleActuator::saveName(const char *name)
{
    if ((!name) || (0 == strlen(name)))
        return false;

    int nameLength = strlen(name);
    if (nameLength >= simpleactuator_nm::MAX_SENSOR_NAME)
        nameLength = simpleactuator_nm::MAX_SENSOR_NAME - 1;
    memcpy(_sensorName, name, nameLength);
    _sensorName[nameLength] = 0;

    return true;
}
SimpleActuator::SimpleActuator(const char *name, IIOActuator &io)
    : _io(io)
{
    _sensorName[0] = 0;
    saveName(name);
    strcpy(_actuatorState, simpleactuator_nm::UNKNOWN_STATE);
}

uint8_t SimpleActuator::getName(char *name)
{
    if (!name)
        return 0;

    memcpy(name, _sensorName, strlen(_sensorName) + 1);
    return strlen(_sensorName);
}
bool SimpleActuator::setName(const char *name)
{
    if ((!name) || (0 == strlen(name)))
        return false;

    int nameLength = strlen(name);
    if (nameLength >= simpleactuator_nm::MAX_SENSOR_NAME)
        nameLength = simpleactuator_nm::MAX_SENSOR_NAME - 1;
    memcpy(_sensorName, name, nameLength);
    _sensorName[nameLength] = 0;

    return true;
}

bool SimpleActuator::init()
{
    if (0 == strlen(_sensorName))
        return false;

    _actuatorInitCompleted = _io.init();
    if (_actuatorState)
        strcpy(_actuatorState, simpleactuator_nm::OFF_STATE_COMMAND);

    return _actuatorInitCompleted;
}

bool SimpleActuator::changeState(uint8_t state)
{
    bool result = _io.write(state);
    if (!result)
        return false;

    if (simpleactuator_nm::OFF == state)
        strcpy(_actuatorState, simpleactuator_nm::OFF_STATE_COMMAND);
    if (simpleactuator_nm::ON == state)
        strcpy(_actuatorState, simpleactuator_nm::ON_STATE_COMMAND);

    return result;
}
bool SimpleActuator::execute(const char *command)
{
    if (!command)
        return false;
    if (!_actuatorInitCompleted)
        return false;

    bool result = false;
    if (0 == strcmp(simpleactuator_nm::OFF_STATE_COMMAND, command))
        result = changeState(simpleactuator_nm::OFF);
    if (0 == strcmp(simpleactuator_nm::ON_STATE_COMMAND, command))
        result = changeState(simpleactuator_nm::ON);

    return result;
}

uint8_t SimpleActuator::getState(char *state)
{
    if (!state)
        return 0;

    strcpy(state, _actuatorState);

    return strlen(state);
}