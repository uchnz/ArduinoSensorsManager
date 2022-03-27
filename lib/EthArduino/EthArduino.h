#pragma once

#include <IEth.h>

// namespace sensor_manager
// {
class EthArduino : public IEth
{
public:
    bool connect(uint8_t (&mac)[6]) override;
};
// }