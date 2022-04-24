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
    _totalAddresses = 0;
    _totalPortsWithSensors = 0;
    _ISenosorObjectsManagingArray2D = nullptr;
    _measurementsArray2D = nullptr;
    _numberOfSensorsOnEachPort = nullptr;
    _sensorsNamesArray = nullptr;
}

SensorManager::~SensorManager()
{
    if (_addressesToSendMeasurementsTo)
    {
        for (uint8_t i = 0; i < _totalAddresses; i++)
            delete[] _addressesToSendMeasurementsTo[i];

        delete[] _addressesToSendMeasurementsTo;
    }

    if (_measurementsArray2D)
    {
        for (uint8_t i = 0; i < _totalPortsWithSensors; i++)
            delete[] _measurementsArray2D[i];

        delete[] _measurementsArray2D;
    }

    if (_numberOfSensorsOnEachPort)
        delete[] _numberOfSensorsOnEachPort;

    if (_sensorsNamesArray)
    {
        for (uint8_t i = 0; i < _totalPortsWithSensors; i++)
            delete[] _sensorsNamesArray[i];

        delete[] _sensorsNamesArray;
    }
}

bool SensorManager::setAddressesToSendMeasurementsTo(const char **addressesArray, uint8_t totalAddresses)
{
    if (nullptr == addressesArray)
        return false;
    if (totalAddresses <= 0)
        return false;

    _totalAddresses = totalAddresses;
    _addressesToSendMeasurementsTo = new char *[totalAddresses];
    for (uint8_t i = 0; i < totalAddresses; i++)
    {
        uint8_t sizeOfAddressWithEndOfString = 1;
        if (nullptr != addressesArray[i])
            sizeOfAddressWithEndOfString = strlen(addressesArray[i]) + 1;
        _addressesToSendMeasurementsTo[i] = new char[sizeOfAddressWithEndOfString];
        memcpy(_addressesToSendMeasurementsTo[i], (sizeOfAddressWithEndOfString > 1) ? addressesArray[i] : "", sizeOfAddressWithEndOfString);
    }

    return true;
}

void SensorManager::getAddressToSendToByID(uint8_t id, char *address)
{
    if (id >= _totalAddresses)
        return;

    uint8_t sizeOfTopicWitEndOfString = strlen(_addressesToSendMeasurementsTo[id]) + 1;
    memcpy(address, _addressesToSendMeasurementsTo[id], sizeOfTopicWitEndOfString);
}

uint8_t SensorManager::getTotalNumberOfOccupiedPINs()
{

    return _totalPortsWithSensors;
}

void SensorManager::fillArraysWithInitialValues()
{
    for (uint8_t i = 0; i < _totalPortsWithSensors; i++)
    {
        // uint8_t num = 0;
        _numberOfSensorsOnEachPort[i] = 0;
        if (!_ISenosorObjectsManagingArray2D[i])
            continue;
        _numberOfSensorsOnEachPort[i] = _ISenosorObjectsManagingArray2D[i]->getNumberOfConnectedSensors();
        _measurementsArray2D[i] = new double[_numberOfSensorsOnEachPort[i]];
        for (uint8_t j = 0; j < _numberOfSensorsOnEachPort[i]; j++)
            _measurementsArray2D[i][j] = -128.0;
        char sensorName[30];
        int nameLength = _ISenosorObjectsManagingArray2D[i]->getName(sensorName);
        _sensorsNamesArray[i] = new char[nameLength + 1];
        memcpy(_sensorsNamesArray[i], (nameLength > 1) ? sensorName : "Empty", nameLength + 1);
    }
}
void SensorManager::initArrays(ISensor **sensorsArray2D, uint8_t totalOccupiedPINs)
{
    _ISenosorObjectsManagingArray2D = sensorsArray2D;
    _totalPortsWithSensors = totalOccupiedPINs;
    _sensorsNamesArray = new char *[totalOccupiedPINs];
    _measurementsArray2D = new double *[totalOccupiedPINs];
    _numberOfSensorsOnEachPort = new uint8_t[totalOccupiedPINs];
    for (uint8_t i = 0; i < totalOccupiedPINs; i++)
    {
        _sensorsNamesArray[i] = nullptr;
        _measurementsArray2D[i] = nullptr;
        _numberOfSensorsOnEachPort[i] = 0;
    }
}
bool SensorManager::isSensorArrayAllNulls(ISensor **sensorsArray2D, uint8_t totalOccupiedPINs)
{
    if (!sensorsArray2D)
        return true;

    bool allNulls = true;
    for (uint8_t i = 0; i < totalOccupiedPINs; i++)
        if (sensorsArray2D[i])
            allNulls = false;

    return allNulls;
}
bool SensorManager::initSenorsOnAllPINs(ISensor **sensorsArray2D, uint8_t totalOccupiedPINs)
{
    if (isSensorArrayAllNulls(sensorsArray2D, totalOccupiedPINs))
        return false;

    initArrays(sensorsArray2D, totalOccupiedPINs);
    fillArraysWithInitialValues();

    return true;
}

