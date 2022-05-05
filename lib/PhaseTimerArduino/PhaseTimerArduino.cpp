#include <PhaseTimerArduino.h>

ThreePhaseTimerArduino::ThreePhaseTimerArduino()
{
    reset();
}

void ThreePhaseTimerArduino::reset()
{
    _phaseStartedMls = 0;
    _currentPhase = phasetimer_nm::NONE;
}

bool ThreePhaseTimerArduino::setPhase(phasetimer_nm::PHASES phase)
{
    _phaseStartedMls = millis();
    _currentPhase = phase;

    return true;
}

bool ThreePhaseTimerArduino::isInPhase(phasetimer_nm::PHASES phase)
{
    return _currentPhase == phase;
}

bool ThreePhaseTimerArduino::isOutsideMillisInterval(phasetimer_nm::PHASES phase, uint32_t delta)
{
    return phase <= delta;
}
bool ThreePhaseTimerArduino::isPhaseCompleted(phasetimer_nm::PHASES phase)
{
    uint32_t now = millis();
    uint32_t delta = now - _phaseStartedMls;

    return isOutsideMillisInterval(phase, delta);
}
