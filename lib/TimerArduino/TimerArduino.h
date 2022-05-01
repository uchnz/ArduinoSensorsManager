#pragma once
#include <Arduino.h>
#include <ITimer.h>

namespace timer_nm
{
    const uint16_t DEFAULT_READING_INTERVAL = 3000;
}

class TimerArduino : public ITimer
{
private:
    uint32_t _startReadMillis;
    uint16_t _readingInterval;

public:
    TimerArduino(uint16_t readingInterval = 3000);

    bool isReadyForNextRead() override;
    virtual void setReadingInterval(uint16_t readingInterval) override;
};
