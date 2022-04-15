#include <LibPrintf.h>
#include <main_support.h>

EthArduino ethernetModule;

MQTTArduino mqttClientModule;
const uint8_t totalSensorPorts = 6;
const char *addressesToSendTo[totalSensorPorts] = {"/UZV1/temp1", "/UZV2/temp1and2", "/UZV1/mousure", "/UZV1/mq7co", "/UZV1/rd", "/UZV1/floatSensor"};

DallasArduino dallasModule1(32);
DallasArduino dallasModule2(22);
MQ7COArduino mq7co(2, 7);
SASArduino moisureR1(0);
SASArduino moisureC1(5);
SASArduino raindrop(4);
SASArduino co2(3);
SASArduino uv(6);
FloatArduino floatSensor(38);

// ISensor *d_array[totalSensorPorts] = {&dallasModule1, &dallasModule2, &moisureR1, &mq7co, &raindrop, &floatSensor};

SensorManager sensorsManager(mqttClientModule);

void setup()
{
    initSystemParameters();
    printf("\nStarting setup...\n");

    initNetworkCard(ethernetModule);
    initConnectionToMQTTBroker(mqttClientModule);
    InitDallasSensor(dallasModule1);
    InitDallasSensor(dallasModule2);
    InitMQ7COSensor(mq7co);
    InitSASSensor(moisureR1);
    InitSASSensor(moisureC1);
    InitSASSensor(raindrop);
    InitSASSensor(co2);
    InitSASSensor(uv);
    // sensorsManager.initSenorsOnAllPINs(d_array, totalSensorPorts);
    sensorsManager.setAddressesToSendMeasurementsTo(addressesToSendTo, totalSensorPorts);

    printf("Setup complete.\n\n");
}

uint64_t millisPassedSinceLastParse = 0;
const uint16_t scanInterval = 2000;
uint64_t i = 0;
void loop()
{
    mq7co.requestCurrentMeasurement();
    moisureR1.requestCurrentMeasurement();
    moisureC1.requestCurrentMeasurement();
    raindrop.requestCurrentMeasurement();
    co2.requestCurrentMeasurement();
    uv.requestCurrentMeasurement();
    if (!isItTimeToParse(millisPassedSinceLastParse, scanInterval))
        return;

    // printf("mq7: %2f\n", mq7co.getCurrentMeasurementByID());
    printf("mouisureR: %2f\n", moisureR1.getCurrentMeasurementByID());
    printf("mouisureC: %2f\n", moisureC1.getCurrentMeasurementByID());
    printf("raindrop: %2f\n", raindrop.getCurrentMeasurementByID());
    printf("co2: %2f\n", co2.getCurrentMeasurementByID());
    // printf("uv: %2f\n", uv.getCurrentMeasurementByID());

    // sensorsManager.refreshSensorsData2D();
    // sensorsManager.sendSensorsData2D();
    millisPassedSinceLastParse = millis();
}
