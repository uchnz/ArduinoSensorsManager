#pragma once

class ITimer
{
public:
    virtual bool isReadyForNextRead() = 0;
    virtual void setReadingInterval(uint16_t interval) = 0;
};
