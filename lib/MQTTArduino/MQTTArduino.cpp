#include <MQTTArduino.h>

namespace sensor_manager
{
    bool MQTTArduino::begin(char *srvIP)
    {
        if (nullptr == srvIP)
            return false;
        IPAddress ip;
        ip.fromString(srvIP);
        Serial.print("Initializing client with MQTT broker IP: ");
        Serial.println(srvIP);

        _clientMQTT.begin(ip, _net);

        return true;
    }

    void MQTTArduino::setKeepAlive(uint16_t keepAlive)
    {
        _clientMQTT.setKeepAlive(keepAlive);
    }

    bool MQTTArduino::connect()
    {
        if (_clientMQTT.connected())
            return false;
        Serial.print("Connecting to MQTT broker with ID: arduinoID...");
        while (!_clientMQTT.connect("arduinoID", "admin", "admin"))
        {
            Serial.print(".");
            delay(1000);
        }
        Serial.println("connected.");
        return true;
    }

    bool MQTTArduino::connected()
    {
        bool status = _clientMQTT.connected();
        // Serial.print("Connection status (0-disconnected, 1-connected): ");
        // Serial.println(status);
        return status;
    }

    bool MQTTArduino::loop()
    {
        return _clientMQTT.loop();
    }

    void MQTTArduino::onMessage(MQTTClientCallback cb)
    {
        _clientMQTT.onMessage(cb);
    }

    bool MQTTArduino::publish(char *topic, char *data)
    {
        return _clientMQTT.publish(topic, data);
    }

    void MQTTArduino::subscribeToTopic(const char *topic)
    {
        if (nullptr != topic)
            _clientMQTT.subscribe(topic);
    }

    bool MQTTArduino::send(const char *data, const char *topic)
    {
        return _clientMQTT.publish(topic, data);
    }

    bool MQTTArduino::receive()
    {
        return _clientMQTT.loop();
    }
}