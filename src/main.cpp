#include <LibPrintf.h>
#include <main_support.h>

EthArduino ethernetModule;

MQTTArduino mqttClientModule;
const char *addressesToSendTo[] = {"/UZV1/temp1", "/UZV2/temp1", "/UZV2/temp2"};

DallasArduino dallasModule1(32);
DallasArduino dallasModule2(22);
ISensor *d_array[2] = {&dallasModule1, &dallasModule2};

SensorManager sensorsManager(mqttClientModule);

void setup()
{
    initSystemParameters();
    printf("\nStarting setup...\n");

    initNetworkCard(ethernetModule);
    initConnectionToMQTTBroker(mqttClientModule);
    InitDallasSensors(dallasModule1);
    InitDallasSensors(dallasModule2);
    sensorsManager.initSenorsOnAllPINs(d_array, 2);

    sensorsManager.setAddressesToSendMeasurementsTo(addressesToSendTo, 3);

    printf("Setup complete.\n\n");
}

uint64_t millisPassedSinceLastParse = 0;
const uint16_t scanInterval = 10000;

void loop()
{
    if (!isItTimeToParse(millisPassedSinceLastParse, scanInterval))
        return;

    sensorsManager.refreshSensorsData2D();
    sensorsManager.sendSensorsData2D();

    millisPassedSinceLastParse = millis();
}
