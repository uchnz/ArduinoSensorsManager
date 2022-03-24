#pragma once

#include <IMQTT.h>
#include <MQTT.h>
#include <EthernetENC.h>

namespace sensor_manager
{
    class MQTTArduino : public IMQTT
    {
    private:
        EthernetClient _net;
        MQTTClient _clientMQTT;

    public:
        bool begin(char *srvIP) override;
        void onMessage(MQTTClientCallback cb) override;
        void setKeepAlive(uint16_t keepAlive) override;
        bool connect() override;
        bool connected() override;
        bool loop() override;
        bool publish(char *topic, char *data) override;
        void subscribeToTopic(const char *topic) override;
        bool send(const char *data, const char *topic) override;
        //        void receive(char *topic, char *data) override;
        bool receive() override;
    };
}