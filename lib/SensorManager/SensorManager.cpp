#ifndef __TESTS__
#include <LibPrintf.h>
#endif

#include <stdio.h>
#include <string.h>

#include <SensorManager.h>

// namespace sensor_manager
// {
SensorManager::SensorManager(IMQTT &mqtt, IDallas &dallas)
    : _clientMQTT(mqtt), _dallas(dallas)
{
    _topics = nullptr;
    _numberOfTopics = 0;
    _temperatures = nullptr;
    _numberOfSensors = 0;
    lastTimeProcessedInMillisecs = 0;
}

SensorManager::~SensorManager()
{
    if (nullptr != _topics)
    {
        for (uint8_t i = 0; i < _numberOfTopics; i++)
            delete[] _topics[i];

        delete[] _topics;
    }

    if (nullptr != _temperatures)
        delete[] _temperatures;
}

// Private
void SensorManager::updateAllTemperatures()
{
    for (uint8_t i = 0; i < _numberOfSensors; i++)
        _temperatures[i] = _dallas.getTemperatureByID(i);
}

// Public
// MQTT
// bool SensorManager::sendSensorsData(const char *dataToSend, const char *addressToSendTo)
// {
//     if ((nullptr != dataToSend) && (nullptr != addressToSendTo))
//         return _clientMQTT.send(dataToSend, addressToSendTo);

//     return false;
// }

bool SensorManager::receiveManagingData()
{
    return _clientMQTT.receive();
}

// Dallas
void SensorManager::scanConnectedTemperatureSensors()
{
    _numberOfSensors = _dallas.getNumberOfConnectedSensors();
    if (0 == _numberOfSensors)
        return;

    _temperatures = new float[_numberOfSensors];
    for (uint8_t i = 0; i < _numberOfSensors; i++)
        _temperatures[i] = -128;
}

uint8_t SensorManager::getSavedNumberSensors()
{
    return _numberOfSensors;
}

bool SensorManager::refreshSensorsData()
{
    if (0 == _numberOfSensors)
        return false;

    _dallas.requestCurrentTemperatures();
    updateAllTemperatures();

    return true;
}

float SensorManager::getCurrentTemperatureByID(uint8_t id)
{
    if (id < _numberOfSensors)
        return _temperatures[id];

    return -128;
}

// Business logic
// float SensorManager::getCurrentTemperature()
// {
//     return _currentTemperature;
// }

// std::string SensorManager::printTemperatureDebugInfo(uint8_t id, float temperature)
// {
//     char buffer[50] = "";
//     sprintf(buffer, "Sensor[%d] = %.2f, Address: 28e4ac0400fe", id, temperature);
//     std::string stringToReturn = buffer;

//     return stringToReturn;
// }

bool SensorManager::fillTopicsStrings(const char **topics, uint8_t totalTopics)
{
    if (nullptr == topics)
        return false;
    if (totalTopics <= 0)
        return false;

    _topics = new char *[totalTopics];
    for (uint8_t i = 0; i < totalTopics; i++)
    {
        uint8_t sizeOfTopicWitEndOfString = strlen(topics[i]) + 1;
        _topics[i] = new char[sizeOfTopicWitEndOfString];
        memcpy(_topics[i], topics[i], sizeOfTopicWitEndOfString);
    }
    _numberOfTopics = totalTopics;

    return true;
}

void SensorManager::GetTopicByID(uint8_t id, char *topic)
{
    if (id >= _numberOfTopics)
        return;

    uint8_t sizeOfTopicWitEndOfString = strlen(_topics[id]) + 1;
    memcpy(topic, _topics[id], sizeOfTopicWitEndOfString);
    return;
}

bool SensorManager::processDataWithInterval()
{
    if (!refreshSensorsData())
        return false;

    if (!sendSensorsData())
        return false;

    return true;
}

bool SensorManager::sendSensorsData()
{
    if (0 == _numberOfSensors)
        return false;

    for (uint8_t i = 0; i < _numberOfSensors; i++)
    {
        char tempConverted[10];
        sprintf(tempConverted, "%.2f", (double)_temperatures[i]);
        _clientMQTT.send(tempConverted, _topics[i]);
    }

    return true;
}

bool SensorManager::sendSensorDataByID(uint8_t id)
{
    return false;
}
// }
