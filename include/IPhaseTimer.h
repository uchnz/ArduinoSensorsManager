#pragma once

namespace phasetimer_nm
{
    const uint16_t DEFAULT_READING_INTERVAL = 3000;
    enum PHASES
    {
        NONE = 0,
        HEATING = 59999, // phases should be different in milliseconds, otherwise
        COOLING = 60000, // comparing different phases won't work for isInPhase()
        READING = 30000
    };
}

class IPhaseTimer
{
public:
    virtual bool setPhase(phasetimer_nm::PHASES phase) = 0;
    virtual bool isInPhase(phasetimer_nm::PHASES phase) = 0;
    virtual bool isPhaseCompleted(phasetimer_nm::PHASES phase) = 0;
};
