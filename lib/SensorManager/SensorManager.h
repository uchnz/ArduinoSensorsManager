#pragma once

#include <stdint.h>

#include <IEth.h>
#include <IMQTT.h>
#include <ISensor.h>

class SensorManager
{
private:
    IMQTT &_clientMQTT;

    char **_addressesToSendMeasurementsTo;
    uint8_t _totalAddresses;

    uint8_t _totalNumberOfOccupiedPINsByISensorObjects;
    ISensor **_ISenosorObjectsManagingArray2D;
    uint8_t *_numberOfSensorsOnPINsArray;
    float **_measurementsArray2D;

    void initArrays(ISensor **sensorsArray2D, uint8_t totalOccupiedPINs);
    void fillArraysWithInitialValues();
    bool isSensorArrayAllNulls(ISensor **sensorsArray2D, uint8_t totalOccupiedPINs);

public:
    SensorManager(IMQTT &mqtt);
    virtual ~SensorManager();

    bool setAddressesToSendMeasurementsTo(const char **addressesArray, uint8_t totalAddresses);
    void getAddressToSendToByID(uint8_t id, char *address);

    uint8_t getTotalNumberOfOccupiedPINs();
    bool initSenorsOnAllPINs(ISensor **sensorsArray2D, uint8_t totalOccupiedPINs);

    float getCurrentMeasurementOfOneSenorByID(uint8_t row, uint8_t col = 0);

    uint8_t getNumberOfSensorsOnPINByID(uint8_t id);

    bool refreshSensorsData2D();
    bool sendSensorsData2D();
};
