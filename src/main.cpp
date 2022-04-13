#include <LibPrintf.h>
#include <main_support.h>

EthArduino ethernetModule;

MQTTArduino mqttClientModule;
const uint8_t totalSensorPorts = 6;
const char *addressesToSendTo[totalSensorPorts] = {"/UZV1/temp1", "/UZV2/temp1and2", "/UZV1/mousure", "/UZV1/mq7co", "/UZV1/rd", "/UZV1/floatSensor"};

DallasArduino dallasModule1(32);
DallasArduino dallasModule2(22);
MoisureRArduino moisureR1(0);
MQ7COArduino mq7co(2, 7);
RaindropsArduino rd(4);
FloatArduino floatSensor(38);

ISensor *d_array[totalSensorPorts] = {&dallasModule1, &dallasModule2, &moisureR1, &mq7co, &rd, &floatSensor};

SensorManager sensorsManager(mqttClientModule);

void setup()
{
    initSystemParameters();
    printf("\nStarting setup...\n");

    initNetworkCard(ethernetModule);
    initConnectionToMQTTBroker(mqttClientModule);
    InitDallasSensor(dallasModule1);
    InitDallasSensor(dallasModule2);
    moisureR1.init();
    InitMQ7COSensor(mq7co);
    rd.init();
    sensorsManager.initSenorsOnAllPINs(d_array, totalSensorPorts);
    sensorsManager.setAddressesToSendMeasurementsTo(addressesToSendTo, totalSensorPorts);

    printf("Setup complete.\n\n");
}

uint64_t millisPassedSinceLastParse = 0;
const uint16_t scanInterval = 3000;
uint64_t i = 0;
void loop()
{
    mq7co.requestCurrentMeasurement();
    if (!isItTimeToParse(millisPassedSinceLastParse, scanInterval))
        return;

    printf("mq7: %2f\n", mq7co.getCurrentMeasurementByID());

    // sensorsManager.refreshSensorsData2D();
    // sensorsManager.sendSensorsData2D();
    millisPassedSinceLastParse = millis();
}
