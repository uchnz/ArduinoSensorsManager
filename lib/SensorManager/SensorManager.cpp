#ifndef __TESTS__
#include <LibPrintf.h>
#endif

#include <stdio.h>
#include <string.h>

#include <SensorManager.h>

// SensorManager::SensorManager(IMQTT &mqtt, IDallas &dallas)
// : _clientMQTT(mqtt), _dallas(dallas)
SensorManager::SensorManager(IMQTT &mqtt)
    : _clientMQTT(mqtt)
{
    _addressesToSendMeasurementsTo = nullptr;
    _totalAddresses = 0;
    // _temperatures = nullptr;
    // _numberOfSensors = 0;
    _totalNumberOfOccupiedPINsByISensorObjects = 0;
    _ISenosorObjectsManagingArray2D = nullptr;
    _measurementsArray2D = nullptr;
    _numberOfSensorsOnPINsArray = nullptr;
}

SensorManager::~SensorManager()
{
    if (nullptr != _addressesToSendMeasurementsTo)
    {
        for (uint8_t i = 0; i < _totalAddresses; i++)
            delete[] _addressesToSendMeasurementsTo[i];

        delete[] _addressesToSendMeasurementsTo;
    }

    // if (nullptr != _temperatures)
    //     delete[] _temperatures;

    if (nullptr != _measurementsArray2D)
    {
        for (uint8_t i = 0; i < _totalNumberOfOccupiedPINsByISensorObjects; i++)
            delete[] _measurementsArray2D[i];

        delete[] _measurementsArray2D;
    }

    if (nullptr != _numberOfSensorsOnPINsArray)
        delete[] _numberOfSensorsOnPINsArray;
}

// Private
void SensorManager::initArrays(IDallas **sensorsArray2D, uint8_t totalOccupiedPINs)
{
    _ISenosorObjectsManagingArray2D = sensorsArray2D;

    _totalNumberOfOccupiedPINsByISensorObjects = totalOccupiedPINs;
    _measurementsArray2D = new float *[totalOccupiedPINs];
    _numberOfSensorsOnPINsArray = new uint8_t[totalOccupiedPINs];
}

void SensorManager::fillArraysWithInitialValues()
{
    for (uint8_t i = 0; i < _totalNumberOfOccupiedPINsByISensorObjects; i++)
    {
        uint8_t num = 0;
        if (nullptr != _ISenosorObjectsManagingArray2D[i])
            num = _ISenosorObjectsManagingArray2D[i]->getNumberOfConnectedSensors();
        _numberOfSensorsOnPINsArray[i] = num;
        _measurementsArray2D[i] = new float[num];
        for (uint8_t j = 0; j < num; j++)
            _measurementsArray2D[i][j] = -128.0;
    }
}

bool SensorManager::isSensorArrayAllNulls(IDallas **sensorsArray2D, uint8_t totalOccupiedPINs)
{
    if (nullptr == sensorsArray2D)
        return true;

    bool allNull = true;
    for (uint8_t i = 0; i < totalOccupiedPINs; i++)
        if (nullptr != sensorsArray2D[i])
            allNull = false;

    return allNull;
}

// Private END

// void SensorManager::updateAllTemperatures()
// {
//     for (uint8_t i = 0; i < _numberOfSensors; i++)
//         _temperatures[i] = _dallas.getTemperatureByID(i);
// }

// Public
// MQTT
// bool SensorManager::sendSensorsData(const char *dataToSend, const char *addressToSendTo)
// {
//     if ((nullptr != dataToSend) && (nullptr != addressToSendTo))
//         return _clientMQTT.send(dataToSend, addressToSendTo);

//     return false;
// }

// bool SensorManager::receiveManagingData()
// {
//     return _clientMQTT.receive();
// }

// Dallas
// void SensorManager::scanConnectedTemperatureSensors()
// {
//     _numberOfSensors = _dallas.getNumberOfConnectedSensors();
//     if (0 == _numberOfSensors)
//         return;

//     _temperatures = new float[_numberOfSensors];
//     for (uint8_t i = 0; i < _numberOfSensors; i++)
//         _temperatures[i] = -128;
// }

// uint8_t SensorManager::getSavedNumberSensors()
// {
//     return _numberOfSensors;
// }

// float SensorManager::getCurrentTemperatureByID(uint8_t id)
// {
//     if (id < _numberOfSensors)
//         return _temperatures[id];

//     return -128;
// }

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

