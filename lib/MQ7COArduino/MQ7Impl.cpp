#include <MQ7Impl.h>

void MQ7Impl::resetOnInit(uint16_t ReadInterval)
{
    _phaseStartedMls = 0;
    _currentPhase = mq7impl::NONE;
    _previusReadingMillis = 0;
    _readInterval = ReadInterval;
}
MQ7Impl::MQ7Impl()
{
    resetOnInit(mq7impl::DEFAULT_READING_INTERVAL);
}
MQ7Impl::MQ7Impl(uint16_t ReadInterval)
{
    resetOnInit(ReadInterval);
}

bool MQ7Impl::setPhase(mq7impl::PHASES phase)
{
    _phaseStartedMls = millis();
    _currentPhase = phase;

    return true;
}

bool MQ7Impl::isInPhase(mq7impl::PHASES phase)
{
    return _currentPhase == phase;
}

bool MQ7Impl::isOutsideMillisInterval(mq7impl::PHASES phase, uint32_t delta)
{
    return phase <= delta;
}
bool MQ7Impl::isPhaseCompleted(mq7impl::PHASES phase)
{
    uint32_t now = millis();
    uint32_t delta = now - _phaseStartedMls;

    return isOutsideMillisInterval(phase, delta);
}

bool MQ7Impl::isTimeToReadMeasurement()
{
    if (!isInPhase(mq7impl::READING))
        return false;

    uint32_t now = millis();
    if (now - _previusReadingMillis < _readInterval)
        return false;

    _previusReadingMillis = now;

    return true;
}
