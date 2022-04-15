#pragma once

#include <Arduino.h>

namespace mq7impl
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

// #define DEFAULT_READING_INTERVAL 3000
// enum PHASES
// {
//     NONE = 0,
//     HEATING = 59999, // phases should be different in milliseconds, otherwise
//     COOLING = 60000, // comparing different phases won't work for isInPhase()
//     READING = 30000
// };

class MQ7Impl
{
private:
    uint32_t _phaseStartedMls;
    mq7impl::PHASES _currentPhase;
    uint32_t _previusReadingMillis;
    uint16_t _readInterval;

    bool isOutsideMillisInterval(mq7impl::PHASES phase, uint32_t delta);

public:
    MQ7Impl();
    MQ7Impl(uint16_t ReadInterval);
    void resetOnInit(uint16_t ReadInterval);

    bool setPhase(mq7impl::PHASES phase);
    bool isInPhase(mq7impl::PHASES phase);
    bool isPhaseCompleted(mq7impl::PHASES phase);
    bool isTimeToReadMeasurement();
};
