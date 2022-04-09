#pragma once

#include <IEth.h>

class EthArduino : public IEth
{
public:
    bool connect(uint8_t (&mac)[6]) override;
};
