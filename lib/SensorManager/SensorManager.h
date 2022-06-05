#pragma once

#include <stdint.h>

#include <IEth.h>
#include <IMQTT.h>
#include <ISensor.h>
#include <IActuator.h>

namespace SM_nm
{
    const uint8_t MAX_MESSAGE_SIZE = 100;

    struct ActuatorCommand
    {
        char name[30];
        char command[10];
    };

}

class SensorManager
{
private:
    IMQTT &_clientMQTT;

    char **_addressesToSendMeasurementsTo;
    uint8_t _totalPortsWithSensors;
    ISensor **_ISenosorObjectsManagingArray2D;
    IActuator **_IActuatorObjects;
    uint8_t _totalActuators;
    char **_addressesToReadActuatorsCommands;

    // void initArrays2(ISensor **sensorsArray2D, uint8_t totalOccupiedPINs);
    // bool arraysHaveNulls(ISensor **sensorsArray2D, const char **addressesArray, uint8_t totalOccupiedPINs);
    bool arraysHaveNulls(void **sensorsArray2D, const char **addressesArray, uint8_t totalOccupiedPINs);
    uint16_t makeJSON2(char *message, uint16_t len, uint8_t sensorID);
    uint16_t makeSystemJSON(char *message, uint16_t len, uint32_t millisSinceStart);
    // bool saveAddresses(const char **addressesArray);
    bool saveAddresses2(const char **addressesArray, char **&destination, uint8_t size);
    bool parseCommand(const char *commandJSON, SM_nm::ActuatorCommand &ac);
    uint8_t getActuatorIDByName(const char *name);

public:
    SensorManager(IMQTT &mqtt);
    virtual ~SensorManager();

    bool init2(ISensor **sensorsArray2D, const char **addressesArray, uint8_t totalOccupiedPINs);
    bool refreshSensors();
    bool sendSensorsData();
    bool sendSystemInfo(uint32_t millisSinceStart);

    bool initActuators(IActuator **actuatorsArray, const char **addressesArray, uint8_t totalOccupiedPINs);
    bool executeCommand(const char *commandJSON);
};
