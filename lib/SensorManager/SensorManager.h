#pragma once

#include <stdint.h>

#include <IEth.h>
#include <IMQTT.h>
#include <IDallas.h>

class SensorManager
{
private:
    IMQTT &_clientMQTT;
    // IDallas &_dallas;

    char **_addressesToSendMeasurementsTo;
    uint8_t _totalAddresses;
    // float *_temperatures;
    // uint8_t _numberOfSensors;

    uint8_t _totalNumberOfOccupiedPINsByISensorObjects;
    IDallas **_ISenosorObjectsManagingArray2D;
    uint8_t *_numberOfSensorsOnPINsArray;
    float **_measurementsArray2D;

    // Dallas private
    void initArrays(IDallas **sensorsArray2D, uint8_t totalOccupiedPINs);
    void fillArraysWithInitialValues();
    bool isSensorArrayAllNulls(IDallas **sensorsArray2D, uint8_t totalOccupiedPINs);

    // void updateAllTemperatures();

public:
    //    SensorManager(IMQTT &mqtt, IDallas &dallas);
    SensorManager(IMQTT &mqtt);
    virtual ~SensorManager();

    bool setAddressesToSendMeasurementsTo(const char **addressesArray, uint8_t totalAddresses);
    void getAddressToSendToByID(uint8_t id, char *address);

    uint8_t getTotalNumberOfOccupiedPINs();
    bool initSenorsOnAllPINs(IDallas **sensorsArray2D, uint8_t totalOccupiedPINs);

    float getCurrentMeasurementOfOneSenorByID(uint8_t row, uint8_t col = 0);

    uint8_t getNumberOfSensorsOnPINByID(uint8_t id);

    bool refreshSensorsData2D();
    bool sendSensorsData2D();

    // MQTT
    // bool sendSensorsData(const char *dataToSend, const char *addressToSendTo);
    // bool receiveManagingData();

    // Dallas
    // void scanConnectedTemperatureSensors();
    // uint8_t getSavedNumberSensors();
    // bool refreshSensorsData();
    // float getCurrentTemperatureByID(uint8_t id);
    //        void getStringAddressByID(uint8_t id, char *address);

    // Business logic
    // std::string printTemperatureDebugInfo(uint8_t id, float temperature);

    // bool processDataWithInterval();
    // bool sendSensorsData();
    // bool sendSensorDataByID(uint8_t id);
};
