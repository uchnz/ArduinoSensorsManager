#include <LibPrintf.h>
#include <main_support.h>

// sensor_manager::EthArduino ethernetModule;

// sensor_manager::MQTTArduino mqttClientModule;
// const char *topics[] = {"/UZV1/temp1", "/UZV2/temp1"};

// sensor_manager::DallasArduino dallasModule;

// sensor_manager::SensorManager sensorsManager(mqttClientModule, dallasModule);
EthArduino ethernetModule;

MQTTArduino mqttClientModule;
const char *topics[] = {"/UZV1/temp1", "/UZV2/temp1"};

DallasArduino dallasModule;

SensorManager sensorsManager(mqttClientModule, dallasModule);

void setup()
{
    initSystemParameters();
    printf("\nStarting setup...\n");

    initNetworkCard(ethernetModule);
    initConnectionToMQTTBroker(mqttClientModule);
    InitDallasSensors(dallasModule);

    sensorsManager.scanConnectedTemperatureSensors();
    sensorsManager.fillTopicsStrings(topics, 2);

    printf("Setup complete.\n\n");
}

uint64_t millisPassedSinceLastParse = 0;
const uint16_t scanInterval = 10000;

void loop()
{
    sensorsManager.receiveManagingData();

    if (!isItTimeToParse(millisPassedSinceLastParse, scanInterval))
        return;

    sensorsManager.processDataWithInterval();

    millisPassedSinceLastParse = millis();
}
