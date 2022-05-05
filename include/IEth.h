#pragma once
#include <stdint.h>

class IEth
{
public:
    virtual ~IEth() = default;
    virtual bool connect(uint8_t (&mac)[6]) = 0;
};
