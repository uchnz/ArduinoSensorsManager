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
    mq7co.init();
    rd.init();
    sensorsManager.initSenorsOnAllPINs(d_array, totalSensorPorts);
    sensorsManager.setAddressesToSendMeasurementsTo(addressesToSendTo, totalSensorPorts);

    printf("Setup complete.\n\n");
}

uint64_t millisPassedSinceLastParse = 0;
const uint16_t scanInterval = 3000;
void loop()
{
    printf("Heating Started on: %ld", millis());
    analogWrite(7, 255);
    for (uint8_t i = 0; i < 60; i++)
    {
        printf(".");
        delay(1000);
    }
    printf("\nHeating ended on: %ld\n", millis());

    printf("Cooling Started on: %ld", millis());
    analogWrite(7, 72);
    for (uint8_t i = 0; i < 45; i++)
    {
        printf(".");
        delay(1000);
    }
    printf("\nCooling in the middle...: %ld\n", millis());

    printf("Start reading sensor data: ");
    uint16_t num = mq7co.getCurrentMeasurementByID();
    for (uint8_t i = 0; i < 9; i++)
    {
        uint16_t nextVal = mq7co.getCurrentMeasurementByID();
        printf("(%d)", nextVal);
        num += nextVal;
        delay(4500);
    }
    printf("\nStop reading sensor data\n");
    printf("Sum of 9 measurements = %d\n", num);
    printf("Current average mq7 = %d\n", num / 10);
    printf("Colling ended on: %ld\n\n", millis());

    // num = rd.getCurrentMeasurementByID();
    // printf("current rd = %.0f\n", num);
    // num = floatSensor.getCurrentMeasurementByID();
    // printf("current float = %.0f\n", num);
    // delay(1000);

    // if (!isItTimeToParse(millisPassedSinceLastParse, scanInterval))
    //     return;

    // sensorsManager.refreshSensorsData2D();
    // sensorsManager.sendSensorsData2D();

    // millisPassedSinceLastParse = millis();
}
