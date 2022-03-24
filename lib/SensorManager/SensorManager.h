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
        //        IEth &_eth;
        IMQTT &_clientMQTT;
        IDallas &_dallas;
        char **_topics;
        uint8_t _numberOfTopics;
        float _currentTemperature;
        float *_temperatures;
        uint8_t _numberOfSensors;

        bool isEmptyMAC(uint8_t (&mac)[6]);
        void updateAllTemperatures();

        // Dallas private
        float getTemperatureByID(uint8_t id);

    public:
        //        SensorManager(IEth &eth, IMQTT &mqtt, IDallas &dallas);
        SensorManager(IMQTT &mqtt, IDallas &dallas);
        virtual ~SensorManager();

        // Eth
        //        bool connectWithDHCP(uint8_t (&mac)[6]);

        // MQTT
        // bool initMQTT(char *srvIP);
        // void setKeepAliveClient(uint16_t keepAlive);
        // bool installCallback(sensor_manager::MQTTClientCallback cb);
        static void callbackIncommingMessages(String &topic, String &payload);
        // static void callbackIncommingMessages(char *topic, char *payload);
        bool connectToMQTT();
        bool connectedToMQTT();
        bool checkForIncomingMessages();
        bool publishMessageToBroker(char *topic, char *message);

        // Dallas
        void initSensors();
        uint8_t getSavedNumberSensors();
        float GetCurrentTemperatureByID(uint8_t id);
        void requestCurrentTemperatures();
        //        void getStringAddressByID(uint8_t id, int *address);
        void getStringAddressByID(uint8_t id, char *address);

        // Business logic
        // std::string printTemperatureDebugInfo(uint8_t id, float temperature);
        bool fillTopicsStrings(const char **topics, uint8_t totalTopics);
        void GetTopicByID(uint8_t id, char *topic);
        bool sendSensorData(const char *dataToSend, const char *addressToSendTo);
        bool receiveManagingData();
    };
}
