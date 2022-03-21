#pragma once

#include <stdint.h>
//#include <iostream>
//#include <StandardCplusplus.h>
//#include <string>

#include <IEth.h>
#include <IMQTT.h>
#include <IDallas.h>

namespace sensor_manager
{
    class SensorManager
    {
    private:
        IEth &_eth;
        IMQTT &_clientMQTT;
        IDallas &_dallas;
        char **_topics;
        uint8_t _numberOfTopics;
        float _currentTemperature;
        float *_temperatures;
        uint8_t _numberOfSensors;

        bool isEmptyMAC(uint8_t (&mac)[6]);

    public:
        SensorManager(IEth &eth, IMQTT &mqtt, IDallas &dallas);
        virtual ~SensorManager();
        // IEth
        bool connectWithDHCP(uint8_t (&mac)[6]);
        // IMQTT
        bool initMQTT(char *srvIP);
        bool installCallback(sensor_manager::MQTTClientCallback cb);
        static void callbackIncommingMessages(char *topic, char *payload);
        void setKeepAliveClient(uint16_t keepAlive);
        bool connectToMQTT();
        bool connectedToMQTT();
        bool checkForIncomingMessages();
        bool publishMessageToBroker(char *topic, char *message);
        // IDallas
        // void initDallasSensors();
        // void setSensorsPrecision(int precision);
        void requestCurrentTemperatures();
        uint8_t getNumberOfSensors();
        float getTemperatureByID(uint8_t id);
        char *getStringAddressByID(uint8_t id);
        bool updateTemperature(uint8_t id);
        float GetCurrentTemperatureByID(uint8_t id);
        void initSensors();
        void updateAllTemperatures();
        // Business logic
        // std::string printTemperatureDebugInfo(uint8_t id, float temperature);
        bool fillTopicsStrings(char **topics, uint8_t totalTopics);
        char *GetTopicByID(uint8_t id);
        float getCurrentTemperature();
    };
}
