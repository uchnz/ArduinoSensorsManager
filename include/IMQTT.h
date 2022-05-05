#pragma once
#include <stdint.h>

class IMQTT
{
public:
    virtual ~IMQTT() = default;

    virtual bool send(const char *data, const char *topic) = 0;
    virtual bool receive() = 0;
};
