#ifndef __TESTS__
#include <LibPrintf.h>
#endif

#include <stdio.h>
#include <string.h>
#include <ArduinoJson.h>
#include <SensorManager.h>

SensorManager::SensorManager(IMQTT &mqtt)
    : _clientMQTT(mqtt)
{
    _addressesToSendMeasurementsTo = nullptr;
    // _totalAddresses = 0;
    _totalPortsWithSensors = 0;
    _ISenosorObjectsManagingArray2D = nullptr;
    _IActuatorObjects = nullptr;
    _totalActuators = 0;
    _addressesToReadActuatorsCommands = nullptr;

    // _measurementsArray2D = nullptr;
    // _numberOfSensorsOnEachPort = nullptr;
    // _sensorsNamesArray = nullptr;
}

SensorManager::~SensorManager()
{
    if (_addressesToSendMeasurementsTo)
    {
        // for (uint8_t i = 0; i < _totalAddresses; i++)
        for (uint8_t i = 0; i < _totalPortsWithSensors; i++)
            delete[] _addressesToSendMeasurementsTo[i];

        delete[] _addressesToSendMeasurementsTo;
    }

    if (_addressesToReadActuatorsCommands)
    {
        for (uint8_t i = 0; i < _totalActuators; i++)
            delete[] _addressesToReadActuatorsCommands[i];

        delete[] _addressesToReadActuatorsCommands;
    }

    // if (_measurementsArray2D)
    // {
    //     for (uint8_t i = 0; i < _totalPortsWithSensors; i++)
    //         delete[] _measurementsArray2D[i];

    //     delete[] _measurementsArray2D;
    // }

    // if (_numberOfSensorsOnEachPort)
    //     delete[] _numberOfSensorsOnEachPort;

    // if (_sensorsNamesArray)
    // {
    //     for (uint8_t i = 0; i < _totalPortsWithSensors; i++)
    //         delete[] _sensorsNamesArray[i];

    //     delete[] _sensorsNamesArray;
    // }
}

// bool SensorManager::setAddressesToSendMeasurementsTo(const char **addressesArray, uint8_t totalAddresses)
// {
//     if (nullptr == addressesArray)
//         return false;
//     if (totalAddresses <= 0)
//         return false;

//     _totalAddresses = totalAddresses;
//     _addressesToSendMeasurementsTo = new char *[totalAddresses];
//     for (uint8_t i = 0; i < totalAddresses; i++)
//     {
//         uint8_t sizeOfAddressWithEndOfString = 1;
//         if (nullptr != addressesArray[i])
//             sizeOfAddressWithEndOfString = strlen(addressesArray[i]) + 1;
//         _addressesToSendMeasurementsTo[i] = new char[sizeOfAddressWithEndOfString];
//         memcpy(_addressesToSendMeasurementsTo[i], (sizeOfAddressWithEndOfString > 1) ? addressesArray[i] : "", sizeOfAddressWithEndOfString);
//     }

//     return true;
// }
// bool SensorManager::saveAddresses(const char **addressesArray)
// {
//     if (nullptr == addressesArray)
//         return false;

//     _addressesToSendMeasurementsTo = new char *[_totalPortsWithSensors];
//     for (uint8_t i = 0; i < _totalPortsWithSensors; i++)
//     {
//         uint8_t sizeOfAddressWithEndOfString = 1;
//         if (nullptr != addressesArray[i])
//             sizeOfAddressWithEndOfString = strlen(addressesArray[i]) + 1;
//         _addressesToSendMeasurementsTo[i] = new char[sizeOfAddressWithEndOfString];
//         memcpy(_addressesToSendMeasurementsTo[i], (sizeOfAddressWithEndOfString > 1) ? addressesArray[i] : "", sizeOfAddressWithEndOfString);
//     }

