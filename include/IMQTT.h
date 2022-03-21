#pragma once
#include <stdint.h>

namespace sensor_manager
{
    typedef void (*MQTTClientCallback)(char *topic, char *payload);
    
    class IMQTT
    {
        public:
            virtual ~IMQTT() = default;

            virtual bool begin(char *srvIP) = 0;
            virtual bool onMessage(MQTTClientCallback cb) = 0;
            virtual void setKeepAlive(uint16_t keepAlive) = 0;
            virtual bool connect() = 0;
            virtual bool connected() = 0;
            virtual bool loop() = 0;
            virtual bool publish(char *topic, char *data) = 0;
    };
}