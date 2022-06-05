#ifndef __TESTS__
#include <LibPrintf.h>
#endif

#include <IOActuator.h>

IOActuator::IOActuator(uint8_t pin)
{
    _signalPIN = pin;
    _initCompleted = false;
}

bool IOActuator::init()
{
    pinMode(_signalPIN, OUTPUT);
    _initCompleted = true;

    return _initCompleted;
}

bool IOActuator::write(uint8_t value)
{
    if (!_initCompleted)
        return false;
    if (value > 1)
        return false;

    if (0 == value)
        digitalWrite(_signalPIN, LOW);
    if (1 == value)
        digitalWrite(_signalPIN, HIGH);

    return true;
}
