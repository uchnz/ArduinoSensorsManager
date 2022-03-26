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

// IMQTT
// bool SensorManager::initMQTT(char *srvIP)
// {
//     if (nullptr == srvIP)
//         return false;

//     return _clientMQTT.begin(srvIP);
// }

// bool SensorManager::installCallback(sensor_manager::MQTTClientCallback cb)
// {
//     if (nullptr == cb)
//         return false;

//     _clientMQTT.onMessage(cb);
//     return true;
// }

//    void SensorManager::callbackIncommingMessages(char *topic, char *payload)

// void SensorManager::callbackIncommingMessages(String &topic, String &payload)
// {
//     printf("callback called:%s -> %s.\n ", topic.c_str(), payload.c_str());
// }

// void SensorManager::setKeepAliveClient(uint16_t keepAlive)
// {
//     _clientMQTT.setKeepAlive(keepAlive);
// }

// bool SensorManager::connectToMQTT()
// {
//     if (connectedToMQTT())
//         return true;

//     return _clientMQTT.connect();
// }

// bool SensorManager::connectedToMQTT()
// {
//     return _clientMQTT.connected();
// }

// bool SensorManager::checkForIncomingMessages()
// {
//     return _clientMQTT.loop();
// }

// bool SensorManager::publishMessageToBroker(char *topic, char *message)
// {
//     if (nullptr == topic || nullptr == message)
//         return false;

//     return _clientMQTT.publish(topic, message);
// }