double SensorManager::getCurrentMeasurementOfOneSenorByID(uint8_t row, uint8_t col)
{
    if (!_numberOfSensorsOnEachPort)
        return -128;
    if (!_numberOfSensorsOnEachPort[row])
        return -128;
    if (0 == _numberOfSensorsOnEachPort[row])
        return -128;
    if (_numberOfSensorsOnEachPort[row] < col)
        return -128;

    return _measurementsArray2D[row][col];
}

uint8_t SensorManager::getNumberOfSensorsOnPINByID(uint8_t id)
{
    if (nullptr == _measurementsArray2D)
        return 0;

    if (nullptr == _measurementsArray2D[id])
        return 0;

    return _numberOfSensorsOnEachPort[id];
}

bool SensorManager::refreshSensorsData2D()
{
    if ((!_measurementsArray2D))
        return false;

    for (uint8_t i = 0; i < _totalPortsWithSensors; i++)
    {
        _ISenosorObjectsManagingArray2D[i]->requestCurrentMeasurement();
        for (uint8_t j = 0; j < _numberOfSensorsOnEachPort[i]; j++)
            _measurementsArray2D[i][j] = _ISenosorObjectsManagingArray2D[i]->getCurrentMeasurementByID(j);
    }
    return true;
}

// void SensorManager::getJSON(double value, char *id, char *output)
// {
//     StaticJsonDocument<200> doc;
//     doc["id"] = "IDS1_temp";
//     doc["value"] = value;
//     // serializeJson(doc, output);
// }
// bool SensorManager::sendSensorsData2D()
// {
//     bool atLeastOneError = false;
//     for (uint8_t i = 0; i < _totalPortsWithSensors; i++)
//         for (uint8_t j = 0; j < _numberOfSensorsOnEachPort[i]; j++)
//         {
//             char tempConverted[10];
//             sprintf(tempConverted, "%.2f", (double)_measurementsArray2D[i][j]);
//             // bool result = _clientMQTT.send(tempConverted, _addressesToSendMeasurementsTo[i]);
//             char output[200];
//             char *id; // = "IDS1_temp";
//             _ISenosorObjectsManagingArray2D[i]->getName(id);
//             // getJSON(_measurementsArray2D[i][j], id, output);
//             StaticJsonDocument<200> doc;
//             doc["id"] = id;
//             doc["value"] = tempConverted;
//             serializeJson(doc, output);
//             bool result = _clientMQTT.send(output, _addressesToSendMeasurementsTo[i]);
//             if (!result)
//                 atLeastOneError = true;
//             printf("s%d -> id[%d] temp is: %s, sending to: %s, status: %s\n", i, j, output, _addressesToSendMeasurementsTo[i], result ? "sent" : "failed");
//         }

//     return !atLeastOneError;
// }
// void SensorManager::formatToJSON(uint8_t sensorID, uint8_t valueID, char *message, uint16_t messageSize)
// {
//     StaticJsonDocument<200> doc;
//     doc["id"] = _sensorsNamesArray[sensorID];
//     char valueTruncated[10];
//     sprintf(valueTruncated, "%.2f", _measurementsArray2D[sensorID][valueID]);
//     doc["value"] = valueTruncated;

//     serializeJson(doc, message, messageSize);
// }

// bool SensorManager::sendSensorsData2D()
// {
//     bool atLeastOneError = false;
//     for (uint8_t i = 0; i < _totalPortsWithSensors; i++)
//         for (uint8_t j = 0; j < _numberOfSensorsOnEachPort[i]; j++)
//         {
//             char messageToSend[200];
//             formatToJSON(i, j, messageToSend, 200);
//             bool result = _clientMQTT.send(messageToSend, _addressesToSendMeasurementsTo[i]);
//             if (!result)
//                 atLeastOneError = true;
//             // printf("s%d -> id[%d] temp is: %s, sending to: %s, status: %s\n", i, j, messageToSend, _addressesToSendMeasurementsTo[i], result ? "sent" : "failed");
//         }

//     return !atLeastOneError;
// }
uint16_t SensorManager::makeJSON(char *message, uint16_t len, uint8_t sensorID)
{
    size_t capacity = JSON_ARRAY_SIZE(_numberOfSensorsOnEachPort[sensorID] + 1) + JSON_OBJECT_SIZE(2);
    DynamicJsonDocument doc(capacity);
    doc["name"] = _sensorsNamesArray[sensorID];
    JsonArray data = doc.createNestedArray("data");
    for (int i = 0; i < _numberOfSensorsOnEachPort[sensorID]; i++)
        data[i] = _measurementsArray2D[sensorID][i];

    serializeJson(doc, message, len);

    return strlen(message);
}
bool SensorManager::sendSensorsData2D()
{
    bool atLeastOneError = false;
    for (uint8_t i = 0; i < _totalPortsWithSensors; i++)
    {
        char messageToSend[SM_nm::MAX_MESSAGE_SIZE];
        uint16_t len = makeJSON(messageToSend, SM_nm::MAX_MESSAGE_SIZE, i);
        bool result = _clientMQTT.send(messageToSend, _addressesToSendMeasurementsTo[i]);
        if (!result)
            atLeastOneError = true;
    }
    return !atLeastOneError;
}
