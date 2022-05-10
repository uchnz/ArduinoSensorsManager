#pragma once

#include <stdint.h>

#include <IEth.h>
#include <IMQTT.h>
#include <ISensor.h>

namespace SM_nm
{
    const uint8_t MAX_MESSAGE_SIZE = 100;
}

class SensorManager
{
private:
    IMQTT &_clientMQTT;

    char **_addressesToSendMeasurementsTo;
    // uint8_t _totalAddresses;

    uint8_t _totalPortsWithSensors;
    ISensor **_ISenosorObjectsManagingArray2D;
    // uint8_t *_numberOfSensorsOnEachPort;
    // double **_measurementsArray2D;
    // char **_sensorsNamesArray;

    // void initArrays(ISensor **sensorsArray2D, uint8_t totalOccupiedPINs);
    void initArrays2(ISensor **sensorsArray2D, uint8_t totalOccupiedPINs);
    // void fillArraysWithInitialValues();
    // bool isSensorArrayAllNulls(ISensor **sensorsArray2D, uint8_t totalOccupiedPINs);

    // bool areArraysAllNulls(ISensor **sensorsArray2D, const char **addressesArray, uint8_t totalOccupiedPINs);
    bool arraysHaveNulls(ISensor **sensorsArray2D, const char **addressesArray, uint8_t totalOccupiedPINs);
    // uint16_t makeJSON(char *message, uint16_t len, uint8_t sensorID);
    uint16_t makeJSON2(char *message, uint16_t len, uint8_t sensorID);
    uint16_t makeSystemJSON(char *message, uint16_t len, uint32_t millisSinceStart);
    bool saveAddresses(const char **addressesArray);

public:
    SensorManager(IMQTT &mqtt);
    virtual ~SensorManager();

    // bool setAddressesToSendMeasurementsTo(const char **addressesArray, uint8_t totalAddresses);
    // void getAddressToSendToByID(uint8_t id, char *address);

    // uint8_t getTotalNumberOfOccupiedPINs();
    // bool init(ISensor **sensorsArray2D, uint8_t totalOccupiedPINs);
    bool init2(ISensor **sensorsArray2D, const char **addressesArray, uint8_t totalOccupiedPINs);

    // double getCurrentMeasurementOfOneSenorByID(uint8_t row, uint8_t col = 0);

    // uint8_t getNumberOfSensorsOnPINByID(uint8_t id);

    // bool refreshSensorsData2D();
    // bool sendSensorsData2D();
    bool refreshSensors();
    bool sendSensorsData();

    bool sendSystemInfo(uint32_t millisSinceStart);
};
