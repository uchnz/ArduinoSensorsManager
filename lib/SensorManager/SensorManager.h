#pragma once

#include <stdint.h>

#include <IEth.h>
#include <IMQTT.h>
#include <IDallas.h>

class SensorManager
{
private:
    IMQTT &_clientMQTT;
    IDallas &_dallas;

    char **_topics;
    uint8_t _numberOfTopics;
    float *_temperatures;
    uint8_t _numberOfSensors;

    uint8_t _totalNumberOfSensorTypesInArray;
    IDallas **_arrayOfISenosor;
    float **_2DArrayOfTemperatures;
    uint8_t *_numberOfsesorsInArrayCell;

    // Dallas private
    void initArrays(IDallas **arrayOfSensors, uint8_t totalSensorTypes);
    void fillArraysWithInitialValues();
    void updateAllTemperatures();

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

    uint8_t getTotalNumberOfSensorTypesInArray();
    bool initSenorsInArray(IDallas **arrayOfSensors, uint8_t totalSensors);
    float getCurrentTemperatureOfSingleSenorByID(uint8_t addressOfSensorTypeInArray, uint8_t addressOfExactSensor);
    uint8_t getNumberOfSensorsInArrayCellByID(uint8_t id);
    bool refreshSensorsData2D();
    bool sendSensorsData2D();
};
