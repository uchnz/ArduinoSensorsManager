#pragma once
#include <stdint.h>

namespace sensor_manager
{
    typedef void (*MQTTClientCallback)(char *topic, char *payload);
    typedef void (*MQTTClientCallback)(char *topic, char *payload);

    // void messageReceived(MQTTClient *client, char topic[], char bytes[], int length){}
    //  std::function<void(MQTTClient *client, char topic[], char bytes[], int length)>;
    ;

    class IMQTT
    {
    public:
        virtual ~IMQTT() = default;

        virtual bool begin(char *srvIP) = 0;
        virtual void onMessage(MQTTClientCallback cb) = 0;
        virtual void setKeepAlive(uint16_t keepAlive) = 0;
        virtual bool connect() = 0;
        virtual bool connected() = 0;
        virtual bool loop() = 0;
        virtual bool publish(char *topic, char *data) = 0;
        virtual void subscribeToTopic(const char *topic) = 0;
        virtual bool send(const char *data, const char *topic) = 0;
        virtual bool receive() = 0;
    };
}