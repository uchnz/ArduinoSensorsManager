#include <TimerArduino.h>

TimerArduino::TimerArduino(uint16_t readingInterval)
{
    _startReadMillis = 0;
    _readingInterval = readingInterval > 0 ? readingInterval : timer_nm::DEFAULT_READING_INTERVAL;
}

bool TimerArduino::isReadyForNextRead()
{
    uint32_t now = millis();
    bool isTime = (now - _startReadMillis) >= _readingInterval;
    if (isTime)
        _startReadMillis = now;

    return isTime;
}

void TimerArduino::setReadingInterval(uint16_t readingInterval)
{
    _readingInterval = readingInterval;
    if (readingInterval < 1)
        _readingInterval = timer_nm::DEFAULT_READING_INTERVAL;
}