//     return true;
// }
bool SensorManager::saveAddresses2(const char **addressesArray, char **&destination, uint8_t size)
{
    if (nullptr == addressesArray)
        return false;

    destination = new char *[size];
    for (uint8_t i = 0; i < size; i++)
    {
        uint8_t sizeOfAddressWithEndOfString = 1;
        if (nullptr != addressesArray[i])
            sizeOfAddressWithEndOfString = strlen(addressesArray[i]) + 1;
        destination[i] = new char[sizeOfAddressWithEndOfString];
        memcpy(destination[i], (sizeOfAddressWithEndOfString > 1) ? addressesArray[i] : "", sizeOfAddressWithEndOfString);
    }

    return true;
}

// void SensorManager::getAddressToSendToByID(uint8_t id, char *address)
// {
//     if (id >= _totalAddresses)
//         return;

//     uint8_t sizeOfTopicWitEndOfString = strlen(_addressesToSendMeasurementsTo[id]) + 1;
//     memcpy(address, _addressesToSendMeasurementsTo[id], sizeOfTopicWitEndOfString);
// }

// uint8_t SensorManager::getTotalNumberOfOccupiedPINs()
// {

//     return _totalPortsWithSensors;
// }

// void SensorManager::fillArraysWithInitialValues()
// {
//     for (uint8_t i = 0; i < _totalPortsWithSensors; i++)
//     {
//         _numberOfSensorsOnEachPort[i] = 0;
//         if (!_ISenosorObjectsManagingArray2D[i])
//             continue;
//         _numberOfSensorsOnEachPort[i] = _ISenosorObjectsManagingArray2D[i]->getNumberOfConnectedSensors();
//         _measurementsArray2D[i] = new double[_numberOfSensorsOnEachPort[i]];
//         for (uint8_t j = 0; j < _numberOfSensorsOnEachPort[i]; j++)
//             _measurementsArray2D[i][j] = -128.0;
//         char sensorName[30];
//         int nameLength = _ISenosorObjectsManagingArray2D[i]->getName(sensorName);
//         _sensorsNamesArray[i] = new char[nameLength + 1];
//         memcpy(_sensorsNamesArray[i], (nameLength > 1) ? sensorName : "Empty", nameLength + 1);
//     }
// }
// void SensorManager::initArrays(ISensor **sensorsArray2D, uint8_t totalOccupiedPINs)
// {
//     _ISenosorObjectsManagingArray2D = sensorsArray2D;
//     _totalPortsWithSensors = totalOccupiedPINs;
//     _sensorsNamesArray = new char *[totalOccupiedPINs];
//     _measurementsArray2D = new double *[totalOccupiedPINs];
//     _numberOfSensorsOnEachPort = new uint8_t[totalOccupiedPINs];
//     for (uint8_t i = 0; i < totalOccupiedPINs; i++)
//     {
//         _sensorsNamesArray[i] = nullptr;
//         _measurementsArray2D[i] = nullptr;
//         _numberOfSensorsOnEachPort[i] = 0;
//     }
// }

// bool SensorManager::isSensorArrayAllNulls(ISensor **sensorsArray2D, uint8_t totalOccupiedPINs)
// {
//     if (!sensorsArray2D)
//         return true;

//     bool allNulls = true;
//     for (uint8_t i = 0; i < totalOccupiedPINs; i++)
//         if (sensorsArray2D[i])
//             allNulls = false;

//     return allNulls;
// }
// bool SensorManager::init(ISensor **sensorsArray2D, uint8_t totalOccupiedPINs)
// {
//     if (isSensorArrayAllNulls(sensorsArray2D, totalOccupiedPINs))
//         return false;

//     initArrays(sensorsArray2D, totalOccupiedPINs);
//     fillArraysWithInitialValues();

//     return true;
// }

// bool SensorManager::areArraysAllNulls(ISensor **sensorsArray2D, const char **addressesArray, uint8_t totalOccupiedPINs)
// {
//     if (!sensorsArray2D || !addressesArray)
//         return true;

