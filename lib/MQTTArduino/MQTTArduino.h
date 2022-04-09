#pragma once

#include <IMQTT.h>
#include <MQTT.h>
#include <EthernetENC.h>

typedef void (*clientCallbackPtr)(String &topic, String &payload);

class MQTTArduino : public IMQTT
{
private:
    EthernetClient _net;
    MQTTClient _clientMQTT;

public:
    bool begin(char *srvIP);
    void onMessage(clientCallbackPtr cb);
    void setKeepAlive(uint16_t keepAlive);
    bool connect();
    bool connected();
    bool loop();
    bool publish(char *topic, char *data);
    void subscribeToTopic(const char *topic);

    bool send(const char *data, const char *topic) override;
    bool receive() override;
};