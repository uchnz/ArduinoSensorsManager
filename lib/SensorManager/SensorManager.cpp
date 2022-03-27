#ifndef __TESTS__
#include <LibPrintf.h>
#endif

#include <stdio.h>
#include <string.h>

#include <SensorManager.h>

SensorManager::SensorManager(IMQTT &mqtt, IDallas &dallas)
    : _clientMQTT(mqtt), _dallas(dallas)
{
    _topics = nullptr;
    _numberOfTopics = 0;
    _temperatures = nullptr;
    _numberOfSensors = 0;
    _totalNumberOfSensorsInArray = 0;
    _arrayOfISenosor = nullptr;
    _2DArrayOfTemperatures = nullptr;
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

    if (nullptr != _2DArrayOfTemperatures)
    {
        for (uint8_t i = 0; i < _totalNumberOfSensorsInArray; i++)
            delete[] _2DArrayOfTemperatures[i];

        delete[] _2DArrayOfTemperatures;
    }
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

bool SensorManager::refreshSensorsData()
{
    if (0 == _numberOfSensors)
        return false;

    _dallas.requestCurrentTemperatures();
    updateAllTemperatures();

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

uint8_t SensorManager::getTotalNumberOfSensorTypesInArray()
{

    return _totalNumberOfSensorsInArray;
}

bool SensorManager::initSenorsInArray(IDallas **arrayOfSensors, uint8_t totalSensors)
{
    if ((nullptr == arrayOfSensors) || (0 == totalSensors))
        return false;

    _arrayOfISenosor = arrayOfSensors;
    _totalNumberOfSensorsInArray = totalSensors;

    _2DArrayOfTemperatures = new float *[totalSensors];
    for (uint8_t i = 0; i < totalSensors; i++)
    {
        uint8_t num = _arrayOfISenosor[i]->getNumberOfConnectedSensors();
        _2DArrayOfTemperatures[i] = new float[num];
        for (uint8_t j = 0; j < num; j++)
            _2DArrayOfTemperatures[i][j] = -128.0;
    }

    return true;
}

float SensorManager::getCurrentTemperatureOfSingleSenorByID(uint8_t addressOfSensorTypeInArray, uint8_t addressOfExactSensor)
{
    return _2DArrayOfTemperatures[addressOfSensorTypeInArray][addressOfExactSensor];
}

uint8_t SensorManager::getNumberOfSensorsInArrayByID(uint8_t id)
{
    if (nullptr == _2DArrayOfTemperatures)
        return 1111;

    if (nullptr == _2DArrayOfTemperatures[id])
        return 11111;

    size_t numOfSensors = sizeof(_2DArrayOfTemperatures[id]) / sizeof(_2DArrayOfTemperatures[id][0]);

    return numOfSensors;
}