//     bool allNulls = true;
//     for (uint8_t i = 0; i < totalOccupiedPINs; i++)
//         if (sensorsArray2D[i] && (0 != addressesArray[i][0]))
//             allNulls = false;

//     return allNulls;
// }

// bool SensorManager::arraysHaveNulls(ISensor **sensorsArray2D, const char **addressesArray, uint8_t totalOccupiedPINs)
bool SensorManager::arraysHaveNulls(void **sensorsArray2D, const char **addressesArray, uint8_t totalOccupiedPINs)
{
    if (!sensorsArray2D || !addressesArray)
        return true;

    bool hasNull = false;
    for (uint8_t i = 0; i < totalOccupiedPINs; i++)
    {
        if (!sensorsArray2D[i])
            hasNull = true;
        if (!addressesArray[i] || (0 == addressesArray[i][0]))
            hasNull = true;
    }

    return hasNull;
}

// void SensorManager::initArrays2(ISensor **sensorsArray2D, uint8_t totalOccupiedPINs)
// {
//     _ISenosorObjectsManagingArray2D = sensorsArray2D;
//     _totalPortsWithSensors = totalOccupiedPINs;
//     // _sensorsNamesArray = new char *[totalOccupiedPINs];
//     // _measurementsArray2D = new double *[totalOccupiedPINs];
//     // _numberOfSensorsOnEachPort = new uint8_t[totalOccupiedPINs];
//     // for (uint8_t i = 0; i < totalOccupiedPINs; i++)
//     // {
//     //     _sensorsNamesArray[i] = nullptr;
//     //     _measurementsArray2D[i] = nullptr;
//     //     _numberOfSensorsOnEachPort[i] = 0;
//     // }
// }
// bool SensorManager::init2(ISensor **sensorsArray2D, const char **addressesArray, uint8_t totalOccupiedPINs)
bool SensorManager::init2(ISensor **sensorsArray2D, const char **addressesArray, uint8_t totalOccupiedPINs)
{
    if (!totalOccupiedPINs)
        return false;

    // if (arraysHaveNulls(sensorsArray2D, addressesArray, totalOccupiedPINs))
    if (arraysHaveNulls((void **)sensorsArray2D, addressesArray, totalOccupiedPINs))
        return false;

    _ISenosorObjectsManagingArray2D = sensorsArray2D;
    _totalPortsWithSensors = totalOccupiedPINs;
    // saveAddresses(addressesArray);
    saveAddresses2(addressesArray, _addressesToSendMeasurementsTo, _totalPortsWithSensors);

    return true;
}

// double SensorManager::getCurrentMeasurementOfOneSenorByID(uint8_t row, uint8_t col)
// {
//     if (!_numberOfSensorsOnEachPort)
//         return -128;
//     if (!_numberOfSensorsOnEachPort[row])
//         return -128;
//     if (!_numberOfSensorsOnEachPort[row])
//         return -128;
//     if (_numberOfSensorsOnEachPort[row] < col)
//         return -128;

//     return _measurementsArray2D[row][col];
// }

// uint8_t SensorManager::getNumberOfSensorsOnPINByID(uint8_t id)
// {
//     if (!_measurementsArray2D)
//         return 0;

//     if (!_measurementsArray2D[id])
//         return 0;

//     return _numberOfSensorsOnEachPort[id];
// }

// bool SensorManager::refreshSensorsData2D()
// {
//     if ((!_measurementsArray2D))
//         return false;

//     for (uint8_t i = 0; i < _totalPortsWithSensors; i++)
//     {
//         _ISenosorObjectsManagingArray2D[i]->requestCurrentMeasurement();
//         for (uint8_t j = 0; j < _numberOfSensorsOnEachPort[i]; j++)
//             _measurementsArray2D[i][j] = _ISenosorObjectsManagingArray2D[i]->getCurrentMeasurementByID(j);
//     }

