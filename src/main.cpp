#include <LibPrintf.h>
#include <main_support.h>

// sensor_manager::EthArduino ethernetModule;

// sensor_manager::MQTTArduino mqttClientModule;
// const char *topics[] = {"/UZV1/temp1", "/UZV2/temp1"};

// sensor_manager::DallasArduino dallasModule1;

// sensor_manager::SensorManager sensorsManager(mqttClientModule, dallasModule1);
EthArduino ethernetModule;

MQTTArduino mqttClientModule;
const char *topics[] = {"/UZV1/temp1", "/UZV2/temp1", "/UZV2/temp2"};

DallasArduino dallasModule1(32);
DallasArduino dallasModule2(22);
IDallas *d_array[2] = {&dallasModule1, &dallasModule2};

SensorManager sensorsManager(mqttClientModule, dallasModule1);

void setup()
{
    initSystemParameters();
    printf("\nStarting setup...\n");

    initNetworkCard(ethernetModule);
    initConnectionToMQTTBroker(mqttClientModule);
    InitDallasSensors(dallasModule1);
    InitDallasSensors(dallasModule2);
    sensorsManager.initSenorsInArray(d_array, 2);

    sensorsManager.scanConnectedTemperatureSensors();
    sensorsManager.fillTopicsStrings(topics, 3);

    printf("Setup complete.\n\n");
}

uint64_t millisPassedSinceLastParse = 0;
const uint16_t scanInterval = 10000;

void loop()
{
    sensorsManager.receiveManagingData();

    if (!isItTimeToParse(millisPassedSinceLastParse, scanInterval))
        return;

    //    sensorsManager.processDataWithInterval();
    sensorsManager.refreshSensorsData2D();
    sensorsManager.sendSensorsData2D();

    millisPassedSinceLastParse = millis();
}
