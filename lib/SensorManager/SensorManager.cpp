#include <SensorManager.h>

namespace sensor_manager
{
    SensorManager::SensorManager(IEth &eth, IMQTT &mqtt, IDallas &dallas)
        : _eth(eth), _clientMQTT(mqtt), _dallas(dallas)
    {
    }

    bool SensorManager::isEmptyMAC(uint8_t (&mac)[6])
    {
        bool emptyMAC = true;
        for (uint8_t i = 0; i < sizeof(mac); i++)
        {
            if (0 != mac[i])
                emptyMAC = false;
        }
        return emptyMAC;
    }

    // IEth
    bool SensorManager::connectWithDHCP(uint8_t (&mac)[6])
    {
        if (isEmptyMAC(mac))
            return false;

        return _eth.connect(mac);
    }

    // IMQTT
    bool SensorManager::initMQTT(char *srvIP)
    {
        if (nullptr == srvIP)
            return false;

        return _clientMQTT.begin(srvIP);
    }

    bool SensorManager::installCallback(sensor_manager::MQTTClientCallback cb)
    {
        if (nullptr == cb)
            return false;

        _clientMQTT.onMessage(cb);
        return true;
    }

    void SensorManager::callbackIncommingMessages(char *topic, char *payload)
    {
    }

    void SensorManager::setKeepAliveClient(uint16_t keepAlive)
    {
        _clientMQTT.setKeepAlive(keepAlive);
    }

    bool SensorManager::connectToMQTT()
    {
        if (connectedToMQTT())
            return true;

        return _clientMQTT.connect();
    }

    bool SensorManager::connectedToMQTT()
    {
        return _clientMQTT.connected();
    }

    bool SensorManager::checkForIncomingMessages()
    {
        return _clientMQTT.loop();
    }

    bool SensorManager::publishMessageToBroker(char *topic, char *message)
    {
        if (nullptr == topic || nullptr == message)
            return false;

        return _clientMQTT.publish(topic, message);
    }

    // IDallas
    void SensorManager::initDallasSensors()
    {
        _dallas.init();
    }

    void SensorManager::setSensorsPrecision(int precision)
    {
        _dallas.setSensorsPrecision(precision);
    }

    void SensorManager::requestCurrentTemperatures()
    {
        _dallas.requestCurrentTemperatures();
    }

    uint8_t SensorManager::getNumberOfSensors()
    {
        return _dallas.getNumberOfSensors();
    }

    float SensorManager::getTemperatureByID(uint8_t id)
    {
        return _dallas.getTemperatureByID(id);
    }

    char *SensorManager::getStringAddressByID(uint8_t id)
    {
        return _dallas.getStringAddressByID(id);
    }
}