//     return true;
// }
bool SensorManager::refreshSensors()
{
    if ((!_ISenosorObjectsManagingArray2D))
        return false;

    for (uint8_t i = 0; i < _totalPortsWithSensors; i++)
    {
        // printf("before request, total ports: %d\n", _totalPortsWithSensors);
        // char sensorName[30];
        // _ISenosorObjectsManagingArray2D[i]->getName(sensorName);
        // printf("reading port[%d]: name<%s>, sensors on port<%d>\n", i, sensorName, _ISenosorObjectsManagingArray2D[i]->getNumberOfConnectedSensors());
        // printf("reading port[%d]: name<%s>, sensors on port<-->\n", i, sensorName);
        _ISenosorObjectsManagingArray2D[i]->requestCurrentMeasurement();
    }

    return true;
}

// uint16_t SensorManager::makeJSON(char *message, uint16_t len, uint8_t sensorID)
// {
//     size_t capacity = JSON_ARRAY_SIZE(_numberOfSensorsOnEachPort[sensorID] + 1) + JSON_OBJECT_SIZE(2);
//     DynamicJsonDocument doc(capacity);
//     doc["name"] = (const char *)_sensorsNamesArray[sensorID]; // explisit cast to avoid copying of a string and reduce size of 'doc' object
//                                                               // without 'const' doesn't work either way, don't know why yet
//     JsonArray data = doc.createNestedArray("data");
//     for (uint8_t i = 0; i < _numberOfSensorsOnEachPort[sensorID]; i++)
//         data[i] = _measurementsArray2D[sensorID][i];
//     serializeJson(doc, message, len);

//     return strlen(message);
// }
// bool SensorManager::sendSensorsData2D()
// {
//     bool atLeastOneError = false;
//     for (uint8_t i = 0; i < _totalPortsWithSensors; i++)
//     {
//         char messageToSend[SM_nm::MAX_MESSAGE_SIZE];
//         uint16_t len = makeJSON(messageToSend, SM_nm::MAX_MESSAGE_SIZE, i);
//         bool result = _clientMQTT.send(messageToSend, _addressesToSendMeasurementsTo[i]);
//         if (!result)
//             atLeastOneError = true;
//     }
//     return !atLeastOneError;
// }

uint16_t SensorManager::makeJSON2(char *message, uint16_t len, uint8_t sensorID)
{
    uint8_t sensorsOnPort = _ISenosorObjectsManagingArray2D[sensorID]->getNumberOfConnectedSensors();
    size_t capacity = JSON_ARRAY_SIZE(sensorsOnPort + 1) + JSON_OBJECT_SIZE(2);
    DynamicJsonDocument doc(capacity);

    char sensorName[30];
    _ISenosorObjectsManagingArray2D[sensorID]->getName(sensorName);
    doc["name"] = (const char *)sensorName; // explisit cast to avoid copying of a string and reduce size of 'doc' object
                                            // without 'const' doesn't work either way, don't know why yet
    JsonArray data = doc.createNestedArray("data");
    for (uint8_t i = 0; i < sensorsOnPort; i++)
        data[i] = _ISenosorObjectsManagingArray2D[sensorID]->getCurrentMeasurementByID(i);

    serializeJson(doc, message, len);
    return strlen(message);
}
bool SensorManager::sendSensorsData()
{
    if (!_ISenosorObjectsManagingArray2D || !_addressesToSendMeasurementsTo)
        return false;

    bool atLeastOneError = false;
    for (uint8_t i = 0; i < _totalPortsWithSensors; i++)
    {
        char messageToSend[SM_nm::MAX_MESSAGE_SIZE];
        // uint16_t len = makeJSON2(messageToSend, SM_nm::MAX_MESSAGE_SIZE, i);
        makeJSON2(messageToSend, SM_nm::MAX_MESSAGE_SIZE, i);
        bool result = _clientMQTT.send(messageToSend, _addressesToSendMeasurementsTo[i]);
        if (!result)
            atLeastOneError = true;
        // printf("address: %s -> message: %s\n", _addressesToSendMeasurementsTo[i], messageToSend);
    }
    return !atLeastOneError;
}

