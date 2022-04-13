#include <MQ7Impl.h>

MQ7Impl::MQ7Impl()
{
    _phaseStartedMls = 0;
    _currentPhase = NONE;
    _previusReadingMillis = 0;
}

bool MQ7Impl::setPhase(PHASES phase)
{
    _phaseStartedMls = millis();
    _currentPhase = phase;

    return true;
}

bool MQ7Impl::isInPhase(PHASES phase)
{
    return _currentPhase == phase;
}

bool MQ7Impl::isOutsideMillisInterval(PHASES phase, uint32_t delta)
{
    return phase <= delta;
}
bool MQ7Impl::isPhaseCompleted(PHASES phase)
{
    uint32_t now = millis();
    uint32_t delta = now - _phaseStartedMls;

    return isOutsideMillisInterval(phase, delta);
}

bool MQ7Impl::isTimeToReadMeasurement()
{
    uint32_t now = millis();
    if (now - _previusReadingMillis < READING_INTERVAL)
        return false;

    _previusReadingMillis = now;

    return true;
}
