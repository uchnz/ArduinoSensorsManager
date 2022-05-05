#include <MQ7COIOArduino.h>

MQ7COIOArduino::MQ7COIOArduino(uint8_t pinSignal, uint8_t pinHeater, IPhaseTimer &timer)
    : _timer(timer)
{
    _signalPIN = pinSignal;
    _heaterPIN = pinHeater;
    _initCompleted = false;
}

bool MQ7COIOArduino::isInitCompleted()
{
    return _initCompleted;
}
bool MQ7COIOArduino::init()
{
    if (_signalPIN == _heaterPIN)
        return false;

    pinMode(_signalPIN, INPUT);
    pinMode(_heaterPIN, OUTPUT);
    _timer.setPhase(phasetimer_nm::HEATING);
    setHeaterVoltageForPhase(mq7coio_nm::HIGH_5_0);
    _initCompleted = true;

    return _initCompleted;
}

uint8_t MQ7COIOArduino::getTotalSensors()
{
    return mq7coio_nm::TOTAL_SENSORS_ON_BUS;
}

void MQ7COIOArduino::setHeaterVoltageForPhase(mq7coio_nm::VOLTAGE voltage)
{
    analogWrite(_heaterPIN, voltage);
}
double MQ7COIOArduino::read(uint8_t id)
{
    if (!isInitCompleted())
        return mq7coio_nm::UNINITIALIZED_SENSOR_VALUE;

    if (_timer.isInPhase(phasetimer_nm::HEATING))
    {
        if (!_timer.isPhaseCompleted(phasetimer_nm::HEATING))
            return mq7coio_nm::UNINITIALIZED_SENSOR_VALUE;

        _timer.setPhase(phasetimer_nm::COOLING);
        setHeaterVoltageForPhase(mq7coio_nm::LOW_1_4);
    }
    if (_timer.isInPhase(phasetimer_nm::COOLING))
    {
        if (!_timer.isPhaseCompleted(phasetimer_nm::COOLING))
            return mq7coio_nm::UNINITIALIZED_SENSOR_VALUE;

        _timer.setPhase(phasetimer_nm::READING);
    }
    if (_timer.isInPhase(phasetimer_nm::READING))
    {
        if (!_timer.isPhaseCompleted(phasetimer_nm::READING))
            return (double)analogRead(_signalPIN);
    }
    _timer.setPhase(phasetimer_nm::HEATING);
    setHeaterVoltageForPhase(mq7coio_nm::HIGH_5_0);

    return mq7coio_nm::UNINITIALIZED_SENSOR_VALUE;
}