uint16_t SensorManager::makeSystemJSON(char *message, uint16_t len, uint32_t millisSinceStart)
{
    size_t capacity = JSON_OBJECT_SIZE(3);
    DynamicJsonDocument doc(capacity);
    doc["DeviceID"] = "VIJ2022";
    doc["uptime"] = millisSinceStart;
    doc["sensors"] = _totalPortsWithSensors;

    serializeJson(doc, message, len);
    return strlen(message);
}
bool SensorManager::sendSystemInfo(uint32_t millisSinceStart)
{
    char messageToSend[SM_nm::MAX_MESSAGE_SIZE];
    // uint16_t len = makeSystemJSON(messageToSend, SM_nm::MAX_MESSAGE_SIZE, millisSinceStart);
    makeSystemJSON(messageToSend, SM_nm::MAX_MESSAGE_SIZE, millisSinceStart);
    bool result = _clientMQTT.send(messageToSend, "/SystemInfo");
    printf("address: /SystemInfo -> message: %s\n", messageToSend);
    return result;
}

// bool SensorManager::arraysHaveNullsActuators(void **actuatorsArray, const char **addressesArray, uint8_t totalOccupiedPINs)
// {
//     if (!actuatorsArray || !addressesArray)
//         return true;

//     bool hasNull = false;
//     for (uint8_t i = 0; i < totalOccupiedPINs; i++)
//     {
//         if (!actuatorsArray[i])
//             hasNull = true;
//         if (!addressesArray[i] || (0 == addressesArray[i][0]))
//             hasNull = true;
//     }

//     return hasNull;
// }
bool SensorManager::initActuators(IActuator **actuatorsArray, const char **addressesArray, uint8_t totalOccupiedPINs)
{
    if (!totalOccupiedPINs)
        return false;

    if (arraysHaveNulls((void **)actuatorsArray, addressesArray, totalOccupiedPINs))
        return false;

    _IActuatorObjects = actuatorsArray;
    _totalActuators = totalOccupiedPINs;
    saveAddresses2(addressesArray, _addressesToReadActuatorsCommands, _totalActuators);

    for (uint8_t i = 0; i < _totalActuators; i++)
        _clientMQTT.subscribeToTopic(addressesArray[0]);

    return true;
}

bool SensorManager::parseCommand(const char *commandJSON, SM_nm::ActuatorCommand &ac)
{
    const int capacity = JSON_OBJECT_SIZE(2) + 75;
    StaticJsonDocument<capacity> doc;
    DeserializationError error = deserializeJson(doc, commandJSON);
    if (error)
        return false;

    uint8_t maxSize = sizeof(ac.name);
    strncpy(ac.name, doc["name"] | "no name", maxSize);
    ac.name[maxSize - 1] = 0;

    maxSize = sizeof(ac.command);
    strncpy(ac.command, doc["action"] | "no action", maxSize);
    ac.command[maxSize - 1] = 0;

    return true;
}
uint8_t SensorManager::getActuatorIDByName(const char *name)
{
    char actuatorName[30];
    for (uint8_t i = 0; i < _totalActuators; i++)
    {
        _IActuatorObjects[i]->getName(actuatorName);
        if (0 == strcmp(name, actuatorName))
            return i;
    }
    return 255;
}
bool SensorManager::executeCommand(const char *commandJSON)
{
    if (!_IActuatorObjects)
        return false;

    SM_nm::ActuatorCommand ac;
    if (!parseCommand(commandJSON, ac))
        return false;

    uint8_t id = getActuatorIDByName(ac.name);
    if (255 != id)
        return _IActuatorObjects[id]->execute(ac.command);

    return false;
}
