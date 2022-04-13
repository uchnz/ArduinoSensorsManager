#pragma once

// #include <MQ7COArduino.h>
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

// MQ7Impl::MQ7Impl()
// {
//     _phaseStartedMls = 0;
//     _currentPhase = NONE;
//     _previusReadingMillis = 0;
// }

// bool MQ7Impl::setPhase(PHASES phase)
// {
//     _phaseStartedMls = millis();
//     _currentPhase = phase;

//     return true;
// }

// bool MQ7Impl::isInPhase(PHASES phase)
// {
//     return _currentPhase == phase;
// }

// bool MQ7Impl::isOutsideMillisInterval(PHASES phase, uint32_t delta)
// {
//     return phase <= delta;
// }
// bool MQ7Impl::isPhaseCompleted(PHASES phase)
// {
//     uint32_t now = millis();
//     uint32_t delta = now - _phaseStartedMls;

//     return isOutsideMillisInterval(phase, delta);
// }

// bool MQ7Impl::isTimeToReadMeasurement()
// {
//     uint32_t now = millis();
//     if (now - _previusReadingMillis < READING_INTERVAL)
//         return false;

//     _previusReadingMillis = now;

//     return true;
// }
