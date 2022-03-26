#pragma once

#include <stdint.h>

#include <IEth.h>
#include <IMQTT.h>
#include <IDallas.h>

namespace sensor_manager
{
    class SensorManager
    {
    private:
        IMQTT &_clientMQTT;
        IDallas &_dallas;

        char **_topics;
        uint8_t _numberOfTopics;
        float *_temperatures;
        uint8_t _numberOfSensors;

        uint64_t lastTimeProcessedInMillisecs;

        // Dallas private
        void
        updateAllTemperatures();

    public:
        SensorManager(IMQTT &mqtt, IDallas &dallas);
        virtual ~SensorManager();

        // MQTT
        bool sendSensorsData(const char *dataToSend, const char *addressToSendTo);
        bool receiveManagingData();

        // Dallas
        void scanConnectedTemperatureSensors();
        uint8_t getSavedNumberSensors();
        bool refreshSensorsData();
        float getCurrentTemperatureByID(uint8_t id);
        //        void getStringAddressByID(uint8_t id, char *address);

        // Business logic
        // std::string printTemperatureDebugInfo(uint8_t id, float temperature);
        bool fillTopicsStrings(const char **topics, uint8_t totalTopics);
        void GetTopicByID(uint8_t id, char *topic);

        bool processDataWithInterval();
        bool sendSensorsData();
        bool sendSensorDataByID(uint8_t id);
    };
}