bool SensorManager::setAddressesToSendMeasurementsTo(const char **addressesArray, uint8_t totalAddresses)
{
    if (nullptr == addressesArray)
        return false;
    if (totalAddresses <= 0)
        return false;

    _addressesToSendMeasurementsTo = new char *[totalAddresses];
    for (uint8_t i = 0; i < totalAddresses; i++)
    {
        uint8_t sizeOfAddressWitEndOfString = strlen(addressesArray[i]) + 1;
        _addressesToSendMeasurementsTo[i] = new char[sizeOfAddressWitEndOfString];
        memcpy(_addressesToSendMeasurementsTo[i], addressesArray[i], sizeOfAddressWitEndOfString);
    }
    _totalAddresses = totalAddresses;

    return true;
}

void SensorManager::getAddressToSendToByID(uint8_t id, char *address)
{
    if (id >= _totalAddresses)
        return;

    uint8_t sizeOfTopicWitEndOfString = strlen(_addressesToSendMeasurementsTo[id]) + 1;
    memcpy(address, _addressesToSendMeasurementsTo[id], sizeOfTopicWitEndOfString);
    return;
}

// bool SensorManager::processDataWithInterval()
// {
//     if (!refreshSensorsData())
//         return false;

//     if (!sendSensorsData())
//         return false;

//     return true;
// }

// bool SensorManager::refreshSensorsData()
// {
//     if (0 == _numberOfSensors)
//         return false;

//     _dallas.requestCurrentTemperatures();
//     updateAllTemperatures();

//     return true;
// }

// bool SensorManager::sendSensorsData()
// {
//     if (0 == _numberOfSensors)
//         return false;

//     for (uint8_t i = 0; i < _numberOfSensors; i++)
//     {
//         char tempConverted[10];
//         sprintf(tempConverted, "%.2f", (double)_temperatures[i]);
//         _clientMQTT.send(tempConverted, _addressesToSendMeasurementsTo[i]);
//     }

//     return true;
// }

// bool SensorManager::sendSensorDataByID(uint8_t id)
// {
//     return false;
// }

uint8_t SensorManager::getTotalNumberOfOccupiedPINs()
{

    return _totalNumberOfOccupiedPINsByISensorObjects;
}

bool SensorManager::initSenorsOnAllPINs(IDallas **sensorsArray2D, uint8_t totalOccupiedPINs)
{
    if (isSensorArrayAllNulls(sensorsArray2D, totalOccupiedPINs))
        return false;

    initArrays(sensorsArray2D, totalOccupiedPINs);
    fillArraysWithInitialValues();

    return true;
}

float SensorManager::getCurrentMeasurementOfOneSenorByID(uint8_t row, uint8_t col)
{
    if (nullptr == _numberOfSensorsOnPINsArray)
        return -128;
    if (0 == _numberOfSensorsOnPINsArray[row])
        return -128;
    if (_numberOfSensorsOnPINsArray[row] < col)
        return -128;

    return _measurementsArray2D[row][col];
}

uint8_t SensorManager::getNumberOfSensorsOnPINByID(uint8_t id)
{
    if (nullptr == _measurementsArray2D)
        return 0;

    if (nullptr == _measurementsArray2D[id])
        return 0;

    return _numberOfSensorsOnPINsArray[id];
}

//--------------------------
//--------------------------
// MORE TESTS TO CHECK BELOW
//--------------------------
//--------------------------

bool SensorManager::refreshSensorsData2D()
{
    if ((nullptr == _measurementsArray2D))
        return false;

    for (uint8_t i = 0; i < _totalNumberOfOccupiedPINsByISensorObjects; i++)
    {
        _ISenosorObjectsManagingArray2D[i]->requestCurrentTemperatures();
        for (uint8_t j = 0; j < _numberOfSensorsOnPINsArray[i]; j++)
            _measurementsArray2D[i][j] = _ISenosorObjectsManagingArray2D[i]->getTemperatureByID(j);
    }
    return true;
}

bool SensorManager::sendSensorsData2D()
{

    for (uint8_t i = 0; i < _totalNumberOfOccupiedPINsByISensorObjects; i++)
        for (uint8_t j = 0; j < _numberOfSensorsOnPINsArray[i]; j++)
        {
            char tempConverted[10];
            sprintf(tempConverted, "%.2f", (double)_measurementsArray2D[i][j]);
            _clientMQTT.send(tempConverted, _addressesToSendMeasurementsTo[i]);
            printf("s%d -> id[%d] temp is: %s, sent to: %s\n", i, j, tempConverted, _addressesToSendMeasurementsTo[i]);
        }

    return true;
}
