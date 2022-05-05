#pragma once
#include <Arduino.h>
#include <IPhaseTimer.h>

class ThreePhaseTimerArduino : public IPhaseTimer
{
private:
    uint32_t _phaseStartedMls;
    phasetimer_nm::PHASES _currentPhase;

    bool isOutsideMillisInterval(phasetimer_nm::PHASES phase, uint32_t delta);

public:
    ThreePhaseTimerArduino();

    void reset();

    // IPhaseTimer
    bool setPhase(phasetimer_nm::PHASES phase) override;
    bool isInPhase(phasetimer_nm::PHASES phase) override;
    bool isPhaseCompleted(phasetimer_nm::PHASES phase) override;
};
