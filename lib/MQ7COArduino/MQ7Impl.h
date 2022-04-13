#pragma once

#include <Arduino.h>

#define READING_INTERVAL 3000
enum PHASES
{
    NONE = 0,
    HEATING = 59999, // phases should be different in milliseconds, otherwise
    COOLING = 60000, // comparing different phases won't work for isInPhase()
    READING = 30000
};

class MQ7Impl
{
private:
    uint32_t _phaseStartedMls;
    PHASES _currentPhase;
    uint32_t _previusReadingMillis;

    bool isOutsideMillisInterval(PHASES phase, uint32_t delta);

public:
    MQ7Impl();

    bool setPhase(PHASES phase);
    bool isInPhase(PHASES phase);
    bool isPhaseCompleted(PHASES phase);
    bool isTimeToReadMeasurement();
};
